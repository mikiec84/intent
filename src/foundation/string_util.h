#ifndef _FOUNDATION_STRUTIL_H_
#define _FOUNDATION_STRUTIL_H_

#include <string>
#include <vector>

#include "foundation/thread_semantics.h"

typedef std::vector<std::string> stringvec_t;

/** trim from start */
THREADSAFE
std::string & ltrim(std::string & s);

/** trim from start */
THREADSAFE
char const * ltrim(char const * s, char const * end=nullptr);

/** trim from end */
THREADSAFE
std::string & rtrim(std::string & s);

/** trim from both ends */
THREADSAFE
std::string & trim(std::string & s);

/**
 * Split a string at an arbitrary delimiter, and push any items that are
 * found into a vector of strings.
 */
THREADSAFE
stringvec_t & split(std::string const & s, char delim, stringvec_t & elems);

/**
 * Split a string at an arbitrary delimiter, and push any items that are
 * found into a vector of strings.
 */
THREADSAFE
stringvec_t & split(char const * s, char delim, stringvec_t & elems);

/**
 * Split a string at an arbitrary delimiter, and push any items that are
 * found into a vector of strings. Return the newly built vector.
 */
THREADSAFE
stringvec_t split(std::string const & s, char delim);

/**
 * Split a string at an arbitrary delimiter, and push any items that are
 * found into a vector of strings. Return the newly built vector.
 */
THREADSAFE
stringvec_t split(char const * s, char delim);

/**
 * Parse the hostnames from exec_hosts and place them into exec_host_vector
 * exec_host is in the following format:
 *    hostname/index[+hostname2/index2[...]]
 */
THREADSAFE
void parse_exec_hosts(std::string const &exec_hosts,
		stringvec_t &exec_host_vector);

#endif // sentry
