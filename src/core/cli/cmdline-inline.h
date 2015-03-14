#ifndef _675249a6c9f111e49b5f3c15c2ddfeb4
#define _675249a6c9f111e49b5f3c15c2ddfeb4

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
