#include <array>
#include <vector>

#include "core/text/str_view.h"
#include "core/text/strutil.h"
#include "core/cli/ansi_color_palette.h"
#include "core/util/dbc.h"

using std::array;
using std::tuple_size;
using std::vector;

using intent::core::text::is_null_or_empty;
using intent::core::text::str_view;

namespace intent {
namespace core {
namespace cli {

static inline char next_name_char(char const * &p, char const * end) {
    while (p < end) {
        char c = *p++;
        if (c >= 'a' && c <= 'z') return c;
        if (c >= 'A' && c <= 'Z') return c + 32;
        if (c >= '0' && c <= '9') return c;
        if (c != '.' && c != '-' && c != '_') break;
    }
    return 0;
}

static inline bool find_next_name(str_view & txt, str_view & name) {
    name.make_null();
    auto p = txt.begin;
    auto end = txt.end();
    while (p < end) {
        char c = *p;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'z')) {
            name.begin = p;
            txt.begin_at(p);
            break;
        } else {
            ++p;
        }
    }
    if (!name.begin) return false;
    while (p < end) {
        char c = *p;
        if (isalnum(c) || c == '.' || c == '-' || c == '_') {
            ++p;
        } else {
            name.end_at(p);
            txt.begin_at(p);
            break;
        }
    }
    return true;
}

static inline bool find_next_value(str_view & txt, uint8_t & value) {
    bool found_equal = false;
    const auto end = txt.end();
    char const * p = txt.begin;
    while (p < end) {
        auto c = *p++;
        switch (c) {
        case '=':
            if (found_equal) return false;
            found_equal = true;
            txt.begin_at(p);
            break;
        case ' ':
            txt.begin_at(p);
            break;
        default:
            if (!found_equal || c < '0' || c > '9') return false;
            value = (c - '0');
            if (p < end) {
                c = *p;
                if (c >= '0' && c <= '9') {
                    value *= 10 + (c - '0');
                    ++p;
                }
            }
            txt.begin_at(p);
            return true;
        }
    }
    return false;
}

typedef std::array<char, 16> spec_entry_t;
typedef std::array<spec_entry_t, 240> spec_entries_t;

static inline void copy_name(str_view const & src, char * dest) {
    auto p = src.begin;
    auto end = src.end();
    char c = next_name_char(p, end);
    auto dest_end = dest + tuple_size<spec_entry_t>::value - 2;
    for (; c && dest < dest_end; c = next_name_char(p, end)) {
        *dest++ = c;
    }
    *dest = 0;
}

/*
Consider this spec:

    header=14
    see-also=2
    callout=10
    error=9
    cross-ref=15
    warning=11
    keyword=6
    optional=4
    normal=8
    note=3
    hidden=0

We want to build a series of mini arrays that tell us, for each significant
letter in each color name, which array contains info about its subsequent chars,
or which core color it resolves to. There are various ways to store this, but
for debuggability and speed I've chosen fixed-size arrays of 36 bytes, since
there are 36 possible characters that can be used in names.

The initial letters for entries in our spec are: c (2 of them), e, h (2), k,
n (2), o, s, and w.

Given this, the first 36-byte array we want to build (let's call it the "base"
array) should look like this:

[0] = 0xFF -- index 0 is the position in the array for the letter 'a'; there are
              no letter 'a' occurrences, so 'a' doesn't require checks of a
              subsequent array, and doesn't map to any color. 0xFF is the value
              reserved for telling us to end search with a failure.)
[1] = 0xFF -- likewise for 'b'
[2] = 17   -- index 2 is the position in the array for the letter 'c'. Since we
              have more than 1 entry with a name starting with 'c', we need at
              least one additional array to disambiguate. Where will this array
              be found? The number 17 tells us. If we had seen a number <= 15,
              we would know that it represents a literal color. Arrays are
              numbered beginning with the base array (the one we're defining),
              which is 16. The next available array is 17; we'll allocate it for
              disambiguation of 'c' and record that now. We can seek to whatever
              array we want using: start_of_base_array + (16 - n) * 36. Thus,
              the array to disambiguate c is located 36 bytes after base.
[3] = 0xFF -- 'd' is not present
[4] = 9    -- 'e' is a long enough sequence of chars to uniquely identify "error"
              in the spec, so we terminate the search with success and provide
              the resulting color value (9) here.
[5]..[6]   -- =0xFF; 'f', 'g' not present
[7] = 18   -- the array to disambiguate 'h' will be #18, at offset base + 72.
[8]..[9]   -- =0xFF; 'i', 'j' not present
[10] = 6   -- 'k' maps color 6 (end of handling of "keyword")
[11]..[12] -- =0xFF; 'l', 'm' not present
[13] = 19  -- 'n' gets array at base + 108 to disambiguate
[14] = 4   -- 'o' maps to color 4 (end of handling of "optional")
[15]..[17] -- =0xFF; 'p', 'q', 'r' not present
[18] = 2   -- 's' maps to color 2 (end of handling of "see-also")
[19]..[21] -- =0xFF; 't', 'u', 'v' not present
[22] = 11  -- 'w' maps to color 11 (end of handling of "warning")
[23]..[35] -- =0xFF; 'x' through '9' not present

The second 36-byte array (#17 above) is pretty sparse. To disambiguate an
initial 'c', we place 0xFF as the value everywhere except two places:

[0] = 10   -- 'c' + 'a' maps to color 10 (end of handling "callout")
[17] = 15  -- 'c' + 'r' maps to color 15 (end of handling "cross-ref")

The third 36-byte array (#18) is similar:

[4] = 14   -- 'h' + 'e' maps to color 14 (end of handling "header")
[8] = 0    -- 'h' + 'i' maps to color 0 (end of handling "hidden")

The fourth 36-byte array (#19) is even sparser:

[14] = 20  -- 'n' + 'o' requires another array to disambiguate.

The fifth 36-byte array (#20) finally clarifies the 'n' + 'o' keys:

[17] = 8   -- 'n' + 'o' + 'r' maps to color 8 (end of handling "normal")
[19] = 3   -- 'n' + 'o' + 't' maps to color 3 (end of handling "note")
*/

typedef array<uint8_t, 36> mapper_t;
typedef vector<bool> spec_entry_bitmask;

static constexpr uint8_t NO_MATCH = 0xFF;

static inline uint8_t get_mapper_idx_for_char(char c) {
    if (c <= 'z') {
        return static_cast<uint8_t>(c - 'a');
    }
    return static_cast<uint8_t>(c - '0');
}

static inline void alloc_mapper(vector<mapper_t> & mappers) {
    mappers.push_back(std::move(mapper_t()));
    memset(&mappers[mappers.size() - 1], NO_MATCH, tuple_size<mapper_t>::value);
}

void fill_maps(vector<mapper_t> & mappers, spec_entries_t const & entries,
        size_t entry_count, unsigned & unfinished_entry_count,
        spec_entry_bitmask & entries_to_consider, unsigned name_idx) {

    mapper_t & this_map = mappers[mappers.size() - 1];

    // For each entry that shared its previous char with us...
    for (unsigned j = 0; j < entry_count; ++j) {
        if (entries_to_consider[j]) {

            // Get the char at the position we're considering.
            auto & this_entry = entries[j];
            auto c = this_entry[name_idx];

            // And get the index within this_map where we'll be recording map
            // info for c.
            auto idx = get_mapper_idx_for_char(c);

            // Calculate which other entries have this same letter at this
            // position.
            spec_entry_bitmask same(entry_count, false);
            same[j] = true;
            unsigned count_of_c_in_this_position = 1;

            for (unsigned k = 0; k < entry_count; ++k) {
                if (k != j && entries_to_consider[k] && entries[k][name_idx] == c) {
                    ++count_of_c_in_this_position;
                    same[k] = true;
                    entries_to_consider[k] = false;
                }
            }

            // If we're unique...
            if (count_of_c_in_this_position == 1) {

                // Copy direct color mapping into this_map.
                this_map[idx] = this_entry[0];

                if (--unfinished_entry_count == 0) {
                    return;
                }
            } else {
                alloc_mapper(mappers);
                this_map[idx] = 16 + (mappers.size() - 1);
                // Recurse. This causes us to look at the next char beyond the
                // one that the current entry had in common with some others.
                fill_maps(mappers, entries, entry_count, unfinished_entry_count,
                          same, name_idx + 1);
                if (unfinished_entry_count == 0)
                    return;
            }
            entries_to_consider[j] = false;
        }
    }
}

void generate_maps(vector<mapper_t> & mappers, spec_entries_t const & entries,
        size_t entry_count) {
    alloc_mapper(mappers);
    unsigned unfinished_entry_count = entry_count;

    spec_entry_bitmask bits(entry_count, true);
    fill_maps(mappers, entries, entry_count, unfinished_entry_count, bits, 1);
}

struct ansi_color_palette::data_t {
    vector<mapper_t> mappers;
    data_t() {
        mappers.reserve(8);
    }
};

ansi_color_palette::ansi_color_palette(str_view const & _spec) : data(new data_t) {
    precondition(_spec);
    str_view spec(_spec);

    spec_entries_t entries;
    size_t entry_count = 0;

    str_view name;
    while (find_next_name(spec, name) && entry_count <= tuple_size<spec_entries_t>::value) {
        uint8_t value;
        precondition(find_next_value(spec, value));
        auto & buf = entries[entry_count++];
        buf[0] = value;
        copy_name(name, &buf[1]);
    }
    generate_maps(data->mappers, entries, entry_count);
}

ansi_color_palette::~ansi_color_palette() {
    delete data;
}

unsigned ansi_color_palette::map_name(char const * name) const {
    precondition(!is_null_or_empty(name));
    auto end = strchr(name, 0);
    mapper_t const * this_map = &data->mappers[0];
    for (auto p = name; *p && p < end;) {
        auto c = next_name_char(p, end);
        if (c) {
            auto m = *this_map;
            auto n = m[get_mapper_idx_for_char(c)];
            if (n < static_cast<decltype(n)>(16)) {
                return n;
            }
            this_map = &data->mappers[n - 16];
        } else {
            break;
        }
    }
    return 0;
}

}}} // end namespace


