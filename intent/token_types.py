END = -1
NAME = 1
TEXT = 2
INDENT = 3
DEDENT = 4
EOL = 5
BEGIN_ANCHOR = '['
END_ANCHOR = ']'
BEGIN_ASIDE = '[^'
BEGIN_LINK = '[@'
END_HYPERTEXT = ']'
DEFINER = ':'
ARG_SEP = ';'
STAR = '*'
HYPERTEXT_DIVIDER = 6

def load():
    g = globals()
    x = {}
    for name, value in g.items():
        if isinstance(value, str) or isinstance(value, int):
            x[value] = name
    del g['load']
    return x

names_by_value = load()

