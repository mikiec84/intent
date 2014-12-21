// No sentry. Don't add one!

//TUPLE( number, precedence, associativity, name, example, comment )

// NEED TO FIT THESE IN AS FAR AS PRECENDENCE, ASSOCIATIVITY
// anchor $
// hyperlink
// backtick
// apostrophe
// string contat?
// have a look at C++ operators like sizeof, new, delete, offsetof, etc: http://www.cplusplus.com/doc/tutorial/operators/

TUPLE( 0x0101, 1, left_to_right, paren, "(", "group" )
TUPLE( 0x0102, 1, left_to_right, close_paren, ")", "ungroup" )
TUPLE( 0x0103, 1, left_to_right, dot, ".", "navigate a namespace" )
TUPLE( 0x0104, 1, left_to_right, safe_dot, "?.", "navigate a possibly null namespace" )
TUPLE( 0x0105, 1, left_to_right, spread, "*.", "spread across all members of a container" )
TUPLE( 0x0110, 1, left_to_right, brace, "[", "subscript" )
TUPLE( 0x0111, 1, left_to_right, safe_subscript, "?[", "subscript on a possibly null container" )
TUPLE( 0x0112, 1, left_to_right, safe_empty, "[?", "subscript in a possibly empty container" )
TUPLE( 0x0113, 1, left_to_right, safe_subscript_safe_empty, "?[?", "subscript in a possibly null, possibly empty container" )
TUPLE( 0X0114, 1, left_to_right, close_brace, "]", "end of subscript")
TUPLE( 0x0201, 2, right_to_left, mark, "\\", "positive mark" )
TUPLE( 0x0202, 2, right_to_left, negative_mark, "\\-", "negative mark" )
TUPLE( 0x0203, 2, right_to_left, tentative_mark, "?\\", "implied positive mark" )
TUPLE( 0x0204, 2, right_to_left, tentative_negative_mark, "?\\-", "implied negative mark" )
TUPLE( 0x0210, 2, right_to_left, bit_not, "~", "bitwise not" )
TUPLE( 0x0211, 2, right_to_left, bool_not, "!", "logical not" )
TUPLE( 0x0301, 3, right_to_left, cast, "->", "cast" )
TUPLE( 0x0401, 4, left_to_right, star, "*", "multiply" )
TUPLE( 0x0402, 4, left_to_right, slash, "/", "divide" )
TUPLE( 0x0403, 4, left_to_right, mod, "%", "remainder" )
TUPLE( 0x0501, 5, left_to_right, plus, "+", "add" )
TUPLE( 0x0502, 5, left_to_right, minus, "-", "subtract" )
TUPLE( 0x0503, 5, left_to_right, increment, "++", "increment" )
TUPLE( 0x0504, 5, left_to_right, decrement, "--", "decrement" )
TUPLE( 0x0601, 6, left_to_right, lshift, "<<", "shift left" )
TUPLE( 0x0602, 6, left_to_right, rshift, ">>", "shift right" )
TUPLE( 0x0603, 6, left_to_right, unsigned_rshift, ">>>", "unsigned shift right" )
TUPLE( 0x0701, 7, left_to_right, greater, ">", "greater than" )
TUPLE( 0x0702, 7, left_to_right, less, "<", "less than" )
TUPLE( 0x0703, 7, left_to_right, greater_equal, ">=", "greater than or equal to" )
TUPLE( 0x0704, 7, left_to_right, less_equal, "<=", "less than or equal to" )
TUPLE( 0x0710, 7, left_to_right, instance_of, "::", "is an instance of" )
TUPLE( 0x0801, 8, left_to_right, bool_equal, "==", "test equality" )
TUPLE( 0x0802, 8, left_to_right, not_equal, "!=", "test non-equality" )
TUPLE( 0x0810, 8, left_to_right, spaceship, "<=>", "compare -1 0 or 1" )
TUPLE( 0x0811, 8, left_to_right, qspaceship, "?<=>", "compare tolerating null; nulls sort first" )
TUPLE( 0x0812, 8, left_to_right, spaceshipq, "<=>?", "compare tolerating null; nulls sort last" )
TUPLE( 0x0820, 8, left_to_right, in, "-[", "in (test set membership)" )
TUPLE( 0x0821, 8, left_to_right, not_in, "!-[", "not in (negate set membership)" )
TUPLE( 0x0901, 9, left_to_right, bit_and, "&", "bitwise and" )
TUPLE( 0x0a01, 10, left_to_right, bit_xor, "^", "bitwise xor" )
TUPLE( 0x0b01, 11, left_to_right, bit_or, "|", "bitwise or" )
TUPLE( 0x0c01, 12, left_to_right, bool_and, "&&", "boolean and" )
TUPLE( 0x0d01, 13, left_to_right, bool_or, "||", "boolean or" )
TUPLE( 0x0e01, 14, right_to_left, ternary, "?", "if ... else ..." )
TUPLE( 0x0e02, 14, right_to_left, elvis, "?:", "collapse ternary operator" )
TUPLE( 0x0f01, 15, right_to_left, colon, ":", "define" )
TUPLE( 0x0f02, 15, right_to_left, gets, ":=", "define and assign" )
TUPLE( 0x0f03, 15, right_to_left, assign_equals, "=", "assignment" )
TUPLE( 0x0f20, 15, right_to_left, plus_equals, "+=", "increment" )
TUPLE( 0x0f21, 15, right_to_left, minus_equals, "-=", "decrement" )
TUPLE( 0x0f22, 15, right_to_left, times_equals, "*=", "times equals" )
TUPLE( 0x0f23, 15, right_to_left, divide_equals, "/=", "divide equals" )
TUPLE( 0x0f24, 15, right_to_left, mod_equals, "%=", "remainder equals" )
TUPLE( 0x0f25, 15, right_to_left, bit_and_equals, "&=", "bitwise and" )
TUPLE( 0x0f26, 15, right_to_left, bit_xor_equals, "^=", "bitwise xor" )
TUPLE( 0x0f27, 15, right_to_left, bit_or_equals, "|=", "bitwise or" )
TUPLE( 0x0f28, 15, right_to_left, lshift_equals, "<<=", "shift left and assign" )
TUPLE( 0x0f29, 15, right_to_left, rshift_equals, ">>=", "shift right and assign" )

TUPLE( 0x0fff, 15, left_to_right, comma, ",", "sequence") // <<-- should be lowest precedence

#undef TUPLE
