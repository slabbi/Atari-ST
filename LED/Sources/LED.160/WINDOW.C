#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"
#include "vars.h"
#include "language.h"

extern void effect_v_gtext(long win,word x,word y,word lc,byte *text);

#define STATUS_JUSTIFY 			-2
#define STATUSLINE_JUSTIFY	 1

void upd_status(long win, byte *s)
{	word x,y,xy[4],pxy[4],eff;

	strcpy(wi[win].status,s);

	if (gem_statusline || wi[win].listwind || FROM_LINE==1)
	{	wind_set(wi[win].handle,WF_INFO,(long)wi[win].status,0,0);
	}
	else
	{	set_wiclip(win);
		set_wifont(win);
		get_wiwork(win);

		x=wi[win].x+4;
		y=wi[win].y;
		xy[0]=wi[win].x;
		xy[2]=xy[0]+wi[win].w-1;

		vsl_color(handle,BLACK);

		eff=opt.texteffects;
		opt.texteffects|=O_TEXTEFFSCR;

		hide_mouse();
		wind_update(BEG_UPDATE);
		wind_get(wi[win].handle,WF_FIRSTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		while (pxy[2] && pxy[3])
		{
			pxy[2] += pxy[0]-1;
			pxy[3] += pxy[1]-1;
			if (pxy[2] > gl_x2desk)	pxy[2]=gl_x2desk;
			if (pxy[3] > gl_y2desk)	pxy[3]=gl_y2desk;
			vs_clip(handle,TRUE,pxy);

			xy[1]=wi[win].y;
			xy[3]=xy[1]+gl_hbox;
			vr_recfl(handle,xy);
			effect_v_gtext(win,x,y+gl_hbox+STATUS_JUSTIFY,-2,s);
			xy[3]+=STATUSLINE_JUSTIFY;
			xy[1]=xy[3];
			v_pline(handle,2,xy);

			wind_get(wi[win].handle,WF_NEXTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		}
		opt.texteffects=eff;
		wind_update(END_UPDATE);
		show_mouse();
	}
}

void new_status(long win, word status)
{	byte tmp[1024],flags[80],*p;

	if (status != NO_CHANGE)
	{	strcpy(flags,"(");

		if (wi[win].ramhdr)
			strcat(flags,"HC,");

		switch(opt.umlaut & (O_ISOCONV|O_MIMESTYLE))
		{	case O_ISOCONV:		strcat(flags,"RC,"); break;
			case O_MIMESTYLE:	strcat(flags,"MI,"); break;
			case O_MIMESTYLE|O_ISOCONV:	strcat(flags,"RM,"); break;
		}

		if (opt.texteffects & O_TEXTEFFSCR)
			strcat(flags,"TE,");

#if 0
		if (gl_area[wi[win].aktarea].flag & (AR_CONVERT_LAT1S|AR_CONVERT_LAT1S))
			strcat(flags,"LC,");
#endif

		p=strchr(flags,EOS);
		if (*(p-1)=='(')
			*(p-1)=' ';
		else
			strcpy(p-1,") ");

		if (strlen(wi[win].status)+strlen(flags)>=MAX_STATUSLINE)
			wi[win].status[MAX_STATUSLINE-strlen(flags)-1]=EOS;

		strcpy(tmp,wi[win].status);
		strcpy(wi[win].status,flags);
		strcat(wi[win].status,tmp);

		*flags=EOS;

		if (status==END_OFMSG)
		{	strcpy(flags,msgtxt[SYSTEM_ENDOFMSGS].text);
			gl_area[wi[win].aktarea].newmsg &= ~UNRDMSG;
		}
		else if (status==START_OFMSG)
		{	strcpy(flags,msgtxt[SYSTEM_STARTOFMSGS].text);
		}
		else if (status==NO_MSGINAREA)
		{	strcpy(flags,msgtxt[SYSTEM_NOMSGS].text);
		}
					
		if (*flags)
		{	if (strlen(wi[win].status)+strlen(flags)>=MAX_STATUSLINE)
				wi[win].status[MAX_STATUSLINE-strlen(flags)-1]=EOS;
			strcat(wi[win].status,flags);
		}

		upd_status(win,wi[win].status);
	}
}

word chk_status(long win, word redraw)
{	WIND *winp;
	word ret;
	winp=&wi[win];
	if (winp->aktmsg==0)
		ret=NO_MSGINAREA;
	else if (winp->aktmsg==srch_down(win,winp->lastmsg))
		ret=END_OFMSG;
	else if (winp->aktmsg==srch_up(win,winp->firstmsg))
		ret=START_OFMSG;
	else
		ret=ST_BLANK;
	if (redraw)
		new_status(win,ret);
	return(ret);
}

word align(word x, word n)
{	x += (n >> 2) - 1;
	x = n * (x / n);
	return (x);
}

word alignx(word x)
{	x++;
	x&=~7;
	x+=3;
	return(x);
}

void align_edit(long win, word *x, word *y, word *w, word *h)
{	word oldw;

	if (!opt.sredraw)
		get_maxwindcol(win,opt.maxedcol,w,h);
	else
	{	
#if 1
		oldw=*w;
#endif
		get_maxwindcol(win,opt.maxedcol,w,h);
#if 1
		*w=oldw;
#endif
		if (*w > full.w)
			*w = full.w;
#if 0
		if (*h > full.h)
			*h = full.h;
#endif
	}
	
	if (*x + *w > full.w)
		*x = full.w - *w;
	*x = alignx(*x);
	if (*y + *h - full.y > full.h)
		*y = full.h - *h + full.y;
}

word new_window(long win, word elements, byte *name)
{	word handle;
	word xwork,ywork,wwork,hwork,yworkmin;
	word xw,yw,ww,hw;
	ulong temp;

	wind_calc(WC_WORK,elements,full.x,full.y,full.w,full.h,&xwork,&ywork,&wwork,&hwork);
	yworkmin=ywork;
	xwork+=4;
	ywork=((ywork+3)/gl_hbox)*gl_hbox;			/* war hbox	 */
	wwork=(((wwork-8)/gl_wbox)*gl_wbox)+4;	/* war wchar */
	if (ywork<yworkmin)	ywork=yworkmin; 				/* Snap in die Menzeile */
	
	if (wi[win].px<=0 || wi[win].py<=0 || wi[win].pw<=0 || wi[win].ph<=0)
	{
		wi[win].px=0;

		temp=(ulong)opt.winprm[win][0] * (full.w+1);
		xw=(uword)(temp >> 15);

		temp=(ulong)opt.winprm[win][1] * (full.y+full.h+1);
		yw=(uword)(temp >> 15);

		temp=(ulong)opt.winprm[win][2] * (full.w+1);
		ww=(uword)(temp >> 15);

		temp=(ulong)opt.winprm[win][3] * (full.y+full.h+1);
		hw=(uword)(temp >> 15);
	}
	else
	{
		xw=wi[win].px;		/* +gl_wbox */
		yw=wi[win].py;
		ww=wi[win].pw;
		hw=wi[win].ph;
	}
	if (xw>0 && yw>0 && ww>0 && hw>0)
	{
#if 0
		xw-=gl_wbox;
#endif
		if (xw<full.x || xw>full.w)
			xw=full.x;
		else
			xw=alignx(xw);
		if (yw<full.y || yw>=full.y+full.h)
			yw=full.y;
		if (ww>full.w || ww<=10*gl_wbox)
			ww=full.w;
		if (hw>full.h || hw<=wi[win].topmsg_line*gl_hbox)
			hw=full.h;
		wind_calc(WC_WORK,elements,xw,yw,ww,hw,&xw,&yw,&ww,&hw);
	}
	else
	{
		xw=xwork;
		yw=ywork;
		ww=wwork;
		hw=hwork;
	}
	
	wind_calc(WC_BORDER,elements,xw,yw,ww,hw,&xwork,&ywork,&wwork,&hwork);
	if (wi[win].px==0 && (opt.winprm[win][0]<=0 || opt.winprm[win][1]<=0 ||
	    opt.winprm[win][2]<=0 || opt.winprm[win][3]<=0))
	{
		xwork+=(word)(win*8);
		ywork+=(word)(win*4);
	}
	xwork=alignx(xwork-1);
	
	wi[win].px=xwork;
	wi[win].py=ywork;
	wi[win].pw=wwork;
	wi[win].ph=hwork;
	
	handle = wind_create(elements,full.x,full.y,full.w,full.h);
	if (handle > 0)
	{
		if (elements & NAME)
			wind_set(handle,WF_NAME,(long)name,0,0);
		if (elements & INFO)
			wind_set(handle,WF_INFO,"***new_window***" /*(long)wi[win].status*/,0,0);
		wind_open(handle,xwork,ywork,wwork,hwork);
	}
	return(handle);
}


word
rc_intersect(word p1[],word p2[])
{
	int tw = min(p1[0] + p1[2], p2[0] + p2[2]);
	int th = min(p1[1] + p1[3], p2[1] + p2[3]);
	int tx = max(p1[0], p2[0]);
	int ty = max(p1[1], p2[1]);
	
	p2[0] = tx;
	p2[1] = ty;
	p2[2] = tw - tx;
	p2[3] = th - ty;
	
	return ((tw > tx) && (th > ty));
}

void set_wiclip(long win)
{	word pxy[4];
	WIND *winp;
	winp=&wi[win];
	get_wiwork(win);
	pxy[0]=winp->x;
	pxy[1]=winp->y;
	pxy[2]=winp->w+pxy[0]-1;
	pxy[3]=winp->h+pxy[1]-1;
	vs_clip(handle,TRUE,pxy);
}

void get_wiwork(long win)
{	WIND *winp;
	winp=&wi[win];
	wind_get(winp->handle, WF_WORKXYWH, &winp->x, &winp->y, &winp->w , &winp->h);
	if (winp->x+winp->w > gl_x2desk)
		winp->w=gl_x2desk-winp->x;
	if (winp->y+winp->h > gl_y2desk)
		winp->h=gl_y2desk-winp->y;
}

word top_window(void)
{	int handle,d;
	wind_get(0,WF_TOP,&handle,&d,&d,&d);
	return(handle);
}

void writeflag(WIND *winp, byte *s, word fl, byte *what)
{	if (winp->hdr->wFlags & fl)
		strcat(s,what);
}

#if defined(__NEWFIDO__)
	void writeextflag(long win, byte *s, ulong fl, byte *what)
	{	if (wi[win].hdr->XFlags & fl)
			strcat(s,what);
	}
	
	void writemausflag(long win, byte *s, ulong fl, byte *what)
	{	if (wi[win].hdr->MFlags & fl)
			strcat(s,what);
	}
	
	void get_maus_flags(long win, byte *s)
	{	if (wi[win].hdr->XFlags & XF_MAUSMSG)
		{	strcat(s," ");
			writemausflag(win,s,MF_NICHTGELESEN,"n");
			writemausflag(win,s,MF_ZURUECK,"z");
			writemausflag(win,s,MF_BEANTWORTET,"b");
			writemausflag(win,s,MF_GELESEN,"g");
			writemausflag(win,s,MF_WEITER,"w");
			writemausflag(win,s,MF_MAUSNET,"m");
			writemausflag(win,s,MF_ANGEKOMMEN,"a");
			writemausflag(win,s,MF_GATEWAY,"y");
			writemausflag(win,s,MF_KOPIERT,"k");
		}
	}
#else
	void writeprocflag(WIND *winp, byte *s, uword fl, byte *what)
	{	if (winp->hdr->wProcessed & fl)
			strcat(s,what);
	}

	void writeextflag(WIND *winp, byte *s, uword fl, byte *what)
	{	if (winp->hdr->XFlags & fl)
			strcat(s,what);
	}
	
	void writemausflag(WIND *winp, byte *s, uword fl, byte *what)
	{	if (winp->hdr->MFlags & fl)
			strcat(s,what);
	}

	void get_maus_flags(WIND *winp, byte *s)
	{	if (winp->hdr->XFlags & XF_MAUSMSG)
		{	strcat(s," ");
			writemausflag(winp,s,MF_NICHTGELESEN,"n");
			writemausflag(winp,s,MF_ZURUECK,"z");
			writemausflag(winp,s,MF_BEANTWORTET,"b");
			writemausflag(winp,s,MF_GELESEN,"g");
			writemausflag(winp,s,MF_WEITER,"w");
			writemausflag(winp,s,MF_MAUSNET,"m");
			writemausflag(winp,s,MF_ANGEKOMMEN,"a");
			writemausflag(winp,s,MF_GATEWAY,"y");
			writemausflag(winp,s,MF_KOPIERT,"k");
			writemausflag(winp,s,MF_UNBEKANNT,"u");
			writemausflag(winp,s,MF_MAUSTAUSCH,"t");
			writemausflag(winp,s,MF_INTERESTING1,"1");
			writemausflag(winp,s,MF_INTERESTING2,"2");
			writemausflag(winp,s,MF_HEREDITARY,"h");
			writemausflag(winp,s,MF_RESERVED14,"r14");
			writemausflag(winp,s,MF_RESERVED15,"r15");
		}
	}
#endif

void get_flags(long win,byte *s1,byte *s2,byte *s3)	/* ssl 291294: so mag ich es... */
{	WIND *winp;
	char tmp[512],*p;
	winp=&wi[win];
	*tmp=EOS;
	writeflag 	(winp,tmp, F_LOCAL,					" Local");
	writeflag 	(winp,tmp, F_PRIVATE, 				" Pvt");
	writeextflag(winp,tmp, XF_FIXEDADDRESS,	" Fixed");
	writeflag 	(winp,tmp, F_KILLSENT,				" K/S");
	writeflag 	(winp,tmp, F_CRASH,		   		" Crash");
	writeflag 	(winp,tmp, F_HOLD,				 		" Hold");
	writeextflag(winp,tmp, XF_IMMEDIATE,	  	" Immed");
	writeextflag(winp,tmp, XF_DIRECT,    	  " Direct");
	writeflag 	(winp,tmp, F_SENT,   	 			" Sent");
	writeflag 	(winp,tmp, F_FILEATTACH,			" W/F");
	writeextflag(winp,tmp, XF_MULTIPLECC		 ," CC");
	writeextflag(winp,tmp, XF_TRUNCFILESENT,	" TFS");
	writeextflag(winp,tmp, XF_KILLFILESENT, 	" KFS");
	writeflag 	(winp,tmp, F_FILEREQ,				" FReq");
	writeflag 	(winp,tmp, F_RECEIVED,				" Recd");
	writeflag 	(winp,tmp, F_RETRECREQ,			" RetRec");
	writeflag 	(winp,tmp, F_ISRETREC,				" IsRec");
	writeflag 	(winp,tmp, F_AUDITREQ,				" AudRec");
	writeflag 	(winp,tmp, F_INTRANSIT,			" InTr");
	writeflag 	(winp,tmp, F_ORPHAN,  				" Orph");
	writeextflag(winp,tmp, XF_ARCHIVSENT,  	" Arc/S");
	writeextflag(winp,tmp, XF_LOCK,     	   	" Locked");
	writeextflag(winp,tmp, XF_GATED,					" Gated");
	writeextflag(winp,tmp, XF_HOSTROUTE, 	  " HostR");
	writeextflag(winp,tmp, XF_ZONEGATE,    	" ZGate");
	writeextflag(winp,tmp, XF_SIGNATURE		 ," Sign");
	writeextflag(winp,tmp, XF_CREATEFLOWFILE," CFF");
	writeflag 	(winp,tmp, F_DELETED, 				" Del");
	writeflag 	(winp,tmp, F_RESERVED,				" R10");
	writeextflag(winp,tmp, XF_RESERVED12		 ," R12");
	writeextflag(winp,tmp, XF_MAUSMSG,				" Maus");

	writeprocflag(winp,tmp, 0x200,		" Uucp");

	get_maus_flags(winp,tmp);

	if (strlen(tmp)<=30)
	{	sprintf(s1,"%30s",tmp);
		*s2=*s3=EOS;
		return;
	}

	p=tmp+30;
	while(p>tmp && *p!=' ') p--;
	*p=EOS;
	sprintf(s1,"%30s",tmp);
	strcpy(tmp,p+1);
/****************************/
	if (strlen(tmp)<=30)
	{	sprintf(s2,"%30s",tmp);
		*s3=EOS;
		return;
	}

	p=tmp+30;
	while(p>tmp && *p!=' ') p--;
	*p=EOS;
	sprintf(s2,"%30s",tmp);
	strcpy(tmp,p+1);
/*****************************/	
	if (strlen(tmp)<=29)
	{	sprintf(s3,"%30s",tmp);
		return;
	}
	tmp[30]='>';
	tmp[31]=EOS;
	sprintf(s3,"%31s",tmp);
}

void set_wislider(long win)
{	word stmp, wi_maxcol;
	WIND *winp;
	
	winp=&wi[win];
	if (winp->editierbar)
		wi_maxcol=opt.maxedcol;
	else
	{	if (winp->listwind)
			wi_maxcol=MAXCOL_LISTWIND-1;
		else
			wi_maxcol=opt.maxcol;
	}
	
	get_wiwork(win);
		
	winp->bspalten = (winp->w-4)/gl_wbox;
	winp->bzeilen = (winp->h-gl_hbox+gl_hchar)/gl_hbox;
	winp->maxshow = winp->bzeilen-winp->topmsg_line;
	if (winp->maxshow < 0)
		winp->maxshow = 0;
	if (winp->lzahl>winp->maxshow)
	{
		if (winp->topline>winp->lzahl-winp->maxshow)
			winp->topline=winp->lzahl-winp->maxshow;
	}
	else
		winp->topline=0;

	if (winp->editierbar && winp->cy>=TOPMSG_LINE)
	{
		winp->cy = winp->lnr-winp->topline+winp->topmsg_line;
		if (winp->cy >= winp->bzeilen)
		{
			winp->cy = winp->bzeilen-1;
			winp->lnr = winp->topline+winp->cy-winp->topmsg_line;
		}
		if (winp->cx >= winp->bspalten)
			winp->cx = winp->bspalten-1;
		set_cxpos(win);
	}
	
	hide_mouse();
		
	if (winp->lzahl <= winp->maxshow)
		stmp=1;
	else 
		stmp=(word)((winp->topline)*1000L/(winp->lzahl-winp->maxshow));
	wind_set(winp->handle,WF_VSLIDE,stmp,0,0,0);
     
	if (winp->lzahl>0)
		stmp=min(1000,(word)(winp->maxshow*1000L/winp->lzahl));
	else 
		stmp=1000;
	wind_set(winp->handle,WF_VSLSIZE,stmp,0,0,0);
	
	if (winp->bspalten > wi_maxcol)
		stmp=1000;
	else 
		stmp=(word) (winp->firstc*1000L/(wi_maxcol+1-winp->bspalten));
	wind_set(winp->handle,WF_HSLIDE,stmp,0,0,0);
	
	wind_set(winp->handle,WF_HSLSIZE,min(1000,(int)(winp->bspalten*1000L/(wi_maxcol+1))),0,0,0);
	
	show_mouse();
}


void sys_wisetfiles(long win, word leermachen)
{	long topwin;
	WIND *winp;
	word line,lc,x,y,effect_fl;
	byte from[MAX_STR], to[MAX_STR], topic[MAX_STR], time[MAX_STR];
	byte onfrom[MAX_STR], onto[MAX_STR];
	byte fflags1[MAX_STR], fflags2[MAX_STR], fflags3[MAX_STR];
	word pxy[4];

	winp=&wi[win];
	topwin=found_wind();
	if (win==topwin)
	{	
		if (winp->listwind==WI_LIST_AREAS)
			set_menu(ALIST_WIND);
		else if (winp->listwind==WI_LIST_MSGS)
			set_menu(MLIST_WIND);
		else if (winp->lastmsg==0)
			set_menu(NO_MSG);
		else if (winp->aktmsg==0)
			set_menu(DEL_MSG);
		else if (winp->editierbar)
			set_menu(EDIT_WIND);	
		else		
			set_menu(MSG_WIND);	
	}
	
/*	set_wiclip(win); */
	set_wifont(win);
	set_wislider(win);

	get_wiwork(win);
	x=winp->x+4;
	y=winp->y;
	
	hide_mouse();
	
	if (leermachen)
		pxy[1]=winp->y;
	else
		pxy[1]=winp->y+winp->topmsg_line*gl_hbox;
	pxy[0]=winp->x;
	pxy[2]=pxy[0]+winp->w-1;
	pxy[3]=pxy[1]+winp->h-1;
	vr_recfl(handle,pxy);
	
/*	hide_mouse(); */
		
	lc=winp->topline;
	
	if (leermachen && winp->topmsg_line!=0)
	{
		sprintf(from, msgtxt[DIV_FROM].text,winp->hdr->sFrom);
		sprintf(to,   msgtxt[DIV_TO  ].text,winp->hdr->sTo);
		sprintf(topic,msgtxt[DIV_SUBJ].text,winp->hdr->sSubject);
		sprintf(time, msgtxt[DIV_MSGTIME].text,winp->aktmsg,
															winp->lastmsg,winp->hdr->sDate);

		/*					vvvvvvvvvvvvvvvvv war ==0 */
		if (is_netmail(winp->aktarea) ||
		    !stricmp(gl_area[winp->aktarea].name,"ExtraExpArea") || 
		    !stricmp(gl_area[winp->aktarea].name,"Bad_Msgs"))
		{
			if (winp->hdr->wFromZone!=0 || winp->hdr->wFromNet !=0 ||
					winp->hdr->wFromNode!=0 || winp->hdr->wFromPoint!=0)
			{	sprintf(onfrom,msgtxt[DIV_ON].text,
																mergeaddr(winp->hdr->wFromZone,
																					winp->hdr->wFromNet,
			    	    													winp->hdr->wFromNode,
			      	  													winp->hdr->wFromPoint,NULL));
				strcat(from,onfrom);
			}
			if (winp->hdr->wToNet!=0)
			{	sprintf(onto,msgtxt[DIV_ON].text,
															mergeaddr(winp->hdr->wToZone,
																				winp->hdr->wToNet,
				        												winp->hdr->wToNode,
				        												winp->hdr->wToPoint,
				        												gl_area[winp->aktarea].domain));
				strcat(to,onto);
			}
		}
		else if (!is_netmail(winp->aktarea) &&
						 gl_area[winp->aktarea].flag & AR_FIXEDADDRESS)
		{	if (winp->hdr->wFromZone!=0 || winp->hdr->wFromNet !=0 ||
					winp->hdr->wFromNode!=0 || winp->hdr->wFromPoint!=0)
			{	sprintf(onfrom,msgtxt[DIV_ON].text,
																mergeaddr(winp->hdr->wFromZone,
																					winp->hdr->wFromNet,
			    	    													winp->hdr->wFromNode,
			      	  													winp->hdr->wFromPoint,NULL));
				strcat(from,onfrom);
			}
#if 0
			if (winp->hdr->wToNet!=0)
			{	sprintf(onto," via %s",mergeaddr(winp->hdr->wToZone,
																				winp->hdr->wToNet,
				        												winp->hdr->wToNode,
				        												winp->hdr->wToPoint,NULL));
				strcat(to,onto);
			}
#endif
		}
		
		get_flags(win,fflags1,fflags2,fflags3);
/*		sprintf(fflags1,"%30.30s",flags); */
		
		vst_effects(handle,0);

		if (!gem_statusline)
		{	y+=gl_hbox;
			effect_v_gtext(win,x,y+STATUS_JUSTIFY,-2,winp->status);

/*		pxy[0]=winp->x;
			pxy[2]=pxy[0]+winp->w-1;   haben wir schon oben getan */
			pxy[1]=pxy[3] = winp->y+gl_hbox+STATUSLINE_JUSTIFY;
			vsl_color(handle,BLACK);
			v_pline(handle,2,pxy);
		}

		vst_effects(handle,0);

		y+=gl_hbox;
		v_gtext(handle,x+(45*8),y,fflags1);
		v_gtext(handle,x+(45*8),y+gl_hbox,fflags2);
		v_gtext(handle,x+(45*8),y+(gl_hbox << 1),fflags3);

		v_gtext(handle,x,y,time);

		y+=gl_hbox;
		effect_v_gtext(win,x,y,-1,from);

		y+=gl_hbox;
/*	effect_v_gtext(win,x,y,-1,to); */
		v_gtext(handle,x,y,to);

		y+=gl_hbox;
		effect_v_gtext(win,x,y,-1,topic);
/*	v_gtext(handle,x,y,topic); */

		y+=gl_hbox;

#if 0
		vsf_interior(handle,FIS_PATTERN);
		vsf_style(handle,4);
		vsf_color(handle,BLACK);
		pxy[1]=pxy[3]=y-(gl_hbox >> 1);
		pxy[3]+=2;
		vr_recfl(handle,pxy);
#else
		vsf_interior(handle,FIS_SOLID);
		vsf_style(handle,0);
		vsf_color(handle,BLACK);
		vsl_color(handle,BLACK);
		pxy[1]=pxy[3]=y-(gl_hbox >> 1)+1;
		pxy[3]++;
		vr_recfl(handle,pxy);
		if (gl_hbox>=8)
		{	pxy[1]=pxy[3]=y-(gl_hbox >> 1)-1;
			v_pline(handle,2,pxy);
			pxy[1]=pxy[3]=y-(gl_hbox >> 1)+4;
			v_pline(handle,2,pxy);
		}
#endif
		vsf_interior(handle,FIS_SOLID);
		vsf_style(handle,0);
		vsf_color(handle,WHITE);

/*	v_gtext(handle,x,y,"---------------------------------------------------------------------------------"); */
	}
	else
		y+=winp->topmsg_line*gl_hbox;
	
	x -= winp->firstc*gl_wbox;

	if (winp->lines[0]!=NULL || winp->editierbar)
	{

		if (winp->listwind==WI_LIST_AREAS)
		{	for (line=1;line<=winp->maxshow+1;line++)
			{	
				if (lc < winp->lzahl)
				{	y+=gl_hbox;
					effect_fl=0;
					if (lc==winp->selline)
						effect_fl |= EFF_INVERS;
					my_gtext(x,y,winp->lines[lc],effect_fl);
	#if 0
					{	int pxy[4];
						pxy[0]=pxy[2]=winp->x;
						pxy[2]+=winp->w;
						pxy[1]=pxy[3]=y+1;
						vsl_color(handle,1);
						v_pline(handle,2,pxy);
					}
	#endif
					lc++;
				}		
			}
		}
		else if (winp->listwind==WI_LIST_MSGS)
		{	for (line=1;line<=winp->maxshow+1;line++)
			{	
				if (lc < winp->lzahl)
				{	
					y+=gl_hbox;
					effect_fl=0;
					if (lc==winp->selline)
						effect_fl |= EFF_INVERS;

					if (winp->lines[lc][1]=='d')
						effect_fl |= EFF_HELL;
	#if 0
					else if (winp->lines[lc][1]=='x')
						effect_fl |= EFF_KURSIV;
	#endif
/* Hier stand ein else vor dem if, genauso in WINDLIST.C */
					if (winp->lines[lc][0]=='\x3')	
						effect_fl |= EFF_FETT;
					my_gtext(x,y,winp->lines[lc],effect_fl);
					lc++;
				}		
			}
		}
		else
		{ 
			effect_fl=winp->editierbar;

			for (line=1;line<=winp->maxshow+1;line++)
			{	if (lc<winp->lzahl)
				{	y+=gl_hbox;
					effect_v_gtext(win,x,y,effect_fl ? lc : -1,winp->lines[lc]);
					lc++;
				}
			}
		}
	}
	show_mouse();
}

void wisetfiles(long win, word leermachen, word stat)
{	word pxy[4];

	new_status(win,stat);			/* ssl 120395 */
	wind_get(wi[win].handle,WF_FIRSTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
	while (pxy[2] && pxy[3])
	{
		pxy[2] += pxy[0]-1;
		pxy[3] += pxy[1]-1;
		if (pxy[2] > gl_x2desk)
			pxy[2]=gl_x2desk;
		if (pxy[3] > gl_y2desk)
			pxy[3]=gl_y2desk;
		vs_clip(handle,TRUE,pxy);
		sys_wisetfiles(win,leermachen);
		wind_get(wi[win].handle,WF_NEXTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
	}
}

void get_maxwindcol(long win, word maxcol, word *w, word *h)
{	word wdiff, hdiff;
	word currw, currh;
	word workw, workh;
	word dummy;
	
	set_wifont(win);
	wind_get(wi[win].handle,WF_CURRXYWH,&dummy,&dummy,&currw,&currh);
	wind_get(wi[win].handle,WF_WORKXYWH,&dummy,&dummy,&workw,&workh);
	wdiff=currw-workw;
	hdiff=currh-workh;
	
	*w = (maxcol+2)*gl_wbox+wdiff;
	if (*w > full.w)
		*w = full.w;
	if (wi[win].editierbar)
	{
		if (*h - hdiff < MIN_LINES_EDIT_WIND*gl_hbox+4)
		{
			*h = MIN_LINES_EDIT_WIND*gl_hbox+4+hdiff;
			if (*h > full.h)
				*h = full.h;
		}
	}
}

void redraw_all(void)
{	form_dial(FMD_START, 0, 0, 0, 0, gl_xdesk, gl_ydesk, gl_wdesk, gl_hdesk);
	form_dial(FMD_FINISH,0, 0, 0, 0, gl_xdesk, gl_ydesk, gl_wdesk, gl_hdesk);
}

word open_defltwind(void)
{	long win,this_should_be_on_top=-1;
	word i,akt_area=0;
	word ret;
	word stat;
	word neuladen;
	
	show_doing(DO_OPENMESSAGEAREA);
	biene();
	ret=FALSE;
	win=MAX_WIND;    /* Das erste, "Default"-Fenster wird ausgew„hlt */
	while (win>0)
	{
		win--;

		if (opt.winprm[win][0]<=0 || opt.winprm[win][1]<=0 ||
				opt.winprm[win][2]<=0 || opt.winprm[win][3]<=0 ||
				opt.winprm[win][6]==0)
		{	continue;
		}
		else
		{	
/*		if (win==0 && (opt.winprm[win][0]<=0 || opt.winprm[win][1]<=0))
				wi[win].aktarea=0;
			else */

			wi[win].aktarea=opt.winprm[win][4];
			if (wi[win].aktarea>=gl_arzahl)
				wi[win].aktarea=0;
			
			neuladen=TRUE;
			for (i=0;i<MAX_WIND;i++)			/* gibt es die Area schon? */
			{	if (wi[i].belegt && wi[i].aktarea==wi[win].aktarea &&
						i!=win)
				{	del_doing();
					top_it(i);
					neuladen=FALSE;
					break;
				}
			}

			if (neuladen && open_area(win, wi[win].aktarea))
			{
/*			if (opt.winprm[win][5]>0)
					wi[win].aktmsg=opt.winprm[win][5]; */
				akt_area = wi[win].aktarea;		/* ssl 111294 */
				del_doing();

				open_window(win,wi[win].areaname,TRUE);
#if 1
				stat=select_msg(win,wi[win].aktmsg,FALSE,TRUE);
#else
				stat=select_msg(win,0,FALSE,TRUE);		/* 0 war wi[win].aktmsg */ 
#endif
				set_comstatus(win,wi[win].aktmsg);

				new_status(win,stat);
				this_should_be_on_top=win;
				ret=TRUE;
			}
		}
	}

	if (opt.listawindow)		/* ssl 91294: erzeuge areawindow */
	{	if (
	#if 0
				opt.winprm[AREAWIND][0]>=0 && opt.winprm[AREAWIND][1]>=0 &&
				opt.winprm[AREAWIND][2]>=0 && opt.winprm[AREAWIND][3]>=0 &&
	#endif
				opt.winprm[AREAWIND][6]==1)
		{	show_doing(DO_OPENAREAWINDOW);
			if (!wi[AREAWIND].belegt)
			{	
				wi[AREAWIND].aktarea=akt_area;
/*				wi[AREAWIND].aktarea=opt.winprm[AREAWIND][4]; */
/*				wi[AREAWIND].aktmsg=opt.winprm[AREAWIND][5];	*/
				del_doing();
				openarea_listwindow(AREAWIND, wi[AREAWIND].aktarea,TRUE);
			}
		}
	}

	if (this_should_be_on_top>=0)			/* ssl 101294 */
	{	
		if (opt.listmwindow)
		{	
			if (!wi[MSGSWIND].belegt)
				if (
	#if 0
						opt.winprm[MSGSWIND][0]>=0 && opt.winprm[MSGSWIND][1]>=0 &&
						opt.winprm[MSGSWIND][2]>=0 && opt.winprm[MSGSWIND][3]>=0 &&
	#endif
						opt.winprm[MSGSWIND][6]==1)
				{	open_msglistwindow(this_should_be_on_top);		/* Nein, aber es soll eins ge”ffnet werden */
																												/* Also machen wir dieses auch!						 */
				}
		}

		top_it(this_should_be_on_top);
		set_wiclip(this_should_be_on_top);
		set_wifont(this_should_be_on_top);
	}

	win=0;
	del_doing();
	mouse_normal();
	return(ret);
}

void open_window(long win, byte *wname, word msgwindfl)
{	word elements;

#if 0
	switch(windowtyp)
	{	case	0	:
		case	1	:	elements=MAX_LINES_VIEW;	break;
		case	2	: elements=MAX_LINES_AREA;	break;
		case	3	:	elements=MAX_LINES_MSGS;	break;
		default	:	elements=1;
	}
	wi[win].number_of_lines=elements;
	wi[win].lines=(byte **)calloc(elements,sizeof(byte *));
	if (!wi[win].lines)
	{	LangAlert(MSG_OUTOFMEMORY);
		return;
	}
#endif

	strcpy(wi[win].areaname,wname);
	if (msgwindfl && !gem_statusline)
		elements = W_MSG;
	else
		elements = W_ALL;

	if (iconify_possible)
		elements |= ICONIFIER;

	wi[win].gadgets = elements;
	wi[win].handle = new_window(win, elements, wi[win].areaname);

	if (wi[win].handle <= 0)
		LangAlert(MSG_WINDOPENERROR);
	else
	{
		wi[win].maxshow = 0;
		wi[win].topline = 0;
		wi[win].belegt = TRUE;
		wi[win].firstc = 0;
		wi[win].topmsg_line = TOPMSG_LINE;
	}
}

word close_window(long win)
{	word ext_btn;

	if (wi[win].editierbar)
	{	show_mouse();
		ext_btn=LangAlert(MSG_CLOSEWINDOW);
		if (ext_btn==3)
			return FALSE;
		else if (ext_btn==1)
		{	edit_end(win,TRUE);
			return TRUE;
		}
		else if (ext_btn==2)
		{	edit_end(win,FALSE);
			return TRUE;
		}	
	}
	
	close_area(win);
	
	if (!wi[win].iconify)
	{	wind_get(wi[win].handle,WF_CURRXYWH,&wi[win].px,&wi[win].py,
	  	                                  &wi[win].pw,&wi[win].ph);
		wind_get(wi[win].handle,WF_CURRXYWH,
			         &opt.winprm[win][0],	&opt.winprm[win][1],
			         &opt.winprm[win][2],	&opt.winprm[win][3]);
	}
	else
	{	wind_calc(WC_BORDER,wi[win].gadgets,
									wi[win].xb,wi[win].yb,wi[win].wb,wi[win].hb,
									&(word)opt.winprm[win][0],&(word)opt.winprm[win][1],
									&(word)opt.winprm[win][2],&(word)opt.winprm[win][3]);
		wind_calc(WC_BORDER,wi[win].gadgets,
									wi[win].xb,wi[win].yb,wi[win].wb,wi[win].hb,
									&wi[win].px,&wi[win].py,
                  &wi[win].pw,&wi[win].ph);
	}
	setze_raster(win);
	
	if (wi[win].hdr!=NULL)
		free(wi[win].hdr);
	wi[win].hdr=NULL;
	
	wi[win].belegt=
	wi[win].iconify=
	wi[win].editierbar=FALSE;
	wi[win].firstc=0;		/* Zeichen, erste Spalte in Wind-Work */
	wi[win].firstl=0;		/* Zeichen, erste Zeile in Wind-Work */
	wi[win].cursor=FALSE;		/* Flag, Cursor da */
	wi[win].cx=0;			/* Zeichen, Cursorposition x */
	wi[win].cy=0;			/* Zeichen, Cursorposition y */
	wi[win].msgfilelen=0L;	/* L„nge des MSG-Files */

	memset(wi[win].lines,0,sizeof(wi[win].lines));

	wi[win].lnr=0;			/* Nummer der akt. Zeile */
	wi[win].edline=0;		/* pointer auf akt. Editier-Zeile */
	wi[win].firstmsg=0;		/* Nummer erster Msg in Area */
	wi[win].lastmsg=0;		/* Nummer letzter Msg in Area */
	wi[win].aktmsg=0;		/* Nummer der akt. Msg */
	wi[win].oldmsg=0;
	memset(wi[win].msgda,LEER,sizeof(wi[win].msgda));
	*wi[win].areaname=EOS;	/* akt. Area-Name */
	*wi[win].areapath=EOS;	/* akt. Area-Pfad */
	wi[win].aktarea=0;		/* Nummer der akt. Area */
	wi[win].maxshow=0;		/* Zahl (Zeichen) der max. Zeilen in Window */
	wi[win].lzahl=0;			/* Zahl der Zeilen in Msg-Puffer */
	wi[win].bspalten=0;		/* Zahl der Spalten in Window (Edit) */
	wi[win].bzeilen=0;		/* Zahl der Zeilen in Window (Edit) */
	wi[win].topline=0;		/* Index der 1. Zeile im Window aus Msg-Puffer */

	hide_mouse();
	wind_close(wi[win].handle);
	wind_delete(wi[win].handle);
	show_mouse();
	wi[win].handle = -1;
	if (found_wind()==-1L)
	{	word pxy[4];
		set_menu(NO_WIND);
		pxy[0]=full.x;
		pxy[1]=full.y;
		pxy[2]=full.x+full.w;
		pxy[3]=full.x+full.h;
		vs_clip(handle,FALSE,pxy);
	}
	return TRUE;	
}

void 
wiopen(void)
{	long win,i;
	
	win=-1;
	for (i=0;i<MAX_WIND;i++)
	{	if (wi[i].belegt==FALSE)
		{	win=i;
			break;
		}
	}
	if (win>=0)
	{	set_menu(MSG_WIND);
		if ((i=found_wind())>=0)
			wi[win].aktarea=wi[i].aktarea;
		if (!lade_wind(win))
		{	if (i>=0 && wi[i].editierbar)
				set_menu(EDIT_WIND);
		}
	}
	else
		LangAlert(MSG_NOWIND);
}

void 
wicycle(void)
{	long win,laufw;
	word wifound;
	
	wifound=FALSE;
	if ((win=found_wind())>=0)
	{
		laufw=win;
		while (laufw<TOTAL_WIND-1 && wifound==FALSE)
			if (wi[++laufw].belegt)
				wifound=TRUE;
		if (!wifound)
		{
			laufw=-1;
			while (laufw<win && wifound==FALSE)
				if (wi[++laufw].belegt)
					wifound=TRUE;
		}
		if (wifound && laufw!=win)
		{
			if (wi[win].editierbar)
				cursor(win,FALSE);
			top_it(laufw);
			set_wiclip(laufw);
			set_wifont(laufw);

			if (wi[laufw].editierbar)
			{	set_editfnt();
				set_menu(EDIT_WIND);
				s_cutbuff=e_cutbuff=-1;
				upded_stat(laufw,0);
				cursor(laufw,TRUE);
			}
			else if (wi[laufw].listwind==WI_LIST_MSGS)
			{	set_mlistfnt();	
			}
			else if (wi[laufw].listwind==WI_LIST_AREAS)
			{	set_alistfnt();	
			}
			else
			{	set_msgfnt();
				set_menu(MSG_WIND);	
			}
		}
	}
}

void wifull(long win)
{	RECHTECK prev, curr;
	
	wi[win].topline=0;
	wind_get(wi[win].handle,WF_CURRXYWH,&curr.x,&curr.y,&curr.w,&curr.h);
	wind_get(wi[win].handle,WF_PREVXYWH,&prev.x,&prev.y,&prev.w,&prev.h);
	
	set_wiclip(win);
	set_wifont(win);

	if (wi[win].editierbar)
	{	cursor(win,FALSE);
		align_edit(win,&prev.x,&prev.y,&prev.w,&prev.h);
	}

	if (curr.x==full.x && curr.y==full.y && curr.w==full.w-full.x && curr.h==full.h)
	{	wind_set(wi[win].handle,WF_CURRXYWH,prev.x,prev.y,prev.w,prev.h);
	}
	else
	{	wind_set(wi[win].handle,WF_CURRXYWH,full.x,full.y,full.w-full.x,full.h);
		wi[win].firstc=0;
	}

	set_wiclip(win);
	set_wifont(win);
	set_wislider(win);
	wi[win].oldcx = wi[win].cx;
	if (wi[win].editierbar && wi[win].oldcx >= wi[win].bspalten)
		wi[win].oldcx = wi[win].bspalten-1;
	wisetfiles(win,TRUE,NO_CHANGE);

	if (wi[win].editierbar)
		cursor(win,TRUE);
}		

word 						/* Topped Window nach Area-Index */	
srch_winarea(long win, word area)
{
	long laufw;
	
	for (laufw=0;laufw<MAX_WIND;laufw++)
	{
		if (wi[laufw].belegt && !wi[laufw].editierbar && 
			 !wi[laufw].listwind)			/* ssl 111294 */
		{
			if (wi[laufw].aktarea==area && laufw!=win)
			{
				top_it(laufw);
				return(TRUE);
			}
		}
	}
	return(FALSE);
}

long 
get_windarea(long win, word area)
{
	long laufw;
	
	for (laufw=0;laufw<MAX_WIND;laufw++)
	{
		if (wi[laufw].belegt && !wi[laufw].editierbar &&
				!wi[laufw].listwind)			/* ssl 101294 */
		{
			if (wi[laufw].aktarea==area && laufw!=win)
			{
				top_it(laufw);
				return(laufw);
			}
		}
	}
	return(-1);
}

long found_wind(void)	/* wi-Index vom Top-Window */
{	word whndl;
	long win;
	whndl=top_window();
	for (win=0;win<TOTAL_WIND;win++)
	{
		if (wi[win].belegt)
			if (wi[win].handle==whndl)
				return win;
	}		
	return -1L;		
}

word edit_window(long win)     /* Test ob Top-Window editierbar */
{	
		if (wi[win].editierbar)
			return TRUE;
		else
			return FALSE;
}	

word list_window(long win)	/* Test ob Top-Window Liste ist */
{	
		if (wi[win].listwind)
			return TRUE;
		else
			return FALSE;
}

long area_msgwind(word area_index)	/* wi-Index von Msg-Area */
{	long win;
	for (win=0;win<MAX_WIND;win++)
	{
		if (wi[win].belegt && wi[win].aktarea==area_index && wi[win].editierbar==FALSE)
			return win;
	}		
	return -1L;		
}

long get_freewind(void)
{	long i;
	for (i=0;i<MAX_WIND;i++)
		if (wi[i].belegt==FALSE)
			return (i);
	mouse_normal();
	show_mouse();
	LangAlert(MSG_NOWIND);
	return (-1L);		
}

long get_firstmsgwind(void)
{	long i;
 	for (i=0;i<MAX_WIND;i++)
 		if (wi[i].belegt && !wi[i].editierbar && !wi[i].listwind)
 			return i;
 	return (-1L);		
}