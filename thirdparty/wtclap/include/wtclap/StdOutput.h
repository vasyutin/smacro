// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  StdOutput.h
 *
 *  Copyright (c) 2004, Michael E. Smoot
 *  Copyright (c) 2017, Google LLC
 *  Copyright (c) 2022 Sergey Vasyutin .
 *  All rights reserved.
 *
 *  See the file COPYING in the top directory of this distribution for
 *  more information.
 *
 *  THE SOFTWARE IS PROVIDED _AS IS_, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#ifndef TCLAP_STD_OUTPUT_H
#define TCLAP_STD_OUTPUT_H

#include <wtclap/Arg.h>
#include <wtclap/ArgGroup.h>
#include <wtclap/CmdLineInterface.h>
#include <wtclap/CmdLineOutput.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

namespace TCLAP {

/**
 * A class that isolates any output from the CmdLine object so that it
 * may be easily modified.
 */
class StdOutput : public CmdLineOutput {
public:
    /**
     * Prints the usage to stdout.  Can be overridden to
     * produce alternative behavior.
     * \param c - The CmdLine object the output is generated for.
     */
    virtual void usage(CmdLineInterface &c);

    /**
     * Prints the version to stdout. Can be overridden
     * to produce alternative behavior.
     * \param c - The CmdLine object the output is generated for.
     */
    virtual void version(CmdLineInterface &c);

    /**
     * Prints (to stderr) an error message, short usage
     * Can be overridden to produce alternative behavior.
     * \param c - The CmdLine object the output is generated for.
     * \param e - The ArgException that caused the failure.
     */
    virtual void failure(CmdLineInterface &c, ArgException &e);

protected:
    /**
     * Writes a brief usage message with short args.
     * \param c - The CmdLine object the output is generated for.
     * \param os - The stream to write the message to.
     */
    void _shortUsage(CmdLineInterface &c, std::wostream &os) const;

    /**
     * Writes a longer usage message with long and short args,
     * provides descriptions and prints message.
     * \param c - The CmdLine object the output is generated for.
     * \param os - The stream to write the message to.
     */
    void _longUsage(CmdLineInterface &c, std::wostream &os) const;

