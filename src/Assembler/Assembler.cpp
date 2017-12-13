#include <iostream>
#include <string>

#include "Assembler.hpp"
#include "parser/Lexer.hpp"
#include "parser/Syntax.hpp"
#include "parser/exceptions/LexerException.hpp"
#include "parser/exceptions/SyntaxException.hpp"
#include "ast/Program.hpp"
#include "../Utils/ScopeGuard.hpp"

void Assembler::assemble(const char * filePath, const char * resFilePath)
{
	Lexer * lex = nullptr;
	Syntax * syntax = nullptr;

	OnExitingScope(([lex, syntax]() {
		if (lex != nullptr)
			delete lex;
		if (syntax != nullptr)
			delete syntax;
	}));

    try
    {
        lex = new Lexer(std::string(filePath));
        lex->setKeys({ " ", "\n", "\r", "\t" });
        lex->setSkips({ " ", "\n", "\r", "\t" });
        lex->setComs({ std::make_pair(";", "\n") });
    }
    catch (const LexerException & e)
    {
        std::cout << e.toString() << std::endl;
        exit(-1);
    }

	try 
	{
		syntax = new Syntax(lex);
		ast::Program * programAst = syntax->generateAst();
		std::cout << programAst->toString() << std::endl;
	}
	catch (const SyntaxException & e)
	{
		std::cout << e.toString() << std::endl;
		exit(-1);
	}
}