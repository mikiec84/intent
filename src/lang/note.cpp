#include <sstream>

#include "lang/note.h"

using std::string;
using std::stringstream;

namespace intent {
namespace lang {

note::note(issue_id id, dependent_code_site const & site):
    issue(id),
    site(site.compilation_unit_url, site.line_number, site.offset_on_line) {
}

note::note(issue_id id, char const * url, unsigned line, unsigned offset):
    issue(id),
    site(url, line, offset) {
}

string note::to_string() const {
    stringstream txt;
    txt << site.compilation_unit_url << ", line " << site.line_number << ", offset "
        << site.offset_on_line << ": error " << issue << " -- " << get_issue_msg(issue);
    return txt.str();
}

} // end namespace lang
} // end namespace intent
