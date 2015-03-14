#ifndef _675118a6c9f111e4a8f03c15c2ddfeb4
#define _675118a6c9f111e4a8f03c15c2ddfeb4

#include "core/cli/ansi_color.h"

namespace intent {
namespace core {
namespace cli {

inline char const * get_ansi_color_name(ansi_color which) {
    return get_ansi_color_name(static_cast<unsigned>(which));
}

inline char const * get_ansi_color_esc_seq(ansi_color which) {
    return get_ansi_color_esc_seq(static_cast<unsigned>(which));
}

}}} // end namespace

#endif // sentry

