#ifndef CMDLINE_H
#define CMDLINE_H

#include <initializer_list>
#include <string>
#include <vector>

#include "scan_numbers.h"
#include "numeric_formats.h"

namespace intent {
namespace core {

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
 * Hold information about a combination of parameters and semantics that
 * apply to one cmdline.
 *
 * The lifetime of this object falls into two distinct phases: definition,
 * and usage. In the definition phase, it is legal to call the ctor and all
 * setters/adders. The usage phase begins when set_args() is called; thereafter,
 * it is legal to call getters, but not to change the object's definitions.
 * set_args() may be called repeatedly on the same object; each time, validation
 * and and errors are recalculated.
 */
class cmdline {
public:
    cmdline();
    cmdline(const cmdline &) = delete;

    virtual ~cmdline();

    void set_args(int argc, char const * argv[]);
    void set_args(std::initializer_list<std::string> args);

    /**
     * Override argv[0] as the name of the program.
     */
    void set_program_name(char const * name);
    char const * get_program_name() const;

    /**
     * Describe the purpose and behavior associated with this cmdline.
     */
    void set_description(char const * descrip);
    char const * get_description() const;

    /**
     * Add explanatory text after syntax dump.
     */
    void set_epilog(char const * epilog);
    char const * get_epilog() const;

    /**
     * Copy args from another cmdline. This method can be called multiple
     * times.
     */
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
     */
    void define_param(char const * names, char const * help,
        char occurrence_rule = '1', char const * placeholder = nullptr,
        arg_validator_func validator = nullptr,
        void * validator_reference_data = nullptr);

    /**
     * Define a flag -- a parameter that, if present, signals "true", and if
     * missing, signals false.
     */
    void define_flag(char const * names, char const * help);

    std::vector<cmdline_param> const & get_params() const;
    std::vector<cmdline_flag> const & get_flags() const;
    std::vector<std::string> const & get_args() const;

    /**
     * Given all known flags and params, what is the maximum width of the
     * widest names?
     */
    size_t get_max_name_width() const;

    bool help_needed() const;
    std::string get_help() const;

private:
    struct data_t;
    data_t * data;
};

}} // end namespace

#include "cmdline-inline.h"

#endif // sentry
