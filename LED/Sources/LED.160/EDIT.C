#include "ccport.h"
#include <time.h>
#include "ledscan.h"
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"
#include "vars.h"
#include "language.h"

/* #define setclip(g)  vs_clip(handle, 1, setxy(0, g.g_x, g.g_y, g.g_x+g.g_w, g.g_y+g.g_h)) */
/* #define clip_off()  vs_clip(handle, 0, pxy) */

static long SWAP(long value) 0x4840;

static byte cntrltab[256];
static word cntrltabvalid=FALSE;

word msgbuff[8];
byte oldline[LINELEN+2]="\0";

byte *cutbuff[MAX_LINES];

void clear_hdr(long win)
{	if (wi[win].hdr!=NULL)
		free(wi[win].hdr);
	wi[win].hdr=(HDR *) calloc(1,sizeof(HDR));
}

void get_xypos(long win, word *x, word *y)
{
	*x = (wi[win].cx - wi[win].firstc) * gl_wbox + wi[win].x + 4;
	*y = (wi[win].cy - wi[win].firstl) * gl_hbox + wi[win].y + gl_hbox-gl_hchar;
}

void cursor(long win, word mode)
{	word  x, y;
	WIND *winp;
	
	winp=&wi[win];
	if (winp->cy < TO_LINE)
		winp->cy = TO_LINE;
	if (winp->cy<=TOPIC_LINE && winp->cx < HDRCOL)
		winp->cx = HDRCOL;

	if (winp->cy > winp->bzeilen-1)
	{
		winp->cy = winp->bzeilen-1;
		winp->lnr = winp->topline+winp->cy-TOPMSG_LINE;
		set_cxpos(win);
	}

	get_xypos(win,&x,&y);
	
	if (x < winp->x || x+gl_wbox > winp->x + winp->w || 
	    y < winp->y || y+gl_hbox > winp->y + winp->h)
		return;
	
	if (mode != winp->cursor)
	{	word pxy[4];								/* ssl 111294 */
		wind_update(BEG_UPDATE);
		wind_get(winp->handle,WF_FIRSTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		while (pxy[2] && pxy[3])
		{	pxy[2] += pxy[0]-1;
		  pxy[3] += pxy[1]-1;
		  vs_clip(handle,TRUE,pxy);
			REV(x, y, gl_wbox-1, gl_hbox-1);
		  wind_get(winp->handle,WF_NEXTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		}
		wind_update(END_UPDATE);
		winp->cursor = mode;
	}
}

static word myv_gtext(word x,word y,word lnr,byte *line,byte *complete_line,word extra_fl)
{	word i;
	word isquote,ext[8],width=0;
	byte buf[256],*lptr;
	
	if (line!=NULL)
	{	isquote=FALSE;
		i=0;
		lptr=complete_line;
		while (*lptr && i++<6)
		{	if (*lptr++=='>')
			{	isquote=TRUE;
				break;
			}
		}

		strcpy(buf,line);
		lptr=strrchr(buf,'\n');
		if (lptr)
		{	if (opt.showcr)
				*lptr++='\r';
			*lptr=EOS;
		}
		
		vswr_mode(handle,MD_REPLACE);

		if (opt.blockrev)
		{
			if (lnr<=e_cutbuff && lnr>=s_cutbuff && lnr>=0)
				extra_fl ^= EFF_INVERS; /*vswr_mode(handle,MD_ERASE);*/

			if (extra_fl & EFF_INVERS)
			{	vswr_mode(handle,MD_ERASE);
				extra_fl &= ~EFF_INVERS;
			}

			if (isquote && opt.qthick)
				vst_effects(handle,1 ^ extra_fl);
			else
				vst_effects(handle,extra_fl);

			vqt_extent(handle,buf,ext);
			width=ext[2]-ext[0];
			v_gtext(handle,x,y,buf);
		}
		else
		{
			if (extra_fl & EFF_INVERS)
			{	vswr_mode(handle,MD_ERASE);
				extra_fl &= ~EFF_INVERS;
			}

			i=0;
			if (isquote && opt.qthick)
				i|=1;
			if (lnr<=e_cutbuff && lnr>=s_cutbuff && lnr>=0)
				i|=2;
			i = i ^ extra_fl;
			vst_effects(handle,i);

			vqt_extent(handle,buf,ext);
			width=ext[2]-ext[0];
			v_gtext(handle,x,y,buf);

		}
		vswr_mode(handle,MD_REPLACE);
		vst_effects(handle,0);
	}
	return(width);
}

#define ON_VALID		1
#define OFF_VALID		2
#define ESC_CONTROL	4
#define ESC_TEON		8
#define ESC_TEOFF		16
static void init_cntrltab(void)
{	memset(cntrltab,FALSE,256);
	cntrltab['"' ]=
	cntrltab[' ' ]=ON_VALID|OFF_VALID;
	cntrltab['\n']=
	cntrltab['\r']=
	cntrltab[',' ]=
	cntrltab[';' ]=
	cntrltab[':' ]=
	cntrltab['-' ]=
	cntrltab['.' ]=
	cntrltab['!' ]=
	cntrltab[')' ]=
	cntrltab['?' ]=OFF_VALID;
	cntrltab['(' ]=ON_VALID;
	cntrltab['_' ]=
	cntrltab['/' ]=
	cntrltab['*' ]=
	cntrltab['#' ]=ESC_CONTROL;
	cntrltab[TEON_U]=
	cntrltab[TEON_B]=
	cntrltab[TEON_C]=
	cntrltab[TEON_I]=ESC_TEON;
	cntrltab[TEOFF_U]=
	cntrltab[TEOFF_B]=
	cntrltab[TEOFF_C]=
	cntrltab[TEOFF_I]=ESC_TEOFF;
}

#define isSPACE(x,valid) (cntrltab[x] & valid)

word scan_effects(byte *text,byte *ret)
{	byte *cp,buf[512],*pp;
	word chr1,flag,eff,c1,c2,c3,c4;
	word found_effect=FALSE;

	if (!cntrltabvalid)
	{	init_cntrltab();
		cntrltabvalid=TRUE;
	}

	eff=0;
	strcpy(buf,text);
	cp=buf-1;
	while (cp<buf || *cp)
	{	
		if (cp<buf || isSPACE(*cp,ON_VALID))
		{	chr1=cp[1];

			if (chr1)
			{	if (!isSPACE(chr1,ESC_CONTROL))	{ cp++; continue; }
#if 0
						chr1!='_' && chr1!='*' &&
						chr1!='/' && chr1!='#') { cp++; continue; }
#endif
				eff|=1;				/* Es ist min. ein "*_/" vorhanden */

				pp=cp+1;
				flag=FALSE;
				c1=c2=c3=c4=0;
				while(*pp && !flag && !isSPACE(*pp,ON_VALID))
				{	switch(*pp++)
					{ case '_': c1++; break;
						case '*': c2++; break;
						case '/': c3++; break;
						case '#': c4++; break;
						default	: flag=TRUE;
					}
				}
				if (c1>1 || c2>1 || c3>1 || c4>1)
					flag=FALSE;
				if (!flag)
				{	cp=pp; continue; }

				cp++;
				while(*cp)
				{			 if (*cp=='_') { *cp++=TEON_U; eff|=2; }
					else if (*cp=='*') { *cp++=TEON_B; eff|=2; }
					else if (*cp=='/') { *cp++=TEON_C; eff|=2; }
					else if (*cp=='#') { *cp++=TEON_I; eff|=2; }
					else
						break;
				}
			}
		}
		cp++;
	}

	if (!(eff & 1))			/* Kein Effektzeichen vorhanden */
	{	strcpy(ret,buf);
		return(FALSE);
	}

	while (cp>buf)
	{	
		if (isSPACE(*cp,OFF_VALID) || *cp==EOS)
		{	
			if (cp>buf+1)
			{	chr1=*(cp-1);

				if (!isSPACE(chr1,ESC_CONTROL))	{ cp--; continue; }
#if 0
				if (chr1!='_' && chr1!='*' &&
						chr1!='/' && chr1!='#') { cp--; continue; }
#endif
				pp=cp-1;
				flag=FALSE;
				c1=c2=c3=c4=0;
				while(pp>=buf && *pp!=' ' && !flag)
				{	switch(*pp--)
					{ case '_': c1++; break;
						case '*': c2++; break;
						case '/': c3++; break;
						case '#': c4++; break;
						default	: flag=TRUE;
					}
				}
				if (c1>1 || c2>1 || c3>1 || c4>1)
					flag=FALSE;
				if (!flag)
				{	cp=pp; continue; }

				cp--;

				while(cp>buf)
				{			 if (*cp=='_') { *cp--=TEOFF_U; eff|=4; }
					else if (*cp=='*') { *cp--=TEOFF_B; eff|=4; }
					else if (*cp=='/') { *cp--=TEOFF_C; eff|=4; }
					else if (*cp=='#') { *cp--=TEOFF_I; eff|=4; }
					else
						break;
				}
			}
		}
		cp--;
	}
	
	if (!(eff & 7))			/* Kein Effektzeichen und kein On/Off-Code vorhanden */
	{	strcpy(ret,buf);
		return(FALSE);
	}
	
	/* 2.Phase */

	{	byte *pnt[LINELEN], chr[LINELEN];
		word cnt=-1;
		cp=buf;
		while(*cp)
		{	if (isSPACE(*cp,ESC_TEON))
#if 0
					*cp==TEON_U || *cp==TEON_B ||
					*cp==TEON_C || *cp==TEON_I)
#endif
			{	cnt++;
				pnt[cnt]=cp;
				chr[cnt]=*cp +1;		/* war +3 */
				cp++;
				continue;
			}
			if (isSPACE(*cp,ESC_TEOFF))
#if 0
					*cp==TEOFF_U || *cp==TEOFF_B ||
					*cp==TEOFF_C || *cp==TEOFF_I)
#endif
			{	/* Liegt das Zeichen auf dem Stack? */
				if (cnt>=0 && chr[cnt]==*cp)
				{	/* Ja, dann ist der Effekt OK */
					cp++;
					cnt--;
					found_effect=TRUE;
					continue;
				}
				/* Zeichen liegt nicht auf dem Stack, also OFF-Character
					 wieder zurÅcksetzen und warten...
				*/
				switch(*cp)
				{	case	TEOFF_U	:	*cp='_'; break;
					case	TEOFF_B : *cp='*'; break;
					case	TEOFF_C	: *cp='/'; break;
					case	TEOFF_I	: *cp='#'; break;
				}
				cp++;
				continue;
			}
			cp++;
		}

		while (cnt>=0)
		{	switch(chr[cnt])
			{	case	TEOFF_U	:	*pnt[cnt]='_'; break;
				case	TEOFF_B : *pnt[cnt]='*'; break;
				case	TEOFF_C	: *pnt[cnt]='/'; break;
				case	TEOFF_I	: *pnt[cnt]='#'; break;
			}
			cnt--;
		}
	}
	strcpy(ret,buf);
	return(found_effect);
}

