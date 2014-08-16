#include "core/sandbox.h"

using namespace boost::filesystem;

namespace intent {
namespace core {

const char * const sandbox_prop_file = "sandbox.i";

bool is_sandbox_root(path const & p) {
    return is_regular_file(p / sandbox_prop_file);
}

path sandbox::find_root(char const * folder_within_sandbox) noexcept(false) {
    path p(folder_within_sandbox);
    p = absolute(p);
    while (true) {
        if (is_sandbox_root(p)) {
            return p;
        }
        if (p.has_parent_path()) {
            p = p.parent_path();
        } else {
            return path();
        }
    }
}

}} // end namespace
