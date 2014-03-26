#ifndef issue_h_4c59d3e33c734c299b7d6a1bc2d01cc6
#define issue_h_4c59d3e33c734c299b7d6a1bc2d01cc6

namespace intent {
namespace lang {

/**
 * Uniquely identify a particular issue.
 */
enum issue_id {
    ii_none = 0,
    #define TUPLE(number, name, msg) \
        ii_##name = number,
    #include "lang/issue_tuples.h"
};

size_t get_issue_count();
issue_id get_issue_id_by_index(size_t i);
issue_id get_issue_id_from_name(char const * name);
char const * get_issue_name(issue_id);
char const * get_issue_msg(issue_id);

} // end namespace lang
} // end namespace intent

#include "lang/token_type-inline.h"

#endif // sentry
