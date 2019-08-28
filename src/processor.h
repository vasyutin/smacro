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
	bool isExcluded(const TFileNameString &FileName_) const {
		return matchesPatterns(FileName_, m_ExcludePatterns);
		}
	bool isIgnored(const TFileNameString &FileName_) const {
		return matchesPatterns(FileName_, m_IgnorePatterns);
		}

private:
	struct TProcessData {
		TProcessData(const TFileNameString &InputFile_, const TFileNameString &OutputFile_);
		bool initialized() const;
		const std::string &errorMessage() const {return ErrorMessage;}

		std::vector<std::unique_ptr<std::ifstream> > Input;
		// Track included files to prevent cyclic includes
		std::vector<TFileNameString> InputFiles;
		std::vector<unsigned> CurrentLines;

		std::ofstream Output;
		const TFileNameString &OutputFile;
		std::string ErrorMessage;

		const TFileNameString& inputFile() const {return InputFiles.back();}
		unsigned lineNo() const {return CurrentLines.back();}
		};

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
	const TExcludePatterns &m_ExcludePatterns, &m_IgnorePatterns;
	//
	// DoProcessing - process variables substitution and files inclusion
	TResult readNextLine(TProcessData &Data_, std::string &Line_, bool DoProcessing_);
	TResult processOperator(TProcessData &Data_, std::string &Line_, bool Skip_);
	static bool isOperator(TResult Result_);
	void valuesSubstitution(std::string &Line_);
	TResult processLinesTillNextKeyword(TProcessData &Data_, std::string &Line_, bool Skip_);

	std::regex m_VariableRegExp, m_IfRegExp, m_ElifRegExp, m_ElseRegExp, m_EndifRegExp, 
		m_CommentOperatorRegExp, m_CommentRegExp, m_IncludeRegExp;

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

	// Data_ is necessary for obtaining an input file name
	bool calculateExp(const std::string &Line_, bool &Result_, TProcessData &Data_);
	static std::string::const_iterator firstNonSpace(std::string::const_iterator Begin_, 
		std::string::const_iterator End_);

	static bool matchesPatterns(const TFileNameString &FileName_, const TExcludePatterns &Patterns_);
	};
