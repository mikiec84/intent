# Yacc example

import yacc
import lex
import intent_tokens

# Get the token map from the lexer.  This is required.
from intent_tokens import tokens

def p_expression_plus(p):
    '''
    expression : expression PLUS term
               | expression MINUS term
    '''
    p[0] = p[1] + p[3]

def p_expression_term(p):
    'expression : term'
    p[0] = p[1]

def p_term_times(p):
    'term : term TIMES factor'
    p[0] = p[1] * p[3]

def p_term_div(p):
    'term : term DIVIDE factor'
    p[0] = p[1] / p[3]

def p_term_factor(p):
    'term : factor'
    p[0] = p[1]

def p_factor_num(p):
    'factor : NUMBER'
    p[0] = p[1]

def p_factor_expr(p):
    'factor : LPAREN expression RPAREN'
    p[0] = p[2]

# Error rule for syntax errors
def p_error(p):
    print "Syntax error in input with %s!" % p.value

# Build the lexer.
mylex = lex.lex(intent_tokens)
mylex.indents = []
mylex.indent_char = None
mylex.indent_delta = 0

# Build the parser.
parser = yacc.yacc()

while True:
    try:
        s = raw_input('calc > ')
    except EOFError:
        break
    if not s: continue
    result = parser.parse(s, lexer=mylex)
    print result