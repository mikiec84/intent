#include <pcrecpp.h>

#include "core/cmdline.h"
#include "core/countof.h"
#include "core/interp.h"
#include "core/scope_guard.h"

#include "gtest/gtest.h"

using std::string;
using namespace intent::core;

TEST(cmdline_test, basic) {
    cmdline c;
    c.define_flag("-h, --help", "show help for program");
    EXPECT_EQ(1U, c.get_flags().size());

    char const * argv[] = {"this", "is", "a", "test"};
    int argc = countof(argv);
    c.set_args(argc, argv);

    EXPECT_EQ(4U, c.get_args().size());
}

bool text_echoed = false;

inline void check_text(char const * txt, char const * expected) {
    if (!strstr(txt, expected)) {
        if (!text_echoed) {
            text_echoed = true;
            ADD_FAILURE() << "Actual text was: \"" << txt << "\".";
        }
        ADD_FAILURE() << "Expected to find \"" << expected <<
            "\" in text, but did not.";
    }
}

template <typename T>
void check_item(char const * txt, T const & item) {
    check_text(txt, item.help.c_str());
    for (auto & name: item.names) {
        check_text(txt, name.c_str());
    }
}

TEST(cmdline_test, get_help) {
    text_echoed = false;
    on_scope_exit(text_echoed = false);

    cmdline c;
    c.set_program_name("foo");
    c.set_description("Convert some files.");
    c.define_flag("-h, --help", "show help for program");
    c.define_flag("-v, --verbose", "run in verbose mode");
    c.define_param("-i, --infile", "input file", '+', "FNAME");
    c.define_param("--outfile", "output file", '?');
    c.define_param("-s, --style", "what style of output should we create?", '4');
    c.set_epilog("Files should be smaller than 10MB.");
    auto help = c.get_help();
    auto help_c = help.c_str();
    //printf("help = \"%s\"", help_c);
    for (auto & item: c.get_params()) {
        check_item(help_c, item);
    }
    for (auto & item: c.get_flags()) {
        check_item(help_c, item);
    }
    check_text(help_c, c.get_program_name());
    check_text(help_c, c.get_description());
    check_text(help_c, c.get_epilog());
    check_text(help_c, "[-h]");
    check_text(help_c, "-i=FNAME [-i=FNAME...]");
    check_text(help_c, "[--outfile=OUTFILE]");
    check_text(help_c, "-s=STYLE1...-s=STYLE4");
}

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

TEST(cmdline_test, in_numeric_range1) {
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

TEST(cmdline_test, in_numeric_range2) {
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

TEST(cmdline_test, in_numeric_range3) {
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

TEST(cmdline_test, matches_regex) {
    pcrecpp::RE ipaddr_pat(
        "(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    check_regex("192.168.1.1", ipaddr_pat, true);
    check_regex("292.168.1.1", ipaddr_pat, false);
}

TEST(cmdline_test, DISABLED_more_to_do) {
#if 0
    create validation logic in cmdline that proves param/flag names don't collide
    create validation logic when args are added
    make help insert error messages at top, if they exist
    calculate shortest name possible for a given param/flag
    support embedding other cmdlines as variants
    write default validators (file [or folder] exists [or not], regex, numeric range)
#endif
}

