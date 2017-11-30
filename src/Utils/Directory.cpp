#include "Directory.hpp"
#include "OnExitingScope.hpp"

using namespace std;

Directory::Directory(string path) : _path(path) 
{
	_wpath = Utils::StringToWString(_path);
}

vector<File> Directory::getFiles() const
{
	vector<File> vec_res;
	HANDLE fileHandle;
	WIN32_FIND_DATA dataFile;

	wstring newPath = _wpath + L'\\' + L'*';
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

wstring Directory::getPathW() const
{
	return _wpath;
}

string Directory::GetFullFilePath(const string filePath)
{
	wstring wfilePath = Utils::StringToWString(filePath);
	LPTSTR resPath = NULL;
	DWORD length = 0, res = 0;

	if ((res = GetFullPathName(wfilePath.c_str(), 0, NULL, NULL)) == 0)
		throw DirectoryException("Directory!GetDirectoryPath on GetFullPathName (get size)", GetLastError());

	length = res;
	
	if ((resPath = new TCHAR[length]) == NULL)
		throw DirectoryException("Directory!GetDirectoryPath on alloc", GetLastError());

	/*OnExitingScope(([resPath]() {
		delete[] resPath;
	}));
*/
	if ((res = GetFullPathName(wfilePath.c_str(), length, resPath, NULL)) == 0)
		delete[] resPath;
		throw DirectoryException("Directory!GetDirectoryPath on GetFullPathName", GetLastError());

	string resDirPath = Utils::WStringToString(wstring(resPath));
	delete[] resPath;

	return resDirPath;
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
	
	if ((res = GetCurrentDirectory(length, currentDir)) == 0)
	{
		delete[] currentDir;
		throw DirectoryException("Directory!GetCurrentPath() on GetCurrentDirectory", GetLastError());
	}

	string currentPath = Utils::WStringToString(wstring(currentDir));
	delete[] currentDir;

	return currentPath;
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