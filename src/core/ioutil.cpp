#include "core/ioutil.h"

using boost::filesystem::path;
using boost::filesystem::temp_directory_path;
using boost::filesystem::unique_path;

c_file::c_file(FILE * _f) : f(_f)
{
}

c_file::c_file(path const & fpath, char const * mode) :
    f(fopen(fpath.c_str(), mode)) {
}

c_file::~c_file() {
    if (f) {
        fclose(f);
    }
}

c_file::operator FILE *() {
    return f;
}

c_file::operator bool() const {
    return f != nullptr;
}

int fclose(c_file & cf) {
    int n = fclose(cf.f);
    cf.f = nullptr;
    return n;
}

file_delete_on_exit::file_delete_on_exit(path const & p) : fpath(p) {
}

file_delete_on_exit::~file_delete_on_exit() {
    boost::filesystem::remove(fpath);
}

std::vector<uint8_t> read_binary_file(path const & fpath, size_t max_size) {
    std::vector<uint8_t> bytes;
    c_file f(fpath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        if (size < max_size) {
            fseek(f, 0, SEEK_SET);
            if (size > 0) {
                bytes.resize(size);
                fread(&bytes[0], 1, size, f);
            }
        }
    }
    // C++11 move semantics make this super cheap.
    return bytes;
}

std::string read_text_file(path const & fpath, size_t max_size) {
    std::string text;
    c_file f(fpath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        if (size < max_size) {
            fseek(f, 0, SEEK_SET);
            if (size > 0) {
                text.resize(size);
                // This is legal. See http://www.cplusplus.com/reference/string/string/at/.
                fread(&text.at(0), 1, size, f);
            }
        }
    }
    // C++11 move semantics make this super cheap.
    return text;
}

path easy_temp_file_path() {
    return unique_path(temp_directory_path() / "%%%%-%%%%-%%%%-%%%%");
}

easy_temp_c_file::easy_temp_c_file() :
    c_file(nullptr),
    path(easy_temp_file_path()) {
    f = fopen(path.c_str(), "w");
}

