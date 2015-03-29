#ifndef _b24750f44d3446858e95cab7f984814d
#define _b24750f44d3446858e95cab7f984814d

#include <initializer_list>
#include <string>

#include "core/filesystem.h"
#include "core/util/enum_operators.h"
#include "core/text/numeric_formats.h"

namespace pcrecpp {
    class RE;
}

namespace intent {
namespace core {
namespace cli {

struct cmdline_param;

struct numeric_range_info {
    double min;
    double max;
    intent::core::text::numeric_formats allowed_formats;
};

/**
 * A validator for numbers passed as cmdline args.
 */
std::string in_numeric_range(cmdline_param const & param, char const * value,
    void /*numeric_range_info*/ const *);

/**
 * A validator that vetts string args against a regex.
 *
 * By default, the error message generated by this validator displays the
 * regex that was used. If you have an extraordinarily complex regex, or if
 * you'd simply like a cleaner message, then declare your regex to use the
 * pcre extended bit, and insert a comment at the top; the comment will be
 * displayed instead of the full pattern. Like this:
 *
 * pcrecpp::RE my_regex(
 *     "#ipv6 address\n"
 *     "..... a lot of complicated gobbledy-gook .....",
 *     pcrecpp::RE_options().set_extended());
 */
std::string matches_regex(cmdline_param const & param, char const * value,
    void /*precpp::RE*/ const *);

struct filesys_info {
    std::initializer_list<filesystem::file_type> type_must_be_in;
    unsigned perms_mask;
    unsigned masked_perms_must_equal;
    double min_size;
    double max_size;
    pcrecpp::RE const * name_must_match;
};

std::string matches_filesys_info(cmdline_param const & param, char const * value,
    void /*filesys_info*/ const *);

}}} // end namespace

#endif // sentry
