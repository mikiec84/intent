#ifndef intent_core_cmdline_inline_h
#define intent_core_cmdline_inline_h

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
