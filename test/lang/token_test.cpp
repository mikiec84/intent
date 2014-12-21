#include <unordered_map>
#include "lang/token.h"

#include "gtest/gtest.h"

using namespace intent::lang;

TEST(token_test, name_lookup) {
    #define TUPLE(number, name, example, comment) EXPECT_STREQ("tt_" #name, get_token_type_name(tt_##name));
    #include "lang/token_type_tuples.h"

    #define TUPLE(number, precedence, associativity, name, example, comment) EXPECT_STREQ("tt_operator_" #name, get_token_type_name(tt_operator_##name));
    #include "lang/operator_tuples.h"
}

// This is implemented as a macro instead of a function so we get errors on line numbers inside a test func,
// instead of inside a helper func.
#define prove_correct(is_func, first, last) \
    for (size_t i = 0; i < get_token_type_count(); ++i) { \
        token_type tt = get_token_type_by_index(i); \
        bool expected = (tt >= first && tt <= last); \
        bool actual = is_func(tt); \
        if (expected != actual) { \
            ADD_FAILURE() << #is_func << "(" << get_token_type_name(tt) << ") was " << actual << " instead of " << expected << "."; \
        } \
    }

TEST(token_test, is_comment) {
    prove_correct(is_comment, tt_comment_mask, tt_explanation);
}

TEST(token_test, is_string_literal) {
    prove_correct(is_string_literal, tt_string_literal_mask, tt_regex);
}

TEST(token_test, is_literal) {
    prove_correct(is_literal, tt_literal_mask, tt_duration);
}

TEST(token_test, is_number_literal) {
    prove_correct(is_number_literal, tt_number_literal_mask, tt_floating_point_number);
}

TEST(token_test, is_date_literal) {
    prove_correct(is_date_literal, tt_date_literal_mask, tt_duration);
}

TEST(token_test, is_operator) {
    prove_correct(is_operator, tt_operator_mask, tt_operator_comma);
}

TEST(token_test, get_token_type_from_name) {
    for (size_t i = 0; i < get_token_type_count(); ++i) {
        token_type tt = get_token_type_by_index(i);
        char const * expected_name = get_token_type_name(tt);
        token_type looked_up = get_token_type_from_name(expected_name);
        char const * actual_name = get_token_type_name(looked_up);
        EXPECT_STREQ(expected_name, actual_name);
    }
}

TEST(token_test, precedence) {
    #define TUPLE(number, precedence, associativity, name, example, comment) EXPECT_EQ(precedence, get_operator_precedence(tt_operator_##name));
    #include "lang/operator_tuples.h"
}

TEST(token_test, associativity) {
    #define TUPLE(number, precedence, associativity, name, example, comment) EXPECT_EQ(oa_##associativity, get_operator_associativity(tt_operator_##name));
    #include "lang/operator_tuples.h"
}

TEST(token_test, unique_numbers) {
    std::unordered_map<int, char const *> counts;
    #define TUPLE(number, name, example, comment) \
    { \
        auto already = counts[number]; \
        if (already) { \
            ADD_FAILURE() << number << " already bound to \"" << already \
                << "\"; can't also bind to \"" << "tt_" #name << "\""; \
        } else { \
            counts[number] = "tt_" #name; \
        } \
    }
    #include "lang/token_type_tuples.h"

    #define TUPLE(number, precedence, associativity, name, example, comment) \
    { \
        auto already = counts[0x8000 | number]; \
        if (already) { \
            ADD_FAILURE() << number << " already bound to \"" << already \
                << "\"; can't also bind to \"" << "tt_operator_" #name << "\""; \
        } else { \
            counts[0x8000 | number] = "tt_operator_" #name; \
        } \
    }
    #include "lang/operator_tuples.h"
}
