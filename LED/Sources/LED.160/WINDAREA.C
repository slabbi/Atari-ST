#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "language.h"

void update_arealistwindow(long win, word aktarea)
{	memset(wi[win].lines[aktarea],' ',6);
	if (gl_area[aktarea].newmsg & NEWMSG)		wi[win].lines[aktarea][0]='*';
	if (gl_area[aktarea].newmsg & UNRDMSG)	wi[win].lines[aktarea][1]='/';
	if (gl_area[aktarea].newmsg & TOSYSOP)	wi[win].lines[aktarea][2]='#';
	if (gl_area[aktarea].flag & AR_MAUS)		wi[win].lines[aktarea][4]='M';
	if (gl_area[aktarea].flag & AR_NOEXPORT)wi[win].lines[aktarea][5]='x';
}

word make_arealist(long newwin)
{	char tmp[8],temp[1024],temp2[MAX_STR];
	int	i,maxlen,len;

	memset(wi[newwin].lines,0,sizeof(wi[newwin].lines));
	maxlen=0;

	for (i=0;i<gl_arzahl;i++)
		if ((len=(int)strlen(gl_area[i].name))>maxlen)
			maxlen=len;
	for (i=0;i<gl_arzahl;i++)
	{	strcpy(tmp,"      ");
		if (gl_area[i].newmsg & NEWMSG)		tmp[0]='*';
		if (gl_area[i].newmsg & UNRDMSG)	tmp[1]='/';
		if (gl_area[i].newmsg & TOSYSOP)	tmp[2]='#';
		if (gl_area[i].flag & AR_MAUS)		tmp[4]='M';
		if (gl_area[i].flag & AR_NOEXPORT)tmp[5]='x';

		if (gl_area[i].flag & AR_FOLLOWUP)
		{	strcpy(temp2," ->");
			strcat(temp2,gl_area[i].followup);
			strcat(temp2,"  ");
		}
		else
		{	*temp2=EOS;
		}

		if (opt.showdesc && gl_area[i].desc)
			sprintf(temp,"%s %-*.*s%s %s",tmp,
																		maxlen+1,maxlen+1,
																		gl_area[i].name,
																		temp2,
																		gl_area[i].desc);
		else
			sprintf(temp,"%s %s%s",	tmp,
															gl_area[i].name,
															temp2);
		wi[newwin].lines[i]=strdup(temp);
	}
	return(0);
}

long openarea_listwindow(long win, word arindex, word changeflag)
{	HDR  *hp;
	byte win_titel[MAX_STR];

	win=win;

	biene();
	wi[AREAWIND].changearea=changeflag;
	
	if (wi[AREAWIND].belegt && wi[AREAWIND].listwind==WI_LIST_AREAS)
	{	mouse_normal();
		if (arindex >= 0)
		{	printline(AREAWIND,wi[AREAWIND].selline,FALSE);			/* ssl 91294: Bei Žnderung von Open->Change */
			wi[AREAWIND].selline=arindex;
			printline(AREAWIND,wi[AREAWIND].selline,TRUE);
		}
		top_it(AREAWIND);
		set_wiclip(AREAWIND);				/* ssl 101294 */
		set_wifont(AREAWIND);

		areawind_successiv();
		return TRUE;
	}

	hp=calloc(1,sizeof(HDR));
	if (hp!=NULL)
		wi[AREAWIND].hdr=hp;	
	else
	{	LangAlert(MSG_NOMEMNEWHEADER);
		return(-1);	
	}

#if 0
	if (opt.winprm[AREAWIND][0]>=0 && opt.winprm[AREAWIND][1]>=0 &&		/* ssl 91294 */
			opt.winprm[AREAWIND][2]>=0 && opt.winprm[AREAWIND][3]>=0)
#endif
		wi[AREAWIND].px=-1;

	strcpy(win_titel,msgtxt[DIV_AREAS].text);
	open_window(AREAWIND,win_titel,FALSE);	

	make_arealist(AREAWIND);
	wi[AREAWIND].lzahl=gl_arzahl;
	wi[AREAWIND].ramhdr=FALSE;
	wi[AREAWIND].hdrptr=wi[AREAWIND].hdrbuf=NULL;
	wi[AREAWIND].hdrlen=0L;

	wi[AREAWIND].listwind=WI_LIST_AREAS;
	wi[AREAWIND].topmsg_line=0;
	wi[AREAWIND].oldarea=
	wi[AREAWIND].aktarea;
	wi[AREAWIND].msgfp=
	wi[AREAWIND].hdrfp=NULL;
	wi[AREAWIND].selline=
	wi[AREAWIND].topline=0;
	if (arindex >= 0L)
		wi[AREAWIND].selline=wi[AREAWIND].topline=arindex;
	wi[AREAWIND].cy=
	wi[AREAWIND].cx=0;

	areawind_successiv();
	mouse_normal();	
	return (AREAWIND);
}

void update_areawindow(word newarea)
{
	if (wi[AREAWIND].belegt && wi[AREAWIND].listwind==WI_LIST_AREAS)
	{	
		printline(AREAWIND,wi[AREAWIND].selline,FALSE);
		set_wiclip(AREAWIND);													/* ssl 111294 */
		set_wifont(AREAWIND);
			
		if (newarea<wi[AREAWIND].topline)
		{	wi[AREAWIND].topline=newarea;
			wi[AREAWIND].selline=newarea;
			wisetfiles(AREAWIND,FALSE,NO_CHANGE);
		}

		if (newarea>=wi[AREAWIND].topline+wi[AREAWIND].maxshow)
		{	set_wislider(AREAWIND);
			wi[AREAWIND].topline=newarea-wi[AREAWIND].maxshow+1;
			wi[AREAWIND].selline=newarea;
			wisetfiles(AREAWIND,FALSE,NO_CHANGE);
		}

		wi[AREAWIND].selline=newarea;
		printline(AREAWIND,wi[AREAWIND].selline,TRUE);
	}	
}
