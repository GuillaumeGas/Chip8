#pragma once

#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <functional>

#include "Cpu.hpp"
#include "Opcodes.hpp"
#include "Screen.hpp"

#define STEP_COMMAND       "p"
#define REG_COMMAND        "r"
#define QUIT_COMMAND       "q"
#define CONTINUE_COMMAND   "c"
#define PRINT_HELP_COMMAND "h" // can't use HELP_COMMAND macro because already use by winuser.h...
#define UNKNOWN_COMMAND    "unknown"

class Chip8;
class CpuDebug : public Cpu {
public:
	CpuDebug(Chip8 * ctrl, Screen * sc);

	virtual bool emulateCycle();

private:
	void _memoryDump() const;
	void _debugInst(uint16_t opcode, bool hasBreak);
	void _initDebugCommandMap();

	bool _stepMode;
	std::string _lastCmd;
	std::list<uint16_t> _bpList;
	std::map<std::string, std::function<bool(std::string&)>> _debugCommand;

	bool _stepCommand(std::string & param);
	bool _regCommand(std::string & param);
	bool _quitCommand(std::string & param);
	bool _continueCommand(std::string & param);
	bool _helpCommand(std::string & param);
	bool _unknownCommand(std::string & param);
};
