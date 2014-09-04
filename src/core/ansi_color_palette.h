#ifndef intent_core_ansi_color_palette_h
#define intent_core_ansi_color_palette_h

#include "sslice.h"
#include "thread_semantics.h"

namespace intent {
namespace core {
namespace tty {

/**
 * Palettes are a way to manage a combination of colors by logical rather than
 * literal meaning. For example, if you want to colorize help emitted by a
 * console application, you might have certain categories of text that deserve
 * special colors--keywords might have one color, examples might have another,
 * warnings still another, "see also" blocks still another, and so forth.
 * Instead of hard-coding the colors for each of these categories, you can give
 * names to them, and then map them to colors with a palette. That way, if you
 * ever change your mind about what color should be used for "see also" blocks,
 * you don't have to change your string literals at all; you simply change the
 * palette that's used.
 *
 * Palettes are threadsafe, lightweight and efficient to use. They do some
 * computation during construction; although it is not very expensive, it is not
 * nearly as cheap as ongoing usage, so the expectation is that they will be
 * kept around and reused instead of being repeatedly created.
 */
IMMUTABLE
class ansi_color_palette {
    struct data_t;
    data_t * data;
public:
    /**
     * Create a palette from a string.
     *
     * @param spec: a string that contains a series of name=value pairs,
     *     delimited by \n. Color names are not case-sensitive and consist of a
     *     single token of [a-z][a-z0-9_-.]* (non-alphanums are ignored in
     *     comparisons, so "stat-msg", "Stat.Msg", "STAT_MSG", and "statMsg"
     *     are equivalent). In actual usage, names only need to be specified
     *     with as many chars as it takes to make them unique, and should be
     *     kept short for performance and terseness. Values are numbers in the
     *     range [0..15]; not all of them need be defined. Order of items in the
     *     spec is irrelevant, and a single value may have more than one name.
     *     For example:
     *
     *         title=15
     *         see-also=3
     *         warning=9
     *         error=10
     *         xref=7
     */
    ansi_color_palette(intent::core::text::sslice const & spec);
    ~ansi_color_palette();

    unsigned map_name(char const * name) const;
};

}}} // end namespace

#endif // sentry

