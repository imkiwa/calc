#ifndef OPERATOR_H
#define OPERATOR_H

namespace kiva {
    struct Operator
    {
        static int getPriority(int token);
        static bool isOperator(int ch);
        static bool isOperatorToken(int token);
        static bool isNumericalOperatorToken(int token);
        static bool isLogicalOperatorToken(int token);
    };
}

#endif // OPERATOR_H
