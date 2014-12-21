#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <typeinfo>
#include <unistd.h>

#include "core/util/countof.h"
#include "core/text/escape_sequence.h"
#include "core/text/interp.h"
#include "core/io/ioutil.h"
#include "core/text/line_iterator.h"
#include "core/text/strutil.h"

#include "lang/lexer.h"

#include "helpers/testutil.h"
#include "gtest/gtest.h"

using std::string;
using std::vector;

using boost::any_cast;
using boost::lexical_cast;
using namespace boost::filesystem;

using namespace intent::core;
using namespace intent::core::io;
using namespace intent::core::text;
using namespace intent::lang;

#define EXPECT_TOKEN_TYPE(ex, actual, i) \
    if (ex != actual) FAIL() << "Expected token[" << i << "] type to be " << get_token_type_name(ex) \
               << " instead of " << get_token_type_name(actual) << "."

TEST(lexer_test, unterminated_string_literal) {
    lexer lex("\"abc\n"
              "xyz");
    auto it = lex.begin();
    ASSERT_EQ(tt_quoted_string, it->type);
    string v = any_cast<string>(it->value);
    ASSERT_STREQ("abc", v.c_str());

    // Prove that we emit an error in the correct place
    ++it;
    ASSERT_EQ(tt_error, it->type);
    v = any_cast<string>(it->value);
    string expected = interp("line 2, offset 0: error {1} -- {2}", {
                           static_cast<int>(ii_unterminated_string_literal),
                           get_issue_msg(ii_unterminated_string_literal)});
    ASSERT_STREQ(expected.c_str(), v.c_str());
    // Now prove that we resume in the correct state
    ++it;
    ASSERT_EQ(tt_noun_phrase, it->type);
    v = any_cast<string>(it->value);
    ASSERT_STREQ("xyz", v.c_str());
}

TEST(lexer_test, wrapped_comment) {
    lexer lex("  | this is a comment\n"
              "  ... that spans multiple\n"
              "  ... lines.\n"
              "  x = 3"
                );
    auto it = lex.begin();
    ASSERT_EQ(tt_indent, it->type);
    ++it;
    ASSERT_EQ(tt_doc_comment, it->type);
    string const & v = any_cast<string const &>(it->value);
    ASSERT_STREQ("this is a comment that spans multiple lines.", v.c_str());
}

TEST(lexer_test, indent_and_dedent) {
    lexer lex(
                "1\n"
                "\t11\n"
                "\t12\n"
                "\t\t121\n"
                "\t\t122\n"
                "20\n"
                "30\n"
                "\t31\n"
                );
    token_type expected[] = {
        tt_decimal_number,
        tt_indent,
        tt_decimal_number,
        tt_decimal_number,
        tt_indent,
        tt_decimal_number,
        tt_decimal_number,
        tt_dedent,
        tt_dedent,
        tt_decimal_number,
        tt_decimal_number,
        tt_indent,
        tt_decimal_number,
        tt_dedent
    };
    auto it = lex.begin();
    for (unsigned i = 0; i < countof(expected); ++i) {
        EXPECT_TOKEN_TYPE(expected[i], it->type, i);
        ++it;
        if (it == lex.end() && i < countof(expected) - 1) {
            FAIL() << "Ended after Token[" << i << "].";
        }
    }
}

void check_tokens(char const * txt, std::initializer_list<token_type> tokens) {
    lexer lex(txt);
    lexer::iterator lit = lex.begin();
    unsigned i;
    auto t = tokens.begin();
    for (i = 0; i < 100u; ++i) {
        EXPECT_TOKEN_TYPE(*t, lit->type, i);
        ++lit;
        if (!lit) {
            break;
        }
        ++t;
        if (!t) {
            break;
        }
    }
    EXPECT_EQ(tokens.size(), i + 1);
}

void check_tokens_with_and_without_whitespace(char const * txt, std::initializer_list<token_type> tokens) {
    check_tokens(txt, tokens);
    string without_whitespace;
    for (; *txt; ++txt) {
        auto c = *txt;
        if (c != ' ' && c != '\t') {
            without_whitespace += c;
        }
    }
    if (strlen(txt) > without_whitespace.size()) {
        check_tokens(without_whitespace.c_str(), tokens);
    }
}

// This used to hang because noun phrases didn't advance and got in an endless loop.
// It also used to fail because the ':' operator wasn't handled. So, even though
// it's a ridiculously simple test, it's a good canary-in-the-coal-mine.
TEST(lexer_test, simple_def) {
    check_tokens_with_and_without_whitespace("x: 3", {tt_noun_phrase, tt_operator_define, tt_decimal_number});
}

