#ifndef _0345e71582f34fa59792443fba711afd
#define _0345e71582f34fa59792443fba711afd

#include <cstdlib>

/**
 * A slice of a string that doesn't own the memory it
 * refers to.
 */
class token {
public:
    /**
     * Create a token that includes an entire C string.
     */
    token(char const * cstr);

    /**
     * Create a token from a subset of a C string.
     */
    token(char const * cstr, char const * end);

    /**
     * Create a token from a C string and a size.
     */
    token(char const * cstr, size_t len);
    
    /**
     * Copy another token's state. Neither token owns
     * the memory it points to; this is just a simple memcpy.
     */
    token(token const & rhs);
    
    /**
     * Create one token from another, specifying a different
     * length.
     */
    token(token const & rhs, size_t len);

    size_t size() const;

private:
    char const * txt;
    char const * end;
};

extern const token null_token;

#include "core/token-inline.h"

#endif
