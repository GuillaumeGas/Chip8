#pragma once

#include <string>
#include <vector>

#include "Ast.hpp"
#include "Instruction.hpp"
#include "../global/Position.hpp"

namespace ast
{
	class Program : public Ast
	{
	public:
		Program(std::vector<Instruction*> * instructions, Position * pos);
		~Program();

		std::string toString() const override;
		void Program::assemble(std::ostream & stream);

	private:
		std::vector<Instruction*> * _instructions;
	};
}