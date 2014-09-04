#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "tty.h"

namespace intent {
namespace core {
namespace tty {

uint16_t get_tty_column_count() {
    if (is_a_tty()) {
        struct winsize w;
        ioctl(STDOUT_FILE_DESCRIPTOR, TIOCGWINSZ, &w);
        return w.ws_col;
    }
    return 0;
}

uint16_t get_tty_row_count() {
    if (is_a_tty()) {
        struct winsize w;
        ioctl(STDOUT_FILE_DESCRIPTOR, TIOCGWINSZ, &w);
        return w.ws_row;
    }
    return 0;
}

bool is_a_tty(int file_desc) {
    return isatty(file_desc) != 0;
}

}}} // end namespace


