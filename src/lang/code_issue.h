#ifndef CODE_ISSUE_69cd2c3b9bf94f40b25d4aed9ae07240
#define CODE_ISSUE_69cd2c3b9bf94f40b25d4aed9ae07240

#include <stdexcept>

#include "lang/code_site.h"

namespace intent {
namespace lang {

/**
 * A code_issue is any type of bug, parse error, warning, suggestion, or
 * other meta information about code at a specific place. Although derived
 * from std::runtime_error, these issues are not necessarily thrown as
 * exceptions.
 */
class code_issue: public std::runtime_error
{
public:
    const unsigned code;
    const code_site site;

    code_issue(unsigned code, code_site const & site);
    code_issue(unsigned code, char const * url, unsigned line, unsigned offset,
                 code_site::url_copy_strategy url_strategy = code_site::dont_copy);
};

} // end namespace lang
} // end namespace intent

#endif // SYNTAX_ERROR_H
