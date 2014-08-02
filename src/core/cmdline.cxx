#include <string.h>
#include <stdlib.h>

#include "foundation/interp.h"
#include "core/cmdline.h"
#include "core/event_codes.h"

using namespace intent::core::event_codes;

namespace intent {

char const * cmdline::get_valid_flags() const {
	return "--help|-h";
}

/**
 * TODO: this is not the way I want to do things in the long run. I'd like to
 * call a base-class method that defines an option with a replacement text and
 * a validation function. So, for example, I'd like to do something like this:
 *
 *   cmdline.add_option("--inputfile", "FILE", file_must_exist);
 *
 * For the time being, this is as far as I got.
 */
char const * cmdline::get_valid_options() const {
	return "";
}

char const * cmdline::get_default_program_name() const {
	return "intent";
}

void cmdline::parse(int argc, char const ** argv) {
	cmdline_base::parse(argc, argv);
}

cmdline::cmdline(int argc, char const ** argv) {
	parse(argc, argv);
}

cmdline::~cmdline() {
}


std::string cmdline::get_help() const {
	std::string e;
	for (auto err: get_errors()) {
		if (!e.empty()) {
			e += "\n";
		}
		e += err.what();
	}
	return interp(
		"%1{errors}\n%2{progname} -- work with intent code\n"
		"\n"
		" Syntax: %2{progname} [flags] [options] [target(s)]\n"
		"\n"
		"  Flags:\n"
		"   --help or -h      -- Display this screen.\n"
		"\n"
		"  Options:\n"
		"   none at this time\n"
		"\n",
		e, get_program_name()
		);
}

} // end namespace intent
