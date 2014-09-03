#include <cstdint>
#include <vector>

#include "dbc.h"
#include "ioutil.h"
#include "xsv_file.h"

using std::vector;
using std::pair;

namespace intent {
namespace core {

typedef uint32_t idx_t;

static constexpr idx_t MAX_RECORD_SIZE = 1024 * 1024;
static constexpr idx_t INITIAL_BUF_SIZE = 8 * 1024;

struct xsv_file::data_t {
    FILE * f;
    char * buf;
    size_t buf_size;
    size_t content_size;
    char * p;
    char * end;

    typedef pair<idx_t, idx_t> field_boundaries;

    static_assert(std::is_same<decltype(field_boundaries::first),
            decltype(field_boundaries::second)>::value,
            "field boundary types should be identical");

    static_assert(std::numeric_limits<decltype(field_boundaries::first)>::max()
            >= MAX_RECORD_SIZE,
            "indexes must be capable of representing any offset <= MAX_RECORD_SIZE");

    static_assert(MAX_RECORD_SIZE % INITIAL_BUF_SIZE == 0,
            "MAX_RECORD_SIZE must be a multiple of INITIAL_BUF_SIZE");

    typedef vector<field_boundaries> fields_t;
    fields_t fields;
    char delim;

    data_t() : buf(nullptr), buf_size(0), content_size(0), p(nullptr),
            end(nullptr) {
    }

    ~data_t() {
        if (buf) {
            free(buf);
        }
        if (f) {
            fclose(f);
        }
    }

    inline bool is_writable() const { return buf == nullptr; }

    inline void shift(idx_t n) {
        content_size -= n;
        memmove(buf, buf + n, content_size);
    }

    inline char * extend() {
        size_t offset = p - buf;
        if (content_size < MAX_RECORD_SIZE) {
            auto remaining = buf_size - content_size;
            if (remaining == 0) {
            }
            return buf + offset;
        } else {
            delete buf;
            buf = nullptr;
            return nullptr;
        }
    }

    inline char next_char(idx_t & field_begin) {
        // If we reached the end of the content we're currently holding in the
        // buffer without finding the end of a record, see if we can read more
        // data. This may or may not grow the buffer, depending on whether it
        // is currently full.
        if (p == end) {
            size_t old_field_begin_offset = (field_begin ? field_begin : 0);
            if ((p = extend()) == nullptr) {
                if (p > buf) {
                    add_field(p);
                    return true;
                }
                return false;
            }
            end = buf + content_size;
            field_begin = old_field_begin_offset;
        }
        return *p++;
    }

    inline void add_field(char const * p) {
#if 0
        uint16_t fbegin = static_cast<uint16_t>(field_begin - buf);
        uint16_t fend = static_cast<uint16_t>(p - field_begin);
        fields.push_back(field_boundaries(fbegin, fend));
#endif
    }

};

xsv_file::xsv_file(xsv_file && rhs): data(nullptr) {
    *this = std::move(rhs);
}

xsv_file xsv_file::open_for_read(char const * fname, char delim) {
    PRECONDITION(fname && *fname);
    PRECONDITION(delim);
    xsv_file x;
    x.data->f = fopen(fname, "r");
    PRECONDITION(x.data->f);
    x.data->buf_size = 1024;
    x.data->buf = reinterpret_cast<char *>(malloc(x.data->buf_size));
    x.data->delim = delim;
    return std::move(x);
}

xsv_file xsv_file::open_for_write(char const * fname, char delim) {
    PRECONDITION(fname && *fname);
    PRECONDITION(delim);
    xsv_file x;
    x.data->f = fopen(fname, "w");
    PRECONDITION(x.data->f);
    x.data->delim = delim;
    return std::move(x);
}

bool xsv_file::is_writable() const {
    return data->is_writable();
}

size_t xsv_file::get_field_count() const {
    return data->fields.size();
}

char const * xsv_file::get_field_by_index(size_t i) const {
    return (i < data->fields.size()) ? data->buf + data->fields[i].first : nullptr;
}

xsv_file & xsv_file::operator =(xsv_file && other) {
    if (data) {
        delete data;
    }
    data = std::move(other.data);
    other.data = nullptr;
    return *this;
}

bool xsv_file::next_record() {
    if (!data->is_writable()) {
        return false;
    }

    data->fields.clear();

    char c;
    bool more_fields = true;
    bool in_quote = false;
    uint16_t field_begin = 0;
    char * p = data->buf;
    //char * write_ptr = p;

    for ( ; more_fields && (c = next_char() != 0); ++p, ++field_begin) {
        char c = *p;
        switch (c) {
        case '\n':
        case '\r':
            more_fields = false;
            break;
        case '"':
            if (!in_quote) {
                ++field_begin;
            } else {
            }
            in_quote = !in_quote;
            break;
#if 0
        case
        if (c == data->delim) {
            *write_ptr = 0;
            add_field(fields, field_begin, p);
            field_begin += 2;
        } else if (c == '\r' || c == '\n') {
            *write_ptr = 0;
            add_field(fields, field_begin, p);
            ++field_begin;
            if (c == '\r' && (c = next_char() == '\n')) {
                ++field_begin;
            }
            data->shift();
        } else {
            if (write_ptr < p) {
                *write_ptr = c;
            }
        }
            *write_ptr = 0;
            add_field(fields, field_begin, p);
            ++field_begin;
            if (c == '\r' && (c = next_char() == '\n')) {
                ++field_begin;
            }
            data->shift();
#endif
        }
    }
    return false;
}

xsv_file::~xsv_file() {
    if (data) {
        if (data->f) {
            fclose(data->f);
        }
        delete data;
    }
}

}} // end namespace

