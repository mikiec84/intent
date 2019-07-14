from ..lex import lex
from ..token_types import *


def get_lex_output(text):
    return [x for x in lex(text)]


def assert_token(t, ttype, value=None):
    assert names_by_value[t.ttype] == names_by_value[ttype]
    if not (value is None):
        assert t.value == value


def assert_tokens(tokens, *ttypes):
    actual = ', '.join(names_by_value[t.ttype] for t in tokens)
    expected = ', '.join([names_by_value[t] for t in ttypes])
    assert actual == expected


def test_simple_text():
    out = get_lex_output('hello, world')
    assert len(out) == 2
    assert_token(out[0], NAME, 'hello, world')
    assert_token(out[1], END)


def test_header():
    assert_tokens(get_lex_output('James Bond: spy'), NAME, DEFINER, TEXT, END)


def test_header_with_eol():
    assert_tokens(get_lex_output('James Bond: spy\n'), NAME, DEFINER, TEXT, EOL, END)
