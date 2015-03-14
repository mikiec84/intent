#ifndef _6752d4fac9f111e496cb3c15c2ddfeb4
#define _6752d4fac9f111e496cb3c15c2ddfeb4

#include <map>
#include <string>


namespace intent {
namespace core {
namespace net {
namespace curl {


typedef bool (*header_is_less_func)(std::string const & a, std::string const & b);
bool header_is_less(std::string const & a, std::string const & b);

typedef std::map<std::string, std::string, header_is_less_func> headers;


}}}} // end namespace


#endif // sentry
