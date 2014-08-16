#include <cstring>
#include <sstream>
#include "dbc.h"

using std::stringstream;

namespace {

using intent::core::contract_type;

constexpr char const * get_contract_type_name(contract_type ctype) {
    return (ctype == contract_type::pre ? "precondition" :
            ctype == contract_type::post ? "postcondition" : "check");
}

std::string make_exception_msg(contract_type ctype, char const * expr,
    char const * file, int line, char const * function) {

    stringstream s;
    auto p = strrchr(file, '/');
    auto q = strrchr(file, '\\');
    if (q > p) {
        p = q;
    }
    if (p) {
        file = p + 1;
    }
    s << "Failed " << get_contract_type_name(ctype) << " at " << file
        << ", line " << line << ", function " << function << "(). Expected \""
        << expr << "\" to be true.";
    return s.str();
}

} // end anonymous namespace

namespace intent {
namespace core {

contract_violation::contract_violation(contract_type ctype, char const * expr,
    char const * file, int line, char const * function) :
    std::logic_error(make_exception_msg(ctype, expr, file, line, function)),
    ctype(ctype) {
}

}} // end namespace
