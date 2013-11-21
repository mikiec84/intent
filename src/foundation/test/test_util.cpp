#include "foundation/test/test_util.h"

using std::string;

void expect_str_contains(char const * haystack, char const * needle,
		bool expected) {
	if ((strstr(haystack, needle) != 0) != expected) {
		char const * in_phrase = expected ? "IN" : "NOT IN";
		ADD_FAILURE() << "Expected \"" << needle << "\" " << in_phrase
				<< " \"" << haystack << "\".";
	}
}

void expect_str_contains(char const * haystack, string const & needle,
		bool expected) {
	expect_str_contains(haystack, needle.c_str(), expected);
}

void expect_str_contains(string const & haystack, char const * needle,
		bool expected) {
	expect_str_contains(haystack.c_str(), needle, expected);
}

void expect_str_contains(string const & haystack, string const & needle,
		bool expected) {
	expect_str_contains(haystack.c_str(), needle, expected);
}
