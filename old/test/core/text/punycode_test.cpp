#include "core/text/punycode.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* For testing, we'll just set some compile-time limits rather than */
/* use malloc(), and set a compile-time option rather than using a  */
/* command-line option.                                             */

enum {
  unicode_max_length = 256,
  ace_max_length = 256
};

TEST(punycode_test, basic) {
#if 0
    punycode_status status;
  int r;
  unsigned int input_length, output_length, j;
  unsigned char case_flags[unicode_max_length];

  if (argc != 2) usage(argv);
  if (argv[1][0] != '-') usage(argv);
  if (argv[1][2] != 0) usage(argv);

  if (argv[1][1] == 'e') {
    punycode_uint input[unicode_max_length];
    unsigned long codept;
    char output[ace_max_length+1], uplus[3];
    int c;

    /* Read the input code points: */

    input_length = 0;

    for (;;) {
      r = scanf("%2s%lx", uplus, &codept);
      if (ferror(stdin)) fail(io_error);
      if (r == EOF || r == 0) break;

      if (r != 2 || uplus[1] != '+' || codept > (punycode_uint)-1) {
        fail(invalid_input);
      }

      if (input_length == unicode_max_length) fail(too_big);

      if (uplus[0] == 'u') case_flags[input_length] = 0;
      else if (uplus[0] == 'U') case_flags[input_length] = 1;
      else fail(invalid_input);

      input[input_length++] = codept;
    }

    /* Encode: */

    output_length = ace_max_length;
    status = punycode_encode(input_length, input, case_flags,
                             &output_length, output);
    if (status == punycode_bad_input) fail(invalid_input);
    if (status == punycode_big_output) fail(too_big);
    if (status == punycode_overflow) fail(overflow);
    assert(status == punycode_success);

    /* Convert to native charset and output: */

    for (j = 0;  j < output_length;  ++j) {
      c = output[j];
      assert(c >= 0 && c <= 127);
      if (print_ascii[c] == 0) fail(invalid_input);
      output[j] = print_ascii[c];
    }

    output[j] = 0;
    r = puts(output);
    if (r == EOF) fail(io_error);
    return EXIT_SUCCESS;
  }

  if (argv[1][1] == 'd') {
    char input[ace_max_length+2], *p, *pp;
    punycode_uint output[unicode_max_length];

    /* Read the Punycode input string and convert to ASCII: */

    fgets(input, ace_max_length+2, stdin);
    if (ferror(stdin)) fail(io_error);
    if (feof(stdin)) fail(invalid_input);
    input_length = strlen(input) - 1;
    if (input[input_length] != '\n') fail(too_big);
    input[input_length] = 0;

    for (p = input;  *p != 0;  ++p) {
      pp = strchr(print_ascii, *p);
      if (pp == 0) fail(invalid_input);
      *p = pp - print_ascii;
    }

    /* Decode: */

    output_length = unicode_max_length;
    status = punycode_decode(input_length, input, &output_length,
                             output, case_flags);
    if (status == punycode_bad_input) fail(invalid_input);
    if (status == punycode_big_output) fail(too_big);
    if (status == punycode_overflow) fail(overflow);
    assert(status == punycode_success);

    /* Output the result: */

    for (j = 0;  j < output_length;  ++j) {
      r = printf("%s+%04lX\n",
                 case_flags[j] ? "U" : "u",
                 (unsigned long) output[j] );
      if (r < 0) fail(io_error);
    }

    return EXIT_SUCCESS;
  }

  usage(argv);
  return EXIT_SUCCESS;  /* not reached, but quiets compiler warning */
#endif
}

