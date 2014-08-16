#ifndef _token_type_inline_h_c91db24c55bf4309ab2f258d899efd3f
#define _token_type_inline_h_c91db24c55bf4309ab2f258d899efd3f

#include "lang/token_type.h"

namespace intent {
namespace lang {

inline unsigned operator &(token_type a, token_type b) {
    return static_cast<unsigned>(a) & static_cast<unsigned>(b);
}

inline bool is_comment(token_type tt) {
    return (tt & tt_comment_mask) == tt_comment_mask;
}

inline bool is_literal(token_type tt) {
    return (tt & tt_literal_mask) == tt_literal_mask;
}

inline bool is_number_literal(token_type tt) {
    return (tt & tt_number_literal_mask) == tt_number_literal_mask;
}

inline bool is_string_literal(token_type tt) {
    return (tt & tt_string_literal_mask) == tt_string_literal_mask;
}

inline bool is_date_literal(token_type tt) {
    return (tt & tt_date_literal_mask) == tt_date_literal_mask;
}

inline bool is_operator(token_type tt) {
    return (tt & tt_operator_mask) == tt_operator_mask;
}

}} // end namespace

#endif // sentry
