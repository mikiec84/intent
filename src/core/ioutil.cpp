#include <cstdio>
#include "core/ioutil.h"

std::vector<uint8_t> read_file(char const * fpath, size_t max_size) {
    std::vector<uint8_t> bytes;
    FILE * f = fopen(fpath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        if (size < max_size) {
            fseek(f, 0, SEEK_SET);
            if (size == 0) {
                bytes.clear();
            } else {
                bytes.resize(size + 1);
                fread(&bytes[0], 1, size, f);
                bytes[size] = 0;
            }
        }
        fclose(f);
    }
    return bytes;
}
