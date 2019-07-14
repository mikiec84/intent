import re
from pygments.lexer import Lexer
from pygments.token import Token, Comment, Name, String, Number, Punctuation, Error, Keyword, Text

# Although Intent has a context-free grammar, it is much easier to lex with lookaheads
# and state. Pygment's lexers are certainly capable of being stateful, but the input
# to pygments might be a small gist rather than a whole compilation unit, which means
# we might be starting in a state that's difficult to derive without a full-blown
# state machine. Therefore, we're not attempting the same perfection in lexing, here,
# that we'd need if we were feeding a parser. Also, we are not deriving from RegexLexer
# because it's easier and faster to test certain constructions without regex.
class IntentLexer(Lexer):
    """
    For `Intent <http://intentlang.org>`_ source code.
    """
    name = 'Intent'
    aliases = ['intent']
    filenames = ['*.i', '*.I']
    mimetypes = ['text/x-intent']
    # Higher than swig, lower than C/C++
    priority = 0.09

    # Swig also uses the *.i extension. Therefore we need to do some scanning to
    # decide whether a *.i file is likely to be intent code or a swig module. This
    # scanning is helpful anyway, in case other languages get added that also
    # use *.i.

    # These regexes are very typical of swig interfaces.
    swig_module_pat = re.compile(r'^\s*%module', re.MULTILINE)
    swig_other_directive_pat = re.compile(r'^\s*%[a-z]+', re.MULTILINE)
    swig_curlies_pat = re.compile(r'%\{.*?%\}', re.DOTALL)

    # These regexes are very typical of intent code.
    define_pat = re.compile(r'^\s*[a-zA-Z _]+\s*:', re.MULTILINE)
    soft_line_break_pat = re.compile(r'^\s*\.\.\.', re.MULTILINE)
    marks_in_assignment_pat = re.compile(':\s+[-+][a-z]+')

    def analyse_text(txt):
        try:
            # Because it's possible for swig templates to contain intent code, we need to
            # eliminate the possibility of swig first. The following swig markers should
            # virtually never occur in intent code.
            if (IntentLexer.swig_module_pat.search(txt) or
                IntentLexer.swig_other_directive_pat.search(txt)):
                return 0.0
            line_count = 1.0 + txt.count('\n')
            # A smoking gun for intent code is its unique line wrap strategy. If there
            # are any soft line breaks, it's a good indicator. However, these may not
            # show up very often...
            weight = 7
            count = len(IntentLexer.soft_line_break_pat.findall(txt))
            probability = min(1.0, weight * (count / line_count))
            if probability < 1.0:
                # The define pattern that intent uses to declare variables, functions, and
                # classes is very common; most code samples show it on at least 10% of all
                # lines. If we see it on 10% of the lines, we want to be 50% sure we're
                # looking at intent code.
                weight = 5
                count = len(IntentLexer.define_pat.findall(txt))
                ratio = count / line_count
                # If we see a really low ratio of this, and our sample size is at all
                # significant, be less confident.
                if line_count > 2 and ratio < 0.33:
                    probability /= 2
                else:
                    probability = min(1.0, probability + (weight * (count / line_count)))
                if probability < 1.0:
                    # Marks on definitions in lect code are also fairly distinctive. We'd expect
                    # these on 5-20% of all lines.
                    weight = 4
                    count = len(IntentLexer.marks_in_assignment_pat.findall(txt))
                    probability = min(1.0, probability + (weight * (count / line_count)))
            return probability
        except:
            import traceback
            traceback.print_exc()

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
                start_statement = txt.find('\n', begin, end) > -1 or txt.find('\r', begin, end) > -1
            elif c in '#|':
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
                    if nxt.isalpha() or nxt in '+-':
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
            elif c in '*/%&^\\[],=():.<>{}?':
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
                    start_statement = False
                    ttype = Name.Function
                    end = IntentLexer._end_of_phrase(txt, begin + 1, end_of_string)
                    phrase = txt[begin:end]
                    if phrase.isupper():
                        ttype = Name.Constant
                elif start_statement:
                    for tuple in IntentLexer.handle_first_word(txt, begin, end_of_string):
                        yield tuple
                        begin += len(tuple[2])
                    start_statement = False
                    continue
                else:
                    end = IntentLexer._end_of_phrase(txt, begin + 1, end_of_string)
                    ttype = Name.Variable
            else:
                ttype = Error
                end = IntentLexer._end_of_line(txt, begin + 1, end_of_string)
                start_statement = True
            if ttype is None:
                break
            yield (begin, ttype, txt[begin:end])
            begin = end

    block_start_pat = re.compile(r'(if|for|with|while|try|catch|handle|when|else|lock|use)(\s+[^:]+):')
    end_pat = re.compile('end(\\s+[^\r\n]+)$', re.MULTILINE)

    @staticmethod
    def handle_first_word(txt, i, end_of_string):
        end = i
        end_of_statement = IntentLexer._end_of_statement(txt, i, end_of_string)
        # Is the first word in this statement one of the reserved statement starters?
        m = IntentLexer.block_start_pat.match(txt, i, end_of_statement)
        if m:
            yield (i, Keyword.Reserved, txt[i:m.end(1)])
            # Is this block "explained" with an inline comment?
            j = txt.find('(', m.end(1), end_of_statement)
            if j > -1:
                k = txt.find(')', j + 1, end_of_statement)
                if k == -1:
                    yield(m.end(1), Error, txt[m.end(1):end_of_statement])
                else:
                    yield(m.end(1), Comment, txt[m.end(1):j])
        else:
            m = IntentLexer.end_pat.match(txt, i, end_of_statement)
            if m:
                yield(i, Keyword.Reserved, txt[i:i+3])
                if m.group(1):
                    yield(m.start(1), Comment, txt[m.start(1):m.end(1)])
                    end = m.end(1)
            else:
                end = IntentLexer._end_of_phrase(txt, i + 1, end_of_string)
                yield(i, Name.Variable, txt[i:end])

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
            nxt = IntentLexer._end_of_spaces_and_tabs(txt, eol + 1, end)
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
        with open('/tmp/x.i', 'r') as f:
            code = f.read()
        if not code:
            break
        lx = IntentLexer()
        for idx, ttype, val in lx.get_tokens_unprocessed(code):
            print('%04d %s "%s"' % (idx, ttype, val))
        break
