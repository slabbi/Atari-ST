#include <stddef.h>
#include <stdlib.h>
#include <tos.h>
#include <linea.h>

static int x,y;

static long set_it(void)
	{	linea_init();
		Vdiesc->v_cel_mx = x;
		Vdiesc->v_cel_my = y;
		return(0);
	}

void set(void)
	{	Supexec(set_it);
	}

int main(int argc, const char *argv[])
	{	if (--argc!=2)
			{	Bconout(2,7);
				return(1);
			}
		y=atoi(argv[1])-1;
		x=atoi(argv[2])-1;
		if (x<0 || y<0)
			{	Bconout(2,7);
				return(2);
			}
		set();
		return(0);
	}

