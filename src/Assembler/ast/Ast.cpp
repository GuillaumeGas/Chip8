#include "Ast.hpp"

using namespace ast;

Ast::Ast(Position * pos) : _pos(pos) {}

Ast::~Ast()
{
	if (_pos != nullptr)
		delete _pos;
}

Position * Ast::getPosition() const
{
	return _pos;
}