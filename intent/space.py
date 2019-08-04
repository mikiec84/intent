import os


OUTDIR_NAME = '.out'
ENVDIR_NAME = '.env'


def _normpath(path):
    path = os.path.expanduser(path)
    p = os.path.normpath(os.path.abspath(path)).replace('\\', '/')
    # Clean up concatenation errors
    while '//' in p:
        p = p.replace('//', '/')
    if p[-1] == '/':
        p = p[:-1]
    return p


class Space:
    def __init__(self, path):
        self._path = _normpath(path)

    @property
    def path(self):
        return self._path

    @property
    def name(self):
        return os.path.split(self._path)[1]

    @property
    def outdir(self):
        return os.path.join(self._path, OUTDIR_NAME)

    @property
    def envdir(self):
        return os.path.join(self._path, ENVDIR_NAME)

    def init(self):
        os.makedirs(self.outdir, exist_ok=True)
        os.makedirs(self.envdir, exist_ok=True)
        gi = os.path.join(self._path, '.gitignore')
        if not os.path.isfile(gi):
            with open(gi, 'wt') as f:
                f.write('.out\n.env\n')

    def get_rel_path(self, abspath):
        abspath = _normpath(abspath)
        return os.path.relpath(abspath, self.path)


def walk_code_files(root, topdown=True):
    for root, folders, files in os.walk(root, topdown=topdown):
        skipdirs = [x for x in folders if x.startswith('.')]
        for skipdir in skipdirs:
            folders.remove(skipdir)
        for f in files:
            if f.endswith('.i'):
                yield _normpath(os.path.join(root, f))


def find_space_for_path(path):
    path = _normpath(path)
    while path:
        if is_space(path):
            return path
        i = path.rfind('/')
        if i < 0:
            break
        path = path[:i]


def is_space(path):
    if os.path.isdir(path):
        return os.path.isdir(os.path.join(path, OUTDIR_NAME)) and os.path.isdir(os.path.join(path, ENVDIR_NAME))