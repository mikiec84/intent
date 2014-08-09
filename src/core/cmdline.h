#ifndef CMDLINE_H
#define CMDLINE_H

#include <initializer_list>
#include <string>
#include <vector>

namespace intent {
namespace core {

struct cmdline_parameter;

/**
 * Examine a single arg and validate it against its corresponding parameter
 * definition. Return a string describing problems, or the empty string on
 * success.
 */
typedef std::string (* arg_validator_func)(cmdline_parameter const &,
    char const * value);

/**
 * Hold information about one defined parameter.
 */
struct cmdline_parameter {
    std::vector<std::string> names;
    std::string help;
    char occurrence_rule;
    std::string placeholder;
    arg_validator_func validator;
};

/**
 * Hold information about a combination of parameters and semantics that
 * apply to one cmdline.
 */
class cmdline {
public:
    cmdline(int argc, char const * argv[]);
    cmdline(std::initializer_list<std::string> args);
    virtual ~cmdline();

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
     *     such as "FNAME" in --infile FNAME.
     */
    void define_parameter(char const * names, char const * help,
        char occurrence_rule = '1', char const * placeholder = nullptr,
        arg_validator_func validator=nullptr);

    std::vector<cmdline_parameter> get_parameters();

    bool help_needed() const;
    std::string get_help() const;

private:
    struct data_t;
    data_t * data;
};

}} // end namespace

#endif // sentry
