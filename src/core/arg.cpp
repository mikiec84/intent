#include <boost/filesystem.hpp>
#include <cstdio>
#include <cstring>

#include "core/arg.h"
#include "core/sslice.h"

using std::string;

arg::~arg() {
    if (type == vt_allocated_str) {
        delete[] allocated_str;
    }
}

arg::arg() : type(vt_empty) {
}

arg const & make_empty_arg() {
    static arg the_empty_arg;
    return the_empty_arg;
}

arg const & arg::empty = make_empty_arg();

int arg::snprintf(char * buf, size_t buflen, char const * format) const {
    // I don't know whether snprintf() will accept a nullptr as an arg. To
    // avoid passing one, we can use this static buffer instead. The buffer
    // will never be written, because the contract for posix's snprintf is
    // that it will write at most buflen characters, and we're going to tell
    // it that it can write 0 characters. Thus, this buffer doesn't have to
    // be threadsafe.
    static char not_used[1] = {0};
    if (buf == nullptr) {
        buf = not_used;
        buflen = 0;
    }

    std::string const * s = nullptr;
    switch (type) {
    case vt_empty:
        return ::snprintf(buf, buflen, (format ? format : "%s"), "");
    case vt_signed:
        return ::snprintf(buf, buflen, (format ? format : "%lld"), i64);
    case vt_unsigned:
        return ::snprintf(buf, buflen, (format ? format : "%llu"), u64);
    case vt_float:
        return ::snprintf(buf, buflen, (format ? format : "%g"), dbl);
    case vt_bool:
        return ::snprintf(buf, buflen, "%s", (boolean ? "true" : "false"));
    case vt_date:
        return 0;
    case vt_path:
        s = &path->native();
        // DO NOT BREAK
    case vt_str:
        if (s == nullptr) {
            s = str;
        }
        // On overflow, don't ask snprintf() to do any calculations; we
        // already know the answer.
        if (buflen < s->size() + 1) {
            return s->size();
        }
        if (format == nullptr) {
            // Use memcpy() to support embedded nulls.
            ::memcpy(buf, s->c_str(), s->size() + 1);
            return s->size();
        }
        return ::snprintf(buf, buflen, (format ? format : "%s"), s->c_str());
    case vt_sslice:
    {
        size_t required_size = slice->size() + 1;
        // If we are just going to calculate a size instead of actually printing
        // something, don't bother calling snprintf(). This isn't just an optimization;
        // in one impl of snprintf, the observed behavior was that snprintf() took
        // strlen() of the arg, which would be wrong for a non-null-terminated sslice.
        if (buflen < required_size) {
            return slice->size();
        }
        return ::snprintf(buf, required_size, (format ? format : "%s"),
                          (*slice ? slice->begin : ""));
    }
    case vt_cstr:
    case vt_allocated_str:
        return ::snprintf(buf, buflen, (format ? format : "%s"), (cstr ? cstr : "(null)"));
    default:
        return 0;
    }
}

string arg::to_string(char const * format) const {

    switch (type) {
    case vt_empty:
        return "";
    case vt_str:
        return *str;
    case vt_sslice:
        return string(slice->begin, slice->end);
    case vt_path:
        return path->native();
    case vt_cstr:
    case vt_allocated_str:
        return cstr ? cstr : "(null)";
    case vt_bool:
        return boolean ? "true" : "false";
    default:
    {
        char buf[36];
        switch (type) {
        case vt_signed:
            ::snprintf(buf, sizeof(buf), (format ? format : "%lld"), i64);
            return buf;
        case vt_unsigned:
            ::snprintf(buf, sizeof(buf), (format ? format : "%llu"), i64);
            return buf;
        case vt_float:
            ::snprintf(buf, sizeof(buf), (format ? format : "%g"), dbl);
            return buf;
        case vt_date:
            return "";
        default:
            return "";
        }
    }
    }
}