/*
** lc - aktuelle Zeile: Wird fÅr Blockmarkierung benutzt
**      -1: Keine Blockmarkierung anzeigen!
**			-2: Wie -1, aber TextEffekte auf jeden Fall benutzen!
*/

void effect_v_gtext(long win,word x,word y,word lc,byte *text)
{	byte tmp[512],*cp;
	word extra_effect=0,tmpx,chr;

	if (!text) return;
	if (wi[win].editierbar || wi[win].listwind)
	{	myv_gtext(x,y,lc,text,text,FALSE);
		return;
	}
	if ((lc!=-2) && (!(opt.texteffects & O_TEXTEFFSCR)) )
	{	myv_gtext(x,y,lc,text,text,FALSE);
		return;
	}

	tmpx=x;
	if (!scan_effects(text,tmp))
	{	myv_gtext(x,y,lc,text,text,FALSE);
		return;
	}

	cp=tmp;
	while(*cp)
	{	
		if (*cp=='\n')
		{	*cp=EOS;

			tmpx+=myv_gtext(tmpx,y,lc,tmp,text,extra_effect);

			extra_effect = 0;
			strcpy(tmp,cp+1);
			cp=tmp;
			vswr_mode(handle,MD_TRANS);

			tmpx+=myv_gtext(tmpx,y,lc,"\n",text,extra_effect);
			vswr_mode(handle,MD_REPLACE);
			continue;
		}
		if (*cp==TEON_U || *cp==TEON_B ||
				*cp==TEON_C || *cp==TEON_I)
		{	chr=*cp;
			*cp=EOS;

			tmpx+=myv_gtext(tmpx,y,lc,tmp,text,extra_effect);

			while (chr)
			{	switch (chr)
				{	case TEON_U	:	extra_effect |= 8; break;
					case TEON_B	:	extra_effect |= 1; break;
					case TEON_C	:	extra_effect |= 4; break;
					case TEON_I	:	extra_effect |= EFF_INVERS;
				}
				chr=cp[1];
				if (chr==TEON_U || chr==TEON_B ||
						chr==TEON_C || chr==TEON_I)
					chr=*++cp;
				else
					chr=0;
			}
			strcpy(tmp,cp+1);
			cp=tmp;
			continue;
		}
		if (*cp==TEOFF_U || *cp==TEOFF_B ||
				*cp==TEOFF_C || *cp==TEOFF_I)
		{	chr=*cp;
			*cp=EOS;

			tmpx+=myv_gtext(tmpx,y,lc,tmp,text,extra_effect);

			while (chr)
			{	switch (chr)
				{	case TEOFF_U	:	extra_effect &= ~8; break;
					case TEOFF_B	:	extra_effect &= ~1; break;
					case TEOFF_C	:	extra_effect &= ~4;
													vswr_mode(handle,MD_TRANS); break;
					case TEOFF_I	:	extra_effect &= ~EFF_INVERS;
				}
				chr=cp[1];
				if (chr==TEOFF_U || chr==TEOFF_B ||
						chr==TEOFF_C || chr==TEOFF_I)
					chr=*++cp;
				else
					chr=0;
			}
			strcpy(tmp,cp+1);
			cp=tmp;
			continue;
		}
		cp++;
	}
	if (*tmp)
		myv_gtext(tmpx,y,lc,tmp,text,extra_effect);
	vswr_mode(handle,MD_REPLACE);
}

void gputc1(ubyte c, word x, word y)
{	static ubyte b[] = " ";
	*b = c;
	v_gtext(handle, x, y, b);
}

void phdrline(long win)
{	word oldcx,oldcy;
	word x,y,w;
	byte tmp[MAX_STR],tmp2[MAX_STR];
	oldcx=wi[win].cx;
	oldcy=wi[win].cy;
	wi[win].cx=HDRCOL;
	wi[win].cy=TO_LINE;
	if (!is_netmail(wi[win].aktarea) &&
	    stricmp(gl_area[wi[win].aktarea].name,"ExtraExpArea") && 
	    stricmp(gl_area[wi[win].aktarea].name,"Bad_Msgs") &&
	    !(gl_area[wi[win].aktarea].flag & AR_FIXEDADDRESS))
		strcpy(tmp,wi[win].hdr->sTo);
	else
	{	
#if 0
		sprintf(tmp2,msgtxt[DIV_ON].text,
							mergeaddr(wi[win].hdr->wToZone,
												wi[win].hdr->wToNet,
			        					wi[win].hdr->wToNode,
			        					wi[win].hdr->wToPoint,NULL));
#else
		sprintf(tmp2,msgtxt[DIV_ON].text,
							mergeaddr(wi[win].hdr->wToZone,
												wi[win].hdr->wToNet,
			        					wi[win].hdr->wToNode,
			        					wi[win].hdr->wToPoint,
			        					gl_area[wi[win].aktarea].domain));
#endif

		sprintf(tmp,"%s%s",wi[win].hdr->sTo,tmp2);
	}
	get_xypos(win,&x,&y);
	w = wi[win].w-HDRCOL*gl_wbox;
	hide_mouse();
	RWHITE(x,y,w,gl_hbox-1);
/*myv_gtext(x,y+gl_hchar,-1,tmp,tmp,FALSE);*/
/*effect_v_gtext(win,x,y+gl_hchar,-1,tmp); */
	v_gtext(handle,x,y+gl_hchar,tmp);

	show_mouse();
	wi[win].cx=oldcx;
	wi[win].cy=oldcy;
}

