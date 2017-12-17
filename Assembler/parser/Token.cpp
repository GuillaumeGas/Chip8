#include "Token.hpp"
#include <string>

using namespace std;

Token::Token(TokenType type, const string & value, location_t loc) :
    type(type),
    value(value),
    line(loc.line),
    column(loc.column),
    eof(false) {}

TokenPtr Token::make(const std::string & value, location_t loc) {
    TokenType ttype = Token::getFromStr(value);
    return TokenPtr(new Token(ttype, value, loc));
}

TokenPtr Token::makeEof(location_t loc) {
    Token * t = new Token(_EOF_, "EOF", loc);
    t->eof = true;
    return TokenPtr(t);
}

string Token::toString() const {
    string type = Token::getType(this->type);
    return "<" + type + "(" + value + "), loc(" + std::to_string(line) + ", " + std::to_string(column) + ")>";
}

bool Token::isEof() const {
    return this->eof;
}

location_t Token::getLocation() const {
    return location_t({ line, column });
}
