#pragma once

#include <sstream>
#include <cstdint>
#include <string>

class Utils
{
public:
	static std::string htos(uint16_t n);
	static std::string WStringToString(std::wstring);
	static std::wstring StringToWString(std::string);
};
