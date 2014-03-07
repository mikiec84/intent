#include <unordered_map>
#include "lang/parse_token.h"

#include "gtest/gtest.h"

TEST(parse_token_test, unique_numbers) {
    std::unordered_map<int, char const *> counts;
    #define TUPLE(name, number, example, precedence, associativity, comment) \
    { \
        auto pair = counts.emplace(number, #name); \
        if (!pair.second) { \
            ADD_FAILURE() << number << " already bound to \"" << pair.first->second \
                << "\"; can't also bind to \"" << #name << "\""; \
        } \
    }
    #include "lang/operator_tuples.h"
}

