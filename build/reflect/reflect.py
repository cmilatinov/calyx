import re
from pprintpp import pprint
from clang.cindex import *
import os


os.add_dll_directory("C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\Llvm\\bin")


def traverse(node, func):
    func(node)
    for child in node.get_children():
        traverse(child, func)


def parent_namespace(node):
    if node is None:
        return ''
    elif node.kind == CursorKind.TRANSLATION_UNIT:
        return ''
    else:
        res = full_name(node.semantic_parent)
        if res != '':
            return res
    return '::'


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


def find_classes(node):
    classes = []
    find_type(node, [CursorKind.CLASS_DECL, CursorKind.STRUCT_DECL], classes)
    return classes


def find_macros(node, macro_names):
    macros = []
    find_type(node, CursorKind.MACRO_INSTANTIATION, macros)
    return list(filter(lambda m: m.spelling in macro_names, macros))


def is_location_after(loc, start):
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
    original_macro = substr_file('test.h', macro.extent.start, macro.extent.end)

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


def parse_class(node, node_list, macros):
    if node.kind != CursorKind.CLASS_DECL and node.kind != CursorKind.STRUCT_DECL:
        return None

    annotations = []
    try:
        prev_index = node_list.index(node) - 1
        if prev_index >= 0:
            prev = node_list[prev_index]
            annotations = list(filter(lambda m: is_location_between(m['node'].location, prev.location, node.location),
                                      macros))
            for a in annotations:
                macros.remove(a)
            pass
    except ValueError:
        pass

    fields = []
    methods = []
    base = None
    prev = node

    def parse_child_node(n):
        nonlocal prev, base

        if n.kind == CursorKind.FIELD_DECL and n.semantic_parent == node:
            annotations = list(filter(lambda m: is_location_between(m['node'].location, prev.location, n.location),
                                      macros))
            for a in annotations:
                macros.remove(a)

            fields.append({
                'type': 'field',
                'var_type': n.type.spelling,
                'name': n.spelling,
                'parent_namespace': parent_namespace(n),
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
                'parent_namespace': parent_namespace(n),
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
        'parent_namespace': parent_namespace(node),
        'fields': fields,
        'methods': methods,
        'annotations': annotations,
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


def run():
    index = Index.create()
    tu = index.parse('test.h', args=['-std=c++17'], options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
    root = tu.cursor

    node_list = []
    traverse(root, lambda n: node_list.append(n))

    macros = find_macros(root, ['CX_PROPERTY', 'REFLECT'])
    macros = list(map(parse_macro, macros))

    classes = find_classes(root)
    classes = list(map(lambda c: parse_class(c, node_list, macros), classes))

    pprint(classes)


if __name__ == '__main__':
    run()
