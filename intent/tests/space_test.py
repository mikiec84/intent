import os
import tempfile

from ..space import *


this_folder = os.path.abspath(os.path.dirname(__file__))

def test_ctor():
    s = Space(os.path.join(this_folder, '..'))
    assert s.name == 'intent'
    assert '..' not in s.path
    assert s.outdir.endswith('/.out')


def test_init(scratchdir):
    s = Space(scratchdir.name)
    s.init()
    assert os.path.isdir(s.outdir)
    assert os.path.isdir(s.envdir)
    assert is_space(s.path)


def test_find_with_path_that_doesnt_exist(scratchdir):
    s = Space(scratchdir.name)
    s.init()
    subdir = os.path.join(s.path, 'a/b/c')
    path = find_space_for_path(subdir)
    assert path == s.path


def test_find_with_path_that_exists(scratchdir):
    s = Space(scratchdir.name)
    s.init()
    subdir = os.path.join(s.path, 'a/b/c')
    os.makedirs(subdir)
    os.chdir(subdir)
    path = find_space_for_path('..')
    assert s.name == Space(path).name
