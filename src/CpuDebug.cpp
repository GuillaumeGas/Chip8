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
	auto it = _bpMap.find(this->pc);
	bool hasBp = (it != _bpMap.end()) && it->second;

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

		std::pair<string, string> cmdAndParam = _parseCommand(cmd);

		if (_debugCommand.find(cmdAndParam.first) == _debugCommand.end())
			cmdAndParam.first = UNKNOWN_COMMAND;

		continueExecution = _debugCommand[cmdAndParam.first](cmdAndParam.second);
		_lastCmd = cmd;
	}
}

std::pair<string, string> CpuDebug::_parseCommand(string command)
{
	size_t indexSpace = command.find_first_of(' ');
	string cmd = command.substr(0, indexSpace);
	string param = indexSpace == string::npos ? "" : command.substr(indexSpace + 1);

	return std::pair<string, string>(cmd, param);
}

void CpuDebug::_initDebugCommandMap()
{
	_debugCommand[STEP_COMMAND] = std::bind(&CpuDebug::_stepCommand, this, std::placeholders::_1);
	_debugCommand[REG_COMMAND] = std::bind(&CpuDebug::_regCommand, this, std::placeholders::_1);
	_debugCommand[BP_COMMAND] = std::bind(&CpuDebug::_bpCommand, this, std::placeholders::_1);
	_debugCommand[BL_COMMAND] = std::bind(&CpuDebug::_blCommand, this, std::placeholders::_1);
	_debugCommand[BD_COMMAND] = std::bind(&CpuDebug::_bdCommand, this, std::placeholders::_1);
	_debugCommand[BE_COMMAND] = std::bind(&CpuDebug::_beCommand, this, std::placeholders::_1);
	_debugCommand[BC_COMMAND] = std::bind(&CpuDebug::_bcCommand, this, std::placeholders::_1);
	_debugCommand[QUIT_COMMAND] = std::bind(&CpuDebug::_quitCommand, this, std::placeholders::_1);
	_debugCommand[CONTINUE_COMMAND] = std::bind(&CpuDebug::_continueCommand, this, std::placeholders::_1);
	_debugCommand[PRINT_HELP_COMMAND] = std::bind(&CpuDebug::_helpCommand, this, std::placeholders::_1);
	_debugCommand[UNKNOWN_COMMAND] = std::bind(&CpuDebug::_unknownCommand, this, std::placeholders::_1);
}

bool CpuDebug::_stepCommand(string & param)
{
	_stepMode = true;
	return true;
}

bool CpuDebug::_regCommand(string & param)
{
	_memoryDump();
	return false;
}

bool CpuDebug::_bpCommand(string & param)
{
	uint16_t addr = Utils::StringToUint16(param);
	if (_bpMap.find(addr) != _bpMap.end())
	{
		cout << "Enabled breakpoint on [" << param << "]" << endl;
	}
	else
	{
		cout << "Added breakpoint on " << param << endl;
	}
	_bpMap[addr] = true;
	return false;
}

bool CpuDebug::_blCommand(string & param)
{
	for (auto bp : _bpMap)
		cout << "[" << bp.first << "] : " << (bp.second ? "Enabled" : "Disabled") << endl;
	return false;
}

bool CpuDebug::_bdCommand(string & param)
{
	uint16_t addr = Utils::StringToUint16(param);
	if (_bpMap.find(addr) != _bpMap.end())
	{
		_bpMap[addr] = false;
		cout << "Disabled breakpoint on [" << param << "]" << endl;
	}
	else
	{
		cout << "No breakpoint found on [" << param << "]" << endl;
	}
	return false;
}

bool CpuDebug::_beCommand(string & param)
{
	uint16_t addr = Utils::StringToUint16(param);
	if (_bpMap.find(addr) != _bpMap.end())
	{
		_bpMap[addr] = true;
		cout << "Enabled breakpoint on [" << param << "]" << endl;
	}
	else
	{
		cout << "No breakpoint found on [" << param << "]" << endl;
	}
	return false;
}

bool CpuDebug::_bcCommand(string & param)
{
	uint16_t addr = Utils::StringToUint16(param);
	if (_bpMap.find(addr) != _bpMap.end())
	{
		_bpMap.erase(addr);
		cout << "Cleared breakpoint on [" << param << "]" << endl;
	}
	else
	{
		cout << "No breakpoint found on [" << param << "]" << endl;
	}
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
	cout << "   - p : step (executes one instruction and break)." << endl;
	cout << "   - r : reg (displays all the registrers)." << endl;
	cout << "   - c : continues the execution." << endl;
	cout << "   - b* breakpoint commands :" << endl;
	cout << "      - bp addr : adds a breakpoint on [addr]" << endl;
	cout << "      - bl : breakpoints list." << endl;
	cout << "      - bd addr : disables the breakpoint on this address." << endl;
	cout << "      - be addr : enables the breakpoint on this address." << endl;
	cout << "      - bc addr : clears the breakpoint on this address." << endl;
	cout << "   - q : quits the emulator." << endl;
	cout << "   - h : displays this help." << endl << endl;
	return false;
}

bool CpuDebug::_unknownCommand(string & param)
{
	cout << "> Unknown command ! Enter h for help." << endl;
	return false;
}