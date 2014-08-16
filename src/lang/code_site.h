#ifndef _code_site_h_e2e12222496d404491498772700655f4
#define _code_site_h_e2e12222496d404491498772700655f4

#include <cstddef>

namespace intent {
namespace lang {

struct no_copy_strategy;
struct make_private_copy_strategy;

/**
 * Describe the site of something interesting in code -- an error or warning,
 * for example.
 *
 * Instances of this class are immutable once they're created.
 */
template <class URL_COPY_STRATEGY = no_copy_strategy>
struct code_site
{
    char const * const compilation_unit_url;
    const unsigned line_number;
    const unsigned offset_on_line;

    /**
     * Construct a code_site in an anonymous compilation unit.
     */
    code_site(unsigned line, unsigned offset);

    /**
     * Construct a code_site.
     */
    code_site(char const * url, unsigned line, unsigned offset);
    ~code_site();

    code_site(code_site const &);
    // Can't have full value semantics, because these objects are immutable
    // once they've been created.
    code_site & operator =(code_site const &) = delete;
};

typedef code_site<no_copy_strategy> dependent_code_site;
typedef code_site<make_private_copy_strategy> independent_code_site;

}} // end namespace

#include "lang/code_site-inline.h"

#endif // sentry
