from .dbc import precondition, postcondition
from .tok_types import *


class Container:
    def __init__(self):
        self.items = []


class Doc(Container):
    def __init__(self, space, relpath):
        Container.__init__(self)
        self.space = space
        self.relpath = relpath


class Text:
    def __init__(self, token):
        precondition(token.ttype == TEXT, "token type should be TEXT")
        self.token = token


class Anchor:
    def __init__(self, token_stream):
        self.text = next(token_stream)
        t = next(token_stream)
        postcondition(t.ttype == END_HYPERTEXT, "should be followed by END_HYPERTEXT") # temporary; rest not implemented


class Paragraph(Container):
    def __init__(self, token_stream):
        Container.__init__(self)
        self.parts = []
        for t in token_stream:
            if ends_line(t):
                break
            if t.ttype == TEXT:
                self.parts.append(Text(t))
            elif t.ttype == BEGIN_ANCHOR:
                self.parts.append(Anchor(token_stream))
            else:
                raise NotImplemented


class Term(Container):
    def __init__(self, token_stream):
        Container.__init__(self)
        self.name = next(token_stream)
        assert next(token_stream).ttype == PIVOT_TERM
        self.definition = next(token_stream)

