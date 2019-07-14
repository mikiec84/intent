#include <algorithm>

#include "core/util/dbc.h"
#include "core/text/str_view.h"
#include "core/text/strutil.h"
#include "core/text/wrap_lines.h"

using std::string;

using intent::core::text::str_view;
using intent::core::text::EIGHTY_SPACES;

namespace {

inline void find_best_hyphen_wrap(char const *& ptr, char const * end,
        unsigned & len, unsigned width, char const *& wrap_before) {

    // Hyphens are generally valid points to wrap, but unlike
    // spaces, they don't get replaced, and the preferred
    // place to wrap is *after* the hyphen, not before. We
    // also have wrinkles with double hyphens (if present,
    // don't wrap in middle) and with long runs of hyphens
    // (wrap anywhere).
    bool double_hyphen = (ptr < end - 1) && ptr[1] == '-';
    bool many_hyphens = ptr[-1] == '-' || (
            double_hyphen && (ptr < end - 2 && ptr[2] == '-'));

    if (many_hyphens) {
        // Find the last hyphen in the sequence that will fit on the current
        // line.
        while (ptr < end && len < width && *ptr == '-') {
            ++ptr;
            ++len;
        }
        wrap_before = ptr;
    } else if (double_hyphen) {
        // Can we fit both hyphens on this line?
        if (len < width - 1) {
            wrap_before = ptr = ptr + 2;
        }
        // We only assing wrap_before if we can fit both hyphens--but regardless,
        // we advance by two places because we need to treat these hyphens as a
        // single unit.
        len += 2;
    } else {
        wrap_before = ++ptr;
        ++len;
    }
}

inline void insert_soft_wrap(string & wrapped, char const * wrap_before,
        char const *&p, char const *&ptr, unsigned & len_after_this_char,
        char const * line_delim, size_t line_delim_len, char const * indent,
        str_view const & input) {

    // If we didn't find anywhere at all that it was possible to
    // wrap, just break at the final valid position. This is ugly,
    // but it is a safety measure that guarantees we never exceed
    // the specified line length.
    if (!wrap_before) {
        wrap_before = ptr;
    }

    wrapped.append(p, wrap_before - p);
    wrapped.append(line_delim, line_delim_len);
    if (indent) {
        auto indent_width = indent - EIGHTY_SPACES;
        wrapped.append(EIGHTY_SPACES, indent_width);
        len_after_this_char = indent_width + 1;
    } else {
        len_after_this_char = 1;
    }

    // Skip over all spaces at the break point; they are not
    // significant. We want to start the new line with the next
    // visible char.
    ptr = wrap_before;
    auto end = input.end();
    while (*ptr == ' ' && ptr < end) {
        ++ptr;
    }
}

} // end anonymous namespace

