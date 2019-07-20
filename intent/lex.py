from .lex_state import *
from .tok_types import *
from .tok import Token


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


def _line(ls: LexState, code=True):
    ls.push(NEW_LINE)
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
                # Did line end?
                if c == '\n':
                    yield Token(ls, EOL, ls.i)
                    ls.i += 1
                    ls.line_num += 1
                    # We could stay in this function, since we're now starting a new line
                    # all over again. However, the function's contract is to yield all the
                    # tokens on a single line. So exit the function and we'll be called again.
                    break
                else:
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
                        yield Token(ls, TERM_START, ls.i)
                        ls.i = _skip_whitespace(text, ls.i + 1, end)
                        if ls.i >= end:
                            break
                        # Now we should be in a header
                        for token in _term(ls):
                            yield token
                    else:
                        for token in _paragraph(ls):
                            yield token
                    break
    finally:
        ls.pop()


def _term(ls: LexState):
    ls.push(TERM)
    try:
        # Consume the name portion of the header, including leading and trailing whitespace.
        for token in _name(ls):
            yield token
        # If we've got more to consume...
        if ls.i < ls.end:
            c = ls.text[ls.i]
            if c == ':':
                yield Token(ls, TERM_PIVOT, ls.i, ls.i+1)
                ls.i = _skip_whitespace(ls.text, ls.i + 1, ls.end)
                if ls.i < ls.end:
                    if ls.text[ls.i] == '\n':
                        return
                    for token in _def(ls):
                        yield token
    finally:
        ls.pop()


def _break_on_any(text, any, i, end):
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
    return begin, max(last_non_whitespace + 1, begin), i


def _def(ls: LexState):
    ls.push(DESCRIPTOR)
    try:
        begin, end, ls.i = _break_on_any(ls.text, '\n', ls.i, ls.end)
        if begin < end:
            yield Token(ls, TEXT, begin, end)
    finally:
        ls.pop()


def _name(ls: LexState):
    ls.push(NAME)
    try:
        begin, end, ls.i = _break_on_any(ls.text, ':;\n', ls.i, ls.end)
        if begin < end:
            yield Token(ls, NAME, begin, end)
    finally:
        ls.pop()


def _paragraph(ls: LexState):
    ls.push(PARAGRAPH)
    try:
        while True:
            begin, end, ls.i = _break_on_any(ls.text, '[\n', ls.i, ls.end)
            if begin < end:
                yield Token(ls, TEXT, begin, end)
            if ls.i >= end or ls.text[ls.i] == '\n':
                return
            # If we get here, we found a [
            for token in _hypertext(ls):
                yield token
            if ls.i >= ls.end:
                break
    finally:
        ls.pop()


def _hypertext(ls: LexState):
    ls.push(HYPERTEXT)
    try:
        # Get char after [, if any
        c = ls.text[ls.i + 1:ls.i + 2]
        if not c:
            return
        n = 2
        breakers = ';|]'
        if c == '@':
            tt = BEGIN_LINK
        elif c == '^':
            tt = BEGIN_ASIDE
        else:
            tt = BEGIN_ANCHOR
            n = 1
            breakers = '|]'
        yield Token(ls, tt, ls.i, ls.i + n)
        ls.i += n
        while True:
            ls.i = _skip_whitespace(ls.text, ls.i, ls.end)
            begin, end, ls.i = _break_on_any(ls.text, breakers, ls.i, ls.end)
            if begin < end:
                yield Token(ls, TEXT, begin, end)
            if ls.i < ls.end:
                c = ls.text[ls.i]
                if c == '|':
                    yield Token(ls, TEXT, ls.i, ls.i + 1)
                    ls.i = _skip_whitespace(ls.text, ls.i + 2, ls.end)
                    begin, end, ls.i = _break_on_any(ls.text, ']', ls.i, ls.end)
                    yield Token(ls, END_HYPERTEXT, begin, end)
                elif c == ';':
                    raise NotImplemented
                else:
                    yield Token(ls, END_HYPERTEXT, ls.i, ls.i + 1)
    finally:
        ls.pop()



def lex(text, code=True):
    ls = LexState(text)
    while ls.i < ls.end:
        for token in _line(ls, code):
            yield token
    yield Token(ls, END, ls.end, ls.end)
