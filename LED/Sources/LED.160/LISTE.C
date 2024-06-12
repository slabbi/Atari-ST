#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"

word ascii2hex(word x)
	{			 if (x>='a') return(x-87);
		else if (x>='A') return(x-55);
		else						 return(x-48);
	}

void buf_to_liste(long win)
{	word i,convert=0;
	word lc;
	word wi_maxcol;
	WIND *winp;
	byte *cp, *oldend;
	byte *ptr, *linep, *hp;

	winp=&wi[win];

			 if (gl_area[winp->aktarea].flag & AR_CONVERT_LAT1S)	convert=LATIN1S+1;
	else if (gl_area[winp->aktarea].flag & AR_CONVERT_LAT1 )	convert=LATIN1 +1;

	if (winp->editierbar)
		wi_maxcol=opt.maxedcol;
	else
	{	if (winp->listwind)
			wi_maxcol=MAXCOL_LISTWIND;
		else
			wi_maxcol=opt.maxcol;
	}
	
	if (wi_maxcol<=10 || wi_maxcol>(word)LINELEN-2)
		wi_maxcol=(word)LINELEN-2;
	
	lc=0;
	winp->lzahl=0;
	*(winp->dup_msgid)=
	*(winp->dup_replyid)=
	*(winp->dup_replyto)=
	*(winp->dup_replyaddr)=EOS;
	
	if (winp->msgbuf!=NULL)
	{
		cp=winp->msgbuf;

		while (*cp && winp->lzahl<MAX_LINES-1)
		{

		/* ssl 281294: ...ich kam, sah, wrgte... ;-)		*/

/* opt.hideeid, opt.hideca */

			if (*cp==1)									/* Ist es ein ^A? */
			{
				ptr=strchr(cp,'\n');			/* Ja, wo ist denn die Zeile zu Ende?	*/
				if (!ptr)
					ptr=strchr(cp,EOS);

				if (ptr!=NULL)						/* Alles hat ein Ende, auch diese Zeile... */
				{	*ptr=EOS;

					if (!strncmp(cp,"\001MSGID:",7))				/* MSGID? */
					{	strcpy(winp->dup_msgid,cp);
						strcat(winp->dup_msgid,"\n");
						if (opt.hideeid)
						{	cp=ptr+1;
							continue;
						}
					}
					else if (!strncmp(cp,"\001REPLY:",7))	/* REPLY?	*/
					{	strcpy(winp->dup_replyid,cp);
						strcat(winp->dup_replyid,"\n");
						if (opt.hideeid)
						{	cp=ptr+1;
							continue;
						}
					}
					else if (!strncmp(cp,"\001REPLYTO",8))	/* REPLYTO?	*/
					{	strcpy(winp->dup_replyto,cp);
						strcat(winp->dup_replyto,"\n");
						if (opt.hideeid)
						{	cp=ptr+1;
							continue;
						}
					}
					else if (!strncmp(cp,"\001REPLYADDR",10))	/* REPLYADDR?	*/
					{	strcpy(winp->dup_replyaddr,cp);
						strcat(winp->dup_replyaddr,"\n");
						if (opt.hideeid)
						{	cp=ptr+1;
							continue;
						}
					}
#if 0
					else if (!strncmp(cp,"\001ALERT:",7))	/* ALERT?	*/
					{	strcpy(winp->alert,cp);
						if (opt.hideca)
						{	cp=ptr+1;
							continue;
						}
					}
#endif
					else if (!strncmp(cp,"\001PID",4) ||
									 !strncmp(cp,"\001TID",4) ||
									 !strncmp(cp,"\001EID",4) )	/* PID etc.? */
					{	if (opt.hidepid)
						{	cp=ptr+1;
							continue;
						}
					}

					else if (opt.umlaut & O_ISOCONV &&
									 (!strncmp(cp,"\001CHRS:",6) ||
									  !strncmp(cp,"\001CHARSET:",9)) )	/* CHARSET? */
					{	hp=cp;
						if (cp[3]=='R')	hp+=6;
						else						hp+=9;
						hp=skip_blanks(hp);
								 if (!strncmp(hp,"LATIN-1 2",9)) convert=LATIN1+1;
						else if (!strncmp(hp,"IBMPC 2"	,7)) convert=0;
						else if (!strncmp(hp,"ATARIST 2",9)) convert=(opt.umlaut & O_ATARIFONT) ? 0 : ATARIST+1;
						else if (!strncmp(hp,"ASCII 2",  7)) convert=0;
						else if (!strncmp(hp,"MAC 2",	   5)) convert=0;
						else if (!strncmp(hp,"VT100 2",  7)) convert=0;
						else if (!strncmp(hp,"FRENCH 1", 8)) convert=100+ISO_FRENSH;
						else if (!strncmp(hp,"GERMAN 1", 8)) convert=100+ISO_GERMAN;
						else if (!strncmp(hp,"ITALIAN 1",9)) convert=100+ISO_ITALIAN;
						else if (!strncmp(hp,"NORWEG 1", 8)) convert=100+ISO_NORWEG;
						else if (!strncmp(hp,"PORTU 1",  7)) convert=100+ISO_PORTU;
						else if (!strncmp(hp,"SPANISH 1",9)) convert=100+ISO_SPANISH;
						else if (!strncmp(hp,"SWEDISH 1",9)) convert=100+ISO_SWEDISH;
						else if (!strncmp(hp,"UK 1",		 4)) convert=100+ISO_UK;

						if (opt.hideca)
						{	cp=ptr+1;
							continue;
						}
					}
					else
					{	if (opt.hideca)
						{	cp=ptr+1;
							continue;
						}
					}
					*ptr='\n';
				}
			} /* while *cp = ^A */

	
			if (opt.hideseen && *cp=='S')
			{
				if (!strncmp(cp,"SEEN-BY",7))
				{
					ptr=strchr(cp,'\n');
					if (ptr!=NULL)
						cp=ptr+1;
					else
						cp=strchr(cp,EOS);
					continue;
				}
			}

			if (*cp)		/* Msg nicht zuende? */
			{
				if ((linep=(char *)calloc(1,LINELEN))==NULL)
				{
					nomemalert(0);
					break;
				}
				winp->lines[lc++]=linep;
				winp->lzahl++;
				ptr=linep;
				
				/* w„hrend wir suchen k”nnen wir auch kopieren */
				while (*cp!='\n' && *cp && ((int)(ptr-linep) < wi_maxcol))
				{
					if (opt.umlaut & O_MIMESTYLE)
					{	if (*cp=='=')
						{	hp=cp+1;
							if (isxdigit(*hp) && isxdigit(hp[1]))
							{	i=(ascii2hex(*hp) << 4) + ascii2hex(hp[1]);
								*cp=i;
								strcpy(hp,hp+2);
							}
						}
					}

					if (convert>=100)
					{	i=convert-100;
						switch(*cp)
						{ case  35	: *cp=iso_convert_table[i][ 0]; break;
							case  36	: *cp=iso_convert_table[i][ 1]; break;
							case  64	: *cp=iso_convert_table[i][ 2]; break;
							case  91	: *cp=iso_convert_table[i][ 3]; break;
							case  92	: *cp=iso_convert_table[i][ 4]; break;
							case  93	: *cp=iso_convert_table[i][ 5]; break;
							case  94	: *cp=iso_convert_table[i][ 6]; break;
							case  96	: *cp=iso_convert_table[i][ 7]; break;
							case 123	: *cp=iso_convert_table[i][ 8]; break;
							case 124	: *cp=iso_convert_table[i][ 9]; break;
							case 125	: *cp=iso_convert_table[i][10]; break;
							case 126	: *cp=iso_convert_table[i][11]; break;
						}
					}
					else if (convert)
						*cp=convert_table[convert-1][*cp];
					else if (opt.umlaut & O_MIMESTYLE)
						*cp=convert_table[LATIN1][*cp];

					if (opt.texteffects & O_TEXTEFFSCR)
						if (*cp>=16 && *cp<=25)
							*cp+=32;

					if (*cp=='\t' && opt.exttab)
					{
						if ((int)(ptr-linep) < LINELEN-opt.tabsize-1)
						{	i=0;
							while (i++ < opt.tabsize)
								*ptr++=' ';
						}
						else
							*ptr++=' ';
						cp++;
					}
					else
						*ptr++=*cp++;
				}
				
				if (*cp=='\n')
 				{
					if (ptr>linep && *(ptr-1)==' ')
						*(ptr-1)='\n';
					else
						*ptr++='\n';
					*ptr=EOS;
					cp++;
				}
				else if (*cp)
				{
					oldend=cp;
					*ptr=EOS;
					while (*ptr!=' ' && ptr>linep)
					{
						ptr--;
						cp--;
					}
					
					if (ptr>linep)
					{
						*(++ptr)=EOS;
						cp++;
					}
					else
						cp=oldend;
					
					while (*cp==' ')
						cp++;
					if (*cp=='\n')
						cp++;
				}
			}
		} /* while *cp != EOS */
		
		free(winp->msgbuf);
		winp->msgbuf=NULL;
	}
	winp->lines[lc]=NULL;
}

