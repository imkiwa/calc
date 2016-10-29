#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdexcept>
#include <exception>

#include "config.h"
#include "var.h"

namespace kiva {
    namespace expression {
        using namespace kiva::var;

        enum ResultType
        {
            RESULT_NONE, RESULT_NIL, RESULT_BOOL, RESULT_NUMBER, RESULT_STRING,
        };

        Var evalDirectly(const String &str) throw(std::runtime_error);
        Var evalDirectly(const String &str, int &resultType) throw(std::runtime_error);
    }
}

#endif // EXPRESSION_H
