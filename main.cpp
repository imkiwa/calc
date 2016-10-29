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

    Var v = evalDirectly("a=111*111; b = 999 - 111; a * b");
    if (v.isValid()) {
        printf("%lf\n", v.as<Real>());
    }
    return 0;
}
