#include <sstream>

#include "Program.hpp"

using namespace ast;

Program::Program(std::vector<Instruction*> * instructions, Position * pos) : Ast(pos), _instructions(instructions) {}

Program::~Program()
{
	for (auto inst : *_instructions)
		if (inst != nullptr)
			delete inst;
	if (_instructions != nullptr)
		delete _instructions;
}

std::string Program::toString() const
{
	std::stringstream ss;
	for (auto inst : *_instructions)
		ss << inst->toString() << std::endl;
	return ss.str();
}

void Program::assemble(std::ostream & stream)
{
	for (auto inst : *_instructions)
		inst->firstPass(0);

	for (auto inst : *_instructions)
		inst->secondPass(stream);
}