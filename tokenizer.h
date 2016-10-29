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
            STRING,

            KEYWORD,
            VAR,      /* var */
            FUNCTION, /* function */
            RETURN,   /* return */
            NATIVE,   /* native */
            KEYWORD_END,

            NUM_OPERATOR,
            ADD,  /* + */
            SUB,  /* - */
            NAV,  /* - (负号) */
            MUL,  /* * */
            DIV,  /* / */
            MOD,  /* % */
            XOR,  /* ^ */
            AND,  /* & */
            OR,   /* | */
            LSHF, /* << */
            RSHF, /* >> */
            NUM_OPERATOR_END,

            LOGICAL_OPERATOR,
            LAND, /* && */
            LOR,  /* || */
            LE,   /* <= */
            LT,   /* < */
            GE,   /* >= */
            GT,   /* > */
            EQ,   /* == */
            NEQ,  /* != */
            NOT,  /* ! */
            LOGICAL_OPERATOR_END,

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

            const char* currentPosition() const;
            String duplicateFromHere() const;
            void skipUntil(int end);
            char peek() const;
            char peekChar() const;
            bool next(Token &token, bool peek = false);
            void match(Token &t, int token) throw(std::runtime_error);
        };

    }
}

#endif // TOKENIZER_H
