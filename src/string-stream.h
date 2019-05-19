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

// -----------------------------------------------------------------------
template <typename _Type>
std::string& operator<<(std::string &String_, const _Type &Value_) 
{
return (String_ += std::to_string(Value_));
}

// -----------------------------------------------------------------------
inline std::string& operator<<(std::string &String_, const std::string &Value_) 
{
return String_.append(Value_);
}

// -----------------------------------------------------------------------
inline std::string& operator<<(std::string &String_, const char *Value_)
{
return String_.append(Value_);
}

// -----------------------------------------------------------------------
inline std::string& operator<<(std::string &String_, const char Value_)
{
return String_ += Value_;
}
