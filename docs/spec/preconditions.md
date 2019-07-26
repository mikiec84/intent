The compiler needs to capture the expression that's evaluated as a string.
not just the result of the evaluation. Ideally, it should also report the
value of components of the expression when false, like pytest does for
failed assertions.

Compiler should prove that preconditions con't call functions with side
effects. Maybe we need a version of precondition that allows or even
requires side effects -- "enforce"? Example: we want to guarantee that
the first item returned by an iterator is X. This HAS a side effect, and
we want it (namely, to consume the first item). But we also want
precondition logic. (Do we just consume the first item, and then call
precondition on the value of that item, instead of calling precondition
on the item as it's being consumed in a single call?)

Note that all of the above applies to postconditions, too.

Do we need a special precondition for input sanitation for args to a
function (and maybe for input from CLI or files)?