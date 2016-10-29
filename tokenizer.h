#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "config.h"
#include <stdexcept>
#include <exception>

namespace kiva {
    namespace parser {
        enum TokenType
        {
            ID = 300,
            NUMBER,

            OPERATOR,
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            XOR,
            AND,
            OR,
            LSHF,
            RSHF,
            NAV,
            OPERATOR_END,

            TOKEN_COUNT
        };

        struct Token
        {
            String strval;
            Real numval;
            int token;
        };

        class Tokenizer
        {
        private:
            String originalSource;
            const char *src;
            const char *orig;

            void skipUntil(int end);
            bool isIndentifierName(int token, bool first);

        public:
            Tokenizer(const String &src);
            ~Tokenizer();

            bool next(Token &token);
            void match(Token &t, int token) throw(std::runtime_error);
        };

    }
}

#endif // TOKENIZER_H
