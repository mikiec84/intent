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
    assert isinstance(items[1], Paragraph)
    assert isinstance(items[2], Term)