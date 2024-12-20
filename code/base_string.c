#include "base_string.h"

UWORD format(CHAR *buffer, UWORD size, const CHAR *format, ...)
{
	VARGS vargs;
	begin_vargs(vargs, format);
	UWORD result = format_v(buffer, size, format, vargs);
	end_vargs(vargs);
	return result;
}

_IMPORTED_ SIZE _CDECL_ strlen(const CHAR *);

SIZE gauge_string(const CHAR *string)
{
	return strlen(string);
}

// from https://bjoern.hoehrmann.de/utf-8/decoder/dfa/
typedef enum : UHALF
{
	UTF8_accept = 0,
	UTF8_reject = 1,
} UTF8_STATE;

UHALF decode_utf8_(UTF8_STATE *state, UTF32 *character, UTF8 byte)
{
	static const UBYTE table[] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3,
		0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,
		0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,
		1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,
		1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1,
		1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	UHALF type = table[byte];
	*character = *state != UTF8_accept ? byte & 0x3fu | *character << 6 : 0xff >> type & byte;
	*state = table[256 + *state * 16 + type];
	return *state;
}

UNICODE_DECODING decode_utf8(const UTF8 bytes[4])
{
	UNICODE_DECODING decoding = { .increment = 0 };
	UTF8_STATE state = UTF8_accept;
	while(decode_utf8_(&state, &decoding.codepoint, bytes[decoding.increment++]) != UTF8_accept
		&& decoding.increment < 4);
	return decoding;
}
