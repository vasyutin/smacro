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

#include "parameters.h"
#include <fstream>

// -----------------------------------------------------------------------
class TProcessor {
public:
	TProcessor(const TParameters &Parameters_);
	bool processFile(const TFileNameString &Input_, const TFileNameString &Output_);
	bool isExcluded(const TFileNameString &FileName_) const;

private:
	enum class TResult {
		OK,
		ReadError,
		WriteError,
		EndOfFile,
		SyntaxError,
		OperatorIf,
		OperatorElif,
		OperatorElse,
		OperatorEndif
		};

	const TVariables &m_Variables;
	const TExcludePatterns &m_ExcludePatterns;
	unsigned m_Line;
	//
	TResult readNextLine(std::istream &Input_, std::string &Line_, const TFileNameString &Output_);
	TResult processOperator(std::istream &Input_, std::string &Line_, std::ostream &Output_, 
		bool Skip_);
	static bool isOperator(TResult Result_);
	void valuesSubstitution(std::string &Line_);
	TResult processLinesTillNextKeyword(std::istream &Input_, std::string &Line_, 
		std::ostream &Output_, bool Skip_);

	std::regex m_NotWhitespaceRegExp, m_VariableRegExp,
		m_IfRegExp, m_ElifRegExp, m_ElseRegExp, m_EndifRegExp, m_CommentOperatorRegExp,
		m_CommentRegExp;

	enum class TLexemeType {
		String,
		Variable,
		Defined,
		Not,
		Eq,
		Neq,
		Less,
		Greater,
		LessOrEqual,
		GreaterOrEqual,
		OpenBracket,
		CloseBracket,
		And,
		Or
		};

	//
	struct TLexemeRegExp {
		TLexemeType Type;
		std::regex RegExp;
		};
	std::vector<TLexemeRegExp> m_LexemeRegExps;

	// Input_ is necessary for obtaining an input file name
	bool calculateExp(const std::string &Line_, bool &Result_, const std::string &InputFile_);
	};
