// No sentry. Don't add one!

//TUPLE( number, precedence, associativity, name, example, comment )

TUPLE( 0x0101, 1, left_to_right, group, "()", "group" )
TUPLE( 0x0102, 1, left_to_right, dot, ".", "navigate a namespace" )
TUPLE( 0x0103, 1, left_to_right, safe_dot, "?.", "navigate a possibly null namespace" )
TUPLE( 0x0104, 1, left_to_right, spread, "*.", "spread across all members of a container" )
TUPLE( 0x0105, 1, left_to_right, invoke, "()", "invoke a function" )
TUPLE( 0x0110, 1, left_to_right, subscript, "[]", "subscript" )
TUPLE( 0x0111, 1, left_to_right, safe_subscript, "?[]", "subscript on a possibly null container" )
TUPLE( 0x0112, 1, left_to_right, safe_empty, "[?]", "subscript in a possibly empty container" )
TUPLE( 0x0113, 1, left_to_right, safe_subscript_safe_empty, "?[?]", "subscript in a possibly null, possibly empty container" )
TUPLE( 0x0120, 1, left_to_right, closed_interval, "[..]", "subscript" )
TUPLE( 0x0121, 1, left_to_right, open_closed_interval, "(..]", "subscript" )
TUPLE( 0x0122, 1, left_to_right, closed_open_interval, "[..)", "subscript" )
TUPLE( 0x0123, 1, left_to_right, open_interval, "(..)", "subscript" )

TUPLE( 0x0201, 2, right_to_left, unary_plus, "+", "positive mark" )
TUPLE( 0x0202, 2, right_to_left, unary_minus, "-", "negative mark" )
TUPLE( 0x0203, 2, right_to_left, implied_unary_plus, "+?", "implied positive mark" )
TUPLE( 0x0204, 2, right_to_left, implied_unary_minus, "-?", "implied negative mark" )
TUPLE( 0x0205, 2, right_to_left, implied_unary_mark, "#", "mark" )
TUPLE( 0x0210, 2, right_to_left, bit_not, "~", "bitwise not" )
TUPLE( 0x0211, 2, right_to_left, not, "!", "logical not" )

TUPLE( 0x0301, 3, right_to_left, cast, "->", "cast" )

TUPLE( 0x0401, 4, left_to_right, times, "*", "multiply" )
TUPLE( 0x0402, 4, left_to_right, divide, "/", "divide" )
TUPLE( 0x0403, 4, left_to_right, modulo, "%", "remainder" )

TUPLE( 0x0501, 5, left_to_right, plus, "+", "add" )
TUPLE( 0x0502, 5, left_to_right, minus, "-", "subtract" )

TUPLE( 0x0601, 6, left_to_right, lshift, "<<", "shift left" )
TUPLE( 0x0602, 6, left_to_right, rshift, ">>", "shift right" )

TUPLE( 0x0701, 7, left_to_right, greater, ">", "greater than" )
TUPLE( 0x0702, 7, left_to_right, less, "<", "less than" )
TUPLE( 0x0703, 7, left_to_right, greater_equal, ">=", "greater than or equal to" )
TUPLE( 0x0704, 7, left_to_right, less_equal, "<=", "less than or equal to" )
TUPLE( 0x0710, 7, left_to_right, instance_of, "::", "is an instance of" )

TUPLE( 0x0801, 8, left_to_right, bool_equals, "==", "test equality" )
TUPLE( 0x0802, 8, left_to_right, not_equals, "!=", "test non-equality" )
TUPLE( 0x0810, 8, left_to_right, spaceship, "<=>", "compare -1 0 or 1" )
TUPLE( 0x0811, 8, left_to_right, qspaceship, "?<=>", "compare tolerating null; nulls sort first" )
TUPLE( 0x0812, 8, left_to_right, spaceshipq, "<=>?", "compare tolerating null; nulls sort last" )
TUPLE( 0x0820, 8, left_to_right, in, "=>", "in (test set membership)" )

TUPLE( 0x0901, 9, left_to_right, bit_and, "&", "bitwise and" )

TUPLE( 0x0a01, 10, left_to_right, bit_xor, "^", "bitwise xor" )

TUPLE( 0x0b01, 11, left_to_right, bit_or, "|", "bitwise or" )

TUPLE( 0x0c01, 12, left_to_right, and, "&&", "boolean and" )

TUPLE( 0x0d01, 13, left_to_right, or, "||", "boolean or" )

TUPLE( 0x0e01, 14, right_to_left, ternary, "? :", "if ... else ..." )
TUPLE( 0x0e02, 14, right_to_left, elvis, "?:", "collapse ternary operator" )

TUPLE( 0x0f01, 15, right_to_left, define, ":", "define" )
TUPLE( 0x0f02, 15, right_to_left, gets, ":=", "define and assign" )
TUPLE( 0x0f03, 15, right_to_left, assignment, "=", "assignment" )
TUPLE( 0x0f20, 15, right_to_left, plus_equals, "+=", "add" )
TUPLE( 0x0f21, 15, right_to_left, minus_equals, "-=", "subtract" )
TUPLE( 0x0f22, 15, right_to_left, times_equals, "*=", "multiply" )
TUPLE( 0x0f23, 15, right_to_left, divide_equals, "/=", "divide" )
TUPLE( 0x0f24, 15, right_to_left, modulo_equals, "%=", "remainder" )
TUPLE( 0x0f25, 15, right_to_left, bit_and_equals, "&=", "bitwise and" )
TUPLE( 0x0f26, 15, right_to_left, bit_xor_equals, "^=", "bitwise xor" )
TUPLE( 0x0f27, 15, right_to_left, bit_or_equals, "|=", "bitwise or" )
TUPLE( 0x0f28, 15, right_to_left, lshift_equals, "<<=", "bitwise or" )
TUPLE( 0x0f29, 15, right_to_left, rshift_equals, ">>=", "bitwise or" )

#undef TUPLE
