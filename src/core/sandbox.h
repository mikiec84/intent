#ifndef SANDBOX_H
#define SANDBOX_H

#include <boost/filesystem.hpp>

class sandbox
{
public:
    static boost::filesystem::path find_root(char const * folder_within_sandbox) noexcept(false);
};

#endif // SANDBOX_H
