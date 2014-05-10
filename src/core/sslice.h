#ifndef _0345e71582f34fa59792443fba711afd
#define _0345e71582f34fa59792443fba711afd

#include <cstdlib>
#include <iosfwd>

/**
 * A subset of a string that doesn't own the memory it refers to.
 */
struct sslice {
    /**
     * Create an empty sslice that begins and ends with nullptr.
     */
    sslice();

    /**
     * Create a sslice that includes an entire C string.
     */
    sslice(char const * cstr);

    /**
     * Create a sslice from a subset of a C string.
     */
    sslice(char const * cstr, char const * end);

    /**
     * Create a sslice from a C string and a size.
     */
    sslice(char const * cstr, size_t len);
    
    /**
     * Create one sslice from another, specifying a different
     * length.
     */
    sslice(sslice const & rhs, size_t len);

    /**
     * @return Number of bytes that this slice includes.
     */
    size_t size() const;

    /**
     * @return true if slice is empty.
     */
    bool empty() const;

    /**
     * @return true if slice contains text.
     */
    operator bool() const;

    /**
     * Slices are == IFF their begin and end pointers are ==. Not the same as
     * string compare.
     */
    bool operator ==(sslice const & rhs) const;
    bool operator !=(sslice const & rhs) const;

    /**
     * Change value of sslice.
     */
    sslice & assign(char const * cstr);
    sslice & assign(char const * cstr, char const * end);

    char const * begin;
    char const * end;
};

extern const sslice null_sslice;

int strcmp(char const * a, sslice const & b);
int strcmp(sslice const & a, char const * b);
int strcmp(sslice const & a, sslice const & b);
int strncmp(char const * a, sslice const & b, size_t size);
int strncmp(sslice const & a, char const * b, size_t size);
int strncmp(sslice const & a, sslice const & b, size_t size);

std::ostream & operator<<(std::ostream & out, const sslice & slice);

#include "core/sslice-inline.h"

#endif
