#ifndef _6753ecf3c9f111e4846a3c15c2ddfeb4
#define _6753ecf3c9f111e4846a3c15c2ddfeb4

// C++ namespaces are very similar to packages in intent, and some of our C++
// code ultimately needs to be exposed that way. However, the "namespace"
// statement in C++ is quite repetitive, and we'd like to use macros to work
// with namespaces, instead of using the namespace statement directly, so we can
// define the macros in multiple ways to generate marks, static assertions,
// traditional namespace statements, and more.
#define

// This is deep macro kung fu, but it's actually not that mysterious when you
// parse it out. What we'd like to do is have a macro that can accept any number
// of args (that's easy with __VA_ARGS__), but that also
#define _nest_ns_(GET_MACRO(_0, _1, _2, NAME, ...) NAME
#define FOO(...) GET_MACRO(_0, ##__VA_ARGS__, FOO2, FOO1, FOO0)(__VA_ARGS__)

#endif // sentry
