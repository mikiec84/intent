#ifndef intent_core_sandbox_h
#define intent_core_sandbox_h

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
