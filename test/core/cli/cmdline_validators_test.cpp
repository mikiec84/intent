#include <pcrecpp.h>

#include "core/cli/cmdline.h"
#include "core/cli/cmdline_validators.h"
#include "core/text/interp.h"
#include "core/util/countof.h"
#include "core/util/scope_guard.h"

#include "helpers/testutil.h"

#include "gtest/gtest.h"

using std::string;
using namespace intent::core::cli;
using namespace intent::core::text;
using namespace boost::filesystem;

static void check_is_in_range(char const * value, numeric_range_info const & nri,
        bool expected) {
    cmdline_param param;
    param.names.push_back("--number");
    auto err = in_numeric_range(param, value, &nri);
    if (expected != err.empty()) {
        auto msg = interp("With value = \"{1}\", min = {2}, max = {3}, and "
            "allowed_formats = {4}, is_in_range() should {5}.\nInstead, it {6}",
            { value, nri.min, nri.max,
              get_names_for_numeric_formats(nri.allowed_formats),
              (expected ? "succeed" : "fail"),
              (expected ? interp("failed with an error:\n    {1}\n", {err})
                        : string("succeeded.\n"))});
        ADD_FAILURE() << msg;
    }
}

TEST(cmdline_validators_test, in_numeric_range1) {
    numeric_range_info range = {0, 31, numeric_formats::all};
    check_is_in_range("3", range, true);
    check_is_in_range("31", range, true);
    check_is_in_range("0", range, true);
    check_is_in_range("07", range, true);
    check_is_in_range("0x0000001f", range, true);
    check_is_in_range("14.7", range, true);
    check_is_in_range("0b0000_1011", range, true);
    check_is_in_range("-1", range, false);
}

TEST(cmdline_validators_test, in_numeric_range2) {
    numeric_range_info range = {-2.9, 3.61e3,
        numeric_formats::floating_point};
    check_is_in_range("3", range, true);
    check_is_in_range("0", range, true);
    check_is_in_range("-2", range, true);
    check_is_in_range("3610", range, true);
    check_is_in_range("3610.1", range, false);
    check_is_in_range("0x0000001f", range, false); // disallowed format
    check_is_in_range("14.7", range, true);
    check_is_in_range("0b0000_1011", range, false); // disallowed format
    check_is_in_range("-5", range, false);
}

TEST(cmdline_validators_test, in_numeric_range3) {
    numeric_range_info range = {1025, 65535, numeric_formats::decimal};
    check_is_in_range("3", range, false);
    check_is_in_range("0", range, false);
    check_is_in_range("1024", range, false);
    check_is_in_range("3610", range, true);
    check_is_in_range("3610.1", range, false);
    check_is_in_range("0x0000001f", range, false); // disallowed format
    check_is_in_range("14.7", range, false); // disallowed format
    check_is_in_range("0b0000_1011", range, false); // disallowed format
    check_is_in_range("-5", range, false);
    check_is_in_range("65536", range, false);
}

static void check_regex(char const * value, pcrecpp::RE const & re,
        bool expected) {
    cmdline_param param;
    param.names.push_back("--expr");
    auto err = matches_regex(param, value, &re);
    if (expected != err.empty()) {
        auto msg = interp("With value = \"{1}\" and regex = \"{2}\", "
            "matches_regex() should {3}.\nInstead, it {4}",
            { value, re.pattern(),
              (expected ? "succeed" : "fail"),
              (expected ? interp("failed with an error:\n    {1}\n", {err})
                        : string("succeeded.\n"))});
        ADD_FAILURE() << msg;
    }
}

TEST(cmdline_validators_test, matches_regex) {
    pcrecpp::RE ipaddr_pat(
        "# ipv4 address\n"
        "(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)",
                pcrecpp::RE_Options().set_extended(true));
    check_regex("192.168.1.1", ipaddr_pat, true);
    check_regex("292.168.1.1", ipaddr_pat, false);

    // Prove that error message uses initial comment in regex instead of using
    // complex regex, if it's present.
    cmdline_param param;
    param.names.push_back("--expr");
    auto err = matches_regex(param, "abc", &ipaddr_pat);
    EXPECT_TRUE(strstr(err.c_str(), "<ipv4 address>"));
    EXPECT_FALSE(strstr(err.c_str(), "[0-9]"));
}

#define THIS_FNAME "cmdline_validators_test.cpp"
const path & this_folder() {
    static const path the_path = find_test_folder("test/core/cli/" THIS_FNAME);
    return the_path;
}

const path & this_file() {
    static const path the_path = this_folder() / THIS_FNAME;
    return the_path;
}

static const filesys_info existing_file_readable = {
    {regular_file}, 0444U, 0444U,
    0, std::numeric_limits<double>::infinity(), nullptr
};

static const filesys_info existing_file_min_size = {
    {regular_file}, 0, 0, 1024*1024,
    std::numeric_limits<double>::infinity(), nullptr
};

static const pcrecpp::RE txt_extension_pat(".*\\.txt");

static const filesys_info cant_exist_regex = {
    {file_not_found, status_error}, 0, 0,
    0, std::numeric_limits<double>::infinity(), &txt_extension_pat
};

inline cmdline_param const & get_sample_param() {
    static cmdline_param param;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        param.names.push_back("--in");
    }
    return param;
}

TEST(cmdline_validators_test, matches_filesys_info1) {
    auto err = matches_filesys_info(get_sample_param(), this_file().c_str(), &existing_file_readable);
    EXPECT_STREQ("", err.c_str());
}

TEST(cmdline_validators_test, matches_filesys_info2) {
    auto err = matches_filesys_info(get_sample_param(), this_folder().c_str(), &existing_file_readable);
    EXPECT_TRUE(strstr(err.c_str(), "is a folder"));
}

TEST(cmdline_validators_test, matches_filesys_info3) {
    auto err = matches_filesys_info(get_sample_param(), "This file doesn't exist!", &cant_exist_regex);
    EXPECT_TRUE(strstr(err.c_str(), "doesn't exist"));
    EXPECT_TRUE(strstr(err.c_str(), "invalid name"));
}

TEST(cmdline_validators_test, matches_filesys_info4) {
    auto err = matches_filesys_info(get_sample_param(), this_file().c_str(), &existing_file_min_size);
    EXPECT_TRUE(strstr(err.c_str(), "too small"));
}

static const filesys_info existing_file_exe_max_size = {
    {regular_file}, 0111U, 0111U,
    0, 10, nullptr
};

TEST(cmdline_validators_test, matches_filesys_info5) {
    auto err = matches_filesys_info(get_sample_param(), this_file().c_str(), &existing_file_exe_max_size);
    EXPECT_TRUE(strstr(err.c_str(), "too big"));
    EXPECT_TRUE(strstr(err.c_str(), "not executable"));
}

