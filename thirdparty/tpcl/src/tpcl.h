#pragma once

/*
* This file is part of Tiny Platfrom Crossing Library (TPCL).
*
* Written by Sergey Vasyutin (in[at]vasyut.in)
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

#include <tpclbase.h>
#include <string>
#include <vector>

namespace tpcl {
// -----------------------------------------------------------------------
#if defined(TPCL_MSC)
	#define TPCL_TO_FNSTR(S_) L ## S_
	#define TPCL_FNSTR(S_) TPCL_TO_FNSTR(S_)
	typedef wchar_t TFileNameChar;
	typedef std::wstring TFileNameString;
	#define TPCL_FS_SEPARATOR L'\\'
	#define TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T
#else
	#define TPCL_FNSTR(S_) S_
	typedef char TFileNameChar;
	typedef std::string TFileNameString;
	#define TPCL_FS_SEPARATOR '/'
	#define TPCL_FILE_NAME_CHAR_TYPE_IS_CHAR
#endif

// ----------------------------------------------------------------------------
#if defined(TPCL_OS_WINDOWS)
	// ---
	// utf8 -> wchar_t
	// ---
	bool Utf8ToWide(const char* UTF8_, int UTF8Size_, std::wstring& String_);
	inline bool Utf8ToWide(const char* UTF8_, std::wstring& String_) {return Utf8ToWide(UTF8_, -1, String_);}
	inline bool Utf8ToWide(const std::string &UTF8_, std::wstring& String_) {return Utf8ToWide(UTF8_.c_str(), (int)UTF8_.size(), String_);}

	inline std::wstring Utf8ToWideString(const char* UTF8_, int UTF8Size_, bool* Ok_ = nullptr) {
		std::wstring RetValue;
		bool IsOk = Utf8ToWide(UTF8_, UTF8Size_, RetValue);
		if(Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::wstring Utf8ToWideString(const char* UTF8_, bool* Ok_ = nullptr) { return Utf8ToWideString(UTF8_, -1, Ok_); }
	inline std::wstring Utf8ToWideString(const std::string& String_, bool* Ok_ = nullptr) { return Utf8ToWideString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// utf8 -> local 8 bit
	// ---
	bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_);
	inline bool Utf8ToLocal(const char* UTF8_, std::string& String_) {return Utf8ToLocal(UTF8_, -1, String_);}
	inline bool Utf8ToLocal(const std::string& UTF8_, std::string& String_) { return Utf8ToLocal(UTF8_.c_str(), (int)UTF8_.size(), String_); }

	inline std::string Utf8ToLocalString(const char* UTF8_, int UTF8Size_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = Utf8ToLocal(UTF8_, UTF8Size_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::string Utf8ToLocalString(const char* UTF8_, bool* Ok_ = nullptr) {return Utf8ToLocalString(UTF8_, -1, Ok_); }
	inline std::string Utf8ToLocalString(const std::string& String_, bool* Ok_ = nullptr) { return Utf8ToLocalString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// wchar_t -> utf8
	// ---
	bool WideToUtf8(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_);
	inline bool WideToUtf8(const wchar_t* Unicode_, std::string& String_) { return WideToUtf8(Unicode_, -1, String_); }
	inline bool WideToUtf8(const std::wstring &WString_, std::string& String_) {return WideToUtf8(WString_.c_str(), (int)WString_.size(), String_);}

	inline std::string WideToUtf8String(const wchar_t* Unicode_, int UnicodeSize_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = WideToUtf8(Unicode_, UnicodeSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::string WideToUtf8String(const wchar_t* Unicode_, bool* Ok_ = nullptr) { return WideToUtf8String(Unicode_, -1, Ok_); }
	inline std::string WideToUtf8String(const std::wstring& String_, bool* Ok_ = nullptr) { return WideToUtf8String(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// wchar_t -> local 8 bit
	// ---
	bool WideToLocal(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_);
	inline bool WideToLocal(const wchar_t* Unicode_, std::string& String_) { return WideToLocal(Unicode_, -1, String_); }
	inline bool WideToLocal(const std::wstring& WString_, std::string& String_) { return WideToLocal(WString_.c_str(), (int)WString_.size(), String_); }

	inline std::string WideToLocalString(const wchar_t* Unicode_, int UnicodeSize_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = WideToLocal(Unicode_, UnicodeSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::string WideToLocalString(const wchar_t* Unicode_, bool* Ok_ = nullptr) {return WideToLocalString(Unicode_, -1, Ok_);}
	inline std::string WideToLocalString(const std::wstring& String_, bool* Ok_ = nullptr) { return WideToLocalString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// local 8 bit -> utf8
	// ---
	bool LocalToUtf8(const char* Local8Bit_, int Local8BitSize_, std::string& String_);
	inline bool LocalToUtf8(const char* Local8Bit_, std::string& String_) {return LocalToUtf8(Local8Bit_, -1, String_);}
	inline bool LocalToUtf8(const std::string& LocalString_, std::string& String_) { return LocalToUtf8(LocalString_.c_str(), (int)LocalString_.size(), String_); }

	inline std::string LocalToUtf8String(const char* Local8Bit_, int Local8BitSize_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = LocalToUtf8(Local8Bit_, Local8BitSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::string LocalToUtf8String(const char* Local8Bit_, bool* Ok_ = nullptr) {return LocalToUtf8String(Local8Bit_, -1, Ok_);}
	inline std::string LocalToUtf8String(const std::string& String_, bool* Ok_ = nullptr) { return LocalToUtf8String(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// local 8 bit -> wchar_t
	// ---
	bool LocalToWide(const char* Local8Bit_, int Local8BitSize_, std::wstring& String_);
	inline bool LocalToWide(const char* Local8Bit_, std::wstring& String_) {return LocalToWide(Local8Bit_, -1, String_);}
	inline bool LocalToWide(const std::string& Local8Bit_, std::wstring& String_) { return LocalToWide(Local8Bit_.c_str(), (int)Local8Bit_.size(), String_); }

	inline std::wstring LocalToWideString(const char* Local8Bit_, int Local8BitSize_, bool* Ok_ = nullptr) {
		std::wstring RetValue;
		bool IsOk = Utf8ToWide(Local8Bit_, Local8BitSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::wstring LocalToWideString(const char* Local8Bit_, bool* Ok_ = nullptr) {return LocalToWideString(Local8Bit_, -1, Ok_);}
	inline std::wstring LocalToWideString(const std::string& String_, bool* Ok_ = nullptr) { return LocalToWideString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// FileName -> Default console encoding
	// ---
	std::string FileNameToConsoleString(const TFileNameChar* Message_);
	inline std::string FileNameToConsoleString(const TFileNameString& Message_) {
		return FileNameToConsoleString(Message_.c_str());
	}

	std::string Utf8ToConsoleString(const char* Message_);
	inline std::string Utf8ToConsoleString(const std::string Message_) {
		return Utf8ToConsoleString(Message_.c_str());
	}

#else
	inline const char* FileNameToConsoleString(const char *Message_) {return Message_;}
	inline const std::string& FileNameToConsoleString(const std::string& Message_) {return Message_;}
	inline const char* Utf8ToConsoleString(const char* Message_) {return Message_;}
	inline const std::string& Utf8ToConsoleString(const std::string& Message_) {return Message_;}
#endif

// ----------------------------------------------------------------------------
size_t FileNameLength(const TFileNameChar *FileName);

void AppendSeparatorIfAbsent(TFileNameString& Value_);
bool FolderExists(const TFileNameChar *Folder_);
bool CreatePath(const TFileNameChar *Path_);
bool FolderEntries(const TFileNameChar *Folder_, std::vector<TFileNameString> &Folders_, std::vector<TFileNameString> &Files_);
bool FileExists(const TFileNameChar *File_);
bool RemoveFile(const TFileNameChar *File_);
bool DuplicateFile(const TFileNameChar *Src_, const TFileNameChar *Dst_);

} // namespace tpcl
