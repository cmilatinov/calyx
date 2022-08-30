import ctypes
from argparse import ArgumentParser
from clang.cindex import AccessSpecifier, StorageClass
from clang.cindex import *
from pathlib import Path
from pprintpp import pprint

import os.path
import sys
import re


def traverse(node, func):
    func(node)
    for child in node.get_children():
        try:
            traverse(child, func)
        except:
            pass


def full_name(node):
    if node is None:
        return ''
    elif node.kind == CursorKind.TRANSLATION_UNIT:
        return ''
    else:
        res = full_name(node.semantic_parent)
        if res != '':
            return res + '::' + node.spelling
    return node.spelling


def find_type(node, kind, arr=None):
    if arr is None:
        arr = []

    def traverse_fn(n):
        if isinstance(kind, list):
            if n.kind in kind:
                arr.append(n)
        else:
            if n.kind == kind:
                arr.append(n)

    traverse(node, traverse_fn)


def find_params(node):
    params = []
    find_type(node, CursorKind.PARM_DECL, params)
    return params


def find_all(node, macro_names):
    macros = []
    classes = []
    enums = []
    prev = node

    def traverse_fn(n):
        nonlocal prev

        if n.kind == CursorKind.MACRO_INSTANTIATION and n.spelling in macro_names:
            macros.append(parse_macro(n))
        elif n.kind == CursorKind.CLASS_DECL or n.kind == CursorKind.STRUCT_DECL:
            annotations = list(
                filter(lambda m: m['name'] == 'CX_BEGIN_REFLECT' and
                                 is_location_between(m['node'].location, n.extent.start, n.extent.end), macros)
            )
            for a in annotations:
                macros.remove(a)
            if len(annotations) > 0:
                classes.append(n)
        elif n.kind == CursorKind.ENUM_DECL:
            annotations = list(filter(lambda m: m['type'] == 'macro' and
                                                m['name'] == 'CX_REFLECT_ENUM' and
                                                is_location_between(m['node'].location, prev.location, n.location),
                                      macros))
            if len(annotations) > 0:
                enums.append(n)

        prev = n

    traverse(node, traverse_fn)
    return macros, classes, enums


def is_location_after(loc, start):
    if str(loc.file) != str(start.file):
        return False

    if loc.line > start.line:
        return True
    elif loc.line == start.line and loc.column >= start.column:
        return True

    return False


def is_location_before(loc, end):
    return is_location_after(end, loc)


def is_location_between(loc, start, end):
    return is_location_after(loc, start) and is_location_before(loc, end)


def parse_macro(macro):
    original_macro = substr_file(str(macro.location.file), macro.extent.start, macro.extent.end)

    arguments = []
    regex = re.escape(macro.spelling) + r"\((.+)\)"
    match = re.match(regex, original_macro)
    if match:
        trimmed_arglist = match.groups()[0].strip()
        if trimmed_arglist != '':
            arguments = match.groups()[0].strip().split(",")
            arguments = list(map(lambda arg: arg.strip(), arguments))

    return {
        'type': 'macro',
        'node': macro,
        'name': macro.spelling,
        'arguments': arguments
    }


def parse_enum(node):
    enum_values = {}

    def parse_enum_const(n):
        if n.kind == CursorKind.ENUM_CONSTANT_DECL:
            enum_values[n.spelling] = n.enum_value

    traverse(node, parse_enum_const)

    return {
        'type': 'enum',
        'node': node,
        'name': node.spelling,
        'full_name': full_name(node),
        'values': enum_values,
        'var_type': node.enum_type.spelling
    }


def parse_class(node, macros):
    if node.kind != CursorKind.CLASS_DECL and node.kind != CursorKind.STRUCT_DECL:
        return None

    fields = []
    methods = []
    base = None
    prev = node

    def parse_child_node(n):
        nonlocal prev, base

        if (n.kind == CursorKind.FIELD_DECL or n.kind == CursorKind.VAR_DECL) and n.semantic_parent == node:
            annotations = list(filter(lambda m: is_location_between(m['node'].location, prev.location, n.location),
                                      macros))
            for a in annotations:
                macros.remove(a)

            fields.append({
                'type': 'field',
                'var_type': n.type.spelling,
                'name': n.spelling,
                'full_name': full_name(n),
                'annotations': annotations,
                'node': n,
                'access': n.access_specifier,
                'storage_class': n.storage_class
            })

        elif n.kind == CursorKind.CXX_METHOD and n.semantic_parent == node:
            annotations = list(filter(lambda m: is_location_between(m['node'].location, prev.location, n.location),
                                      macros))
            for a in annotations:
                macros.remove(a)

            params = find_params(n)
            params = list(map(lambda pd: {
                'type': 'parameter',
                'node': pd,
                'name': pd.spelling,
                'var_type': pd.type.spelling
            }, params))

            methods.append({
                'type': 'method',
                'name': n.spelling,
                'full_name': full_name(n),
                'annotations': annotations,
                'node': n,
                'access': n.access_specifier,
                'storage_class': n.storage_class,
                'return_type': n.result_type.spelling,
                'parameters': params
            })

        elif n.kind == CursorKind.CXX_BASE_SPECIFIER:

            base = n.referenced

        prev = n

    traverse(node, parse_child_node)

    return {
        'type': 'class' if node.kind == CursorKind.CLASS_DECL else
        'struct' if node.kind == CursorKind.STRUCT_DECL else
        'unknown',
        'name': node.spelling,
        'full_name': full_name(node),
        'fields': fields,
        'methods': methods,
        'base_class': full_name(base) if base else None,
        'node': node,
    }


