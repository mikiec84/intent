#include "lang/parser.h"

#include "gtest/gtest.h"


using namespace intent::lang;

static constexpr char * const simple_module =
"car: vehicle\n"
"    properties:\n"
"        - make: #name\n"
"        - price: double\n";

TEST(parser_test, simple_module) {
    parser p(simple_module);
}

