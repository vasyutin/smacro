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

#define TPCL_LITTLE_ENDIAN 1234  /// IBM PC (lower byte, 2 byte, 3 byte, 4 byte)
#define TPCL_BIG_ENDIAN    4321  /// Network order (4 byte, 3 byte, 2 byte, lower byte)

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Platform and compiler
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#ifdef _MSC_VER
   // TPCL_MSC
   #ifndef TPCL_MSC
      #define TPCL_MSC
   #endif
	// TPCL_OS_WINDOWS
   #ifndef TPCL_OS_WINDOWS
   	#define TPCL_OS_WINDOWS
   #endif

   #if defined(_WIN64)
      // TPCL_64BIT
      #ifndef TPCL_64BIT
         #define TPCL_64BIT
      #endif
   #elif defined(_WIN32)
      // TPCL_32BIT
      #ifndef TPCL_32BIT
         #define TPCL_32BIT
      #endif
   #endif
   // _MSC_VER
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#elif defined(__MINGW32__)
   // TPCL_MINGW
   #ifndef TPCL_MINGW
      #define TPCL_MINGW
   #endif
   // TPCL_OS_WINDOWS
   #ifndef TPCL_OS_WINDOWS
      #define TPCL_OS_WINDOWS
   #endif

   #if defined(__MINGW64__)
      // TPCL_64BIT
      #ifndef TPCL_64BIT
         #define TPCL_64BIT
      #endif
   #else // !defined(__MINGW64__)
      // TPCL_32BIT
      #ifndef TPCL_32BIT
         #define TPCL_32BIT
      #endif
   #endif
   // __MINGW32__
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#elif defined(__GNUC__)
   // TPCL_MINGW
   #ifndef TPCL_GCC
      #define TPCL_GCC
   #endif

   #if defined(_WIN64)
		// TPCL_OS_WINDOWS
      #ifndef TPCL_OS_WINDOWS
			#define TPCL_OS_WINDOWS
      #endif
      // TPCL_64BIT
      #ifndef TPCL_64BIT
         #define TPCL_64BIT
      #endif
   #elif defined(_WIN32)
		// TPCL_OS_WINDOWS
      #ifndef TPCL_OS_WINDOWS
			#define TPCL_OS_WINDOWS
      #endif
      // TPCL_32BIT
      #ifndef TPCL_32BIT
         #define TPCL_32BIT
      #endif
	#elif defined(__linux__) || defined(__linux)
	   // TPCL_OS_LINUX
      #ifndef TPCL_OS_LINUX
		   #define TPCL_OS_LINUX
      #endif
	   // TPCL_OS_UNIX
      #ifndef TPCL_OS_UNIX
		   #define TPCL_OS_UNIX
      #endif

      #if defined(__LP64__)
         // TPCL_64BIT
         #ifndef TPCL_64BIT
            #define TPCL_64BIT
         #endif
      #else
         // TPCL_32BIT
         #ifndef TPCL_32BIT
            #define TPCL_32BIT
         #endif
      #endif
   #endif
#else
   #error The platform is now unsupported by TPCL
#endif 

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Byte order
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
#if defined(TPCL_MSC) || defined(TPCL_MINGW)
    #define TPCL_BYTE_ORDER  TPCL_LITTLE_ENDIAN
#elif defined(TPCL_GCC)
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		#define TPCL_BYTE_ORDER  TPCL_LITTLE_ENDIAN
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		#define TPCL_BYTE_ORDER  TPCL_BIG_ENDIAN
	#else
		#error Unsupported byte order.
	#endif
#endif

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Standard types
// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
namespace tpcl {
#if defined(TPCL_MSC)
	typedef unsigned __int64 	uint64;
	typedef __int64 	int64;
	#define TPCL_INT64_CONSTANT(c_)  c_ ## i64
	#define TPCL_UINT64_CONSTANT(c_)  c_ ## ui64
#elif defined(__GNUC__) || defined(__MINGW32__)
	// GNU C++
	typedef unsigned long long 	uint64;
	typedef long long 	int64;
	#define TPCL_INT64_CONSTANT(c_)  c_ ## LL
	#define TPCL_UINT64_CONSTANT(c_)  c_ ## ULL
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

