def token_count(*items):
    token_count = 0
    for item in items:
        if hasattr(item, 'token_count'):
            token_count += getattr(item, 'token_count')
        else:
            token_count += 1
    return token_count

class GroupedExpr(Chunk):
    def __init__(self, lparen, expr, rparen):
        self.first_token = self.lparen = lparen
        self.expr = expr
        self.last_token = self.rparen = rparen
        self.token_count = 2 + token_count(expr)

class BinaryExpr:
    def __init__(self, lhs, op, rhs):
        self.first_token = self.lhs = lhs
        self.op = op
        self.last_token = self.rhs = rhs
        self.token_count = 1 + token_count(lhs, rhs)

class Term:
    def __init__(self, value):
        self.value = value