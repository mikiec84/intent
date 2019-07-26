import os

from .ast import *
from .lex import lex
from .tok_types import *
from ..space import walk_code_files


def parse_space(root):
    for abspath in walk_code_files(root):
        for node in parse_file(abspath, root):
            yield node


def parse_file(fname, root=None):
    fname = os.path.normpath(fname)
    source_uri = os.path.relpath(fname, root) if root else fname
    with open(fname, 'rt') as f:
        text = f.read()
    for node in parse_text(text, source_uri):
        yield node


def syntax_checked_token_stream(ts):
    """
    Only omit tokens for lines that don't have known syntax errors.
    This simplifies logic required in downstream parsing classes, because
    they only have to deal with parsing problems, not lexer problems.
    """
    pending = []
    error_on_line = None
    try:
        while True:
            t = next(ts)
            if ends_line(t):
                if error_on_line:
                    yield error_on_line
                else:
                    for p in pending:
                        yield p
                yield t
                pending = []
            elif t.ttype == LEX_ERROR:
                error_on_line = t
            else:
                pending.append(t)
    except StopIteration:
        pass
    for t in pending:
        yield t


def parse_text(text, source_uri=None):
    try:
        if source_uri is None:
            source_uri = '(buffer)'
        token_stream = syntax_checked_token_stream(lex(text))
        t = next(token_stream)
        precondition(t.ttype == BEGIN_DOC, "first token should be BEGIN_DOC")
        for t in token_stream:
            if t.ttype == BEGIN_PARA:
                yield Paragraph(token_stream)
            elif t.ttype == BEGIN_TERM:
                yield Term(token_stream)
            elif t.ttype == LEX_ERROR:
                yield t
            else:
                assert ends_line(t)
    except StopIteration:
        yield SyntaxError('Incomplete data.')
