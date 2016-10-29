#include "operator.h"

#include <string.h>
#include "tokenizer.h"

namespace kiva {
        int Operator::getPriority(int token) {
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

        bool Operator::isOperator(int ch) {
            const char *operators = "+-*/^&|<>";
            return strchr(operators, ch) != nullptr;
        }

        bool Operator::isOperatorToken(int token)
        {
            return Operator::isNumericalOperatorToken(token) || isLogicalOperatorToken(token);
        }

        bool Operator::isNumericalOperatorToken(int token)
        {
            return token > parser::TokenType::NUM_OPERATOR
                                && token < parser::TokenType::NUM_OPERATOR_END;
        }

        bool Operator::isLogicalOperatorToken(int token)
        {
            return token > parser::TokenType::LOGICAL_OPERATOR
                                && token < parser::TokenType::LOGICAL_OPERATOR_END;
        }
}

