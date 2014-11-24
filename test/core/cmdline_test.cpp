#include <pcrecpp.h>

#include "core/cli/cmdline.h"
#include "core/util/countof.h"
#include "core/text/interp.h"
#include "core/util/scope_guard.h"

#include "gtest/gtest.h"

using std::string;
using namespace intent::core::cli;

TEST(cmdline_test, basic) {
    cmdline c;
    c.define_flag("-h, --help", "show help for program");
    EXPECT_EQ(1U, c.get_flags().size());

    char const * argv[] = {"this", "is", "a", "test"};
    int argc = countof(argv);
    c.validate();
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
    c.validate();
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