def substr_file(file, start, end):
    with open(file) as f:
        lines = f.readlines()

    result = ""
    line_index = start.line
    while line_index < end.line:
        result += lines[line_index - 1] + "\n"
        line_index += 1

    if line_index == start.line:
        result += lines[line_index - 1][start.column - 1:end.column - 1]
    else:
        result += lines[line_index - 1][0:end.column]

    return result


def get_access_specifier(obj):
    if obj['node'].access_specifier == AccessSpecifier.PRIVATE:
        return 'AccessSpecifier::PRIVATE'
    elif obj['node'].access_specifier == AccessSpecifier.PROTECTED:
        return 'AccessSpecifier::PROTECTED'
    elif obj['node'].access_specifier == AccessSpecifier.PUBLIC:
        return 'AccessSpecifier::PUBLIC'
    return ''


def get_storage_type(obj):
    if obj['node'].storage_class == StorageClass.STATIC:
        return 'StorageType::STATIC'
    return 'StorageType::INSTANCE'


def get_method_type(m, clazz):
    #
    replacement = \
        F"(*)\\1" if m['storage_class'] == StorageClass.STATIC else \
        F"({clazz['full_name']}::*)\\1"
    return re.sub(r'(\(.*?\))', replacement, m['node'].type.spelling)


def serialize_enum(enum, header, source):
    typename = F"Enum<{enum['full_name']}>"
    header_template = F"""
    template<>
    class {typename} : public IEnum {{
    
    private:
        static std::unordered_map<std::string, int> s_enumValues;
    
    public:
        static std::unique_ptr<{typename}> s_instance;
    
        std::string GetName() const override {{ return "{enum['name']}"; }}
        std::string GetFullName() const override {{ return "{enum['full_name']}"; }}
        int GetTypeID() const override {{ return reflect::GetTypeId<{enum['full_name']}>(); }}
        
        std::unordered_map<std::string, int> GetEnumValues() const override {{ return s_enumValues; }}
        
        std::vector<std::string> GetStringValues() const override;
        std::vector<int> GetIntValues() const override;

        bool Translate(const std::string& value, int& out) const override;
        bool Translate(int value, std::string& out) const override;
        
    }};
    """

    comma1 = ',\n        '
    comma2 = ',\n            '
    source_template = F"""
    std::unique_ptr<{typename}> {typename}::s_instance = std::make_unique<{typename}>();
    
    std::unordered_map<std::string, int> {typename}::s_enumValues = {{ 
        {comma1.join(list(map(lambda i: F'{{ "{i[0]}", {i[1]} }}', enum['values'].items())))} 
    }};
    
    std::vector<std::string> {typename}::GetStringValues() const {{
        return {{ 
            {comma2.join(list(map(lambda k: F'"{k}"', enum['values'].keys())))}
        }};
    }}
    
    std::vector<int> {typename}::GetIntValues() const {{
        return {{ 
            {comma2.join(list(map(lambda v: F'{str(v)}', enum['values'].values())))} 
        }};
    }}

    bool {typename}::Translate(const std::string& value, int& out) const {{
        for (auto& it : s_enumValues) {{
            if (it.first == value) {{
                out = it.second;
                return true;
            }}
        }}
        return false;
    }}
    
    bool {typename}::Translate(int value, std::string& out) const {{
       for (auto& it : s_enumValues) {{
            if (it.second == value) {{
                out = it.first;
                return true;
            }}
        }}
        return false;
    }}
    """

    header.write(header_template)
    source.write(source_template)


