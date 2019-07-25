import importlib
import os

def main(args):
    """
    Provide help for all intent commands.
    """
    for f in os.listdir(os.path.dirname(__file__)):
        if f.startswith('i_') and f.endswith('.py'):
            verb = f[2:-3]
            module = importlib.import_module(f'..i_{verb}', __name__)
            explanation = module.main.__doc__.strip()
            print(f'i {verb}: {explanation}')
    print('\nTry "i <verb> help" for details.')