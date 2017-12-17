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
		virtual uint16_t firstPass(uint32_t nextOffset) = 0;
		virtual void secondPass(std::istream & stream) = 0;
		
		uint16_t size;
	};
	
	// C++
	Instruction::Instruction(uint16_t size, Position * pos) : Ast(pos), size(size) {}
	
	class Jump : public Instruction
	{
	public:
		Jump(std::string label, Position * pos);
		
		std::string toString() const override;
		uint16_t firstPass(uint16_t nextOffset);
		void secondPass(std::istream & stream);
			
		std::string label;	
	}
	
	// C++
	Jump::Jump(std::string label, Position * pos) : Instruction(INSTRUCTION_SIZE + sizeof(uint16_t), pos), label(label) {}
	
	std::string Jump::toString() const 
	{
		return "jump " + label;
	}
	
	void Jump::firstPass(uint32_t nextOffset)
	{
		std::map<Instruction*, uint32_t> * labels = Global::Instance()->getLabelsMap();
		if (labels->find(this) != labels->end())
		{
			labels[this] = nextOffset;
		}
		return nextOffset + this->size;	
	}
	
	void Jump::secondPath(std::istream & stream)
	{
		std::map<Instruction*, uint_32> * instAddrMap = Global::Instance()->getInstAddrList();
		std::map<std::string, Instruction*> * labelsMap = Global::Instance()->getLabelsMap();		
		uint16_t addr = instAddrMap[labelsMap[this->label]];
		uint16_t value = 0x1000 | addr;
		istream << value;
	}
	
}