void phdrfromline(long win)
{	word oldcx,oldcy;
	word x,y,w;
	byte tmp[MAX_STR],tmp2[MAX_STR];
	oldcx=wi[win].cx;
	oldcy=wi[win].cy;
	wi[win].cx=HDRCOL;
	wi[win].cy=FROM_LINE;

/*	if (!is_netmail(wi[win].aktarea) && 
	    stricmp(gl_area[wi[win].aktarea].name,"ExtraExpArea") && 
	    stricmp(gl_area[wi[win].aktarea].name,"Bad_Msgs"))
		strcpy(tmp,wi[win].hdr->sTo);
	else */

	sprintf(tmp2,msgtxt[DIV_ON].text,
							mergeaddr(wi[win].hdr->wFromZone,
												wi[win].hdr->wFromNet,
			        					wi[win].hdr->wFromNode,
			        					wi[win].hdr->wFromPoint,NULL));
	sprintf(tmp,"%s%s",wi[win].hdr->sFrom,tmp2);

	get_xypos(win,&x,&y);
	w = wi[win].w-HDRCOL*gl_wbox;
	RWHITE(x,y,w,gl_hbox-1);
/*myv_gtext(x,y+gl_hchar,-1,tmp,tmp,FALSE); */
	effect_v_gtext(win,x,y+gl_hchar,-1,tmp);
	wi[win].cx=oldcx;
	wi[win].cy=oldcy;
}

void psubjline(long win)
{	word oldcx,oldcy;
	word x,y,w;
	
	oldcx=wi[win].cx;
	oldcy=wi[win].cy;
	wi[win].cx=HDRCOL;
	wi[win].cy=TOPIC_LINE;
	
	get_xypos(win,&x,&y);
	w = wi[win].w-HDRCOL*gl_wbox;
	RWHITE(x,y,w,gl_hbox-1);
/*myv_gtext(x,y+gl_hchar,-1,wi[win].hdr->sSubject,wi[win].hdr->sSubject,FALSE); */
	effect_v_gtext(win,x,y+gl_hchar,-1,wi[win].hdr->sSubject);
	
	wi[win].cx=oldcx;
	wi[win].cy=oldcy;
}

void pline(long win, word fcy, word lcy, word vslddraw, word hslddraw)
{	word ytop,x2,y2,l;
	WIND *winp;
	long stmp;
	word wi_maxcol;
	word pxy[4];
	
	winp=&wi[win];
	if (winp->editierbar)
		wi_maxcol=opt.maxedcol;
	else
	{	if (winp->listwind)
			wi_maxcol=MAXCOL_LISTWIND;
		else
			wi_maxcol=opt.maxcol;
	}
	
	ytop=winp->y+gl_hbox-gl_hchar;
	x2=winp->x+winp->w-1;
	y2=winp->y+winp->h-1;
	
	if (vslddraw)
	{
		if (winp->lzahl <= winp->maxshow)
			stmp=1;
		else 
			stmp=winp->topline*1000L/(winp->lzahl-winp->maxshow);
		wind_set(winp->handle,WF_VSLIDE,(word)min(1000,stmp),0,0,0);

		if (winp->lzahl>0)
			stmp=winp->maxshow*1000L/winp->lzahl;
		else 
			stmp=1000;
		wind_set(winp->handle,WF_VSLSIZE,(word)min(1000,stmp),0,0,0);
	}
	if (hslddraw)
	{
		if (winp->bspalten > wi_maxcol)
			stmp=1000;
		else 
			stmp=winp->firstc*1000L/(wi_maxcol+1-winp->bspalten);
		wind_set(winp->handle,WF_HSLIDE,(word)min(1000,stmp),0,0,0);

		stmp=winp->bspalten*1000L/(wi_maxcol+1);
		wind_set(winp->handle,WF_HSLSIZE,(word)min(1000,stmp),0,0,0);
	}
	
	if (fcy>lcy)
	{
		l=fcy;
		fcy=lcy;
		lcy=l;
	}
	if (fcy<TOPMSG_LINE)
		fcy=TOPMSG_LINE;
	if (lcy>winp->bzeilen || lcy<TOPMSG_LINE)
		lcy=winp->bzeilen;
	pxy[0]=winp->x;
	pxy[1]=fcy*gl_hbox+ytop;
	if (pxy[1]<gl_y2desk)
	{
		pxy[2]=x2;
		pxy[3]=(lcy+1)*gl_hbox+ytop-1;
		if (pxy[3]>=y2)
			pxy[3]=y2;
		if (pxy[3]>gl_y2desk)
			pxy[3]=gl_y2desk;
		
		vr_recfl(handle,pxy);
		
		pxy[0]+=4-winp->firstc*gl_wbox;
		pxy[1]+=gl_hchar;
		fcy+=winp->topline-TOPMSG_LINE;
		lcy+=winp->topline-TOPMSG_LINE;
		while (fcy <= lcy)
		{
/*		myv_gtext(pxy[0],pxy[1],fcy,winp->lines[fcy],winp->lines[fcy],FALSE); */
			effect_v_gtext(win,pxy[0],pxy[1],fcy,winp->lines[fcy]);

			fcy++;
			pxy[1]+=gl_hbox;
		}
	}
	vst_effects(handle,0);
}

void del_line(long win, word lnr, word redraw)
{	word l;
	char *cp;
	WIND *winp;
	
	winp=&wi[win];
	l=lnr;
	if (winp->lines[lnr]!=NULL)
	{
		ch_blkbounds(win);
		if (s_cutbuff>=0)
		{
			if (lnr<s_cutbuff)
			{
				s_cutbuff--;
				e_cutbuff--;
				upded_stat(win,0);
			}
			else if (lnr>=s_cutbuff && lnr<=e_cutbuff)
				del_blkbounds(win,redraw);
		}
		strcpy(oldline,winp->lines[lnr]);
		
		if (winp->lzahl>1)
		{
			if (winp->lines[lnr+1]!=NULL)
			{
				free(winp->lines[lnr]);
				while (winp->lines[lnr+1]!=NULL)
				{
					winp->lines[lnr]=winp->lines[lnr+1];
					lnr++;
				}
				winp->lines[lnr]=NULL;
				winp->lzahl--;
			}
			else
			{
				cp=winp->lines[lnr];
				*cp=EOS;
			}
			if (redraw)
			{
				if (winp->lnr>lnr)
					winp->lnr=lnr;
				if (l==winp->lnr)
				{
					winp->oldcx=MSGCOL;
					set_firstc(win,winp->oldcx);
					set_cxpos(win);
				}
				l=l-winp->topline+TOPMSG_LINE;
				if (l>=TOPMSG_LINE && l<winp->bzeilen-1)
					lines_up(win,l+1);
				pline(win,winp->bzeilen-1,winp->bzeilen,TRUE,TRUE);
			}
		}
		else
		{
			winp->lzahl=1;
			winp->lnr=0;
			*winp->lines[0]=EOS;
			if (winp->indent>=0)
				winp->indent=0;
			winp->oldcx=MSGCOL;
			set_firstc(win,winp->oldcx);
			set_cxpos(win);
			if (redraw)
				pline(win,TOPMSG_LINE,TOPMSG_LINE,FALSE,TRUE);
		}
	}
}

