#pragma once

#include <iostream>
#include <memory>

#include "TokenList.hpp"

enum TokenType {
#define LEX_TOKEN(name, _) name,
#define LEX_TOKEN_KEYWORD(x, y) LEX_TOKEN(x, y)
    LEX_TOKEN_LIST
#undef LEX_TOKEN
#undef LEX_TOKEN_KEYWORD
};

struct location_t {
    unsigned int column;
    unsigned int line;
};

class Token;
typedef std::shared_ptr<Token> TokenPtr;

class Token {
public:
    Token(TokenType type, const std::string & value, location_t loc);

    static TokenPtr make(const std::string & value, location_t loc);
    static TokenPtr makeEof(location_t loc);

    static TokenType getFromStr(const std::string & str) {
#define LEX_TOKEN(name, value)			\
	    if (str == value) return name;
#define LEX_TOKEN_KEYWORD(x, y) LEX_TOKEN(x, y)
        LEX_TOKEN_LIST
#undef LEX_TOKEN_KEYWORD
#undef LEX_TOKEN
            return OTHER;
    }

    static const char * getType(TokenType ttype) {
        switch (ttype) {
#define LEX_TOKEN(name, value)			\
		case name:			\
		    return #name;
#define LEX_TOKEN_KEYWORD(x, y) LEX_TOKEN(x, y)
            LEX_TOKEN_LIST
#undef LEX_TOKEN_KEYWORD
#undef LEX_TOKEN
        default:
            return NULL;
        }
    }

    static const char * getValue(TokenType ttype) {
        switch (ttype) {
#define LEX_TOKEN(name, value)			\
		case name:			\
		    return value;
#define LEX_TOKEN_KEYWORD(x, y) LEX_TOKEN(x, y)
            LEX_TOKEN_LIST
#undef LEX_TOKEN_KEYWORD
#undef LEX_TOKEN
        default:
            return NULL;
        }
    }

    std::string toString() const;
    location_t getLocation() const;
    bool isEof() const;

    TokenType type;
    std::string value;
    unsigned int line;
    unsigned int column;
    bool eof;
};

class TokenEof : public Token {
public:
    TokenEof();
};
