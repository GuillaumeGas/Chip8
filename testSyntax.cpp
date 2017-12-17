#include "Syntax.hpp"

ast::Program Syntax::generateAst()
{
	return visitProgram();
}

ast::Program Syntax::visitProgram()
{
	TokenPtr next;
	std::vector<ast::Instruction*> * vecInst = new std::vector<ast::Instruction*>();
	do
	{
		next = _lex->next();
		if (next->type != TokenType::_EOF_)
			vecInst->push_back(visitInstruction(next));
	} while(next->type != TokenType::_EOF_);
	
	return new ast::Program(vecInst, DefaulPostion);
}

ast::Instruction * visitInstruction(TokenPtr token)
{
	 switch (next->type)
	 {
		case TokenType::JUMP:
			vecInst->push_back(visitJump(next));
			break;
		case TokenType::OTHER:
			vecInst->push_back(visitLabel(next));
		default:
	 }
}

ast::Instruction * visitLabel(TokenPtr token)
{
	if (!_isValidLabel(token->value))
		throw SyntaxErrorException("Invalid label '" + token->value + "'", Position(token->line, token->column));
		
	TokenPtr next = _lex->next();
	
	if (next->type != TokenType::DOUBLEPOINT)
		throw SyntaxErrorException("Expected ':' after '" + token->value + "'", Position(token->line, token->column));
		
	std::map<std::string, ast::Instruction*> * labelsMap = Global::Instance()->getLabelsMap();
	std::map<ast::Instruction*, uint16_t> * instsMap = Global::Instance()->getInstsMap();
	ast::Instruction * nextInstruction = visitInstruction();
	labelsMap[token->value] = nextInstruction;
	instsMap[nextInstruction] = 0;

	return nextInstruction;	
}

ast::Instruction * visitJump(TokenPtr token)
{
	TokenPtr next = _lex->next();
	if (next->type != TokenType::OTHER || !_isValidLabel(next->value))
		throw SyntaxErrorExceptiob("Expected label", Position(token->line, token->column));
	
	Position * pos = new Position(token->line, token->column);
	
	return static_cast<ast::Instruction*> (new ast::Jump(next->value, pos));	
}