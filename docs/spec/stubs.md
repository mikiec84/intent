Use the keyword "stub" to make an impl that's not yet done. Better than
 "TODO" because compiler can interview you when
it hits a stub in simulated mode, and can record what you
tell it, then use it as your default answer next time. Can also use to
build test cases and maybe to create docs for the stubbed function
(sample args and return values). Can
also let you specify conditions to avoid prompting (just give stubbed
value) in future. Alternately, can raise unimplemented error.

Precondition or postcondition can take a condition, indicating that if
the contract is violated, the stub applies.

```precondition or stub(x < 20)```

Maybe stubs need descriptors or marks, like:

```stub: +cantbeta```

...which would say the stub has to be removed by beta.

Need stub in if statements that can't be evaluated:

```if stub(something that can't be evaluated)```


Implicit stubs: run in "conceptual mode" -- anything that's not stubbed
out yet and not implemented can have stubs generated.

Default stub might be to just print out a notice that a stub was called.

Interaction with step routines?

Do stubs need to be named or tagged so the same stub can be found in
multiple places?

Can a stub be upgraded but not removed (that is, partly implemented)?