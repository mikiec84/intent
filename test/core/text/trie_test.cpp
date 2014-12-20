#include "core/text/trie.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

#if 0
TEST(trie_test, size) {
    static constexpr char * items[] = {"A", "to", "tea", "ted", "ten", "i", "in", "inn"};
    trie<char, char> t;
    for (auto item: items) {
        t.insert(item, 0);
    }
}
#endif


