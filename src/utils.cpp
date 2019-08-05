/*
* This file is part of SMACRO.
*
* Written by Sergey Vasyutin (in@vasyut.in)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will `be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this software. If not, see <http://www.gnu.org/licenses/>.
*/
#include "utils.h"

#if defined(SMACRO_WINDOWS)
	#include <windows.h>
	#include <Shlobj.h>
#elif defined(SMACRO_LINUX)
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <string.h>
#endif

#include <assert.h>

#include <iostream>

// -----------------------------------------------------------------------
bool FolderExists(const TFileNameChar *Folder_)
{
assert(Folder_);
#if defined(SMACRO_WINDOWS)
	DWORD Attribs = GetFileAttributesW(Folder_);
	if(Attribs == INVALID_FILE_ATTRIBUTES || 
		!(Attribs & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
		}
	return true;
#else
	struct stat Stat;
	if(stat(Folder_, &Stat)) return false;
	return ((Stat.st_mode & S_IFMT) == S_IFDIR);
#endif
}

// -----------------------------------------------------------------------
bool MakePath(const TFileNameChar *Path_)
{
assert(Path_);
#if defined(SMACRO_WINDOWS)
	return SHCreateDirectoryExW(NULL, Path_, NULL) == ERROR_SUCCESS;
#else
	const mode_t Mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
	char *PathCopy = strdup(Path_);
	if(!PathCopy) return false;

	char *Ptr = PathCopy;
	while(*Ptr) {
		Ptr++;
		while(*Ptr && *Ptr != '/') Ptr++;
		bool NotEndOfLine = (*Ptr != (char)0);
		if(NotEndOfLine) *Ptr = (char)0;
		if(mkdir(PathCopy, Mode) == -1 && errno != EEXIST) {
			free(PathCopy);
			return false;
			}
		if(NotEndOfLine) *Ptr = '/';
		}
	free(PathCopy);
	return true;
#endif
}

// -----------------------------------------------------------------------
bool FolderEntries(const TFileNameChar *Folder_, std::vector<TFileNameString> &Folders_, 
	std::vector<TFileNameString> &Files_)
{
Folders_.clear();
Files_.clear();

#if defined(SMACRO_WINDOWS)
	WIN32_FIND_DATAW FindFileData;
	TFileNameString Dir(Folder_);
	Dir += L"*.*";
	HANDLE Handler = FindFirstFileW(Dir.c_str(), &FindFileData);
	if(Handler == INVALID_HANDLE_VALUE) return true;
	//
	do {
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if(!wcscmp(FindFileData.cFileName, L"..") || !wcscmp(FindFileData.cFileName, L"."))
				continue;
			Folders_.push_back(FindFileData.cFileName);
			}
		else {
			Files_.push_back(FindFileData.cFileName);
			}
		} while (FindNextFileW(Handler, &FindFileData) != 0);
	FindClose(Handler);
#else 
	DIR *DirStream = opendir(Folder_);
	if(!DirStream) return true;
	//
	struct dirent* FileHandle;
	while((FileHandle = readdir(DirStream)) != NULL) {
		if(FileHandle->d_type == DT_DIR) {
			if(!strcmp(FileHandle->d_name, ".") || !strcmp(FileHandle->d_name, ".."))
				continue;
			Folders_.push_back(FileHandle->d_name);
			}
		else {
			Files_.push_back(FileHandle->d_name);
			}
		}
	closedir(DirStream);
#endif
return true;
}

