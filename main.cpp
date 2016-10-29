#include <iostream>

#include "operator.h"
#include "config.h"
#include "var.h"
#include "varscope.h"
#include "expression.h"

using namespace std;

int main(int argc, char *argv[])
{
    using namespace kiva::var;
    using namespace kiva::expression;

    String s;
    Var v;
    int resultType;

    do {
        printf("> ");
        std::getline(std::cin, s);
        try {
            v = evalDirectly(s, resultType);
            if (resultType != RESULT_NIL && v.isValid()) {
                switch (resultType) {
                    case RESULT_BOOL:
                        printf("@: %s\n", v.as<Real>() == 0 ? "false" : "true");
                        break;
                    case RESULT_NUMBER:
                        printf("@: %lf\n", v.as<Real>());
                        break;
                    case RESULT_STRING:
                        printf("@: %s\n", v.as<String>().c_str());
                        break;
                }
            }

        } catch (std::exception &e) {
            printf("!: %s\n", e.what());
        }
    } while (std::cin.good());

    return 0;
}