word ins_line(long win, word lnr, byte *line, word useind, word redraw)
{	word cy;
	WIND *winp;
	byte *prev;
	byte *ptr;
	byte *lptr;
	
	winp=&wi[win];
	cy=lnr-winp->topline+TOPMSG_LINE;
	
	if (line==NULL)
	{
		line=oldline;
		if (*line==EOS)
			return(FALSE);
	}
	if (winp->lzahl+1>=MAX_LINES)
	{
		nomemalert(5);
		return(FALSE);
	}
	if ((lptr=(char *)calloc(1,LINELEN))==NULL)
	{
		nomemalert(0);
		return(FALSE);
	}
	
	if (useind)
	{
		prev=winp->lines[lnr-1];
		if (lnr>0 && prev!=NULL && winp->indent>=0)
		{
			winp->indent=getindent(win,prev,lptr);
			ptr=line;
			while (*ptr==' ')
				ptr++;
			if (strlen(lptr)+strlen(ptr)<opt.maxedcol)
			{
				if (strlen(lptr) >= ptr-line)
					line=ptr;
				else
					line+=strlen(lptr);
			}
			else
			{
				winp->indent=0;
				*lptr=EOS;
			}
		}
	}
	strcat(lptr,line);
	if (*lptr==EOS)
		strcpy(lptr,"\n");
	
	ch_blkbounds(win);
	if (s_cutbuff>=0)
	{
		if (lnr<=s_cutbuff)
		{
			s_cutbuff++;
			e_cutbuff++;
			upded_stat(win,0);
		}
		else if (lnr>s_cutbuff && lnr<=e_cutbuff)
			del_blkbounds(win,redraw);
	}
	
	if (winp->lines[lnr]==NULL)
	{
		winp->lines[lnr]=lptr;
		if (redraw)
			pline(win,cy,cy,TRUE,FALSE);
	}
	else
	{
		ins_arline(win,lnr,1);
		winp->lines[lnr]=lptr;
		if (redraw)
			lines_down(win,cy);
	}
	
	winp->lzahl++;
	
	if (redraw)
		set_cxpos(win);
	return(TRUE);
}

word ins_arline(long win, word lnr, word lanz)
{	word i;
	WIND *winp;
	
	winp=&wi[win];
	if (lanz+winp->lzahl>MAX_LINES)
		return(FALSE);
	else
	{
		if (lnr<0)
			lnr=0;
		i=winp->lzahl-1;
		while (i>=lnr)
		{
			winp->lines[i+lanz]=winp->lines[i];
			i--;
		}
		return(TRUE);
	}
}

#pragma warn -par

word getindent(long win, byte *line, byte *indstr)
{	word i;
	word ret;
	byte *cp,*ptr;
	
	if (line!=NULL /*&& wi[win].indent>=0*/)		/* ssl 060495 */
	{
		cp=line;
		while (*cp==' ')  /* Anf.leerstellen Åberspringen */
			cp++;
		/* evtl. Quote Åberspringen */
		if (cp-line<5)
		{
			ptr=cp;
			while (*ptr && *ptr!='\n' && *ptr!=' ')
			{
				if (*ptr=='>' && *(ptr+1)==' ')		/* auf SPACE Test ab 180796 */
					break;
				ptr++;
			}
			if (*ptr=='>')
			{
				cp=ptr;
				while (*cp=='>')
					cp++;
				while (*cp==' ')  /* leerstellen Åberspringen */
					cp++;
			}
		}
		i=(word)*cp;
		*cp=EOS;
		if (indstr!=NULL)
			strcpy(indstr,line);
		ret=(word)strlen(line);
		*cp=(byte)i;
	}
	else
		ret=-1;
	return(ret);
}

#pragma warn +par

void set_cxpos(long win)
{	WIND *winp;
	winp=&wi[win];
	winp->edline=winp->lines[winp->lnr];
	winp->cx=winp->oldcx;
	if (winp->cx>opt.maxedcol)
		winp->cx=opt.maxedcol;
	if (winp->edline!=NULL)
	{
		if (strlen(winp->edline) <= (long)winp->cx)
		{
			winp->pos=strchr(winp->edline,0);
			winp->cx=(word)strlen(winp->edline);
			if (*(winp->pos-1)=='\n')
			{
				winp->pos--;
				winp->cx--;
			}
		}
		else
			winp->pos=(char *)(winp->edline+winp->cx);
		set_firstc(win,winp->cx);
	}
	else
	{
		*oldline=EOS;
		winp->pos=winp->edline=oldline;
		winp->cx=0;
	}
}

void set_firstc(long win, word cx)
{
	word fc;
	
	if (wi[win].cy>=TOPMSG_LINE && 
	   (cx < wi[win].firstc || cx >= wi[win].firstc+wi[win].bspalten))
	{
		fc=0;
		while(cx-fc >= wi[win].bspalten)
			fc+=(wi[win].bspalten/3);
		if (fc!=wi[win].firstc)
		{
			wi[win].firstc=fc;
			pline(win,TOPMSG_LINE,wi[win].bzeilen,FALSE,TRUE);
		}
	}
}

void init_cutbuff(void)
{	memset(cutbuff,0,sizeof(cutbuff));
}

void clear_cutbuff(void)
{	word i;
	
	for (i=0;i<MAX_LINES;i++)
		if (cutbuff[i]!=NULL)
			free(cutbuff[i]);
	init_cutbuff();
}

void ch_blkbounds(long win)
{	word i;
	
	if (s_cutbuff<0 || e_cutbuff<0)
	{	s_cutbuff=-1;
		e_cutbuff=-1;
	}
	else
	{	if (s_cutbuff>wi[win].lzahl-1)
			s_cutbuff=wi[win].lzahl-1;
		if (e_cutbuff>wi[win].lzahl-1)
			e_cutbuff=wi[win].lzahl-1;
		if (s_cutbuff>e_cutbuff)
		{	i=s_cutbuff;
			s_cutbuff=e_cutbuff;
			e_cutbuff=i;
		}
	}
}

void del_blkbounds(long win, word redraw)
{	word s,e;
	
	s=s_cutbuff-wi[win].topline+TOPMSG_LINE;
	e=e_cutbuff-wi[win].topline+TOPMSG_LINE;
	s_cutbuff=-1;
	e_cutbuff=-1;
	upded_stat(win,0);
	
	if (redraw)
	{
		if (e>=TOPMSG_LINE && s<=wi[win].bzeilen)
		{
			if (s<TOPMSG_LINE)
				s=TOPMSG_LINE;
			if (e>wi[win].bzeilen)
				e=wi[win].bzeilen;
			
			hide_mouse();
			pline(win,s,e,TRUE,FALSE);
			show_mouse();
		}
	}
}

void append_line(long win)
{	word i,line;
	byte *lptr;
	
	biene();
	line=wi[win].cy;
	e_cutbuff=
	s_cutbuff=wi[win].lnr;
	hide_mouse();
	pline(win,line,line,TRUE,FALSE);
	show_mouse();
	evnt_timer(100,0);

	i=0;
	while (cutbuff[i]!=NULL && i<=MAX_LINES) i++;
	mouse_normal();
	if (i>MAX_LINES)
	{	LangAlert(MSG_CUTBUFFERFULL);
	}
	else
	{	if ((lptr=(char *)calloc(1,LINELEN))!=NULL)
		{	cutbuff[i]=lptr;
			strcpy(lptr,wi[win].lines[wi[win].lnr]);
		}
		else
		{	nomemalert(4);
		}
	}
	e_cutbuff=
	s_cutbuff=-1;
	hide_mouse();
	pline(win,line,line,FALSE,FALSE);
	show_mouse();
}

void copy_blk(long win, word redraw)
{	word i;
	byte *lptr;
	
	ch_blkbounds(win);
	e_cutbuff++;
	if (s_cutbuff>=0 && s_cutbuff!=e_cutbuff)
	{
		clear_cutbuff();
		if ((long)Malloc(-1) > (long)(e_cutbuff-s_cutbuff)*((long)LINELEN))
		{
			biene();
			for (i=s_cutbuff;i<e_cutbuff;i++)
			{
				if ((lptr=(char *)calloc(1,LINELEN))!=NULL)
				{
					cutbuff[i-s_cutbuff]=lptr;
					strcpy(lptr,wi[win].lines[i]);
				}
				else
				{
					clear_cutbuff();
					nomemalert(4);
					break;
				}
			}
			mouse_normal();
		}
		else
			nomemalert(4);
	}
	del_blkbounds(win,redraw);
}

