# Marks are words or phrases that describe some enriching semantics the compiler
# and coders ought to know about. They are prefixed by punctuation that tells
# whether the semantics are being affirmed or denied.

# The most common place for marks is in a definition, after the colon and data
# type. Here, our mark, -<nullable, says that a variable named "cheapest
# property", of type house, is not allowed to be null.
cheapest property: house -<nullable

# The punctuation at the beginning of a mark is called its "claim". In the
# example above, -<nullable makes a negative claim about nullability. The
# positive counterpart, +<, is more common. For example, to mark a class as
# thread-safe, you could use:
foo: class +<threadsafe

# If a definition includes an assignment, it comes after the mark:
cheapest property: house -<nullable = Find best deal()

# If a mark implies a datatype, then the mark may be used standalone:
class vehicle:
    properties:
        # Here, the "name" mark implies a short, single-line string.
        - make: +<name

# We've seen normal positive and negative claims with marks. Besides +< and -<,
# there are two additional claims; ++< and --< are used for explicit overrides.
# For example, if most of a class's methods are thread-safe, but one is not,
# you might use a normal positive claim and a negative override claim:
foo: class +<threadsafe
    Do something not parallelizable: --<threadsafe
        # function body

# Usually, marks play the same sort of role as adjectives in natural language.
# However, they can also be used as adverbs. Notice +<backward here:
Count down for launch:
    do:
        loop +<backward (countdown: 10):
            Say(Text to speech(To string(countdown)))

# Or how about an optimization hint on the "If" keyword:
If +<unlikely (size < 100 \kb):
    # do something to handle the corner case

# By convention, the claim prefix for a mark is glued onto the name that follows
# it. However, the following two lines are equivalent:
x: +<y = z
x: +< y = z

# Names of mark are lower-case words or phrases. Their position usually allows
# them to be delimited by the end of a statement or by operators. However,
# sometimes it is helpful to delimit the end of a mark with the ; character.
# What if the "if" statement in the example above had an informal name? We'd
# need the ; to clarify:
if +<unlikely; we got a super small file (size < 100 \kb):
    # do something to handle the corner case

# Multiple marks can be delimited with their claim prefix:
x: foo +<a +<b +<c -<d ++<e = Get default()
