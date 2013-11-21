#include <cstdio>
#include <iostream>
#include <fstream>

#include "foundation/error.h"
#include "foundation/event_codes.h"
#include "foundation/file_lines.h"
#include "foundation/file_util.h"
#include "foundation/test/test_util.h"

#include "gtest/gtest.h"

using namespace intent::foundation::event_codes;
using namespace intent::foundation;

TEST(file_lines_test, nonexistent_file) {
	try {
		file_lines fl("doesn't@exist");
		ADD_FAILURE() << "Expected error_event about unavailable file.";
	} catch (error_event const & e) {
		ASSERT_EQ(e.get_event_code(), E_INPUT_FILE1_UNREADABLE);
	}
}

TEST(file_lines_test, readable_binary_file) {
	try {
		file_lines fl("/bin/ls");
		ADD_FAILURE() << "Expected error_event about binary file.";
	} catch (error_event const & e) {
		ASSERT_EQ(e.get_event_code(), E_FILE1_BAD_SEEMS_BINARY);
	}
}

TEST(file_lines_test, empty_existing_file) {
	auto temp_file = make_temp_file();
	delete_file_on_exit x(temp_file);
	try {
		file_lines fl(temp_file.c_str());
		ADD_FAILURE() << "Expected error_event about unreadable file.";
	} catch (error_event const & e) {
		ASSERT_EQ(e.get_event_code(), E_INPUT_FILE1_EMPTY);
	}
}

TEST(file_lines_test, text_file) {
	char const * const LINE = "Here's a happy line";
	auto temp_file = make_temp_file();
	delete_file_on_exit x(temp_file);
	FILE * f = fopen(temp_file.c_str(), "w");
	ASSERT_TRUE(f != nullptr);
	{
		close_file_on_exit y(f);
		char buf[512];
		buf[0] = 0;
		for (int i = 0; i < 5; ++i) {
			switch (i) {
			case 0:
				strcat(buf, "  "); break;
			case 1:
				strcat(buf, "\n\n\n"); break;
			case 2:
				strcat(buf, "\r\n\t"); break;
			case 3:
				strcat(buf, "  #"); break;
			}
			strcat(buf, LINE);
			if (i == 4) {
				strcat(buf, "  ");
			}
			strcat(buf, "\n");
		}
		fwrite(buf, 1, strlen(buf), f);
	}

	// Now see if we get the lines we expect (a few blank ones, plus a few
	// where our "seed" line value shows up in one form or another).
	file_lines fl(temp_file.c_str(), true, true);
	EXPECT_EQ(static_cast<size_t>(0), fl.get_current_line_num());
	int valid_line_count = 0;
	int blank_line_count = 0;
	for (int i = 0; i < 20; ++i) {
		char const * line = fl.next();
		if (!line) {
			// Don't break, but just ignore input; we do this to make sure
			// repeatedly calling at EOF doesn't cause problems.
		} else if (*line) {
			if (*line == '#') {
				++line;
			}
			EXPECT_STREQ(LINE, line);
			++valid_line_count;
		} else {
			++blank_line_count;
		}
	}
	EXPECT_EQ(5, valid_line_count);
	EXPECT_EQ(4, blank_line_count);
	EXPECT_EQ(static_cast<size_t>(9), fl.get_current_line_num());
	EXPECT_STREQ(NULL, fl.next());
}

// Undocumented but used by unit tests to artificially test boundaries without
// having to use massive amounts of RAM and do lots of I/O. This is the only
// gulp size < 64k that file_lines will accept.
const size_t TESTING_GULP_SIZE = 40;

TEST(file_lines_test, line_bigger_than_gulp) {
	auto temp_file = make_temp_file();
	delete_file_on_exit x(temp_file);
	FILE * f = fopen(temp_file.c_str(), "w");
	ASSERT_TRUE(f != nullptr);
	{
		close_file_on_exit y(f);
		char const * const FORTY_CHARS = ".123456789.123456789.123456789.123456789";

		char buf[128];
		memset(buf, 'x', 19);
		buf[19] = ' ';
		strcpy(buf + 20, FORTY_CHARS);
		strcat(buf + 60, "\nabc");
		fwrite(buf, 1, strlen(buf), f);
	}

	try {
		// Since we have a line that's longer than 40 chars, we should get
		// an exception about the file having lines that are too long. In
		// production, this exception could happen in the ctor or any time
		// thereafter when we do another gulp. In this test, we've forced it
		// to occur on the first gulp.
		file_lines fl(temp_file.c_str(), false, false, TESTING_GULP_SIZE);
		ADD_FAILURE() << "Expected block to throw an error_event.";
	} catch (error_event const & e) {
		EXPECT_EQ(E_FILE1_BAD_HUGE_LINE_2BYTES, e.get_event_code());
	}
}

TEST(file_lines_test, line_spanning_gulp) {
	auto temp_file = make_temp_file();
	delete_file_on_exit x(temp_file);
	FILE * f = fopen(temp_file.c_str(), "w");
	ASSERT_TRUE(f != nullptr);
	{
		close_file_on_exit y(f);
		char buf[128];
		memset(buf, 'x', 35);
		strcpy(buf + 35, "\nabcdefgjijklmnop \r\n xyz\n");
		fwrite(buf, 1, strlen(buf), f);
	}

	file_lines fl(temp_file.c_str(), false, false, TESTING_GULP_SIZE);
	auto line = fl.next();
	EXPECT_EQ(static_cast<size_t>(35), strlen(line));
	line = fl.next();
	EXPECT_STREQ("abcdefgjijklmnop ", line);
	line = fl.next();
	EXPECT_STREQ(" xyz", line);
	line = fl.next();
	EXPECT_STREQ(nullptr, line);
}
