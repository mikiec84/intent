import argparse
import traceback

from ..lang import parse
from ..space import Space, find_space_for_path, walk_code_files
from ..placeholders import expressions as pexprs, docs as pdocs, names as pnames, space as pspace, relsym, relfbase
from .item import *


def _generate_doc(ast):
    pass


def _document(space, abs_path):
    rel_path = space.get_rel_path(abs_path)
    item = PrettyProcessedItem(rel_path)
    try:
        try:
            with open(abs_path, 'rt') as f:
                text = f.read()
            ast = parse.parse_text(text, rel_path)
            _generate_doc(ast)
        except:
            item.report_error('unhandled-exception', traceback.format_exc())
    finally:
        item.finish()


def get_arg_parser():
    p = []
    for i in range(len(pnames)):
        p.append(pexprs[i] + ' (' + pdocs[i] + ')')

    ap = argparse.ArgumentParser(prog='i doc', description='generate docs from source')
    ap.add_argument('--outpat', metavar='PATTERN', help='''
    Pattern for generating path of output files. May be a specific folder or
    filename, but may also include placeholders: %s. %s yields a flat folder of
    output, whereas %s puts output files in a folder hierarchy.
    ''' % ('; '.join(p), pexprs[relsym], pexprs[relfbase]), default=f"{pexprs[pspace]}/{pexprs[relfbase]}")
    ap.add_argument('--css', metavar='FILE', help='file that contains css to link')
    ap.add_argument('target', metavar='PATH', nargs='+', help='input file or folder')
    return ap


def main(args):
    args = get_arg_parser().parse_args(args)
    old_space_dir = None
    for target in args.target:
        space_dir = find_space_for_path(target)
        if old_space_dir != space_dir:
            outpath = args.outpat
            outpath = os.path.expanduser(outpath)
            outpath = outpath.replace(pexprs[pspace], space_dir)
            space = Space(space_dir)
            if old_space_dir:
                print('')
            old_space_dir = space_dir
            #print('# `i doc` in space: ' + space_dir)
        if os.path.isdir(target):
            for item in walk_code_files(target):
                _document(space, item)
        else:
            _document(space, target)
