#pragma once

#include <iostream>
#include "../../global/Position.hpp"

class SyntaxException {
public:
	SyntaxException(Position& pos) throw() : pos(pos) {}
	virtual std::string toString() const {
		return "[!] Syntax error " + pos.toString();
	}
protected:
	Position pos;
};

class SyntaxErrorException : public SyntaxException {
public:
	SyntaxErrorException(const std::string& msg, Position pos) throw() : msg(msg), SyntaxException(pos) {}
	std::string toString() const throw() {
		return "[!] Syntax error, unexpected '" + msg + "' on " + pos.toString();
	}
private:
	std::string msg;
};

class MissingErrorException : public SyntaxException {
public:
	MissingErrorException(const std::string& msg, Position pos) throw() : SyntaxException(pos), msg(msg) {}
	std::string toString() const throw() {
		return "[!] Syntax error, expected '" + msg + "' on " + pos.toString();
	}
private:
	std::string msg;
};

