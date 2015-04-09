#ifndef _b740fd84ca6c4d6598d0cdd86fca006b
#define _b740fd84ca6c4d6598d0cdd86fca006b

#include <cstdlib>
#include <iosfwd>
#include <string>

namespace intent {
namespace core {
namespace text {

/**
 * A subset of a string that doesn't own the memory it references, and that
 * is designed for a short lifetime (e.g., for use as a function arg). Allows
 * efficient slicing and dicing of text without making endless copies.
 * The lifetime of the string data wrapped by a strview must exceed the
 * lifetime of strview.
 */
template<typename C>
struct strview {
    /**
     * Create an empty strview that begins and ends with nullptr.
     */
    strview();

    /**
     * Create a strview that includes an entire C string.
     */
    strview(C const * cstr);

    /**
     * Create a strview from a subset of a C string.
     */
    strview(C const * cstr, C const * end);

    /**
     * Create a strview from a C string and a size.
     */
    strview(C const * cstr, size_t len);

    /**
     * Create a strview from a std::string.
     */
    strview(std::basic_string<C> const & str);

    /**
     * Create a strview from a std::string and a length.
     */
    strview(std::basic_string<C> const & str, size_t len);

    /**
     * Create one strview from another, specifying a different
     * length.
     */
    strview(strview const & rhs, size_t len);

    /**
     * Make state equal to result of default ctor.
     */
    void make_null();

    /**
     * Change where view begins, without changing where it ends. If new begin
     * is >= end(), length becomes 0.
     */
    void begin_at(C const * new_begin);

    /**
     * Change where view ends, without changing where it begins. Has no effect
     * if begin == nullptr. If new_end < begin, begin is not changed, but length
     * becomes 0.
     */
    void end_at(C const * new_end);

    /**
     * @return First char beyond end of this view.
     */
    C const * end() const;

    /**
     * @return true if slice has length 0. Note: this is not the same as
     *     being null!
     */
    bool is_empty() const;

    /**
     * @return true if slice points to nullptr and has length 0.
     */
    bool is_null() const;

    /**
     * @return true if slice is not empty (and therefore, not null as well).
     */
    operator bool() const;

    /**
     * Slices are == IFF their begin and end pointers are ==. Not the same as
     * string compare.
     */
    bool operator ==(strview const & rhs) const;
    bool operator !=(strview const & rhs) const;

    /**
     * Change value of strview.
     */
    strview & assign(C const * cstr);
    strview & assign(C const * cstr, C const * end);
    strview & assign(C const * cstr, size_t length);

    C const * begin;

    // A strview really has 3 core attributes: begin, end, and len. The
    // begin pointer must be stored in the object, and one of the other members
    // must be stored as well. Initially we stored end and computed len; it
    // it seemed simpler to have a pair of the same datatype. However, later
    // experience shows that len is the better choice to store, because:
    //   1) if the string is relocated, only one member must be adjusted;
    //   2) It may be possible to compress len in some situations, allocating
    //      less memory for massive arrays of strview.
    size_t length;
};

typedef strview<char> str_view;
typedef strview<wchar_t> wstr_view;

template <typename C>
int strcmp(C const * a, strview<C> const & b);

template <typename C>
int strcmp(strview<C> const & a, C const * b);

template <typename C>
int strcmp(strview<C> const & a, strview<C> const & b);

template <typename C>
int strncmp(C const * a, strview<C> const & b, size_t length);

template <typename C>
int strncmp(strview<C> const & a, C const * b, size_t length);

template <typename C>
int strncmp(strview<C> const & a, strview<C> const & b, size_t length);

}}} // end namespace

std::ostream & operator<<(std::ostream & out, const intent::core::text::str_view &);
std::ostream & operator<<(std::ostream & out, const intent::core::text::wstr_view &);

#include "core/text/str_view-inline.h"

#endif
