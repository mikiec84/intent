#ifndef _427e4537328c4799bdaaf21d8286b211
#define _427e4537328c4799bdaaf21d8286b211

/*
Some things to know about macros:

1. Macros are evaluated at the place where they are called, not where they are
   defined. This is why special symbols like __FILE__, __LINE__, and __func__
   can have different values each time they appear. You can use this in macros
   you write.

2. Arguments to macros generally undergo macro expansion.

3. An exception to rule 2 is those arguments that are preceded by # or ##, or
   followed by ##.

4. Another exception to rule 2 is that if a macro is defined to receive args, but
   it appears without parens, it is not expanded. This means that a macro that
   expands to an expression that includes its own name without parens is not
   further expanded.

5. The # operator converts a name/symbol in a macro into its string equivalent.

6. The ## operator allows you to concatenate macro arguments onto identifiers.
*/


/**
 * These macros provide a way to convert a numeric macro such as __LINE__
 * to a string at compile time. See http://j.mp/1kSzg2l and http://j.mp/1rcwHGg.
 *
 * Suppose you have this:
 *    #define MY_FIELD_WIDTH 17
 * and you need to get the string "17", e.g., to use in a field width specifier
 * for printf.
 *
 * The stringize() macro will convert its arg to a string. So if you call it with
 * MY_FIELD_WIDTH, you'll get "MY_FIELD_WIDTH". You can use this to convert
 * variable names and expressions to strings.
 *
 * The stringize_value_of() macro expands its arg *before* it calls stringize(),
 * so it yields the string representation of the *value* of its arg, instead of
 * the string representation of the *name* of its arg. So if you call it with
 * MY_FIELD_WIDTH, you'll get "17".
 */
#define stringize(x) #x
#define stringize_value_of(x) stringize(x) // use this to stringize __LINE__ or other numeric macro expressions

/**
 * These macros provide a way to convert two tokens into a single token. The
 * first is straightforward: glue_tokens(a, b) --> ab.
 *
 * The second, however, is non-obvious. It allows you to use a macro like
 * __LINE__ or __COUNTER__ to create a unique token based on a line number or
 * a macro invocation count, because the args to the macro are expanded into
 * their values, and then glued.
 */
#define glue_tokens(a, b) a ## b
#define glue_token_values(a, b) glue_tokens(a, b)


// IMPL DETAIL: accept any number of args >= 51, but expand to just the 51st one.
#define _get_51st_arg( \
     _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, N, ...) N

/**
 * Count how many args were passed to a variadic macro. Up to 50 args are supported.
 */
#define count_varargs(...) _get_51st_arg( \
    "ignored", ##__VA_ARGS__, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
     9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

