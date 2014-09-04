#include "ansi_color.h"

namespace intent {
namespace core {
namespace tty {

static char const * ansi_color_names[] = {
    #define TUPLE(name, hi, lo) #name,
    #include "ansi_color_tuples.h"
};

static char const * ansi_color_esc_seqs[] {
    #define TUPLE(name, hi, lo) ANSI_ESCAPE(hi, lo),
    #include "ansi_color_tuples.h"
};

char const * get_ansi_color_name(unsigned which) {
    if (which <= 15) {
        return ansi_color_names[which];
    }
    return "";
}

char const * get_ansi_color_esc_seq(unsigned which) {
    if (which <= 15) {
        return ansi_color_esc_seqs[which];
    }
    return "";
}

void print_in_color(char const * txt, int file_descriptor) {
}

std::string colorize(char const * txt);

}}} // end namespace

