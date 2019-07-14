#include "helpers/testutil.h"

using namespace intent::core::filesystem;

path find_test_folder(char const * cpp_file, char const * start_folder) {
    path p(start_folder);
    p = absolute(p);
    while (true) {
        path combined = p / cpp_file;
        if (is_regular_file(combined)) {
            return canonical(combined.parent_path());
        }
        if (p.has_parent_path()) {
            p = p.parent_path();
        } else {
            return path();
        }
    }
}
