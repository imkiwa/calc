#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdexcept>
#include <exception>

#include "config.h"
#include "var.h"

namespace kiva {
    namespace expression {
        using namespace kiva::var;

        Var evalDirectly(const String &str) throw(std::runtime_error);
    }
}

#endif // EXPRESSION_H
