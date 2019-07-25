from ..tok import *
from ..tok_types import *
from ..lex_state import LexState

hello_token = Token(LexState("hello there"), TEXT, 0, 5)


def test_ctor():
    t = hello_token
    assert t.ttype == TEXT
    assert t.value == 'hello'
    assert t.begin == 0
    assert t.end == 5


def test_str():
    assert str(hello_token) == 'TEXT: "hello" at ln 1, col 1'


def test_line_num():
    ls = LexState("\n\na bc")
    ls.line_start = 2
    ls.line_num = 3
    t = Token(ls, TEXT, 4, 6)
    assert t.line_num == 3
    assert t.col == 3