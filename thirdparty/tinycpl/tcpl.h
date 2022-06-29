#pragma once

#include <tcpl-base.h>
#include <string>

namespace tcpl {
// -----------------------------------------------------------------------
#if defined(TCPL_MSC)
	#define TCPL_TO_FNSTR(S_) L ## S_
	#define TCPL_FNSTR(S_) TCPL_TO_FNSTR(S_)
	typedef wchar_t TFileNameChar;
	typedef std::wstring TFileNameString;
	#define TCPL_FS_SEPARATOR L'\\'
	#define TCPL_FILE_NAME_CHAR_TYPE TCPL_WCHAR_T
#else
	#define TCPL_FNSTR(S_) S_
	typedef char TFileNameChar;
	typedef std::string TFileNameString;
	#define TCPL_FS_SEPARATOR '/'
	#define TCPL_FILE_NAME_CHAR_TYPE TCPL_CHAR
#endif

// ----------------------------------------------------------------------------
#if defined(TCPL_OS_WINDOWS)
	// ---
	// utf8 -> wchar_t
	// ---
	bool Utf8ToWide(const char* UTF8_, int UTF8Size_, std::wstring& String_);

	inline bool Utf8ToWide(const std::string &UTF8_, std::wstring& String_) {return Utf8ToWide(UTF8_.c_str(), (int)UTF8_.size(), String_);}

	std::wstring Utf8ToWideString(const char* UTF8_, int UTF8Size_, bool* Ok_ = nullptr) {
		std::wstring RetValue;
		bool IsOk = Utf8ToWide(UTF8_, UTF8Size_, RetValue);
		if(Ok_) *Ok_ = IsOk;
		return RetValue;
	}

	inline std::wstring Utf8ToWideString(const std::string& String_, bool* Ok_ = nullptr) { return Utf8ToWideString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// utf8 -> local 8 bit
	// ---
	bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_);

	inline bool Utf8ToLocal(const std::string& UTF8_, std::string& String_) { return Utf8ToLocal(UTF8_.c_str(), (int)UTF8_.size(), String_); }

	std::string Utf8ToLocalString(const char* UTF8_, int UTF8Size_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = Utf8ToLocal(UTF8_, UTF8Size_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}

	inline std::string Utf8ToLocalString(const std::string& String_, bool* Ok_ = nullptr) { return Utf8ToLocalString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// wchar_t -> utf8
	// ---
	bool WideToUtf8(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_);

	inline bool WideToUtf8(const std::wstring &WString_, std::string& String_) {return WideToUtf8(WString_.c_str(), (int)WString_.size(), String_);}

	std::string WideToUtf8String(const wchar_t* Unicode_, int UnicodeSize_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = WideToUtf8(Unicode_, UnicodeSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}

	inline std::string WideToUtf8String(const std::wstring& String_, bool* Ok_ = nullptr) { return WideToUtf8String(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// wchar_t -> local 8 bit
	// ---
	bool WideToLocal(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_);

	inline bool WideToLocal(const std::wstring& WString_, std::string& String_) { return WideToLocal(WString_.c_str(), (int)WString_.size(), String_); }

	std::string WideToLocalString(const wchar_t* Unicode_, int UnicodeSize_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = WideToLocal(Unicode_, UnicodeSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}

	inline std::string WideToLocalString(const std::wstring& String_, bool* Ok_ = nullptr) { return WideToLocalString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// local 8 bit -> utf8
	// ---
	bool LocalToUtf8(const char* Local8Bit_, int Local8BitSize_, std::string& String_);

	inline bool LocalToUtf8(const std::string& LocalString_, std::string& String_) { return LocalToUtf8(LocalString_.c_str(), (int)LocalString_.size(), String_); }

	std::string LocalToUtf8String(const char* Local8Bit_, int Local8BitSize_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = LocalToUtf8(Local8Bit_, Local8BitSize_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}

	inline std::string LocalToUtf8String(const std::string& String_, bool* Ok_ = nullptr) { return LocalToUtf8String(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// local 8 bit -> wchar_t
	// ---
	bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_);

	inline bool Utf8ToLocal(const std::string& UTF8_, std::string& String_) { return Utf8ToLocal(UTF8_.c_str(), (int)UTF8_.size(), String_); }

	std::string Utf8ToLocalString(const char* UTF8_, int UTF8Size_, bool* Ok_ = nullptr) {
		std::string RetValue;
		bool IsOk = Utf8ToLocal(UTF8_, UTF8Size_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}

	inline std::string Utf8ToLocalString(const std::string& String_, bool* Ok_ = nullptr) { return Utf8ToLocalString(String_.c_str(), (int)String_.size(), Ok_); }

	const char* FileNameToConsoleString(const TFileNameChar* Message_);
	const std::string& FileNameToConsoleString(const TFileNameString& Message_);

#else
	inline const char* FileNameToConsoleString(const char *Message_) {return Message_;}
	inline const std::string& FileNameToConsoleString(const std::string& Message_) {return Message_;}
#endif

// ----------------------------------------------------------------------------
size_t FileNameLength(const TFileNameChar *FileName);
void AppendSeparatorIfAbsent(TFileNameString& Value_);

} // namespace tcpl
