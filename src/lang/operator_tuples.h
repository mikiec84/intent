// No sentry. Don't add one!

//TUPLE(name, number, example, precedence, associativity, comment)

//TUPLE(structure, indent, 1, "    ", 0, none, "add a level of nesting")
//TUPLE(structure, dedent, 2, "<<<<", 0, none, "remove a level of nesting")
//TUPLE(structure, soft_break, 3, "... ", 0, none, "continue a statement")

TUPLE(group, 0x0101, "()", 1, left_to_right, "group")
TUPLE(dot, 0x0102, ".", 1, left_to_right, "navigate a namespace")
TUPLE(safe_dot, 0x0103, "?.", 1, left_to_right, "navigate a possibly null namespace")
TUPLE(spread, 0x0104, "*.", 1, left_to_right, "spread across all members of a container")
TUPLE(invoke, 0x0105, "()", 1, left_to_right, "invoke a function")
TUPLE(subscript, 0x0110, "[]", 1, left_to_right, "subscript")
TUPLE(safe_subscript, 0x0111, "?[]", 1, left_to_right, "subscript on a possibly null container")
TUPLE(safe_empty, 0x0112, "[?]", 1, left_to_right, "subscript in a possibly empty container")
TUPLE(safe_subscript_safe_empty, 0x0113, "?[?]", 1, left_to_right, "subscript in a possibly null, possibly empty container")
TUPLE(closed_interval, 0x0120, "[..]", 1, left_to_right, "subscript")
TUPLE(open_closed_interval, 0x0121, "(..]", 1, left_to_right, "subscript")
TUPLE(closed_open_interval, 0x0122, "[..)", 1, left_to_right, "subscript")
TUPLE(open_interval, 0x0123, "(..)", 1, left_to_right, "subscript")

TUPLE(unary_plus, 0x0201, "+", 2, right_to_left, "positive mark")
TUPLE(unary_minus, 0x0202, "-", 2, right_to_left, "negative mark")
TUPLE(implied_unary_plus, 0x0203, "+?", 2, right_to_left, "implied positive mark")
TUPLE(implied_unary_minus, 0x0204, "-?", 2, right_to_left, "implied negative mark")
TUPLE(implied_unary_mark, 0x0205, "#", 2, right_to_left, "mark")
TUPLE(bit_not, 0x0210, "~", 2, right_to_left, "bitwise not")
TUPLE(not, 0x0211, "!", 2, right_to_left, "logical not")

TUPLE(cast, 0x0301, "->", 3, right_to_left, "cast")

TUPLE(times, 0x0401, "*", 4, left_to_right, "multiply")
TUPLE(divide, 0x0402, "/", 4, left_to_right, "divide")
TUPLE(modulo, 0x0403, "%", 4, left_to_right, "remainder")

TUPLE(plus, 0x0501, "+", 5, left_to_right, "add")
TUPLE(minus, 0x0502, "-", 5, left_to_right, "subtract")

TUPLE(lshift, 0x0601, "<<", 6, left_to_right, "shift left")
TUPLE(rshift, 0x0602, ">>", 6, left_to_right, "shift right")

TUPLE(greater, 0x0701, ">", 7, left_to_right, "greater than")
TUPLE(less, 0x0702, "<", 7, left_to_right, "less than")
TUPLE(greater_equal, 0x0703, ">=", 7, left_to_right, "greater than or equal to")
TUPLE(less_equal, 0x0704, "<=", 7, left_to_right, "less than or equal to")
TUPLE(instance_of, 0x0710, "::", 7, left_to_right, "is an instance of")

TUPLE(bool_equals, 0x0801, "==", 8, left_to_right, "test equality")
TUPLE(not_equals, 0x0802, "!=", 8, left_to_right, "test non-equality")
TUPLE(spaceship, 0x0810, "<=>", 8, left_to_right, "compare -1 0 or 1")
TUPLE(qspaceship, 0x0811, "?<=>", 8, left_to_right, "compare tolerating null; nulls sort first")
TUPLE(spaceshipq, 0x0812, "<=>?", 8, left_to_right, "compare tolerating null; nulls sort last")
TUPLE(in, 0x0820, "=>", 8, left_to_right, "in (test set membership)")

TUPLE(bit_and, 0x0901, "&", 9, left_to_right, "bitwise and")

TUPLE(bit_xor, 0x0a01, "^", 10, left_to_right, "bitwise xor")

TUPLE(bit_or, 0x0b01, "|", 11, left_to_right, "bitwise or")

TUPLE(and, 0x0c01, "&&", 12, left_to_right, "boolean and")

TUPLE(or, 0x0d01, "||", 13, left_to_right, "boolean or")

TUPLE(ternary, 0x0e01, "? :", 14, right_to_left, "if ... else ...")
TUPLE(elvis, 0x0e02, "?:", 14, right_to_left, "collapse ternary operator")

TUPLE(define, 0x0f01, ":", 15, right_to_left, "define")
TUPLE(gets, 0x0f02, ":=", 15, right_to_left, "define and assign")
TUPLE(assignment, 1510, "=", 15, right_to_left, "assignment")
TUPLE(plus_equals, 0x0f20, "+=", 15, right_to_left, "add")
TUPLE(minus_equals, 0x0f21, "-=", 15, right_to_left, "subtract")
TUPLE(times_equals, 0x0f22, "*=", 15, right_to_left, "multiply")
TUPLE(divide_equals, 0x0f23, "/=", 15, right_to_left, "divide")
TUPLE(modulo_equals, 0x0f24, "%=", 15, right_to_left, "remainder")
TUPLE(bit_and_equals, 0x0f25, "&=", 15, right_to_left, "bitwise and")
TUPLE(bit_xor_equals, 0x0f26, "^=", 15, right_to_left, "bitwise xor")
TUPLE(bit_or_equals, 0x0f27, "|=", 15, right_to_left, "bitwise or")
TUPLE(lshift_equals, 0x0f28, "<<=", 15, right_to_left, "bitwise or")
TUPLE(rshift_equals, 0x0f29, ">>=", 15, right_to_left, "bitwise or")

#undef TUPLE
