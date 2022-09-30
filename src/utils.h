#pragma once

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
#include <tpclbase.h>

#include <vector>
#include <cctype>
#include <algorithm>
#include <ctype.h>

// -----------------------------------------------------------------------
template <typename _TString>
void TrimString(_TString &String_) 
{
struct THelper {
	static bool notASpace(typename _TString::value_type Char_) {
		#if defined(TPCL_OS_WINDOWS) && defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
			return !iswspace(Char_);
		#else
			return Char_ < 0? true: !std::isspace(Char_);
		#endif				
		
		}
	};

// ---
String_.erase(String_.begin(), std::find_if(String_.begin(), String_.end(), THelper::notASpace));
String_.erase(std::find_if(String_.rbegin(), String_.rend(), THelper::notASpace).base(), 
	String_.end());
}
