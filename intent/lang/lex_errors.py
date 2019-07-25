NO_CLOSE_BRACKET = "Expected a ] to match [."

def load():
    g = globals()
    codes = []
    msgs = []
    for name, value in g.items():
        if isinstance(value, str):
            codes.append(name)
            msgs.append(value)
    i = 0
    for item in codes:
        g[item] = i
        i += 1
    del g['load']
    return codes, msgs

codes, msgs = load()
