#include "keyword.h"
#include "tokenizer.h"
#include <string.h>

namespace kiva {
    int Keyword::constantify(const String &token)
    {
        using namespace kiva::parser;
        if (token == "function") {
            return FUNCTION;
        } else if (token == "return") {
            return RETURN;
        } else if (token == "var") {
            return VAR;
        }
        return -1;
    }
}
