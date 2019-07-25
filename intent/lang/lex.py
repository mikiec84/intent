from .lex_state import *
from .tok_types import *
from .tok import Token
from .lex_errors import *


_SKIPPABLE_WHITESPACE = ' \t\r'


def _fragment_is_next(text, i, end, fragment):
    token_end = len(fragment)
    j = 0
    while i < end and j < token_end:
        if text[i] != fragment[j]:
            return False
    return j == token_end


def _skip_whitespace(text, i, end):
    while i < end and text[i] in _SKIPPABLE_WHITESPACE:
        i += 1
    return i


def _line(ls: LexState):
    """
    Called for every line in the doc. This means it's called every time we are at beginning
    of a doc or have just finished processing a \n that terminated the previous line. Emits
    tokens for any lines that contain something other than whitespace.
    """
    ls.push(NEW_LINE)
    ls.line_start = ls.i
    try:
        # For brevity and maybe performance, get local variables from ls
        end, text, old_indent = ls.end, ls.text, ls.indent
        indents = []
        while ls.i < end:
            c = text[ls.i]
            if c == '\t':
                indents.append(Token(ls, INDENT, ls.i))
                ls.i += 1
            elif _fragment_is_next(text, ls.i, end, '    '):
                indents.append(Token(ls, INDENT, ls.i, ls.i + 4))
                ls.i += 4
            else:
                # Consume any leading whitespace that's not a full indent.
                ls.i = _skip_whitespace(text, ls.i, end)
                if ls.i >= end:
                    break
                c = ls.text[ls.i]
                if c == '\n':
                    break
                # Line contained something other than whitespace. Account for INDENT
                # or DEDENT, and set ls.indent to detected value.
                n = len(indents)
                while old_indent > n:
                    yield Token(ls, DEDENT, ls.i, ls.i) # token len = 0
                    old_indent -= 1
                while old_indent < n:
                    yield indents.pop(0)
                    n -= 1
                ls.indent = n

                if c == '-':
                    yield Token(ls, BEGIN_TERM, ls.i)
                    ls.i = _skip_whitespace(text, ls.i + 1, end)
                    if ls.i >= end:
                        break
                    # Now we should be in a term
                    for token in _term(ls):
                        yield token
                else:
                    yield Token(ls, BEGIN_PARA, ls.i, ls.i)
                    for token in _paragraph(ls):
                        yield token
                break
        # Handle the line break, if there is one.
        if ls.i < end and ls.text[ls.i] == '\n':
            yield Token(ls, END_LINE, ls.i)
            ls.i += 1

    finally:
        ls.line_num += 1
        ls.pop()


def _term(ls: LexState):
    ls.push(TERM)
    try:
        # Consume the name portion of the term, including leading and trailing whitespace.
        for token in _name(ls):
            yield token
        # If we've got more to consume...
        if ls.i < ls.end:
            c = ls.text[ls.i]
            if c == ':':
                yield Token(ls, PIVOT_TERM, ls.i, ls.i+1)
                ls.i = _skip_whitespace(ls.text, ls.i + 1, ls.end)
                if ls.i < ls.end:
                    if ls.text[ls.i] == '\n':
                        return
                    for token in _def(ls):
                        yield token
    finally:
        ls.pop()


def _break_on_any(text, any, i, end, trim_trailing_whitespace=True):
    begin = _skip_whitespace(text, i, end)
    if begin == end:
        return begin, end, end
    i = begin
    last_non_whitespace = -1
    while i < end:
        c = text[i]
        if c in any:
            break
        elif c not in _SKIPPABLE_WHITESPACE:
            last_non_whitespace = i
        i += 1
    return begin, max(last_non_whitespace + 1, begin), i, c if i < end else None


def _def(ls: LexState):
    ls.push(DESCRIPTOR)
    try:
        begin, end, ls.i, c = _break_on_any(ls.text, '\n', ls.i, ls.end)
        yield Token(ls, TEXT, begin, end)
    finally:
        ls.pop()


def _name(ls: LexState):
    ls.push(NAME)
    try:
        begin, end, ls.i, c = _break_on_any(ls.text, ':;\n', ls.i, ls.end)
        yield Token(ls, NAME, begin, end)
    finally:
        ls.pop()


def _paragraph(ls: LexState):
    ls.push(PARAGRAPH)
    try:
        while True:
            begin, end, ls.i, c = _break_on_any(ls.text, '[\n', ls.i, ls.end)
            if begin < end:
                yield Token(ls, TEXT, begin, end if c != '[' else ls.i)
            if ls.i >= ls.end or c == '\n':
                return
            # If we get here, we found a [
            for token in _hypertext(ls):
                yield token
            if ls.i >= ls.end:
                break
    finally:
        ls.pop()


def _lex_error(ls, code):
    etok = Token(ls, LEX_ERROR, ls.i, ls.i)
    etok.code = codes[code]
    etok.msg = msgs[code]
    remainder = None
    begin = ls.i
    end = ls.text.find('\n', ls.i)
    if end > begin:
        remainder = Token(ls, UNLEXED, begin, end)
    return etok, remainder


def _hypertext(ls: LexState):
    ecode = NO_CLOSE_BRACKET
    ls.push(HYPERTEXT)
    try:
        c = '' if ls.i + 1 >= ls.end else ls.text[ls.i]
        n = 2
        breakers = '[;|]'
        if c == '@':
            tt = BEGIN_LINK
        elif c == '^':
            tt = BEGIN_ASIDE
        else:
            tt = BEGIN_ANCHOR
            n = 1
            breakers = '[|]'
        # Yield the token that starts this hypertext section.
        yield Token(ls, tt, ls.i, ls.i + n)
        ls.i += n
        while ls.i < ls.end:
            # Look for the end of the first token or the end of the hypertext section itself.
            begin, end, ls.i, c = _break_on_any(ls.text, breakers, ls.i, ls.end)
            if begin < end:
                yield Token(ls, TEXT, begin, end)
            # More text to consume?
            if ls.i < ls.end:
                # The divider?
                if c == '|':
                    yield Token(ls, HYPERTEXT_DIVIDER, ls.i, ls.i + 1)
                    ls.i += 2
                    if ls.i < ls.end:
                        begin, end, ls.i, c = _break_on_any(ls.text, ']', ls.i, ls.end)
                        if begin < end:
                            yield Token(ls, TEXT, begin, end)
                        if c == ']':
                            yield Token(ls, END_HYPERTEXT, begin, end)
                            ecode = None
                        break
                elif c == ';':
                    ecode = None
                    raise NotImplemented
                elif c == '[':
                    # Nested hypertext of some kind. Recurse.
                    for token in _hypertext(ls):
                        yield token
                else:
                    assert c == ']'
                    ecode = None
                    yield Token(ls, END_HYPERTEXT, ls.i, ls.i + 1)
                    ls.i += 1
                    break
    finally:
        if ecode is not None:
            etok, remainder = _lex_error(ls, ecode)
            yield etok
            if remainder:
                yield remainder
        ls.pop()


def lex(text):
    ls = LexState(text)
    yield Token(ls, BEGIN_DOC, 0, 0)
    while ls.i < ls.end:
        for token in _line(ls):
            yield token
    yield Token(ls, END_DOC, ls.end, ls.end)
