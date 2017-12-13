#pragma once

#include <iostream>

#include "exceptions/SyntaxException.hpp"
#include "../ast/Program.hpp"

#define TODO(p)					\
    printf ("[!] Unimplement token : %s\n", p);	\
    exit (-1);

class Syntax {
public:
	Syntax(Lexer * lex);

	ast::Program * generateAst();

	TokenPtr pop() const;
	void rewind(int count = 1);

	/* Visitor */
	void visitProgram();

	bool find(TokenType type, std::vector <TokenType> list);

private:
	Lexer * _lex;
};
