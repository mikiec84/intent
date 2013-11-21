#ifndef _FOUNDATION_FILE_UTIL_H_
#define _FOUNDATION_FILE_UTIL_H_

#include <string>
#include <stdio.h>

namespace intent { namespace foundation {

struct delete_file_on_exit {
	std::string fname;
	delete_file_on_exit(char const * fname);
	delete_file_on_exit(std::string const & fname);
	~delete_file_on_exit();
	void release();
private:
	delete_file_on_exit(delete_file_on_exit const &);
	delete_file_on_exit & operator =(delete_file_on_exit const &);
};

struct close_file_on_exit {
	FILE * f;
	close_file_on_exit(FILE * f);
	~close_file_on_exit();
	void release();
private:
	close_file_on_exit(close_file_on_exit const &);
	close_file_on_exit & operator =(close_file_on_exit const &);
};

/**
 * Works like mkstemps(), but makes buffer mgmt unnecessary.
 *
 * @param prefix
 *     If null, <temp dir>/XXXXXX is used. The XXXXXX chars are
 *     replaced by generated name.
 *
 * @pre Combined length of prefix and suffix should not exceed 511 bytes.
 * @throws
 *     error_event(E_PRECONDITION_1EXPR_VIOLATED) if length too long.
 *     std::runtime_error if unable to create file.
 */
std::string make_temp_file(char const * prefix=nullptr,
		char const * suffix=".tmp");

bool path_exists(char const * path);
bool is_file(char const * path);
bool is_dir(char const * path);

}} // end namespace

#endif
