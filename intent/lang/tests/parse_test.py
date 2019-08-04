from ..parse import parse_text
from ..ast import *


def assert_items(items, *types):
    assert len(items) == len(types)
    for i in range(len(items)):
        item = items[i]
        typ = types[i]
        assert isinstance(item, typ)


def test_simple_parse():
    txt = """
This is a simple paragraph.
This is [another] with an anchor.

-term: definition
"""
    items = [x for x in parse_text(txt)]
    assert_items(items, Paragraph, Paragraph, Term)
    assert_items(items[1].parts, Text, Anchor, Text)

def test_simple_parse():
    txt = """
This is a simple paragraph.
This is [another] with an anchor.

-term: definition
"""
    items = [x for x in parse_text(txt)]
    assert_items(items, Paragraph, Paragraph, Term)
    assert_items(items[1].parts, Text, Anchor, Text)
