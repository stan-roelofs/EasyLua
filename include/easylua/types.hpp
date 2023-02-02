#ifndef __EASYLUA_TYPES_H
#define __EASYLUA_TYPES_H

#include <cstddef>

namespace easylua
{
    struct nil_t
    {
        bool operator==(const nil_t &) const { return true; }
        bool operator!=(const nil_t &) const { return false; }
    };

    static constexpr nil_t nil;
}

#endif