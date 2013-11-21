from lexer import *
from ast_nodes import *

class TokenStream:
    def __init__(self, txt):
        self.stack = []
        self.lexer = Lexer()
        self.lexer.input(txt)
        self.idx = -1

    def push(self, n=1):
        self.idx += n
        # If we've already buffered the next token, just return it.
        if self.idx < len(self.stack):
            token = self.stack[self.idx]
            return token
        # Otherwise, see if the lexer can give us the next token.
        while self.lexer:
            token = self.lexer.token()
            if token.type == END_OF_STREAM:
                self.lexer = None
                # If we get here, it always means we've exhausted
                # the token stream before we got the token the caller
                # asked for. Always return None.
                return
            self.stack.append(token)
            if len(self.stack) > self.idx:
                return token

    def pop(self, n=1):
        self.idx -= n

    def consume(self, token_count):
        self.tokens = self.tokens[token_count:]

class PushedToken:
    def __init__(self, stream, delta=1):
        self.stream = stream
        self.delta = delta
    def __enter__(self):
        return self.stream.push(self.delta)
    def __exit__(self, type, value, traceback):
        if self.delta:
            self.stream.pop(self.delta)
'''
Here's the rule I want:

expr:
    expr SPACE PLUS SPACE expr | NUMBER | WORD | grouped_expr

...but that rule causes infinite recursion in an LL parser. So we rewrite like this:

expr:
    NUMBER expr_rest | WORD exprrest | grouped_expr expr_rest

exprest:
    EOS | SPACE PLUS SPACE expr

'''

NULL = "NULL"

def expr(tokens):
    with PushedToken(tokens) as t1:
        if t1.type in [NUMBER, WORD]:
            rest = expr_rest(tokens)
            if rest:
                if rest == NULL:
                    return t1
                else:
                    return BinaryExpr(t1, rest[0], rest[1])
    grp = grouped_expr(tokens)
    if grp:
        rest = expr_rest(tokens)
        if rest:
            if rest == NULL:
                return grp
            else:
                return grp, rest[0], rest[1]

def grouped_expr(tokens):
    with PushedToken(tokens) as t1:
        if t1.type == LPAREN:
            e = expr(tokens)
            if e:
                with PushedToken(tokens, 1 + token_count(e)) as t2:
                    if t2.type == RPAREN:
                        return t1, e, t2

def expr_rest(tokens):
    with PushedToken(tokens) as t1:
        if t1 is None or t1.type == RPAREN:
            return NULL
        elif t1.type in BINARY_OPERATORS:
            e = expr(tokens)
            if e:
                return t1, e

if __name__ == '__main__':
    while True:
        try:
            print('\nCode:')
            code = raw_input()
            stream = TokenStream(code)
            ex = expr(stream)
            print str(ex)
        except EOFError:
            break