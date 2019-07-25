import os

from .lex import lex
from .tok_types import *
from .ast import *
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


def parse_text(text, source_uri=None):
    if source_uri is None:
        source_uri = '(buffer)'
    token_stream = lex(text)
    assert next(token_stream).ttype == BEGIN_DOC
    for t in token_stream:
        if t.ttype == BEGIN_PARA:
            yield Paragraph(token_stream)
        elif t.ttype == BEGIN_TERM:
            yield Term(token_stream)
        else:
            assert t.ttype in [END_LINE, END_DOC]
