#include <cstdint>
#include <vector>

#include "core/util/dbc.h"
#include "core/io/ioutil.h"
#include "core/data/xsv.h"

using std::vector;
using std::pair;

using intent::core::io::c_file;

namespace intent {
namespace core {
namespace data {

typedef uint32_t idx_t;

static constexpr idx_t MAX_RECORD_SIZE = 1024 * 1024;
static constexpr idx_t INITIAL_ALLOCED_SIZE = 8 * 1024;

struct xsv_reader::data_t {
    c_file * f;
    char * buf;
    char * p;
    char * end;
    idx_t alloced_size;
    idx_t content_size;

    typedef pair<idx_t, idx_t> field_boundaries;

    static_assert(std::numeric_limits<decltype(field_boundaries::first)>::max()
            >= MAX_RECORD_SIZE,
            "indexes must be capable of representing any offset <= MAX_RECORD_SIZE");

    static_assert(MAX_RECORD_SIZE % INITIAL_ALLOCED_SIZE == 0,
            "MAX_RECORD_SIZE must be a multiple of INITIAL_ALLOCED_SIZE");

    typedef vector<field_boundaries> fields_t;
    fields_t fields;
    char delim;

    data_t(char c) : f(nullptr), buf(nullptr), p(nullptr), end(nullptr),
            alloced_size(0), content_size(0), fields(), delim(c) {
    }

    ~data_t() {
        // Only free buf if we allocated it.
        if (alloced_size && buf) {
            free(buf);
        }
        delete f;
    }

    inline void shift(idx_t n) {
        content_size -= n;
        memmove(buf, buf + n, content_size);
    }

    inline char * extend(char * p) {
        auto current_offset = p - buf;
        bool extended = false;

        if (f) {
            // Do we have room to read in more content?
            if (content_size < MAX_RECORD_SIZE) {

                // Do we have any unused buffer space?
                auto bytes_to_read = alloced_size - content_size;

                // If no, then try growing the buffer.
                if (bytes_to_read == 0 && alloced_size < MAX_RECORD_SIZE) {
                    auto new_alloced_size = std::min(MAX_RECORD_SIZE,
                            std::max(alloced_size * 2, INITIAL_ALLOCED_SIZE));
                    buf = reinterpret_cast<char *>(realloc(buf, new_alloced_size));
                    if (!buf) {
                        throw std::bad_alloc();
                    }
                    alloced_size = new_alloced_size;
                    bytes_to_read = alloced_size - content_size;
                }

                // See what we can read from our file.
                auto bytes_read = fread(buf + content_size, 1, bytes_to_read, *f);
                extended = (bytes_read > 0);

            } else {
                extended = false;
            }

            // Close file as soon as we're done with it; do not wait until
            // reader is destroyed.
            if (feof(*f)) {
                delete f;
                f = nullptr;
            }
        }
        return extended ? buf + current_offset : nullptr;
    }

    inline char next_char() {
        // If we reached the end of the content we're currently holding in the
        // buffer without finding the end of a record...
        if (p >= end) {

            // See if we can read more.
            auto resume_point = extend(p);

            // If we couldn't get any more data, then finish with what we have.
            if (resume_point == nullptr) {

                // If we started a field, end it.
                if (p > buf) {
                    add_field(p);
                }

                return 0; // final null terminator indicates end of input.
            } else {
                p = resume_point;
            }
            end = buf + content_size;
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

xsv_reader::xsv_reader(xsv_reader && rhs): data(nullptr) {
    *this = std::move(rhs);
}

xsv_reader::xsv_reader(char * txt, char delim): data(nullptr) {
    precondition(txt);
    precondition(delim);
    data = new data_t(delim);
    data->buf = txt;
    data->content_size = strlen(txt);
}

xsv_reader::xsv_reader(std::string & txt, char delim) :
    // Casting .data() to a modifiable char array is a bit odd, but harmless.
    // Before C++11, there was no guarantee that .data() would yield a null-
    // terminated buffer, or that it pointed to the same thing as .c_str()--
    // but now, that guarantee holds. For more details, see the C++11 tab on:
    //     http://www.cplusplus.com/reference/string/string/data/.
    // Since the string is mutable, altering its internal buffer should be
    // harmless, as long as we don't need extra space. And we don't.
    xsv_reader(const_cast<char *>(txt.data()), delim) {
}

size_t xsv_reader::get_field_count() const {
    return data->fields.size();
}

char const * xsv_reader::get_field_by_index(size_t i) const {
    return (i < data->fields.size()) ? data->buf + data->fields[i].first : nullptr;
}

xsv_reader & xsv_reader::operator =(xsv_reader && other) {
    if (data) {
        delete data;
    }
    data = std::move(other.data);
    other.data = nullptr;
    return *this;
}

#if 0
bool xsv_reader::next_record() {
    data->fields.clear();

    char c;
    bool more_fields = true;
    bool in_quote = false;
    uint16_t field_begin = 0;
    char * p = data->buf;
    //char * write_ptr = p;

    for ( ; more_fields && (c = data->next_char() != 0); ++p, ++field_begin) {
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
        }
    }
    return false;
}
#endif

xsv_reader::xsv_reader(c_file && file, char delim) {
}

xsv_reader::xsv_reader(boost::filesystem::path const & path, char delim) :
    xsv_reader(c_file(path, "r"), delim) {
}

xsv_reader::~xsv_reader() {
    delete data;
}

}}} // end namespace

