#pragma once

#include <iostream>

#include "Lexer.hpp"
#include "exceptions/SyntaxException.hpp"
#include "../ast/Program.hpp"
#include "../ast/Instruction.hpp"
#include "Token.hpp"

#define TODO(p)					\
    printf ("[!] Unimplement token : %s\n", p);	\
    exit (-1);

class Syntax {
public:
	Syntax(Lexer * lex);

	ast::Program * generateAst();
	ast::Program * visitProgram();
	ast::Instruction * visitInstruction();
	ast::Instruction * visitLabel(TokenPtr token);
	ast::Instruction * visitJump(TokenPtr token);
	inline bool isValidLabel(std::string str) const;

	TokenPtr pop() const;
	void rewind(int count = 1);

private:
	Lexer * _lex;
};
