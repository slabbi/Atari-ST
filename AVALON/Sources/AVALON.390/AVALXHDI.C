#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>

#include "cookie.h"
#include "avalxhdi.h"

#define XHDIMAGIC 0x27011992L

typedef long cdecl (*cookie_fun)(int opcode,...);

static cookie_fun get_fun_ptr (void)
{	static cookie_fun XHDI = NULL;
	static int have_it = 0;
	if (!have_it)
		{	long *magic_test;
			getcookie ('XHDI', (long *)&XHDI);
			have_it = 1;
			magic_test = (long *)XHDI;
			if (magic_test && (magic_test[-1] != XHDIMAGIC))
				XHDI = NULL;
		}
	return XHDI;
}

int XHGetVersion (void)
{	cookie_fun XHDI = get_fun_ptr ();
	long oldstack = 0;
	int ret = 0;
	if (!Super ((void *)1L)) oldstack = Super (0L);
	if (XHDI)
		ret = (int) XHDI (0);
	if (oldstack) Super ((void *)oldstack);
	return ret;
}

long XHInqTarget (int major,int minor,long *block_size,long *device_flags,char *product_name)
{	cookie_fun XHDI = get_fun_ptr ();
	long oldstack = 0,ret;
	if (!Super ((void *)1L)) oldstack = Super (0L);
	ret = XHDI (1,major,minor,block_size,device_flags,product_name);
	if (oldstack) Super ((void *)oldstack);
	return ret;
}

long XHStop (int major,int minor,int do_stop,int key)
{	cookie_fun XHDI = get_fun_ptr ();
	long oldstack = 0,ret;
	if (!Super ((void *)1L)) oldstack = Super (0L);
	ret = XHDI (4,major,minor,do_stop,key);
	if (oldstack) Super ((void *)oldstack);
	return ret;
}
