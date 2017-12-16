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

#ifndef __processor_h
#define __processor_h

#include "parameters.h"

#include <QList>

class QFile;

// -----------------------------------------------------------------------
class TProcessor {
public:
	TProcessor(const TParameters &Parameters_);
	bool processFile(const QString &Input_, const QString &Output_);
	//
	bool isExcluded(const QString &FileName_) const;

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
	TResult readNextLine(QFile &Input_, QString &Line_);
	TResult processOperator(QFile &Input_, QString &Line_, QFile &Output_, bool Skip_);
	static bool isOperator(TResult Result_);
	void valuesSubstitution(QString &Line_);
	TResult processLinesTillNextKeyword(QFile &Input_, QString &Line_, 
		QFile &Output_, bool Skip_);

	QRegExp m_NotWhitespaceRegExp, m_VariableRegExp,
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
		QRegExp RegExp;
		};
	QList<TLexemeRegExp> m_LexemeRegExps;

	// Input_ is necessary for obtaining an input file name
	bool calculateExp(const QString &Line_, bool &Result_, const QFile &Input_);
	};

#endif // #ifndef __processor_h