#ifndef OPERATOR_H
#define OPERATOR_H

#include <string.h>
#include "tokenizer.h"

namespace kiva {
    struct Operator
    {
        static int getPriority(int token) {
            using namespace kiva::parser;
            switch (token) {
                case '(':
                    return 1;
                case ADD:
                case SUB:
                    return 2;
                case MUL:
                case DIV:
                case MOD:
                    return 3;
                case POW:
                    return 4;
                case NAV:
                    return 5;
            }
            return 0;
        }

        static int isOperator(int ch) {
            static const char *operators = "+-*/^";
            return strchr(operators, ch) != nullptr;
        }
    };
}

#endif // OPERATOR_H