void cut_blk(long win)
{	word i,s,e;
	word redraw;
	
	ch_blkbounds(win);
	e_cutbuff++;
	if (s_cutbuff>=0 && s_cutbuff!=e_cutbuff)
	{
		biene();
		clear_cutbuff();
		s=s_cutbuff;
		e=e_cutbuff;

		for (i=s_cutbuff;i<e_cutbuff;i++)
			cutbuff[i-s_cutbuff]=wi[win].lines[i];

		while (i++<=wi[win].lzahl)
			wi[win].lines[s++]=wi[win].lines[e++];
		
		wi[win].lzahl-=(e_cutbuff-s_cutbuff);
		if (wi[win].lzahl<1)
		{
			wi[win].lzahl=0;
			wi[win].lnr=0;
			ins_line(win,0,"\0",FALSE,TRUE);
		}
		else if (wi[win].lnr>s_cutbuff && wi[win].lnr<e_cutbuff)
			wi[win].lnr=s_cutbuff;
		else if (wi[win].lnr>=e_cutbuff)
			wi[win].lnr-=(e_cutbuff-s_cutbuff);
		if (wi[win].lnr>wi[win].lzahl-1)
			wi[win].lnr=wi[win].lzahl-1;
		
		for (i=wi[win].lzahl;i<MAX_LINES;i++)
			wi[win].lines[i]=NULL;
		
		if (wi[win].lzahl>wi[win].maxshow)
		{
			if (wi[win].lnr==s_cutbuff)
				wi[win].topline=s_cutbuff+TOPMSG_LINE-wi[win].cy;
			else if (wi[win].lnr>=e_cutbuff)
				wi[win].topline-=(e_cutbuff-s_cutbuff);
			
			if (wi[win].topline<0)
			{
				wi[win].topline=0;
				wi[win].lnr=s_cutbuff;
			}
			if (wi[win].topline>wi[win].lzahl-wi[win].maxshow)
				wi[win].topline=wi[win].lzahl-wi[win].maxshow;
			wi[win].cy=wi[win].lnr-wi[win].topline+TOPMSG_LINE;
		}
		else
		{
			wi[win].topline=0;
			wi[win].cy=wi[win].lnr+TOPMSG_LINE;
			if (wi[win].cy>TOPMSG_LINE+wi[win].lzahl-1)
				wi[win].cy=TOPMSG_LINE+wi[win].lzahl-1;
		}
		wi[win].lnr=wi[win].topline+wi[win].cy-TOPMSG_LINE;
		if (wi[win].indent>=0)
			wi[win].indent=getindent(win,wi[win].lines[wi[win].lnr],NULL);
		wi[win].oldcx=MSGCOL;
		set_cxpos(win);
		mouse_normal();
		redraw=TRUE;
	}
	else
		redraw=FALSE;
	s_cutbuff=wi[win].topline;
	e_cutbuff=s_cutbuff+wi[win].bzeilen;
	del_blkbounds(win,redraw);
}

void ins_blk(long win)
{	word i,s,e;
	word redraw;
	
	ch_blkbounds(win);
	if (s_cutbuff>=0)
	{
		if (cutbuff[0]==NULL)
			copy_blk(win, FALSE);
		redraw=TRUE;
	}
	else
		redraw=FALSE;
	
	s=s_cutbuff;
	e=e_cutbuff;
	i=0;
	while (cutbuff[i++]!=NULL);
	e_cutbuff=i-1;
	if (e_cutbuff>0)
	{
		if (e_cutbuff+wi[win].lzahl < MAX_LINES)
		{
			biene();
			if (ins_arline(win,wi[win].lnr,e_cutbuff))
			{
				i=0;
				while (cutbuff[i]!=NULL)
				{
					wi[win].lines[i+wi[win].lnr]=cutbuff[i];
					cutbuff[i]=NULL;			/* nicht mehr freigeben */
					i++;
				}
				wi[win].lzahl+=e_cutbuff;
				init_cutbuff();
				s_cutbuff=wi[win].lnr;
				e_cutbuff+=s_cutbuff-1;
				if (wi[win].indent>=0)
					wi[win].indent=getindent(win,wi[win].lines[s_cutbuff],NULL);
				wi[win].oldcx=MSGCOL;
				set_firstc(win,wi[win].oldcx);
				set_cxpos(win);
				s=s_cutbuff;
				e=e_cutbuff;
				copy_blk(win, FALSE);
				redraw=TRUE;
			}
			mouse_normal();
		}
		else
		{
			show_mouse();
			LangAlert(MSG_MAXLINESEXCEEDED);
		}
	}
	s_cutbuff=s;
	e_cutbuff=e;
	upded_stat(win,0);
	if (redraw)
	{
		hide_mouse();
		pline(win,TOPMSG_LINE,wi[win].bzeilen,TRUE,TRUE);
		show_mouse();
	}
}

void c_rt(long win)
{	if (wi[win].cx < opt.maxedcol
		&& *wi[win].pos && *wi[win].pos!='\n')
	{
		wi[win].cx++;
		wi[win].pos++;
		if (wi[win].cx-wi[win].firstc >= wi[win].bspalten)
		{
			wi[win].firstc+=(wi[win].bspalten/3);
			pline(win,TOPMSG_LINE,wi[win].bzeilen,FALSE,TRUE);
		}
	}
	else if (wi[win].lines[wi[win].lnr+1] != NULL)
	{
		if (wi[win].cy < wi[win].bzeilen-1) /* schon letzte wind-zeile? */
			wi[win].cy++;
		else
			scr_up(win);
		wi[win].lnr++;
		if (wi[win].indent>=0)
		{
			wi[win].indent=getindent(win,wi[win].lines[wi[win].lnr],NULL);
			wi[win].oldcx=wi[win].indent;
		}
		else
			wi[win].oldcx=0;
		set_firstc(win,wi[win].oldcx);
		set_cxpos(win);
	}
	wi[win].oldcx=wi[win].cx;
}


void c_lf(long win)
{	if (wi[win].cx > MSGCOL && wi[win].pos > wi[win].edline)
	{
		wi[win].cx--;
		wi[win].pos--;
		if (wi[win].cx < wi[win].firstc)
		{
			wi[win].firstc-=(wi[win].bspalten/3);
			if (wi[win].firstc<0)
				wi[win].firstc=0;
			pline(win,TOPMSG_LINE,wi[win].bzeilen,FALSE,TRUE);
		}
	}     
	else if (wi[win].lnr > 0)
	{
		if (wi[win].cy > TOPMSG_LINE) /* schon erste wind-zeile? */
			wi[win].cy--;
		else
			scr_down(win);
		wi[win].lnr--;
		wi[win].oldcx=(word)strlen(wi[win].lines[wi[win].lnr]);
		set_firstc(win,wi[win].oldcx);
		set_cxpos(win);
	}
	wi[win].oldcx=wi[win].cx;
}

void c_up(long win)
{	if (wi[win].lnr > 0 )
	{	if (wi[win].cy > TOPMSG_LINE) /* schon erste wind-zeile? */
			wi[win].cy--;
		else
			scr_down(win);
		wi[win].lnr--;
		set_cxpos(win);
	} 
}

void c_dn(long win)
{ if (wi[win].lnr<wi[win].lzahl-1 && wi[win].lines[wi[win].lnr+1]!=NULL)
	{ if (wi[win].cy < wi[win].bzeilen-1) /* schon letzte wind-zeile? */
			wi[win].cy++;
		else
			scr_up(win);
		wi[win].lnr++;
		set_cxpos(win);
	}
	else
	{	wi[win].lzahl=0;
		while (wi[win].lines[wi[win].lzahl]!=NULL && wi[win].lzahl<MAX_LINES)
			wi[win].lzahl++;
		wi[win].lnr=wi[win].lzahl-1;
	}
}

