#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"
#include "language.h"

uword is_registered=TRUE;

word test_registered(void)
{	char *pointer,tmp[255];
	ulong kuerzel;

	pointer=strupr(strcpy(tmp,akasysop[0]));
	kuerzel =*pointer++;	kuerzel <<= 8;
	kuerzel|=*pointer++;	kuerzel <<= 8;
	if (strchr(pointer,' '))
	{	pointer=strchr(pointer,' ');
		pointer++;
	}
	kuerzel|=*pointer++;	kuerzel <<= 8;
	kuerzel|=*pointer;

	pidserial[2]=(byte) ((kuerzel & 0xff000000ul) >> 24);
	pidserial[3]=(byte) ((kuerzel & 0xff0000ul) >> 16);
	pidserial[4]=(byte) ((kuerzel & 0xff00) >> 8);
	pidserial[5]=(byte) kuerzel;
	pidserial[6]=
	pidserial[7]=
	pidserial[8]=
	pidserial[9]=
	pidserial[10]='\0';
	return(TRUE);
}

word available(byte *txt)
{	byte tmp[MAX_STR];
	if (test_registered())
		if (is_registered)
			return(TRUE);
	sprintf(tmp,msgtxt[MSG_REGISTERFUNCTION].text,txt);
	FormAlert(msgtxt[MSG_REGISTERFUNCTION].defbut,msgtxt[MSG_REGISTERFUNCTION].icon,tmp,msgtxt[MSG_REGISTERFUNCTION].button);
	return(FALSE);
}

void register_speicher_freigeben(void)
{
}
