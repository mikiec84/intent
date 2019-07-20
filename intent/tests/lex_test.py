from ..lex import lex
from ..tok_types import *


def get_lex_output(text):
    output = [x for x in lex(text)]
    return output


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
    assert_token(out[0], TEXT, 'hello, world')
    assert_token(out[1], END)


def test_paragraph():
    assert_tokens(get_lex_output('James Bond: spy'), TEXT, END)


def test_term():
    out = get_lex_output('- James Bond: spy')
    assert_tokens(out, TERM_START, NAME, TERM_PIVOT, TEXT, END)
    assert out[1].value == "James Bond"


def test_term_with_eol():
    assert_tokens(get_lex_output('- James Bond: spy\n'), TERM_START, NAME, TERM_PIVOT, TEXT, EOL, END)
