#pragma once

#include <sstream>
#include <cstdint>
#include <cinttypes>
#include <string>

class Utils
{
public:
	static std::string htos(uint16_t n);
	static uint16_t StringToUint16(std::string);
	static std::string WStringToString(std::wstring);
	static std::wstring StringToWString(std::string);
};
