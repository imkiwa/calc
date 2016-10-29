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
                // 防止解析表达式时缺少括号导致结果不对
                case '(':
                    return 1;
                case XOR:
                case AND:
                case OR:
                    return 2;
                case LSHF:
                case RSHF:
                    return 3;
                case ADD:
                case SUB:
                    return 4;
                case MUL:
                case DIV:
                case MOD:
                    return 5;
                case NAV:
                    return 6;
            }
            return 0;
        }

        static bool isOperator(int ch) {
            static const char *operators = "+-*/^&|<>";
            return strchr(operators, ch) != nullptr;
        }

        static bool isOperatorToken(int token)
        {
            return token > parser::TokenType::NUM_OPERATOR
                    && token < parser::TokenType::NUM_OPERATOR_END;
        }
    };
}

#endif // OPERATOR_H