    /**
     * This function inserts line breaks and indents long strings
     * according the  params input. It will only break lines at spaces,
     * commas and pipes.
     * \param os - The stream to be printed to.
     * \param s - The string to be printed.
     * \param maxWidth - The maxWidth allowed for the output line.
     * \param indentSpaces - The number of spaces to indent the first line.
     * \param secondLineOffset - The number of spaces to indent the second
     * and all subsequent lines in addition to indentSpaces.
     */
    void spacePrint(std::wostream &os, const std::wstring &s, int maxWidth,
                    int indentSpaces, int secondLineOffset) const;
};

inline void StdOutput::version(CmdLineInterface &_cmd) {
    std::wstring progName = _cmd.getProgramName();
    std::wstring xversion = _cmd.getVersion();

    std::wcout << std::endl
              << progName << L"  version: " << xversion << std::endl
              << std::endl;
}

inline void StdOutput::usage(CmdLineInterface &_cmd) {
    std::wcout << std::endl << L"USAGE: " << std::endl << std::endl;

    _shortUsage(_cmd, std::wcout);

    std::wcout << std::endl << std::endl << L"Where: " << std::endl << std::endl;

    _longUsage(_cmd, std::wcout);

    std::wcout << std::endl;
}

inline void StdOutput::failure(CmdLineInterface &_cmd, ArgException &e) {
    std::wstring progName = _cmd.getProgramName();

    std::wcerr << L"PARSE ERROR: " << e.argId() << std::endl
              << L"             " << e.error() << std::endl
              << std::endl;

    if (_cmd.hasHelpAndVersion()) {
        std::wcerr << L"Brief USAGE: " << std::endl;

        _shortUsage(_cmd, std::wcerr);

        std::wcerr << std::endl
                  << L"For complete USAGE and HELP type: " << std::endl
                  << L"   " << progName << L" " << Arg::nameStartString()
                  << L"help" << std::endl
                  << std::endl;
    } else {
        usage(_cmd);
    }

    throw ExitException(1);
}

// TODO: Remove this
inline void removeChar(std::wstring &s, wchar_t r) {
    size_t p;
    while ((p = s.find_first_of(r)) != std::wstring::npos) {
        s.erase(p, 1);
    }
}

inline bool cmpSwitch(const wchar_t &a, const wchar_t &b) {
    int lowa = std::tolower(a);
    int lowb = std::tolower(b);

    if (lowa == lowb) {
        return a < b;
    }

    return lowa < lowb;
}

namespace internal {
inline bool IsVisibleShortSwitch(const Arg &arg) {
    return !(arg.getName() == Arg::ignoreNameString() ||
             arg.isValueRequired() || arg.getFlag() == L"") &&
           arg.visibleInHelp();
}

inline bool IsVisibleLongSwitch(const Arg &arg) {
    return (arg.getName() != Arg::ignoreNameString() &&
            !arg.isValueRequired() && arg.getFlag() == L"" &&
            arg.visibleInHelp());
}

inline bool IsVisibleOption(const Arg &arg) {
    return (arg.getName() != Arg::ignoreNameString() && arg.isValueRequired() &&
            arg.hasLabel() && arg.visibleInHelp());
}

inline bool CompareShortID(const Arg *a, const Arg *b) {
    if (a->getFlag() == L"" && b->getFlag() != L"") {
        return false;
    }
    if (b->getFlag() == L"" && a->getFlag() != L"") {
        return true;
    }

    return a->shortID() < b->shortID();
}

// TODO: Fix me not to put --gopt before -f
inline bool CompareOptions(std::pair<const Arg *, bool> a,
                           std::pair<const Arg *, bool> b) {
    // First optional, then required
    if (!a.second && b.second) {
        return true;
    }
    if (a.second && !b.second) {
        return false;
    }

    return CompareShortID(a.first, b.first);
}
}  // namespace internal

/**
 * Usage statements should look like the manual pages.  Options w/o
 * operands come first, in alphabetical order inside a single set of
 * braces, upper case before lower case (AaBbCc...).  Next are options
 * with operands, in the same order, each in braces.  Then required
 * arguments in the order they are specified, followed by optional
 * arguments in the order they are specified.  A bar (`|') separates
 * either/or options/arguments, and multiple options/arguments which
 * are specified together are placed in a single set of braces.
 *
 * Use getprogname() instead of hardcoding the program name.
 *
 * "usage: f [-aDde] [-b b_arg] [-m m_arg] req1 req2 [opt1 [opt2]]\n"
 * "usage: f [-a | -b] [-c [-de] [-n number]]\n"
 */
inline void StdOutput::_shortUsage(CmdLineInterface &_cmd,
                                   std::wostream &os) const {
    std::list<ArgGroup *> argSets = _cmd.getArgGroups();

    std::wostringstream outp;
    outp << _cmd.getProgramName() + L" ";

    std::wstring switches = Arg::flagStartString();

    std::list<ArgGroup *> exclusiveGroups;
    std::list<ArgGroup *> nonExclusiveGroups;
    for (std::list<ArgGroup *>::iterator sit = argSets.begin();
         sit != argSets.end(); ++sit) {
        if (CountVisibleArgs(**sit) <= 0) {
            continue;
        }

        if ((*sit)->isExclusive()) {
            exclusiveGroups.push_back(*sit);
        } else {
            nonExclusiveGroups.push_back(*sit);
        }
    }

    // Move "exclusive groups" that have at most a single item to
    // non-exclusive groups as exclusivit doesn't make sense with a
    // single option. This can happen if args are hidden in help for
    // example.
    for (std::list<ArgGroup *>::iterator it = exclusiveGroups.begin();
         it != exclusiveGroups.end();) {
        if (CountVisibleArgs(**it) < 2) {
            nonExclusiveGroups.push_back(*it);
            it = exclusiveGroups.erase(it);
        } else {
            ++it;
        }
    }

    // First short switches (needs to be special because they are all
    // stuck together).
    for (std::list<ArgGroup *>::iterator sit = nonExclusiveGroups.begin();
         sit != nonExclusiveGroups.end(); ++sit) {
        for (ArgGroup::iterator it = (*sit)->begin(); it != (*sit)->end();
             ++it) {
            if (internal::IsVisibleShortSwitch(**it)) {
                switches += (*it)->getFlag();
            }
        }

        std::sort(switches.begin(), switches.end(), cmpSwitch);
    }

    outp << L" [" << switches << L']';

    // Now do long switches (e.g., --version, but no -v)
    std::vector<Arg *> longSwitches;
    for (std::list<ArgGroup *>::iterator sit = nonExclusiveGroups.begin();
         sit != nonExclusiveGroups.end(); ++sit) {
        for (ArgGroup::iterator it = (*sit)->begin(); it != (*sit)->end();
             ++it) {
            Arg &arg = **it;
            if (internal::IsVisibleLongSwitch(arg)) {
                longSwitches.push_back(&arg);
            }
        }
    }

    std::sort(longSwitches.begin(), longSwitches.end(),
              internal::CompareShortID);
    for (std::vector<Arg *>::const_iterator it = longSwitches.begin();
         it != longSwitches.end(); ++it) {
        outp << L" [" << (**it).shortID() << L']';
    }

    // Now do all exclusive groups
    for (std::list<ArgGroup *>::iterator sit = exclusiveGroups.begin();
         sit != exclusiveGroups.end(); ++sit) {
        ArgGroup &argGroup = **sit;
        outp << (argGroup.isRequired() ? L" {" : L" [");

        std::vector<Arg *> args;
        for (ArgGroup::iterator it = argGroup.begin(); it != argGroup.end();
             ++it) {
            if ((**it).visibleInHelp()) {
                args.push_back(*it);
            }
        }

        std::sort(args.begin(), args.end(), internal::CompareShortID);
        std::wstring sep = L"";
        for (std::vector<Arg *>::const_iterator it = args.begin();
             it != args.end(); ++it) {
            outp << sep << (**it).shortID();
            sep = L"|";
        }

        outp << (argGroup.isRequired() ? L'}' : L']');
    }

    // Next do options, we sort them later by optional first.
    std::vector<std::pair<const Arg *, bool> > options;
    for (std::list<ArgGroup *>::iterator sit = nonExclusiveGroups.begin();
         sit != nonExclusiveGroups.end(); ++sit) {
        for (ArgGroup::iterator it = (*sit)->begin(); it != (*sit)->end();
             ++it) {
            Arg &arg = **it;
            int visible = CountVisibleArgs(**sit);
            bool required = arg.isRequired();
            if (internal::IsVisibleOption(arg)) {
                if (visible == 1 && (**sit).isRequired()) {
                    required = true;
                }

                options.push_back(std::make_pair(&arg, required));
            }
        }
    }

    std::sort(options.begin(), options.end(), internal::CompareOptions);
    for (std::vector<std::pair<const Arg *, bool> >::const_iterator it =
             options.begin();
         it != options.end(); ++it) {
        const Arg &arg = *it->first;
        bool required = it->second;
        outp << (required ? L" " : L" [");
        outp << arg.shortID();
        outp << (required ? L"" : L"]");
    }

    // Next do argsuments ("unlabled") in order of definition
    for (std::list<ArgGroup *>::iterator sit = nonExclusiveGroups.begin();
         sit != nonExclusiveGroups.end(); ++sit) {
        for (ArgGroup::iterator it = (*sit)->begin(); it != (*sit)->end();
             ++it) {
            Arg &arg = **it;
            if (arg.getName() == Arg::ignoreNameString()) {
                continue;
            }

            if (arg.isValueRequired() && !arg.hasLabel() &&
                arg.visibleInHelp()) {
                outp << (arg.isRequired() ? L" " : L" [");
                outp << arg.shortID();
                outp << (arg.isRequired() ? L"" : L"]");
            }
        }
    }

    // if the program name is too long, then adjust the second line offset
    int secondLineOffset = static_cast<int>(_cmd.getProgramName().length()) + 2;
    if (secondLineOffset > 75 / 2) secondLineOffset = static_cast<int>(75 / 2);

    spacePrint(os, outp.str(), 75, 3, secondLineOffset);
}

inline void StdOutput::_longUsage(CmdLineInterface &_cmd,
                                  std::wostream &os) const {
    std::wstring message = _cmd.getMessage();
    std::list<ArgGroup *> argSets = _cmd.getArgGroups();

    std::list<Arg *> unlabled;
    for (std::list<ArgGroup *>::iterator sit = argSets.begin();
         sit != argSets.end(); ++sit) {
        ArgGroup &argGroup = **sit;

        int visible = CountVisibleArgs(argGroup);
        bool exclusive = visible > 1 && argGroup.isExclusive();
        bool forceRequired = visible == 1 && argGroup.isRequired();
        if (exclusive) {
            spacePrint(os, argGroup.isRequired() ? L"One of:" : L"Either of:", 75,
                       3, 0);
        }

        for (ArgGroup::iterator it = argGroup.begin(); it != argGroup.end();
             ++it) {
            Arg &arg = **it;
            if (!arg.visibleInHelp()) {
                continue;
            }

            if (!arg.hasLabel()) {
                unlabled.push_back(&arg);
                continue;
            }

            bool required = arg.isRequired() || forceRequired;
            if (exclusive) {
                spacePrint(os, arg.longID(), 75, 6, 3);
                spacePrint(os, arg.getDescription(required), 75, 8, 0);
            } else {
                spacePrint(os, arg.longID(), 75, 3, 3);
                spacePrint(os, arg.getDescription(required), 75, 5, 0);
            }
            os << L'\n';
        }
    }

    for (ArgListIterator it = unlabled.begin(); it != unlabled.end(); ++it) {
        const Arg &arg = **it;
        spacePrint(os, arg.longID(), 75, 3, 3);
        spacePrint(os, arg.getDescription(), 75, 5, 0);
        os << L'\n';
    }

    if (!message.empty()) {
        spacePrint(os, message, 75, 3, 0);
    }

    os.flush();
}

namespace {
inline void fmtPrintLine(std::wostream &os, const std::wstring &s, int maxWidth,
                         int indentSpaces, int secondLineOffset) {
    const std::wstring splitChars(L" ,|");
    int maxChars = maxWidth - indentSpaces;
    std::wstring indentString(indentSpaces, L' ');
    int from = 0;
    int to = 0;
    int end = (int)s.length();
    for (;;) {
        if (end - from <= maxChars) {
            // Rest of string fits on line, just print the remainder
            os << indentString << s.substr(from) << std::endl;
            return;
        }

        // Find the next place where it is good to break the string
        // (to) by finding the place where it is too late (tooFar) and
        // taking the previous one.
        int tooFar = to;
        while (tooFar - from <= maxChars &&
               static_cast<std::size_t>(tooFar) != std::wstring::npos) {
            to = tooFar;
            tooFar = (int)s.find_first_of(splitChars, to + 1);
        }

        if (to == from) {
            // In case there was no good place to break the string,
            // just break it in the middle of a word at line length.
            to = from + maxChars - 1;
        }

        if (s[to] != L' ') {
            // Include delimiter before line break, unless it's a space
            to++;
        }

        os << indentString << s.substr(from, to - from) << L'\n';

        // Avoid printing extra white space at start of a line
        for (; s[to] == L' '; to++) {
        }
        from = to;

        if (secondLineOffset != 0) {
            // Adjust offset for following lines
            indentString.insert(indentString.end(), secondLineOffset, L' ');
            maxChars -= secondLineOffset;
            secondLineOffset = 0;
        }
    }
}
}  // namespace

inline void StdOutput::spacePrint(std::wostream &os, const std::wstring &s,
                                  int maxWidth, int indentSpaces,
                                  int secondLineOffset) const {
    std::wstringstream ss(s);
    std::wstring line;
    std::getline(ss, line);
    fmtPrintLine(os, line, maxWidth, indentSpaces, secondLineOffset);
    indentSpaces += secondLineOffset;

    while (std::getline(ss, line)) {
        fmtPrintLine(os, line, maxWidth, indentSpaces, 0);
    }
}

}  // namespace TCLAP

#endif  // TCLAP_STD_OUTPUT_H
