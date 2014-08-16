#ifndef _cmdline_inline_h_929db26470444d75a6330f6f7a95c30b
#define _cmdline_inline_h_929db26470444d75a6330f6f7a95c30b

#include "cmdline.h"

namespace intent {
namespace core {

inline bool cmdline_param::is_positional() const {
    return names.size() == 1 && names[0][0] != '-';
}

inline std::string const & cmdline_param::get_preferred_name() const {
    return names[0];
}


}} // end namespace

#endif // sentry
