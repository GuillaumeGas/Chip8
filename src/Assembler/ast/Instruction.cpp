#include <map>
#include <iostream>

#include "../global/Global.hpp"
#include "Instruction.hpp"

using namespace ast;

Instruction::Instruction(uint16_t size, Position * pos) : Ast(pos), size(size) {}

Jump::Jump(std::string label, Position * pos) : Instruction(INSTRUCTION_SIZE + sizeof(uint16_t), pos), label(label) {}

std::string Jump::toString() const
{
	return "jump " + label;
}

uint16_t Jump::firstPass(uint16_t nextOffset)
{
	std::map<Instruction*, uint16_t> * instAddrMap = Global::Instance()->getInstsMap();
	if (instAddrMap->find(this) != instAddrMap->end())
	{
		(*instAddrMap)[this] = nextOffset;
	}
	return nextOffset + this->size;
}

void Jump::secondPass(std::ostream & stream)
{
	std::map<Instruction*, uint16_t> * instAddrMap = Global::Instance()->getInstsMap();
	std::map<std::string, Instruction*> * labelsMap = Global::Instance()->getLabelsMap();
	uint16_t addr = (*instAddrMap)[(*labelsMap)[this->label]];
	uint16_t value = 0x1000 | addr;
	stream << value;
}