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
}

// -----------------------------------------------------------------------
bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_)
{
}

// -----------------------------------------------------------------------
bool WideToUtf8(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_)
{
}

// -----------------------------------------------------------------------
bool WideToLocal(const wchar_t* Unicode_, int UnicodeSize_, std::string& String_)
{
}

// -----------------------------------------------------------------------
bool LocalToUtf8(const char* Local8Bit_, int Local8BitSize_, std::string& String_)
{
}

// -----------------------------------------------------------------------
bool Utf8ToLocal(const char* UTF8_, int UTF8Size_, std::string& String_)
{
}

// -----------------------------------------------------------------------
const char* FileNameToConsoleString(const TFileNameChar* Message_)
{
}

// -----------------------------------------------------------------------
const std::string& FileNameToConsoleString(const TFileNameString& Message_)
{
}

/*
static bool normalizeFolderName(TFileNameString& Value_) {
	assert(!Value_.empty());
#if defined(SMACRO_WINDOWS)
	Value_ = AbsolutePath(Value_);
	if (Value_.empty()) {
		std::cerr << "Path '" << FileNameStringToConsole(Value_) << "' is invalid.";
		return false;
	}
#endif
	if (Value_[Value_.size() - 1] != DIR_SEPARATOR)
		Value_ += DIR_SEPARATOR;
	return true;
}
*/


} // namespace tcpl
