import argparse
import traceback

from ..lang import parse
from ..space import Space, find_space_for_path, walk_code_files
from .. import placeholders as ph
from .item import *


def _generate_doc(ast):

    pass


def _document(space, abspath, outpat):
    relpath = space.get_rel_path(abspath)
    if '[^@' in outpat:
        relfbase = relpath[:-2]
        outpat = outpat.replace(ph.exprs[ph.relfbase], relfbase)
        outpat = outpat.replace(ph.exprs[ph.relsym], relfbase.replace('/', '.'))
        outpat = outpat.replace(ph.exprs[ph.fbase], os.path.basename(relfbase))
    outpath = outpat + '.html'
    with ProcessedItem(relpath, space.get_rel_path(outpath)) as item:
        try:
            with open(abspath, 'rt') as f:
                text = f.read()
            ast = parse.parse_text(text, relpath)
            _generate_doc(ast)

        except:
            item.report_error('unhandled-exception', traceback.format_exc())


def get_arg_parser():
    p = []
    for i in range(len(ph.names)):
        p.append(ph.exprs[i] + ' (' + ph.docs[i] + ')')

    ap = argparse.ArgumentParser(prog='i doc', description='generate docs from source')
    ap.add_argument('--outpat', metavar='PATTERN', help='''
    Pattern for generating path of output files. May be a specific folder or
    filename, but may also include placeholders: %s. %s yields a flat folder of
    output, whereas %s puts output files in a folder hierarchy.
    ''' % ('; '.join(p), ph.exprs[ph.relsym], ph.exprs[ph.relfbase]), default=f"{ph.exprs[ph.space]}/{ph.exprs[ph.relfbase]}")
    ap.add_argument('--css', metavar='FILE', help='file that contains css to link')
    ap.add_argument('target', metavar='PATH', nargs='+', help='input file or folder')
    return ap


def main(args):
    args = get_arg_parser().parse_args(args)
    old_space_dir = None
    for target in args.target:
        space_dir = find_space_for_path(target)
        if old_space_dir != space_dir:
            outpat = args.outpat
            outpat = os.path.expanduser(outpat)
            outpat = outpat.replace(ph.exprs[ph.space], space_dir)
            space = Space(space_dir)
            if old_space_dir:
                print('')
            old_space_dir = space_dir
        if os.path.isdir(target):
            for item in walk_code_files(target):
                _document(space, item, outpat)
        else:
            _document(space, target)
