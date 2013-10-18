'''
This module is designed to more or less match the lexer interface expected by ply.
However, its implementation is totally different because I didn't want to have it
be regex-centric. I think this version is much closer to what I would be able to
implement in C++, and it's probably also considerably faster.
'''

import re

# Terminal tokens, identified by int. In general, I'm using the
# numeric value of the first char of the token as its identifier,
# but there are a few exceptions. Numeric values that are formed
# by adding or multiplying need to stay that way, since we are
# using some tricks to simplify code based on that logic.

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

# List of terminal tokens is required by ply.yacc...
tokens = [name for name in locals().keys() if re.match('^t_[A-Z_]+$', name)]

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
            return _word(txt, i + 2, end), t_MARK
        elif c == '=':
            return i + 2, operator_token + ord('=')
    return i + 1, operator_token

class LexToken:
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
    Matches Lexer interface expected by ply.yacc -- is iterable and also
    has a .token() method.
    '''
    def __init__(self):
        self.indenter = None
        self.indent_level = 0
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
            eos = LexToken(t_END_OF_STREAM, None, self.line_number, self.end)
            self.__init__()
            return eos

        # Just for ease of use, make some local variables...
        txt = self.txt
        i = self.i
        end = self.end

        ttype = t_INVALID
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
            ttype = t_LINEBREAK
        elif c == ' ':
            self.i = _spaces(txt, i + 1, end)
            if self._line_start == i:
                # First time we see an indented line, figure out what
                # indent convention is used in this code. Thereafter,
                # make sure we see only pure multiples of that string
                # as indents.
                if self.indenter is None:
                    self.indenter = txt[i:self.i]
                    ttype = t_INDENT
                elif (self.i - i) % len(self.indenter) == 0:
                    ttype = t_INDENT
                else:
                    self.i = _end_of_line(txt, i + 1, end)
                    ttype = t_INVALID
            else:
                ttype = t_SPACE
        elif c == '#':
            self.i = _end_of_line(txt, i + 1, end)
            ttype = t_COMMENT
        elif c in '+-*/%&|^~':
            self.i, ttype = _operator_equals_or_mark(txt, i, end)
        elif c in ',=():.':
            ttype = ord(c)
        elif c == '"':
            self.i = _quoted(txt, self.i, end)
            ttype = t_QUOTE
        elif c == '<':
            if i < end - 1:
                c = txt[i+1]
                if c == '=':
                    self.i += 1
                    ttype = t_LESS_THAN_EQUAL
                elif c == '<':
                    self.i += 1
                    ttype = t_DOUBLE_LESS_THAN
            else:
                ttype = t_LESS_THAN
        elif c == '>':
            if i < end - 1:
                c = txt[i+1]
                if c == '=':
                    self.i += 1
                    ttype = t_GREATER_THAN_EQUAL
                elif c == '<':
                    self.i += 1
                    ttype = t_DOUBLE_GREATER_THAN
            else:
                ttype = t_GREATER_THAN
        elif c.isdigit():
            self.i = _digits(txt, i + 1, end)
            ttype = t_NUMBER
        elif c.isalpha():
            self.i = _word(txt, i + 1, end)
            ttype = t_WORD
        return LexToken(ttype, txt[i:self.i], self.line_number, self.i)

if __name__ == '__main__':
    print tokens
    while True:
        print('Enter some code.')
        code = raw_input()
        if not code:
            break
        lx = Lexer()
        lx.input(code)
        for token in lx:
            print(token)
