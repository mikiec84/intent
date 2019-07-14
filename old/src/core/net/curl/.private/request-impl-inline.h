#ifndef _77730a1b28e645cd9733bb7d8ac017b8
#define _77730a1b28e645cd9733bb7d8ac017b8

#include "core/net/curl/.private/request-impl.h"
#include "core/text/strutil.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


extern const char * const standard_get_verb;
extern const char * const standard_put_verb;
extern const char * const standard_post_verb;
extern const char * const standard_head_verb;
extern const char * const standard_options_verb;

uint32_t get_next_request_id();


inline request::impl_t::impl_t(class session * s):
        id(get_next_request_id()),
        session(s),
        verb(nullptr),
        url(nullptr),
        headers(),
        ref_count(1) {
}


inline request::impl_t::~impl_t() {
    free_verb();
    if (url) {
        free(url);
    }
}


inline void request::impl_t::add_ref() const {
    ++ref_count;
}


inline void request::impl_t::release_ref() const {
    if (ref_count.fetch_sub(1) == 1) {
        delete this;
    }
}


inline void request::impl_t::set_url(char const * value) {
    if (url) {
        free(url);
    }
    if (value && *value) {
        url = strdup(value);
    }
}


inline void request::impl_t::set_verb(char const * value) {
    using text::compare_str_ascii_case_insensitive;

    // Avoid the common case where a request's verb is set to what it already
    // contained.
    if (verb && value && compare_str_ascii_case_insensitive(verb, value) == 0) {
        return;
    }

    free_verb();

    if (value) {
        // To avoid thrashing the heap with lots of tiny allocs that store the
        // same string over and over again, don't alloc any of the 5 most common
        // strings.
        char const * standard = nullptr;
        if (compare_str_ascii_case_insensitive(standard_get_verb, value) == 0) {
            standard = standard_get_verb;
        } else if (compare_str_ascii_case_insensitive(standard_post_verb, value) == 0) {
            standard = standard_post_verb;
        }  else if (compare_str_ascii_case_insensitive(standard_put_verb, value) == 0) {
            standard = standard_put_verb;
        } else if (compare_str_ascii_case_insensitive(standard_head_verb, value) == 0) {
            standard = standard_head_verb;
        } else if (compare_str_ascii_case_insensitive(standard_options_verb, value) == 0) {
            standard = standard_options_verb;
        } else {
            verb = strdup(value);
            return;
        }
        verb = const_cast<char *>(standard);
    }
}


inline void request::impl_t::free_verb() {
    if (verb) {
        if (verb != standard_get_verb
                && verb != standard_post_verb
                && verb != standard_put_verb
                && verb != standard_head_verb
                && verb != standard_options_verb) {
            free(verb);
        }
        verb = nullptr;
    }
}


}}}} // end namespace


#endif
