#include "core/net/curl/headers.h"
#include "core/text/strutil.h"


using intent::core::text::compare_str_ascii_case_insensitive;


namespace intent {
namespace core {
namespace net {
namespace curl {


bool header_is_less(std::string const & a, std::string const & b) {
	return compare_str_ascii_case_insensitive(a.c_str(), b.c_str()) < 0;
}


}}}} // end namespace
