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
    assert str(hello_token) == 'TEXT: "hello" at [0:5]'
