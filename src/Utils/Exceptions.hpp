#pragma once

#include <iostream>
#include <sstream>

class DirectoryException
{
public:
	DirectoryException(std::string message, int errorCode) : _msg(message), _errorCode(errorCode) {}
	virtual std::string toString()
	{
		std::stringstream ss;
		ss << "DirectoryException : " << _msg << " (error code : " << _errorCode << ")";
		return ss.str();
	}
protected:
	std::string _msg;
	int _errorCode;
};
