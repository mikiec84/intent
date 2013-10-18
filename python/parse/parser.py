from ply import yacc

# reserved words
reserved = 'if|then|else|while|for|class|func|try|catch|return|use|as'
x = {}
for word in reserved.split('|'):
  x[word] = word.upper()
reserved = x
del(x)

# List of token names.
tokens = 'ID|ASSIGN|DOT|NUMBER|COLON|PLUS|MINUS|TIMES|DIVIDE|CPP_COMMENT|LPAREN|RPAREN|LANGL|RANGL|LBRACK|RBRACK|STRING'.split(
  '|') + list(reserved.values())

# Regular expression rules for simple tokens. These are in precedence order.
t_FUNC = r'func'
t_CLASS = r'class'
t_IF = r'if'
t_FOR = r'for'
t_USE = r'use'
t_AS = r'as'
t_ASSIGN = r'='
t_DOT = '\.'
t_PLUS    = r'\+'
t_MINUS   = r'-'
t_TIMES   = r'\*'
t_CPP_COMMENT = r'//.*?(?=\n)'
t_DIVIDE  = r'/'
t_LPAREN  = r'\('
t_RPAREN  = r'\)'
t_LANGL  = r'<'
t_RANGL  = r'>'
t_LBRACK  = r'\['
t_RBRACK  = r'\]'
t_COLON = r':'
t_STRING = r'".*?"'

def t_ID(t):
    r'[a-zA-Z_][a-zA-Z_0-9]*'
    if t.value[0].islower():
      t.type = reserved.get(t.value,'ID')    # Check for reserved words
    return t

def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

# Define a rule so we can track line numbers
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# A string containing ignored characters (spaces and tabs)
t_ignore  = ' \t'

# Error handling rule
def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)

# Build the lexer
lexer = lex.lex()

if __name__ == '__main__':
    import sys
    with open(sys.argv[1], 'r') as f:
        txt = f.read()
    lexer.input(txt)
    for token in lexer:
        print token