namespace intent {
namespace core {
namespace text {

static constexpr ptrdiff_t MAX_INDENT = 64;

string wrap_lines(str_view const & input, unsigned width,
           char const * line_delim, wrap_lines_advance_func nxt) {

    precondition(nxt != nullptr);
    precondition(width >= 10);

    char const * indent = nullptr;

    std::string wrapped;
    wrapped.reserve(input.length);
    auto line_delim_len = strlen(line_delim);

    // As we walk through the text, we need to know what the length of the
    // current line is. This is a logical/visible length, not a number-of-bytes
    // length. If a single glyph is represented as 3 bytes in UTF8, length only
    // increments by 1. The smallest value for this variable is 1, not 0...
    unsigned len_after_this_char = 1;

    const auto end = input.end();
    for (auto p = input.begin; p < end; ) {

        auto remaining = end - p;

        // If we have less characters left than the wrap width, just append them.
        if (remaining + (len_after_this_char - 1) < width) {
            if (remaining) {
                wrapped.append(p, remaining);
            }
            break;

        // Otherwise we're going to have to look for places to wrap.
        } else {

            // Remains false until we find the first non-space char on this line.
            bool found_visible_char = false;
            bool soft_wrap = true;
            char const * ptr;
            char const * wrap_before = nullptr;

            // The correct upper bound for this loop is non-obvious, and must be
            // well understood before altering logic.
            //
            // Generally, when people say they want to wrap at 80 chars, they
            // mean that they want lines that are a maximum of 80 glyphs wide,
            // without any careful thought. However, in a TTY that's 80 columns
            // wide, you cannot write 80 glyphs and then insert an LF (or a CR+
            // LF), because as soon as the 80th column receives a glyph, the tty
            // repositions to the next visible line down. This means we actually
            // only have 79 printable chars to work with. Whether we write an
            // LF or a CR+LF after that, we end up correctly positioned at the
            // front of the next row--so regardless of line delim, we can assume
            // only one glyph need be reserved for end-of-line.
            //
            // You would think, therefore, on a display with N columns (e.g.,
            // 80), that a for loop should go from 0 to N-1, exclusive:
            //               for (int i = 0; i < N - 1; ++x)
            // ...and then you'd write the line delim. This would give you N - 1
            // printable chars plus the line delim--right?
            //
            // However, this is wrong because of another subtlety: on an 80-
            // column TTY, we must test the *80th* character to see if it's a
            // space, not just the 79th. We'll write a line break regardless,
            // but if the 80th character is a space, it gets chosen instead of
            // any preceding wrap position. In other words, a soft wrap at
            // position 80 is valid.
            //
            // Whew! So the correct upper bound is width, not width - 1. Sort of.
            //
            // Next wrinkle: we are using 1-based indices here, because from the
            // top of each iteration we are assuming that the line will include
            // the current char. This means that to iterate 80 times, we must
            // either use for(i=1; i < 81; ++i) or (for i=1; i <= 80; ++i).
            //
            // Last wrinkle: the final char we look at is special. If it's a
            // space or hard break, fine. If not, then we have to back up one
            // char because we advanced too far.

            for (ptr = p; len_after_this_char <= width && ptr < end;
                    ptr = nxt(ptr), ++len_after_this_char) {

                char c = *ptr;
                switch (c) {

                case ' ':
                    if (found_visible_char) {
                        wrap_before = ptr;
                    }
                    break;

                case '-':
                    if (len_after_this_char == width) {
                        --ptr;
                    } else if (found_visible_char) {
                        find_best_hyphen_wrap(ptr, end, len_after_this_char,
                                width, wrap_before);
                    } else {
                        // A hyphen that's the first visible char on a line isn't
                        // a valid place to break. Treat it like any other visible
                        // char.
                        found_visible_char = true;
                        if (ptr > p) {
                            indent = EIGHTY_SPACES + std::min(MAX_INDENT, ptr - p);
                        }
                    }
                    break;

                case '\r':
                case '\n':
                    // If we found anything visible on the current line, append
                    // it. If not, we'll just trim leading spaces.
                    if (found_visible_char) {
                        wrapped.append(p, ptr);
                    }
                    wrapped.append(line_delim, line_delim_len);
                    //++ptr; // we'll increment once as we break inner loop

                    // Handle \r\n as if it were a single char.
                    if (c == '\r' && ptr + 1 < end && ptr[1] == '\n') {
                        ++ptr;
                    }

                    // Reset indent.
                    indent = nullptr;

                    // Break inner loop, but in such a way that we don't insert
                    // a redundant soft wrap.
                    len_after_this_char = width;
                    soft_wrap = false;
                    break;

                default:
                    if (len_after_this_char == width) {
                        --ptr;
                    } else if (!found_visible_char) {
                        found_visible_char = true;
                        if (ptr > p) {
                            indent = EIGHTY_SPACES + std::min(MAX_INDENT, ptr - p);
                        }
                    }
                    break;
                } // end switch
            } // end for

            // It is possible that we ended up with less visible chars than it
            // takes to fill a line, even though we tested the remaining byte
            // count before calculating wrap points. This could happen if the
            // line contained escape sequences or multi-byte characters...
            if (ptr == end && len_after_this_char < width) {
                wrapped.append(p, ptr);

            // More likely, we should do the soft wrap...
            } else if (soft_wrap) {
                insert_soft_wrap(wrapped, wrap_before, p, ptr,
                        len_after_this_char, line_delim, line_delim_len, indent,
                        input);

            // Although we might have found a hard line break instead...
            } else {
                len_after_this_char = 1;
            }
            p = ptr;
        }
    }
    return wrapped;
}

}}} // end namespace


