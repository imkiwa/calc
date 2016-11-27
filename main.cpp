#include <iostream>

#include "config.h"
#include "expression.h"
#include "function.h"
#include "operator.h"
#include "var.h"
#include "varscope.h"

#define FARG __a
#define FRT __rt
#define FARGS const Args &FARG, int &FRT
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

    F::linkFunction("table", [](FARGS) {
        IFunction *func = nullptr;
        int start = 0;
        int end = 10;
        int step = 1;

        switch (FARG.size()) {
            case 4:
                step = static_cast<int>(FARG[3].as<Real>());
            case 3:
                end = static_cast<int>(FARG[2].as<Real>());
            case 2:
                start = static_cast<int>(FARG[1].as<Real>());
            case 1:
                func = F::getFunction(FARG[0].as<String>());
                break;

            default:
                throw std::runtime_error("Too few arguments");
                FRETURN_VOID;
        }

        if (!func) {
            throw std::runtime_error("Function not found");
        }

        printf("Function %s\n", func->getName().c_str());

        std::vector<Var> args;
        args.push_back(Var(start));
        int resultType;

        for (; start <= end; start += step) {
            args[0] = Var(static_cast<Real>(start));
            printf("    x = %3d, y = %lf\n", start,
                   func->invoke(args, resultType).as<Real>());
        }

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
