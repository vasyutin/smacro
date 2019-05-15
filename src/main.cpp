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
#include <cctype>
#include <functional>

#include <assert.h>

const int RETCODE_OK = 0;
const int RETCODE_INVALID_PARAMETERS = 1;
const int RETCODE_PROCESS_ERROR = 1;

// -----------------------------------------------------------------------
void Usage(void)
{
std::cout << "SMACRO. Simple macro processor. Helps prepare documentation. "
"Written by Sergey Vasyutin.\n"
"\n"
"Usage: smacro <switches>\n"
"\n"
"The switches are:\n"
" -i<input folder> - the folder, containing documentation to process\n"
" -o<output folder> - the destination folder for the processed documentation\n"
" -v<variables file> - the file, containing values of the variables for the current run.\n"
"    The text in the file is assumed to be in UTF-8\n"
" -e<masks> - The masks of filenames to exclude from processing. This files are only\n"
"    copied to the output folder. The masks are separated by semicolons.\n"
"\n"
"All the files being processed (except the files excluded from the processing with the -e\n"
"switch) are assumed to be in UTF-8 encoding.\n\n"
"Example:\n"
" smacro -i..\\..\\example\\source -o..\\..\\build\\doc_res "
"-v..\\..\\example\\config -e*.txt;*.png";
}

// -----------------------------------------------------------------------
inline std::string& TrimLeft(std::string &String_) 
{
String_.erase(String_.begin(), std::find_if(String_.begin(), String_.end(), 
	std::not1(std::ptr_fun<int, int>(std::isspace))));
return String_;
}

// -----------------------------------------------------------------------
inline std::string& TrimRight(std::string &String_) 
{
String_.erase(std::find_if(String_.rbegin(), String_.rend(), 
	std::not1(std::ptr_fun<int, int>(std::isspace))).base(), String_.end());
return String_;
}

// -----------------------------------------------------------------------
inline std::string& TrimString(std::string &String_) 
{
return TrimLeft(TrimRight(String_));
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
			m_Parameters.Variables.insert(std::make_pair(Match[1], std::string()));
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
	std::cerr << "Can't open file: '" << FileNameToConsole(FileName_) << "'.";
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
bool ParseMasks(const TFileNameChar *Masks_, TExcludePatterns &ExcludePatterns_)
{
QSet<QString> Patterns;



QStringList MasksList = QString::fromLocal8Bit(Masks_).split(';', QString::SkipEmptyParts);
for(QStringList::Iterator it = MasksList.begin(); it != MasksList.end(); ++it) {
	*it = it->trimmed();
	if(it->isEmpty()) return false;
	}
//
for(QStringList::Iterator it = MasksList.begin(); it != MasksList.end(); ++it) {
	if(Patterns_.contains(*it)) continue;
	//
	ExcludePatterns_.push_back(QRegExp(*it, 
		#ifdef Q_OS_WIN
			Qt::CaseInsensitive, QRegExp::Wildcard));
		#else
			Qt::CaseSensitive, QRegExp::WildcardUnix));
		#endif
	Patterns_.insert(*it);
	}
return true;
}

// -----------------------------------------------------------------------
bool ParseParameters(int Argc_, TFileNameChar *Argv_[], TParameters &Parameters_)
{
struct THelper {
	static size_t length(const TFileNameChar *String_) {
		#if defined(SMACRO_WINDOWS)
			return wcslen(String_);
		#else
			return strlen(String_);
		#endif
		}
	};

// ---
for(int i = 1; i < Argc_; ++i) {
	const TFileNameChar *Argument = Argv_[i];
	size_t ArgLen = THelper::length(Argument);
	if(ArgLen < 2 || Argument[0] != TFileNameChar('-')) {
		std::cerr << "Invalid argument: '" << FileNameToConsole(Argument) << "'.";
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
			std::cerr << "Invalid file mask found: '" << FileNameToConsole(Argument + 2) << "'.";
			return false;
			}
		break;
	default:
		std::cerr << "Invalid switch: '" << FileNameToConsole(Argument) << "'.";
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
QDir InputFolder(Input_);
if(!InputFolder.exists()) {
	std::cerr << "Error accessing folder '" << (const char*)Input_.toLocal8Bit() << "'.";
	return false;
	}
//
QDir OutputFolder(Output_);
if(!OutputFolder.exists()) {
	if(!OutputFolder.mkpath(Output_)) {
		std::cerr << "Can't create folder '" << (const char*)Output_.toLocal8Bit() << "'.";
		return false;
		}
	}
// Processing files
QStringList FilesList = InputFolder.entryList(QDir::Files | QDir::Hidden | QDir::System);
for(QStringList::iterator it = FilesList.begin(); it != FilesList.end(); ++it) {
	QString InputFile(Input_ + *it), OutputFile(Output_ + *it);
	if(QFile::exists(OutputFile)) {
		if(!QFile::remove(OutputFile)) {
			std::cerr << "Can't write file '" << (const char*)OutputFile.toLocal8Bit() << "'.";
			return false;
			}
		}
	bool Result = Processor_.isExcluded(*it)? 
		QFile::copy(InputFile, OutputFile):
		Processor_.processFile(InputFile, OutputFile);
	//
	if(!Result) {
		//std::cerr << "Can't write file '" << (const char*)OutputFile.toLocal8Bit() << "'.";
		return false;
		}
	}
// Processing folders
FilesList = InputFolder.entryList(QDir::Dirs | QDir::Hidden | QDir::System | 
	QDir::NoDotAndDotDot | QDir::NoSymLinks);
for(QStringList::iterator it = FilesList.begin(); it != FilesList.end(); ++it) {
	if(!ProcessFolder(Input_ + *it + QDir::separator(), Output_ + *it + QDir::separator(), 
		Processor_))
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
	static void normalizeFolderName(TFileNameString &Value_) {
		assert(!Value_.empty());
		if(Value_[Value_.size() - 1] != DIR_SEPARATOR)
			Value_ += DIR_SEPARATOR;
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
if(!ParseParameters(Argc_, Argv_, Parameters)) {
	std::cerr << '\n' << std::endl;
	Usage();
	return RETCODE_INVALID_PARAMETERS;
	}
//	
THelper::normalizeFolderName(Parameters.InputFolder);
THelper::normalizeFolderName(Parameters.OutputFolder);
TProcessor Processor(Parameters);
if(!ProcessFolder(Parameters.InputFolder, Parameters.OutputFolder, Processor))
	return RETCODE_PROCESS_ERROR;

return RETCODE_OK;
}
