import re
from pygments.token import Token, Comment, Name, String, Number, Punctuation, Error, Keyword, Text

class IntentLexer:

    reserved_words = ['if', 'for', 'with', 'while', 'try', 'end', 'handle', 'use', 'when', 'yield', 'return']

    def get_tokens_unprocessed(self, txt):
        '''
        Return an iterable of (index, tokentype, value) tuples where index is the
        starting position of the token within the input text.
        '''
        end_of_string = len(txt)
        begin = 0
        start_statement = True
        while begin < end_of_string:
            ttype = None
            c = txt[begin]
            if c in ' \t\r\n':
                end = IntentLexer._end_of_whitespace(txt, begin + 1, end_of_string)
                ttype = Text
                start_statement = True
            elif c == '#' or c == '|':
                end = IntentLexer._end_of_statement(txt, begin + 1, end_of_string)
                ttype = Comment
            elif c == '`':
                end = txt.find('`', begin + 1)
                if end == -1:
                    end = end_of_string
                else:
                    end += 1
                ttype = String.Backtick
            elif c == '~':
                ttype = Punctuation
                end = begin + 1
                if begin + 1 < end_of_string:
                    nxt = txt[begin + 1]
                    if nxt.isalpha():
                        end = IntentLexer._end_of_phrase(txt, begin + 2, end_of_string)
                        ttype = Name.Decorator
                    elif nxt in "\"'":
                        end, ttype = IntentLexer._handle_quoted(txt, begin, end_of_string)
                start_statement = False
            elif c in '+-':
                ttype = Punctuation
                end = begin + 1
                if begin + 1 < end_of_string:
                    nxt = txt[begin + 1]
                    if nxt.isalpha():
                        end = IntentLexer._end_of_phrase(txt, begin + 2, end_of_string)
                        ttype = Name.Decorator
                    elif nxt.isdigit():
                        end, ttype = IntentLexer._handle_number(txt, begin, end_of_string)
                    elif nxt == '.':
                        if begin + 2 < end and txt[begin + 2].isdigit():
                            end, ttype = IntentLexer._handle_number(txt, begin, end_of_string)
                start_statement = False
            elif c in '*/%&^\\[],=():.<>':
                ttype = Punctuation
                end = begin + 1
                start_statement = False
            elif c == '"':
                end, ttype = IntentLexer._handle_quoted(txt, begin, end_of_string)
                start_statement = False
            elif c.isdigit():
                end, ttype = IntentLexer._handle_number(txt, begin, end_of_string)
                start_statement = False
            elif c.isalpha():
                if c.isupper():
                    ttype = Name.Function
                    end = IntentLexer._end_of_phrase(txt, begin + 1, end_of_string)
                    phrase = txt[begin:end]
                    if phrase.isupper():
                        ttype = Name.Constant
                else:
                    yield_name = True
                    if start_statement:
                        j = begin + 1
                        while j < end_of_string and txt[j].isalpha() and txt[j].islower():
                            j += 1
                        word = txt[begin:j]
                        if word in IntentLexer.reserved_words:
                            ttype = Keyword.Reserved
                            end = j
                            yield_name = False
                    if yield_name:
                        ttype = Name.Variable
                        end = IntentLexer._end_of_phrase(txt, begin + 1, end_of_string)
                start_statement = False
            else:
                ttype = Error
                end = IntentLexer._end_of_line(txt, begin + 1, end_of_string)
                start_statement = True
            if ttype is None:
                break
            yield (begin, ttype, txt[begin:end])
            begin = end

    @staticmethod
    def _handle_number(txt, i, end, ttype=Number):
        digit_chars = '0123456789'
        c = txt[i]
        if c in '-+':
            i += 1
            if i >= end:
                return end, Error
            c = txt[i]
        if c == '0':
            if i + 1 < end:
                nxt = txt[i + 1].lower()
                if nxt == 'b':
                    digit_chars = '01'
                    dot = False
                    ttype = Number.Binary
                    i += 1
                elif nxt == 'x':
                    digit_chars = '0123456789abcdefABCDEF'
                    dot = False
                    ttype = Number.Hex
                    i += 1
                elif nxt in '01234567':
                    digit_chars = '01234567'
                    dot = False
                    ttype = Number.Oct
                    i += 1
                elif nxt == '.':
                    dot = True
                    ttype = Number.Float
                    i += 1
                elif nxt == '_':
                    pass
                else:
                    return i + 1, Number.Integer
            else:
                return i + 1, Number.Integer

        last_was_digit = True
        i += 1
        while i < end:
            c = txt[i]
            if c == '.':
                if ttype is Number:
                    ttype = Number.Float
                    last_was_digit = False
                elif ttype is Number.Float:
                    # Second dot is a problem; truncate number here
                    return i, Number.Float
                else:
                    # We're reading binary, hex, or octal; truncate here
                    return i, ttype
            elif c == '_':
                # Grouping symbol is valid as long as surrounded by digits
                if not last_was_digit:
                    return i, ttype
                last_was_digit = False
            elif c in digit_chars:
                last_was_digit = True
            elif c in 'eE' and (ttype is Number.Float or ttype is Number) and i + 1 < end and txt[i + 1] in '-=0123456789':
                end, ignored = IntentLexer._handle_number(txt, i + 1, end, None)
                return end, Number.Float
            else:
                return i, ttype
            i += 1
        return end, ttype

    @staticmethod
    def _handle_quoted(txt, i, end):
        c = txt[i]
        stop = IntentLexer._end_of_quoted(txt, i + 1, end)
        if c == '"':
            ttype = String.Double
        else:
            if stop == i + 3:
                ttype = String.Char
            else:
                ttype = String.Single
        return stop, ttype

    @staticmethod
    def _end_of_whitespace(txt, i, end):
        while i < end:
            if txt[i] not in ' \t\r\n':
                return i
            i += 1
        return end

    @staticmethod
    def _end_of_spaces_and_tabs(txt, i, end):
        while i < end:
            if txt[i] not in ' \t':
                return i
            i += 1
        return end

    @staticmethod
    def _end_of_line(txt, i, end):
        '''Find end of line in txt.'''
        while i < end:
            c = txt[i]
            if c == '\n':
                return i
            elif c == '\r':
                if i + 1 < end and txt[i + 1] == '\n':
                    return i + 1
                return i
            i += 1
        return end

    @staticmethod
    def _at_soft_break(txt, i, end):
        return i <= end - 3 and txt[i] == '.' and txt[i + 1] == '.' and txt[i + 2] == '.'

    @staticmethod
    def _end_of_statement(txt, i, end):
        '''Find end of current statement in txt.'''
        nxt = i
        # Go to end of current line. Then peek to see if next line begins
        # with an ellipsis (soft line break). If yes, keep expanding boundaries
        # of statement. Otherwise, stop.
        while True:
            eol = IntentLexer._end_of_line(txt, nxt, end)
            if eol == end:
                break
            nxt = IntentLexer._end_of_spaces_and_tabs(txt, eol, end)
            if nxt == end or not IntentLexer._at_soft_break(txt, nxt, end):
                return eol
        return end

    @staticmethod
    def _end_of_soft_breakable_whitespace(txt, i, end):
        '''Find end of whitespace, including soft line breaks.'''
        while True:
            j = IntentLexer._end_of_spaces_and_tabs(txt, i, end)
            if j == end:
                return j
            c = txt[j]
            position = j
            if c == '\r':
                # If we don't have room for ellipsis
                if j > end - 4:
                    return j
                if txt[j + 1] == '\n':
                    position += 1
            if c == '\n':
                # Peek to see what comes next.
                nxt = IntentLexer._end_of_spaces_and_tabs(txt, position + 1, end)
                if nxt == end or not IntentLexer._at_soft_break(txt, nxt, end):
                    return j
            else:
                return j

    @staticmethod
    def _end_of_phrase(txt, i, end):
        while i < end:
            c = txt[i]
            # Alphanumerics, underscore, and extended chars can appear
            # freely in phrases. Other stuff has special rules that require
            # lookahead.
            if (not c.isalnum()) and (c != '_') and ord(c) < 128:
                # Consecutive whitespace gets collapsed to a single space; this
                # makes line wrapping much less messy to manage. Whitespace chars
                # can't be the final char of a phrase.
                if c in ' \t':
                    j = IntentLexer._end_of_soft_breakable_whitespace(txt, i, end)
                    if j + 1 >= end or not txt[j].isalnum():
                        return i
                    i = j
                # Hyphenated words are fine in phrases, but not at the end
                elif c == '-':
                    if i + 1 == end or not txt[i + 1].isalnum():
                        return i
                else:
                    return i
            i += 1
        return end

    @staticmethod
    def _end_of_digits(txt, i, end):
        '''
        Find end of a run of digits in txt.
        '''
        while i < end:
            if not txt[i].isdigit():
                return i
            i += 1
        return end

    @staticmethod
    def _end_of_quoted(txt, i, end):
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


if __name__ == '__main__':
    while True:
        #print('\nEnter some code.')
        #code = raw_input()
        with open('/Users/dhardman/code/intent/snippets/helper engine.i', 'r') as f:
            code = f.read()
        if not code:
            break
        lx = IntentLexer()
        for idx, ttype, val in lx.get_tokens_unprocessed(code):
            print('%04d %s "%s"' % (idx, ttype, val))
        break
