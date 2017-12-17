#pragma once

#include <string>

#include "Ast.hpp"
#include "../global/Position.hpp"

namespace ast
{
	class Instruction : public Ast
	{
	public:
		Instruction(Position * pos);

		virtual std::string toString() const = 0;
	};
}