void c_return(long win)
{	word lz,cx;
	char *lptr,*pos;
	
	cx=wi[win].cx;
	if (wi[win].cy > wi[win].bzeilen-1)
		wi[win].cy=wi[win].bzeilen-1;
	
	if (wi[win].cy >= TOPIC_LINE)
	{
		if (wi[win].lzahl+1>=MAX_LINES)
		{
			nomemalert(5);
			return;
		}
		if ((lptr=(char *)calloc(1,LINELEN))==NULL)
		{
			nomemalert(0);
			return;
		}
		ch_blkbounds(win);
		if (wi[win].lnr>=s_cutbuff && wi[win].lnr<=e_cutbuff)
			del_blkbounds(win,TRUE);
		
		wi[win].oldcx=MSGCOL;
		lz=wi[win].lzahl;
		pos=wi[win].pos;
		if (wi[win].cy >= TOPMSG_LINE)  /* beim wechsel aus hdr nichts verÑndern */
		{
			strcpy(lptr,pos);
			*pos++='\n';
			*pos=EOS;
			pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
			
			if (ins_line(win,wi[win].lnr+1,lptr,TRUE,TRUE))
			{
				if (wi[win].indent>=0)
					wi[win].oldcx=wi[win].indent;
				wi[win].lnr++;
				if (wi[win].cy <  wi[win].bzeilen-1)
					wi[win].cy++;
				else if (wi[win].firstc==0)
					scr_up(win);
				if (wi[win].firstc!=0)
				{
					set_firstc(win,wi[win].oldcx);
					pline(win,TOPMSG_LINE,wi[win].bzeilen,FALSE,TRUE);
				}
			}
			else
			{
				strcpy(--pos,lptr);
				pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
			}
			free(lptr);
		}
		else
		{
			if (wi[win].lines[wi[win].lnr]==NULL) /* sind schon zeilen vorhanden? */
			{
				wi[win].lzahl++;
				wi[win].lines[wi[win].lnr]=lptr;
			}	
			else
				free(lptr);
			lz=0;
			wi[win].cy=TOPMSG_LINE;
			pline(win,TOPMSG_LINE,wi[win].bzeilen,FALSE,TRUE);
		}
		if (wi[win].lzahl!=lz)
			set_cxpos(win);
		else
		{
			wi[win].cx=wi[win].oldcx=cx;
		}
	}
	else if (wi[win].cy==TO_LINE && strlen(wi[win].hdr->sTo)>0)
	{
		wi[win].cy++;
		wi[win].cx=HDRCOL;
		wi[win].pos=wi[win].edline=wi[win].hdr->sSubject;
	}
}

void backsp(long win)
{	if (wi[win].cx > MSGCOL)
	{
		c_lf(win);
		delete(win,FALSE,FALSE,TRUE);
	}
	else
	{
		c_lf(win);
		ch_blkbounds(win);
		if (wi[win].lnr>=s_cutbuff && wi[win].lnr<=e_cutbuff)
			del_blkbounds(win,TRUE);
		
		if (*wi[win].pos=='\n')
			*wi[win].pos=EOS;
		if (*wi[win].pos==EOS)
			wrap_bwd(win,FALSE);
	}
}

void delete(long win, word wrap, word multi, word draw)
{	word x,y,w;
	byte *s;
	
	ch_blkbounds(win);
	if (wi[win].lnr>=s_cutbuff && wi[win].lnr<=e_cutbuff)
		del_blkbounds(win,TRUE);
	
	s = (byte *)wi[win].pos;
	while (*s++)
		*(s-1) = *s;
	
	if (draw)
	{
		get_xypos(win,&x,&y);
		w = (word)strlen(wi[win].pos) * gl_wbox;
		if (x+w+gl_wbox<wi[win].x+wi[win].w)
		{
			rcopy(x+gl_wbox, y, w, gl_hbox-1, x, y, S_ONLY);
			RWHITE(x+w,y,gl_wbox,gl_hbox-1);
		}
		else
			pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
	}
	
	if (wrap && *wi[win].pos==EOS)
		wrap_bwd(win,multi);
}

void insert_tab(long win, word draw)
{	word c,i;
	
	if ((!wi[win].insert && wi[win].cx < opt.maxedcol-opt.tabsize) ||
	    strlen(wi[win].edline) < opt.maxedcol-opt.tabsize)
	{
		c=opt.tabsize - wi[win].cx % opt.tabsize;
		for (i=0; i<c; i++)
			output(win,' ',draw);
	}
}

void extend_tab(long win)
{	word lnr,cx,cy,top,old_insmode;
	byte *cp;
	WIND *winp;
	
	winp=&wi[win];
	s_cutbuff=-1;
	e_cutbuff=-1;
	upded_stat(win,0);

	old_insmode=winp->insert;
	winp->insert=TRUE;
	lnr=winp->lnr;
	cx=winp->cx;
	cy=winp->cy;
	top=winp->topline;
	
	show_mouse();
	biene();
	winp->lnr=0;
	winp->cy=TOPMSG_LINE;
	while (winp->lines[winp->lnr]!=NULL)
	{
		while ((cp=strchr(winp->lines[winp->lnr],'\t'))!=NULL)
		{
			winp->oldcx=(int)(cp-winp->lines[winp->lnr]);

/*		winp->cy=TOPMSG_LINE+winp->lnr; */
			set_cxpos(win);
			cp=strchr(winp->edline,EOS);
			if (*(cp-1)!='\n')
			{
				*cp++='\n';
				*cp=EOS;
			}
			delete(win,FALSE,FALSE,FALSE);
			insert_tab(win,FALSE);
		}
		winp->lnr++;
	}
	mouse_normal();
	hide_mouse();
	
	winp->lnr=lnr;
	winp->oldcx=cx;
	winp->cy=cy;
	winp->topline=top;
	winp->insert=old_insmode;
	set_cxpos(win);
	pline(win,TOPMSG_LINE,winp->bzeilen,TRUE,TRUE);
	show_mouse();
}

void output(long win, ubyte c, word draw)
{ word netmail;
	netmail=is_netmail(wi[win].aktarea);

	if ((gl_area[wi[win].aktarea].flag & AR_UMLAUTE) ||
      ( netmail && (opt.umlaut & O_UMLAUT_N)) ||
      (!netmail && (opt.umlaut & O_UMLAUT_E)) )
	{
		if (!(opt.umlaut & O_ATARIFONT) && c==(ubyte)158)
			c=(ubyte)225;
		outmode(win,c,draw);
	}
	else
	{
		switch(c)
		{
			case 'Ñ':
				outmode(win,'a',draw);
				outmode(win,'e',draw);
				break;
			case 'î':
				outmode(win,'o',draw);
				outmode(win,'e',draw);
				break;
			case 'Å':
				outmode(win,'u',draw);
				outmode(win,'e',draw);
				break;
			case 'é':
				outmode(win,'A',draw);
				outmode(win,'e',draw);
				break;
			case 'ô':
				outmode(win,'O',draw);
				outmode(win,'e',draw);
				break;
			case 'ö':
				outmode(win,'U',draw);
				outmode(win,'e',draw);
				break;
			case 'û':
				outmode(win,'s',draw);
				outmode(win,'s',draw);
				break;
			default:
				outmode(win,c,draw);
				break;
		}
	}
}

void outmode(long win, ubyte c, word draw)
{	if (wi[win].insert)	insert(win, c, draw);
	else								overwrite(win, c);
}

void upded_stat(long win, word state)
{ word i;
	byte tmp[MAX_STR];
	byte tmp1[32];
	
	if (wi[win].editierbar)
	{
		strcpy(tmp,msgtxt[SYSTEM_EDITWINDOW].text);

		if (state==CH_INSMD)
			wi[win].insert = !wi[win].insert;

		if (wi[win].insert)	strcat(tmp,msgtxt[SYSTEM_INSERT].text);
		else								strcat(tmp,msgtxt[SYSTEM_OVERWRITE].text);

		if (wi[win].indent>=0)
			strcat(tmp,msgtxt[SYSTEM_INDENT].text);
	}
	else
		strcpy(tmp,"  ");
	
	if (opt.flag & O_FLAG_SHOWPOS)
	{	sprintf(tmp1,"(%u/%u)  ",wi[win].lnr+1,wi[win].cx+1);
		strcat(tmp,tmp1);
	}
	
	if (s_cutbuff>=0 && e_cutbuff>=0)
	{
		i=wi[win].lzahl;
		if (s_cutbuff>=i)
			s_cutbuff=i-1;
		if (e_cutbuff>=i)
			e_cutbuff=i-1;
		if (s_cutbuff>e_cutbuff)
		{
			i=s_cutbuff;
			s_cutbuff=e_cutbuff;
			e_cutbuff=i;
		}
	}
	if (s_cutbuff>=0)
	{	sprintf(tmp1,msgtxt[SYSTEM_BEGINBLK].text,s_cutbuff+1);
		strcat(tmp,tmp1);
	}
	if (e_cutbuff>=0)
	{	sprintf(tmp1,msgtxt[SYSTEM_ENDBLK].text,e_cutbuff+1);
		strcat(tmp,tmp1);
	}
	upd_status(win,tmp);
}

