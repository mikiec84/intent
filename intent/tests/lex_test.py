from ..lex import lex
from ..tok_types import *


def get_lex_output(text):
    output = [x for x in lex(text)]
    return output


def assert_token_values(tokens, *index_value_pairs):
    for pair in index_value_pairs:
        idx = pair[0]
        expected = pair[1]
        assert tokens[idx].value == expected


def assert_token_types(tokens, *ttypes):
    actual = ', '.join(names_by_value[t.ttype] for t in tokens)
    expected = 'BEGIN_DOC, BEGIN_LINE, ' + ', '.join([names_by_value[t] for t in ttypes]) + ', END_DOC'
    assert actual == expected


def test_simple_text():
    out = get_lex_output('hello, world')
    assert_token_types(out, BEGIN_PARA, TEXT)
    assert out[3].value == 'hello, world'


def test_simple_text_with_eol():
    out = get_lex_output('hello, world\n')
    assert_token_types(out, BEGIN_PARA, TEXT, END_LINE)
    assert out[3].value == 'hello, world'


def test_paragraph_trailing_whitespace():
    out = get_lex_output('hello, world   ')
    assert_token_types(out, BEGIN_PARA, TEXT)
    assert out[3].value == 'hello, world'


def test_text_with_anchor():
    out = get_lex_output("Don't [cry for me], Argentina")
    assert len(out) == 9
    assert_token_types(out, BEGIN_PARA, TEXT, BEGIN_ANCHOR, TEXT, END_HYPERTEXT, TEXT)
    assert_token_values(out, (3, "Don't "), (5, "cry for me"), (7, ", Argentina"))


def test_paragraph():
    out = get_lex_output('James Bond: spy')
    assert_token_types(out, BEGIN_PARA, TEXT)
    assert_token_values(out, (3, 'James Bond: spy'))


def test_term():
    out = get_lex_output('- James Bond: spy')
    assert_token_types(out, BEGIN_TERM, NAME, PIVOT_TERM, TEXT)
    assert_token_values(out, (3, "James Bond"), (5, "spy"))


def test_term():
    out = get_lex_output('- James Bond: spy\n')
    assert_token_types(out, BEGIN_TERM, NAME, PIVOT_TERM, TEXT, END_LINE)
