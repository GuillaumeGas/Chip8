#pragma once

#include <iostream>
#include <cstdint>

#include "Exceptions.hpp"
#include "Opcodes.hpp"

#define BUF_SIZE 3584 // chip8 memory size (4096 bytes) - 512 bytes of the first part of the memory

class Disassembly {
public:
	Disassembly(const char * file_name);

	void disass();

private:
	void loadFile(const char * file_name);
	void _disass(const uint16_t opcode, const int offset);

	uint8_t buffer[BUF_SIZE];
};
