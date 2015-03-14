# The next line creates an anchor named "gather statistics"
$gather statistics

# Here is a hyperlink back to that line...
@gather statistics

# Hyperlinks can also use arbitrary urls and arbitary schemas...
@http://intentlang.org/why
@mailto://fred@flinstones.org
@file://C:/Users/Fred
@file:///proc/self/exe

# Hyperlinks can have parenthesized "friendly text":
@(app icon)file://../assets/app icon.png

# Hyperlinks end when their containing statement ends, or when the ; char is
... encountered followed by whitespace. This means you don't have to use
... unnatural encodings. Web urls can be pasted directly out of a browser's
... address bar, and file sysem paths can be used as-is, even if they contain
... spaces or other punctuation. Hyperlinks can be seamlessly line wrapped and
... unwrapped, without being broken.
@http://www.google.com/url?sa=t&rct=j&q=&esrc=s;&source=web&cd=3&cad=rja&uact=8&
....sqi=2&ved=0CCwQFjAC&url=http%3A%2F%2Fen.wikipedia.org%2Fwiki%2FEponym&ei=dAw
....EVd__FtLhoATJk4G4Bw&usg=AFQjCNHYl3xhQ1ZSuyecY_VHW5ReXWQc3Q&sig2=MOx70o6z-r1i
....urssWiVYmw&bvm=%75v.88198703,d.cGU
@file:///tmp/My Crazy Life: a Memoir
$tricky logic; x = sqrt(y + (z / 2)^-4)
# We can now refer to @trick logic; in comments (see how the ; came in handy,
... both in the anchor and in the hyperlink?)

# File system URLs resolve a leading "~" as the current user's home folder on
... *nix and windows.
@file://~/.config

# Hyperlinks that begin with | are assumed to use the "ispace" schema and
... are resolved relative to the root of the current code space. The ispace
... schema is a superset of the "file" schema that supports environment
... variables, logical/semantic paths (e.g., the decl of a function no matter
... which file it's in, etc).
@|/component A/in/x.i
@|

# Hyperlinks that begin with ./ or ../ are treated as file system paths, relative
... to the current file. You cannot link to files and folders in the same
... directory as the current file just by using their names--you must use either
... "file://" or "./" as a prefix--because file names and names of constructs
... within the code could be ambiguous.
@../assets/foo.ico
@./

# Any code structure that declares a name automatically creates an eponymous
... anchor. So the "if" statement below can be referred to as "@if enough time"
... or "@if we have enough time".
if we have enough time (remaining > min timeout):
    proceed

# Likewise, the definition of a variable or function is its anchor, so the
... next line can be hyperlinked with...
# @average rainfall".
average rainfall := Calculate mean(rainfall this month)
# @vehicle
vehicle: class
    # body of class
# @Calculate k nearest neighbor
Calculate k nearest neighbor:
    takes:
        # The next line defines a parameter named spread. It can be hyperlinked
        ... with @Calculate k nearest neighbor.spread
        - spread: float
    # body of function

# Besides where it's defined, many other appearances of a name establish anchors.
... For example, every line that contains an assignment to a variable is
... linkable using a URL of the form @<name>#<I>, where I is a
... zero-based number.
x := 25 # hyperlinkable as @x#0
x += 5 # @x#1
if (time of day > 1350):
    x %= 60 # @x#2
else:
    x++ # @x#3



