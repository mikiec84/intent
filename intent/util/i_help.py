import argparse
import importlib
import os


def get_arg_parser():
    ap = argparse.ArgumentParser(prog='i help', description='provide help for all intent commands')
    return ap


def main(args):
    for f in os.listdir(os.path.dirname(__file__)):
        if f.startswith('i_') and f.endswith('.py'):
            verb = f[2:-3]
            module = importlib.import_module(f'..i_{verb}', __name__)
            descrip = module.get_arg_parser().description
            print(f'i {verb.ljust(8)} - {descrip}')
    print('\nTry "i <verb> -h" for details.')