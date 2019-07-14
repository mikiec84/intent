#ifndef _d0f4cc6e979b45668daad20e93705f39
#define _d0f4cc6e979b45668daad20e93705f39

#include <cstdint>
#include <vector>

#include "core/util/advanced_macros.h"

struct timed_experiment {
    static std::vector<timed_experiment *> & all();

    virtual ~timed_experiment() {}

    virtual char const * get_suite() const = 0;
    virtual char const * get_name() const = 0;
    void run();
    virtual void run_trial() = 0;
};

bool reg_ex(timed_experiment * ex);

#define time_this(suite, name, code) \
    struct te_##suite##_##name: public timed_experiment { \
        virtual char const * get_suite() const { return #suite; } \
        virtual char const * get_name() const { return #name; } \
        virtual void run_trial() code \
    /* Next line must all be together to get consistent value of __LINE__... */ \
    } glue_token_values(__ex, __LINE__); bool glue_token_values(__regd, __LINE__) = reg_ex(&glue_token_values(__ex, __LINE__));

#endif // sentry
