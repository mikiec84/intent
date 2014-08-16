#ifndef _pstr_h_70f7ab8f82804dbd841d7599d5e84008
#define _pstr_h_70f7ab8f82804dbd841d7599d5e84008

#include <cstdlib>

namespace intent {
namespace core {

/**
 * A length-prefixed string.
 */
class pstr {
public:
    pstr(char const * cstr);
    pstr(char const * cstr, char const * end);
    pstr(char const * cstr, size_t len);
    pstr(pstr const & rhs);
    pstr(pstr const & rhs, size_t len);

    size_t size() const;

private:
    char const * txt;
    char const * end;
};

}} // end namespace

#endif // sentry
