# The numeric value sof these tokens doesn't matter. They'll
# be reassigned to unique values just below.
BEGIN_DOC = 0
BEGIN_LINE = 0
BEGIN_PARA = 0
BEGIN_TERM = 0
BEGIN_INSTR = 0
INDENT = 0
DEDENT = 0
NAME = 0
TEXT = 0
BEGIN_ANCHOR = 0
BEGIN_ASIDE = 0
BEGIN_LINK = 0
BEGIN_QUOTED = 0
END_QUOTED = 0
END_HYPERTEXT = 0
PIVOT_TERM = 0
ARG_SEP = 0
HYPERTEXT_DIVIDER = 0
END_LINE = 0
END_DOC = 0

def load():
    g = globals()
    x = []
    for name, value in g.items():
        if isinstance(value, int):
            x.append(name)
    i = 0
    for item in x:
        g[item] = i
        i += 1
    del g['load']
    return x

names_by_value = load()

