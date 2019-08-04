import argparse
import os

from ..space import Space


def get_arg_parser():
    ap = argparse.ArgumentParser(description='initialize a space')
    ap.add_argument('root', metavar='FOLDER', type=str, help='root folder for the space')
    return ap


def main(args):
    args = get_arg_parser().parse_args(args)
    space = Space(args.root)
    space.init()