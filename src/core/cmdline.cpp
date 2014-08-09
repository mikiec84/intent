#include "cmdline.h"

using std::string;

namespace intent {
namespace core {

struct cmdline::data_t {
    std::string program_name;
    std::string description;
    std::string epilog;

    typedef std::pair<cmdline const &, bool> source_info;
    std::vector<source_info> sources;

    std::vector<cmdline_parameter> params;
};

cmdline::cmdline(int argc, const char *argv[]) : data(new data_t) {
}

cmdline::cmdline(std::initializer_list<std::string> args) {
}

cmdline::~cmdline() {
    delete data;
}

bool cmdline::help_needed() const {
    return false;
}

string cmdline::get_help() const {
    return "help";
}

void cmdline::set_program_name(char const * name) {
    data->program_name = name;
}

char const * cmdline::get_program_name() const {
    return data->program_name.c_str();
}

void cmdline::set_description(char const * descrip) {
    data->description = descrip;
}

char const * cmdline::get_description() const {
    return data->description.c_str();
}

void cmdline::set_epilog(char const * epilog) {
    data->epilog = epilog;
}

char const * cmdline::get_epilog() const {
    return data->epilog.c_str();
}

void cmdline::add_source(cmdline const & source, bool include_positionals) {
    data->sources.push_back(std::make_pair(source, include_positionals));
}

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
void cmdline::define_parameter(char const * names, char const * help,
    char occurrence_rule, char const * placeholder,
    arg_validator_func validator) {

    cmdline_parameter param;
    //param.names = names;
    param.help = help;
    param.occurrence_rule = occurrence_rule;
    param.placeholder = placeholder;
    param.validator = validator;
    data->params.push_back(std::move(param));
}

std::vector<cmdline_parameter> cmdline::get_parameters() {
    return data->params;
}

}} // end namespace
