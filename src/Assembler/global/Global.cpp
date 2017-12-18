#include "Global.hpp"

using namespace std;

Global * Global::instance = NULL;

Global::Global() 
{
	_labelsMap = new std::map<std::string, ast::Instruction*>();
	_instMap = new std::map<ast::Instruction*, uint16_t>();
}

Global * Global::Instance() {
	if (instance == NULL)
		instance = new Global;
	return instance;
}

void Global::setFile(string& file_name) {
	this->file_name = file_name;
}

void Global::addLine(string& line) {
	file_content.push_back(line);
}

string Global::getLine(unsigned int num) {
	if (num > file_content.size()) {
		cout << "[Error] Line not found !" << endl;
		exit(-1);
	}
	return file_content[num - 1];
}

std::map<std::string, ast::Instruction*> * Global::getLabelsMap() const
{
	return _labelsMap;
}

std::map<ast::Instruction*, uint16_t> * Global::getInstsMap() const
{
	return _instMap;
}