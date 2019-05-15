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

#include <string.h>

// -----------------------------------------------------------------------
#if defined(SMACRO_WINDOWS)
	#if defined(__MINGW32__) || defined(__MINGW64__)
		std::string WStringToWindowsLocal(const std::wstring &Unicode_);
		void WindowsLocalToWString(const char *Local_, std::wstring &String_);
	#endif

	std::string FileNameStringToConsole(const TFileNameString &Unicode_);
#else
	#define FileNameStringToConsole(Param_) (Param_)
#endif // #if defined(SMACRO_WINDOWS)

// -----------------------------------------------------------------------
inline size_t FileNameStringLength(const TFileNameChar *String_) 
{
#if defined(SMACRO_WINDOWS)
	return wcslen(String_);
#else
	return strlen(String_);
#endif
}



