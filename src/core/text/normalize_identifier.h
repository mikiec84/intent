#ifndef _a3973659d55d4e97bd72268703d0a7eb
#define _a3973659d55d4e97bd72268703d0a7eb

#include <cstdint>
#include <string>

namespace intent {
namespace core {
namespace text {

enum class identifier_style: uint8_t {
    /** space-separated names: "use first dns setting" */
    intent,
    /** space-separated names with first word capitalized: "Use first dns setting" */
    intent_verb,
    /** underscore-separated names: "use_first_dns_setting" */
    under,
    /** camelCase names: "useFirstDNSSetting" or "useFirstDnsSetting" */
    camel,
    /** TitleCase names: "UseFirstDNSSetting" or "UseFirstDnsSetting" */
    title,
    /** Like camelCase, but acronyms end with alternate case: useFirstDNSsetting */
    alt_camel,
    /** Like TitleCase, but acronyms end with alternate case: UseFirstDNSsetting */
    alt_title,
};

std::string normalize_identifier(str_view const & in, identifier_style in_style, identifier_style out_style=intent);

}}} // end namespace

#endif // sentry

