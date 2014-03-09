// No sentry. Don't add one!

//TUPLE( number, name, example, comment )

// structural
TUPLE( 0x00000000, invalid, "\x0", "invalid token" )
TUPLE( 0x00000001, indent, "\t", "increase nesting" )
TUPLE( 0x00000002, dedent, "\x08", "decrease nesting" )
TUPLE( 0x00000003, line_break, "\n", "separate one line from another" )
TUPLE( 0x00000004, soft_break, "... ", "continue current statement on new line" )
TUPLE( 0x00000005, noun_phrase, "foo", "" )
TUPLE( 0x00000006, verb_phrase, "Do something", "" )

// keywords
TUPLE( 0x00001000, keyword_mask, "", "" )
TUPLE( 0x00001001, if, "if", "" )
TUPLE( 0x00001002, else, "else", "" )
TUPLE( 0x00001003, end, "end", "assert end of block" )
TUPLE( 0x00001004, while, "while", "" )
TUPLE( 0x00001005, handle, "handle", "" )
TUPLE( 0x00001006, when, "when", "" )
TUPLE( 0x00001007, try, "try", "" )
TUPLE( 0x00001008, catch, "catch", "" )
TUPLE( 0x00001009, finally, "finally", "" )
TUPLE( 0x0000100a, return, "return", "" )

// literals
TUPLE( 0x00002000, literal_mask, "", "" )
// numbers
TUPLE( 0x00002100, number_literal_mask, "", "" )
TUPLE( 0x00002101, binary_number, "0b0111001", "" )
TUPLE( 0x00002102, decimal_number, "42", "" )
TUPLE( 0x00002103, hex_number, "0xdeadbeef", "" )
TUPLE( 0x00002104, octal_number, "033", "" )
TUPLE( 0x00002105, floating_point_number, "3.14159", "" )
// strings
TUPLE( 0x00002200, string_literal_mask, "", "" )
TUPLE( 0x00002201, quoted_string, "\"abc\"", "" )
TUPLE( 0x00002202, char_literal, "", "" )
TUPLE( 0x00002203, regex, "", "" )
// dates
TUPLE( 0x00002400, date_literal_mask, "", "" )
TUPLE( 0x00002401, iso8601_date, "2014-06-12 8:25:00.12345Z", "" )
TUPLE( 0x00002402, epoch_date, "1970+23y4m2d", "" )
TUPLE( 0x00002403, duration, "5h3m2s", "" )

// comments
TUPLE( 0x00004000, comment_mask, "", "" )
TUPLE( 0x00004001, private_comment, "#", "" )
TUPLE( 0x00004002, doc_comment, "|", "" )
TUPLE( 0x00004003, heredoc, "", "" )
TUPLE( 0x00004004, explanation, "", "" )

// operators have additional semantics and are defined in operator_tuples.h
TUPLE( 0x00008000, operator_mask, "", "" )

#undef TUPLE

