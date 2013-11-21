#include <map>
#include <errno.h>

#include "gtest/gtest.h"
#include "foundation/event_codes.h"
#include "foundation/interp.h"
#include "foundation/test/test_util.h"
#include "core/event_codes.h"

using namespace std;
using namespace intent::foundation;
using namespace intent::foundation::event_codes;

TEST(event_test, no_dups) {
	typedef std::map<int, char const *> Map;
	Map names_by_number;
	auto dm = events::get_event_data_items();
	for (auto i = dm.begin(); i != dm.end(); ++i) {
		auto item = **i;
		auto id = item.code;
		auto name = item.name;
		if (names_by_number.find(id) == names_by_number.end()) {
			names_by_number[id] = name;
		} else {
			ADD_FAILURE() << "Duplicate number " << events::get_std_id_repr(id)
					<< " (component " << events::get_component(id)
					<< ", event num " << events::get_nonunique_number(id)
					<< ") for " << name << " and " << names_by_number[id] << ".";
		}
	}
}

TEST(event_test, valid_names_and_argrefs) {
	auto dm = events::get_event_data_items();
	for (auto i = dm.begin(); i != dm.end(); ++i) {
		auto item = **i;
		auto id = item.code;
		auto name = item.name;
		for (auto p = name; *p; ++p) {
			if (toupper(*p) != *p) {
				ADD_FAILURE() << name << " is not upper-case.";
				break;
			}
		}

		// Scan the message text. Notice which args have been used; verify
		// that all argrefs are formatted correctly. Check capitalization and
		// punctuation.
		bool used_argrefs[] = {false, false, false, false, false, false, false,
				false, false};
		auto msg = events::get_msg(id);
		string prefix = interp("Message for %1 (\"%2\") ", name, msg);
		if (isalpha(*msg) && toupper(*msg) != *msg) {
			ADD_FAILURE() << prefix << "doesn't begin with a capital letter.";
		}
		if (strlen(msg) < 10) {
			ADD_FAILURE() << prefix << "is too short to be useful.";
		} else {
			auto end = strchr(msg, 0) - 1;
			if (*end != '.' && *end != '?') {
				if (!(*end >= '1' && *end <= '9' && end[-1] == '%')) {
					if (*end != '}') {
						ADD_FAILURE() << prefix << "isn't punctuated as a"
								" complete sentence.";
					}
				}
			}
		}
		if (strstr(msg, "  ")) {
			ADD_FAILURE() << prefix << "uses multiple spaces in a row.";
		}
		for (auto p = msg; *p; ++p) {

			char c = *p;
			auto argref_found = false;
			char const * bad_argref = NULL;

			if (c == '%') {
				if (p[1] == 0) {
					ADD_FAILURE() << prefix << "ends with an orphaned %.";
				} else if (p[1] == '%') {
					++p;
				} else if (p[1] >= '1' && p[1] <= '9') {
					argref_found = true;
				} else {
					bad_argref = p;
				}
			}

			if (argref_found) {
				// Remember that we saw this argref.
				used_argrefs[p[1] - '1'] = true;

				// Look at any format spec following the arg ref.
				//   p[0] -> %
				//   p[1] -> digit
				//   p[2] -> {
				if (p[2] == '{') {

					// Doubled {{ means no format spec.
					if (p[3] == '{') {
						p += 3;
					} else {

						auto fmtspec_end = strchr(p + 3, '}');
						if (fmtspec_end) {
							p = fmtspec_end;
						} else {
							ADD_FAILURE() << prefix
									<< "has an orphaned { at offset "
									<< p + 3 - msg << ".";
						}
					}
				}
			}
			if (bad_argref) {
				ADD_FAILURE() << prefix << "has an invalid argref at offset "
						<< p - msg	<< ".";
			}
		}

		// Now check argrefs against name; we expect name to have the digit
		// for each argref in it, and we expect no gaps in argrefs.
		auto last_argref = 0;
		for (int i = 9; i >= 1; --i) {
			if (used_argrefs[i - 1]) {
				last_argref = i;
				break;
			}
		}
		for (int i = 1; i <= last_argref; ++i) {

			auto digit = strchr(name, '0' + i);
			auto in_name =
					digit != NULL
					&& digit > name
					// digit appears at beginning of word, like _1XYZ
					&& ((digit[-1] == '_' && isalpha(digit[1])) ||
					// digit appears at end of word, like XYZ1
						((digit[1] == '_' || digit[1] == 0) && isalpha(digit[-1])));

			if (!used_argrefs[i - 1]) {
				if (in_name) {
					ADD_FAILURE() << prefix << "never refers to arg " << i
							<< ".";
				} else {
					ADD_FAILURE() << prefix << "never refers to arg " << i
							<< ", and its symbolic name doesn't contain \"_"
							<< i << "x\""
								" (where x is a mnemonic describing the arg).";
				}
			} else if (!in_name) {
				ADD_FAILURE() << prefix << "refers to arg " << i
						<< ", but its symbolic name doesn't contain \"_" << i
						<< "digit " << i << " at the beginning/end of a word.";
			}
		}
	}
}

TEST(event_test, known_event_properties) {
	auto e = E_NOT_IMPLEMENTED;
	EXPECT_STREQ("E_NOT_IMPLEMENTED", events::get_symbolic_name(e));
	EXPECT_EQ(sev_error, events::get_severity(e));
	EXPECT_EQ(kc_base, events::get_component(e));
	EXPECT_EQ(esc_internal, events::get_escalation(e));
	EXPECT_EQ(1, events::get_nonunique_number(e));
	EXPECT_STREQ("domain.base.internal", events::get_topic(e));
	expect_str_contains(events::get_msg(e), "not yet been implemented");
	expect_str_contains(events::get_comments(e), "stubbed");
	EXPECT_EQ(0, events::get_arg_count(e));
}

TEST(event_test, multi_arg_count) {
	auto e = E_FILE1_BAD_HUGE_LINE_2BYTES;
	EXPECT_EQ(2, events::get_arg_count(e));
}

ecode_t get_foreign_event_id() {
	return static_cast<ecode_t>(
			static_cast<int>(sev_warning) << 28
			| static_cast<int>(kc_mwm) << 16
			| static_cast<int>(esc_poweruser) << 14
			| 1234
			);
}

TEST(event_test, unknown_event_properties) {
	auto e = get_foreign_event_id();
	EXPECT_STREQ("", events::get_symbolic_name(e));
	EXPECT_EQ(sev_warning, events::get_severity(e));
	EXPECT_EQ(kc_mwm, events::get_component(e));
	EXPECT_EQ(esc_poweruser, events::get_escalation(e));
	EXPECT_EQ(1234, events::get_nonunique_number(e));
	EXPECT_STREQ("", events::get_topic(e));
	EXPECT_STREQ("", events::get_msg(e));
	EXPECT_STREQ("", events::get_comments(e));
	EXPECT_EQ(0, events::get_arg_count(e));
}

TEST(event_test, posix_properties) {
	auto e = ENOENT;
	EXPECT_STREQ("", events::get_symbolic_name(e));
	EXPECT_EQ(sev_error, events::get_severity(e));
	EXPECT_EQ(kc_posix, events::get_component(e));
	EXPECT_EQ(esc_user, events::get_escalation(e));
	EXPECT_EQ(2, events::get_nonunique_number(e));
	EXPECT_STREQ("", events::get_topic(e));
	expect_str_contains(events::get_msg(e).c_str(), "such file or directory");
	EXPECT_STREQ("", events::get_comments(e));
	EXPECT_EQ(0, events::get_arg_count(e));
}
