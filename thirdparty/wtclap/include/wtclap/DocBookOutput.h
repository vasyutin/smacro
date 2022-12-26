// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  DocBookOutput.h
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

#ifndef TCLAP_DOC_BOOK_OUTPUT_H
#define TCLAP_DOC_BOOK_OUTPUT_H

#include <wtclap/Arg.h>
#include <wtclap/CmdLineInterface.h>
#include <wtclap/CmdLineOutput.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace TCLAP {

/**
 * A class that generates DocBook output for usage() method for the
 * given CmdLine and its Args.
 */
class DocBookOutput : public CmdLineOutput {
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

    DocBookOutput() : theDelimiter(L'=') {}

protected:
    /**
     * Substitutes the char r for string x in string s.
     * \param s - The string to operate on.
     * \param r - The char to replace.
     * \param x - What to replace r with.
     */
    void substituteSpecialChars(std::wstring &s, wchar_t r,
                                const std::wstring &x) const;
    void removeChar(std::wstring &s, wchar_t r) const;

    void printShortArg(Arg *it, bool required);
    void printLongArg(const ArgGroup &it) const;

    wchar_t theDelimiter;
};

inline void DocBookOutput::version(CmdLineInterface &_cmd) {
    std::wcout << _cmd.getVersion() << std::endl;
}

namespace internal {
const wchar_t *GroupChoice(const ArgGroup &group) {
    if (!group.showAsGroup()) {
        return L"plain";
    }

    if (group.isRequired()) {
        return L"req";
    }

    return L"opt";
}
}  // namespace internal

inline void DocBookOutput::usage(CmdLineInterface &_cmd) {
    std::list<ArgGroup *> argSets = _cmd.getArgGroups();
    std::wstring progName = _cmd.getProgramName();
    std::wstring xversion = _cmd.getVersion();
    theDelimiter = _cmd.getDelimiter();

    std::wcout << L"<?xml version='1.0'?>\n";
    std::wcout
        << L"<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.2//EN\"\n";
    std::wcout
        << L"\t\"http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd\">\n\n";

    std::wcout << L"<refentry>\n";

    std::wcout << L"<refmeta>\n";
    std::wcout << L"<refentrytitle>" << progName << L"</refentrytitle>\n";
    std::wcout << L"<manvolnum>1</manvolnum>\n";
    std::wcout << L"</refmeta>\n";

    std::wcout << L"<refnamediv>\n";
    std::wcout << L"<refname>" << progName << L"</refname>\n";
    std::wcout << L"<refpurpose>" << _cmd.getMessage() << L"</refpurpose>\n";
    std::wcout << L"</refnamediv>\n";

    std::wcout << L"<refsynopsisdiv>\n";
    std::wcout << L"<cmdsynopsis>\n";

    std::wcout << L"<command>" << progName << L"</command>\n";

    for (std::list<ArgGroup *>::iterator sit = argSets.begin();
         sit != argSets.end(); ++sit) {
        int visible = CountVisibleArgs(**sit);
        if (visible > 1) {
            std::wcout << L"<group choice='" << internal::GroupChoice(**sit)
                      << L"'>\n";
        }
        for (ArgGroup::iterator it = (*sit)->begin(); it != (*sit)->end();
             ++it) {
            if (!(*it)->visibleInHelp()) {
                continue;
            }

            printShortArg(*it, (*it)->isRequired() ||
                                   (visible == 1 && (**sit).isRequired()));
        }
        if (visible > 1) {
            std::wcout << L"</group>\n";
        }
    }

    std::wcout << L"</cmdsynopsis>\n";
    std::wcout << L"</refsynopsisdiv>\n";

    std::wcout << L"<refsect1>\n";
    std::wcout << L"<title>Description</title>\n";
    std::wcout << L"<para>\n";
    std::wcout << _cmd.getMessage() << L'\n';
    std::wcout << L"</para>\n";
    std::wcout << L"</refsect1>\n";

    std::wcout << L"<refsect1>\n";
    std::wcout << L"<title>Options</title>\n";

    std::wcout << L"<variablelist>\n";

    for (std::list<ArgGroup *>::iterator sit = argSets.begin();
         sit != argSets.end(); ++sit) {
        printLongArg(**sit);
    }

    std::wcout << L"</variablelist>\n";
    std::wcout << L"</refsect1>\n";

    std::wcout << L"<refsect1>\n";
    std::wcout << L"<title>Version</title>\n";
    std::wcout << L"<para>\n";
    std::wcout << xversion << L'\n';
    std::wcout << L"</para>\n";
    std::wcout << L"</refsect1>\n";

    std::wcout << L"</refentry>" << std::endl;
}

