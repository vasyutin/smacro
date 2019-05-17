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
#include "utils.h"

#include <iostream>
#include <cctype>

#include <assert.h>

#define DEBUG_OUTPUT

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
m_LexemeRegExps.back().RegExp.assign("[\"]([^\"]*)[\"]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Variable;
m_LexemeRegExps.back().RegExp.assign("[A-Za-z_][A-Za-z_0-9]*");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Defined;
m_LexemeRegExps.back().RegExp.assign("defined\\s*[(]\\s*([A-Za-z_][A-Za-z_0-9]*)\\s*[)]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Not;
m_LexemeRegExps.back().RegExp.assign("!");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Eq;
m_LexemeRegExps.back().RegExp.assign("==");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Neq;
m_LexemeRegExps.back().RegExp.assign("!=");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Less;
m_LexemeRegExps.back().RegExp.assign("<");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Greater;
m_LexemeRegExps.back().RegExp.assign(">");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::LessOrEqual;
m_LexemeRegExps.back().RegExp.assign("<=");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::GreaterOrEqual;
m_LexemeRegExps.back().RegExp.assign(">");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::OpenBracket;
m_LexemeRegExps.back().RegExp.assign("[(]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::CloseBracket;
m_LexemeRegExps.back().RegExp.assign("[)]");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::And;
m_LexemeRegExps.back().RegExp.assign("&&");

m_LexemeRegExps.push_back(TLexemeRegExp());
m_LexemeRegExps.back().Type = TLexemeType::Or;
m_LexemeRegExps.back().RegExp.assign("||");
}

// -----------------------------------------------------------------------
bool TProcessor::isExcluded(const TFileNameString &FileName_) const
{
for(auto p_it = m_ExcludePatterns.begin(); p_it != m_ExcludePatterns.end(); ++p_it) {
	if(std::regex_match(FileName_, *p_it)) return true;
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
bool TProcessor::processFile(const TFileNameString &Input_, const TFileNameString &Output_)
{
#if defined(__MINGW32__) || defined(__MINGW64__)
	std::string Local8InputFileName = WStringToWindowsLocal(Input_);
	std::ifstream InputFile(Local8InputFileName);
#else
	std::ifstream InputFile(Input_);
#endif
if(!InputFile) {
	std::cerr << "Can't open input file: '" << FileNameStringToConsole(Input_) << "'.";
	return false;
	}

#if defined(__MINGW32__) || defined(__MINGW64__)
	std::string Local8OutputFileName = WStringToWindowsLocal(Output_);
	std::ofstream OutputFile(Local8OutputFileName);
#else
	std::ofstream OutputFile(Output_);
#endif
if(!OutputFile) {
	std::cerr << "Can't open output file: '" << FileNameStringToConsole(Output_) << "'.";
	return false;
	}
//
m_Line = 0;
std::string Line;
while(true) {
	TResult Result = readNextLine(InputFile, Line);
	if(Result == TResult::OK) {
		if(!OutputFile.write(Line.c_str(), Line.size())) {
			std::cerr << "Error writing file '" << FileNameStringToConsole(Output_) << "'.";
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
		std::cerr << "Expected #if: " << FileNameStringToConsole(Input_) << ':' << m_Line <<
			".";
		return false;
		}
	//
	Result = processOperator(InputFile, Line, OutputFile, false);
	if(Result == TResult::WriteError) {
		std::cerr << "Can't write file: '" << FileNameStringToConsole(Output_) << "'.";
		return false;
		}
	else if(Result != TResult::OK) {
		// The error message is provided by the callee.
		return false;
		}
	}
}

// -----------------------------------------------------------------------
void TProcessor::valuesSubstitution(std::string &Line_)
{
// Replacing variables
if(!Line_.empty()) {
	size_t StartPos = 0;
	std::cmatch Match;
	while(std::regex_search(Line_.c_str() + StartPos, Match, m_VariableRegExp)) {
		std::string Variable(Match[1].first, Match[1].second);
		TVariables::const_iterator it = m_Variables.find(Variable);
		if(it == m_Variables.end()) {
			StartPos += Variable.size();
			}
		else {
			Line_.replace(Match[0].first - Line_.c_str(), Match[0].second - Match[0].first, 
				it->second);
			StartPos += it->second.size();
			}
		}
	}
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::readNextLine(std::istream &Input_, std::string &Line_, 
	const TFileNameString &Output_)
{
std::string::const_iterator Index;
while(true) {
	if(!std::getline(Input_, Line_)) return TResult::EndOfFile;
	// Check if line starts with #
	Index = std::find_if_not(Line_.cbegin(), Line_.cend(),
		[](char Ch_) {return std::isspace((int)Ch_);}
		);
	if(Index == Line_.cend() || *Index != '#') {
		valuesSubstitution(Line_);
		return TResult::OK;
		}
	std::smatch Match;
	if(std::regex_search(Index, Line_.cend(), Match, m_CommentOperatorRegExp) &&
		!Match.position()) {
		continue;
		}
	break;
	}
//
TResult Result;
std::smatch Match;
if(std::regex_search(Index, Line_.cend(), Match, m_IfRegExp) && !Match.position()) {
	Result = TResult::OperatorIf;
	}
else if(std::regex_search(Index, Line_.cend(), Match, m_ElifRegExp) && !Match.position()) {
	Result = TResult::OperatorElif;
	}
else if(std::regex_search(Index, Line_.cend(), Match, m_ElseRegExp) && !Match.position()) {
	Result = TResult::OperatorElse;
	}
else if(std::regex_search(Index, Line_.cend(), Match, m_EndifRegExp) && !Match.position()) {
	Result = TResult::OperatorEndif;
	}
else {
	valuesSubstitution(Line_);
	return TResult::OK;
	}
ptrdiff_t MatchedLen = Match[0].length();
Line_.erase(Index + MatchedLen, Line_.end());
TrimString(Line_);
assert(!Line_.empty());
while(*(Line_.end() - 1) == '\\') {
	std::string NewLine;
	if(!std::getline(Input_, NewLine)) {
		std::cerr << "Line " << m_Line << " ends with '\\' but next string is not present: " <<
			FileNameStringToConsole(Output_) << ':' << m_Line;
		return TResult::SyntaxError;
		}
	m_Line++;
	TrimString(NewLine);
	// Removing '\\'
	*(Line_.end() - 1) = ' ';
	Line_ += NewLine;
	}
// Removing comments from operator's string
if(std::regex_search(Line_.cbegin(), Line_.cend(), Match, m_CommentRegExp)) {
	Line_.erase(Line_.begin() + Match.position(), Line_.end());
	TrimStringRight(Line_);
	}

??

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
*/
return Result;
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::processLinesTillNextKeyword(std::istream &Input_, 
	std::string &Line_, std::ostream &Output_, bool Skip_) 
{
while(true) {
	TResult Result = readNextLine(Input_, Line_);
	if(Result == TResult::OK) {
		if(!Skip_) {
			if(!Output_.write(Line_.c_str(), Line_.size())) {
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
TProcessor::TResult TProcessor::processOperator(std::istream &Input_, std::string &Line_, 
	std::ostream &Output_, bool Skip_)
{
return TResult::OK;


/*
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
*/
}

// -----------------------------------------------------------------------
bool TProcessor::calculateExp(const std::string &Line_, bool &Result_, const std::string &Input_)
{
/*
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

*/
return true;
}