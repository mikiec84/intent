#include <algorithm>
#include <cstring>
#include "lang/token_type.h"

namespace intent {
namespace lang {

struct token_type_info {
    char const * name;
    token_type number;
    char const * example;
    char const * comment;
};

token_type_info const tt_infos[] = {

    #define TUPLE(number, name, example, comment) \
        { "tt_" #name, static_cast<token_type>(number), example, comment },
    #include "lang/token_type.tuples"

    #define TUPLE(number, precedence, associativity, name, example, comment) \
        { "tt_operator_" #name, static_cast<token_type>(0x8000 | number), example, comment },
    #include "lang/operator.tuples"
};

const size_t tt_infos_count = sizeof(tt_infos) / sizeof(token_type_info);
token_type_info const * const tt_infos_begin = &tt_infos[0];
token_type_info const * const tt_infos_end = &tt_infos[tt_infos_count];

size_t get_token_type_count() {
    return tt_infos_count;
}

token_type get_token_type_by_index(size_t i) {
    if (i < tt_infos_count) {
        return tt_infos[i].number;
    }
    return tt_none;
}

struct operator_info {
    token_type number;
    int precedence;
    operator_associativity associativity;
};

operator_info const op_infos[] = {

    #define TUPLE(number, precedence, associativity, name, example, comment) \
        { static_cast<token_type>(0x8000 | number), precedence, oa_##associativity},
    #include "lang/operator.tuples"
};

const size_t op_infos_count = sizeof(op_infos) / sizeof(operator_info);
operator_info const * const op_infos_begin = &op_infos[0];
operator_info const * const op_infos_end = &op_infos[op_infos_count];

token_type get_token_type_from_name(char const * name) {
    if (name && *name) {
        for (size_t i = 0; i < tt_infos_count; ++i) {
            if (strcmp(tt_infos[i].name, name) == 0) {
                return tt_infos[i].number;
            }
        }
    }
    return tt_none;
}

inline bool compare_tt(token_type_info const & tti, token_type tt) {
    return tti.number < tt;
}

inline token_type_info const * find_tt_info(token_type tt) {
    return std::lower_bound(tt_infos_begin, tt_infos_end, tt, compare_tt);
}

char const * get_token_type_name(token_type tt) {
    auto info = find_tt_info(tt);
    return (info != tt_infos_end) ? info->name : nullptr;
}

char const * get_token_type_example(token_type tt) {
    auto info = find_tt_info(tt);
    return (info != tt_infos_end) ? info->example : nullptr;
}

char const * get_token_type_comment(token_type tt) {
    auto info = find_tt_info(tt);
    return (info != tt_infos_end) ? info->comment : nullptr;
}

inline bool compare_op(operator_info const & oi, token_type tt) {
    return oi.number < tt;
}

inline operator_info const * find_op_info(token_type tt) {
    return std::lower_bound(op_infos_begin, op_infos_end, tt, compare_op);
}

int get_operator_precedence(token_type tt) {
    auto info = find_op_info(tt);
    return (info != op_infos_end) ? info->precedence : 100;
}

operator_associativity get_operator_associativity(token_type tt) {
    auto info = find_op_info(tt);
    return (info != op_infos_end) ? info->associativity : oa_left_to_right;
}

} // end namespace lang
} // end namespace intent
