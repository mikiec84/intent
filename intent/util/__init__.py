import sys


def _invoke(verb, args):
    import importlib
    module = importlib.import_module(f'.i_{verb}', __name__)
    module.main(args)


def main():
    try:
        args = sys.argv
        if len(args) >= 2:
            verb = args[1]
        else:
            verb = 'help'
        _invoke(verb, args[2:])
    except KeyboardInterrupt:
        sys.exit(1)