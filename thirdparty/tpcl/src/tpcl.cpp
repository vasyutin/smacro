#include <tpcl.h>

namespace tpcl {

// -----------------------------------------------------------------------
size_t FileNameLength(const TFileNameChar* FileName_)
{
#if TPCL_FILE_NAME_CHAR_TYPE == TPCL_WCHAR_T
	return wcslen(FileName_);
#else
	return strlen(FileName_);
#endif
}

// -----------------------------------------------------------------------
void AppendSeparatorIfAbsent(TFileNameString& Value_)
{
	if(Value_[Value_.size() - 1] != TPCL_FS_SEPARATOR)
		Value_ += TPCL_FS_SEPARATOR;
}

// -----------------------------------------------------------------------
bool Utf8ToWide(const char* UTF8_, int UTF8Size_, std::wstring& String_)
{
	return true;
}

// -----------------------------------------------------------------------
bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_)
{
	return true;
}

// -----------------------------------------------------------------------
bool WideToUtf8(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_)
{
	return true;
}

// -----------------------------------------------------------------------
bool WideToLocal(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_)
{
	return true;
}

// -----------------------------------------------------------------------
bool LocalToUtf8(const char* Local8Bit_, int Local8BitSize_, std::string& String_)
{
	return true;
}

// -----------------------------------------------------------------------
bool LocalToWide(const char* UTF8_, int UTF8Size_, std::wstring& String_)
{
	return true;
}

// -----------------------------------------------------------------------
std::string FileNameToConsoleString(const TFileNameChar* Message_)
{
	return std::string();
}

} // namespace tcpl
