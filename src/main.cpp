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
#include "processor.h"
#include "utils.h"

#include <iostream>
#include <set>

#include <assert.h>

const int RETCODE_OK = 0;
const int RETCODE_INVALID_PARAMETERS = 1;
const int RETCODE_PROCESS_ERROR = 1;

// -----------------------------------------------------------------------
void Usage(void)
{
std::cout << "SMACRO - simple macro processor. Helps prepare documentation.\n"
"Written by Sergey Vasyutin (https://github.com/vasyutin/smacro).\n"
"\n"
"Usage: smacro <switches>\n"
"\n"
"The switches are:\n"
" -i<input folder> - the folder, containing documentation files to process\n"
" -o<output folder> - the destination folder for the processed files\n"
" -v<variables file> - the file, containing values of the variables for the current run.\n"
"    The text in the file is assumed to be in UTF-8\n"
" -e<masks> - The masks of filenames to exclude from processing. This files are only\n"
"    copied to the output folder. The masks are separated by commas.\n"
" -d<masks> - The masks of filenames to ignore. This files are not copied to the output\n"
"    folder. The masks are separated by commas.\n"
"\n"
"All the files being processed (except the files excluded from the processing with the -e\n"
"switch) are assumed to be in UTF-8 encoding.\n\n"
"Example:\n"

#if defined(SMACRO_WINDOWS)
" smacro -i..\\..\\example\\source -o..\\..\\build\\doc_res "
"-v..\\..\\example\\config -e*.txt,*.png\n";
#else
" smacro -i../../example/source -o../../build/doc_res "
"-v../../example/config -e*.txt,*.png\n";
#endif
}

// -----------------------------------------------------------------------
inline size_t FileNameStringLength(const TFileNameChar *String_) 
{
#if defined(SMACRO_WINDOWS)
	return wcslen(String_);
#else
	return strlen(String_);
#endif
}

// -----------------------------------------------------------------------
bool ParseVariables(const TFileNameChar *FileName_, TParameters &Parameters_)
{
struct TProcessor {
	TParameters &m_Parameters;
	std::regex m_VariableWithValue;
	std::regex m_VariableWithoutValue;

	//
	TProcessor(TParameters &Parameters_): 
		m_Parameters(Parameters_),
		m_VariableWithValue("([A-Za-z0-9_]+)\\s*=(.+)"), 
		m_VariableWithoutValue("[A-Za-z0-9_]+") {}

	//
	bool processString(const std::string &WLine_) {
		std::smatch Match;
		if(std::regex_match(WLine_, Match, m_VariableWithValue)) {
			m_Parameters.Variables.insert(std::make_pair(Match[1], Match[2]));
			return true;
			}
		if(std::regex_match(WLine_, Match, m_VariableWithoutValue)) {
			m_Parameters.Variables.insert(std::make_pair(Match[0], std::string()));
			return true;
			}
		return false;
		}
	};

// ---
#if defined(__MINGW32__) || defined(__MINGW64__)
	std::string Local8FileName = WStringToWindowsLocal(FileName_);
	std::ifstream File(Local8FileName);
#else
	std::ifstream File(FileName_);
#endif
if(!File) {
	std::cerr << "Can't open file: '" << FileNameStringToConsole(FileName_) << "'.";
	return false;
	}
//
std::string Line;
TProcessor Processor(Parameters_);
while(std::getline(File, Line)) {
	// Удаляем пробелы в начале и конце строки
	TrimString(Line);
	if(Line.empty() || Line[0] == '#') continue;
	if(!Processor.processString(Line)) 
		return false;
	}
return true;
}

