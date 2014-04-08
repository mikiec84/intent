#include <cstdio>
#include "core/ioutil.h"

bool read_file(char const * fpath, std::vector<uint8_t> & bytes, size_t max_size) {
    bool ok = false;
    FILE * f = fopen(fpath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        if (size <= max_size) {
            ok = true;
            if (size == 0) {
                bytes.clear();
            } else {
                bytes.resize(size);
                fread(&bytes[0], 1, size, f);
            }
        }
        fclose(f);
    }
    return ok;
}
