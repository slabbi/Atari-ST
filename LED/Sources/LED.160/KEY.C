#include "ccport.h"
#include "ledgl.h"

#define CONTROL	    0x004
#define ALTERNATE		0x008
#define LSHIFT			0x002
#define RSHIFT			0x001


ubyte sp_keytab[] = 
	{ 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44,
	  0x0e, 0x53, 0x62, 0x61, 0x52, 0x48, 0x47, 0x4b, 0x50, 0x4d,
	  0x1c, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x4a, 0x6a,
	  0x6b, 0x6c, 0x4e, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x54,
	  0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x0f };

word  is_specialkey(word scancode)
{	word i;
	for (i=0;i<50;i++)
		if (scancode == sp_keytab[i])
			return(TRUE);
	return FALSE;		
}

word get_code(word key, word state)
{	ubyte ascii_byte, scan_byte;		/* low  = ascii_byte -> ASCII */
																	/* high = scan_byte -> Scancode */
	word ret;

/*	printf(" key<%04x> ",key);*/
	
	ascii_byte=(ubyte) key;
	scan_byte=(ubyte) (key>>8);
	
	ret=key;

	if ( is_specialkey(scan_byte) ) /* look in special-key-tab */
	{
		if (state & ALTERNATE)
		{
			if (scan_byte>=0x3b && scan_byte<=0x44 ||			/* F1-F10 */
					scan_byte==0x1c)													/* RETURN	*/
			{	ret=scan_byte+0x2d;
				ret<<=8;
			}
			else if (scan_byte>=0x67 && scan_byte<=0x70)	/* NUM0-NUM9	*/
			{	ret=scan_byte+0x2d;
				ret<<=8;
				ret+=ascii_byte;
			}
		}
		else if (state & CONTROL)
		{
			if (scan_byte>=0x3b && scan_byte<=0x44 ||			/* F1-F10		*/
					scan_byte==0x48 ||												/* CUR-UP		*/
					scan_byte==0x50 ||												/* CUR-DOWN	*/
					scan_byte==0x62)													/* HELP			*/
			{	ret=scan_byte+0x45;
				ret<<=8;
			}
		}
		else if (state & (LSHIFT|RSHIFT))
		{
			if (scan_byte>=0x67 && scan_byte<=0x71 ||			/* NUM0-NUM9,NUM. */
					scan_byte==0x53 ||												/* DELETE					*/
					scan_byte==0x62)													/* HELP						*/
			{	ret=(scan_byte << 8) + ascii_byte+0x80;
			}
		}
	}
	else if (state & ALTERNATE)
	{
		if (ascii_byte==0)  /* Alternate without ASCII */
		{
			if (scan_byte > 0x72) /* ALT 1 - 0 */
			{
				scan_byte -= 0x76;
			}
			if (state & (LSHIFT|RSHIFT))   /* Shift */
				ret=gl_key_p->shift[scan_byte];
			else
				ret=gl_key_p->unshift[scan_byte];
			ret<<=8;
			ret++;
		}
	}
	else if (state & CONTROL)
	{
		if ( ascii_byte > 0 && ascii_byte <= 0x1a ) /* CTRL-A -> CTRL-Z */
		{
			ret=gl_key_p->unshift[scan_byte];
			if (ret >= 'a' || ret <= 'z')
			{	ret=ascii_byte;
				if (state & (LSHIFT|RSHIFT))
					ret=0x0100|ascii_byte;
			}
		}
	}
	else
		ret=ascii_byte;
	
/*	printf(" ret<%04x> ",ret);*/
	return (ret);	
}

