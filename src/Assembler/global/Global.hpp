#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "../ast/Instruction.hpp"

class Global {
public:
	static Global * Instance();

	void setFile(std::string& file_name);
	void addLine(std::string& line);
	std::string getLine(unsigned int num);
	std::map<std::string, ast::Instruction*> * getLabelsMap() const;
	std::map<ast::Instruction*, uint16_t> * getInstsMap() const;

	std::string file_name;
	std::vector<std::string> file_content;

private:
	Global();
	static Global * instance;

	std::map<std::string, ast::Instruction*> * _labelsMap;
	std::map<ast::Instruction*, uint16_t> * _instMap;
};
