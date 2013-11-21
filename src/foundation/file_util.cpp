#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "foundation/dbc.h"
#include "foundation/error.h"
#include "foundation/file_util.h"

using std::string;

namespace intent { namespace foundation {

delete_file_on_exit::delete_file_on_exit(char const * fname) : fname(fname) {
}

delete_file_on_exit::delete_file_on_exit(std::string const & fname) :
		fname(fname) {
}

delete_file_on_exit::~delete_file_on_exit() {
	if (!fname.empty()) {
		unlink(fname.c_str());
	}
}

void delete_file_on_exit::release() {
	fname = "";
}

close_file_on_exit::close_file_on_exit(FILE * f) : f(f) {
}

close_file_on_exit::~close_file_on_exit() {
	if (f) {
		fclose(f);
	}
}

void close_file_on_exit::release() {
	f = nullptr;
}

string make_temp_file(char const * prefix, char const * suffix) {
	char buf[512];
	if (!prefix || *prefix == 0) {
		//TODO: on windows, don't use /tmp
		prefix = "/tmp/XXXXXX";
	}
	if (!suffix) {
		suffix = "";
	}
	auto suffix_len = strlen(suffix);
	PRECONDITION(strlen(prefix) + suffix_len < sizeof(buf));
	strcpy(buf, prefix);
	strcat(buf, suffix);
	int handle = mkstemps(buf, suffix_len);
	if (handle == -1) {
		throw ERROR_EVENT(errno);
	} else {
		close(handle);
	}
	return buf;
}

bool path_exists(char const * path) {
	struct stat info;
	return stat(path, &info) == 0;
}

bool is_file(char const * path) {
	struct stat info;
	return (stat(path, &info) == 0) && !S_ISDIR(info.st_mode);
}

bool is_dir(char const * path) {
	struct stat info;
	return (stat(path, &info) == 0) && S_ISDIR(info.st_mode);
}

}} // end namespace
