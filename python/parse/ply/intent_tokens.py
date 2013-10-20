import lex

# Regular expression rules for simple tokens
t_PLUS    = r'\+'
t_MINUS   = r'-'
t_TIMES   = r'\*'
t_DIVIDE  = r'/'
t_LPAREN  = r'\('
t_RPAREN  = r'\)'
t_INDENT  = r'\x01'
t_DEDENT  = r'\x02'

# A regular expression rule with some action code
def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

def _scan_indents(lexer):
    '''
    At beginning of a new line, see how many indents it has. Each indent can
    potentially be a different width, but we should never mix spaces and tabs.
    Return how many new indents or dedents we see. The max new indent is 1;
    the max number of new dedents == current indent level. Dedents are returned
    as negative numbers (so to dedent 4 times, return -4).
    '''
    i = lexer.lexpos
    txt = lexer.lexdata
    end = lexer.lexlen
    current_indent_idx = len(lexer.indents) - 1

    more_lines = True
    start_over = False
    while more_lines:

        # Did we run out of characters?
        if i + 1 >= end:
            return

        # Sample first char on next line.
        c = txt[i]

        # If line's not indented, record necessary dedents and exit.
        if not c.isspace():
            lexer.indent_delta = -1 * (current_indent_idx + 1)
            return

        # If this is the first indented line, lock the entire lexing
        # pass to either tabs or spaces -- disallow mixture.
        if lexer.indent_char is None:
            lexer.indent_char = c

        # Now walk through each indenter that's currently active, and see
        # how many of them we see on this new line.
        indent_idx = 0
        while indent_idx <= current_indent_idx:
            width = lexer.indents[indent_idx]
            begin = lexer.lexpos
            indent_end = begin + width
            actual_end = min(indent_end, end)
            start_over = False

            j = begin
            while j < actual_end:
                c = txt[j]
                if c != lexer.indent_char:

                    # If we stopped processing an indented line because we hit another
                    # line break, just start the analysis all over.
                    if c == '\n':
                        lexer.lineno += 1
                        start_over = True
                        break

                    # If we found at least one indent char, but not all of the indent chars
                    # we expected, we definitely have a problem.
                    if j > begin:
                        if c.isspace():
                            raise Exception("Bad indent char %d on line %d." % (ord(c), lexer.lineno))
                        else:
                            raise Exception("Incomplete indent on line %d." % lexer.lineno)
                    break
                j += 1

            # If we get here, then we have consumed pure indent chars, until
            # either the indent was finished, a new linebreak was found, the
            # input ended, or we hit non-indent characters.

            # Consume everything we just scanned so lexer doesn't have to
            # reanalyze.
            lexer.lexpos = j

            # If we found a line break or ran out of indent characters without
            # throwing an exception, just exit this loop.
            if start_over or (c != lexer.indent_char):
                break

            # If we did not see the full indent that we were expecting, then the
            # input ended in the middle of a partial indent. This is not an error,
            # but it means our job is done.
            if actual_end < indent_end:
                return

            indent_idx += 1

        # If we found a line break, restart the outermost loop with the fresh line.
        if start_over:
            continue

        # If we get here, we've consumed all existing indents without exhausting
        # our input. Are we as indented as the previous line?
        if indent_idx <= current_indent_idx:

            # No. We need to emit 1 or more dedents.
            lexer.indent_delta = indent_idx - (current_indent_idx + 1)
            # No need to look at subsequent lines; we've found meaningful text on this one.
            more_lines = False

        else:

            # Yes. Are we more indented?
            for j in xrange(lexer.lexpos, end):
                c = txt[j]
                if c != lexer.indent_char:
                    if c == '\n':
                        lexer.lineno += 1
                        lexer.lexpos = j
                        start_over = True
                    elif c.isspace():
                        raise Exception("Bad indent char %d on line %d." % (ord(c), lexer.lineno))

                    break

            # If we found another new line, start all over again.
            if start_over:
                continue

            if j > lexer.lexpos:
                lexer.indents.append(j - lexer.lexpos)
                lexer.lexpos = j
                lexer.indent_delta = 1

            # We're done.
            more_lines = False


# Define a rule so we can track line numbers and track indents.
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)
    _scan_indents(t.lexer)



# A string containing ignored characters (spaces and tabs)
t_ignore  = ' \t\x01\x02'

# Error handling rule
def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)

import re
pat = re.compile('^t_[A-Z_]+$')
# List of token names.   This is always required
tokens = [name[2:] for name in locals().keys() if pat.match(name)]
del re

lexer = lex.lex()
lexer.indents = []
lexer.indent_char = None
lexer.indent_delta = 0

if __name__ == '__main__':
    while True:
        print('Enter some code, then ! to finish...')
        code = ''
        while True:
            line = raw_input()
            if line == '!':
                break
            code = code + line + '\n'
        for i in xrange(len(code)):
            c = code[i]
            if c == '\n':
                c = '\\n'
            elif c == ' ':
                c = 'space'
            print(str(i).rjust(3) + ' ' + c)
        lexer.input(code)
        while True:
            tok = lexer.token()
            if not tok:
                print('')
                break      # No more input
            print('type=%s, value=%s, line=%d, lexpos=%d' % (str(tok.type).rjust(7), str(tok.value).rjust(6), tok.lineno, tok.lexpos))
