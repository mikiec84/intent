import os

# In order to keep this codebase as clean as possible, we want to be very picky
# about warnings. We don't turn on *all* of them, but we turn on most. (An
# example of one we don't care about is -Wdouble-promotion, that warns about
# using a double when a float might suffice. That only matters with 32-bit
# CPUs.) This list is close to the combination of -Wall and -Wextra, but not
# identical.

warnings = '''
-Waddress
-Warray-bounds
-Wc++11-compat
-Wchar-subscripts
-Wchar-subscripts
-Wcomment
-Wcomment
-Wempty-body
-Wenum-compare
-Wformat
-Wformat
-Wignored-qualifiers
-Wmain
-Wmissing-braces
-Wmissing-field-initializers
-Wnonnull
-Wparentheses
-Wreorder
-Wreturn-type
-Wsequence-point
-Wsign-compare
-Wsign-compare
-Wstrict-aliasing
-Wstrict-overflow=1
-Wswitch
-Wtrigraphs
-Wtype-limits
-Wuninitialized
-Wuninitialized
-Wunknown-pragmas
-Wunused-function
-Wunused-label
-Wunused-parameter
-Wunused-value
-Wunused-variable
-Wvolatile-register-var'''

warnings = ' '.join(w.strip() for w in warnings.strip().split('\n'))

_flags = "-D_FILE_OFFSET_BITS=64 " + warnings + ' '

def ccflags(release):
	if release:
		return _flags + '-O3'
	return _flags + '-g'

_my_folder = os.path.dirname(os.path.abspath(__file__))
def list_buildable_subdirs(folder):
	vsd = os.listdir(folder)
	vsd = [x for x in vsd if not x.startswith('.')]
	vsd = [x for x in vsd if os.path.isfile(os.path.join(folder, x, 'sconscript'))]
	return vsd

external_projects = list_buildable_subdirs(os.path.join(_my_folder, 'src', 'external'))
components = list_buildable_subdirs(os.path.join(_my_folder, 'src'))
# Make sure the "i" component comes last (dependent on others)
components.remove('i')
components.append('i')
tests = list_buildable_subdirs(os.path.join(_my_folder, 'test'))

def check_config(config):
	# Do some very simple sanity checking on tools. Prove that we have a compiler
	# that's able to deal with C++ 11 constructs (typically, g++ 4.6 or later).
    #    if not config.CheckCXX():
    #            print('C++ compiler unavailable.')
    #            Exit(1)

	required_headers = ['unordered_map', 'cstdint']
	missing = []
	for rh in required_headers:
		if not config.CheckHeader(rh, language='C++'):
			missing.append(rh)
	if missing:
		print('The following headers are unavailable, but required to compile: %s' %
			', '.join(['<%s>' % h for h in missing]))
		Exit(1)
