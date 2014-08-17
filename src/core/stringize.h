#ifndef intent_core_stringize_h
#define intent_core_stringize_h

// These macros provide a way to convert a numeric macro such as __LINE__
// to a string at compile time. See http://j.mp/1kSzg2l and http://j.mp/1rcwHGg.

// Suppose you have this:
//    #define MY_FIELD_WIDTH 17
// and you need to get the string "17", e.g., to use in a field width specifier
// for printf.

// This macro will convert its arg to a string. So if you call it with
// MY_FIELD_WIDTH, you'll get "MY_FIELD_WIDTH". You can use this to convert
// variable names and expressions to strings.
#define STRINGIZE(x) #x

// This macro expands x *before* it calls SIMPLE_STRINGIZE(), so it yields
// the string representation of the *value* of its arg, instead of the string
// representation of the *name* of its arg.
#define DOUBLE_STRINGIZE(x) STRINGIZE(x) // use this for numbers

#endif // STRINGIZE_H
