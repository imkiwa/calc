#ifndef KEYWORD_H
#define KEYWORD_H

#include "config.h"

namespace kiva {
    struct Keyword {
        static int constantify(const String &token);
    };
}

#endif // KEYWORD_H
