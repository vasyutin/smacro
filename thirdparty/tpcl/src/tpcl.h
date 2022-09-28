#pragma once

#include <tpclbase.h>
#include <string>

namespace tpcl {
// -----------------------------------------------------------------------
#if defined(TPCL_MSC)
	#define TPCL_TO_FNSTR(S_) L ## S_
	#define TPCL_FNSTR(S_) TPCL_TO_FNSTR(S_)
	typedef wchar_t TFileNameChar;
	typedef std::wstring TFileNameString;
	#define TPCL_FS_SEPARATOR L'\\'
	#define TPCL_FILE_NAME_CHAR_TYPE TPCL_WCHAR_T
#else
	#define TPCL_FNSTR(S_) S_
	typedef char TFileNameChar;
	typedef std::string TFileNameString;
	#define TPCL_FS_SEPARATOR '/'
	#define TPCL_FILE_NAME_CHAR_TYPE TPCL_CHAR
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
	bool LocalToWide(const char* UTF8_, int UTF8Size_, std::wstring& String_);
	inline bool LocalToWide(const char* UTF8_, std::wstring& String_) {return LocalToWide(UTF8_, -1, String_);}
	inline bool LocalToWide(const std::string& UTF8_, std::wstring& String_) { return LocalToWide(UTF8_.c_str(), (int)UTF8_.size(), String_); }

	inline std::wstring LocalToWideString(const char* UTF8_, int UTF8Size_, bool* Ok_ = nullptr) {
		std::wstring RetValue;
		bool IsOk = Utf8ToWide(UTF8_, UTF8Size_, RetValue);
		if (Ok_) *Ok_ = IsOk;
		return RetValue;
	}
	inline std::wstring LocalToWideString(const char* UTF8_, bool* Ok_ = nullptr) {return LocalToWideString(UTF8_, -1, Ok_);}
	inline std::wstring LocalToWideString(const std::string& String_, bool* Ok_ = nullptr) { return LocalToWideString(String_.c_str(), (int)String_.size(), Ok_); }

	// ---
	// FileName -> Default console encoding
	// ---
	std::string FileNameToConsoleString(const TFileNameChar* Message_);
	inline std::string FileNameToConsoleString(const TFileNameString& Message_) {
		return FileNameToConsoleString(Message_.c_str());
	}

#else
	inline const char* FileNameToConsoleString(const char *Message_) {return Message_;}
	inline const std::string& FileNameToConsoleString(const std::string& Message_) {return Message_;}
#endif

// ----------------------------------------------------------------------------
size_t FileNameLength(const TFileNameChar *FileName);
void AppendSeparatorIfAbsent(TFileNameString& Value_);

} // namespace tpcl
