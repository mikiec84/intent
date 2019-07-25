import argparse
import os

from ..space import Space


def init(folder):
    folder = os.path.normpath(folder)
    os.makedirs()


def main(args):
    """
    Init a space.
    """
    parser = argparse.ArgumentParser(description='Initialize a space.')
    parser.add_argument('root', metavar='FOLDER', type=str, help='root folder for the space')
    args = parser.parse_args(args)
    space = Space(args.root)
    space.init()