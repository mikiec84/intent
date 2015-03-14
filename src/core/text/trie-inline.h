#ifndef _67538582c9f111e4bdb43c15c2ddfeb4
#define _67538582c9f111e4bdb43c15c2ddfeb4

#include <cstring>

#include "core/text/trie.h"

namespace intent {
namespace core {
namespace text {

template <typename K, typename V, typename I>
inline trie<K, V, I>::node_t::node_t(): key(), value(), children() {
    children.reserve(8);
}

template <typename K, typename V, typename I>
inline trie<K, V, I>::node_t::node_t(K const & key, V const & value):
        key(key), value(value), children() {
    children.reserve(4);
}

template <typename K, typename V, typename I>
inline trie<K, V, I>::trie(size_t start_size) {
    //assert(start_size > 0 && start_size <= max_index, "bad start_size");
    nodes.reserve(start_size);
    nodes.push_back(node_t());
}

template <typename K, typename V, typename I>
inline typename trie<K, V, I>::node_t const & trie<K, V, I>::get_root() const {
    return nodes[0];
}

template <typename K, typename V, typename I>
inline typename trie<K, V, I>::node_t & trie<K, V, I>::get_root() {
    return nodes[0];
}

template <typename K>
inline size_t keylen(K const * key) {
    static constexpr K ZERO_VAL = static_cast<K>(0);
    K const * p;
    for (p = key; *p != ZERO_VAL; ++p)
        ;
    return static_cast<size_t>(p - key);
}

template <>
inline size_t keylen<char>(char const * key) {
    return strlen(key);
}

template <typename T>
struct trie_node_child_is_less {
    T const & trie;
    trie_node_child_is_less(T const & t) : trie(t) {}

    inline bool operator () (typename T::index_t a, typename T::key_t const & k) const {
        return trie.nodes[a].key < k;
    }
};

template <typename K, typename V, typename I>
inline typename trie<K, V, I>::node_t * trie<K, V, I>::find(K const * key, size_t len) {
    return const_cast<node_t *>(const_cast<trie const *>(this)->find(key, len));
}

enum class insert_type : char { before_node, dup, child };

template <typename K, typename V, typename I>
inline typename trie<K, V, I>::indexes_t::iterator trie<K, V, I>::find_insert_position(K const * key,
        size_t len, insert_type & itype) const {

    trie_node_child_is_less<trie<K, V, I>> functor(*this);

    node_t * node = get_root();
    auto const end_of_key = key + len;
    for (K const * p = key; p < end_of_key; ++p) {
        auto c = *p;
        auto end = node->children.end();
        auto i = std::lower_bound(node->children.begin(), end, c, functor);
        if (i == end || i->key != c) {
            itype = insert_type::before_node;
            return i;
        }
        if (p == end_of_key - 1) {
            //itype = insert_type::
        }
    }
}

template <typename K, typename V, typename I>
inline typename trie<K, V, I>::node_t const * trie<K, V, I>::find(K const * key, size_t len) const {
//    if (len == calc_len) {
//        len = keylen(key);
//    }
//    do {
//        auto i = std::lower_bound(children.cbegin(), children.cend(), trie_node_child_is_less(*this));
//        return (i != children.end() && (*i)->key == key) ? *i : nullptr;
//    } while (true);
    return nullptr;
}

template <typename K, typename V, typename I>
inline bool trie<K, V, I>::contains(K const * key, size_t len) const {
    return false;
    //return find(key, len) != nullptr;
}

#if 0
template <typename K, typename V, typename I>
trie<K, V, I>::trie() : root(new node_t(static_cast<K>(0))) {
}

template <typename K, typename V, typename I>
trie<K, V, I>::~trie() {
    delete root;
}

template <typename K, typename V>
void trie<K, V>::insert(K const * key, size_t len) {
    node* n = root;

    for ( int i = 0; i < s.length(); i++ )
    {
        Node* child = current->findChild(s[i]);
        if ( child != NULL )
        {
            current = child;
        }
        else
        {
            Node* tmp = new Node();
            tmp->setContent(s[i]);
            current->appendChild(tmp);
            current = tmp;
        }
        if ( i == s.length() - 1 )
            current->setWordMarker();
    }
}

template <typename K, typename V, typename I>
void trie<K, V, I>::remove(K const * key, size_t len) {
}

template <typename K, typename V, typename I>
typename trie<K, V, I>::node_t const * trie<K, V, I>::insert(K const * key, size_t len) {
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

#endif

}}} // end namespace

#endif // sentry

