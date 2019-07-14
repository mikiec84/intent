#ifndef _4a8e15ca2e8048968e354db8b51f05ff
#define _4a8e15ca2e8048968e354db8b51f05ff

#include <cstdint>

namespace intent {
namespace core {
namespace cli {

constexpr int STDIN_FILE_DESCRIPTOR = 0;
constexpr int STDOUT_FILE_DESCRIPTOR = 1;
constexpr int STDERR_FILE_DESCRIPTOR = 2;

uint16_t get_tty_column_count();
uint16_t get_tty_row_count();
bool is_a_tty(int file_descriptor=1);

}}} // end namespace

#endif // sentry

