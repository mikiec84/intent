#ifndef _6752610ac9f111e4965e3c15c2ddfeb4
#define _6752610ac9f111e4965e3c15c2ddfeb4

#include <boost/filesystem.hpp>

namespace intent {
namespace core {
namespace dev {

class sandbox
{
public:
    static boost::filesystem::path find_root(char const * folder_within_sandbox) noexcept(false);
};

}}} // end namespace

#endif // sentry
