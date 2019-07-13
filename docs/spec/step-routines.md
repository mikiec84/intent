# Step routines

step routines

(((
note comment Martin Fowler made (http://martinfowler.com/bliki/ComposedRegex.html): One of the most powerful tools in writing maintainable code is break large methods into well-named smaller methods - a technique Kent Beck refers to as the Composed Method pattern.

People can read your programs much more quickly and accurately if they can understand them in detail, then chunk those details into higher level structures.

-- Kent Beck
)))
In any block, you can introduce a step routine by writing a sentence that begins with a capital letter. It must end with either a : or a . (: for implemented step routine, . for stubbed). For example, inside a routine to backup a folder, you might see this:

Temporarily elevate privileges to ignore differing ACLs:
security_context.become_root()

A step routine has a name that’s a sentence.
It is always private.
It shows up in a stack trace.
It automatically inherits all the parameters, variables, invariants, and exception behaviors of its parent function.
It cannot have any new preconditions.
It may have new postconditions.
It can return values and therefore be the right-hand-side in an assignment.
The compiler will frequently inline them.
It provides an easy point of collapse (like #region in C#) for code editors striving for progressive disclosure.
It can be refactored automatically into a func def below or above its parent (func Temporarily elevate privileges:), but it still doesn’t have a parameter decl section.