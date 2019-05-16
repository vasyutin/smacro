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
#endif

// -----------------------------------------------------------------------
bool FolderExists(const TFileNameChar *Folder_)
{
#if defined(SMACRO_WINDOWS)
	DWORD Attribs = GetFileAttributesW(Folder_);
	if(Attribs == INVALID_FILE_ATTRIBUTES || 
		!(Attribs & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
		}
	return true;
#else
#endif
}

// -----------------------------------------------------------------------
bool MakePath(const TFileNameChar *Path_)
{
#if defined(SMACRO_WINDOWS)
	return SHCreateDirectoryExW(NULL, Path_, NULL) == ERROR_SUCCESS;
#else
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
return false;
}

// -----------------------------------------------------------------------
bool FileExists(const TFileNameChar *File_)
{
#if defined(SMACRO_WINDOWS)
	DWORD Attribs = GetFileAttributesW(File_);
	if(Attribs == INVALID_FILE_ATTRIBUTES || (Attribs & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
		}
	return true;
#else
#endif
}

// -----------------------------------------------------------------------
bool RemoveFile(const TFileNameChar *File_)
{
#if defined(SMACRO_WINDOWS)
	return (DeleteFileW(File_) == TRUE);
#else
#endif
}

// -----------------------------------------------------------------------
bool CopyFile(const TFileNameChar *Src_, const TFileNameChar *Dst_)
{
#if defined(SMACRO_WINDOWS)
	return ::CopyFile(Src_, Dst_, TRUE) == TRUE;
#else
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
#endif
