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
    expected = 'BEGIN_DOC, ' + ', '.join([names_by_value[t] for t in ttypes]) + ', END_DOC'
    assert actual == expected


def test_simple_text():
    out = get_lex_output('hello, world')
    assert_token_types(out, BEGIN_PARA, TEXT)
    assert out[2].value == 'hello, world'


def test_simple_text_with_eol():
    out = get_lex_output('hello, world\n')
    assert_token_types(out, BEGIN_PARA, TEXT, END_LINE)
    assert out[2].value == 'hello, world'


def test_paragraph_trailing_whitespace():
    out = get_lex_output('hello, world   ')
    assert_token_types(out, BEGIN_PARA, TEXT)
    assert out[2].value == 'hello, world'


def test_text_with_anchor():
    out = get_lex_output("Don't [cry for me], Argentina")
    assert len(out) == 8
    assert_token_types(out, BEGIN_PARA, TEXT, BEGIN_ANCHOR, TEXT, END_HYPERTEXT, TEXT)
    assert_token_values(out, (2, "Don't "), (4, "cry for me"), (6, ", Argentina"))


def test_paragraph():
    out = get_lex_output('James Bond: spy')
    assert_token_types(out, BEGIN_PARA, TEXT)
    assert_token_values(out, (2, 'James Bond: spy'))


def test_term():
    out = get_lex_output('- James Bond: spy')
    assert_token_types(out, BEGIN_TERM, NAME, PIVOT_TERM, TEXT)
    assert_token_values(out, (3, "James Bond"), (5, "spy"))


def test_term():
    out = get_lex_output('- James Bond: spy\n')
    assert_token_types(out, BEGIN_TERM, NAME, PIVOT_TERM, TEXT, END_LINE)


def test_lex_error():
    out = get_lex_output('[')
    assert_token_types(out, BEGIN_PARA, BEGIN_ANCHOR, LEX_ERROR)


def test_lex_error_trailing_text():
    out = get_lex_output('[trailing text')
    assert_token_types(out, BEGIN_PARA, BEGIN_ANCHOR, TEXT, LEX_ERROR)


def test_simple_multiline():
    out = get_lex_output('line1\nline2\nline3')
    assert_token_types(out, BEGIN_PARA, TEXT, END_LINE, BEGIN_PARA, TEXT, END_LINE, BEGIN_PARA, TEXT)
