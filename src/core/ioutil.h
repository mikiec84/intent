#ifndef ioutil_h_cf9b63decce44726bcea23c4d5038f65
#define ioutil_h_cf9b63decce44726bcea23c4d5038f65

#include <cstdint>
#include <vector>

std::vector<uint8_t> read_file(char const * fpath, size_t max_size=1024*1024);

#endif // IOUTIL_H
