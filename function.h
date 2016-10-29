#ifndef FUNCTION_H
#define FUNCTION_H

#include "config.h"
#include "var.h"
#include <vector>

namespace kiva {
    namespace function {
        using namespace var;

        class IFunction
        {
        public:
            IFunction() = default;
            virtual ~IFunction() = default;

            virtual const String& getName() const = 0;
            virtual Var invoke(const std::vector<Var> &vars) = 0;
        };
    }
}

#endif // FUNCTION_H
