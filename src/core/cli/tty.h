#ifndef _67525857c9f111e49d713c15c2ddfeb4
#define _67525857c9f111e49d713c15c2ddfeb4

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

