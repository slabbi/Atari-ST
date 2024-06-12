#include <tos.h>
#include "ccport.h"

word getcookie (ulong cookie,ulong *p_value);

static long *cookiejar;

static long lpeek_cookie(void)
{ cookiejar=*((long **)0x5a0);
	return 0;
}

word getcookie (ulong cookie,ulong *p_value)
{	Supexec(lpeek_cookie);
	if (!cookiejar) return 0;
	do
	{	if (cookiejar[0] == cookie)
		{	if (p_value)
				*p_value = cookiejar[1];
			return 1;
		}
		else
			cookiejar = &(cookiejar[2]);
	}
	while (cookiejar[-2]);
	return 0;
}
