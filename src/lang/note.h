#ifndef NOTE_69cd2c3b9bf94f40b25d4aed9ae07240
#define NOTE_69cd2c3b9bf94f40b25d4aed9ae07240

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

} // end namespace lang
} // end namespace intent

#endif // SYNTAX_ERROR_H
