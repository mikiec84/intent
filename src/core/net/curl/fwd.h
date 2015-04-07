#ifndef _39dbc387489b4e40a7a7a0117497fda0
#define _39dbc387489b4e40a7a7a0117497fda0

#include <memory>

namespace intent {
namespace core {
namespace net {
namespace curl {

class channel;
class session;
class request;
class response;

typedef std::shared_ptr<channel> channel_handle;
typedef std::shared_ptr<const channel> const_channel_handle;

typedef std::shared_ptr<session> session_handle;
typedef std::shared_ptr<const session> const_session_handle;

typedef std::shared_ptr<request> request_handle;
typedef std::shared_ptr<const request> const_request_handle;

typedef std::shared_ptr<response> response_handle;
typedef std::shared_ptr<const response> const_response_handle;

}}}} // end namespace


#endif