void overwrite(long win, ubyte c)
{	word x,y;
	
	ch_blkbounds(win);
	if (wi[win].lnr>=s_cutbuff && wi[win].lnr<=e_cutbuff)
	del_blkbounds(win,TRUE);
	
	if (*wi[win].pos==EOS)
		*(wi[win].pos+1)=EOS;
	*wi[win].pos=c;

	get_xypos(win,&x,&y);
	y+=gl_hchar;
	gputc1(c, x, y);

	if (strlen(wi[win].edline) > opt.maxedcol)
		wrap_fwd(win,TRUE);
	c_rt(win);
}

void insert(long win, ubyte c, word draw)
{	word x,y,w;
	word len;
	byte *s;
	
	ch_blkbounds(win);
	if (wi[win].lnr>=s_cutbuff && wi[win].lnr<=e_cutbuff)
		del_blkbounds(win,TRUE);
	
	len=(int)strlen(wi[win].edline);
	s=strchr(wi[win].edline,EOS);
	if (s>wi[win].edline && *(s-1)=='\n')
		len--;
	
	s = strchr(wi[win].pos,0);
	*(s+1) = EOS;
	while (s > wi[win].pos)
		*s-- = *(s-1);
	s=wi[win].pos;
	*s=c;
	
	if (draw)
	{
		get_xypos(win,&x,&y);
		w = (word)strlen(s) * gl_wbox;
		if (x+w+gl_wbox<wi[win].x+wi[win].w)
		{
			rcopy(x, y, w, gl_hbox-1, x+gl_wbox, y, S_ONLY);
			RWHITE(x, y, gl_wbox-1, gl_hbox-1);
		}
		else
			pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
	}
	
	s++;
	wi[win].pos++;
	wi[win].cx++;
	if (c != 32)
	{
		if (draw)
			gputc1(c, x, y+gl_hchar);
	}
	else if ((*s==EOS || *s=='\n') && len >= opt.maxedcol)
	{
		if (*s=='\n')  /* kein CR am Ende */
			*s=EOS;
		while (s>wi[win].edline && *(s-1)==' ')  /* nur ein Space am Ende */
			*(--s)=EOS;
		if (s>wi[win].edline)
		{
			*s++=' ';
			*s=EOS;
			pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
			if (ins_line(win,wi[win].lnr+1,"\0",TRUE,draw))
			{
				if (draw)
				{
					if (wi[win].cy < wi[win].bzeilen-1) /* schon letzte wind-zeile? */
						wi[win].cy++;
					else
						scr_up(win);
				}
				wi[win].lnr++;
				len=0;
			}
		}
		else
		{
			*s++='\n';
			*s=EOS;
			len=0;
			pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
		}
		wi[win].oldcx=wi[win].cx;
		set_cxpos(win);
	}
	if (len >= opt.maxedcol)
		wrap_fwd(win,draw);

	wi[win].oldcx=wi[win].cx;
	set_firstc(win,wi[win].cx);
}


word wrap_fwd(long win,word draw)
{	word l,oldcx,wrap;
	word t,old_indent;
	word fwrpline;
	word wrpwcur;
	word akt_line=0;
#if 0
	word add_nl_because_of_quote=FALSE;
#endif
	WIND *winp;
	byte *cp, *ptr, *pos;
	byte backup[3*LINELEN];
	byte tmp[3*LINELEN];
	byte indntstr[LINELEN];
	
	winp=&wi[win];
	fwrpline=TRUE;
	wrpwcur=FALSE;
	oldcx=winp->cx;
	winp->oldcx=oldcx;
	akt_line=l=winp->lnr;
	pos=(winp->pos-winp->edline)+backup;  /* genaue Curs-pos im Backup */
	strcpy(backup,winp->lines[l]);  /* aktuelle Zeile zwi.speichern */

#if 0
	getindent(win,backup,indntstr);
	if (*indntstr)
		add_nl_because_of_quote=TRUE;
#endif

	*indntstr=EOS;
	old_indent=winp->indent;		/* ssl 060495 */
	if (winp->indent>=0)
		winp->indent=getindent(win,backup,indntstr);
	
	do
	{
		wrap=FALSE;

		/* Nur ein Space am Ende der Zeile */
		if ((cp=strchr(backup,'\n'))!=NULL)
		{	t=TRUE;
			*cp=EOS;
		}
		else
		{	t=FALSE;
			cp=strchr(backup,0);
		}

		if (cp > backup)
		{
			while (*(--cp)==' ' && cp > backup)
				*cp=EOS;
			if (*cp!=' ' && *cp!='-' && !t)
			{	*(cp+1)=' ';
				*(cp+2)=EOS;
			}
		}
		if (t)
			strcat(backup,"\n");
		
		/* Wrapstelle suchen */
		while ((cp-backup >= opt.maxedcol-1) ||
		       (*cp==' ' && cp-backup<=winp->indent) ||		/* ssl 060495 */
		      (( *cp!=' ' && *cp!='-' && *cp!=',') && cp > backup))
			--cp;
		
		if (cp > backup)
		/* ELSE-Zweig wird nur abgearbeitet, wenn Zeile durchgehend OHNE Space ist! */
		{
			while (*(cp+1)==' ')
				++cp;
			strcpy(tmp,++cp);
			*cp=EOS;
			
			if (fwrpline)
			{
				if (cp <= pos)
				{
					oldcx=(word)(pos-cp); /* nur bei erster Zeile */
					if (winp->indent>0)
						oldcx+=winp->indent;
					wrpwcur=TRUE;
				}
				fwrpline=FALSE;
			}
			strcpy(winp->lines[l],backup);
			addendspc(winp->lines[l]);

#if 0
			if (0 && add_nl_because_of_quote)							/* 180796 */
				strcat(winp->lines[l],"\n");
#endif

			l++;
			akt_line++;
			if (winp->lines[l]==NULL) /* neue Zeile ? */
			{
				if ((winp->lines[l]=calloc(1,LINELEN))!=NULL)
					winp->lzahl++;
				else
					nomemalert(0);
			}


			if (winp->lines[l]!=NULL)
			{
				strcpy(backup,indntstr);
				strcat(backup,tmp);
				cp=strchr(backup,'\n');
				if (cp!=NULL)
				{
					*(cp+1)=EOS;
					ins_line(win,l,backup,FALSE,FALSE);
					l=winp->lnr+winp->bzeilen-1-winp->cy; /* um den Rest des Windows auch zu restaurieren */
				}
				else
				{
					cp=winp->lines[l];
					
					/* skip indent */
					t=0;
					while (*cp==' ')
						cp++;

					/* evtl. Quote Åberspringen */
					if (cp-winp->lines[l]<3)
					{
						ptr=cp;
						while (ptr-cp<6 && *ptr && 			/* ssl 060495, 160396 */
						       *ptr!='\n' 				/*&& *ptr!=' '*/		)
						{
							if (*ptr=='>')
								break;
							ptr++;
						}
						if (*ptr=='>')
						{
							cp=ptr;
							while (*cp=='>')
								cp++;
							while (*cp==' ')  /* leerstellen Åberspringen */
								cp++;
						}
					}
					strcat(backup,cp);
					
					/* nochmal wrapen? */
					if (strlen(backup) >= opt.maxedcol)
						wrap=TRUE;
					else
						strcpy(winp->lines[l],backup);
				}
			}
		}
		else    /* if (cp>backup) */
		{
			t=(word)strlen(winp->lines[l]);
			cp=strchr(winp->lines[l],EOS);
			if (cp>winp->lines[l] && (*(cp-1)=='\n' || *(cp-1)==' '))
				t--;
			if (t >= opt.maxedcol)
			{
				pos=winp->lines[l]+opt.maxedcol-1;		/* 180796 */
				strcpy(backup,indntstr);
				strcat(backup,pos);
				l++;
				akt_line++;
				
				if (t <= winp->cx)
				{
					*pos++=' ';
					if (winp->indent>=0)
						oldcx=winp->indent+1;
					else
						oldcx=1;
					wrpwcur=TRUE;
				}
				*pos=EOS;
				
				cp=winp->lines[l];
				if (winp->indent>=0)
					t=getindent(win,cp,NULL);
				else
					t=0;
				if (cp!=NULL && strlen(indntstr)+strlen(backup)+
				                strlen(cp)-(long)t < opt.maxedcol)
				{
#if 1
					ins_line(win,l,backup,FALSE,draw);		/* 180796 */
#else
					strcpy(tmp,cp+(long)t);
					strcpy(cp,backup);
					strcat(cp,tmp);
#endif
				}
				else
					ins_line(win,l,backup,FALSE,draw);
			}
		}
	} while (wrap);
	winp->oldcx=oldcx;
	if (draw)
	{
		l+=winp->cy-winp->lnr;
		if (wrpwcur)
		{
			c_dn(win);
			pline(win,winp->cy-1,l,TRUE,FALSE);
		}
		else
		{
			set_cxpos(win);
			pline(win,winp->cy,l,TRUE,FALSE);
		}
	}
	winp->indent=old_indent;
	return akt_line;
}


