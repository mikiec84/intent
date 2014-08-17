#ifndef intent_core_tty_h
#define intent_core_tty_h

#include <cstdint>

namespace intent {
namespace core {

constexpr int STDIN_FILE_DESCRIPTOR = 0;
constexpr int STDOUT_FILE_DESCRIPTOR = 1;
constexpr int STDERR_FILE_DESCRIPTOR = 2;

uint16_t get_tty_column_count();
uint16_t get_tty_row_count();
bool is_a_tty(int file_descriptor=1);

}} // end namespace

#endif // sentry

