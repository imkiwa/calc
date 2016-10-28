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

    Var v = evalDirectly("((3+5*2)+3)/5+(-6)/4*2+3");
    cout << v.as<Real>() << endl;
    return 0;
}
