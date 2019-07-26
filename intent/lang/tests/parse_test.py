from ..parse import parse_text
from ..ast import *


def test_simple_parse():
    txt = """
This is a simple paragraph.
This is [another] with an anchor.

-term: definition
"""
    items = [x for x in parse_text(txt)]
    assert len(items) == 3
    assert isinstance(items[0], Paragraph)
    x = items[1]
    assert isinstance(x, Paragraph)
    assert isinstance(x.parts[0], Text)
    assert isinstance(x.parts[1], Anchor)
    assert isinstance(x.parts[2], Text)
    assert isinstance(items[2], Term)