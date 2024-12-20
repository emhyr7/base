#include "base_string.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

UWORD format_v(CHAR *buf, UWORD count, const CHAR *fmt, VARGS va)
{
	stbsp__context c;

	if ( (count == 0) && !buf )
	{
		c.length = 0;

		STB_SPRINTF_DECORATE( vsprintfcb )( stbsp__count_clamp_callback, &c, c.tmp, fmt, va );
	}
	else
	{
		c.buf = buf;
		c.count = count;
		c.length = 0;

		STB_SPRINTF_DECORATE( vsprintfcb )( stbsp__clamp_callback, &c, stbsp__clamp_callback(0,&c,0), fmt, va );
	}

	return c.length;
}
