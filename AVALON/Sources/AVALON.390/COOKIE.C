#include <tos.h>

int getcookie (long cookie,long *p_value);

int getcookie (long cookie,long *p_value)
	{	long oldstack = 0L,*cookiejar;
		if (!Super ((void *)1L)) oldstack = Super (0L);
		cookiejar = *((long **)0x5a0L);
		if (oldstack) Super ((void *)oldstack);
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
