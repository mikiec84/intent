#ifndef intent_core_scope_guard_h
#define intent_core_scope_guard_h

#include "core/util/advanced_macros.h"

// This code was written a couple of times from scratch, but was not working
// quite right. Eventually I found and studied the implementation of scope_guard
// at https://github.com/lichray/deferxx/blob/master/defer.h, and figured out
// that I was missing a move ctor on scope_guard, and a call to std::forward
// in the arg to scope_guard's ctor in make_scope_guard. It's not the case that
// I copied Zhihao Yuan's code, but I owe him thanks for helping me figure out
// the correct algorithm.

#include <utility>

namespace intent {
namespace core {
namespace util {

template <typename FUNC>
struct scope_guard {
    explicit scope_guard(FUNC && _on_exit) : enabled(true),
        on_exit(std::move(_on_exit)) {}
    scope_guard(scope_guard const &) = delete;
    scope_guard & operator =(scope_guard const &) = delete;
    scope_guard(scope_guard && rhs) : enabled(rhs.enabled),
        on_exit(std::move(rhs.on_exit)) {}
    ~scope_guard() noexcept { if (enabled) on_exit(); }
    void dismiss() { enabled = false; }
    bool enabled;
    FUNC on_exit;
};

template <typename FUNC>
auto make_scope_guard(FUNC && f) -> scope_guard<FUNC> {
    return scope_guard<FUNC>(std::forward<FUNC>(f));
}

}}} // end namespace

#define SCOPEGUARD_NAME_CONCAT(a, b) a ## b
#define MAKE_SCOPEGUARD_NAME(ln) SCOPEGUARD_NAME_CONCAT(_scope_guard_on_line_, ln)
#define on_scope_exit(...) \
    auto MAKE_SCOPEGUARD_NAME(__LINE__) = \
        intent::core::util::make_scope_guard([&]{__VA_ARGS__;})
#define named_scope_exit(name, ...) \
    auto name = intent::core::util::make_scope_guard([&]{__VA_ARGS__;})

#endif // sentry
