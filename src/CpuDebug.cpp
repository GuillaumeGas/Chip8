#include "CpuDebug.hpp"
#include "Chip8.hpp"

using namespace std;

CpuDebug::CpuDebug(Chip8 * ctrl, Screen * sc) : Cpu(ctrl, sc) {
	cout << "> Debug mode enabled !" << endl;

	this->stepMode = true;
}

bool CpuDebug::emulateCycle() {
	if (this->pc >= (MEM_SIZE - 1) || !this->running)
		return false;

	uint16_t opcode = getNextOpCode();
	bool hasBp = false; //hasBreakPoint (this->pc);
	if (this->stepMode || hasBp)
		debugInst(opcode, hasBp);
	execOpcode(opcode);
	this->pc += 2;
	count();
	return true;
}

void CpuDebug::memoryDump() const {
	cout << "> Memory dump" << endl;
	cout << "** Register **" << endl;
	cout << "PC : " << this->pc << endl;
	cout << "I : " << this->I << endl;
	cout << "SP : " << (int) this->sp << endl;
	cout << "Delay counter : " << (int) this->delay_timer << endl;
	cout << "Sound counter : " << (int) this->sound_timer << endl;
	printf("Current instruction : %#04x\n", this->memory[this->pc]);
	for (int i = 0; i < V_REGISTERS_SIZE; i++)
		cout << "V" << i << " = " << (int) this->reg[i] << endl;
}

void CpuDebug::debugInst(uint16_t opcode, bool hasBp) {
	auto opcodes = Opcodes::instance()->getList();
	Opcode * op = (*opcodes)[GetOpcodeId(opcode)];

	if (hasBp)
		cout << "Break on " << this->pc << endl;
	cout << "[" << this->pc << "] " << op->disassemble(opcode) << endl;

	string cmd = "";
	bool continueExecution = false;
	while (!continueExecution) {
		cout << " >> ";
		cin >> cmd;
		if (cmd == "r") {
			memoryDump();
		}
		else if (cmd == "p") {
			continueExecution = true;
		}
		else if (cmd == "c") {
			cout << "> Continuing..." << endl;
			this->stepMode = false;
			continueExecution = true;
		}
		else if (cmd == "q") {
			this->running = false;
			continueExecution = true;
		}
		else {
			istringstream ss(cmd);
		}
	}
}
