#pragma once

#define TCPL_LITTLE_ENDIAN 1234  /// IBM PC (lower byte, 2 byte, 3 byte, 4 byte)
#define TCPL_BIG_ENDIAN    4321  /// Network order (4 byte, 3 byte, 2 byte, lower byte)

#ifdef _MSC_VER
   // TCPL_MSC
   #ifndef TCPL_MSC
      #define TCPL_MSC
   #endif

   #if defined(_WIN64)
		// TCPL_OS_WINDOWS
      #ifndef TCPL_OS_WINDOWS
			#define TCPL_OS_WINDOWS
      #endif
		// TCPL_OS_WINDOWS64
      #ifndef TCPL_OS_WINDOWS64
			#define TCPL_OS_WINDOWS64
      #endif
      // TCPL_MSC64
      #ifndef TCPL_MSC64
         #define TCPL_MSC64
      #endif
   #elif defined(_WIN32)
		// TCPL_OS_WINDOWS
      #ifndef TCPL_OS_WINDOWS
			#define TCPL_OS_WINDOWS
      #endif
		// TCPL_OS_WINDOWS32
      #ifndef TCPL_OS_WINDOWS32
			#define TCPL_OS_WINDOWS32
      #endif
      // TCPL_MSC32
      #ifndef TCPL_MSC32
         #define TCPL_MSC32
      #endif
   #endif
#endif // _MSC_VER

#if defined(__MINGW32__)
   // TCPL_MINGW
   #ifndef TCPL_MINGW
      #define TCPL_MINGW
   #endif

   #if defined(__MINGW64__)
      // TCPL_OS_WINDOWS
      #ifndef TCPL_OS_WINDOWS
         #define TCPL_OS_WINDOWS
      #endif
      // TCPL_OS_WINDOWS64
      #ifndef TCPL_OS_WINDOWS64
         #define TCPL_OS_WINDOWS64
      #endif
      // TCPL_MINGW64
      #ifndef TCPL_MINGW64
         #define TCPL_MINGW64
      #endif
   #else // !defined(__MINGW64__)
      // TCPL_OS_WINDOWS
      #ifndef TCPL_OS_WINDOWS
         #define TCPL_OS_WINDOWS
      #endif
      // TCPL_OS_WINDOWS32
      #ifndef TCPL_OS_WINDOWS32
         #define TCPL_OS_WINDOWS32
      #endif
      // TCPL_MINGW32
      #ifndef TCPL_MINGW32
         #define TCPL_MINGW32
      #endif
   #endif
#endif // __MINGW32__

#if defined(__linux__) || defined(__linux)
	// TCPL_OS_LINUX
   #ifndef TCPL_OS_LINUX
		#define TCPL_OS_LINUX
   #endif
	// TCPL_OS_UNIX
   #ifndef TCPL_OS_UNIX
		#define TCPL_OS_UNIX
   #endif
#endif // #if defined(__linux__) || defined(__linux)

#if defined(__GNUC__)
   // TCPL_MINGW
   #ifndef TCPL_GCC
      #define TCPL_GCC
   #endif
#endif

// Byte order
#if defined(TCPL_MSC) || defined(TCPL_MINGW)
    #define TCPL_BYTE_ORDER  TCPL_LITTLE_ENDIAN
#elif defined(TCPL_GCC)
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		#define TCPL_BYTE_ORDER  TCPL_LITTLE_ENDIAN
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		#define TCPL_BYTE_ORDER  TCPL_BIG_ENDIAN
	#else
		#error Unsupported byte order.
	#endif
#endif

namespace tcpl {
#if defined(TCPL_MSC)
	typedef unsigned __int64 	uint64;
	typedef __int64 	int64;
	#define TCPL_INT64_CONSTANT(c_)  c_ ## i64
	#define TCPL_UINT64_CONSTANT(c_)  c_ ## ui64
#elif defined(__GNUC__) || defined(__MINGW32__)
	// GNU C++
	typedef unsigned long long 	uint64;
	typedef long long 	int64;
	#define TCPL_INT64_CONSTANT(c_)  c_ ## LL
	#define TCPL_UINT64_CONSTANT(c_)  c_ ## ULL
#else
   #error Unsupported compiler
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef unsigned char uchar8;

typedef short int16;
typedef unsigned short uint16;
typedef unsigned short ushort;

typedef int int32;
typedef unsigned uint32;

} // namespace tcpl

// C++ 11 support flag
#ifdef _MSC_VER
	#if _MSC_VER >= 1800
		#ifndef TCPL_C11X_SUPPORT
			#define TCPL_C11X_SUPPORT
		#endif
	#endif
#else
	#if __cplusplus > 199711L
		#ifndef TCPL_C11X_SUPPORT
			#define TCPL_C11X_SUPPORT
		#endif
	#endif
#endif

