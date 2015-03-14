#ifndef _6754ba5ec9f111e4af7c3c15c2ddfeb4
#define _6754ba5ec9f111e4af7c3c15c2ddfeb4

#include <boost/filesystem.hpp>

/**
 * Find the folder where a particular test cpp file lives.
 */
boost::filesystem::path find_test_folder(char const * cpp_file, char const * start_folder = ".");


#endif // sentry
