#ifndef _caf831e40d1b462581f856cb80ab8146
#define _caf831e40d1b462581f856cb80ab8146

#include <string>

#include "lang/code_site.h"
#include "lang/issue.h"

namespace intent {
namespace lang {

/**
 * A note is any type of bug, parse error, warning, suggestion, or
 * other meta information about code at a specific place. Notes can
 * be thrown or returned.
 */
class note
{
public:
    const issue_id issue;
    const dependent_code_site site;

    note(issue_id issue, dependent_code_site const & site);
    note(issue_id issue, char const * url, unsigned line, unsigned offset);

    std::string to_string() const;
};

}} // end namespace

#endif // SYNTAX_ERROR_H
