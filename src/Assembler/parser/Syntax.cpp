#include "Syntax.hpp"
#include "../global/Position.hpp"

Syntax::Syntax(Lexer * lex) : _lex(lex) {}

ast::Program * Syntax::generateAst()
{
	return visitProgram();
}

TokenPtr Syntax::pop() const
{
	return _lex->next();
}

void Syntax::rewind(int count)
{
	_lex->rewind(count);
}

ast::Program * Syntax::visitProgram()
{
	std::vector<ast::Instruction*> * vecInst = new std::vector<ast::Instruction*>();
	ast::Instruction * next = nullptr;
	do
	{
		if ((next = visitInstruction()) != nullptr)
			vecInst->push_back(next);
	} while (next != nullptr);

	return new ast::Program(vecInst, new DefaultPosition());
}

ast::Instruction * Syntax::visitInstruction()
{
	TokenPtr next = pop();
	switch (next->type)
	{
	case TokenType::JUMP:
		return visitJump(next);
	case TokenType::OTHER:
		return visitLabel(next);
	default:
		return nullptr;
	}
}

ast::Instruction * Syntax::visitLabel(TokenPtr token)
{
	if (!isValidLabel(token->value))
		throw SyntaxErrorException("Invalid label '" + token->value + "'", Position(token->line, token->column));

	TokenPtr next = _lex->next();

	if (next->type != TokenType::COLON)
		throw SyntaxErrorException("Expected ':' after '" + token->value + "'", Position(token->line, token->column));

	std::map<std::string, ast::Instruction*> * labelsMap = Global::Instance()->getLabelsMap();
	std::map<ast::Instruction*, uint16_t> * instsMap = Global::Instance()->getInstsMap();
	ast::Instruction * nextInstruction = visitInstruction();
	(*labelsMap)[token->value] = nextInstruction;
	(*instsMap)[nextInstruction] = 0;

	return nextInstruction;
}

ast::Instruction * Syntax::visitJump(TokenPtr token)
{
	TokenPtr next = _lex->next();
	if (next->type != TokenType::OTHER || !isValidLabel(next->value))
		throw SyntaxErrorException("Expected label", Position(token->line, token->column));

	Position * pos = new Position(token->line, token->column);

	return static_cast<ast::Instruction*> (new ast::Jump(next->value, pos));
}

bool Syntax::isValidLabel(std::string str) const
{
	if (str.length() <= 0)
		return false;

	if (str[0] < 'A' || (str[0] > 'Z' && str[0] < 'a' && str[0] != '_') || (str[0] > 'z'))
		return false;

	if (str[0] < '0' || (str[0] < 'A' && str[0] > '9') || (str[0] > 'Z' && str[0] < 'a' && str[0] != '_') || (str[0] > 'z'))
		return false;

	return true;
}