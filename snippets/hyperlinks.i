# Hyperlinks are a core and pervasive feature of intent. For starters, any
# construct in intent code that has a name can be the referent (anchor) of a
# hyperlink. Suppose we have a class named "foo":
class foo:
    pass

# We can hyperlink back to that class...
@foo

# We can also create explicit "anchors" using the $ operator:
$gather statistics

# And we can hyperlink back to them:
@gather statistics

# Hyperlinks can use arbitrary uris and arbitary schemas...
@http://intentlang.org/why
@mailto://fred@flinstones.org
@file://C:/Users/Fred
@file:///proc/self/exe

# Hyperlinks can have parenthesized attributes, such as "friendly text", and
#"rel" and "target" attributes as in html. The parens precede the referent
#of the hyperlink, and are subdivided by |. All attributes are optional, but
#they must appear in friendly|rel|target order. Here, the friendly text is
#"app icon", and there is no rel or target:
@(app icon)file://../assets/app icon.png

# Where hyperlinks end depends on their schema. The file schema and the ispace
#schema both support embedded spaces and are terminated by the ; char, whereas
#http, https, mailto, and most other schemas are terminated by a space. The
#practical consequence is that you don't have to use unnatural encodings to
#embed a hyperlink in intent code. Web urls can be pasted directly out of a
#browser's address bar, and so can file sysem paths or the names of classes
#or functions in intent. Also, hyperlinks can be seamlessly line wrapped and
#unwrapped, without being broken.
@http://www.google.com/url?sa=t&rct=j&q=&esrc=s;&source=web&cd=3&cad=rja&uact=8&
....sqi=2&ved=0CCwQFjAC&url=http%3A%2F%2Fen.wikipedia.org%2Fwiki%2FEponym&ei=dAw
....EVd__FtLhoATJk4G4Bw&usg=AFQjCNHYl3xhQ1ZSuyecY_VHW5ReXWQc3Q&sig2=MOx70o6z-r1i
....urssWiVYmw&bvm=%75v.88198703,d.cGU
@file:///tmp/My Crazy Life: a Memoir
$tricky logic; x = sqrt(y + (z / 2)^-4)
# We can now refer to @tricky logic; in comments (see how the ; came in handy,
#both in the anchor and in the hyperlink?)

# Hyperlinks that begin with ./ or ../ are treated as file system paths, relative
#to the current file. You cannot link to files and folders in the same
#directory as the current file just by using their names (you must use "./"
#or "../" or "file://" as a prefix) because file names and names of constructs
#within the code could overlap and therefore be ambiguous.
@../assets/foo.ico
@./

# File system URLs resolve a leading "~" as the current user's home folder on
#*nix and windows.
@file://~/.config

# URLs that begin with "~" have the same effect:
@~/.config

# Hyperlinks that don't begin with an explicit schema (xyz://...) use the
#"ispace" schema by implication. This schema maps code constructs such as
#packages, modules, and classes onto a file system (or similar container) in
#a way that superficially resembles java's mapping of packages onto folders.
#It also supports environment variables and some other important semantic
#enhancements such as cross-cutting "aspects", and reconciling a logical path
#against a prioritized set of physical possibilities. A full discussion how
#intent's spaces are organized is beyond the scope of these brief examples;
#see @http://intentlang.org/spec/spaces/ for more details.

# URLs in the ispace schema can be local to the current module, relative to
#the space, or fully qualified. A local ispace URL begins with an alpha:
@my class.my method

# A space-relative ispace URL begins with the | char, and specifies the component
#and namespace(s) to traverse, from the root of the space, to reach the
#referent of interest:
@|traffic.routing devices.stoplight

# A fully qualified ispace URL must begin with the ispace:// schema. It consists
#of an embedded URL that locates the space, followed by the | char, followed
#by the familiar component/namespace section. If the embedded URL contains
#a | char, it must be %-encoded.
@ispace://git@github.com:dhh1128/intent.git|traffic.routing devices.stoplight

# Any code structure that declares a name automatically creates an eponymous
#anchor. Any variant of its name that's unambiguous is a valid hyperlink, so
#the "if" statement below can be referred to with "@If enough time" or
#"@If we have enough time".
If we have enough time (remaining > min timeout):
    pass

# Likewise, the definition of a variable or function is its anchor, so the
#next line can be hyperlinked with...
# @average rainfall".
average rainfall := Calculate mean(rainfall this month)
# @vehicle
vehicle: class
    # body of class
# @Calculate k nearest neighbor
Calculate k nearest neighbor:
    takes:
        # The next line defines a parameter named spread. It can be hyperlinked
        #with @Calculate k nearest neighbor.spread
        - spread: float
    # body of function

# Besides where it's defined, many other appearances of a name establish anchors.
#For example, every line that contains an assignment to a variable is
#linkable using a URL of the form @<name>#<I>, where I is a
#zero-based number.
x := 25 # hyperlinkable as @x#0
x += 5 # @x#1
if (time of day > 1350):
    x %= 60 # @x#2
else:
    x++ # @x#3