TEST(lexer_test, DISABLED_hyphenated_noun) {
    check_tokens("left-handed batter: 2.4", {tt_noun_phrase, tt_operator_define, tt_floating_point_number});
}

TEST(lexer_test, greater) {
    check_tokens_with_and_without_whitespace("a > b", {tt_noun_phrase, tt_operator_greater, tt_noun_phrase});
}

TEST(lexer_test, less) {
    check_tokens_with_and_without_whitespace("a < b", {tt_noun_phrase, tt_operator_less, tt_noun_phrase});
}

TEST(lexer_test, greater_equal) {
    check_tokens_with_and_without_whitespace("a >= b", {tt_noun_phrase, tt_operator_greater_equal, tt_noun_phrase});
}

TEST(lexer_test, less_equal) {
    check_tokens_with_and_without_whitespace("a <= b", {tt_noun_phrase, tt_operator_less_equal, tt_noun_phrase});
}

TEST(lexer_test, quoted_string) {
    struct test_item {
        char const * txt;
        char const * expected_value;
    };

    #define QUOTE(str) "\"" str "\""
    test_item const quoted_strs[] = {
        {QUOTE("\\x7F \\u5C0D "), "\x7F \xE5\xB0\x8D "},
        {QUOTE("abc"), "abc"},
        {QUOTE(""), ""},
        {QUOTE("\x7F \xE5\xB0\x8D "), "\x7F \xE5\xB0\x8D "},
        {QUOTE("\\r\\n\\t\\b\\v\\a\\f \\u "), "\r\n\t\b\v\a\f \\u "},
        {QUOTE("abc\n... def"), "abc def"},
    };
    for (unsigned i = 0; i < countof(quoted_strs); ++i) {
        auto txt = quoted_strs[i].txt;
        lexer lex(txt);
        auto it = lex.begin();
        if (!is_string_literal(it->type)) {
            ADD_FAILURE() << "Expected \"" << txt << "\" to be parsed as a quoted string; got "
                          << get_token_type_name(it->type) << " instead.";
        }
        string const & v = any_cast<string const &>(it->value);
        EXPECT_STREQ(quoted_strs[i].expected_value, v.c_str());
        if (++it != lex.end()) {
            ADD_FAILURE() << "Did not consume full quoted string with \"" << txt << "\".";
        }
    }
}

TEST(lexer_test, iterator_on_empty_str) {
    lexer lex("");
    token t;
    EXPECT_EQ(0, std::count (lex.begin(), lex.end(), t));
}

TEST(lexer_test, numbers) {
    char const * number_strs[] = {
        "0", "-1", "+25.7", "36121", "123_456_789", "0x03", "0b0100100", "0023", "3.7e-5",
    };

    for (unsigned i = 0; i < countof(number_strs); ++i) {
        lexer lex(number_strs[i]);
        auto it = lex.begin();
        if (!is_number_literal(it->type)) {
            ADD_FAILURE() << "Expected \"" << number_strs[i] << "\" to be parsed as a numeric token; got "
                          << get_token_type_name(it->type) << " instead.";
        }
        if (++it != lex.end()) {
            ADD_FAILURE() << "Did not consume full numeric token with \"" << number_strs[i] << "\".";
        }
    }
}

TEST(lexer_test, DISABLED_malformed_numbers) {
#if 0
    What if a number has multiple underscores in a row, is all underscores, or has
    underscores in a trailing position?

    What if we have overflow or underflow?

    What about width suffixes on numbers?

    What about signed hex/binary/octal?

    What about long doubles and arbitrary precision?

    What about complex numbers?
#endif
}

void complain(path const & p, unsigned n, string const & msg, string & errors) {
    if (!errors.empty()) {
        errors += "\n\n";
    }
    errors += interp("{1}, token {2}: {3}.", {p.filename().c_str(), n, msg});
}

static string get_any_as_string(boost::any const & value) {
    if (value.empty()) {
        return "";
    }
    auto & tid = value.type();
    try {

        #define TRY(typ) \
        if (tid == typeid(typ)) { \
            return lexical_cast<string>(any_cast<typ>(value)); \
        }

        TRY(std::string)
        else TRY(int64_t)
        else TRY(double)
    } catch (const boost::bad_any_cast &) {
    }
    return "<unrepresentable value>";
}

// Track all errors found during a given comparison. If any, emit the errors
// and save the full output of the lexer to a file for human post-mortem.
struct token_dumper {

