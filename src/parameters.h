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
#include <regex>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> TVariables;

#if defined(SMACRO_WINDOWS)
	typedef std::vector<std::wregex> TExcludePatterns;
#else
	typedef std::vector<std::regex> TExcludePatterns;
#endif

// -----------------------------------------------------------------------
struct TParameters {
	TFileNameString InputFolder;
	TFileNameString OutputFolder;
	TVariables Variables;
	TExcludePatterns ExcludePatterns, IgnorePatterns;
	};
