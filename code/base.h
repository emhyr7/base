#if !defined(__INCLUDED_BASE_H__)
#define __INCLUDED_BASE_H__

#if defined(__cplusplus)
	#define __LANGUAGE_CPP__
#else
	#define __LANGUAGE_C__
#endif

// platform

#if defined(__clang__)
	#define __COMPILER_CLANG__
#endif
#if defined(_MSC_VER)
	#define __COMPILER_MSVC__
#endif
#if defined(__GNUC__)
	#define __COMPILER_GNUC__
#endif

#if defined(__amd64__)
	#define __ARCHITECTURE_AMD64__
#else
	#error unknown architecture
#endif

#if defined(__BIG_ENDIAN__)
	#define __ENDIANNESS_BIG__
#elif defined(__LITTLE_ENDIAN__)
	#define __ENDIANNESS_LITTLE__
#else
	#error unknown endianness
#endif

#if defined(_WIN64)
	#define __OS_WINDOWS__
#elif defined(__linux__)
	#define __OS_LINUX__
#else
	#error unknown OS
#endif

// linkage

#if defined(__LANGUAGE_CPP__)
	#define _DEMANGLED_ extern "C"
#else
	#define _DEMANGLED_
#endif

#if defined(__COMPILER_MSVC__)
	#define _EXPORTED_ _DEMANGLED_ __declspec(dllexport)
	#define _IMPORTED_ _DEMANGLED_ __declspec(dllimport)
	#define _CDECL_    __cdecl
	#define _STDCALL_  __stdcall
	#define _FASTCALL_ __fastcall
	#define _INLINE_   static  __forceinline
#else
	#define _EXPORTED_ _DEMANGLED_ __dllexport__
	#define _IMPORTED_ _DEMANGLED_ __dllimport__
	#define _CDECL_    __cdecl__
	#define _STDCALL_  __stdcall__
	#define _FASTCALL_ __fastcall__
	#define _INLINE_   static __always_inline__
#endif

// keywords

#define countof(...)           (sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))
#define offsetof(type, member) ((UINTPTR)&((type *)0)->member - 1)

#if __STDC_VERSION__ < 202302L
	#define alignof(...) _Alignof(__VA_ARGS__)
	#define alignas(...) _Alignas(__VA_ARGS__)

	#if defined(__COMPILER_MSVC__)
		#define thread_local __declspec(thread)
	#else
		#define thread_local __thread
	#endif
#endif

// types

#if defined(__OS_WINDOWS__)
	#define _LONG_ long long
#else
	#define _LONG_ long
#endif

#if defined(__ARCHITECTURE_AMD64__)
	typedef void VOID;

	typedef unsigned char     UINT8;
	typedef unsigned short    UINT16;
	typedef unsigned int      UINT32;
	typedef unsigned _LONG_   UINT64;
	typedef unsigned __int128 UINT128;
	
	typedef signed char     SINT8;
	typedef signed short    SINT16;
	typedef signed int      SINT32;
	typedef signed _LONG_   SINT64;
	typedef signed __int128 SINT128;

	typedef _Float16 FLOAT16;
	typedef float    FLOAT32;
	typedef double   FLOAT64;

	typedef SINT8   BIT;
	typedef SINT8   BIT8;
	typedef SINT16  BIT16;
	typedef SINT32  BIT32;
	typedef SINT64  BIT64;
	typedef SINT128 BIT128;

	typedef BIT8   BYTE;
	typedef BIT16  HALF;
	typedef BIT32  WORD;
	typedef BIT64  LONG;
	typedef BIT128 HUGE;

	typedef UINT8   UBYTE;
	typedef UINT16  UHALF;
	typedef UINT32  UWORD;
	typedef UINT64  ULONG;
	typedef UINT128 UHUGE;

	typedef SINT8   SBYTE;
	typedef SINT16  SHALF;
	typedef SINT32  SWORD;
	typedef SINT64  SLONG;
	typedef SINT128 SHUGE;

	typedef ULONG SIZE;

	typedef ULONG UINTPTR;
	typedef SLONG SINTPTR;

	typedef char      CHAR;
	typedef __wchar_t WCHAR;
#endif

#define _WIDEN_TEXT__(s) L ## s
#define _WIDEN_TEXT_(s)  _WIDEN_TEXT__(s)

#if defined(__COMPILER_MSVC__)
	#define _BREAK_() __debugbreak()
#else
	#define _BREAK_() __builtin_debugtrap()
#endif

#if defined(__COMPILER_MSVC__)
	//_IMPORTED_ VOID _CDECL_ _wassert(const WCHAR *, const WCHAR *, UWORD);

	//#define assert(e) (VOID)(!!(e) || (_wassert((const WCHAR *)_WIDEN_TEXT_(#e), (const WCHAR *)_WIDEN_TEXT_(__FILE__), (UWORD)(__LINE__)), 0))
	#define assert(e) (VOID)(!!(e) || (_BREAK_(), 0))
#else
	#define assert(e) (VOID)(!!(e) || (_BREAK_(), 0))
#endif

#endif
