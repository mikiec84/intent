#ifndef intent_core_trie_inline_h
#define intent_core_trie_inline_h

#include <limits>
#include "trie.h"

namespace intent {
namespace core {

#include <vector>

static constexpr size_t COMPUTE_LEN = std::numeric_limits<size_t>::max();

template <typename K, typename V>
class trie {
    struct node;
    node * root;
public:
    trie();
    ~trie();

    void insert(K const * key, size_t len = COMPUTE_LEN, V value);
    void remove(K const * key, size_t len = COMPUTE_LEN);
    node const * find(K const * key, size_t len = COMPUTE_LEN);
};

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

}} // end namespace

#include "trie-inline.h"

#endif // sentry
