#include <Shlwapi.h>

#include "Directory.hpp"
#include "ScopeGuard.hpp"

// TODO : check if the given directory path exists

using namespace std;

Directory::Directory()
{
    _path = GetCurrentPath();
}

Directory::Directory(string path) : _path(path) {}

vector<File> Directory::getFiles() const
{
	vector<File> vec_res;
	HANDLE fileHandle;
	WIN32_FIND_DATA dataFile;

	wstring newPath = Utils::StringToWString(_path + '\\' + '*');
	LPTSTR currentDir = (LPTSTR)newPath.c_str();

	if ((fileHandle = FindFirstFile(currentDir, &dataFile)) == INVALID_HANDLE_VALUE)
		throw DirectoryException("Directory!getFiles()", GetLastError());

	do
	{
		vec_res.push_back(_getFileFromWin32FileData(&dataFile));
	} while (FindNextFile(fileHandle, &dataFile));

	FindClose(fileHandle);

	return vec_res;
}

string Directory::getPath() const
{
	return _path;
}

void Directory::enter(string directoryName)
{
	stringstream ss;
	ss << _path;
	if (_path[_path.length() - 1] != '\\')
		ss << '\\';
	ss << directoryName;

	wstring res = Utils::StringToWString(ss.str());
	if (!PathFileExists(res.c_str()))
		throw DirectoryException("Can't enter in directory, path not found !", GetLastError());

	_path = ss.str();
}

void Directory::setCurrent(string dirPath)
{
    _path = dirPath;
}

string Directory::GetFullFilePath(const string & filePath)
{
	wstring wfilePath = Utils::StringToWString(filePath);
	LPTSTR resPath = NULL;
	DWORD length = 0, res = 0;

	if ((res = GetFullPathName(wfilePath.c_str(), 0, NULL, NULL)) == 0)
		throw DirectoryException("Directory!GetDirectoryPath on GetFullPathName (get size)", GetLastError());

	length = res;
	
	if ((resPath = new TCHAR[length]) == NULL)
		throw DirectoryException("Directory!GetDirectoryPath on alloc", GetLastError());

	OnExitingScope(([resPath]() {
		delete[] resPath;
	}));

	if ((res = GetFullPathName(wfilePath.c_str(), length, resPath, NULL)) == 0)
		throw DirectoryException("Directory!GetDirectoryPath on GetFullPathName", GetLastError());

	return Utils::WStringToString(wstring(resPath));
}

string Directory::RemoveFileName(const string & filePath)
{
    wstring test = Utils::StringToWString(filePath);
    const wchar_t * str = test.c_str();
    PathRemoveFileSpec((LPWSTR)str);
    string res = Utils::WStringToString(wstring(str));
    return res;
}

string Directory::GetCurrentPath()
{
	LPTSTR currentDir = NULL;
	DWORD length = 0;
	DWORD res = 0;

	if ((res = GetCurrentDirectory(0, NULL)) == 0)
		throw DirectoryException("Directory!GetCurrentPath() on GetCurrentDirectory (get size)", GetLastError());

	length = res;

	if ((currentDir = new TCHAR[length]) == NULL)
		throw DirectoryException("Directory!GetCurrentPath() on alloc", GetLastError());
	
	OnExitingScope(([currentDir]() {
		delete[] currentDir;
	}));

	if ((res = GetCurrentDirectory(length, currentDir)) == 0)
		throw DirectoryException("Directory!GetCurrentPath() on GetCurrentDirectory", GetLastError());

	return Utils::WStringToString(wstring(currentDir));
}

Directory Directory::GetCurrent()
{
	return Directory(GetCurrentPath());
}

File Directory::_getFileFromWin32FileData(WIN32_FIND_DATA * dataFile) const
{
	File file;
	file.fileType = dataFile->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ? FileType::DIRECTORY_TYPE : FileType::FILE_TYPE;
	file.fileName = Utils::WStringToString(dataFile->cFileName);
	file.fileFullPath = _path + "\\" + file.fileName;
	file.filePath = _path;
	return file;
}