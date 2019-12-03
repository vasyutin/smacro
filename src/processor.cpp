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
#include "string-stream.h"

#include <iostream>
#include <cctype>

#include <assert.h>

#if !defined(SMACRO_WINDOWS)
	#include <stddef.h>
#endif

//#define DEBUG_OUTPUT

// -----------------------------------------------------------------------
TProcessor::TProcessor(const TParameters &Parameters_): 
	m_Variables(Parameters_.Variables),
	m_ExcludePatterns(Parameters_.ExcludePatterns),
	m_IgnorePatterns(Parameters_.IgnorePatterns),
	m_VariableRegExp("[$][{]([A-Za-z0-9_]+)[}]"),
	m_IfRegExp("#if\\s+"), 
	m_ElifRegExp("#elif\\s+"), 
	m_ElseRegExp("#else"),
	m_EndifRegExp("#endif"), 
	m_CommentOperatorRegExp("#[/][/]"),
	m_CommentRegExp("[/][/]"),
	m_IncludeRegExp("#include\\s*[<]([^>]*)[>]")
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
bool TProcessor::matchesPatterns(const TFileNameString &FileName_, const TExcludePatterns &Patterns_)
{
for(auto p_it = Patterns_.begin(); p_it != Patterns_.end(); ++p_it) {
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
TProcessor::TProcessData::TProcessData(const TFileNameString &InputFile_, 
	const TFileNameString &OutputFile_):
	OutputFile(OutputFile_)
{
assert(Input.empty());
std::unique_ptr<std::ifstream> Stream(new std::ifstream);
#if defined(__MINGW32__) || defined(__MINGW64__)
	Stream->open(WStringToWindowsLocal(InputFile_), std::ios::binary);
#else
	Stream->open(InputFile_, std::ios::binary);
#endif
if(!(*Stream)) {
	ErrorMessage = "Can't open input file: '";
	ErrorMessage += FileNameStringToConsole(InputFile_);
	ErrorMessage += "'.";
	return;
	}
Input.push_back(std::move(Stream));
InputFiles.push_back(InputFile_);
CurrentLines.push_back(0);

#if defined(__MINGW32__) || defined(__MINGW64__)
	Output.open(WStringToWindowsLocal(OutputFile_), std::ios::binary);
#else
	Output.open(OutputFile_, std::ios::binary);
#endif
if(!Output) {
	ErrorMessage = "Can't open output file: '";
	ErrorMessage += FileNameStringToConsole(OutputFile_);
	ErrorMessage += "'.";
	return;
	}
}

// -----------------------------------------------------------------------
bool TProcessor::TProcessData::initialized() const
{
return !Input.empty() && Input.back()->is_open() && Output.is_open();
}

// -----------------------------------------------------------------------
bool TProcessor::processFile(const TFileNameString &Input_, const TFileNameString &Output_)
{
TProcessData Data(Input_, Output_);
if(!Data.initialized()) {
	std::cerr << Data.errorMessage();
	return false;
	}
//
std::string Line;
while(true) {
	TResult Result = readNextLine(Data, Line, true);
	if(Result == TResult::OK) {
		if(!Data.Output.write(Line.c_str(), Line.size())) {
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
		std::cerr << "Expected #if: " << FileNameStringToConsole(Input_) << ':' << Data.lineNo() <<
			".";
		return false;
		}
	//
	Result = processOperator(Data, Line, false);
	if(Result == TResult::WriteError) {
		std::cerr << "Can't write file: '" << FileNameStringToConsole(Output_) << "'.";
		return false;
		}
	else if(Result != TResult::OK) {
		std::cerr << Data.errorMessage();
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
std::string::const_iterator TProcessor::firstNonSpace(std::string::const_iterator Begin_, 
		std::string::const_iterator End_)
{
return std::find_if_not(Begin_, End_, [](std::string::value_type Ch_) {return std::isspace((int)Ch_);});
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::readNextLine(TProcessData &Data_, std::string &Line_, bool DoProcessing_)
{
assert(!Data_.Input.empty());
std::string::const_iterator Index;
while(true) {
	if(!std::getline(*Data_.Input.back(), Line_)) {
		// Don close the main file
		if(Data_.Input.size() <= 1) return TResult::EndOfFile;
		//
		Data_.Input.pop_back();
		Data_.InputFiles.pop_back();
		Data_.CurrentLines.pop_back();
		continue;
		}
	// Check if line starts with #
	Index = firstNonSpace(Line_.cbegin(), Line_.cend());
	if(Index == Line_.cend() || *Index != '#') {
		valuesSubstitution(Line_);
		return TResult::OK;
		}
	std::smatch Match;
	if(std::regex_search(Index, Line_.cend(), Match, m_CommentOperatorRegExp) &&
		!Match.position()) {
		continue;
		}
	// Substitute values
	if(DoProcessing_) {
		valuesSubstitution(Line_);

		// Include directive
		if(std::regex_search(Index, Line_.cend(), Match, m_IncludeRegExp) && !Match.position()) {
			if(!Match[1].length()) return TResult::SyntaxError;

			std::unique_ptr<std::ifstream> Stream(new std::ifstream);
			#if defined(_MSC_VER)
				TFileNameString FileName(Utf8ToFileNameString(std::string(Match[1].first, Match[1].second)));
				Stream->open(FileName, std::ios::binary);
			#elif defined(__MINGW32__) || defined(__MINGW64__)
				TFileNameString FileName(Utf8ToFileNameString(std::string(Match[1].first, Match[1].second)));
				Stream->open(WStringToWindowsLocal(FileName), std::ios::binary);
			#else
				std::string FileName(Match[1].first, Match[1].second);
				Stream->open(FileName, std::ios::binary);
			#endif
			if(!(*Stream)) {
				Data_.ErrorMessage  << "Can't include file '" << FileNameStringToConsole(FileName) << "': " <<
					FileNameStringToConsole(Data_.inputFile()) << ':' << std::to_string(Data_.lineNo());
				return TResult::SyntaxError;
				}
			Data_.Input.push_back(std::move(Stream));
			Data_.InputFiles.push_back(FileName);
			Data_.CurrentLines.push_back(0);
			continue;
			}
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
	return TResult::OK;
	}
ptrdiff_t MatchedLen = Match.length();
Line_.erase(Line_.begin(), Index + MatchedLen);
TrimString(Line_);

while(!Line_.empty() && *(Line_.end() - 1) == '\\') {
	std::string NewLine;
	if(!std::getline(*Data_.Input.back(), NewLine)) {
		Data_.ErrorMessage  << "Line ends with '\\' but next string is not present: " << 
			FileNameStringToConsole(Data_.inputFile()) << ':' << std::to_string(Data_.lineNo());
		return TResult::SyntaxError;
		}
	(Data_.CurrentLines.back())++;
	TrimString(NewLine);
	// Removing '\\'
	*(Line_.end() - 1) = ' ';
	Line_ += NewLine;
	}
// Removing comments from operator's string
if(std::regex_search(Line_.cbegin(), Line_.cend(), Match, m_CommentRegExp)) {
	Line_.erase(Line_.begin() + Match.position(), Line_.end());
	TrimString(Line_);
	}
if(Result == TResult::OperatorEndif || Result == TResult::OperatorElse) {
	if(!Line_.empty()) {
		Data_.ErrorMessage << "Unexpected symbols after keyword: " <<
			FileNameStringToConsole(Data_.inputFile()) << ':' << Data_.lineNo();
		return TResult::SyntaxError;
		}
	}
else if(Result == TResult::OperatorIf || Result == TResult::OperatorElif) {
	if(Line_.empty()) {
		Data_.ErrorMessage << "Expression expected after keyword: " << 
			FileNameStringToConsole(Data_.inputFile()) << ':' << Data_.lineNo();
		return TResult::SyntaxError;
		}
	}
return Result;
}

// -----------------------------------------------------------------------
TProcessor::TResult TProcessor::processLinesTillNextKeyword(TProcessData &Data_, 
	std::string &Line_, bool Skip_) 
{
while(true) {
	TResult Result = readNextLine(Data_, Line_, !Skip_);
	if(Result == TResult::OK) {
		if(!Skip_) {
			if(!Data_.Output.write(Line_.c_str(), Line_.size())) {
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
TProcessor::TResult TProcessor::processOperator(TProcessData &Data_, std::string &Line_, 
	bool Skip_)
{
bool ValidExpressionFound;
if(!calculateExp(Line_, ValidExpressionFound, Data_)) return TResult::SyntaxError;
//
bool ExpectingEndifOnly = false;
TResult Result = processLinesTillNextKeyword(Data_, Line_, Skip_? true: (!ValidExpressionFound));
while(true) {
	if(Result == TResult::EndOfFile) {
		Data_.ErrorMessage << "Expected #endif: " << FileNameStringToConsole(Data_.inputFile()) << 
			':' << Data_.lineNo();
		return TResult::SyntaxError;
		}
	else if(Result == TResult::SyntaxError) return TResult::SyntaxError;
	else if(Result == TResult::WriteError) return TResult::WriteError;
		
	// Normal results
	else if(Result == TResult::OperatorIf) {
		Result = processOperator(Data_, Line_, Skip_? true: (!ValidExpressionFound));
		if(Result != TResult::OK) return Result;
		Result = processLinesTillNextKeyword(Data_, Line_, Skip_? true: (!ValidExpressionFound));
		}
	else if(Result == TResult::OperatorElif) {
		if(ExpectingEndifOnly) {
			Data_.ErrorMessage << "Unexpected #elif: " << FileNameStringToConsole(Data_.inputFile()) 
				<< ':' << Data_.lineNo();
			return TResult::SyntaxError;
			}
		// Always check the expression to find errors
		bool ElifExpressionResult;
		if(!calculateExp(Line_, ElifExpressionResult, Data_)) return TResult::SyntaxError;
		//
		bool SkipThis = Skip_ || ValidExpressionFound;
		Result = processLinesTillNextKeyword(Data_, Line_, SkipThis? true: (!ElifExpressionResult));
		//
		if(!ValidExpressionFound && ElifExpressionResult) ValidExpressionFound = true;
		assert(!ExpectingEndifOnly);
		}
	else if(Result == TResult::OperatorElse) {
		if(ExpectingEndifOnly) {
			Data_.ErrorMessage << "Unexpected #else: " << FileNameStringToConsole(Data_.inputFile()) 
				<< ':' << Data_.lineNo();
			return TResult::SyntaxError;
			}
		Result = processLinesTillNextKeyword(Data_, Line_, Skip_? true: ValidExpressionFound);
		//
		ExpectingEndifOnly = true;
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
bool TProcessor::calculateExp(const std::string &Line_, bool &Result_, TProcessData &Data_)
{
struct TLexeme {
	TLexemeType Type;
	std::string Value;
	};
//
enum class TValueType {
	Bool, String, Not, Eq, Neq, Less, Greater, LessOrEqual, GreaterOrEqual, And, Or,
	OpenBracket, CloseBracket
	};

// Memory cell of the VM
struct TVMValue {
	TValueType Type;
	std::string StringValue;
	bool BoolValue;
	};

// We need direct access to stack's container, thus we have to make own stack
struct TVMVStack: public std::vector<TVMValue> {
public:
	void push(const TVMValue &Value_) {push_back(Value_);}
	void pop() {pop_back();}
	TVMValue &top() {return back();}
	};

// ---
struct THelper {
	static bool lexAnalysis(TProcessor &This_, const std::string &Line_, 
		std::vector<TLexeme> &Lexemes_) {
		std::string::const_iterator Index = Line_.cbegin();
		while(true) {
			// Skip to first non space
			Index = firstNonSpace(Index, Line_.cend());
			if(Index == Line_.cend()) return true;
			//
			auto iMax = This_.m_LexemeRegExps.end();
			ptrdiff_t MaxLen = 0;
			std::smatch Match;
			std::string Lexeme;
			for(auto it = This_.m_LexemeRegExps.begin(); it != This_.m_LexemeRegExps.end(); 
				++it) {
				if(!std::regex_search(Index, Line_.cend(), Match, it->RegExp) ||
					Match.position()) continue;
				if(Match.length() > MaxLen) {
					MaxLen = Match.length();
					if(it->Type == TLexemeType::String || it->Type == TLexemeType::Defined) {
						Lexeme = Match[1];
						}
					else if(it->Type == TLexemeType::Variable) {
						Lexeme = Match[0];
						}
					else {
						Lexeme.clear();
						}
					iMax = it;
					}
				}
			if(!MaxLen) return false;
			//
			Lexemes_.push_back(TLexeme());
			Lexemes_.back().Type = iMax->Type;
			Lexemes_.back().Value.swap(Lexeme);
			Index += MaxLen;
			}
		}

	// ---
	#if defined(DEBUG_OUTPUT)
	static void printLexeme(const TLexeme &Lexeme_, std::ostream &Stream_) {
		Stream_ << "{";
		switch(Lexeme_.Type) {
		case TLexemeType::String:
			Stream_ << "string, " << Lexeme_.Value;
			break;
		case TLexemeType::Variable:
			Stream_ << "variable, " << Lexeme_.Value;
			break;
		case TLexemeType::Defined:
			Stream_ << "defined, " << Lexeme_.Value;
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
	static bool toVMValues(TProcessor &This_, const std::vector<TLexeme> &Lexemes_, 
		std::vector<TVMValue> &Values_, TProcessData &Data_) {
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
					Data_.ErrorMessage << "Undefined variable '" << 
						FileNameStringToConsole(Data_.inputFile()) << ':' << Data_.lineNo();
					return false;
					}
				Value.Type = TValueType::String;
				Value.StringValue = iVar->second;
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
			return !Value_.StringValue.empty();
		else if(Value_.Type == TValueType::Bool) 
			return Value_.BoolValue;
		else {
			assert(!"Invalid type.");
			return false;
			}
		}

	// ---
	static bool toReversePolishNotation(std::vector<TVMValue> &Values_, TProcessData &Data_) {
		std::vector<TVMValue> Result;
		TVMVStack Stack;
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
				while(!Stack.empty()) {
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
					Data_.ErrorMessage << "Open bracket expected in expression" << 
						FileNameStringToConsole(Data_.inputFile()) << ':' << Data_.lineNo();
					return false;
					}
				}
			else {
				if(!Stack.empty()) {
					// while there is an operator at the top of the operator stack with
					// greater than or equal to precedence and the operator is left associative:
					// pop operators from the operator stack, onto the output queue.
					// push the read operator onto the operator stack.
					int Precedence = operationPrecedence(it->Type);
					while(operationPrecedence(Stack.top().Type) >= Precedence) {
						if(Stack.top().Type == TValueType::OpenBracket) {
							Data_.ErrorMessage << "Unpaired bracket" << 
								FileNameStringToConsole(Data_.inputFile()) << ':' << Data_.lineNo();
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
		while(!Stack.empty()) {
			if(Stack.top().Type == TValueType::OpenBracket) {
				Data_.ErrorMessage << "Unpaired bracket" << 
					FileNameStringToConsole(Data_.inputFile()) << ':' << Data_.lineNo();
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
			Stream_ << "'" << Value_.StringValue << "'";
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
	static bool binaryOp(TValueType Operation_, TVMVStack &Stack_) {
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
	static bool runVM(std::vector<TVMValue> &Values_, bool &Result_) {
		TVMVStack Stack;
		while(!Values_.empty()) {
			const TVMValue &Front = Values_.front();
			switch(Front.Type) {
			case TValueType::String: case TValueType::Bool:
				Stack.push(Front);
				break;

			case TValueType::Not:
				if(Stack.empty() || !(Stack.top().Type == TValueType::String || 
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
			Values_.erase(Values_.begin());
			}
		if(Stack.empty() || !isValue(Stack.top())) return false;
		Result_ = toBool(Stack.top());
		return true;
		}

	};

//
std::vector<TLexeme> Lexemes;
if(!THelper::lexAnalysis(*this, Line_, Lexemes)) {
	Data_.ErrorMessage << "Error in expression: " << FileNameStringToConsole(Data_.inputFile()) << 
		':' << Data_.lineNo();
	return false;
	}
if(Lexemes.empty()) {
	Data_.ErrorMessage << "Expression expected: " << FileNameStringToConsole(Data_.inputFile()) << 
		':' << Data_.lineNo();
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

std::vector<TVMValue> Values;
if(!THelper::toVMValues(*this, Lexemes, Values, Data_) || 
	!THelper::toReversePolishNotation(Values, Data_)) {
	return false;
	}

if(Values.empty()) {
	Data_.ErrorMessage << "Invalid expression: " << FileNameStringToConsole(Data_.inputFile()) << 
		':' << Data_.lineNo();
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
	Data_.ErrorMessage << "Invalid expression: " << FileNameStringToConsole(Data_.inputFile()) << 
		':' << Data_.lineNo();
	return false;
	}

#if defined(DEBUG_OUTPUT)
	std::cout << "Result: " << (Result_? "true": "false") << "\n";
#endif

return true;
}