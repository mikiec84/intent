import re

# Terminal tokens, identified by int. In general, I'm using the
# numeric value of the first char of the token as its identifier,
# but there are a few exceptions. Numeric values that are formed
# by adding or multiplying need to stay that way, since we are
# using some tricks to simplify code based on that logic.

EQUALS = ord('=')
LINEBREAK = 13
SPACE = 32
WORD = ord('a')
COLON = ord(':')
NUMBER = ord('0')
INDENT = ord('\t')
COMMENT = ord('#')
LPAREN = ord('(')
RPAREN = ord(')')
MARK = 1
PLUS = ord('+')
PLUS_EQUALS = PLUS + EQUALS
MINUS = ord('-')
MINUS_EQUALS = MINUS + EQUALS
TIMES = ord('*')
TIMES_EQUALS = TIMES + EQUALS
DIVIDE = ord('/')
DIVIDE_EQUALS = DIVIDE + EQUALS
TILDE = ord('~')
DOT = ord('.')
COMMA = ord(',')
QUOTE = ord('"')
MODULO = ord('%')
MODULO_EQUALS = MODULO + EQUALS
BIT_AND = ord('&')
BIT_AND_EQUALS = BIT_AND + EQUALS
BIT_OR = ord('|')
BIT_OR_EQUALS = BIT_OR + EQUALS
BIT_XOR = ord('^')
BIT_XOR_EQUALS = BIT_XOR + EQUALS
LESS_THAN = ord('<')
LESS_THAN_EQUAL = ord('<') + EQUALS
DOUBLE_LESS_THAN = ord('<') * 2
GREATER_THAN = ord('>')
GREATER_THAN_EQUAL = ord('>') + EQUALS
DOUBLE_GREATER_THAN = ord('>') * 2
END_OF_STREAM = -1
INVALID = -2

BINARY_OPERATORS = [PLUS, PLUS_EQUALS, MINUS, MINUS_EQUALS, TIMES, TIMES_EQUALS, DIVIDE, DIVIDE_EQUALS, MODULO, MODULO_EQUALS, BIT_AND, BIT_AND_EQUALS, BIT_OR, BIT_OR_EQUALS, BIT_XOR, BIT_XOR_EQUALS]

def _spaces(txt, i, end):
    '''
    Find end of a run of spaces in txt.
    '''
    while i < end:
        if txt[i] != ' ':
            return i
        i += 1
    return end

def _digits(txt, i, end):
    '''
    Find end of a run of digits in txt.
    '''
    while i < end:
        if not txt[i].isdigit():
            return i
        i += 1
    return end

def _quoted(txt, i, end):
    '''
    Find end of a quoted string "..." in txt.
    '''
    while i < end:
        c = txt[i]
        if c == '\\':
            i += 1
        elif c == '"':
            return i + 1
        i += 1
    return end

def _word(txt, i, end):
    '''
    Find end of a word in txt.
    '''
    while i < end:
        c = txt[i]
        if not (c.isalnum() or c == '_'):
            return i
        i += 1
    return end

def _end_of_line(txt, i, end):
    '''
    Find end of line in txt.
    '''
    while i < end:
        if txt[i] == '\n':
            return i
        i += 1
    return end

def _operator_equals_or_mark(txt, i, end):
    '''
    Given an operator char like +, -, *, /, or %, see if the char is followed
    by = (in which case the assignment version of the operator is returned),
    by an alpha (in which case we have a mark), or by something else (in which
    case the character is its own operator.
    '''
    operator_char = txt[i]
    operator_token = ord(operator_char)
    if i < end - 1:
        c = txt[i + 1]
        if c.isalpha() and operator_char in '+-':
            return _word(txt, i + 2, end), MARK
        elif c == '=':
            return i + 2, operator_token + EQUALS
    return i + 1, operator_token

