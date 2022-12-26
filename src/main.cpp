/*
* This file is part of SMACRO.
*
* Written by Sergey Vasyutin (sergey [at] vasyut.in)
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
#include <tpcl.h>
#include <processor.h>
#include <utils.h>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#ifdef TPCL_OS_WINDOWS
	#include <wtclap/CmdLine.h>
#else
	#include <tclap/CmdLine.h>
	#define TCLAP_STR(S_) S_
#endif

#include <iostream>
#include <vector>
#include <set>

#include <assert.h>

const int RETCODE_OK = 0;
const int RETCODE_INVALID_PARAMETERS = 1;
const int RETCODE_PROCESS_ERROR = 2;

// -----------------------------------------------------------------------
bool ParseVariables(const tpcl::TFileNameChar *FileName_, TParameters &Parameters_)
{
	struct TProcessor {
		TParameters &m_Parameters;
		std::regex m_VariableWithValue;
		std::regex m_VariableWithoutValue;

		//
		TProcessor(TParameters &Parameters_): 
			m_Parameters(Parameters_),
			m_VariableWithValue("([A-Za-z0-9_]+)\\s*=(.+)"), 
			m_VariableWithoutValue("[A-Za-z0-9_]+") {
		}
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
	std::ifstream File(FileName_);
	if(!File) {
		fmt::print("Can't open file '{}'." << tpcl::FileNameToConsoleString(FileName_) << "'." << std::endl;
		return false;
	}
	//
	size_t LineNo = 0;
	std::string Line;
	TProcessor Processor(Parameters_);
	while(std::getline(File, Line)) {
		// Удаляем пробелы в начале и конце строки
		LineNo++;
		TrimString(Line);
		if(Line.empty() || Line[0] == '#') continue;
		if(!Processor.processString(Line)) {
			std::cerr << "Error reading file '" << tpcl::FileNameToConsoleString(FileName_) << "' (line: " << LineNo << ")." << std::endl;
			return false;
		}
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
		#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
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
bool ParseMasks(const std::vector<std::string> &MasksList_, TExcludePatterns &ExcludePatterns_)
{
	std::set<tpcl::TFileNameString> Patterns;
	for(const auto &Masks: MasksList_) {
		#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
			tpcl::TFileNameString WMasks = tpcl::Utf8ToWideString(Masks);
			const tpcl::TFileNameChar *iMasks = WMasks.c_str();
			const tpcl::TFileNameChar *End = iMasks + WMasks.size();
		#else
			const tpcl::TFileNameChar *iMasks = Masks.c_str();
			const tpcl::TFileNameChar *End = iMasks + Masks.size();
		#endif
		while(true) {
			const tpcl::TFileNameChar *Delim = std::find(iMasks, End, tpcl::TFileNameChar(','));
			if(Delim == iMasks) {
				iMasks++;
				continue;
			}
			//
			tpcl::TFileNameString Value(iMasks, Delim);
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
				std::cerr << "Invalid file mask '" << tpcl::FileNameToConsoleString(Value) << "'.";
				return false;
			}
			//
			if(Delim == End) break;
			iMasks = Delim + 1;
		}
	}
	return true;
}

// -----------------------------------------------------------------------
bool ParseFileList(const tpcl::TFileNameChar *FileName_, std::vector<tpcl::TFileNameString> &Files_)
{
	std::ifstream File(FileName_);
	if(!File) {
		std::cerr << "Can't open file '" << tpcl::FileNameToConsoleString(FileName_) << "'." << std::endl;
		return false;
	}
	//
	size_t LineNo = 0;
	std::string Line;
	while(std::getline(File, Line)) {
		LineNo++;
		// Удаляем пробелы в начале и конце строки
		TrimString(Line);
		if(Line.empty() || Line[0] == '#') continue;

		#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
			tpcl::TFileNameString String;
			if(!tpcl::Utf8ToWide(Line, String)) {
				std::cerr << "Invalid UTF-8 string (" << tpcl::FileNameToConsoleString(FileName_) << ": " << LineNo << ")." << std::endl;
				return false;
			}
			Files_.emplace_back(std::move(String));
		#elif defined(TPCL_OS_WINDOWS)
			std::string String;
			if(!tpcl::Utf8ToLocal(Line, String)) {
				std::cerr << "Invalid UTF-8 string (" << tpcl::FileNameToConsoleString(FileName_) << ": " << LineNo << ")." << std::endl;
				return false;
			}
			Files_.emplace_back(std::move(String));
		#else
			Files_.emplace_back(std::move(Line));
		#endif
	}
	return true;
}

// -----------------------------------------------------------------------
const tpcl::TFileNameChar *g_UsageMessage = 
	TCLAP_STR("All the files being processed (except the files excluded from the processing with the -e switch) are assumed to be in UTF-8 encoding.\n")
	TCLAP_STR("Example:\n")
	#if defined(TPCL_OS_WINDOWS)
		TCLAP_STR("\tsmacro -s ..\\..\\example\\source -d ..\\..\\build\\doc_res -v ..\\..\\example\\config -e *.txt,*.png -e *.jpg\n\n")
	#else
		"\tsmacro -s ../../example/source -d ../../build/doc_res -v ../../example/config -e *.txt,*.png -e *.jpg\n\n"
	#endif
	TCLAP_STR("SMACRO (Simple MACRO processor) is written by Sergey Vasyutin (see https://github.com/vasyutin/smacro).");

// -----------------------------------------------------------------------
bool ParseParameters(int Argc_, const tpcl::TFileNameChar **Argv_, TParameters &Parameters_)
{
	TCLAP::CmdLine CmdParser(g_UsageMessage, TCLAP_STR(' '), TCLAP_STR("2.1"));
	TCLAP::ValueArg<tpcl::TFileNameString> InputFolder(TCLAP_STR("s"), TCLAP_STR("source"), 
		TCLAP_STR("The folder, containing documentation files to process"), true, tpcl::TFileNameString(), 
		TCLAP_STR("source folder"), CmdParser);
	TCLAP::ValueArg<tpcl::TFileNameString> OutputFolder(TCLAP_STR("d"), TCLAP_STR("destination"), 
		TCLAP_STR("The destination folder for the processed files"), true, tpcl::TFileNameString(), 
		TCLAP_STR("destination folder"), CmdParser);
	TCLAP::ValueArg<tpcl::TFileNameString> VariablesFile(TCLAP_STR("v"), TCLAP_STR("variables"), 
		TCLAP_STR("The file, containing values of the variables for the current run (the text in the file is assumed to be in UTF-8)."), 
		true, tpcl::TFileNameString(), TCLAP_STR("variables file"), CmdParser);
	TCLAP::MultiArg<tpcl::TFileNameString> ExcludeMasks(TCLAP_STR("e"), TCLAP_STR("exclude"),
		TCLAP_STR("The mask of filename to exclude from processing. This files are only copied to the output folder."), 
		false, TCLAP_STR("exclude masks"), CmdParser);
	TCLAP::MultiArg<tpcl::TFileNameString> IgnoreMasks(TCLAP_STR("i"), TCLAP_STR("ignore"),
		TCLAP_STR("The mask of filename to ignore. This files are not copied to the output folder."), 
		false, TCLAP_STR("ignore masks"), CmdParser);
	TCLAP::ValueArg<tpcl::TFileNameString> OrderFile(TCLAP_STR("o"), TCLAP_STR("order"), 
		TCLAP_STR("The file containing the list of the files to parse to look to the $number directive in required order"), 
		false, tpcl::TFileNameString(), TCLAP_STR("order file"), CmdParser);
	TCLAP::SwitchArg AtPrefixed(TCLAP_STR("@"), TCLAP_STR("at-prefixed"), 
		TCLAP_STR("Use symbol '@' instead of '#' as the prefix for the control operators (@//, @if, @elif, @else, @endif and @include ")
		TCLAP_STR("instead of #//, #if, #elif, #else, #endif and #include) in text with plenty of '#' (ex. in Markdown)"), CmdParser, false);

	try {
		CmdParser.parse(Argc_, Argv_);
	}
	catch (const TCLAP::ArgException& Exeption_) {
		fmt::print(stderr, "Error: {} for arg {}.\n", tpcl::FileNameToUtf8(Exeption_.error()), tpcl::FileNameToUtf8(Exeption_.argId()));
		return false;
	}

	#if defined(TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T)
		tpcl::Utf8ToWide(InputFolder.getValue(), Parameters_.InputFolder);
		tpcl::Utf8ToWide(OutputFolder.getValue(), Parameters_.OutputFolder);
		if(!ParseVariables(tpcl::Utf8ToWideString(VariablesFile.getValue()).c_str(), Parameters_)) return false;
		if(OrderFile.isSet()) {
			if(!ParseFileList(tpcl::Utf8ToWideString(OrderFile.getValue()).c_str(), Parameters_.OrderedFileList)) return false;
		}
	#else
		Parameters_.InputFolder = InputFolder.getValue();
		Parameters_.OutputFolder = OutputFolder.getValue();
		if(!ParseVariables(VariablesFile.getValue().c_str(), Parameters_)) return false;
		if(OrderFile.isSet()) {
			if(!ParseFileList(OrderFile.getValue().c_str(), Parameters_.OrderedFileList)) return false;
		}
	#endif

	if(ExcludeMasks.isSet() && !ParseMasks(ExcludeMasks.getValue(), Parameters_.ExcludePatterns)) {
		std::cerr << "Error specifying exclude mask." << std::endl;
		return false;
	}
	if(IgnoreMasks.isSet() && !ParseMasks(IgnoreMasks.getValue(), Parameters_.IgnorePatterns)) {
		std::cerr << "Error specifying ignore mask." << std::endl;
		return false;
	}
	Parameters_.AlternativeOperatorPrefix = AtPrefixed.isSet();
	return true;	
}

// -----------------------------------------------------------------------
bool ProcessFolder(const tpcl::TFileNameString &Input_, const tpcl::TFileNameString &Output_, TProcessor &Processor_)
{
	if(!tpcl::FolderExists(Input_.c_str())) {
		std::cerr << "Error accessing folder '" << tpcl::FileNameToConsoleString(Input_) << "'.";
		return false;
	}

	if(Processor_.mode() == TProcessor::TMode::Processing) {
		if(!tpcl::FolderExists(Output_.c_str())) {
			if(!tpcl::CreatePath(Output_.c_str())) {
				std::cerr << "Can't create folder '" << tpcl::FileNameToConsoleString(Output_) << "'.";
				return false;
			}
		}
	}

	std::vector<tpcl::TFileNameString> Folders, Files;
	if(!tpcl::FolderEntries(Input_.c_str(), Folders, Files)) {
		std::cerr << "Can't get contents of the folder '" << tpcl::FileNameToConsoleString(Input_) << "'.";
		return false;
	}

	std::sort(Folders.begin(), Folders.end(), std::locale());
	std::sort(Files.begin(), Files.end(), std::locale());

	for(auto it = Files.begin(); it != Files.end(); ++it) {
		tpcl::TFileNameString InputFile(Input_ + *it), OutputFile(Output_ + *it);
		if(Processor_.mode() == TProcessor::TMode::Processing) {
			if(tpcl::FileExists(OutputFile.c_str())) {
				if(!tpcl::RemoveFile(OutputFile.c_str())) {
					std::cerr << "Can't delete file '" << tpcl::FileNameToConsoleString(OutputFile) << "'.";
					return false;
				}
			}
		}
		if(Processor_.isIgnored(*it)) continue;
		bool Result;
		if(Processor_.isExcluded(*it)) {
			if(Processor_.mode() == TProcessor::TMode::Processing)
				Result = tpcl::DuplicateFile(InputFile.c_str(), OutputFile.c_str());
			else
				Result = true;
		}
		else {
			Result = Processor_.processFile(InputFile, OutputFile);
		}
		//
		if(!Result) return false;
	}

	for(auto it = Folders.begin(); it != Folders.end(); ++it) {
		if(!ProcessFolder(Input_ + *it + TPCL_FS_SEPARATOR, Output_ + *it + TPCL_FS_SEPARATOR, Processor_))
			return false;
	}
	return true;
}

// -----------------------------------------------------------------------
bool ProcessList(const std::vector<tpcl::TFileNameString> &OrderedList_, TProcessor &Processor_)
{
	assert(Processor_.mode() == TProcessor::TMode::Collecting);
	tpcl::TFileNameString NullOutput;
	for(auto it = OrderedList_.begin(); it != OrderedList_.end(); ++it) {
		if(!Processor_.processFile(*it, NullOutput))
			return false;
	}
	return true;
}

// -----------------------------------------------------------------------
// Processes the folder with the documentation in the HTML format.
// Gets the file with the values of variables and processes the documentation's
// files according to the values of the variables.
// -----------------------------------------------------------------------
#ifdef TPCL_OS_WINDOWS
	int wmain(int Argc_, wchar_t *Argv_[])
#else
	int main(int Argc_, char *Argv_[])
#endif
{
	TParameters Parameters;
	if(!ParseParameters(Argc_, (const tpcl::TFileNameChar**)Argv_, Parameters)) {
		std::cerr << '\n' << std::endl;
		return RETCODE_INVALID_PARAMETERS;
	}

	// Add SMACRO_ROOT var
	std::string SmacroRoot("SMACRO_ROOT");
	if(Parameters.Variables.find(SmacroRoot) == Parameters.Variables.end()) {
		?? \ or /
		Parameters.Variables[SmacroRoot] = tpcl::FileNameToUtf8(Parameters.InputFolder.c_str());
	}

	TProcessor Processor(Parameters, TProcessor::TMode::Collecting);
	if(Parameters.OrderedFileList.empty()) {
		if(!ProcessFolder(Parameters.InputFolder, Parameters.OutputFolder, Processor))
			return RETCODE_PROCESS_ERROR;
	}
	else {
		if(!ProcessList(Parameters.OrderedFileList, Processor))
			return RETCODE_PROCESS_ERROR;
	}

	Processor.setMode(TProcessor::TMode::Processing);
	if(!ProcessFolder(Parameters.InputFolder, Parameters.OutputFolder, Processor))
		return RETCODE_PROCESS_ERROR;

	return RETCODE_OK;
}
