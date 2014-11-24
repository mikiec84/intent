#include "core/text/line_iterator.h"
#include "core/text/strutil.h"

namespace intent {
namespace core {
namespace text {

line_iterator::line_iterator(char const * begin, char const * _end) {
    if (begin) {
        end = (_end == nullptr) ? strchr(begin, 0) : _end;
        line.begin = begin;
        char const * p = find_any_char(begin, "\r\n", end);
        if (p != end) {
            line.end = p;
        } else {
            line.end = end;
        }
    } else {
        this->end = _end;
    }
}

line_iterator & line_iterator::operator++() {
    char const * p = line.end;
    if (p + 1 <= end) {
        if (p[0] == '\r') {
            if (p + 1 < end && p[1] == '\n') {
                p += 2;
            } else {
                ++p;
            }
        } else {
            ++p;
        }
        line.begin = p;
        line.end = find_any_char(p, "\r\n", end);
        return *this;
    }
    line.begin = line.end = nullptr;
    return *this;
}

}}} // end namespace
