from ..lang import parse


def document(ast):
    pass


def main(args):
    """
    Generate documents from intent source.
    """
    codebase = []
    for arg in args:
        with open(arg, 'rt') as f:
            text = f.read()
        ast = parse(text)
        codebase.append(ast)
    for ast in codebase:
        document(ast)