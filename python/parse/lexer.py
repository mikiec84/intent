import re

t_LINEBREAK = 13
t_SPACE = 32
t_WORD = ord('a')
t_COLON = ord(':')
t_NUMBER = ord('0')
t_INDENT = ord('\t')
t_COMMENT = ord('#')
t_OPEN_PAREN = ord('(')
t_CLOSE_PAREN = ord(')')
t_MARK = 1
t_PLUS = ord('+')
t_PLUS_EQUALS = ord('+') + ord('=')
t_MINUS = ord('-')
t_MINUS_EQUALS = ord('-') + ord('=')
t_STAR = ord('*')
t_STAR_EQUALS = ord('*') + ord('=')
t_SLASH = ord('/')
t_SLASH_EQUALS = ord('/') + ord('=')
t_EQUALS = ord('=')
t_TILDE = ord('~')
t_DOT = ord('.')
t_COMMA = ord(',')
t_QUOTE = ord('"')
t_MODULO = ord('%')
t_MODULO_EQUALS = ord('%') + ord('=')
t_BIT_AND = ord('&')
t_BIT_OR = ord('|')
t_BIT_XOR = ord('^')
t_LESS_THAN = ord('<')
t_LESS_THAN_EQUAL = ord('<') + ord('=')
t_DOUBLE_LESS_THAN = ord('<') * 2
t_GREATER_THAN = ord('>')
t_GREATER_THAN_EQUAL = ord('>') + ord('=')
t_DOUBLE_GREATER_THAN = ord('>') * 2
t_END_OF_STREAM = -1
t_INVALID = -2

pat = re.compile('^t_[A-Z_]+$')
tokens = [name for name in locals().keys() if pat.match(name)]
del pat

def _spaces(txt, i, end):
    while i < end:
        if txt[i] != ' ':
            return i
        i += 1
    return end

def _number(txt, i, end):
    while i < end:
        if not txt[i].isdigit():
            return i
        i += 1
    return end

def _quoted(txt, i, end):
    while i < end:
        c = txt[i]
        if c == '\\':
            i += 1
        elif c == '"':
            return i + 1
        i += 1
    return end

def _word(txt, i, end):
    while i < end:
        c = txt[i]
        if not (c.isalnum() or c == '_'):
            return i
        i += 1
    return end

def _rest_of_line(txt, i, end):
    while i < end:
        if txt[i] == '\n':
            return i
        i += 1
    return end

def _operator(txt, i, end, operator_token, mark_compatible=False):
    if i < end - 1:
        c = txt[i + 1]
        if mark_compatible and c.isalpha():
            return _word(txt, i + 2, end), t_MARK
        elif c == '=':
            return i + 2, operator_token + ord('=')
    return i + 1, operator_token

class LexToken:
    def __init__(self, type, value, lineno, lexpos):
        self.type = type
        self.value = value
        self.lineno = lineno
        self.lexpos = lexpos
    def __str__(self):
        return '%s "%s" at line %d, %d' % (self.type, self.value, self.lineno, self.lexpos)

class Lexer:
    def __init__(self):
        self.indenter = None
        self.indent_level = 0
        self.linepos = 0
        self.txt = None
        self.i = None
        self.end = None

    def input(self, txt):
        self.__init__()
        self.txt = txt
        self.i = 0
        self.lineno = 0
        self.linepos = 0

    def __iter__(self):
        return self

    def next(self):
        if self.i < 0 or self.i >= len(self.txt):
            raise StopIteration
        return self.token()

    def token(self):

        # Just for ease of use, make some local variables...
        txt = self.txt
        i = self.i
        end = len(self.txt)
        type = t_INVALID
        c = txt[i]

        # Advance past the char we are examining. In the case of multi-char tokens,
        # we will adjust this again down below.
        self.i += 1

        if c == '\n':
            self.linepos = 0
            type = t_LINEBREAK
        elif c == ' ':
            j = _spaces(txt, self.i, end)
            if self.linepos == 0:
                if self.indenter is None:
                    self.indenter = txt[i:j]
                    self.i = j
                    type = t_INDENT
                elif (j - i) % len(self.indenter) == 0:
                    self.i = j
                    type = t_INDENT
                else:
                    self.i = _rest_of_line(txt, self.i, end)
                    type = t_INVALID
            else:
                self.i = j
                type = t_SPACE
        elif c == '#':
            self.i = _rest_of_line(txt, i + 1, end)
            type = t_COMMENT
        elif c == ',':
            type = t_COMMA
        elif c == '=':
            type = t_EQUALS
        elif c == '+':
            self.i, type = _operator(txt, i, end, t_PLUS, mark_compatible=True)
        elif c == '-':
            self.i, type = _operator(txt, i, end, t_MINUS, mark_compatible=True)
        elif c == '*':
            self.i, type = _operator(txt, i, end, t_STAR, mark_compatible=False)
        elif c == '/':
            self.i, type = _operator(txt, i, end, t_SLASH, mark_compatible=False)
        elif c == '%':
            self.i, type = _operator(txt, i, end, t_MODULO, mark_compatible=False)
        elif c == '&':
            self.i, type = _operator(txt, i, end, t_BIT_AND, mark_compatible=False)
        elif c == '|':
            self.i, type = _operator(txt, i, end, t_BIT_OR, mark_compatible=False)
        elif c == '^':
            self.i, type = _operator(txt, i, end, t_BIT_XOR, mark_compatible=False)
        elif c == '~':
            self.i, type = _operator(txt, i, end, t_TILDE, mark_compatible=False)
        elif c == '(':
            type = t_OPEN_PAREN
        elif c == ')':
            type = t_CLOSE_PAREN
        elif c == '"':
            self.i = _quoted(txt, self.i, end)
            type = t_QUOTE
        elif c == ':':
            type = t_COLON
        elif c == '.':
            type = t_DOT
        elif c == '<':
            if i < end - 1:
                c = txt[i+1]
                if c == '=':
                    self.i += 1
                    type = t_LESS_THAN_EQUAL
                elif c == '<':
                    self.i += 1
                    type = t_DOUBLE_LESS_THAN
            else:
                type = t_LESS_THAN
        elif c == '>':
            if i < end - 1:
                c = txt[i+1]
                if c == '=':
                    self.i += 1
                    type = t_GREATER_THAN_EQUAL
                elif c == '<':
                    self.i += 1
                    type = t_DOUBLE_GREATER_THAN
            else:
                type = t_GREATER_THAN
        elif c.isdigit():
            self.i = _number(txt, i + 1, end)
            type = t_NUMBER
        elif c.isalpha():
            self.i = _word(txt, i + 1, end)
            type = t_WORD
        return LexToken(type, txt[i:self.i], self.lineno, self.i)

if __name__ == '__main__':
    while True:
        print('Enter some code.')
        code = raw_input()
        if not code:
            break
        lx = Lexer()
        lx.input(code)
        for token in lx:
            print(token)
