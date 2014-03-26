#include <sstream>
#include <string>
#include "lang/code_issue.h"

using std::string;
using std::stringstream;

namespace {

string make_issue_msg(unsigned code, char const * url, unsigned line, unsigned offset) {
    stringstream txt;
    txt << url << ", line " << line << ", offset " << offset << ": error " << code;
    return txt.str();
}

} // end anonymous namespace

namespace intent {
namespace lang {

code_issue::code_issue(unsigned code, code_site const & site):
    std::runtime_error(
        make_issue_msg(code, site.compilation_unit_url,
                        site.line_number, site.offset_on_line)),
    code(code),
    site(site.compilation_unit_url, site.line_number, site.offset_on_line,
         site.url_strategy) {
}

code_issue::code_issue(unsigned code, char const * url, unsigned line, unsigned offset,
                 code_site::url_copy_strategy url_strategy):
    std::runtime_error(make_issue_msg(code, url, line, offset)),
    code(code),
    site(url, line, offset, url_strategy) {
}

} // end namespace lang
} // end namespace intent