// -----------------------------------------------------------------------
bool FileExists(const TFileNameChar *File_)
{
assert(File_);
#if defined(SMACRO_WINDOWS)
	DWORD Attribs = GetFileAttributesW(File_);
	if(Attribs == INVALID_FILE_ATTRIBUTES || (Attribs & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
		}
	return true;
#else
	struct stat Stat;
	if(stat(File_, &Stat)) return false;
	return ((Stat.st_mode & S_IFMT) != S_IFDIR);
#endif
}

// -----------------------------------------------------------------------
bool RemoveFile(const TFileNameChar *File_)
{
assert(File_);
#if defined(SMACRO_WINDOWS)
	return (DeleteFileW(File_) == TRUE);
#else
	return unlink(File_) == 0;
#endif
}

// -----------------------------------------------------------------------
bool DuplicateFile(const TFileNameChar *Src_, const TFileNameChar *Dst_)
{
assert(Src_ && Dst_);
#if defined(SMACRO_WINDOWS)
	return CopyFileW(Src_, Dst_, TRUE) == TRUE;
#else
	const mode_t Mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int SourceFile = open(Src_, O_RDONLY);
	if(SourceFile == -1) return false;
	int DestFile = open(Dst_, O_CREAT | O_TRUNC | O_WRONLY, Mode);
	if(DestFile == -1) {
		close(SourceFile);
		return false;
		}
	char Buffer[8 * 1024];
	bool RetValue = true;
	while(true) {
		ssize_t BytesRead = read(SourceFile, Buffer, sizeof(Buffer));
		if(!BytesRead) break; // EOF
		else if(BytesRead < 0) {RetValue = false; break;}
		ssize_t BytesWritten = write(DestFile, Buffer, BytesRead);
		if(BytesWritten != BytesRead) {RetValue = false; break;}
		}
	close(SourceFile);
	close(DestFile);
	return RetValue;
#endif
}

// -----------------------------------------------------------------------
#if defined(SMACRO_WINDOWS) && (defined(__MINGW32__) || defined(__MINGW64__))
std::string WStringToWindowsLocal(const std::wstring &Unicode_)
{
int Length = (int)Unicode_.size();
int CharsCount = WideCharToMultiByte(CP_ACP, 0, Unicode_.c_str(), Length, NULL, 0, NULL, NULL);
std::string RetValue;
if(CharsCount) {
	RetValue.resize(CharsCount);
	WideCharToMultiByte(CP_ACP, 0, Unicode_.c_str(), Length, (char*)(RetValue.c_str()), CharsCount,
		NULL, NULL);
	}
return RetValue;
}

// -----------------------------------------------------------------------
void WindowsLocalToWString(const char *Local_, std::wstring &String_)
{
String_.clear();
int Length = strlen(Local_);
int CharsCount = MultiByteToWideChar(CP_ACP, 0, Local_, Length, NULL, 0);
if(!CharsCount) return;
//
String_.resize(CharsCount);
MultiByteToWideChar(CP_ACP, 0, Local_, Length, (wchar_t*)(String_.c_str()), CharsCount);
}
#endif

// -----------------------------------------------------------------------
#if defined(SMACRO_WINDOWS)
std::string FileNameStringToConsole(const TFileNameString &Unicode_)
{
int Length = (int)Unicode_.size();
int CharsCount = WideCharToMultiByte(CP_OEMCP, 0, Unicode_.c_str(), Length, NULL, 0, NULL, NULL);
std::string RetValue;
if(CharsCount) {
	RetValue.resize(CharsCount);
	WideCharToMultiByte(CP_OEMCP, 0, Unicode_.c_str(), Length, (char*)(RetValue.c_str()), 
		CharsCount,	NULL, NULL);
	}
return RetValue;
}

// -----------------------------------------------------------------------
TFileNameString Utf8ToFileNameString(const std::string &Utf8_)
{
int CharsCount = MultiByteToWideChar(CP_UTF8, 0, Utf8_.c_str(), int(Utf8_.size()), NULL, 0);
if(!CharsCount) return TFileNameString();
//
TFileNameString Result;
Result.resize(CharsCount);
MultiByteToWideChar(CP_UTF8, 0, Utf8_.c_str(), int(Utf8_.size()), (wchar_t*)(Result.c_str()), CharsCount);
return Result;
}

// -----------------------------------------------------------------------
TFileNameString AbsolutePath(const TFileNameString &Path_)
{
const DWORD MAX_PATH_SIZE = 1024;
wchar_t APath[MAX_PATH_SIZE];

DWORD RetValue = GetFullPathNameW(Path_.c_str(), MAX_PATH_SIZE, APath, NULL);
if(!RetValue) return TFileNameString();
//
return TFileNameString(APath, APath + RetValue);
}

#endif
