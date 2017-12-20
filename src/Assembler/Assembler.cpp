#include <iostream>
#include <string>
#include <fstream>
#include <ios>

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
        lex->setKeys({ " ", ":", "\n", "\r", "\t" });
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

		std::string resFile = (resFilePath == nullptr) ? DEFAULT_RES_FILE_NAME : resFilePath;
		std::ofstream stream;
		stream.open(resFile, std::ios::out | std::ios::binary);

		programAst->assemble(stream);

		stream.close();
	}
	catch (const SyntaxException & e)
	{
		std::cout << e.toString() << std::endl;
		exit(-1);
	}
}