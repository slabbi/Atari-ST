#include <tos.h>
#include <ext.h>
#include <stdio.h>
#include <aes.h>
#include "define.h"
#include "vars.h"

/* int pause2(void)
	{	int which,msgbuf[8],mx,my,mb,ks,kr,br;
		wind_update(BEG_UPDATE);
		which = evnt_multi(MU_TIMER|MU_KEYBD,2,1,1,0,0,0,0,0,0,0,0,0,0,msgbuf,
						 										  100,0,&mx,&my,&mb,&ks,&kr,&br);
		wind_update(END_UPDATE);
		if (which & MU_KEYBD)
			return(kr);
		else
			return(0);
	}
*/
long inkey(void)								/* Nur unter TOS-Umgebung			*/
	{	return(Crawio(0xff));				/* |xxxxSCAN|xxxxCHAR|				*/
	}

long evnt_inkey(void)						/* Inkey unter GEM-Umgebung		*/
	{															/* |xxxxSCAN|xxxxCHAR|				*/
		int ret=0,d,s,xx[8];
		if (Bconstat(2))
			{	if (evnt_multi(MU_KEYBD|MU_TIMER,2,1,1,0,0,0,0,0,0,0,0,0,0,
      													xx,1,0,&d,&d,&d,&d,&s,&d) & MU_KEYBD)
					{	wind_get(handle,WF_TOP,&d,0,0,0);
			  		if (handle==d)
							ret = s & 0xff;
					}
			}
		return(ret);
/*	return(Crawio(0xff));
*/
	}

int	pause(void)									/* Unter GEM-Umgebung					*/
	{	register int i;
/*	int xx[8],x,y,d,k,s,b; */
		wind_update(BEG_UPDATE);

/*	if (evnt_multi(MU_KEYBD|MU_BUTTON,1,1,1,0,0,0,0,0,0,0,0,0,0,
        xx,0,0,&x,&y,&b,&d,&s,&k) & MU_KEYBD)
    	i = k & 0xff;
*/
		i = evnt_keybd() & 0xff;
		wind_update(END_UPDATE);
		return(i);
	}

long length_of_file(FILE *fp,int rew)
	{	signed long l;
		l = filelength(fp->Handle);
		if (l<=0)
			{	fseek(fp,0L,SEEK_END);
				l = ftell(fp);
				if (rew)
					rewind(fp);
			}
		return(l);
	}

void set_defaultdir(void)
	{	Dsetdrv(def_drive);
		Dsetpath(def_path_slash);
	}
