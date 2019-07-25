from .tok_types import *


class Container:
    def __init__(self):
        self.items = []


class Doc(Container):
    def __init__(self, space, relpath):
        Container.__init__(self)
        self.space = space
        self.relpath = relpath


class Paragraph(Container):
    def __init__(self, token_stream):
        Container.__init__(self)
        self.tokens = []
        for token in token_stream:
            if token.ttype in [END_LINE, END_DOC]:
                break
            self.tokens.append(token)


class Term(Container):
    def __init__(self, token_stream):
        Container.__init__(self)
        self.name = next(token_stream)
        assert next(token_stream).ttype == PIVOT_TERM
        self.definition = next(token_stream)
