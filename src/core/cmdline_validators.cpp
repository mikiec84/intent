#include <algorithm>
#include <cmath>
#include <sstream>

#include <boost/filesystem.hpp>
#include <pcrecpp.h> // "brew install pcre" or "sudo apt-get install libpcre3-dev"

#include "cmdline.h"
#include "cmdline_validators.h"
#include "dbc.h"
#include "interp.h"
#include "scan_numbers.h"
#include "strutil.h"

using std::find;
using std::string;
using std::stringstream;

using boost::filesystem::block_file;
using boost::filesystem::character_file;
using boost::filesystem::directory_file;
using boost::filesystem::fifo_file;
using boost::filesystem::file_not_found;
using boost::filesystem::file_status;
using boost::filesystem::file_type;
using boost::filesystem::path;
using boost::filesystem::regular_file;
using boost::filesystem::socket_file;
using boost::filesystem::status_error;
using boost::filesystem::symlink_file;
using boost::filesystem::read_symlink;

using intent::core::text::interp;

namespace intent {
namespace core {

std::string in_numeric_range(cmdline_param const & param, char const * value, 
    void const * range_info) {

    const char * const MSG = "Error with {1}=\"{2}\": expected a value between "
        "{3=min} and {4=max} (inclusive), formatted as {5}.";
    PRECONDITION(range_info != nullptr);

    numeric_range_info const & nri =
        *reinterpret_cast<numeric_range_info const *>(range_info);
    number_info info;
    auto ptr_to_null_char = strchr(value, 0);
    auto end = scan_number(value, ptr_to_null_char, nri.allowed_formats, info);
    auto valid = (end == ptr_to_null_char);
    if (valid) {
        if (info.format == numeric_formats::floating_point_only) {
            if (!std::isnan(nri.min) && info.floating_point < nri.min) {
                valid = false;
            } else if (!std::isnan(nri.max) && info.floating_point > nri.max) {
                valid = false;
            }
        } else {
            int64_t n = static_cast<int64_t>(info.whole_number);
            if (info.negative) {
                n *= -1;
            }
            valid = n >= nri.min && n <= nri.max;
        }
    }
    return valid ? "" :
        interp(MSG, {param.get_preferred_name(), value, nri.min, nri.max,
            get_names_for_numeric_formats(nri.allowed_formats)});
}

std::string matches_regex(cmdline_param const & param, char const * value,
        void /*precpp::RE*/ const * regex) {

    PRECONDITION(regex != nullptr);
    pcrecpp::RE const & re = *reinterpret_cast<pcrecpp::RE const *>(regex);
    if (!re.FullMatch(value)) {
        auto pat = re.pattern();
        pcrecpp::RE comment_pat("#\\s*(.*?)\n.*");
        string name;
        if (comment_pat.FullMatch(pat, &name)) {
            pat = "<" + name + ">";
        } else {
            pat = "\"" + pat + "\"";
        }
        return interp("Error with {1}=\"{2}\": expected a value matching regex {3}.",
               {param.get_preferred_name(), value, pat});
    }
    return "";
}

inline bool in(std::initializer_list<file_type> types, file_type value) {
    return find(types.begin(), types.end(), value) != types.end();
}

inline void append(stringstream & ss, char const * msg, unsigned & n) {
    if (n++ > 0) {
        ss << ", ";
    }
    ss << msg;
}

string matches_filesys_info(cmdline_param const & param, char const * value,
        void /*filesys_info*/ const * fsinfo_ptr) {

    PRECONDITION(fsinfo_ptr != nullptr);
    filesys_info const & fsinfo = *reinterpret_cast<filesys_info const *>(fsinfo_ptr);

    unsigned problem_count = 0;
    stringstream err;
    path p(value);
    file_status status = boost::filesystem::status(p);
    auto typ = status.type();
    auto exists = !(typ == file_not_found || typ == status_error);
    if (!in(fsinfo.type_must_be_in, typ)) {
        if (!exists) {
            append(err, "doesn't exist or isn't available", problem_count);
        } else {
            bool fnf_ok = in(fsinfo.type_must_be_in, file_not_found);
            bool err_ok = in(fsinfo.type_must_be_in, status_error);
            bool cant_exist = fnf_ok && (fsinfo.type_must_be_in.size() == 1 ||
                                (err_ok && fsinfo.type_must_be_in.size() == 2));
            if (cant_exist) {
                append(err, "already exists", problem_count);
            } else {
                switch(typ) {
                case directory_file:
                    append(err, "is a folder", problem_count); break;
                case symlink_file:
                    append(err, "is a symlink", problem_count); break;
                case file_not_found:
                    append(err, "doesn't exist or isn't available", problem_count); break;
                case status_error:
                    append(err, "unable to stat item", problem_count); break;
                case regular_file:
                    append(err, "is a file", problem_count); break;
                case block_file:
                    append(err, "is a block special file", problem_count); break;
                case character_file:
                    append(err, "is a character special file", problem_count); break;
                case fifo_file:
                    append(err, "is a FIFO/pipe", problem_count); break;
                case socket_file:
                    append(err, "is a socket", problem_count); break;
                default /*type_unknown*/:
                    append(err, "type is unrecognized", problem_count); break;
                }
            }
        }
    }
    if (exists && fsinfo.perms_mask && fsinfo.masked_perms_must_equal) {
        auto perms = status.permissions();
        unsigned x = static_cast<unsigned>(perms) & fsinfo.perms_mask;
        if (x != fsinfo.masked_perms_must_equal) {
            bool need_exe = fsinfo.masked_perms_must_equal & 0111U;
            bool need_write = fsinfo.masked_perms_must_equal & 0222U;
            bool need_read = fsinfo.masked_perms_must_equal & 0444U;
            if ((need_exe & perms) != need_exe) {
                append(err, "not executable", problem_count);
            }
            if ((need_write & perms) != need_write) {
                append(err, "not writable", problem_count);
            }
            if ((need_read & perms) != need_read) {
                append(err, "not readable", problem_count);
            }
        }
    }
    if (typ != directory_file) {
        bool has_min_size = !std::isnan(fsinfo.min_size) > 0 && fsinfo.min_size > 0;
        bool has_max_size = !std::isnan(fsinfo.min_size) > 0 && fsinfo.min_size < std::numeric_limits<double>::max();
        if (exists && (has_min_size || has_max_size)) {
            auto sz = boost::filesystem::file_size(p);
            if (has_min_size && sz < fsinfo.min_size) {
                append(err, "file is too small", problem_count);
            } else if (has_max_size && sz > fsinfo.max_size) {
                append(err, "file is too big", problem_count);
            }
        }
    }
    if (fsinfo.name_must_match) {
        if (!fsinfo.name_must_match->FullMatch(p.c_str())) {
            append(err, "invalid name", problem_count);
        }
    }
    if (problem_count) {
        return interp("Error with {1}=\"{2}\": {3}.", {param.get_preferred_name(), value, err.str()});
    }
    return "";
}


}} // end namespace
