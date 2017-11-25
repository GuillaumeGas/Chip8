#pragma once

#include <iostream>
#include <list>
#include <sstream>

#include "Cpu.hpp"
#include "Opcodes.hpp"
#include "Screen.hpp"

class Chip8;
class CpuDebug : public Cpu {
public:
	CpuDebug(Chip8 * ctrl, Screen * sc);

	virtual bool emulateCycle();

private:
	void memoryDump() const;
	void debugInst(uint16_t opcode, bool hasBreak);

	bool stepMode;
	std::list<uint16_t> bpList;
};
