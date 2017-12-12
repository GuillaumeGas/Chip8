#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>

#include "Utils.hpp"
#include "Exceptions.hpp"

enum FileType
{
	FILE_TYPE,
	DIRECTORY_TYPE,
};

struct File
{
	std::string fileName;
	std::string filePath;
	std::string fileFullPath;
	FileType fileType;
};

class Directory
{
public:
    Directory();
	Directory(std::string path);

	std::vector<File> getFiles() const;
	std::string getPath() const;
	void enter(std::string directoryName);
    void setCurrent(std::string dirPath);

	static std::string GetFullFilePath(const std::string & filePath);
    static std::string RemoveFileName(const std::string & filePath);
	static std::string GetCurrentPath();
	static Directory GetCurrent();

private:
	File _getFileFromWin32FileData(WIN32_FIND_DATA * dataFile) const;

	std::string _path;
};