#include "utils.hpp"

using namespace std;

string Utils::htos (uint16_t n) 
{
    stringstream ss;
    ss << n;
    return ss.str ();
}

uint16_t Utils::StringToUint16(std::string str)
{
	char *end;
	errno = 0;
	intmax_t val = std::strtoimax(str.c_str(), &end, 10);
	if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
		return false;
	return (uint16_t)val;
}

string Utils::WStringToString(wstring str)
{
	string res(str.length(), ' ');
	std::copy(str.begin(), str.end(), res.begin());
	return res;
}

wstring Utils::StringToWString(string str)
{
	wstring res(str.length(), L' ');
	std::copy(str.begin(), str.end(), res.begin());
	return res;
}
