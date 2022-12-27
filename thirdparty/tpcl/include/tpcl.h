#pragma once

/*
* This file is part of Tiny Platform Crossing Library (TPCL).
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

#include <tpcl-base.h>
#include <string>
#include <cstring>
#include <vector>

namespace tpcl {
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Types
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#if defined(TPCL_OS_WINDOWS)
	#define TPCL_TO_FNSTR(S_) L ## S_
	#define TPCL_FNSTR(S_) TPCL_TO_FNSTR(S_)
	#define TPCL_FNCHAR(S_) TPCL_TO_FNSTR(S_)
	typedef wchar_t TFileNameChar;
	typedef std::wstring TFileNameString;
	#define TPCL_FS_SEPARATOR L'\\'
	#define TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T

	#include <windows.h>
#else
	#define TPCL_FNSTR(S_) S_
	#define TPCL_FNCHAR(S_) S_
	typedef char TFileNameChar;
	typedef std::string TFileNameString;
	#define TPCL_FS_SEPARATOR '/'
	#define TPCL_FILE_NAME_CHAR_TYPE_IS_CHAR
#endif

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// FILE methods helpers
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
inline FILE *fopen(const TFileNameChar *FileName_, const TFileNameChar *Mode_)
{
	#ifdef TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T
		#ifdef _MSC_VER
			#pragma warning(push)
			#pragma warning(disable: 4996)
		#endif
		return _wfopen(FileName_, Mode_);
		#ifdef _MSC_VER
			#pragma warning(pop)
		#endif
	#else
		return fopen(FileName_, Mode_);
	#endif
}

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// File name strings useful operations
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
template <class _T>
inline TFileNameString ToFileNameString(const _T& Value_) {
	#ifdef TPCL_FILE_NAME_CHAR_TYPE_IS_WCHAR_T
		return std::to_wstring(Value_);
	#else
		return std::to_string(Value_);
	#endif
}

// ---
inline TFileNameString FillLeft(const TFileNameString &String_, size_t FieldSize_, TFileNameChar FillChar_) 
{
	if(String_.size() >= FieldSize_) return String_;
	TFileNameString Result(FieldSize_ - String_.size(), FillChar_);
	Result += String_;
	return Result;
}

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// File name string -> utf8
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#if defined(TPCL_OS_WINDOWS)
	// −−−
	struct Utf8Converter {
		// ---
		static std::string fromWide(const wchar_t* Unicode_, int UnicodeSize_ = -1, bool* Ok_ = nullptr) {
			if(UnicodeSize_ < 0) UnicodeSize_ = (int)wcslen(Unicode_);
			std::string RetValue;
			if(!UnicodeSize_) {
				if(Ok_) *Ok_ = true;
				return RetValue;
			}
			int CharsCount = WideCharToMultiByte(CP_UTF8, 0, Unicode_, UnicodeSize_, NULL, 0, NULL, NULL);
			if(CharsCount <= 0) {
				if(Ok_) *Ok_ = false;
				return RetValue;
			}
			//
			RetValue.resize(CharsCount);
			if(WideCharToMultiByte(CP_UTF8, 0, Unicode_, UnicodeSize_, (char*)(RetValue.c_str()), CharsCount, NULL, NULL) <= 0) {
				RetValue.clear();
				if(Ok_) *Ok_ = false;
			}
			else {
				if(Ok_) *Ok_ = true;
			}
			return RetValue;
		}
		// ---
		static inline std::string fromWide(const std::wstring& Name_, bool* Ok_ = nullptr) {
			return fromWide(Name_.c_str(), (int)Name_.size(), Ok_);
		}
		// ---
		static std::wstring toWide(const char* UTF8_, int UTF8Size_ = -1, bool* Ok_ = nullptr) {
			if(UTF8Size_ < 0) UTF8Size_ = (int)strlen(UTF8_);
			std::wstring RetValue;
			if(!UTF8Size_) {
				if(Ok_) *Ok_ = true;
				return RetValue;
			}
			int CharsCount = MultiByteToWideChar(CP_UTF8, 0, UTF8_, UTF8Size_, NULL, 0);
			if(CharsCount <= 0) {
				if(Ok_) *Ok_ = false;
				return RetValue;
			}
			//
			RetValue.resize(CharsCount);
			if(MultiByteToWideChar(CP_UTF8, 0, UTF8_, UTF8Size_, (wchar_t*)(RetValue.c_str()), CharsCount) <= 0) {
				RetValue.clear();
				if(Ok_) *Ok_ = false;
			}
			else {
				if(Ok_) *Ok_ = true;
			}
			return RetValue;
		}
		// ---
		static std::wstring toWide(const std::string& Name_, bool* Ok_ = nullptr) {
			return toWide(Name_.c_str(), (int)Name_.size(), Ok_);
		}
	};

	// ---
	inline std::string FileNameToUtf8(const TFileNameChar* Name_) {return Utf8Converter::fromWide(Name_);}
	inline std::string FileNameToUtf8(const TFileNameString& Name_) {return Utf8Converter::fromWide(Name_);}

	inline std::wstring Utf8ToFileName(const char* Name_, bool* Ok_ = nullptr) {return Utf8Converter::toWide(Name_, Ok_);}
	inline std::wstring Utf8ToFileName(const std::string& Name_, bool* Ok_ = nullptr) {return Utf8Converter::toWide(Name_, Ok_);}

	// ---
	inline void AppendSeparatorIfAbsent(std::string &Name_) {
		if(Name_.empty() || Name_.back() != '\\')
			Name_ += '\\';
	}
	// ---
	inline void AppendSeparatorIfAbsent(std::wstring &Name_) {
		if(Name_.empty() || Name_.back() != L'\\')
			Name_ += L'\\';
	}
#else
	template <typename _T>
	inline const _T& FileNameToUtf8(const _T& Name_) {return Name_;}

	template <typename _T>
	inline const _T& Utf8ToFileName(const _T& Name_, bool* Ok_ = nullptr) {
		if(Ok_) *Ok_ = true;
		return Name_;
	}

	//inline std::string FileNameToUtf8(const TFileNameChar* Name_) {return Name_;}
	//inline std::string FileNameToUtf8(const TFileNameString& Name_) {return Name_;}

	// ---
	inline void AppendSeparatorIfAbsent(std::string &Name_) {
		if(Name_.empty() || Name_.back() != '/')
			Name_ += '/';
	}
#endif

} // namespace tpcl