inline void DocBookOutput::failure(CmdLineInterface &_cmd, ArgException &e) {
    static_cast<void>(_cmd);  // unused
    std::wcout << e.wwhat() << std::endl;
    throw ExitException(1);
}

inline void DocBookOutput::substituteSpecialChars(std::wstring &s, wchar_t r,
                                                  const std::wstring &x) const {
    size_t p;
    while ((p = s.find_first_of(r)) != std::wstring::npos) {
        s.erase(p, 1);
        s.insert(p, x);
    }
}

inline void DocBookOutput::removeChar(std::wstring &s, wchar_t r) const {
    size_t p;
    while ((p = s.find_first_of(r)) != std::wstring::npos) {
        s.erase(p, 1);
    }
}

inline void DocBookOutput::printShortArg(Arg *a, bool required) {
    std::wstring lt = L"&lt;";
    std::wstring gt = L"&gt;";

    std::wstring id = a->shortID();
    substituteSpecialChars(id, L'<', lt);
    substituteSpecialChars(id, L'>', gt);
    removeChar(id, L'[');
    removeChar(id, L']');

    std::wstring choice = L"opt";
    if (required) {
        choice = L"plain";
    }

    std::wcout << L"<arg choice='" << choice << L'\'';
    if (a->acceptsMultipleValues()) std::wcout << L" rep='repeat'";

    std::wcout << L'>';
    if (!a->getFlag().empty())
        std::wcout << a->flagStartChar() << a->getFlag();
    else
        std::wcout << a->nameStartString() << a->getName();
    if (a->isValueRequired()) {
        std::wstring arg = a->shortID();
        removeChar(arg, '[');
        removeChar(arg, ']');
        removeChar(arg, '<');
        removeChar(arg, '>');
        removeChar(arg, '.');
        arg.erase(0, arg.find_last_of(theDelimiter) + 1);
        std::wcout << theDelimiter;
        std::wcout << L"<replaceable>" << arg << L"</replaceable>";
    }
    std::wcout << L"</arg>" << std::endl;
}

inline void DocBookOutput::printLongArg(const ArgGroup &group) const {
    const std::wstring lt = L"&lt;";
    const std::wstring gt = L"&gt;";

    bool forceRequired = group.isRequired() && CountVisibleArgs(group) == 1;
    for (ArgGroup::const_iterator it = group.begin(); it != group.end(); ++it) {
        Arg &a = **it;
        if (!a.visibleInHelp()) {
            continue;
        }

        std::wstring desc = a.getDescription(forceRequired || a.isRequired());
        substituteSpecialChars(desc, L'<', lt);
        substituteSpecialChars(desc, L'>', gt);

        std::wcout << L"<varlistentry>\n";

        if (!a.getFlag().empty()) {
            std::wcout << L"<term>\n";
            std::wcout << L"<option>";
            std::wcout << a.flagStartChar() << a.getFlag();
            std::wcout << L"</option>\n";
            std::wcout << L"</term>\n";
        }

        std::wcout << L"<term>\n";
        std::wcout << L"<option>";
        std::wcout << a.nameStartString() << a.getName();
        if (a.isValueRequired()) {
            std::wstring arg = a.shortID();
            removeChar(arg, L'[');
            removeChar(arg, L']');
            removeChar(arg, L'<');
            removeChar(arg, L'>');
            removeChar(arg, L'.');
            arg.erase(0, arg.find_last_of(theDelimiter) + 1);
            std::wcout << theDelimiter;
            std::wcout << L"<replaceable>" << arg << L"</replaceable>";
        }

        std::wcout << L"</option>\n";
        std::wcout << L"</term>\n";

        std::wcout << L"<listitem>\n";
        std::wcout << L"<para>\n";
        std::wcout << desc << '\n';
        std::wcout << L"</para>\n";
        std::wcout << L"</listitem>\n";

        std::wcout << L"</varlistentry>" << std::endl;
    }
}

}  // namespace TCLAP
#endif  // TCLAP_DOC_BOOK_OUTPUT_H
