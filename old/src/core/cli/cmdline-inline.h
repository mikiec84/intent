#ifndef _03d8df0241d34353b2feaa6664b5baf9
#define _03d8df0241d34353b2feaa6664b5baf9

#include "core/cli/cmdline.h"

namespace intent {
namespace core {
namespace cli {

inline bool cmdline_param::is_positional() const {
    return names.size() == 1 && names[0][0] != '-';
}

inline std::string const & cmdline_param::get_preferred_name() const {
    return names[0];
}


}}} // end namespace

#endif // sentry
