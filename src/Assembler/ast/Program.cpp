#include <sstream>

#include "Program.hpp"

using namespace ast;

Program::Program(std::vector<Instruction*> instructions, Position * pos) : Ast(pos), _instructions(instructions) {}

Program::~Program()
{
	for (auto inst : _instructions)
		if (inst != nullptr)
			delete inst;
}

std::string Program::toString() const
{
	std::stringstream ss;
	for (auto inst : _instructions)
		ss << inst->toString() << std::endl;
	return ss.str();
}