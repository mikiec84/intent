#include <algorithm>
#include <cstring>

#include "lang/issue.h"

namespace intent {
namespace lang {

struct issue_info {
    issue_id id;
    char const * name;
    char const * msg;
};

issue_info const issue_infos[] = {

    #define TUPLE( number, name, msg ) \
        { ii_##name, "ii_" #name, msg },
    #include "lang/issue_tuples.h"
};

const size_t issue_infos_count = sizeof(issue_infos) / sizeof(issue_info);
issue_info const * const issue_infos_begin = &issue_infos[0];
issue_info const * const issue_infos_end = &issue_infos[issue_infos_count];

size_t get_issue_count() {
    return issue_infos_count;
}

char const * get_issue_name(issue_id);
char const * get_issue_msg(issue_id);

issue_id get_issue_id_by_index(size_t i) {
    if (i < issue_infos_count) {
        return issue_infos[i].id;
    }
    return ii_none;
}

issue_id get_issue_id_from_name(char const * name) {
    if (name && *name) {
        for (size_t i = 0; i < issue_infos_count; ++i) {
            if (strcmp(issue_infos[i].name, name) == 0) {
                return issue_infos[i].id;
            }
        }
    }
    return ii_none;
}

inline bool compare_by_id(issue_info const & ii, issue_id id) {
    return ii.id < id;
}

inline issue_info const * find_issue(issue_id id) {
    return std::lower_bound(issue_infos_begin, issue_infos_end, id, compare_by_id);
}

char const * get_issue_name(issue_id id) {
    auto info = find_issue(id);
    return (info != issue_infos_end) ? info->name : nullptr;
}

char const * get_issue_msg(issue_id id) {
    auto info = find_issue(id);
    return (info != issue_infos_end) ? info->msg : nullptr;
}

} // end namespace lang
} // end namespace intent
