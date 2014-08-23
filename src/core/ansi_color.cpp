#include "ansi_color.h"

namespace intent {
namespace core {

static char const * ansi_color_names[] = {
    #define TUPLE(name, hi, lo) #name,
    #include "ansi_color_tuples.h"
};

static char const * ansi_color_esc_seqs[] {
    #define TUPLE(name, hi, lo) ANSI_ESCAPE(hi, lo),
    #include "ansi_color_tuples.h"
};

char const * get_ansi_color_name(ansi_color which) {
    auto i = static_cast<unsigned>(which);
    if (which <= ansi_color::white) {
        return ansi_color_names[i];
    }
    return "";
}

char const * get_ansi_color_esc_seq(ansi_color which) {
    auto i = static_cast<unsigned>(which);
    if (which <= ansi_color::white) {
        return ansi_color_esc_seqs[i];
    }
    return "";
}


}} // end namespace


