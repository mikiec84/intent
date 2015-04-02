#ifndef _453f31a57a614870a056f54d34ec5de7
#define _453f31a57a614870a056f54d34ec5de7

namespace intent {
namespace core {
namespace net {
namespace curl {

class options {
	struct impl_t;
	impl_t * impl;
public:
	options();
	~options();

	#define tuple(name, datatype, libcurl_name) \
	datatype get_##name() const; \
	void set_##name(datatype)

	#include "options.tuples"
};

}}}} // end namespace


#endif // sentry
