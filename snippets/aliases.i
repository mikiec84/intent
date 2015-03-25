# Most programming languages provide a way to refer to names that are defined
... elsewhere. In C/C++, you use "#include" or "using"; in java and python, you
... use the "import" keyword. Intent's solution to this problem is elegant,
... terse, and consistent with its overall pattern for how names are defined. To
... name a variable, class, or function, intent expects expressions in the form
... "name: definition". Using a name from elsewhere is similar, except that the
... definition is a hyperlink instead of expanded content. Also, since the
... referenced name is often the same name we wish to use in local scope, intent
... allows us to leave the name on the left side of the : operator implicit.
... Here's a statement that defines a name "my class", in local scope, that has
... the same meaning as "my class" at the url "@component.subpackage.my class":
: @component.subpackage.my class

# We refer to statements like these, that reference a name from elsewhere, as
... "alias" statements. Aliases do not create new storage or impose any run-time
... overhead; they are de-referenced during compilation (although the debugger
... retains them for convenience).

# You can rename a symbol as you import it, simply by providing a local name
... for what you're referencing. Here, the local name "bar" is an alias for
... component.subpackage.foo. This statement is similar to a typedef in C++ (if
... component.subpackage.foo is a type).
bar: @component.subpackage.foo

# It's possible to pull all public names from another location into the current
... scope, like a wildcard import in java or a #include in C/C++:
: @component.subpackage.*

# By convention, alias statements appear at the top of a module. However, they
... can be used in any containing scope. Like all other names, aliases are
... usable on any line in the containing scope, after the port where they are
... defined:
class foo:
    # The next statement makes foo.baz an alias for @component.subpackage.bar;
    ... anywhere that the local scope includes names from foo, "baz" now has
    ... meaning:
    baz: @component.subpackage.bar

# Most aliases are to classes, types, or functions. However, it's also possible
... to alias namespaces, packages, constants, variables, or just about anything
... else that has a name. (Remember, these aliases are not new copies; they just
... refer back to their original definition.)
lbs_per_kg : @weights and measures.imperial.pounds_per_kilogram

# Like other names, aliases can accept assignment (if the construct they name
... is assignable), but cannot have more than one definition. However, it is
... possible to "forward-declare" any alias, and elsewhere provide an expansion,
... as long as the eventual expansion is compatible. Here, the first line is a
... forward decl, and the second line is a compatible expansion.
lbs_per_kg: int +<final
lbs_per_kg: @weights and measures.imperial.pounds_per_kilogram

# Usually, hyperlinks refer to other constructs defined in intent code, and use
... the ispace schema; this is true of all all the examples so far. However,
... aliases can also refer to code exposed by shared libraries (.so, .dll,
... .dynlib), allowing full interoperability with C/C++. Such aliases use the
... "sharedlib" schema, and may include additional attributes to control dynamic
... linking, boxing, and so forth:
Create process: @sharedlib://kernel32|>8.0|CreateProcess;
    in:
        - id of thread to be attached: dword
        - id of thread thats attached to: dword
        - should attach: bool
    out:
        - succeeded: bool

# In the previous example, the name "Create process" was explicit. All aliased
... identifiers from external code must obey intent naming conventions, so their
... names are automatically normalized; this means an implicit name for the alias
... would have produced the same results. In fact, all of the following function
... names in external code map would create the same alias ("Create process") in
... intent:
:@sharedlib://kernel32/CreateProcess
:@sharedlib://my.dll/create_process
:@sharedlib://my.so/createProcess
:@sharedlib://my.dynlib/Create_Process

# Interop with other runtimes besides C/C++ are also possible with aliases. You
... can alias classes in a jvm. The URL consists of 3 parts, delimited by |, in
... the form jvm_selector|classpath|target. This statement aliases "custom router"
... to the com.acme.net.CustomRouter class:
:@jvm://openjdk8|.:~:/opt/local/java:foo.jar|com.acme.net.CustomRouter

# Aliasing .net classes is similar, except that the second part identifies an
... assembly:
:@dotnet://4.5|NetUtils|com.acme.net.Router
