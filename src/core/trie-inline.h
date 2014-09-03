#ifndef intent_core_trie_inline_h
#define intent_core_trie_inline_h

#include "trie.h"

namespace intent {
namespace core {

template <typename K, typename V>
struct trie<K, V>::node {
    K key;
    V value;
    typedef std::vector<node *> children_t;
    children_t children;

    node(K key);
    ~node();
    node * find(K key);
};

template <typename K, typename V>
inline trie<K, V>::node::~node() {
   for (auto & i = children.begin(); i != children.end(); ++i) {
       delete *i;
   }
}

template <typename K, typename V>
inline trie<K, V> * trie<K, V>::node::find(K key) {
    auto i = std::lower_bound(children.begin(), children.end(),
            [](node const * a, node const * b) { return (*a)->key < (*b)->key; });
    return (i != children.end() && (*i)->key == key) ? *i : nullptr;
}

template <typename K, typename V>
trie<K, V>::trie() : root(new node(static_cast<K>(0))) {
}

template <typename K, typename V>
trie<K, V>::~trie() {
    delete root;
}

template <typename K, typename V>
void trie<K, V>::insert(K const * key, size_t len) {
}

template <typename K, typename V>
trie<K, V>::remove(K const * key, size_t len) {
}

template <typename K>
inline size_t keylen(K const * key) {
    static constexpr ZERO_VAL = static_cast<K>(0);
    for (K const * p = key; *p != ZERO_VAL; ++p)
        ;
    return p - key;
}

template <>
inline size_t keylen<char>(char const * key) {
    return strlen(key);
}

template <typename K, typename V>
node const * trie<K, V>::insert(K const * key, size_t len) {
    if (len == COMPUTE_LEN) {
        len = keylen(key);
    }
    if (len == 0) {
        return root;
    }

    node * current = root;
    K const * end = key + len;
    for (K const * p = key; p < end; ++p)
    {
        auto x = *p;
        node * child = current->find(x);
        if (!child) {
            child = new node(x);
            current->push_back(child);
        }
        current = child;
    }
    return current;
}


}} // end namespace

#endif // sentry

