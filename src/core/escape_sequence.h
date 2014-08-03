#ifndef ESCAPE_SEQUENCE_H_a2c4bf80275c4eebb036e7a63c2ed4ca
#define ESCAPE_SEQUENCE_H_a2c4bf80275c4eebb036e7a63c2ed4ca

#include "core/unicode.h"

struct sslice;

std::string expand_escape_sequences(sslice const &);

typedef bool (* should_escape_func)(codepoint_t);
bool should_escape_in_utf8_string_literals(codepoint_t);

std::string insert_escape_sequences(sslice const &, should_escape_func=should_escape_in_utf8_string_literals);

/**
 * Append an escape sequence for a particular unicode codepoint.
 * Does not null-terminate.
 */
bool add_escape_sequence(char *& buf, size_t & buf_length, codepoint_t);

/**
 * Same as add_escape_sequence(), only null-terminates.
 */
bool cat_escape_sequence(char *& buf, size_t & buf_length, codepoint_t);



#endif // sentry
