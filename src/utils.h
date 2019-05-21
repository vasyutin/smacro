#pragma once

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
#include "globals.h"

#include <vector>
#include <cctype>
#include <algorithm>

#include <ctype.h>

// -----------------------------------------------------------------------
bool FolderExists(const TFileNameChar *Folder_);
bool MakePath(const TFileNameChar *Path_);
bool FolderEntries(const TFileNameChar *Folder_, std::vector<TFileNameString> &Folders_, 
	std::vector<TFileNameString> &Files_);
bool FileExists(const TFileNameChar *File_);
bool RemoveFile(const TFileNameChar *File_);
bool DuplicateFile(const TFileNameChar *Src_, const TFileNameChar *Dst_);

// -----------------------------------------------------------------------
template <typename _TString>
void TrimString(_TString &String_) 
{
struct THelper {
	static bool notASpace(typename _TString::value_type Char_) {
		#if defined(SMACRO_WINDOWS)
			if(sizeof(Char_) == sizeof(wchar_t))
				return !iswspace((wint_t)Char_);
			else
		#endif				
		return !std::isspace((int)Char_);
		}
	};

// ---
String_.erase(String_.begin(), std::find_if(String_.begin(), String_.end(), THelper::notASpace));
String_.erase(std::find_if(String_.rbegin(), String_.rend(), THelper::notASpace).base(), 
	String_.end());
}

// -----------------------------------------------------------------------
#if defined(SMACRO_WINDOWS)
	#if defined(__MINGW32__) || defined(__MINGW64__)
		std::string WStringToWindowsLocal(const std::wstring &Unicode_);
		void WindowsLocalToWString(const char *Local_, std::wstring &String_);
	#endif

	std::string FileNameStringToConsole(const TFileNameString &Unicode_);
	TFileNameString AbsolutePath(const TFileNameString &Path_);
#else
	#define FileNameStringToConsole(Param_) (Param_)
#endif // #if defined(SMACRO_WINDOWS)


