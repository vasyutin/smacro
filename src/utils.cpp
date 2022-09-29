/*
* This file is part of SMACRO.
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
#include "utils.h"

#include <assert.h>


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
std::string FileNameStringToUtf8(const TFileNameString &Unicode_)
{
int Length = (int)Unicode_.size();
int CharsCount = WideCharToMultiByte(CP_UTF8, 0, Unicode_.c_str(), Length, NULL, 0, NULL, NULL);
std::string RetValue;
if(CharsCount) {
	RetValue.resize(CharsCount);
	WideCharToMultiByte(CP_UTF8, 0, Unicode_.c_str(), Length, (char*)(RetValue.c_str()), 
		CharsCount,	NULL, NULL);
	}
return RetValue;
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
