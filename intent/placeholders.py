space = 0
fbase = 1
relfbase = 2
relsym = 3

names = [k for k in globals().keys() if isinstance(globals()[k], int)]

expressions = [f"[^@{name}]" for name in names]

docs = [
    'absolute path to root of space',
    'name of input file, minus ext: foo/bar.i --> bar',
    'path to input file, relative to root of the space, minus ext: foo/bar.i --> foo/bar',
    'symbol of input file, relative to root of the space: foo/bar.i --> foo.bar'
]