#pragma once

#include <string>

#include "../global/Position.hpp"

namespace ast
{
	class Ast
	{
	public:
		Ast(Position * pos);
		~Ast();

		virtual std::string toString() const = 0;
		Position * getPosition() const;

	private:
		Position * _pos;
	};
}