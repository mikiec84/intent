#ifndef _369228301fc2464bb1c053a5d9662a65
#define _369228301fc2464bb1c053a5d9662a65

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/text/numeric_formats.h"
#include "core/text/scan_numbers.h"

namespace intent {
namespace core {
namespace cli {

struct cmdline_param;

/**
 * Examine a single arg and validate it against its corresponding parameter
 * definition. Return a string describing problems, or the empty string on
 * success.
 *
 * @param reference_data: allows the same algorithm to compare against different
 *     reference values (e.g., the same "number is in range" function comparing
 *     against different boundaries each time it's invoked).
 */
typedef std::string (* arg_validator_func)(cmdline_param const & param,
	char const * value, void const * reference_data);

/**
 * Hold information about one defined flag.
 */
struct cmdline_flag {
	std::vector<std::string> names;
	std::string help;
};

/**
 * Hold information about one defined parameter.
 */
struct cmdline_param {
	std::vector<std::string> names;
	std::string help;
	char occurrence_rule;
	std::string placeholder;
	arg_validator_func validator;
	void * reference_data;

	/**
	 * Return true if this param is recognized by its position rather than
	 * a hyphen-prefixed name.
	 */
	bool is_positional() const;
	std::string const & get_preferred_name() const;
};

/**
 * Hold information about the combination of parameters and semantics that
 * apply to one cmdline.
 *
 * The lifetime of this object falls into three distinct phases: definition,
 * validation, and usage. In the definition phase, it is legal to call the ctor
 * and all setters/adders. PRECONDITIONs are used to validate some definition
 * input, but holistic validation is deferred. The validation phase lasts for
 * the duration of a call to validate(); during this phase, all definitions are
 * sanity-checked for mutual compatibility. If the caller does not call validate()
 * explicitly, it is called the when a request to transition to the usage phase
 * is made by calling set_args() is called. During the usage phase, it is legal
 * to call getters, but not to change the object's definitions.
 * set_args() may be called repeatedly on the same object; each time, validation
 * and and errors are recalculated.
 */
class cmdline /* -<threadsafe */ {
public:

	enum lifecycle_phase {
		definition,
		validation,
		usage
	};

	cmdline();
	cmdline(const cmdline &) = delete;

	virtual ~cmdline();

	// +<callable_in_lifecycle_phases(usage)
	void set_args(int argc, char const * argv[]);

	// +<callable_in_lifecycle_phases(usage)
	void set_args(std::initializer_list<std::string> args);

	/**
	 * Override argv[0] as the name of the program.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void set_program_name(char const * name);

	char const * get_program_name() const;

	/**
	 * Describe the purpose and behavior associated with this cmdline.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void set_description(char const * descrip);

	char const * get_description() const;

	/**
	 * Add explanatory text after syntax dump.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void set_epilog(char const * epilog);

	char const * get_epilog() const;

	/**
	 * Copy args from another cmdline. This method can be called multiple
	 * times.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void add_source(cmdline const & source, bool include_positionals = false);

	/**
	 * Define a single parameter.
	 *
	 * @param names: a comma-delimited list of names for the argument. Names
	 *     should be prefixed with - if they are short forms, or -- if they are
	 *     long forms. Positional args should not be prefixed.
	 * @param help: an explanation of the meaning of the arg.
	 * @param occurrence_rule: constraint on quantity ("1" means exactly once,
	 *     "?" means zero or 1, "*" means zero or more, "+" means 1 or more,
	 *     any other digit forces that exact count).
	 * @param placeholder: represents the value of this arg in overall usage,
	 *     such as "FNAME" in "--infile FNAME". This parameter is ignored for
	 *     positional args (ones with a names not preceded by hyphens).
	 * @param validator: A function that tests validity of values associated
	 *     with this parameter.
	 * @param validator_reference_data: An arbitrary piece of state that can
	 *     be used to customize validator behavior. For example, multiple
	 *     cmdline_param instances may all use a "matches_regex" validator,
	 *     but each one may take a different regex as input, passed through
	 *     this parameter.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void define_param(char const * names, char const * help,
		char occurrence_rule = '1', char const * placeholder = nullptr,
		arg_validator_func validator = nullptr,
		void * validator_reference_data = nullptr);

	/**
	 * Define a flag -- a parameter that, if present, signals "true", and if
	 * missing, signals false.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void define_flag(char const * names, char const * help);

	std::vector<cmdline_param> const & get_params() const;
	std::vector<cmdline_flag> const & get_flags() const;
	std::vector<std::string> const & get_args() const;

	/**
	 * Given all known flags and params, what is the maximum width of the
	 * widest names? This is useful for printing names in a fixed-width column.
	 */
	size_t get_max_name_width() const;

	// +<callable_in_lifecycle_phases(usage)
	bool help_needed() const;

	// +<callable_in_lifecycle_phases(usage)
	std::string get_help() const;

	lifecycle_phase get_lifecycle_phase() const;

	/**
	 * Thrown when cmdline definitions are not mutually compatible.
	 */
	struct validation_error: public std::logic_error {};

	/**
	 * Transitions from definition to usage. Throws validation_error when
	 * problems are found.
	 */
	// +<callable_in_lifecycle_phases(definition)
	void validate() noexcept(false);

private:
	struct data_t;
	data_t * data;
};

}}} // end namespace

#include "core/cli/cmdline-inline.h"

#endif // sentry
