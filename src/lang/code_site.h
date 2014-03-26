#ifndef CODE_SITE_e2e12222496d404491498772700655f4
#define CODE_SITE_e2e12222496d404491498772700655f4

#include <cstddef>

namespace intent {
namespace lang {

/**
 * Describe the site of something interesting in code -- an error or warning,
 * for example.
 *
 * Instances of this class are immutable once they're created.
 */
struct code_site
{
    /**
     * By default, code_site does not make a private copy of the
     * url it's given. This allows a single compilation unit to generate
     * lots of errors or warnings, without allocating memory or mutexing the
     * global heap to copy a long path to a file over and over again. However,
     * it also means that a code_site object must always have a lifetime that
     * fits within the lifetime of whatever larger context owns its url. If
     * it's desirable to create a code_site with an independent lifetime,
     * use the make_private_copy strategy instead.
     */
    enum url_copy_strategy { dont_copy, make_private_copy };

    char const * const compilation_unit_url;
    const unsigned line_number;
    const unsigned offset_on_line;
    const url_copy_strategy url_strategy;

    /**
     * Construct a code_site in an anonymous compilation unit.
     */
    code_site(unsigned line, unsigned offset);

    /**
     * Construct a code_site.
     */
    code_site(char const * url, unsigned line, unsigned offset,
              url_copy_strategy url_strategy = dont_copy);
    ~code_site();

    // Can't value semantics, because these objects are immutable
    // once they've been created.
    code_site(code_site const &) = delete;
    code_site & operator =(code_site const &) = delete;
};

} // end namespace lang
} // end namespace intent

#endif // sentry
