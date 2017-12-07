#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <functional>

#include "GameScreen.hpp"
#include "Cpu.hpp"
#include "Opcodes.hpp"

#define STEP_COMMAND       "p"
#define REG_COMMAND        "r"
#define BP_COMMAND         "bp"
#define BL_COMMAND         "bl"
#define BD_COMMAND         "bd"
#define BE_COMMAND         "be"
#define BC_COMMAND         "bc"
#define QUIT_COMMAND       "q"
#define CONTINUE_COMMAND   "c"
#define PRINT_HELP_COMMAND "h" // can't use HELP_COMMAND macro because already use by winuser.h...
#define UNKNOWN_COMMAND    "unknown"

class CpuDebug : public Cpu 
{
public:
	CpuDebug(GameScreen * screen);

	virtual bool emulateCycle();

private:
	void _memoryDump() const;
	void _debugInst(uint16_t opcode, bool hasBreak);
	void _initDebugCommandMap();
	std::pair<std::string, std::string> _parseCommand(std::string command);

	bool _stepMode;
	std::string _lastCmd;
	std::map<uint16_t, bool> _bpMap;
	std::map<std::string, std::function<bool(std::string&)>> _debugCommand;

	bool _stepCommand(std::string & param);
	bool _regCommand(std::string & param);
	bool _bpCommand(std::string & param);
	bool _blCommand(std::string & param);
	bool _bdCommand(std::string & param);
	bool _beCommand(std::string & param);
	bool _bcCommand(std::string & param);
	bool _quitCommand(std::string & param);
	bool _continueCommand(std::string & param);
	bool _helpCommand(std::string & param);
	bool _unknownCommand(std::string & param);
};
