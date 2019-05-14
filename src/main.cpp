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

// -----------------------------------------------------------------------
// Processes the folder with the documentation in the HTML format.
// Gets the file with the values of variables and processes the documentation's
// files according to the values of the variables.
// -----------------------------------------------------------------------
#include "processor.h"
#include "utils.h"

#include <iostream>

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
bool ParseVariables(const char *FileName_, TParameters &Parameters_)
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
	bool processString(const QString &WLine_) {
		if(m_VariableWithValue.exactMatch(WLine_)) {
			m_Parameters.Variables.insert(m_VariableWithValue.cap(1), m_VariableWithValue.cap(2));
			return true;
			}
		if(m_VariableWithoutValue.exactMatch(WLine_)) {
			m_Parameters.Variables.insert(m_VariableWithValue.cap(0), QString());
			return true;
			}
		return false;
		}
	};

// ---
QFile VariablesFile(QString::fromLocal8Bit(FileName_));
if(!VariablesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
	std::cerr << "Can't open file: '" << FileName_ << "'.";
	return false;
	}
//
TProcessor Processor(Parameters_);
QByteArray Line;
QString WLine;
while(!VariablesFile.atEnd()) {
	Line = VariablesFile.readLine();
	WLine = QString::fromUtf8(Line.trimmed());
	if(WLine.isEmpty() || WLine[0] == QLatin1Char('#')) continue;
	if(!Processor.processString(WLine)) 
		return false;
	}
return true;
}

// -----------------------------------------------------------------------
bool ParseMasks(const char *Masks_, QSet<QString> &Patterns_, 
	TExcludePatterns &ExcludePatterns_)
{
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
bool ParseParameters(int Argc_, char *Argv_[], TParameters &Parameters_)
{
QSet<QString> Patterns;

for(int i = 1; i < Argc_; ++i) {
	const char *Argument = Argv_[i];
	size_t ArgLen = strlen(Argument);
	if(ArgLen < 2 || Argument[0] != '-') {
		std::cerr << "Invalid argument: '" << Argument << "'.";
		return false;
		}
	//
	switch(Argument[1]) {
	case 'i':
		Parameters_.InputFolder = QString::fromLocal8Bit(Argument + 2);
		break;
	case 'o':
		Parameters_.OutputFolder = QString::fromLocal8Bit(Argument + 2);
		break;
	case 'v':
		if(!ParseVariables(Argument + 2, Parameters_)) return false;
		break;
	case 'e':
		if(!ParseMasks(Argument + 2, Patterns, Parameters_.ExcludePatterns)) {
			std::cerr << "Invalid file mask found: '" << Argument + 2 << "'.";
			return false;
			}
		break;
	default:
		std::cerr << "Invalid switch: '" << Argument[0] << "'.";
		return false;
		} // switch
	}
	
if(Parameters_.InputFolder.isEmpty() || Parameters_.OutputFolder.isEmpty()) {
	std::cerr << "No input or output folder was specified.";
	return false;
	}
return true;	
}

// -----------------------------------------------------------------------
bool ProcessFolder(const QString &Input_, const QString &Output_, TProcessor &Processor_)
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
int main(int Argc_, char *Argv_[])
{
struct THelper {
	static void normalizeFolderName(std::string &Value_) {
		std::string Folder = 

		QDir Folder(Value_);
		Value_ = QDir::toNativeSeparators(Folder.absolutePath());

		ToNativeSeparators

		Q_ASSERT(!Value_.endsWith(QDir::separator()));
		Value_ += QDir::separator();
		}
	};

// ---
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
