#ifndef _e33c74c59d334c299b7d6a1cc61bc2d0
#define _e33c74c59d334c299b7d6a1cc61bc2d0

namespace intent {
namespace lang {

/**
 * Uniquely identify the semantics of a token.
 */
enum token_type {

    // structural
    tt_invalid = 0,
    tt_indent = 1,
    tt_dedent = 2,
    tt_line_break = 3,
    tt_soft_break = 4,

    // operators
    tt_operator_mask = 0x2000,
    #define TUPLE(name, number, example, precedence, associativity, comment) \
        tt_operator##name = 0x2000 | number,
    #include "lang/operator_tuples.h"

    // literals
    tt_literal_mask = 0x10000000,
    // numbers
    tt_number_literal_mask = 0x11000000,
    tt_binary_number = 0x11000001,
    tt_decimal_number = 0x11000002,
    tt_hex_number = 0x11000003,
    tt_octal_number = 0x11000004,
    tt_float_number = 0x11000005,
    // strings
    tt_string_literal_mask = 0x12000000,
    tt_quoted_string = 0x12000001,
    tt_char_literal = 0x12000002,
    tt_regex = 0x12000003,
    // dates
    tt_date_literal_mask = 0x14000000,
    tt_iso8601_date = 0x14000001,
    tt_epoch_date = 0x14000002,
    tt_duration = 0x14000003,

    // comments
    tt_comment_mask = 0x20000000,
    tt_private_comment = 0x20000001,
    tt_doc_comment = 0x20000002,
    tt_heredoc = 0x20000003,
    tt_explanation = 0x20000004,
};

bool is_comment(token_type);
bool is_literal(token_type);
bool is_number(token_type);
bool is_string(token_type);
bool is_date(token_type);
bool is_operator(token_type);

} // end namespace lang
} // end namespace intent

#include "lang/token_type-inline.h"

#endif // sentry
