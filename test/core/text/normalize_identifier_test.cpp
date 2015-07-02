#include "core/text/normalize_identifier.h"
#include "core/text/str_view.h"
#include "core/util/countof.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

static constexpr char const * const intent_noun_id = "use first dns setting";
#if 0
static constexpr char const * const intent_verb_id = "Use first dns setting";
static constexpr char const * const under_id = "use_first_dns_setting";
static constexpr char const * const camel_id1 = "useFirstDNSSetting";
static constexpr char const * const camel_id2 = "UseFirstDnsSetting";
static constexpr char const * const title_under_id1 = "Use_First_DNS_Setting";
static constexpr char const * const title_under_id2 = "Use_First_Dns_Setting";
static constexpr char const * const alt_camel_id = "useFirstDNSsetting";
static constexpr char const * const alt_title_id = "UseFirstDNSsetting";

static constexpr char const * variants[] = {
    intent_noun_id,
    intent_verb_id,
    under_id,
    camel_id1,
    camel_id2,
    title_under_id1,
    title_under_id2,
    alt_camel_id,
    alt_title_id
};
#endif

static inline void _check_norm(char const * expected, char const * input, identifier_style style, int line) {
    auto actual = normalize_identifier(input, style);
    if (strcmp(actual.c_str(), expected) != 0) {
        ADD_FAILURE_AT(__FILE__, line) << "Expected normalize_identifier(\""
            << input << "\", " << (int)style << ") to produce \"" << expected
            << "\", not \"" << actual << "\".";
    }
}
#define check_norm(a, b, c) _check_norm(a, b, c, __LINE__)


TEST(normalize_identifier_test, into_intent_noun) {
    check_norm(intent_noun_id, intent_noun_id, identifier_style::intent_noun);
}


