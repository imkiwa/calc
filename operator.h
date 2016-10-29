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
                case LOR:
                    return 2;
                case LAND:
                    return 3;
                case OR:
                    return 4;
                case XOR:
                    return 5;
                case AND:
                    return 6;
                case EQ:
                case NEQ:
                    return 7;
                case LE:
                case LT:
                case GE:
                case GT:
                    return 8;
                case LSHF:
                case RSHF:
                    return 9;
                case ADD:
                case SUB:
                    return 10;
                case MUL:
                case DIV:
                case MOD:
                    return 11;
                case NAV:
                    return 32;
            }
            return 0;
        }

        static bool isOperator(int ch) {
            static const char *operators = "+-*/^&|<>";
            return strchr(operators, ch) != nullptr;
        }

        static bool isOperatorToken(int token)
        {
            return isNumericalOperatorToken(token) || isLogicalOperatorToken(token);
        }

        static bool isNumericalOperatorToken(int token)
        {
            return token > parser::TokenType::NUM_OPERATOR
                                && token < parser::TokenType::NUM_OPERATOR_END;
        }

        static bool isLogicalOperatorToken(int token)
        {
            return token > parser::TokenType::LOGICAL_OPERATOR
                                && token < parser::TokenType::LOGICAL_OPERATOR_END;
        }
    };
}

#endif // OPERATOR_H
