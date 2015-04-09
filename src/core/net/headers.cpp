#include <atomic>
#include <map>

#include "core/util/dbc.h"
#include "core/net/headers.h"
#include "core/text/strutil.h"


using std::string;
using intent::core::text::compare_str_ascii_case_insensitive;


namespace intent {
namespace core {
namespace net {

typedef bool (*header_is_less_func)(string const & a, string const & b);

bool header_is_less(string const & a, string const & b) {
    return compare_str_ascii_case_insensitive(a.c_str(), b.c_str()) < 0;
}

typedef std::map<string, string, header_is_less_func> headers_map;

struct headers::impl_t {
    mutable std::atomic<unsigned> ref_count;
    headers_map map;
    impl_t * defaults;

    void add_ref() const {
        ++ref_count;
    }

    void release_ref() const {
        if (ref_count.fetch_sub(1) == 1) {
            delete this;
        }
    }

    impl_t() : ref_count(1), map(header_is_less), defaults(nullptr) {
    }

    ~impl_t() {
        if (defaults) {
            defaults->release_ref();
        }
    }

    char const * get(string const & header) {
        auto x = map.find(header);
        if (x != map.end()) {
            return x->second.c_str();
        }
        if (defaults) {
            return defaults->get(header);
        }
        return nullptr;
    }

    header_status contains(string const & header) const {
        auto x = map.find(header);
        if (x != map.end()) {
            return header_status::explicitly_defined;
        }
        if (defaults) {
            if (defaults->contains(header) != header_status::missing) {
                return header_status::inherited;
            }
        }
        return header_status::missing;
    }

    bool is_ancestor(impl_t * x) const {
        if (x == this || x == defaults) {
            return true;
        }
        return defaults ? defaults->is_ancestor(x) : false;
    }

    /**
     * Checks to see if a particular header is assigned a value in me or
     * any of my ancestor impls, up to but not including the impl passed
     * as the "before" arg.
     */
    bool is_masked(string const & header, impl_t const * before) const {
        impl_t const * x = this;
        while (x && x != before) {
            auto iter = x->map.find(header);
            if (iter != x->map.end()) {
                return true;
            }
            x = x->defaults;
        }
        return false;
    }

    impl_t const * get_ancestor_after(impl_t const * after) const {
        impl_t const * x = this;
        while (x) {
            if (x->defaults == after) {
                return x;
            }
            x = x->defaults;
        }
        return nullptr;
    }
};


headers::headers() : impl(new impl_t()) {
}


headers::headers(impl_t * x) : impl(x) {
}


headers & headers::operator =(headers && other) {
    precondition(other.impl != nullptr);
    precondition(impl == nullptr || !impl->is_ancestor(other.impl));
    if (impl) {
        impl->release_ref();
    }
    impl = std::move(other.impl);
    other.impl = nullptr;
    return *this;
}


headers & headers::operator =(headers const & other) {
    precondition(other.impl != nullptr);
    precondition(impl == nullptr || !impl->is_ancestor(other.impl));
    if (impl) {
        impl->release_ref();
    }
    impl = other.impl;
    impl->add_ref();
    return *this;
}


headers::~headers() {
    impl->release_ref();
}


void headers::get_defaults_from(headers h) {
    precondition(h.impl != nullptr);
    precondition(impl == nullptr || !impl->is_ancestor(h.impl));
    if (impl->defaults) {
        impl->defaults->release_ref();
    }
    impl->defaults = h.impl;
    impl->defaults->add_ref();
}


char const * headers::get(text::str_view header) {
    if (header) {
        std::string h(header.begin, header.end());
        return impl->get(h);
    }
    return nullptr;
}


header_status headers::contains(text::str_view header) const {
    if (header) {
        std::string h(header.begin, header.end());
        return impl->contains(h);
    }
    return header_status::missing;
}


void headers::set(text::str_view header, text::str_view value) {
    if (header) {
        std::string h(header.begin, header.end());
        impl->map[h] = std::string(value.begin, value.end());
    }
}


bool headers::is_empty() const {
    impl_t * x = impl;
    while (x) {
        if (x->map.size() > 0) {
            return false;
        }
        x = x->defaults;
    }
    return true;
}


unsigned headers::get_header_count() const {
    unsigned i = 0;
    // Get my most remote ancestor that provides defaults, then work forward to
    // me, adding each ancestor's contributed header values.
    impl_t const * ancestor = impl->get_ancestor_after(nullptr);
    while (ancestor && ancestor != impl) {
        auto end = ancestor->map.end();
        for (auto i = ancestor->map.begin(); i != end; ++i) {
            if (!impl->is_masked(i->first, ancestor)) {
                ++i;
            }
        }
        ancestor = impl->get_ancestor_after(ancestor);
    }
    i += impl->map.size();
    return i;
}


char const * headers::get_header_by_index(unsigned i) const {
    unsigned j = 0;
    // Get my most remote ancestor that provides defaults, then work forward to
    // me, adding each ancestor's contributed header values.
    impl_t const * ancestor = impl->get_ancestor_after(nullptr);
    while (ancestor) {
        auto end = ancestor->map.end();
        for (auto iter = ancestor->map.begin(); iter != end; ++iter) {
            if (!impl->is_masked(iter->first, ancestor)) {
                if (j++ == i) {
                    return iter->second.c_str();
                }
            }
        }
        ancestor = impl->get_ancestor_after(ancestor);
    }
    return nullptr;
}


}}} // end namespace
