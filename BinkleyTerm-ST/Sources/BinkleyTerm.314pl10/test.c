#include <stdio.h>
#include <stddef.h>
#include <portab.h>
#include <time.h>
#include <stdlib.h>
#include <tos.h>

#ifndef min
#define min(a,b)	 ((a)<=(b)?(a):(b))
#endif

typedef int BOOLEAN;

int main(void)
	{	PEEKBYTE();
		return 0;
	}
	
BOOLEAN OUT_EMPTY(void)
{
	IOREC *buf = Iorec(0);		/* Get the RS232 I/O record */
	buf++;								/* and Advance to Output buffer */

	/* If head==TAIL then buffer is empty */

	return (buf->ibufhd == buf->ibuftl);
}


void CLEAR_INBOUND(void)
{
	IOREC *buf = Iorec(0);
	buf->ibuftl = buf->ibufhd;		/* Set tail to head (int changes tl) */
}


int PEEKBYTE(void)
{
	register IOREC *rec;

	rec = Iorec(0);			/* Get the I/O Record */

	if(rec->ibufhd == rec->ibuftl)
		return -1;
	else
	{
		/* get the character from the buffer (head+1) */

		short i = rec->ibufhd + 1;

		if(i >= rec->ibufsiz)
			i = 0;

		return (int) (((unsigned char *)rec->ibuf)[i]);
	}
}

