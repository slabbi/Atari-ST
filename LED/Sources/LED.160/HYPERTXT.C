#include "ccport.h"
#include <magx.h>
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"
#include "language.h"


static word test_uproc(byte *txt)
{	DTA dta;
	word rc,id;
	byte tmp[MAX_STR],*cp;
	long x;

	Fsetdta(&dta);
	rc=Fsfirst("U:\\PROC\\*.*",FA_READONLY|FA_HIDDEN|FA_SYSTEM|FA_VOLUME|FA_SUBDIR|FA_ARCHIVE);
	while (!rc)
	{	cp=strchr(dta.d_fname,EOS)-4;
		if (cp>=dta.d_fname && *cp=='.')
		{	*cp=EOS;
			strcpy(tmp,dta.d_fname);
			id=atoi(cp+1);
			
			x=0xffff0000ul | id;
			id=appl_find((char *)x);
			
			if (!stricmp(tmp,txt))
				return(id);
		}
		rc=Fsnext();
	}
	return(-1);
}

static void try_to_unfreeze(word unfreeze_apid)
{	MAGX_COOKIE *MagX;
	ulong value;
	int scr_id;
	MagX = getcookie( 'MagX', &value ) ? (MAGX_COOKIE *) value : NULL;
	if (MagX && MagX->aesvars && MagX->aesvars->version>=0x200)
	{	wind_update(BEG_UPDATE);
		scr_id=appl_find("SCRENMGR");
		if (scr_id>=0)
		{	word xmsgbuff[8];
			xmsgbuff[0]=101;
			xmsgbuff[1]=gl_apid;
			xmsgbuff[2]=
			xmsgbuff[3]=0;
			xmsgbuff[4]='MA';
			xmsgbuff[5]='GX';
			xmsgbuff[6]=4;
			xmsgbuff[7]=unfreeze_apid;
			appl_write(scr_id,16,xmsgbuff);
		}
		wind_update(END_UPDATE);
	}
}

void aufrufbar_machen(byte *prgname)
{	word i;
	i=appl_find(prgname);
	if (i<0)
	{	i=test_uproc(prgname);
		if (i>=0)
			try_to_unfreeze(i);
	}
}

void hypertext(word mode, byte *txt)
{	word i,msgbuff[8];
	byte tmp[MAX_STR],file[MAX_STR];
	struct ffblk dta;

	aufrufbar_machen("ST-GUIDE");

	i=appl_find("ST-GUIDE");
	if (i<0)
	{	sprintf(file,"%sST-GUIDE.APP",gl_ledpath);
		if (findfirst(file,&dta,0))
			LangAlert(MSG_NOSTGUIDE);
		else
		{	if (mode==HYPER_LED)
				strcpy(tmp+1,"*:\\LED.HYP");
			else if (mode==HYPER_LED_KEYS)
				strcpy(tmp+1,"*:\\LED.REF Tastaturbersicht");
			else
				strcpy(tmp+1,txt);
			tmp[0]=(char)strlen(tmp+1);
			Pexec(0,file,tmp,NULL);
			redraw_all();
		}
	}
	else
	{
		if (mode==HYPER_LED)
			strcpy(tmp,"*:\\LED.HYP");
		else if (mode==HYPER_LED_KEYS)
			strcpy(tmp,"*:\\LED.REF Tastaturbersicht");
		else
			strcpy(tmp,txt);
		msgbuff[0]=0x4711;
		msgbuff[1]=gl_apid;
		msgbuff[2]=0;
		msgbuff[3]=(uword) ((ulong)&tmp >> 16);
		msgbuff[4]=(uword) &tmp;
		msgbuff[5]=0;
		msgbuff[6]=0;
		msgbuff[7]= 0;
		appl_write(i,16,&msgbuff);
	}
}
