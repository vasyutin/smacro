/*
* This file is part of Tiny Platfrom Crossing Library (TPCL).
*
* Written by Sergey Vasyutin (in[at]vasyut.in)
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

#include <tpcl.h>

#if defined(TPCL_OS_WINDOWS)
	#include <windows.h>
	#include <Shlobj.h>
#elif defined(TPCL_OS_LINUX)
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <string.h>
#endif

#include <assert.h>

namespace tpcl {

// -----------------------------------------------------------------------
size_t FileNameLength(const TFileNameChar* FileName_)
{
#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
	return wcslen(FileName_);
#else
	return strlen(FileName_);
#endif
}

// -----------------------------------------------------------------------
void AppendSeparatorIfAbsent(TFileNameString& Value_)
{
	if(Value_[Value_.size() - 1] != TPCL_FS_SEPARATOR)
		Value_ += TPCL_FS_SEPARATOR;
}

// -----------------------------------------------------------------------
bool FolderExists(const TFileNameChar *Folder_)
{
assert(Folder_);
#if defined(TPCL_OS_WINDOWS)
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		DWORD Attribs = GetFileAttributesW(Folder_);
	#else
		DWORD Attribs = GetFileAttributesA(Folder_);
	#endif
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
bool CreatePath(const TFileNameChar *Path_)
{
assert(Path_);
#if defined(TPCL_OS_WINDOWS)
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		return SHCreateDirectoryExW(NULL, Path_, NULL) == ERROR_SUCCESS;
	#else
		return SHCreateDirectoryExA(NULL, Path_, NULL) == ERROR_SUCCESS;
	#endif
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

#if defined(TPCL_OS_WINDOWS)
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
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
		WIN32_FIND_DATAA FindFileData;
		TFileNameString Dir(Folder_);
		Dir += "*.*";
		HANDLE Handler = FindFirstFileA(Dir.c_str(), &FindFileData);
		if(Handler == INVALID_HANDLE_VALUE) return true;
		//
		do {
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if(!strcmp(FindFileData.cFileName, "..") || !strcmp(FindFileData.cFileName, "."))
					continue;
				Folders_.push_back(FindFileData.cFileName);
				}
			else {
				Files_.push_back(FindFileData.cFileName);
				}
			} while (FindNextFileA(Handler, &FindFileData) != 0);
		FindClose(Handler);
	#endif
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
#if defined(TPCL_OS_WINDOWS)
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		DWORD Attribs = GetFileAttributesW(File_);
	#else
		DWORD Attribs = GetFileAttributesA(File_);
	#endif
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
#if defined(TPCL_OS_WINDOWS)
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		return (DeleteFileW(File_) == TRUE);
	#else
		return (DeleteFileA(File_) == TRUE);
	#endif
#else
	return unlink(File_) == 0;
#endif
}

// -----------------------------------------------------------------------
bool DuplicateFile(const TFileNameChar *Src_, const TFileNameChar *Dst_)
{
assert(Src_ && Dst_);
#if defined(TPCL_OS_WINDOWS)
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		return CopyFileW(Src_, Dst_, TRUE) == TRUE;
	#else
		return CopyFileA(Src_, Dst_, TRUE) == TRUE;
	#endif
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
bool Utf8ToWide(const char* UTF8_, int UTF8Size_, std::wstring& String_)
{
	if(UTF8Size_ < 0) UTF8Size_ = (int)strlen(UTF8_);
	if(!UTF8Size_) {
		String_.clear();
		return true;
	}
	int CharsCount = MultiByteToWideChar(CP_UTF8, 0, UTF8_, UTF8Size_, NULL, 0);
	if(CharsCount <= 0) return false;
	//
	String_.resize(CharsCount);
	return MultiByteToWideChar(CP_UTF8, 0, UTF8_, UTF8Size_, (wchar_t*)(String_.c_str()), CharsCount) > 0;
}

// -----------------------------------------------------------------------
bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_)
{
	if(UTF8Size_ < 0) UTF8Size_ = (int)strlen(UTF8_);
	if(!UTF8Size_) {
		String_.clear();
		return true;
	}
	int CharsCount = MultiByteToWideChar(CP_UTF8, 0, UTF8_, UTF8Size_, NULL, 0);
	if(CharsCount <= 0) return false;
	//
	std::wstring Temp;
	Temp.resize(CharsCount);
	if(MultiByteToWideChar(CP_UTF8, 0, UTF8_, UTF8Size_, (wchar_t*)(Temp.c_str()), CharsCount) <= 0)
		return false;
	//
	int LocalCharsCount = WideCharToMultiByte(CP_ACP, 0, Temp.c_str(), CharsCount, NULL, 0, NULL, NULL);
	if(LocalCharsCount <= 0) return false;
	//
	String_.resize(LocalCharsCount);
	return WideCharToMultiByte(CP_ACP, 0, Temp.c_str(), CharsCount, (char*)(String_.c_str()), LocalCharsCount, NULL, NULL) > 0;
}

// -----------------------------------------------------------------------
bool WideToUtf8(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_)
{
	if(UnicodeSize_ < 0) UnicodeSize_ = (int)wcslen(Unicode_);
	if(!UnicodeSize_) {
		String_.clear();
		return true;
	}
	int CharsCount = WideCharToMultiByte(CP_UTF8, 0, Unicode_, UnicodeSize_, NULL, 0, NULL, NULL);
	if(CharsCount <= 0) return false;
	//
	String_.resize(CharsCount);
	return WideCharToMultiByte(CP_UTF8, 0, Unicode_, UnicodeSize_, (char*)(String_.c_str()), CharsCount, NULL, NULL) > 0;
}

// -----------------------------------------------------------------------
bool WideToLocal(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_)
{
	if(UnicodeSize_ < 0) UnicodeSize_ = (int)wcslen(Unicode_);
	if(!UnicodeSize_) {
		String_.clear();
		return true;
	}
	int CharsCount = WideCharToMultiByte(CP_ACP, 0, Unicode_, UnicodeSize_, NULL, 0, NULL, NULL);
	if(CharsCount <= 0) return false;
	//
	String_.resize(CharsCount);
	return WideCharToMultiByte(CP_ACP, 0, Unicode_, UnicodeSize_, (char*)(String_.c_str()), CharsCount, NULL, NULL) > 0;
}

// -----------------------------------------------------------------------
bool LocalToUtf8(const char* Local8Bit_, int Local8BitSize_, std::string& String_)
{
	if(Local8BitSize_ < 0) Local8BitSize_ = (int)strlen(Local8Bit_);
	if(!Local8BitSize_) {
		String_.clear();
		return true;
	}
	int CharsCount = MultiByteToWideChar(CP_UTF8, 0, Local8Bit_, Local8BitSize_, NULL, 0);
	if(CharsCount <= 0) return false;
	//
	std::wstring Temp;
	Temp.resize(CharsCount);
	if(MultiByteToWideChar(CP_UTF8, 0, Local8Bit_, Local8BitSize_, (wchar_t*)(Temp.c_str()), CharsCount) <= 0)
		return false;
	//
	int CharsCount2 = WideCharToMultiByte(CP_UTF8, 0, Temp.c_str(), CharsCount, NULL, 0, NULL, NULL);
	if(CharsCount2 <= 0) return false;
	//
	String_.resize(CharsCount2);
	return WideCharToMultiByte(CP_UTF8, 0, Temp.c_str(), CharsCount, (char*)(String_.c_str()), CharsCount2, NULL, NULL) > 0;
}

// -----------------------------------------------------------------------
bool LocalToWide(const char* Local8Bit_, int Local8BitSize_, std::wstring& String_)
{
	if(Local8BitSize_ < 0) Local8BitSize_ = (int)strlen(Local8Bit_);
	if(!Local8BitSize_) {
		String_.clear();
		return true;
	}
	int CharsCount = MultiByteToWideChar(CP_ACP, 0, Local8Bit_, Local8BitSize_, NULL, 0);
	if(CharsCount <= 0) return false;
	//
	String_.resize(CharsCount);
	return MultiByteToWideChar(CP_UTF8, 0, Local8Bit_, Local8BitSize_, (wchar_t*)(String_.c_str()), CharsCount) > 0;
}

// -----------------------------------------------------------------------
std::string FileNameToConsoleString(const TFileNameChar* Message_)
{
	std::string RetValue;
	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		int Length = (int)wcslen(Message_);
		if(Length) {
			int CharsCount = WideCharToMultiByte(CP_OEMCP, 0, Message_, Length, NULL, 0, NULL, NULL);
			if(CharsCount) {
				RetValue.resize(CharsCount);
				WideCharToMultiByte(CP_OEMCP, 0, Message_, Length, (char*)(RetValue.c_str()), CharsCount, NULL, NULL);
			}
		}
	#else
		std::wstring Temp;
		if(LocalToWide(Message_, -1, Temp) && !Temp.empty()) {
			int CharsCount = WideCharToMultiByte(CP_OEMCP, 0, Temp.c_str(), (int)Temp.size(), NULL, 0, NULL, NULL);
			if(CharsCount) {
				RetValue.resize(CharsCount);
				WideCharToMultiByte(CP_OEMCP, 0, Temp.c_str(), (int)Temp.size(), (char*)(RetValue.c_str()), CharsCount, NULL, NULL);
			}
		}
	#endif
	return RetValue;
}

} // namespace tcpl
