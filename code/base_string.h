#if !defined(__INCLUDED_BASE_STRING_H__)
#define __INCLUDED_BASE_STRING_H__

#include "base.h"

UWORD format_v(CHAR *buffer, UWORD size, const CHAR *format, VARGS vargs);
UWORD format  (CHAR *buffer, UWORD size, const CHAR *format, ...);

SIZE gauge_string(const CHAR *string);

typedef CHAR UTF8;
#if defined(__COMPILER_MSVC__)
	typedef WCHAR UTF16;
#else
	typedef UINT16 UTF16;
#endif
typedef UINT32 UTF32;

typedef struct
{
	UTF32 codepoint;
	WORD  increment;
} UNICODE_DECODING;

UNICODE_DECODING decode_utf8(const UTF8 bytes[4]);

#endif