void wrap_bwd(long win, word multi)
{	long restlen;
	word l,cy2;
	word red;
	word nxtline;
	WIND *winp;
	byte *cp,*ptr;
	byte tmp[LINELEN];
	
	winp=&wi[win];
	red=FALSE;
	nxtline=TRUE;
	l=winp->lnr+1;
	while (nxtline)
	{
		cy2=winp->bzeilen-1;
		cp=strchr(winp->lines[l-1],0);
		if (cp!=NULL && *(cp-1)!='\n' && winp->lines[l]!=NULL)
		{
			nxtline=multi;
			if (strlen(winp->lines[l])==0)
			{
				if (l<winp->lzahl-1)
					del_line(win,l,TRUE);
				else
				{
					free(winp->lines[l]);
					winp->lines[l]=NULL;
					winp->lzahl--;
				}
				nxtline=FALSE;
			}
			else
			{
				cp=winp->lines[l];
				while (*cp==' ')  /* Anf.leerstellen Åberspringen */
					cp++;
				/* evtl. Quote am Anfang lîschen */
				if (cp-winp->lines[l]<6)
				{
					ptr=cp;
					while (*ptr && *ptr!='\n' && *ptr!=' ')
					{
						if (*ptr=='>')
							break;
						ptr++;
					}
					if (*ptr=='>')
					{
						cp=ptr;
						while (*cp=='>')
							cp++;
						while (*cp==' ')  /* Anf.leerstellen Åberspringen */
							cp++;
					}
				}
				if (*cp==EOS || *cp=='\n') /* war zeile leer? */
				{
					del_line(win,l,TRUE);
					cp=strchr(winp->edline,0);
					if (*(cp-1)!='\n')
					{
						*cp++='\n';
						*cp=EOS;
					}
					pline(win,winp->cy,winp->cy,FALSE,FALSE);
					nxtline=FALSE;
				}
				else
				{
					restlen=(long)opt.maxedcol-strlen(winp->lines[l-1]);
					if (restlen>0)
					{
						if (restlen>strlen(cp)) /* passt die Zeile hinein */
						{
							strcat(winp->lines[l-1],cp);
							del_line(win,l,FALSE);
							red=TRUE;
							nxtline=FALSE;
						}
						else
						{
							cy2=winp->cy+l-winp->lnr;
							ptr=(byte *)(cp+restlen);
							while(ptr>=cp && *ptr!='\n' && *ptr!=' ' &&
							      *ptr!='-' && *ptr!=',')  /* Wortende suchen */
							{
								ptr--;
							}
							if (ptr>cp)
							{
								if (*ptr)
									strcpy(tmp,ptr+1);
								else
									*tmp=EOS;
								
								red=(word)*ptr; /* char zw.speichern */
								*ptr=EOS;
								strcat(winp->lines[l-1],cp);
								if (red=='-')
									strcat(winp->lines[l-1],"-");
								else if (red==',')
									strcat(winp->lines[l-1],",");
								
								if (*tmp)
									strcpy(cp,tmp);
								else
								{
									del_line(win,l,FALSE);
									cy2=winp->bzeilen-1;
									nxtline=FALSE;
								}
								red=TRUE;
							}
						}
						if (red)
							addendspc(winp->lines[l-1]);
					}
					else /* restlen>0 */
						nxtline=FALSE;
				}
			}
		}
		else
			nxtline=FALSE;
		l++;
	}
  if (red)
		pline(win,winp->cy,cy2,TRUE,FALSE);
}

void fkey_pressed(long win, word fknr)
{	byte tmp[512],tmp2[10],*cp;
	word old_indent,i,j;
	struct { ulong type, what; } buf[20];
	word msg[8];

	store_msghdr(win);				/* ssl 171294 */
	strcpy(tmp,gl_fkeys[fknr]);

	if (  (*tmp=='%' || *tmp=='&' || *tmp=='$') &&
			strlen(tmp)>=3 && 
			tmp[1]=='[' &&
			(cp=strchr(tmp,']'))!=NULL)
	{	if (*tmp=='&' && wi[win].editierbar)
		{	show_mouse();
			LangAlert(MSG_MSGWINDMACRO);
			hide_mouse();
			return;
		}
		if (*tmp=='%' && !wi[win].editierbar)
		{	LangAlert(MSG_EDITWINDMACRO);
			return;
		}
		*cp++=EOS;
		*cp=EOS;
		old_indent=(word)strlen(tmp)-1;
		if (old_indent<=5 || old_indent % 6 !=0)
		{	if (wi[win].editierbar) show_mouse();
			LangAlert(MSG_MACROERROR);
			if (wi[win].editierbar) hide_mouse();
			return;
		}
		i=0;
		cp=&tmp[2];

		while (*cp)
		{	strncpy(tmp2,cp,4); tmp2[4]=EOS;
			buf[i].what = SWAP(strtoul(tmp2,NULL,16));
			buf[i].what |= cp[4] - '0';
			buf[i].type = 3;
			cp+=6;
			i++;
		}
#if 0
		if (i>0)
			appl_tplay(&buf[0],i,100);
#else
		if (i>0)
		{	for (j=0;j<i;j++)
			{	msg[0]=0x5142;
				msg[1]=gl_apid;
				msg[3]=(uword) buf[j].what;
				msg[4]=(uword) (buf[j].what >> 16);
				msg[2]=
				msg[5]=
				msg[6]=
				msg[7]=0;
				appl_write(gl_apid,16,&msg);
			}
		}
		appl_write(i,16,&msgbuff);
#endif
	}
	else
	{
	if (!wi[win].editierbar) return;
	convert_percent(tmp,wi[win].aktarea,FALSE);

	old_indent=wi[win].indent;
	wi[win].indent=-1;

	cp=tmp;										/* ssl 171294: war vorher gl_fkeys[fknr]; */
	while (*cp)
	{
		if (*cp=='\n' || *cp=='\r')
			c_return(win);
		else	
			output(win, *cp, TRUE);
		cp++;
	}
	wi[win].indent=old_indent;
	}
}

void nomemalert(word mode)
{	mouse_normal();
	show_mouse();
	
	switch (mode)
	{
		case 0:
			LangAlert(MSG_NOMEMFORLINE);
			break;
		case 1:
			LangAlert(MSG_NOMEMHEADER);
			break;
		case 2:
			LangAlert(MSG_NOMEMMESSAGE);
			break;
		case 3:
			LangAlert(MSG_NOMEMCACHE);
			break;
		case 4:
			LangAlert(MSG_NOMEMCUTLINE);
			break;
		case 5:
			LangAlert(MSG_NOMORELINES);
	}
}
