#ifndef testutil_h_8c988b7147964fc189e02abc1b3219a5
#define testutil_h_8c988b7147964fc189e02abc1b3219a5

#include <boost/filesystem.hpp>

/**
 * Find the folder where a particular test cpp file lives.
 */
boost::filesystem::path find_test_folder(char const * cpp_file, char const * start_folder = ".");


#endif // sentry