// -----------------------------------------------------------------------
template <typename _TString>
void WildcardToRegexp(_TString &String_)
{
typedef typename _TString::value_type TChar;

struct THelper {
	static inline bool isAlphaOrDigit(char Char_) {return isalpha(Char_) || isdigit(Char_);}
	#if defined(SMACRO_WINDOWS)
		static inline bool isAlphaOrDigit(wchar_t Char_) {
			return iswalpha(Char_) || iswdigit(Char_);
			}
	#endif
	};

struct TWildcardReplacementItem {
	TChar Source;
	TChar Replacement[2];
	};

static const TWildcardReplacementItem Replacement[] = {
	{TChar('$'), {TChar('\\'), TChar('$')}},
	{TChar('('), {TChar('\\'), TChar('(')}},
	{TChar(')'), {TChar('\\'), TChar(')')}},
	{TChar('*'), {TChar('.'), TChar('*')}},
	{TChar('+'), {TChar('\\'), TChar('+')}},
	{TChar('.'), {TChar('\\'), TChar('.')}},
	{TChar('['), {TChar('\\'), TChar('[')}},
	{TChar('\\'), {TChar('\\'), TChar('\\')}},
	{TChar(']'), {TChar('\\'), TChar(']')}},
	{TChar('^'), {TChar('\\'), TChar('^')}},
	{TChar('{'), {TChar('\\'), TChar('{')}},
	{TChar('|'), {TChar('\\'), TChar('|')}},
	{TChar('}'), {TChar('\\'), TChar('}')}}
	};

// ---
const TWildcardReplacementItem *ReplacementEnd = 
	Replacement + (sizeof(Replacement) / sizeof(TWildcardReplacementItem));

for(auto it = String_.begin(); it != String_.end(); ++it) {
	if(THelper::isAlphaOrDigit(*it)) continue;
	if(*it == TChar('?')) {
		*it = TChar('.');
		continue;
		}
	for(auto ReplIt = Replacement; ReplIt != ReplacementEnd; ++ReplIt) {
		if(ReplIt->Source == *it) {
			*it = ReplIt->Replacement[0];
			it = String_.insert(it + 1, ReplIt->Replacement[1]);
			break;
			}
		}
	}
} 

// -----------------------------------------------------------------------
bool ParseMasks(const TFileNameChar *Masks_, TExcludePatterns &ExcludePatterns_)
{
std::set<TFileNameString> Patterns;

const TFileNameChar *End = Masks_ + FileNameStringLength(Masks_);
while(true) {
	const TFileNameChar *Delim = std::find(Masks_, End, TFileNameChar(','));
	if(Delim == Masks_) {
		Masks_++;
		continue;
		}
	//
	TFileNameString Value(Masks_, Delim);
	TrimString(Value);
	if(Value.empty()) continue;
	//
	if(Patterns.find(Value) != Patterns.end()) continue;
	Patterns.insert(Value);
	WildcardToRegexp(Value);
	try {
		ExcludePatterns_.push_back(std::move(TExcludePatterns::value_type(Value)));
		}
	catch(std::regex_error&) {
		std::cerr << "Invalid pattern: '" << FileNameStringToConsole(Value) << "'.";
		return false;
		}
	//
	if(Delim == End) break;
	Masks_ = Delim + 1;
	}
return true;
}

// -----------------------------------------------------------------------
bool ParseParameters(int Argc_, const TFileNameChar **Argv_, TParameters &Parameters_)
{
for(int i = 1; i < Argc_; ++i) {
	const TFileNameChar *Argument = Argv_[i];
	size_t ArgLen = FileNameStringLength(Argument);
	if(ArgLen < 2 || Argument[0] != TFileNameChar('-')) {
		std::cerr << "Invalid argument: '" << FileNameStringToConsole(Argument) << "'.";
		return false;
		}
	//
	switch(Argument[1]) {
	case TFileNameChar('i'):
		Parameters_.InputFolder = Argument + 2;
		break;
	case TFileNameChar('o'):
		Parameters_.OutputFolder = Argument + 2;
		break;
	case TFileNameChar('v'):
		if(!ParseVariables(Argument + 2, Parameters_)) return false;
		break;
	case TFileNameChar('e'):
		if(!ParseMasks(Argument + 2, Parameters_.ExcludePatterns)) {
			// The error message is provided by the callee.
			return false;
			}
		break;
	case TFileNameChar('d'):
		if(!ParseMasks(Argument + 2, Parameters_.IgnorePatterns)) {
			// The error message is provided by the callee.
			return false;
			}
		break;
	default:
		std::cerr << "Invalid switch: '" << FileNameStringToConsole(Argument) << "'.";
		return false;
		} // switch
	}
	
if(Parameters_.InputFolder.empty() || Parameters_.OutputFolder.empty()) {
	std::cerr << "No input or output folder was specified.";
	return false;
	}
return true;	
}

