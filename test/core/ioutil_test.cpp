#include <cstdio>
#include "core/ioutil.h"
#include "gtest/gtest.h"

using namespace boost::filesystem;
using namespace intent::core;

TEST(ioutil_test, file_delete_on_exit) {
    path tmp = easy_temp_file_path();
    ASSERT_FALSE(exists(tmp));
    FILE * f = fopen(tmp.c_str(), "w");
    fclose(f);
    ASSERT_TRUE(exists(tmp));
    {
        file_delete_on_exit fdoe(tmp);
    }
    ASSERT_FALSE(exists(tmp));
}

TEST(ioutil_test, c_file_closes_automatically) {
    path tmp = easy_temp_file_path();
    FILE * f = fopen(tmp.c_str(), "w");
    {
        file_delete_on_exit fdoe(tmp);
        {
            c_file f2(f);
            ASSERT_TRUE(fileno(f) != -1); // file descriptor is valid
        }
        ASSERT_TRUE(fileno(f) == -1); // file descriptor no longer valid; has been closed
    }
}

TEST(ioutil_test, c_file_usable_as_FILE_ptr) {
    path tmp = easy_temp_file_path();
    file_delete_on_exit fdoe(tmp);
    c_file f(tmp, "w");
    ftell(f);
    fileno(f);
    feof(f);
    fprintf(f, "test");
}

char const * const test_data = "hello\n\x01";

TEST(ioutil_test, read_binary_file) {
    easy_temp_c_file x;
    fwrite(test_data, 1, strlen(test_data), x);
    fclose(x);
    file_delete_on_exit fdoe(x.path);
    std::vector<uint8_t> bytes = read_binary_file(x.path);
    ASSERT_EQ(7U, bytes.size());
    ASSERT_EQ(0, strncmp(test_data, reinterpret_cast<char const *>(&bytes[0]), strlen(test_data)));
}

TEST(ioutil_test, read_text_file) {
    easy_temp_c_file x;
    fwrite(test_data, 1, strlen(test_data), x);
    fclose(x);
    file_delete_on_exit fdoe(x.path);
    std::string txt = read_text_file(x.path);
    ASSERT_EQ(7U, txt.size());
    ASSERT_STREQ(test_data, txt.c_str());
}
