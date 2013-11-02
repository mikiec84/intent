# So-called q-operators in intent are normal operators prefixed with
... a question mark, making them tolerant to a null variable value.

# Groovy's safe navigation operator (?.) is a q-operator in intent.
... When used, it instructs the compiler to attempt to traverse
... to the next level of a namespace. If it cannot because the
... current level is null, then it forces the nearest assignment to
... the left of the operator to be null.

# Assigns null to my_name if myobj is null, subobj is null, or property
... is null; otherwise, assigns the value of myobj.subobj.property.name.
... Since the data type of the final .name property is known to the 
... compiler, my_name is a null str, not just an undefined null.
my_name := myobj?.subobj?.property?.name

# The safe subscript operator tolerates subscripting on a null container:
my_name := names?[0]

# The safe empty operator tolerates subscriting on an empty container:
my_name := names[?0]

# They may be combined:
my_name := names?[?0]

# The safe grow operator appends to a collection, but handles the case
... where the collection is null by automatically constructing and
... initializing an empty container the first time an assignment
... is made.

my_list := list of str
for i in [1..10]:
    my_list ?+= str(i)

# The safe shrink operator is analogous, but removes from a collection without
... error, even if the collection is null.
my_list ?-= value

# The elvis operator is a convenient collapse of the ternary operator,
... just as in groovy. It gets its name from the way a sideways
... view of the operator looks like an emoticon with big hair.
... The following is equivalent to "return a ? a : "default":
return a ?: "default"

# The spaceship operator also comes from groovy. It returns -1 if less, 0
... if equal, and 1 if greater. Its name comes from its flying-saucer
... appearance.

return rhs <=> lhs

# It may be combined with the elvis operator to handle nulls in a slick
... way. This statement treats null strings as the empty string for
... purposes of comparison.

return (rhs ?: "") <=> (lhs ?: "")

# If you want nulls to be tolerated in comparison, but you want nulls
... to sort different from empty values, prefix or suffix the spaceship
... operator with a question mark, depending on whether nulls go first
... or last. This is known as "q-spaceship" (for prefix mode) or
... "spaceship-q" (for suffix)

# q-spaceship, with nulls tolerated and sorting first
return rhs ?<=> lhs

# spaceship-q, with nulls tolerated and sorting last
return rhs <=>? lhs

# The spread operator works on containers just like in groovy:
assert my_pets*.name == ['fido', 'whiskers']
my_pets*.get shots()

# The regex find operator is ~=
hit := /[a-z]+ \d/ ~= my_phrase

# The regex match operator is ~==
match := /[a-z]+ \d/ ~== my_phrase

# The membership operator is "in"
if name in friends:
    say("hello")

# There is no need to worry about a safe version of "in"; membership in
... null is always false.
friends := list of +name
# No error here...
if her name in friends:
    say("hello")

# The dynamic cast operator is "->". It is analogous to the "as" operator
... in some other languages. It returns null if the cast fails.
func := scope->func

# You can combine dynamic cast with ? to seamlessly tolerate null:
my_name := caller->func?.takes

# The link operator creates a reference to another place in code:
my_link := @traffic light.enter maintenance mode.takes

# The expand operator evals the referenced code, either copying
... it or running it.
my_params := *(traffic light.enter maintenance mode.takes)


