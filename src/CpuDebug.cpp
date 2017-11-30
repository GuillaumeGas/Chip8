#include "CpuDebug.hpp"
#include "Chip8.hpp"

using namespace std;

CpuDebug::CpuDebug(Chip8 * ctrl, Screen * sc) : Cpu(ctrl, sc) {
	cout << "> Debug mode enabled !" << endl;

	_initDebugCommandMap();

	this->_stepMode = true;
}

bool CpuDebug::emulateCycle() {
	if (this->pc >= (MEM_SIZE - 1) || !this->running)
		return false;

	uint16_t opcode = getNextOpCode();
	bool hasBp = false; //hasBreakPoint (this->pc);
	if (this->_stepMode || hasBp)
		_debugInst(opcode, hasBp);
	execOpcode(opcode);
	this->pc += 2;
	count();
	return true;
}

void CpuDebug::_memoryDump() const {
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

void CpuDebug::_debugInst(uint16_t opcode, bool hasBp) {
	auto opcodes = Opcodes::instance()->getList();
	Opcode * op = (*opcodes)[GetOpcodeId(opcode)];

	if (hasBp)
		cout << "Break on " << this->pc << endl;
	cout << "[" << this->pc << "] " << op->disassemble(opcode) << endl;

	string cmd = "";
	bool continueExecution = false;

	while (!continueExecution)
	{
		cout << " >> ";
		getline(cin, cmd);

		if (_lastCmd.empty())
			_lastCmd = cmd;

		if (cmd.size() == 0 || cmd.empty())
			cmd = _lastCmd;

		if (_debugCommand.find(cmd) == _debugCommand.end())
			cmd = UNKNOWN_COMMAND;

		continueExecution = _debugCommand[cmd](cmd);
		_lastCmd = cmd;
	}
}

void CpuDebug::_initDebugCommandMap()
{
	_debugCommand[STEP_COMMAND] = std::bind(&CpuDebug::_stepCommand, this, std::placeholders::_1);
	_debugCommand[REG_COMMAND] = std::bind(&CpuDebug::_regCommand, this, std::placeholders::_1);
	_debugCommand[QUIT_COMMAND] = std::bind(&CpuDebug::_quitCommand, this, std::placeholders::_1);
	_debugCommand[CONTINUE_COMMAND] = std::bind(&CpuDebug::_continueCommand, this, std::placeholders::_1);
	_debugCommand[PRINT_HELP_COMMAND] = std::bind(&CpuDebug::_helpCommand, this, std::placeholders::_1);
	_debugCommand[UNKNOWN_COMMAND] = std::bind(&CpuDebug::_unknownCommand, this, std::placeholders::_1);
}

bool CpuDebug::_stepCommand(string & param)
{
	return true;
}

bool CpuDebug::_regCommand(string & param)
{
	_memoryDump();
	return false;
}

bool CpuDebug::_quitCommand(string & param)
{
	this->running = false;
	return true;
}

bool CpuDebug::_continueCommand(string & param)
{
	cout << "> Continuing..." << endl;
	this->_stepMode = false;
	return true;
}

bool CpuDebug::_helpCommand(string & param)
{
	cout << "> Help :" << endl;
	cout << "   - p : step (executes one instruction and break)" << endl;
	cout << "   - r : reg (display all the registrers)" << endl;
	cout << "   - c : continue the execution" << endl;
	cout << "   - q : quit the emulator" << endl;
	cout << "   - h : display the help" << endl << endl;
	return false;
}

bool CpuDebug::_unknownCommand(string & param)
{
	cout << "> Unknown command ! Enter h for help." << endl;
	return false;
}