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
#include <string>

#ifdef _MSC_VER
   #if defined(_WIN64) || defined(_WIN32)
      #ifndef SMACRO_WINDOWS
         #define SMACRO_WINDOWS
		#endif
   #endif
#elif defined(__MINGW32__) || defined(__MINGW64__)
   #ifndef SMACRO_WINDOWS
      #define SMACRO_WINDOWS
	#endif
#elif defined(__linux__) || defined(__linux)
	#ifndef SMACRO_LINUX
		#define SMACRO_LINUX
	#endif
#endif

#if defined(SMACRO_WINDOWS)
	typedef wchar_t TFileNameChar;
	typedef std::wstring TFileNameString;
	#define DIR_SEPARATOR wchar_t('\\')
#else
	typedef char TFileNameChar;
	typedef std::string TFileNameString;
	#define DIR_SEPARATOR '/'
#endif

