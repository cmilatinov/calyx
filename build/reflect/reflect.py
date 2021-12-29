import re
import clang.cindex
from pprintpp import pprint
from clang.cindex import *


def traverse(node, func):
    func(node)
    for child in node.get_children():
        traverse(child, func)


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
    traverse(node, lambda n: arr.append(n) if n.kind == kind else None)


def find_classes(node):
    classes = []
    find_type(node, CursorKind.CLASS_DECL, classes)
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
    regex = re.escape(macro.spelling) + r"\((.*)\)"
    match = re.match(regex, original_macro)
    if match:
        arguments = match.groups()[0].split(",")

    return {
        'type': 'macro',
        'node': macro,
        'name': macro.spelling,
        'arguments': arguments
    }


def parse_class(node, macros):
    if node.kind != CursorKind.CLASS_DECL:
        return None

    fields = []
    methods = []
    base = None
    prev = node

    def parse_field(n):
        nonlocal prev, base

        if n.kind == CursorKind.FIELD_DECL and n.semantic_parent == node:
            annotations = list(filter(lambda m: is_location_between(m['node'].location, prev.location, n.location), macros))
            for a in annotations:
                macros.remove(a)

            fields.append({
                'type': 'field',
                'name': n.spelling,
                'full_name': full_name(n),
                'annotations': annotations,
                'node': n,
                'access': n.access_specifier,
                'kind': 'static' if n.storage_class == clang.cindex.StorageClass.STATIC else 'instance'
            })

        elif n.kind == CursorKind.CXX_METHOD and n.semantic_parent == node:
            annotations = list(filter(lambda m: is_location_between(m['node'].location, prev.location, n.location), macros))
            for a in annotations:
                macros.remove(a)

            methods.append({
                'type': 'method',
                'name': n.spelling,
                'full_name': full_name(n),
                'annotations': annotations,
                'node': n,
                'access': n.access_specifier,
                'kind': 'static' if n.storage_class == clang.cindex.StorageClass.STATIC else 'instance'
            })

        elif n.kind == CursorKind.CXX_BASE_SPECIFIER:

            base = n.referenced

        prev = n

    traverse(node, parse_field)

    return {
        'type': 'class',
        'name': node.spelling,
        'full_name': full_name(node),
        'fields': fields,
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
    tu = index.parse('test.h', args=['-std=c++17'],
                     options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
    root = tu.cursor

    macros = find_macros(root, ['CX_PROPERTY'])
    macros = list(map(parse_macro, macros))

    classes = find_classes(root)
    for c in classes:
        cl = parse_class(c, macros)
        pprint(cl)

    pprint(classes)
    pass


if __name__ == '__main__':
    run()
