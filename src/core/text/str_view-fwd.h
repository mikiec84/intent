#ifndef intent_core_str_view_fwd_h
#define intent_core_str_view_fwd_h

namespace intent {
namespace core {
namespace text {

template<typename C>
struct strview;

typedef strview<char> str_view;
typedef strview<wchar_t> wstr_view;

}}} // end namespace

#endif
