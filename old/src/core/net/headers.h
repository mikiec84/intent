#ifndef _1a370e76942a4aa6ab7fb1d0596788a5
#define _1a370e76942a4aa6ab7fb1d0596788a5

#include <initializer_list>
#include <memory>
#include <string>

#include "core/text/str_view.h"


namespace intent {
namespace core {
namespace net {


/**
 * Describe how a headers collection provides a value for a particular
 * header.
 */
enum class header_status: unsigned {
    /** Header is not recognized/has no value. */
    missing,
    /** Header has a value defined directly in this object. */
    explicitly_defined,
    /** Header has a value inherited from another layer of headers. */
    inherited
};


/**
 * Many network protocols use the concept of "headers" to pass key ~ value
 * pairs back and forth; this is a general implementation suitable for use
 * in http, smtp, and so forth.
 *
 * Supports key (header) ~ value mappings with case-insensitive keys.
 * Provides efficient value semantics using a ref-counted strategy--that is,
 * you can return or assign a headers object by value, with negligible cost.
 * Also offers layering, so that default headers can be combined with headers
 * that override the defaults, without endlessly duplicating key ~ value pairs
 * in multiple maps.
 */
class headers {
    struct impl_t;
    impl_t * impl;

    headers(impl_t *);

public:
    headers();
    headers(std::initializer_list<char const *>);
    headers(headers const & other): headers(nullptr) { *this = other; }
    headers(headers && other): headers(nullptr) { *this = other; }
    headers & operator =(headers &&);
    headers & operator =(headers const &);
    ~headers();

    /**
     * Tell a headers object to use another headers object for any mappings
     * it doesn't override.
     */
    void get_defaults_from(headers);

    /**
     * Set the value for a header.
     *
     * @param header The header to set, such as "User-Agent". Header names
     *     are not case-sensitive.
     * @param value The value for the header. A null or empty value does not
     *     remove the header from the map; it simply makes the header's value
     *     empty (contrast with {@link #remove}).
     */
    void set(text::str_view header, text::str_view value);

    /**
     * Add a line in the form Header-Name: value.
     */
    void add(text::str_view line);

    /**
     * Remove a header.
     */
    void remove(text::str_view header);

    /**
     * @return Desciption of how/whether a header has a value in this collection.
     */
    header_status contains(text::str_view header) const;

    /**
     * @return The value of a header, or nullptr if not found.
     */
    char const * get(text::str_view header);

    bool is_empty() const;

    /**
     * Remove values for all headers.
     */
    void clear();

    /**
     * Allows enumeration.
     */
    unsigned get_header_count() const;

    /**
     * @return The name of a header.
     */
    char const * get_header_by_index(unsigned i) const;
};


}}} // end namespace


#endif // sentry
