#ifndef intent_test_testutil_h
#define intent_test_testutil_h

#include <boost/filesystem.hpp>

/**
 * Find the folder where a particular test cpp file lives.
 */
boost::filesystem::path find_test_folder(char const * cpp_file, char const * start_folder = ".");


#endif // sentry
