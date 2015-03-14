#ifndef _1a370e76942a4aa6ab7fb1d0596788a5
#define _1a370e76942a4aa6ab7fb1d0596788a5

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