    path const & i;
    string & errors;
    size_t initial_error_size;

    token_dumper(path const & i, string & errors): i(i), errors(errors),
        initial_error_size(errors.size()) {
    }

    ~token_dumper() {
        if (errors.size() > initial_error_size) {
            path final_path;
            {
                easy_temp_c_file tmp;
                final_path = tmp.path;
                string i_txt = read_text_file(i);
                if (i_txt.size() > 0) {
                    lexer lex(i_txt.c_str());
                    lexer::iterator lit = lex.begin();
                    while (lit) {
                        char const * type_name = get_token_type_name(lit->type);
                        string actual_value = get_any_as_string(lit->value);
                        actual_value = insert_escape_sequences(actual_value);
                        fprintf(tmp, "%s,%s\n", type_name, actual_value.c_str());
                        ++lit;
                    }
                }
            }
            path x_path = temp_directory_path();
            x_path /= path(i.filename().string() + ".lex");
            if (exists(x_path)) {
                try {
                    boost::filesystem::remove(x_path);
                } catch (...) {
                }
            }
            if (!exists(x_path)) {
                boost::filesystem::rename(final_path, x_path);
                final_path = x_path;
            }
            if (initial_error_size == 0) {
                errors = interp("For human post-mortem, lexer output has been saved in this temp folder:\n  {1}\n\n",
                    {final_path.parent_path().c_str()}) + errors;
            }
            errors += interp("\n  -- see lexer output in <temp>/{1}.", {
                    final_path.filename()});
        }
    }
};

void verify_lex(path const & i, path const & csv, string & errors) {
    string i_txt = read_text_file(i);
    if (i_txt.size() > 0) {

        // Dump all lexed tokens to a file, and emit all errors together, if
        // anything doesn't look correct in the current analysis. (This object
        // does its work by examining the errors that have accumulated by the
        // end of its lifetime.)
        token_dumper td(i, errors);

        string csv_txt = read_text_file(csv);
        if (csv_txt.size() > 0) {

            unsigned n = 1;
            lexer lex(i_txt);
            lexer::iterator lex_it = lex.begin();
            line_iterator csv_it(csv_txt);

            while (csv_it && lex_it) {

                const auto end = csv_it->end();
                char const * p = find_char(csv_it->begin, ',', end);
                if (p != end) {

                    str_view expected_token_type_name(csv_it->begin, p);
                    char const * actual_token_type_name = get_token_type_name(lex_it->type);

                    if (strcmp(expected_token_type_name, actual_token_type_name) != 0) {
                        complain(i, n, interp("expected token to be of type {1}, not {2}",
                                              {expected_token_type_name,
                                              actual_token_type_name} ), errors);
                        break;
                    }

                    str_view expected_value(p + 1, end);
                    string actual_value = get_any_as_string(lex_it->value);

                    if (strcmp(expected_value, actual_value.c_str()) != 0) {
                        complain(i, n, interp("expected token to have value \"{1}\", not \"{2}\"",
                                              {expected_value,
                                              actual_value} ), errors);
                        break;
                    }
                } else {
                    complain(csv, n, "malformed; expected comma-separated line", errors);
                    break;
                }
                ++n;
                ++csv_it;
                ++lex_it;
            }

            if (errors.empty()) {
                if (csv_it) {
                    complain(i, n, interp("ran out of tokens before {1} ended",
                        {csv.filename().c_str()}), errors);
                } else if (lex_it) {
                    complain(i, n, interp("kept generating tokens though {1} ended",
                        {csv.filename().c_str()}), errors);
                }
            }
        } else {
            fprintf(stderr, "%s is empty and will be skipped.\n", csv.c_str());
        }
    } else {
        fprintf(stderr, "%s is empty and will be skipped.\n", i.c_str());
    }
}

TEST(lexer_test, DISABLED_samples) {
    string errors;
    path data_folder = find_test_folder(__FILE__);
    ASSERT_TRUE(data_folder.c_str()[0]);
    data_folder /= "data";
    directory_iterator end = directory_iterator();
    for (directory_iterator i = directory_iterator(data_folder); i != end; ++i) {
        if (strcmp(extension(*i).c_str(), ".i") == 0) {
            path complement(i->path());
            complement += ".csv";
            if (exists(complement)) {
                verify_lex(*i, complement, errors);
            }
        }
    }
    if (!errors.empty()) {
        FAIL() << interp("\nNot all samples in {1} lexed correctly.\n\n{2}", {data_folder, errors});
    }
}
