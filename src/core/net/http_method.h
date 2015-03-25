#ifndef _befe70a0d95645d19f091080a35636aa
#define _befe70a0d95645d19f091080a35636aa

#include <cstdint>

namespace intent {
namespace core {
namespace net {


/**
 * Enumerate methods usable in the http protocol.
 */
enum http_method_id {
	#define TUPLE(id, str, first_rfc, last_rfc) id,
	#include "core/net/http_method.tuples"
};


/**
 * Encapsulate what we know about particular http methods.
 */
struct http_method {
	http_method_id id;
	char const * id_str;
	char const * verb;
	uint16_t first_rfc;
	uint16_t last_rfc;
};


unsigned get_http_method_count();
http_method const * get_http_method_by_index(unsigned i);
http_method const * get_http_method_by_id(http_method_id);
http_method const * get_http_method_by_name(char const * name);


}}} // end namespace


#endif // sentry
