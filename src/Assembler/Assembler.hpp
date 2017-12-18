#pragma once

#define DEFAULT_RES_FILE_NAME "a.ch8"

class Assembler
{
public:
    void assemble(const char * filePath, const char * resFilePath = nullptr);
};