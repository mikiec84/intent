#ifndef _sandbox_h_2364333f74694144a97e12a701cbb1b8
#define _sandbox_h_2364333f74694144a97e12a701cbb1b8

#include <boost/filesystem.hpp>

namespace intent {
namespace core {

class sandbox
{
public:
    static boost::filesystem::path find_root(char const * folder_within_sandbox) noexcept(false);
};

}} // end namespace

#endif // sentry
