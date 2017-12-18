#pragma once

#include <string>

#include "Ast.hpp"
#include "../global/Position.hpp"

#define INSTRUCTION_SIZE 2

namespace ast
{
	class Instruction : public Ast
	{
	public:
		Instruction(uint16_t size, Position * pos);

		virtual std::string toString() const = 0;
		virtual uint16_t firstPass(uint16_t nextOffset) = 0;
		virtual void secondPass(std::ostream & stream) = 0;

		uint16_t size;
	};

	class Jump : public Instruction
	{
	public:
		Jump(std::string label, Position * pos);

		std::string toString() const override;
		uint16_t firstPass(uint16_t nextOffset) override;
		void secondPass(std::ostream & stream) override;

		std::string label;
	};
}