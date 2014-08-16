#include <algorithm>
#include <cmath>
#include <sstream>

#include "cmdline.h"
#include "dbc.h"
#include "strutil.h"

using std::string;
using std::stringstream;
using std::vector;

namespace intent {
namespace core {

static inline char const * get_name_after_prefix(char const * name) {
    while (*name == '-') {
        ++name;
    }
    return name;
}

static string generate_placeholder(vector<string> const & names) {
    string answer;
    char const * best_name = nullptr;
    unsigned best_len = 0;
    if (names.size() == 1) {
        best_name = get_name_after_prefix(names[0].c_str());
        best_len = strlen(best_name);
    } else {
        for (auto & name: names) {
            auto name_ptr = get_name_after_prefix(name.c_str());
            auto name_len = strlen(name_ptr);
            bool choose = false;
            if (!best_name) {
                choose = true;
            } else {
                // Pick smallest name, except prefer at least 3 chars.
                if (best_len < 3 && name_len >= 3) {
                    choose = true;
                } else if (best_len > name_len) {
                    choose = true;
                }
            }
            if (choose) {
                best_name = name_ptr;
                best_len = name_len;
            }
        }
    }
    for (const char * p = best_name; p < best_name + best_len; ++p) {
        if (isalnum(*p)) {
            answer += toupper(*p);
        }
    }
    return answer;
}

static string get_usage_for_param(stringstream & ss, cmdline_param const & param) {
    string occurrence = param.names[0];
    if (!param.is_positional()) {
        occurrence += "=" + param.placeholder;
    }
    switch (param.occurrence_rule) {
    case '*':
        return string("[") + occurrence + "...]";
    case '?':
        return string("[") + occurrence + "]";
    case '+':
        return occurrence + " [" + occurrence + "...]";
    case '1':
        return occurrence;
    case '2':
        return occurrence + "1 " + occurrence + "2";
    default:
        occurrence += "1..." + occurrence;
        occurrence += param.occurrence_rule;
        return occurrence;
    }
}

struct cmdline::data_t {
    bool defining;
    string program_name;
    string description;
    string epilog;

    typedef std::pair<cmdline const *, bool> source_info;
    vector<source_info> sources;

    vector<cmdline_param> params;
    vector<cmdline_flag> flags;
    vector<string> args;

    data_t() : defining(true) {}
};

cmdline::cmdline() : data(new data_t) {
}

void cmdline::set_args(int argc, const char *argv[]) {
    data->defining = false;
    for (int i = 0; i < argc; ++i) {
        data->args.push_back(string(argv[i]));
    }
}

void cmdline::set_args(std::initializer_list<string> args) {
    data->defining = false;
    for (auto arg: args) {
        data->args.push_back(string(arg));
    }
}

cmdline::~cmdline() {
    delete data;
}

vector<string> const & cmdline::get_args() const {
    PRECONDITION(!data->defining);
    return data->args;
}

bool cmdline::help_needed() const {
    PRECONDITION(!data->defining);
    return false;
}

template <typename T>
size_t _get_max_name_width(vector<T> const & items) {
    size_t biggest = 0;
    for (auto & item: items) {
        size_t n = 0;
        for (auto & name: item.names) {
            auto m = name.size();
            if (n) {
                n += 2;
            }
            n += m;
        }
        if (n > biggest) {
            biggest = n;
        }
    }
    return biggest;
}

size_t cmdline::get_max_name_width() const {
    return std::max(_get_max_name_width(data->flags),
                    _get_max_name_width(data->params));
}

template <typename T>
void dump_items(stringstream & ss, vector<T> const & items, char const * label,
    size_t name_width) {

    if (!items.empty()) {
        ss << "\n" << label << "\n";
        for (auto & item: items) {
            ss << "    ";
            size_t my_name_width = 0;
            char const * delim = nullptr;
            for (auto & name: item.names) {
                if (delim) {
                    ss << delim;
                    my_name_width += 2;
                } else {
                    delim = ", ";
                }
                ss << name;
                my_name_width += name.size();
            }
            ss << (EIGHTY_SPACES + (80 - (name_width - my_name_width)));
            ss << "    ";
            ss << item.help;
            ss << '\n';
        }
    }
}

static inline void delimit(stringstream & ss, unsigned n,
    char const * delimiter = " ") {
    if (n) {
        ss << delimiter;
    }
}

string cmdline::get_help() const {
    stringstream ss;
    size_t name_width = std::min(std::max(10UL, get_max_name_width()), 30UL);
    auto prog = get_program_name();
    unsigned n = 0;

    // program name and description
    if (!is_null_or_empty(prog)) {
        ss << prog;
        ++n;
    }
    if (!data->description.empty()) {
        delimit(ss, n, ": ");
        ss << data->description;
        ++n;
    }
    delimit(ss, n, "\n\n");

    // usage
    n = 0;
    if (prog && *prog) {
        ss << prog;
        ++n;
    }
    for (auto & flag: data->flags) {
        if (n) {
            ss << ' ';
        }
        ss << '[' << flag.names[0] << ']';
        ++n;
    }
    for (auto & param: data->params) {
        if (n) {
            ss << ' ';
        }
        ss << get_usage_for_param(ss, param);
        ++n;
    }
    delimit(ss, n, "\n");

    dump_items(ss, data->flags, "flags", name_width);
    dump_items(ss, data->params, "options", name_width);
    if (!data->epilog.empty()) {
        ss << '\n' << data->epilog;
    }
    return ss.str();
}

void cmdline::set_program_name(char const * name) {
    PRECONDITION(data->defining);
    data->program_name = name;
}

char const * cmdline::get_program_name() const {
    return data->program_name.c_str();
}

void cmdline::set_description(char const * descrip) {
    PRECONDITION(data->defining);
    data->description = descrip;
}

char const * cmdline::get_description() const {
    return data->description.c_str();
}

void cmdline::set_epilog(char const * epilog) {
    PRECONDITION(data->defining);
    data->epilog = epilog;
}

char const * cmdline::get_epilog() const {
    return data->epilog.c_str();
}

void cmdline::add_source(cmdline const & source, bool include_positionals) {
    PRECONDITION(data->defining);
    data->sources.push_back(std::make_pair(&source, include_positionals));
}

void cmdline::define_param(char const * names, char const * help,
    char occurrence_rule, char const * placeholder,
    arg_validator_func validator, void * ref_data) {

    PRECONDITION(data->defining);
    PRECONDITION(!is_null_or_empty(names));
    PRECONDITION(!is_null_or_empty(help));
    PRECONDITION(strchr("?*1+23456789", occurrence_rule));
    cmdline_param param;
    param.names = split<string>(names, ", ");
    bool positional = param.is_positional();
    for (unsigned i = 1; i < param.names.size(); ++i) {
        PRECONDITION((param.names[i][0] != '-') == positional);
    }
    param.help = help;
    param.occurrence_rule = occurrence_rule;
    param.placeholder = placeholder ? placeholder : generate_placeholder(param.names);
    param.validator = validator;
    param.reference_data = ref_data;
    data->params.push_back(std::move(param));
}

void cmdline::define_flag(char const * names, char const * help) {
    PRECONDITION(data->defining);
    cmdline_flag flag;
    flag.names = split<string>(names, ", ");
    flag.help = help;
    data->flags.push_back(std::move(flag));
}

vector<cmdline_param> const & cmdline::get_params() const {
    return data->params;
}

vector<cmdline_flag> const & cmdline::get_flags() const {
    return data->flags;
}

}} // end namespace
