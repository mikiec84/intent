#ifndef intent_core_ansi_color_h
#define intent_core_ansi_color_h

#include <cstdint>
#include <string>

#include "enum_operators.h"
#include "tty.h"

namespace intent {
namespace core {

#define ANSI_ESCAPE_PREFIX "\x1B["
#define ANSI_ESCAPE(x,y) ANSI_ESCAPE_PREFIX #x ";" #y "m"

static constexpr char * RESET_COLOR = ANSI_ESCAPE_PREFIX "0m";

enum class ansi_color: uint8_t {
    #define TUPLE(name, hi, lo) name,
    #include "ansi_color_tuples.h"
};
define_numeric_operators_for_enum(ansi_color);

char const * get_ansi_color_name(ansi_color which);
char const * get_ansi_color_esc_seq(ansi_color which);

char const * get_ansi_color_name(unsigned which);
char const * get_ansi_color_esc_seq(unsigned which);

ansi_color get_color_for_name(char const * name);

#define XCOLOR(value) "\x01:" #value ":"

class ansi_palette {
    struct data_t;
public:
    ansi_palette();
};

void print_in_color(char const * txt, int file_descriptor=STDOUT_FILE_DESCRIPTOR);
std::string colorize(char const * txt, ansi_palette const & default_palette);

}} // end namespace

#include "ansi_color-inline.h"

#endif // sentry

