#include <iostream>

#include "config.h"
#include "expression.h"
#include "function.h"
#include "operator.h"
#include "var.h"
#include "varscope.h"

#define FARGS const Args &__a, int &__rt
#define FARG __a
#define FRT __rt
#define FRETURN_VOID                         \
    {                                        \
        FRT = kiva::expression::RESULT_NONE; \
        return Var();                        \
    }

using namespace std;
using namespace kiva::function;
using namespace kiva::var;
using namespace kiva::expression;

int main(int argc, char *argv[])
{
    String s;
    Var v;
    int resultType;

    bool running = true;
    int exitValue = 0;

    using F = FunctionTable;
    F::linkFunction("exit", [&](FARGS) {
        running = false;
        exitValue = (!FARG.empty() && FARG[0].isValid())
                        ? static_cast<int>(FARG[0].as<Real>())
                        : 0;

        FRETURN_VOID;
    });

    F::linkFunction("__android", [](FARGS) {
        printf("hello world\n");
        FRETURN_VOID;
    });

    do {
        printf("> ");
        std::getline(std::cin, s);
        try {
            v = evalDirectly(s, resultType);
            switch (resultType) {
                case RESULT_BOOL:
                    printf("@: %s\n",
                           v.as<Real>() == 0 ? "false" : "true");
                    break;
                case RESULT_NUMBER:
                    printf("@: %.2lf\n", v.as<Real>());
                    break;
                case RESULT_STRING:
                    printf("@: %s\n", v.as<String>().c_str());
                    break;
            }

        } catch (std::exception &e) {
            printf("!: %s\n", e.what());
        }
    } while (running && std::cin.good());

    return exitValue;
}
