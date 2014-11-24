#ifndef intent_core_ansi_color_inline_h
#define intent_core_ansi_color_inline_h

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

