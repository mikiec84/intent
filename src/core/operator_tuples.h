// No sentry. Don't add one!

//TUPLE(name, number, example, precedence, associativity, comment)

//TUPLE(structure, indent, 1, "    ", 0, none, "add a level of nesting")
//TUPLE(structure, dedent, 2, "<<<<", 0, none, "remove a level of nesting")
//TUPLE(structure, soft_break, 3, "... ", 0, none, "continue a statement")

TUPLE(group, 101, "()", 1, left_to_right, "group")
TUPLE(dot, 102, ".", 1, left_to_right, "navigate a namespace")
TUPLE(safe_dot, 103, "?.", 1, left_to_right, "navigate a possibly null namespace")
TUPLE(spread, 104, "*.", 1, left_to_right, "spread across all members of a container")
TUPLE(invoke, 105, "()", 1, left_to_right, "invoke a function")
TUPLE(subscript, 110, "[]", 1, left_to_right, "subscript")
TUPLE(safe_subscript, 111, "?[]", 1, left_to_right, "subscript on a possibly null container")
TUPLE(safe_empty, 112, "[?]", 1, left_to_right, "subscript in a possibly empty container")
TUPLE(safe_subscript_safe_empty, 113, "?[?]", 1, left_to_right, "subscript in a possibly null, possibly empty container")
TUPLE(closed_interval, 120, "[..]", 1, left_to_right, "subscript")
TUPLE(open_closed_interval, 121, "(..]", 1, left_to_right, "subscript")
TUPLE(closed_open_interval, 122, "[..)", 1, left_to_right, "subscript")
TUPLE(open_interval, 123, "(..)", 1, left_to_right, "subscript")

TUPLE(unary_plus, 201, "+", 2, right_to_left, "positive mark")
TUPLE(unary_minus, 202, "-", 2, right_to_left, "negative mark")
TUPLE(implied_unary_plus, 203, "+?", 2, right_to_left, "implied positive mark")
TUPLE(implied_unary_minus, 204, "-?", 2, right_to_left, "implied negative mark")
TUPLE(implied_unary_mark, 205, "#", 2, right_to_left, "mark")
TUPLE(bit_not, 210, "~", 2, right_to_left, "bitwise not")
TUPLE(not, 211, "!", 2, right_to_left, "logical not")

TUPLE(cast, 301, "->", 3, right_to_left, "cast")

TUPLE(times, 401, "*", 4, left_to_right, "multiply")
TUPLE(divide, 402, "/", 4, left_to_right, "divide")
TUPLE(modulo, 403, "%", 4, left_to_right, "remainder")

TUPLE(plus, 501, "+", 5, left_to_right, "add")
TUPLE(minus, 502, "-", 5, left_to_right, "subtract")

TUPLE(lshift, 601, "<<", 6, left_to_right, "shift left")
TUPLE(rshift, 602, ">>", 6, left_to_right, "shift right")

TUPLE(greater, 701, ">", 7, left_to_right, "greater than")
TUPLE(less, 702, "<", 7, left_to_right, "less than")
TUPLE(greater_equal, 703, ">=", 7, left_to_right, "greater than or equal to")
TUPLE(less_equal, 704, "<=", 7, left_to_right, "less than or equal to")
TUPLE(instance_of, 710, "::", 7, left_to_right, "is an instance of")

TUPLE(bool_equals, 801, "==", 8, left_to_right, "test equality")
TUPLE(not_equals, 802, "!=", 8, left_to_right, "test non-equality")
TUPLE(spaceship, 810, "<=>", 8, left_to_right, "compare -1 0 or 1")
TUPLE(qspaceship, 811, "?<=>", 8, left_to_right, "compare tolerating null; nulls sort first")
TUPLE(spaceshipq, 812, "<=>?", 8, left_to_right, "compare tolerating null; nulls sort last")
TUPLE(in, 820, "=>", 8, left_to_right, "in (test set membership)")

TUPLE(bit_and, 901, "&", 9, left_to_right, "bitwise and")

TUPLE(bit_xor, 1001, "^", 10, left_to_right, "bitwise xor")

TUPLE(bit_or, 1101, "|", 11, left_to_right, "bitwise or")

TUPLE(and, 1201, "&&", 12, left_to_right, "boolean and")

TUPLE(or, 1301, "||", 13, left_to_right, "boolean or")

TUPLE(ternary, 1401, "? :", 14, right_to_left, "if ... else ...")
TUPLE(elvis, 1402, "?:", 14, right_to_left, "collapse ternary operator")

TUPLE(define, 1501, ":", 15, right_to_left, "define")
TUPLE(gets, 1502, ":=", 15, right_to_left, "define and assign")
TUPLE(assignment, 1510, "=", 15, right_to_left, "assignment")
TUPLE(plus_equals, 1520, "+=", 15, right_to_left, "add")
TUPLE(minus_equals, 1521, "-=", 15, right_to_left, "subtract")
TUPLE(times_equals, 1522, "*=", 15, right_to_left, "multiply")
TUPLE(divide_equals, 1523, "/=", 15, right_to_left, "divide")
TUPLE(modulo_equals, 1524, "%=", 15, right_to_left, "remainder")
TUPLE(bit_and_equals, 1525, "&=", 15, right_to_left, "bitwise and")
TUPLE(bit_xor_equals, 1526, "^=", 15, right_to_left, "bitwise xor")
TUPLE(bit_or_equals, 1527, "|=", 15, right_to_left, "bitwise or")
TUPLE(lshift_equals, 1528, "<<=", 15, right_to_left, "bitwise or")
TUPLE(rshift_equals, 1529, ">>=", 15, right_to_left, "bitwise or")

#undef TUPLE
