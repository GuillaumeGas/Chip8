#include "utils.hpp"

using namespace std;

string Utils::htos (uint16_t n) 
{
    stringstream ss;
    ss << n;
    return ss.str ();
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
