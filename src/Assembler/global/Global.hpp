#pragma once

#include <iostream>
#include <vector>

class Global {
public:
    static Global * getInstance();

    void setFile(std::string& file_name);
    void addLine(std::string& line);
    std::string getLine(unsigned int num);

    std::string file_name;
    std::vector<std::string> file_content;

private:
    Global();
    static Global * instance;
};
