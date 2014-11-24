#ifndef intent_core_text_trie_h
#define intent_core_text_trie_h

#include <cstdint>
#include <limits>
#include <vector>

#include "core/text/trie.h"
#include "core/util/value_semantics.h"

namespace intent {
namespace core {
namespace text {

enum class insert_type : char;
static constexpr size_t calc_key_len = std::numeric_limits<size_t>::max();

template <typename K, typename V, typename I=uint16_t>
class trie {

public:
    typedef K key_t;
    typedef V value_t;
    typedef I index_t;
    typedef std::vector<I> indexes_t;

    struct node_t {
        K key;
        V value;
        indexes_t children;

        node_t();
        node_t(K const &, V const &);
        ~node_t();

        bool is_leaf() const;
    };

private:
    typedef std::vector<node_t> nodes_t;
    nodes_t nodes;

public:
    trie(size_t start_size=256);
    ~trie();
    MOVEABLE_BUT_NOT_COPYABLE(trie);

    /**
     * Insert a new item.
     * @return true if the item is new and was inserted, false if item was a dup.
     */
    bool insert(K const * key, size_t len = calc_key_len);

    /**
     * Find the specified key in the trie.
     * @return address of node containing the item, if found, nullptr otherwise.
     */
    node_t * find(K const * key, size_t len = calc_key_len);

    /**
     * Find the specified key in the trie.
     * @return address of node containing the item, if found, nullptr otherwise.
     */
    node_t const * find(K const * key, size_t len = calc_key_len) const;

    /**
     * @return true if the item is in the trie, false if not.
     */
    bool contains(K const * key, size_t len = calc_key_len) const;

private:
    static constexpr index_t max_index = std::numeric_limits<index_t>::max();

    typename indexes_t::iterator find_insert_position(K const * key, size_t len, insert_type &) const;

    node_t & get_root();
    node_t const & get_root() const;
};

#if 0
void Trie::addWord(string s)
{
    Node* current = root;

    if ( s.length() == 0 )
    {
        current->setWordMarker(); // an empty word
        return;
    }

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


bool Trie::searchWord(string s)
{
    Node* current = root;

    while ( current != NULL )
    {
        for ( int i = 0; i < s.length(); i++ )
        {
            Node* tmp = current->findChild(s[i]);
            if ( tmp == NULL )
                return false;
            current = tmp;
        }

        if ( current->wordMarker() )
            return true;
        else
            return false;
    }

    return false;
}
#endif

}}} // end namespace

#include "core/text/trie-inline.h"

#endif // sentry
