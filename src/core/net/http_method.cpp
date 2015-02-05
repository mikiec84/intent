#include <cstring>

#include "core/net/http_method.h"
#include "core/text/strutil.h"
#include "core/util/countof.h"


namespace intent {
namespace core {
namespace net {


constexpr http_method known_methods[] = {
	#define TUPLE(id, verb, first_rfc, last_rfc) {id, #id, #verb, first_rfc, last_rfc},
	#include "core/net/http_method_tuples.h"
};

constexpr unsigned known_method_count = countof(known_methods);


unsigned get_http_method_count() {
	return known_method_count;
}


http_method const * get_http_method_by_index(unsigned i) {
	return (i < known_method_count) ? known_methods + i : nullptr;
}


http_method const * get_http_method_by_id(http_method_id id) {
	return get_http_method_by_index(static_cast<unsigned>(id));
}


http_method const * get_http_method_by_name(char const * name) {
	if (name && *name) {
		char normalized[20];
		auto last = sizeof(normalized) - 1;
		strncpy(normalized, name, last);
		normalized[last] = 0;
		text::ascii_to_lower_case(normalized);
		name = normalized;
		if (strncmp(name, "http_", 5) == 0) {
			name += 5;
		} else if (strncmp(name, "webdav_", 7) == 0) {
			name += 7;
		}

		#define test(verb, prefix) \
			if (strcmp(name+1, #verb+1) == 0) \
				return get_http_method_by_id(prefix##_##verb)
		#define try_to_match(ch, verb, prefix) \
			if (*name == ch) \
				return strcmp(name+1, #verb+1) == 0 ? get_http_method_by_id(prefix##_##verb) : nullptr

		try_to_match('g', get, http);
		if (*name == 'p') {
			test(post, http);
			test(put, http);
			test(patch, http);
			test(propfind, webdav);
			test(proppatch, webdav);
			return nullptr;
		}
		try_to_match('o', options, http);
		try_to_match('h', head, http);
		try_to_match('d', delete, http);
		try_to_match('t', trace, http);
		if (*name == 'c') {
			test(connect, http);
			test(copy, webdav);
			return nullptr;
		}
		if (*name == 'l') {
			test(link, http);
			test(lock, webdav);
			return nullptr;
		}
		if (*name == 'u') {
			test(unlink, http);
			test(unlock, webdav);
			return nullptr;
		}
		if (*name == 'm') {
			test(move, webdav);
			test(mkcol, webdav);
			return nullptr;
		}
	}
	return nullptr;
}


}}} // end namespace


