#pragma once

#include <iostream>
#include <cstdint>
#include <sstream>
#include <iomanip>

class ArgsException
{
public:
	ArgsException() {}
	ArgsException(const char * str) : msg(str) {}
	virtual std::string toString() const 
	{
		return msg;
	}
protected:
	std::string msg;
};

class UnknownCommandArgsException : public ArgsException
{
public:
	UnknownCommandArgsException(const char * cmd) : ArgsException(cmd) {}
	std::string toString() const
	{
		return "Unknown command \"" + msg + "\"";
	}
};

class TooManyArgsException : public ArgsException
{
public:
	std::string toString() const
	{
		return "Too many arguments !";
	}
};

class Chip8Exception 
{
public:
	Chip8Exception() {}
	Chip8Exception(const char * str) : msg(str) {}
	virtual std::string toString() const 
	{
		return msg;
	}
protected:
	std::string msg;
};

class SDLException : public Chip8Exception {
public:
	SDLException(const char * str) : Chip8Exception(str) {}
};

class SDLInitException : public SDLException {
public:
	SDLInitException(const char * str) : SDLException(str) {}
	std::string toString() const {
		return "SDL Init : " + msg;
	}
};

class ScreenInitException : public SDLException {
public:
	ScreenInitException(const char * str) : SDLException(str) {}
	std::string toString() const {
		return "SDL SetVideoMode : " + msg;
	}
};

class PixelInitException : SDLException {
public:
	PixelInitException(const char * str) : SDLException(str) {}
	std::string toString() const {
		return "SDL CreateRGBSurface : " + msg;
	}
};

class CpuException : public Chip8Exception {
public:
	CpuException() {}
	CpuException(const char * str) : Chip8Exception(str) {}
};

class OpcodeNotFound : public CpuException {
public:
	OpcodeNotFound(const uint16_t opcode, const uint16_t loc) {
		std::stringstream ss;
		ss << "Opcode not found : ";
		ss << std::showbase << std::internal << std::setfill('0');
		ss << std::hex << std::setw(6) << opcode << std::dec << " at location " << loc << " (" << std::hex << std::setw(6) << loc << ")";
		msg = ss.str();
	}
};

class LoadFileError : public CpuException {
public:
	LoadFileError(const char * file_name) : CpuException(file_name) {}
	std::string toString() const {
		return "Error while loading file : " + msg;
	}
};

class SoundErrorException : public Chip8Exception {
public:
	SoundErrorException(const char * str) {
		std::stringstream ss;
		ss << "Sound error : ";
		ss << str;
		msg = ss.str();
	}
};
