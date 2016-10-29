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

            NUM_OPERATOR,
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
            NUM_OPERATOR_END,

            ASSIGN,

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

            bool isIndentifierName(int token, bool first);

        public:
            Tokenizer(const String &src);
            ~Tokenizer();

            String duplicateFromHere() const;
            void skipUntil(int end);
            char peek() const;
            char peekChar() const;
            bool next(Token &token);
            void match(Token &t, int token) throw(std::runtime_error);
        };

    }
}

#endif // TOKENIZER_H
