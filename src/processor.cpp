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
#include "processor.h"

#include <iostream>
#include <assert.h>

#include <QFile>
#include <QDir>
#include <QStack>

//#define DEBUG_OUTPUT

// -----------------------------------------------------------------------
TProcessor::TProcessor(const TParameters &Parameters_): 
	m_Variables(Parameters_.Variables),
	m_ExcludePatterns(Parameters_.ExcludePatterns),
	m_NotWhitespaceRegExp("\\S"),
	m_VariableRegExp("[$][{]([A-Za-z0-9_]+)[}]"),
	m_IfRegExp("#if\\s+"), 
	m_ElifRegExp("#elif\\s+"), 
	m_ElseRegExp("#else"),
	m_EndifRegExp("#endif"), 
	m_CommentOperatorRegExp("#[/][/]"),
	m_CommentRegExp("[/][/]")
{
m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::String;
m_LexemeRegExps.back().RegExp.setPattern("[\"]([^\"]*)[\"]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Variable;
m_LexemeRegExps.back().RegExp.setPattern("[A-Za-z_][A-Za-z_0-9]*");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Defined;
m_LexemeRegExps.back().RegExp.setPattern("defined\\s*[(]\\s*([A-Za-z_][A-Za-z_0-9]*)\\s*[)]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Not;
m_LexemeRegExps.back().RegExp.setPattern("!");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Eq;
m_LexemeRegExps.back().RegExp.setPattern("==");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Neq;
m_LexemeRegExps.back().RegExp.setPattern("!=");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Less;
m_LexemeRegExps.back().RegExp.setPattern("<");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Greater;
m_LexemeRegExps.back().RegExp.setPattern(">");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::LessOrEqual;
m_LexemeRegExps.back().RegExp.setPattern("<=");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::GreaterOrEqual;
m_LexemeRegExps.back().RegExp.setPattern(">");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::OpenBracket;
m_LexemeRegExps.back().RegExp.setPattern("[(]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::CloseBracket;
m_LexemeRegExps.back().RegExp.setPattern("[)]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::And;
m_LexemeRegExps.back().RegExp.setPattern("&&");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Or;
m_LexemeRegExps.back().RegExp.setPattern("||");
}

// -----------------------------------------------------------------------
bool TProcessor::isExcluded(const QString &FileName_) const
{
for(TExcludePatterns::const_iterator p_it = m_ExcludePatterns.begin(); 
	p_it != m_ExcludePatterns.end(); ++p_it) {
	if(p_it->exactMatch(FileName_)) return true;
	}
return false;
}

// -----------------------------------------------------------------------
bool TProcessor::isOperator(TResult Result_)
{
return Result_ == TResult::OperatorIf || Result_ == TResult::OperatorElif ||
	Result_ == TResult::OperatorElse || Result_ == TResult::OperatorEndif;
}

// -----------------------------------------------------------------------
bool TProcessor::processFile(const QString &Input_, const QString &Output_)
{
QFile InputFile(Input_);
if(!InputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
	std::cerr << "Can't open file '" << (const char*)Input_.toLocal8Bit() << "'.";
	return false;
	}
//
QFile OutputFile(Output_);
if(!OutputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
	std::cerr << "Can't open output file '" << (const char*)Output_.toLocal8Bit() << "'.";
	return false;
	}
//
m_Line = 0;
QString Line;
while(true) {
	TResult Result = readNextLine(InputFile, Line);
	if(Result == TResult::OK) {
		QByteArray Converted = Line.toUtf8();
		if(OutputFile.write(Converted) != Converted.size()) {
			std::cerr << "Error writing file '" << (const char*)Output_.toLocal8Bit() << "'.";
			return false;
			}
		continue;
		}
	else if(Result == TResult::EndOfFile)
		return true;
	else if(Result == TResult::SyntaxError)
		return false;
	//
	assert(isOperator(Result));
	// There can be only #if
	if(Result != TResult::OperatorIf) {
		std::cerr << "Expected #if: " << (const char*)Output_.toLocal8Bit() << ':' << m_Line <<
			".";
		return false;
		}
	//
	Result = processOperator(InputFile, Line, OutputFile, false);
	if(Result != TResult::OK) return false;
	}
}

// -----------------------------------------------------------------------
void TProcessor::valuesSubstitution(QString &Line_)
{
// Replacing variables
if(!Line_.isEmpty()) {
	int StartPos = 0;
	while((StartPos = m_VariableRegExp.indexIn(Line_, StartPos)) >= 0) {
		QString Variable = m_VariableRegExp.cap(1);
		int StringFoundSize = m_VariableRegExp.cap(0).size();
		TVariables::const_iterator it = m_Variables.find(Variable);
		if(it == m_Variables.end()) {
			StartPos += StringFoundSize;
			}
		else {
			Line_.replace(StartPos, StringFoundSize, it.value());
			StartPos += it.value().size();
			}
		}
	}
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::readNextLine(QFile &Input_, QString &Line_)
{
int Index;
while(true) {
	if(Input_.atEnd()) return TResult::EndOfFile;

	QByteArray ByteLine = Input_.readLine();
	m_Line++;
	Line_ = QString::fromUtf8(ByteLine);
	//
	Index = Line_.indexOf(m_NotWhitespaceRegExp);
	if(Index < 0 || Line_[Index] != '#') {
		valuesSubstitution(Line_);
		return TResult::OK;
		}
	if(m_CommentOperatorRegExp.indexIn(Line_, Index) == Index) continue;
	break;
	}
//
TResult Result;
int MatchedLen;
if(m_IfRegExp.indexIn(Line_, Index) == Index) {
	Result = TResult::OperatorIf;
	MatchedLen = m_IfRegExp.matchedLength();
	}
else if(m_ElifRegExp.indexIn(Line_, Index) == Index) {
	Result = TResult::OperatorElif;
	MatchedLen = m_ElifRegExp.matchedLength();
	}
else if(m_ElseRegExp.indexIn(Line_, Index) == Index) {
	Result = TResult::OperatorElse;
	MatchedLen = m_ElseRegExp.matchedLength();
	}
else if(m_EndifRegExp.indexIn(Line_, Index) == Index) {
	Result = TResult::OperatorEndif;
	MatchedLen = m_EndifRegExp.matchedLength();
	}
else {
	valuesSubstitution(Line_);
	return TResult::OK;
	}
//
Line_ = Line_.right(Line_.length() - Index - MatchedLen);
Line_ = Line_.trimmed();
while(Line_.endsWith('\\')) {
	if(Input_.atEnd()) {
		std::cerr << "Line " << m_Line << " ends with '\\' but next string is not present: " <<
			(const char*)Input_.fileName().toLocal8Bit() << ':' << m_Line;
		return TResult::SyntaxError;
		}
	QByteArray ByteLine = Input_.readLine();
	m_Line++;
	Line_ = Line_ + ' ' + QString::fromUtf8(ByteLine).trimmed();
	}
// Removing comments from operator's string
Index = Line_.indexOf(m_CommentRegExp);
if(Index >= 0) 
	Line_ = Line_.left(Index).trimmed();

if(Result == TResult::OperatorEndif || Result == TResult::OperatorElse) {
	if(!Line_.isEmpty()) {
		std::cerr << "Unexpected symbols after keyword: " <<
			(const char*)Input_.fileName().toLocal8Bit() << ':' << m_Line;
		return TResult::SyntaxError;
		}
	}
else if(Result == TResult::OperatorIf || Result == TResult::OperatorElif) {
	if(Line_.isEmpty()) {
		std::cerr << "Expression expected after keyword: " <<
			(const char*)Input_.fileName().toLocal8Bit() << ':' << m_Line;
		return TResult::SyntaxError;
		}
	}
return Result;
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::processLinesTillNextKeyword(QFile &Input_, QString &Line_, 
	QFile &Output_, bool Skip_) 
{
while(true) {
	TResult Result = readNextLine(Input_, Line_);
	if(Result == TResult::OK) {
		if(!Skip_) {
			QByteArray Converted = Line_.toUtf8();
			if(Output_.write(Converted) != Converted.size()) {
				std::cerr << "Error writing file '" << 
					(const char*)Output_.fileName().toLocal8Bit() << "'.";
				return TResult::WriteError;
				}
			}
		continue;
		}
	else if(Result == TResult::EndOfFile) return TResult::EndOfFile;
	else if(Result == TResult::SyntaxError) return TResult::SyntaxError;
	//
	assert(isOperator(Result));
	return Result;
	}
return TResult::OK;
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::processOperator(QFile &Input_, QString &Line_, QFile &Output_,
	bool Skip_)
{
bool ValidExpressionFound;
if(!calculateExp(Line_, ValidExpressionFound, Input_)) return TResult::SyntaxError;
//
bool ExpectiongEndifOnly = false;
TResult Result = processLinesTillNextKeyword(Input_, Line_, Output_, 
	Skip_? true: (!ValidExpressionFound));
while(true) {
	if(Result == TResult::EndOfFile) {
		std::cerr << "Expected #endif: " << (const char*)Input_.fileName().toLocal8Bit() <<
			':' << m_Line;
		return TResult::SyntaxError;
		}
	else if(Result == TResult::SyntaxError) return TResult::SyntaxError;
	else if(Result == TResult::WriteError) return TResult::WriteError;
		
	// Normal results
	else if(Result == TResult::OperatorIf) {
		Result = processOperator(Input_, Line_, Output_, Skip_? true: (!ValidExpressionFound));
		if(Result != TResult::OK) return Result;
		Result = processLinesTillNextKeyword(Input_, Line_, Output_, 
			Skip_? true: (!ValidExpressionFound));
		}
	else if(Result == TResult::OperatorElif) {
		if(ExpectiongEndifOnly) {
			std::cerr << "Unexpected #elif: " << (const char*)Input_.fileName().toLocal8Bit() <<
				':' << m_Line;
			return TResult::SyntaxError;
			}
		// Always check the expression to find errors
		bool ElifExpressionResult;
		if(!calculateExp(Line_, ElifExpressionResult, Input_)) return TResult::SyntaxError;
		//
		bool SkipThis = Skip_ || ValidExpressionFound;
		Result = processLinesTillNextKeyword(Input_, Line_, Output_, 
			SkipThis? true: (!ElifExpressionResult));
		//
		if(!ValidExpressionFound && ElifExpressionResult) ValidExpressionFound = true;
		assert(!ExpectiongEndifOnly);
		}
	else if(Result == TResult::OperatorElse) {
		if(ExpectiongEndifOnly) {
			std::cerr << "Unexpected #else: " << (const char*)Input_.fileName().toLocal8Bit() <<
				':' << m_Line;
			return TResult::SyntaxError;
			}
		Result = processLinesTillNextKeyword(Input_, Line_, Output_, 
			Skip_? true: ValidExpressionFound);
		//
		ExpectiongEndifOnly = true;
		}
	else if(Result == TResult::OperatorEndif) {
		return TResult::OK;
		}
	else {
		assert(!"Return value is not expected.");
		}
	}
}

// -----------------------------------------------------------------------
bool TProcessor::calculateExp(const QString &Line_, bool &Result_, const QFile &Input_)
{
struct TLexeme {
	TLexemeType Type;
	QString Value;
	};
//
enum class TValueType {
	Bool, String, Not, Eq, Neq, Less, Greater, LessOrEqual, GreaterOrEqual, And, Or,
	OpenBracket, CloseBracket
	};

// Memory cell of the VM
struct TVMValue {
	TValueType Type;
	QString StringValue;
	bool BoolValue;
	};

struct THelper {
	static bool lexAnalysis(TProcessor &This_, const QString &Line_, 
		QList<TLexeme> &Lexemes_) {
		int Index = 0;
		while(true) {
			// Skip to first non space
			Index = This_.m_NotWhitespaceRegExp.indexIn(Line_, Index);
			if(Index < 0) return true;
			//
			auto iMax = This_.m_LexemeRegExps.end();
			int MaxLen = 0;
			for(auto it = This_.m_LexemeRegExps.begin(); it != This_.m_LexemeRegExps.end(); 
				++it) {
				if(it->RegExp.indexIn(Line_, Index) != Index) continue;
				if(it->RegExp.matchedLength() > MaxLen) {
					MaxLen = it->RegExp.matchedLength();
					iMax = it;
					}
				}
			if(!MaxLen) return false;
			//
			Lexemes_.push_back(TLexeme());
			Lexemes_.back().Type = iMax->Type;
			if(iMax->Type == TLexemeType::String || iMax->Type == TLexemeType::Defined) {
				Lexemes_.back().Value = iMax->RegExp.cap(1);
				}
			else if(iMax->Type == TLexemeType::Variable) {
				Lexemes_.back().Value = iMax->RegExp.cap(0);
				}
			Index += iMax->RegExp.matchedLength();
			}
		}

	// ---
	#if defined(DEBUG_OUTPUT)
	static void printLexeme(const TLexeme &Lexeme_, std::ostream &Stream_) {
		Stream_ << "{";
		switch(Lexeme_.Type) {
		case TLexemeType::String:
			Stream_ << "string, " << (const char*)Lexeme_.Value.toLocal8Bit();
			break;
		case TLexemeType::Variable:
			Stream_ << "variable, " << (const char*)Lexeme_.Value.toLocal8Bit();
			break;
		case TLexemeType::Defined:
			Stream_ << "defined, " << (const char*)Lexeme_.Value.toLocal8Bit();
			break;
		case TLexemeType::Not:
			Stream_ << "!";
			break;
		case TLexemeType::Eq:
			Stream_ << "==";
			break;
		case TLexemeType::Neq:
			Stream_ << "!=";
			break;
		case TLexemeType::Less:
			Stream_ << "<";
			break;
		case TLexemeType::Greater:
			Stream_ << ">";
			break;
		case TLexemeType::LessOrEqual:
			Stream_ << "<=";
			break;
		case TLexemeType::GreaterOrEqual:
			Stream_ << ">=";
			break;
		case TLexemeType::OpenBracket:
			Stream_ << "(";
			break;
		case TLexemeType::CloseBracket:
			Stream_ << ")";
			break;
		case TLexemeType::And:
			Stream_ << "&&";
			break;
		case TLexemeType::Or:
			Stream_ << "||";
			break;
		default:
			Stream_ << "undefined";
			break;
			}
		Stream_ << "}";
		}
	#endif

	// ---
	static bool toVMValues(TProcessor &This_, const QList<TLexeme> &Lexemes_, 
		QList<TVMValue> &Values_) {
		assert(Values_.empty());
		for(auto it = Lexemes_.begin(); it != Lexemes_.end(); ++it) {
			Values_.push_back(TVMValue());
			TVMValue &Value = Values_.back();
			switch(it->Type) {
			case TLexemeType::String:
				Value.Type = TValueType::String;
				Value.StringValue = it->Value;
				break;

			case TLexemeType::Variable: {
				TVariables::const_iterator iVar = This_.m_Variables.find(it->Value);
				if(iVar == This_.m_Variables.end()) {
					std::cerr << "Undefined variable '" << (const char*)it->Value.toLocal8Bit() <<
						"'";
					return false;
					}
				Value.Type = TValueType::String;
				Value.StringValue = iVar.value();
				}
				break;

			case TLexemeType::Defined: {
				TVariables::const_iterator iVar = This_.m_Variables.find(it->Value);
				Value.Type = TValueType::Bool;
				Value.BoolValue = (iVar != This_.m_Variables.end());
				}
				break;

			case TLexemeType::Not:            Value.Type = TValueType::Not; break;
			case TLexemeType::Eq:             Value.Type = TValueType::Eq; break;
			case TLexemeType::Neq:            Value.Type = TValueType::Neq; break;
			case TLexemeType::Less:           Value.Type = TValueType::Less; break;
			case TLexemeType::Greater:        Value.Type = TValueType::Greater; break;
			case TLexemeType::LessOrEqual:    Value.Type = TValueType::LessOrEqual; break;
			case TLexemeType::GreaterOrEqual: Value.Type = TValueType::GreaterOrEqual; break;
			case TLexemeType::And:            Value.Type = TValueType::And; break;
			case TLexemeType::Or:             Value.Type = TValueType::Or; break;
			case TLexemeType::OpenBracket:    Value.Type = TValueType::OpenBracket; break;
			case TLexemeType::CloseBracket:   Value.Type = TValueType::CloseBracket; break;
			default:
				assert(!"Unknown lexeme type");
				}
			}
		return true;
		}

	// ---
	static int operationPrecedence(TValueType Type_) {
		switch(Type_) {
		case TValueType::And:
		case TValueType::Or:
			return 1;

		case TValueType::Neq:
		case TValueType::Less:
		case TValueType::Greater:
		case TValueType::LessOrEqual:
		case TValueType::GreaterOrEqual:
		case TValueType::Eq:   
			return 2;

		default:
			return 0;
			}
		}

	// ---
	static bool isValue(const TVMValue &Value_) {
		return Value_.Type == TValueType::String || Value_.Type == TValueType::Bool;
		}

	// ---
	static bool toBool(const TVMValue &Value_) {
		if(Value_.Type == TValueType::String)
			return !Value_.StringValue.isEmpty();
		else if(Value_.Type == TValueType::Bool) 
			return Value_.BoolValue;
		else {
			assert(!"Invalid type.");
			return false;
			}
		}

	// ---
	static bool toReversePolishNotation(QList<TVMValue> &Values_) {
		QList<TVMValue> Result;
		QStack<TVMValue> Stack;
		//
		for(auto it = Values_.begin(); it != Values_.end();) {
			if(it->Type == TValueType::String || it->Type == TValueType::Bool) {
				Result.push_back(*it);
				}
			else if(it->Type == TValueType::Not || it->Type == TValueType::OpenBracket) {
				Stack.push(*it);
				}
			else if(it->Type == TValueType::CloseBracket) {
				bool Found = false;
				while(!Stack.isEmpty()) {
					if(Stack.top().Type == TValueType::OpenBracket) {
						Found = true;
						Stack.pop();
						break;
						}
					else {
						Result.push_back(Stack.top());
						Stack.pop();
						}
					}
				if(!Found) {
					std::cerr << "Open bracket expected in expression";
					return false;
					}
				}
			else {
				if(!Stack.isEmpty()) {
					// while there is an operator at the top of the operator stack with
					// greater than or equal to precedence and the operator is left associative:
					// pop operators from the operator stack, onto the output queue.
					// push the read operator onto the operator stack.
					int Precedence = operationPrecedence(it->Type);
					while(operationPrecedence(Stack.top().Type) >= Precedence) {
						if(Stack.top().Type == TValueType::OpenBracket) {
							std::cerr << "Unpaired bracket";
							return false;
							}
						Result.push_back(Stack.top());
						Stack.pop();
						}
					}
				Stack.push(*it);
				}
			it = Values_.erase(it);
			}
		//
		while(!Stack.isEmpty()) {
			if(Stack.top().Type == TValueType::OpenBracket) {
				std::cerr << "Unpaired bracket";
				return false;
				}
			Result.push_back(Stack.top());
			Stack.pop();
			}
		Values_ = Result;
		return true;
		}

	#if defined(DEBUG_OUTPUT)
	static void printValue(const TVMValue Value_, std::ostream &Stream_) {
		switch(Value_.Type) {
		case TValueType::String:
			Stream_ << "'" << (const char*)Value_.StringValue.toLocal8Bit() << "'";
			break;
		case TValueType::Bool:
			Stream_ << (Value_.BoolValue? "true": "false");
			break;
		case TValueType::Not:
			Stream_ << "!";
			break;
		case TValueType::Eq:
			Stream_ << "==";
			break;
		case TValueType::Neq:
			Stream_ << "!=";
			break;
		case TValueType::Less:
			Stream_ << "<";
			break;
		case TValueType::Greater:
			Stream_ << ">";
			break;
		case TValueType::LessOrEqual:
			Stream_ << "<=";
			break;
		case TValueType::GreaterOrEqual:
			Stream_ << ">=";
			break;
		case TValueType::OpenBracket:
			Stream_ << "(";
			break;
		case TValueType::CloseBracket:
			Stream_ << ")";
			break;
		case TValueType::And:
			Stream_ << "&&";
			break;
		case TValueType::Or:
			Stream_ << "||";
			break;
		default:
			Stream_ << "undefined";
			break;
			}
		}
	#endif

	// ---
	static bool binaryOp(TValueType Operation_, QStack<TVMValue> &Stack_) {
		if(Stack_.size() < 2) return false;
		const TVMValue &Right = Stack_.top();
		TVMValue &Left = *(Stack_.end() - 2);
		//
		if(!isValue(Left) || !isValue(Right)) return false;
		//
		bool Result;
		if(Left.Type == TValueType::String && Right.Type == TValueType::String) {
			switch(Operation_) {
			case TValueType::And: 
				Result = toBool(Left) && toBool(Right); 
				break;
			case TValueType::Or:  
				Result = toBool(Left) || toBool(Right); 
				break;
			case TValueType::Eq:  
				Result = (Left.StringValue == Right.StringValue); 
				break;
			case TValueType::Neq: 
				Result = (Left.StringValue != Right.StringValue); 
				break;
			case TValueType::Less: 
				Result = (Left.StringValue < Right.StringValue); 
				break;
			case TValueType::Greater: 
				Result = (Left.StringValue > Right.StringValue); 
				break;
			case TValueType::LessOrEqual: 
				Result = (Left.StringValue <= Right.StringValue); 
				break;
			case TValueType::GreaterOrEqual:
				Result = (Left.StringValue >= Right.StringValue); 
				break;
			default:
				return false;
				}
			}
		else if(Left.Type == TValueType::Bool && Right.Type == TValueType::Bool) {
			switch(Operation_) {
			case TValueType::And: 
				Result = Left.BoolValue && Right.BoolValue;
				break;
			case TValueType::Or:  
				Result = Left.BoolValue || Right.BoolValue;
				break;
			case TValueType::Eq:  
				Result = (Left.BoolValue == Right.BoolValue); 
				break;
			case TValueType::Neq: 
				Result = (Left.BoolValue != Right.BoolValue); 
				break;
			default:
				return false;
				}
			}
		else {
			return false;
			}
		Left.BoolValue = Result;
		Left.Type = TValueType::Bool;
		Left.StringValue.clear();
		Stack_.pop();
		return true;
		}

	// ---
	static bool runVM(QList<TVMValue> &Values_, bool &Result_) {
		QStack<TVMValue> Stack;
		while(!Values_.isEmpty()) {
			const TVMValue &Front = Values_.front();
			switch(Front.Type) {
			case TValueType::String: case TValueType::Bool:
				Stack.push(Front);
				break;

			case TValueType::Not:
				if(Stack.isEmpty() || !(Stack.top().Type == TValueType::String || 
					Stack.top().Type == TValueType::Bool)) {
					return false;
					}

				if(Stack.top().Type == TValueType::String) {
					Stack.top().BoolValue = !toBool(Stack.top());
					Stack.top().StringValue.clear();
					Stack.top().Type = TValueType::Bool;
					}
				else {
					assert(Stack.top().Type == TValueType::Bool);
					Stack.top().BoolValue = !Stack.top().BoolValue;
					}
				break;

			default:
				if(!binaryOp(Front.Type, Stack)) return false;
				}
			Values_.pop_front();
			}
		if(Stack.empty() || !isValue(Stack.top())) return false;
		Result_ = toBool(Stack.top());
		return true;
		}

	};

//
QList<TLexeme> Lexemes;
if(!THelper::lexAnalysis(*this, Line_, Lexemes)) {
	std::cerr << "Error in expression: " << (const char*)Input_.fileName().toLocal8Bit() <<
		':' << m_Line;
	return false;
	}
if(Lexemes.empty()) {
	std::cerr << "Expression expected: " << (const char*)Input_.fileName().toLocal8Bit() <<
		':' << m_Line;
	return false;
	}

#if defined(DEBUG_OUTPUT)
	std::cout << "Lexemes: ";
	for(auto it = Lexemes.begin(); it != Lexemes.end(); ++it) {
		if(it != Lexemes.begin()) std::cout << ", ";
		THelper::printLexeme(*it, std::cout);
		}
	std::cout << "\n";
#endif

QList<TVMValue> Values;
if(!THelper::toVMValues(*this, Lexemes, Values) || 
	!THelper::toReversePolishNotation(Values)) {
	std::cerr << ": " << (const char*)Input_.fileName().toLocal8Bit() << ':' << m_Line;
	return false;
	}

if(Values.empty()) {
	std::cerr << "Invalid expression: " << (const char*)Input_.fileName().toLocal8Bit() <<
		':' << m_Line;
	return false;
	}

#if defined(DEBUG_OUTPUT)
	std::cout << "Code: ";
	for(auto it = Values.begin(); it != Values.end(); ++it) {
		if(it != Values.begin()) std::cout << ", ";
		THelper::printValue(*it, std::cout);
		}
	std::cout << "\n";
#endif

if(!THelper::runVM(Values, Result_)) {
	std::cerr << "Invalid expression: " << (const char*)Input_.fileName().toLocal8Bit() <<
		':' << m_Line;
	return false;
	}

#if defined(DEBUG_OUTPUT)
	std::cout << "Result: " << (Result_? "true": "false") << "\n";
#endif

return true;
}