def _angle_operator(txt, i, end):
    operator_char = txt[i]
    ttype = ord(operator_char)
    if i < end - 1:
        c = txt[i + 1]
        if c == '=':
            i += 1
            ttype = LESS_THAN_EQUAL
        elif c == '<':
            i += 1
            ttype = DOUBLE_LESS_THAN
    i += 1
    return i, ttype

class LexToken(object):
    '''
    Interface required by ply.yacc -- holds info about a single token.
    '''
    def __init__(self, ttype, value, line_number, lexpos):
        self.type = ttype
        self.value = value
        self.line_number = line_number
        self.lexpos = lexpos
    @property
    def lineno(self):
        # ply.yacc requires lineno, but I prefer the clearer name. So
        # provide an alias to make yacc happy.
        return self.line_number
    def __str__(self):
        return '%s "%s" at line %s, %s' % (self.type, self.value, self.line_number, self.lexpos)

class Lexer:
    '''
    Generates next token.
    '''
    def __init__(self):
        self.indenter = None
        self.indenlevel = 0
        self._line_start = 0
        self.line_number = 0
        self.txt = None
        self.i = None
        self.end = None

    def input(self, txt):
        '''
        Set the string that's going to be tokenized.
        '''
        self.__init__()
        self.txt = txt
        self.i = 0
        self.line_number = 1
        self._line_start = 0
        self.end = len(txt)

    @property
    def position_on_line(self):
        '''
        What is the 1-based index of the cursor within the current line?
        '''
        if self.line_number > 0:
            return self.i - self._line_start
        return -1

    def __iter__(self):
        return self

    def next(self):
        if self.line_number > 0:
            return self.token()
        raise StopIteration

    def token(self):

        # Detect end of stream. Return explicit marker and reset state.
        if self.i >= self.end:
            eos = LexToken(END_OF_STREAM, None, self.line_number, self.end)
            self.__init__()
            return eos

        # Just for ease of use, make some local variables...
        txt = self.txt
        i = self.i
        end = self.end

        ttype = INVALID
        c = txt[i]

        # Ignore \r entirely. This isn't exactly correct for text files created
        # by old mac editors, but we're going to ignore that possiblity for now.
        if c == '\r':
            i = self.i = i + 1

        # Advance past the char we are examining. In the case of
        # multi-char tokens, we will advance more, below.
        self.i += 1

        if c == '\n':
            self._line_start = self.i
            self.line_number += 1
            ttype = LINEBREAK
        elif c == ' ':
            # Read to end of spaces.
            self.i = _spaces(txt, i + 1, end)
            # Are these leading spaces at front of line?
            if self._line_start == i:
                # First time we see an indented line, figure out what
                # indent convention is used in this code. Thereafter,
                # make sure we see only pure multiples of that string
                # as indents.
                if self.indenter is None:
                    self.indenter = txt[i:self.i]
                    ttype = INDENT
                elif (self.i - i) % len(self.indenter) == 0:
                    ttype = INDENT
                else:
                    self.i = _end_of_line(txt, i + 1, end)
                    ttype = INVALID
            else:
                return self.token()
        elif c == '#':
            self.i = _end_of_line(txt, i + 1, end)
            ttype = COMMENT
        elif c in '+-*/%&|^~':
            self.i, ttype = _operator_equals_or_mark(txt, i, end)
        elif c in ',=():.':
            ttype = ord(c)
        elif c == '"':
            self.i = _quoted(txt, self.i, end)
            ttype = QUOTE
        elif c in '<>':
            self.i, ttype = _angle_operator(txt, i, end)
        elif c.isdigit():
            self.i = _digits(txt, i + 1, end)
            ttype = NUMBER
        elif c.isalpha():
            self.i = _word(txt, i + 1, end)
            ttype = WORD
        token = LexToken(ttype, txt[i:self.i], self.line_number, self.i)
        return token

if __name__ == '__main__':
    while True:
        print('\nEnter some code.')
        code = raw_input()
        if not code:
            break
        lx = Lexer()
        lx.input(code)
        for token in lx:
            print(token)