// -----------------------------------------------------------------------
bool ProcessFolder(const TFileNameString &Input_, const TFileNameString &Output_, 
	TProcessor &Processor_)
{
if(!FolderExists(Input_.c_str())) {
	std::cerr << "Error accessing folder '" << FileNameStringToConsole(Input_) << "'.";
	return false;
	}
if(!FolderExists(Output_.c_str())) {
	if(!MakePath(Output_.c_str())) {
		std::cerr << "Can't create folder '" << FileNameStringToConsole(Output_) << "'.";
		return false;
		}
	}

std::vector<TFileNameString> Folders, Files;
if(!FolderEntries(Input_.c_str(), Folders, Files)) {
	std::cerr << "Can't get contents of the folder '" << FileNameStringToConsole(Input_) << "'.";
	return false;
	}
for(auto it = Files.begin(); it != Files.end(); ++it) {
	TFileNameString InputFile(Input_ + *it), OutputFile(Output_ + *it);
	if(FileExists(OutputFile.c_str())) {
		if(!RemoveFile(OutputFile.c_str())) {
			std::cerr << "Can't delete file '" << FileNameStringToConsole(OutputFile) << "'.";
			return false;
			}
		}
	if(Processor_.isIgnored(*it)) continue;

	bool Result = Processor_.isExcluded(*it)? 
		DuplicateFile(InputFile.c_str(), OutputFile.c_str()):
		Processor_.processFile(InputFile, OutputFile);
	//
	if(!Result) return false; /*{
		std::cerr << "Can't write file '" << FileNameStringToConsole(OutputFile) << "'.";
		return false;
		}*/
	}

for(auto it = Folders.begin(); it != Folders.end(); ++it) {
	if(!ProcessFolder(Input_ + *it + DIR_SEPARATOR, Output_ + *it + DIR_SEPARATOR, Processor_))
		return false;
	}
return true;
}

// -----------------------------------------------------------------------
// Processes the folder with the documentation in the HTML format.
// Gets the file with the values of variables and processes the documentation's
// files according to the values of the variables.
// -----------------------------------------------------------------------
#if defined(__MINGW32__) || defined(__MINGW64__)
	int main(int Argc_, char *ArgvLocal8_[])
#elif defined(SMACRO_WINDOWS)
	int wmain(int Argc_, wchar_t *Argv_[])
#else
	int main(int Argc_, char *Argv_[])
#endif
{
struct THelper {
	static bool normalizeFolderName(TFileNameString &Value_) {
		assert(!Value_.empty());
		#if defined(SMACRO_WINDOWS)
			Value_ = AbsolutePath(Value_);
			if(Value_.empty()) {
				std::cerr << "Path '" << FileNameStringToConsole(Value_) << "' is invalid.";
				return false;
				}
		#endif
		if(Value_[Value_.size() - 1] != DIR_SEPARATOR)
			Value_ += DIR_SEPARATOR;
		return true;
		}
	};

// ---
#if defined(__MINGW32__) || defined(__MINGW64__)
	std::vector<const wchar_t*> ArgvPtrs(Argc_);
	std::vector<std::wstring> ArgvStrings(Argc_);
	for(int i = 0; i < Argc_; ++i) {
		WindowsLocalToWString(ArgvLocal8_[i], ArgvStrings[i]);
		ArgvPtrs[i] = ArgvStrings[i].c_str();
		}
	const wchar_t **Argv_ = &ArgvPtrs[0];
#endif

TParameters Parameters;
if(!ParseParameters(Argc_, (const TFileNameChar**)Argv_, Parameters)) {
	std::cerr << '\n' << std::endl;
	Usage();
	return RETCODE_INVALID_PARAMETERS;
	}
//	
if(!THelper::normalizeFolderName(Parameters.InputFolder) ||
	!THelper::normalizeFolderName(Parameters.OutputFolder)) {
	return RETCODE_INVALID_PARAMETERS;
	}

// Add SMACRO_ROOT var
std::string SmacroRoot("SMACRO_ROOT");
if(Parameters.Variables.find(SmacroRoot) == Parameters.Variables.end()) {
	#if defined(SMACRO_WINDOWS)
		Parameters.Variables[SmacroRoot] = FileNameStringToUtf8(Parameters.InputFolder);
	#else
		Parameters.Variables[SmacroRoot] = Parameters.InputFolder;
	#endif
	}

TProcessor Processor(Parameters);
if(!ProcessFolder(Parameters.InputFolder, Parameters.OutputFolder, Processor))
	return RETCODE_PROCESS_ERROR;

return RETCODE_OK;
}