def serialize_class(clazz, header, source):
    def method_header(m):
        typename = F"Method<{get_method_type(m, clazz)}, &{clazz['full_name']}::{m['name']}>"
        return F"""
    template<>
    class {typename} : public IMethod {{
    
    public:
        static std::unique_ptr<{typename}> s_instance;
    
        std::string GetName() const override {{ return "{m['name']}"; }}
        std::string GetFullName() const override {{ return "{m['full_name']}"; }}
        int GetTypeID() const override {{ return reflect::GetTypeId<{get_method_type(m, clazz)}>(); }}
        
        ReturnType GetReturnType() const override {{ return {{ reflect::GetTypeId<{m['return_type']}>(), "{m['return_type']}" }}; }}
        std::vector<Parameter> GetParameterList() const override;
        
        AccessSpecifier GetAccessSpecifier() const override {{ return {get_access_specifier(m)}; }}
        StorageType GetStorageType() const override {{ return {get_storage_type(m)}; }}
        
        Object Invoke(const std::vector<Object>& args) const override;
        Object Invoke(const Reference& o, const std::vector<Object>& args) const override;
        
    }};"""

    newline = '\n'
    typename = F"Class<{clazz['full_name']}>"
    header_template = F"""
    template<>
    class {typename} : public IClass {{
    
    public:
        static std::unique_ptr<{typename}> s_instance;
    
        std::string GetName() const override {{ return "{clazz['name']}"; }}
        std::string GetFullName() const override {{ return "{clazz['full_name']}"; }}
        int GetTypeID() const override {{ return reflect::GetTypeId<{clazz['full_name']}>(); }}
        
        std::vector<Field> GetFields() const override;
        std::vector<IMethod*> GetMethods() const override;
        
    }};
    {newline.join(list(map(method_header, clazz['methods'])))}"""

    def map_annotation(a):
        return F"""{{ "{a['name']}", {{ {', '.join(list(map(lambda arg: F'"{arg}"', a['arguments'])))} }} }}"""

    def map_field(f):
        comma = ",\n"
        return F"""{{
                {
                    F"(size_t)&{clazz['full_name']}::{f['name']}"
                    if f['storage_class'] == StorageClass.STATIC
                    else F"offsetof({clazz['full_name']}, {f['name']})"
                },
                {get_access_specifier(f)}, 
                {get_storage_type(f)}, 
                reflect::GetTypeId<{f['var_type']}>(),
                "{f['var_type']}", 
                "{f['name']}",
                {{
                    {comma.join(list(map(map_annotation, f['annotations'])))}
                }}
            }},"""

    def map_method(m):
        return \
            F"Method<{get_method_type(m, clazz)}, &{clazz['full_name']}::{m['name']}>::s_instance.get(),"

    def map_method_source(m):

        def map_parameter(p):
            return \
                F"""            {{ reflect::GetTypeId<{p['var_type']}>(), "{p['var_type']}", "{p['name']}" }},"""

        typename = F"Method<{get_method_type(m, clazz)}, &{clazz['full_name']}::{m['name']}>"

        if m['storage_class'] == StorageClass.STATIC:
            invoke_instance = \
F"""Object {typename}::Invoke(const Reference& o, const std::vector<Object>& args) const {{
        throw Exception("Static method cannot be invoked on an instance!");
    }}"""
            invocation = F"""{clazz["full_name"]}::{m["name"]}({", ".join(list(map(lambda enum: F"args[{enum[0]}].GetT<std::decay_t<{enum[1]['var_type']}>>()", enumerate(m['parameters']))))})"""
            invoke_static = \
F"""Object {typename}::Invoke(const std::vector<Object>& args) const {{ 
        auto params = GetParameterList();
        if (params.size() != args.size())
            throw Exception("Argument count does not match!");
            
        for (int i = 0; i < params.size(); i++) {{
            if (params[i].typeID != args[i].GetTypeID())
                throw Exception("Arguments type mismatch!");
        }}
        
        {F'{invocation};' if m['return_type'] == 'void' else ''}
        {F'return Object({invocation});' if m['return_type'] != 'void' else 'return Object();'}
    }}"""
        else:
            invocation = F"""o.GetT<{clazz['full_name']}>().{m['name']}({', '.join(list(map(lambda enum: F"args[{enum[0]}].GetT<std::decay_t<{enum[1]['var_type']}>>()", enumerate(m['parameters']))))})"""
            invoke_instance = \
F"""Object {typename}::Invoke(const Reference& o, const std::vector<Object>& args) const {{
        auto params = GetParameterList();
        if (params.size() != args.size())
            throw Exception("Argument count does not match!");
            
        for (int i = 0; i < params.size(); i++) {{
            if (params[i].typeID != args[i].GetTypeID())
                throw Exception("Arguments type mismatch!");
        }}
        
        {F'{invocation};' if m['return_type'] == 'void' else ''}
        {F'return Object({invocation});' if m['return_type'] != 'void' else 'return Object();'}
    }}"""
            invoke_static = \
