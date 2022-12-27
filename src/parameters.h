/*
* This file is part of SMACRO.
*
* © 2017-2022 Sergey Vasyutin (sergey [at] vasyut.in)
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
#pragma once

#include <tpcl.h>

#include <filesystem>
#include <vector>
#include <regex>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> TVariables;

#ifdef TPCL_OS_WINDOWS
	typedef std::vector<std::wregex> TExcludePatterns;
#else
	typedef std::vector<std::regex> TExcludePatterns;
#endif

// -----------------------------------------------------------------------
struct TParameters {
	std::filesystem::path InputFolder;
	std::filesystem::path OutputFolder;
	TVariables Variables;
	TExcludePatterns ExcludePatterns, IgnorePatterns;
	std::vector<std::filesystem::path> OrderedFileList;
	bool AlternativeOperatorPrefix;
	};
