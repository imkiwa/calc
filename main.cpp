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

    cout << evalDirectly("(0b110 & 0b010) * 6").as<Real>()<< endl;
    return 0;
}