F"""Object {typename}::Invoke(const std::vector<Object>& args) const {{
        throw Exception("Instance method cannot be invoked statically!");
    }}"""

        return \
            F"""
    std::unique_ptr<{typename}> {typename}::s_instance = std::make_unique<{typename}>();
    
    std::vector<Parameter> {typename}::GetParameterList() const {{
        return {{
{newline.join(list(map(map_parameter, m['parameters'])))}          
        }};
    }}
    
    {invoke_static}
    
    {invoke_instance}
"""

    comma = '\n            '
    source_template = F"""
    std::unique_ptr<{typename}> {typename}::s_instance = std::make_unique<{typename}>();
    
    std::vector<Field> {typename}::GetFields() const {{
        return {{
            {comma.join(list(map(map_field, clazz['fields'])))}
        }};
    }}
    
    std::vector<IMethod*> {typename}::GetMethods() const {{
        return {{
            {comma.join(list(map(map_method, clazz['methods'])))}
        }};
    }}
    
    {''.join(list(map(map_method_source, clazz['methods'])))}
    """

    header.write(header_template)
    source.write(source_template)


def serialize(header_dir, source_dir, relative_path, filename, enums, classes):
    with open(F"{os.path.join(header_dir, filename)}.gen.h", 'w') as header:
        with open(F"{os.path.join(source_dir, filename)}.gen.cpp", 'w') as source:
            header_template = F"""#pragma once

/**
 *  ========================================================
 *         THIS FILE IS AUTO-GENERATED BY REFLECT.PY
 * 
 *     DO NOT MODIFY THIS FILE AS IT WILL BE RE-GENERATED
 *          PRIOR TO EACH BUILD IF YOU HAVE MADE ANY 
 *            MODIFICATIONS TO THE ORIGINAL HEADER.
 *  ========================================================
 */

#include "{os.path.join(relative_path, filename) if relative_path != '.' else filename}.h"

namespace reflect {{\n"""
            header.write(header_template)

            source_template = F"""
/**
 *  ========================================================
 *         THIS FILE IS AUTO-GENERATED BY REFLECT.PY
 * 
 *     DO NOT MODIFY THIS FILE AS IT WILL BE RE-GENERATED
 *          PRIOR TO EACH BUILD IF YOU HAVE MADE ANY 
 *            MODIFICATIONS TO THE ORIGINAL HEADER.
 *  ========================================================
 */          
            
#include "{filename}.gen.h"

namespace reflect {{\n"""
            source.write(source_template)

            for enum in enums:
                serialize_enum(enum, header, source)

            for clazz in classes:
                serialize_class(clazz, header, source)

            header.write('\n}')
            source.write('\n}')
            header.close()
            source.close()
            pass


def run():
    Config.set_library_file(os.getenv('LIBCLANG_PATH'))

    arg_parser = ArgumentParser(description="Generate reflection data for a header file.")
    arg_parser.add_argument('input', metavar='IN', type=str, help='An input C++ header file.')
    arg_parser.add_argument('-o', '--output', dest='output', metavar='OUT_DIR', type=str,
                            help='An output directory for the generated source files.')
    arg_parser.add_argument('-oh', '--output-header', dest='output_header', metavar='OUT_HEADER', type=str,
                            help='An output directory for the generated headers.')
    arg_parser.add_argument('-inc', '--include-root', dest='include_root', metavar='INCLUDE', type=str,
                            help='The root include directory of the project. '
                                 'Will be used for preprocessor include directives.')
    arg_parser.add_argument('-pch', '--precompiled-header', dest='pch', metavar='PCH', type=str,
                            help='A precompiled header file to use during parsing.')
    args, unknown = arg_parser.parse_known_args(sys.argv[1:])

    filename = args.input
    source_dir = args.output if args.output is not None else os.path.dirname(filename)
    header_dir = args.output_header if args.output_header is not None else source_dir
    include_dir = args.include_root if args.include_root is not None else header_dir
    pch_header = args.pch

    with open(filename, 'r') as input_file:
        temp_file = open('temp.h', 'w+')
        if pch_header is not None:
            pch_path = os.path.relpath(pch_header, include_dir)
            temp_file.write(F'#include "{pch_path}"\n')

        temp_file.write(input_file.read())
        temp_file.flush()
        temp_file.close()

        index = Index.create()
        tu = index.parse(temp_file.name, args=unknown, options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
        root = tu.cursor

        macros, classes, enums = find_all(root, ['CX_BEGIN_REFLECT', 'CX_REFLECT_ENUM', 'CX_PROPERTY'])

        classes = list(map(lambda c: parse_class(c, macros), classes))
        enums = list(map(parse_enum, enums))

        # pprint(enums)
        # pprint(classes)

        os.makedirs(header_dir, exist_ok=True)
        os.makedirs(source_dir, exist_ok=True)

        relative_path = os.path.relpath(os.path.dirname(filename), include_dir)

        serialize(header_dir, source_dir, relative_path, Path(filename).stem, enums, classes)

        # os.remove(temp_file.name)


if __name__ == '__main__':
    run()
