#include "core/strutil.h"

#if 0
const int INTEGER = 1;
const int FLOAT = 2;
const int HEX = 3;
const int OCT = 4;
const int BINARY = 5;

const char
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

#endif