// IMPL DETAIL: make versions of a for-each macro that are overridden on arg count.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x) _fe_3(_call, __VA_ARGS__)
#define _fe_5(_call, x, ...) _call(x) _fe_4(_call, __VA_ARGS__)
#define _fe_6(_call, x, ...) _call(x) _fe_5(_call, __VA_ARGS__)
#define _fe_7(_call, x, ...) _call(x) _fe_6(_call, __VA_ARGS__)
#define _fe_8(_call, x, ...) _call(x) _fe_7(_call, __VA_ARGS__)
#define _fe_9(_call, x, ...) _call(x) _fe_8(_call, __VA_ARGS__)
#define _fe_10(_call, x, ...) _call(x) _fe_9(_call, __VA_ARGS__)
#define _fe_11(_call, x, ...) _call(x) _fe_10(_call, __VA_ARGS__)
#define _fe_12(_call, x, ...) _call(x) _fe_11(_call, __VA_ARGS__)
#define _fe_13(_call, x, ...) _call(x) _fe_12(_call, __VA_ARGS__)
#define _fe_14(_call, x, ...) _call(x) _fe_13(_call, __VA_ARGS__)
#define _fe_15(_call, x, ...) _call(x) _fe_14(_call, __VA_ARGS__)
#define _fe_16(_call, x, ...) _call(x) _fe_15(_call, __VA_ARGS__)
#define _fe_17(_call, x, ...) _call(x) _fe_16(_call, __VA_ARGS__)
#define _fe_18(_call, x, ...) _call(x) _fe_17(_call, __VA_ARGS__)
#define _fe_19(_call, x, ...) _call(x) _fe_18(_call, __VA_ARGS__)
#define _fe_20(_call, x, ...) _call(x) _fe_19(_call, __VA_ARGS__)
#define _fe_21(_call, x, ...) _call(x) _fe_20(_call, __VA_ARGS__)
#define _fe_22(_call, x, ...) _call(x) _fe_21(_call, __VA_ARGS__)
#define _fe_23(_call, x, ...) _call(x) _fe_22(_call, __VA_ARGS__)
#define _fe_24(_call, x, ...) _call(x) _fe_23(_call, __VA_ARGS__)
#define _fe_25(_call, x, ...) _call(x) _fe_24(_call, __VA_ARGS__)
#define _fe_26(_call, x, ...) _call(x) _fe_25(_call, __VA_ARGS__)
#define _fe_27(_call, x, ...) _call(x) _fe_26(_call, __VA_ARGS__)
#define _fe_28(_call, x, ...) _call(x) _fe_27(_call, __VA_ARGS__)
#define _fe_29(_call, x, ...) _call(x) _fe_28(_call, __VA_ARGS__)
#define _fe_30(_call, x, ...) _call(x) _fe_29(_call, __VA_ARGS__)
#define _fe_31(_call, x, ...) _call(x) _fe_30(_call, __VA_ARGS__)
#define _fe_32(_call, x, ...) _call(x) _fe_31(_call, __VA_ARGS__)
#define _fe_33(_call, x, ...) _call(x) _fe_32(_call, __VA_ARGS__)
#define _fe_34(_call, x, ...) _call(x) _fe_33(_call, __VA_ARGS__)
#define _fe_35(_call, x, ...) _call(x) _fe_34(_call, __VA_ARGS__)
#define _fe_36(_call, x, ...) _call(x) _fe_35(_call, __VA_ARGS__)
#define _fe_37(_call, x, ...) _call(x) _fe_36(_call, __VA_ARGS__)
#define _fe_38(_call, x, ...) _call(x) _fe_37(_call, __VA_ARGS__)
#define _fe_39(_call, x, ...) _call(x) _fe_38(_call, __VA_ARGS__)
#define _fe_40(_call, x, ...) _call(x) _fe_39(_call, __VA_ARGS__)
#define _fe_41(_call, x, ...) _call(x) _fe_40(_call, __VA_ARGS__)
#define _fe_42(_call, x, ...) _call(x) _fe_41(_call, __VA_ARGS__)
#define _fe_43(_call, x, ...) _call(x) _fe_42(_call, __VA_ARGS__)
#define _fe_44(_call, x, ...) _call(x) _fe_43(_call, __VA_ARGS__)
#define _fe_45(_call, x, ...) _call(x) _fe_44(_call, __VA_ARGS__)
#define _fe_46(_call, x, ...) _call(x) _fe_45(_call, __VA_ARGS__)
#define _fe_47(_call, x, ...) _call(x) _fe_46(_call, __VA_ARGS__)
#define _fe_48(_call, x, ...) _call(x) _fe_47(_call, __VA_ARGS__)
#define _fe_49(_call, x, ...) _call(x) _fe_48(_call, __VA_ARGS__)
#define _fe_50(_call, x, ...) _call(x) _fe_49(_call, __VA_ARGS__)

/**
 * Provide a for-each construct for variadic macros.
 *
 * Example usage:
 *     #define FWD_DECLARE_CLASS(cls) class cls;
 *     call_macro_x_for_each(FWD_DECLARE_CLASS, Foo, Bar)
 */
#define call_macro_x_for_each(x, ...) \
    _get_51st_arg("ignored", ##__VA_ARGS__, \
    _fe_49, _fe_48, _fe_47, _fe_46, _fe_45, _fe_44, _fe_43, _fe_42, _fe_41, _fe_40, \
    _fe_39, _fe_38, _fe_37, _fe_36, _fe_35, _fe_34, _fe_33, _fe_32, _fe_31, _fe_30, \
    _fe_29, _fe_28, _fe_27, _fe_26, _fe_25, _fe_24, _fe_23, _fe_22, _fe_21, _fe_20, \
    _fe_19, _fe_18, _fe_17, _fe_16, _fe_15, _fe_14, _fe_13, _fe_12, _fe_11, _fe_10, \
    _fe_9, _fe_8, _fe_7, _fe_6, _fe_5, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)

/**
 * Provide a convenient way to place variadic args somewhere other than the
 * end of a macro invocation. This is syntatic sugar that provides a variation
 * to call_macro_x_for_each; it allows the unbounded list of args to be used
 * at the front of a macro, and the predicate applied to each to come at the end.
 *
 * Example usage:
 *     with( each(a, b, c), do_something);
 */
#define with(args, pred) call_macro_x_for_each(pred, args)
#define each(...) __VA_ARGS__

#endif // sentry
