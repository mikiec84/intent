#ifndef _960509aa774247688eab63910cc21aa5
#define _960509aa774247688eab63910cc21aa5

#include <boost/filesystem.hpp>

/**
 * Find the folder where a particular test cpp file lives.
 */
boost::filesystem::path find_test_folder(char const * cpp_file, char const * start_folder = ".");


#endif // sentry
