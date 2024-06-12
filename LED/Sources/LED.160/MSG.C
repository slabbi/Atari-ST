#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "language.h"

word is_netmail(word area)
{	if (area==gl_mailarea)
		return(TRUE);
	if (gl_area[area].flag & AR_NETMAIL)
		return(TRUE);
	return(FALSE);
}

word is_ptowner(byte *sysop,word area)
{	word i;
	if (opt.flag & O_FLAG_AKASYSOP)
	{
		for (i=0;i<=akasysopcount;i++)
		{	if (!stricmp(sysop,akasysop[i]))
				return(TRUE);
		}
		if (pseudonymsysopcount!=-1)
		{	for (i=0;i<=pseudonymsysopcount;i++)
			{	if ((area==pseudonymarea[i] || is_netmail(area)) && !stricmp(sysop,pseudonymsysop[i]))
					return(TRUE);
			}
		}
	}
	else
	{	if (!strnicmp(sysop,gl_ptowner,strlen(gl_ptowner)))
			return(TRUE);
	}
	return(FALSE);
}

word get_lastmsg(word arindex)
{	byte   tmp[MAX_STR];
	struct ffblk dta;
	long   hdrfile_len=0L;
	word   lastmsg=0;

	sprintf(tmp, "%s.%s", gl_area[arindex].path, HEADEREXT);
	if (!findfirst(tmp,&dta,0))
		hdrfile_len=dta.ff_fsize;
	lastmsg=(word) (hdrfile_len	/ sizeof(HDR));
	return (lastmsg);
}

void berechne_msgp(long win)
{	WIND *winp;
	int i;

	winp=&wi[win];
	i=1;
	while (i<MAX_MSG && winp->msgda[i]!=LEER)	i++;
	winp->lastmsg = i-1;

	if (winp->lastmsg==0)
		winp->firstmsg=0;
	else
		winp->firstmsg=1;
}

void msg_log(char *s)
{
	byte tmp[256];
	byte act_date[40];
	long sec_now;
	struct tm *tm_now;
	FILE *log_fp;
	static word first_fl=TRUE;
	
	log_fp=fopen(gl_logfile,"a");		/* war a+ */
	if (log_fp==NULL)
		return;
	if (first_fl)	
	{
		fputc(LF,log_fp);
		first_fl=FALSE;
	}	
	time(&sec_now);
	tm_now=localtime(&sec_now);
	strftime(act_date,30,"%d %b %H:%M:%S",tm_now);
	sprintf(tmp,"+ %s LED  %s\n",act_date,s);
	fputs(tmp,log_fp);
	fclose(log_fp);
}

void hole_msgp(long win)
{	WIND *winp;
	word i;
	HDR hdr;
	word maxhdr;
	uword delfl;
	char tmp[256];
	struct ffblk dta;
	long hdrlen=0L;

	winp=&wi[win];
	memset(wi[win].msgda,LEER,sizeof(wi[win].msgda));

	sprintf(tmp, "%s.%s", winp->areapath, HEADEREXT);
	if (!findfirst(tmp,&dta,0))
		hdrlen=dta.ff_fsize;
	else
		hdrlen=winp->hdrlen;
	maxhdr=(word) (hdrlen/sizeof(HDR));
	hdrseek(win,winp->hdrfp,0,SEEK_SET);
	if (winp->msgfilelen > 0)
	{
		i=1;
		while (hdrread(win,&hdr, 1,winp->hdrfp)==1 && i<=maxhdr)
		{
			if (hdr.lStart<0L || hdr.lStart>winp->msgfilelen || 
					hdr.Size<=0 ||
					hdr.lStart+(ulong)hdr.Size>winp->msgfilelen)
			{   
				if (hdr.lStart<0L || hdr.lStart>winp->msgfilelen) 
					sprintf(tmp,"corrupted Msg: Area <%s>, MsgNr <%d> start <%ld>: start of Msgtext invalid (<0 or >MSG-File)",winp->areaname,i,hdr.lStart);
				else if (hdr.Size<=0)
					sprintf(tmp,"corrupted Msg: Area <%s>, MsgNr <%d> size <%u>: size of Msgtext invalid (<=0)",winp->areaname,i,hdr.Size);
				else	
					sprintf(tmp,"corrupted Msg: Area <%s>, MsgNr <%d> start <%ld>, size<%u>: start+size of Msgtext invalid (>MSG-File)",winp->areaname,i,hdr.lStart,hdr.Size);
				msg_log(tmp);
/*			if (opt.protecthdr==TRUE) */
					break;
			}	
			if (strlen(hdr.sFrom)==0L)
			{	sprintf(tmp,"corrupted Msg: Area <%s>, MsgNr <%d>: From-field is empty",winp->areaname,i);
				msg_log(tmp);
/*			if (opt.protecthdr==TRUE) */
/*				break; */
			}
			delfl=(uword)(hdr.wFlags & F_DELETED);
			if (delfl!=F_DELETED)
				winp->msgda[i]=TRUE;
			else if (delfl==F_DELETED)
				winp->msgda[i]=FALSE;
			if ((++i)>MAX_MSG-1)
				break;
		}
		hdrseek(win,winp->hdrfp,0,SEEK_SET);
	}
	berechne_msgp(win);
}

word select_msg(long win, word msgnr, word redraw, word read_fl)
{	HDR *thdr;
	WIND *winp;
	word i;
	word ret;
	word oldmsg;
	byte err_str[200];

	winp=&wi[win];
	biene();

	if (msgnr<=0)					/* Wenn msgnr==0, dann lastread nehmen */
		msgnr=gl_area[winp->aktarea].lread;				/* ssl 181294 */

	gl_oldwin=win;	
	oldmsg=winp->oldmsg;
	winp->oldmsg=winp->aktmsg;

	if (winp->lastmsg==0)
		msgnr=0;
	else if (msgnr>winp->lastmsg)
	{	msgnr=srch_down(win,winp->lastmsg);
	}
	else if (msgnr<winp->firstmsg)
	{	msgnr=srch_up(win,winp->firstmsg);
	}
	else if (winp->msgda[msgnr]==FALSE)
	{	if (msgnr==winp->oldmsg && oldmsg>0)
		{	if (msgnr<oldmsg)
			{	msgnr--;
				if (msgnr<winp->firstmsg)
					msgnr=winp->firstmsg;
			}
			else
			{	msgnr++;
				if (msgnr>winp->lastmsg)
					msgnr=winp->lastmsg;
			}
		}
		if (msgnr >= winp->oldmsg)
		{	msgnr=srch_up(win,msgnr);
			if (winp->msgda[msgnr]==FALSE)
				msgnr=srch_down(win,msgnr);
		}
		else
		{	msgnr=srch_down(win,msgnr);
			if (winp->msgda[msgnr]==FALSE)
				msgnr=srch_up(win,msgnr);
		}
	}

	if (winp->msgda[msgnr]==TRUE)
		winp->aktmsg=msgnr;
	else
		winp->aktmsg=0;
	
	if (winp->aktmsg==0)
	{
		if (winp->hdr!=NULL)
			free(winp->hdr);
		winp->hdr=(HDR *)calloc(1,sizeof(HDR));
		winp->hdr->Size=0;
		winp->hdr->lStart=0L;
		
		for (i=0;i<MAX_LINES;i++)
			if (winp->lines[i])
				free(winp->lines[i]);
		memset(winp->lines,0,sizeof(winp->lines));
		winp->maxshow=
		winp->topline=
		winp->lzahl=
		winp->firstc=0;
		
		winp->belegt = TRUE;
		gl_area[winp->aktarea].lread=winp->aktmsg;
		if (winp->aktmsg == winp->lastmsg)
			gl_area[winp->aktarea].newmsg &= ~(UNRDMSG | TOSYSOP);
		if (redraw)
			wisetfiles(win,TRUE,NO_CHANGE);	/* ssl 150395 war ST_BLANK */
	}
	else if (winp->oldmsg!=winp->aktmsg ||
	        (oldmsg!=winp->oldmsg && oldmsg==0))
	{
		if (winp->hdr==NULL)
			winp->hdr=(HDR *)calloc(1,sizeof(HDR));
		if (winp->hdr==NULL)
			return(FALSE);
		hdrseek(win,winp->hdrfp,(winp->aktmsg-1),SEEK_SET);
		hdrread(win,winp->hdr, 1, winp->hdrfp);

		if (read_fl)
		{	winp->hdr->XFlags |= XF_READ;
			hdrseek(win,winp->hdrfp,(winp->aktmsg-1),SEEK_SET);
			hdrwrite(win,winp->hdr, 1, winp->hdrfp);
		}
		thdr=winp->hdr;
		thdr->sTo[35]=
		thdr->sFrom[35]=
		thdr->sSubject[71]=EOS;

		for (i=0;i<MAX_LINES;i++)
			if (winp->lines[i])
				free(winp->lines[i]);
		memset(winp->lines,0,sizeof(winp->lines));
		if (thdr->Size!=0)
		{
			if ((winp->msgbuf=(char *)calloc(1,(ulong)thdr->Size+2))!=NULL)
			{
				if (thdr->lStart<0L || thdr->lStart>winp->msgfilelen ||
				    thdr->Size<=0 || /* ssl 71294 */
				    thdr->lStart+(ulong)thdr->Size > winp->msgfilelen)
				{
					sprintf(err_str,msgtxt[MSG_HDRCRASH].text,
										        thdr->lStart,thdr->Size,winp->msgfilelen);
					FormAlert(msgtxt[MSG_HDRCRASH].defbut,
										msgtxt[MSG_HDRCRASH].icon,
										err_str,
										msgtxt[MSG_HDRCRASH].button);
				}
				else
				{
					fseek(winp->msgfp,thdr->lStart,SEEK_SET);
					if (fread(winp->msgbuf,(ulong)thdr->Size,1L,winp->msgfp)!=1L)
						LangAlert(MSG_READTEXTFAILED);
					
					buf_to_liste(win);
					
					winp->firstc	=
					winp->maxshow	=
					winp->topline	= 0;
					winp->belegt = TRUE;
					gl_area[winp->aktarea].lread=winp->aktmsg;
					if (winp->aktmsg == winp->lastmsg)
						gl_area[winp->aktarea].newmsg &= ~UNRDMSG;

					if (is_ptowner(thdr->sTo,winp->aktarea) ||
/*				if (strnicmp(thdr->sTo,gl_ptowner,strlen(gl_ptowner))==0 */
					    stricmp(thdr->sTo,"sysop")==0)
						gl_area[winp->aktarea].newmsg &= ~TOSYSOP;

					if (read_fl)
						if (is_ptowner(thdr->sTo,winp->aktarea))
						{	thdr->wFlags|=F_RECEIVED;
							hdrseek(win,winp->hdrfp,(winp->aktmsg-1),SEEK_SET);
							hdrwrite(win, winp->hdr, 1, winp->hdrfp);
						}
					
					if (redraw)
					{
						if (is_netmail(winp->aktarea))
						{	/* ssl 181294 ###ADDR### */
							if (!addr[0].point && !(thdr->wFlags & F_RECEIVED) && 
							    thdr->wToPoint==0 && 
							    thdr->wToNode ==addr[0].node && 
							    thdr->wToNet  ==addr[0].net && 
							    thdr->wToZone ==addr[0].zone)
							{	if (!stricmp(thdr->sTo,gl_ptowner))
								{	thdr->wFlags|=F_RECEIVED;
									hdrseek(win,winp->hdrfp,(winp->aktmsg-1),SEEK_SET);
									hdrwrite(win, winp->hdr, 1, winp->hdrfp);
								}
							}
						}
						wisetfiles(win,TRUE,NO_CHANGE);
					}

					if (opt.winprm[MSGSWIND][6]==1)			/* ssl 141294 */
					{	if (!wi[MSGSWIND].belegt)
							open_msglistwindow(win);
					}
				}
			}
			else
			{
				nomemalert(2);
				mouse_normal();
				if (redraw)
					wisetfiles(win,TRUE,NO_CHANGE);
				return(NO_CHANGE);
			}
		}
	}
	if (redraw)
		set_comstatus(win,winp->aktmsg);
	ret=chk_status(win,redraw);
	mouse_normal();
	return ret;
}

word srch_up(long win, word nr)
{	WIND *winp;
	word i;

	winp=&wi[win];
	for (i=nr;i<winp->lastmsg;i++)
		if (winp->msgda[i]==TRUE)
			break;
	return(i);
}

word srch_down(long win, word nr)
{	WIND *winp;
	word i;

	winp=&wi[win];
	for (i=nr;i>winp->firstmsg;i--)
		if (winp->msgda[i]==TRUE)
			break;
	return(i);
}

/* --------------------------------- */

word open_area(long win, word areanr)
{	char tmp[MAX_STR];
	struct ffblk dta;

	biene();
	if (areanr < 0 || areanr > gl_arzahl)
		areanr = gl_mailarea;
	
	sprintf(tmp, "%s.MSG", gl_area[areanr].path);
	
	if ((wi[win].msgfp=fopen(tmp, "r+b"))==NULL)
		wi[win].msgfp=fopen(tmp, "w+b");
	if (!findfirst(tmp,&dta,0))
		wi[win].msgfilelen=dta.ff_fsize;

	sprintf(tmp, "%s.%s", gl_area[areanr].path, HEADEREXT);
	
	if (wi[win].msgfilelen == 0)
	   	wi[win].hdrfp=fopen(tmp, "w+b"); /* wenn msg 0 wird hdr verworfen */
	else
	{
		if ((wi[win].hdrfp=fopen(tmp, "r+b"))==NULL)
		   	wi[win].hdrfp=fopen(tmp, "w+b");
	}

	/* [*] */

	if (wi[win].msgfp!=NULL && wi[win].hdrfp !=NULL)
	{
		init_hdrcache(win,tmp);	/* stand vorher bei [*] */

		wi[win].aktarea = areanr;
		strcpy(wi[win].areaname,gl_area[areanr].name);
		strcpy(wi[win].areapath,gl_area[areanr].path);
		gl_area[areanr].newmsg &= ~NEWMSG;					/* NewMsgFlag l”schen */
		hole_msgp(win);
		wi[win].aktmsg = gl_area[areanr].lread;
		wi[win].oldmsg = 0;
		mouse_normal();

		if (wi[MSGSWIND].belegt)				/* ssl 161294 */
		{	opt.winprm[MSGSWIND][6]=1;		/* Window wieder ”ffnen!!! */
			close_window(MSGSWIND);
		}

		mouse_normal();
		return TRUE;
	}
	else
	{
		if (wi[MSGSWIND].belegt)				/* ssl 161294 */
			close_window(MSGSWIND);
		LangAlert(MSG_INVALIDAREA);
		if (wi[win].msgfp!=NULL)	fclose(wi[win].msgfp);
		if (wi[win].hdrfp!=NULL)	fclose(wi[win].hdrfp);
		mouse_normal();
		return FALSE;
	}	   	   
}

void close_area(long win)
{
	/* In "hdrfp2" wird ein Zeiger auf die Filestruktur des aktuellen
		 MsgWindow gesichert. Dieser darf beim Msglistwindow nicht
		 gel”scht werden.
	*/
		 
	if (win!=MSGSWIND		/*||wi[win].listwind!=WI_LIST_MSGS*/ )
		wi[win].hdrfp2=NULL;			/* ssl 151294: Nur auf NULL setzen!!! */

	if (wi[win].msgfp!=NULL)
	{	fclose(wi[win].msgfp);
		wi[win].msgfp=NULL;
	}

	if (wi[win].hdrfp!=NULL)
	{	fclose(wi[win].hdrfp);
		wi[win].hdrfp=NULL;
	}

	if (wi[win].msgbuf!=NULL)
	{	free(wi[win].msgbuf);
		wi[win].msgbuf=NULL;
	}
	
	free_allmsglines(win);

	if (wi[win].ramhdr)                      
	{	free(wi[win].hdrbuf);
		wi[win].hdrbuf=NULL;
		wi[win].hdrptr=NULL;
		wi[win].ramhdr=FALSE;
		wi[win].hdrlen=0L;
	}
	
	if (opt.savelread && !wi[win].listwind) /* ssl 81294 */
		save_lread(wi[win].aktarea);
}

void change_area(long win, word newarea)
{	biene();
	close_area(win);

	wi[win].aktarea=newarea;
	if (open_area(win, wi[win].aktarea))
	{
		wind_set(wi[win].handle,WF_NAME,(long)wi[win].areaname,0,0);
/*		new_status(win,select_msg(win,wi[win].aktmsg,TRUE));*/
		select_msg(win,wi[win].aktmsg,TRUE,TRUE);
	}

	update_areawindow(newarea);
	mouse_normal();
}

void open_allarea(word area)
{	long laufw;
	
	for (laufw=0;laufw<MAX_WIND;laufw++)
	{
		if (wi[laufw].belegt)
		{
			if (wi[laufw].aktarea==area && !wi[laufw].editierbar)
/*			if (wi[laufw].aktarea==area)*/
			if (!wi[laufw].listwind)		/* ssl 91294 */
			{
				if (open_area(laufw,area))
					select_msg(laufw,wi[laufw].aktmsg,TRUE,TRUE);
				else
					close_window(laufw);
			}
		}
	}
}

void close_allarea(word area)
{	long laufw;
	
	for (laufw=0;laufw<MAX_WIND;laufw++)
	{
		if (wi[laufw].belegt)
		if (!wi[laufw].listwind)		/* ssl 91294 */
		{
			if (wi[laufw].aktarea==area && !wi[laufw].editierbar)
			{
				close_area(laufw);
			}
		}
	}
}

void killnrmsg(long ed_win, word msgnr)
{
	HDR thdr;
	long win;
	char tmp[MAX_STR];
	FILE *hdr_fp;

	win=area_msgwind(wi[ed_win].aktarea);
	if (win<0L)
	{
	    /* Kein Msg-Fenster der Original-Area offen */
	    /* HDR-File selber ”ffnen und schreiben */
		sprintf(tmp, "%s.%s", gl_area[wi[ed_win].aktarea].path, HEADEREXT);
	
		if ((hdr_fp=fopen(tmp, "r+b"))==NULL)
		   	hdr_fp=fopen(tmp, "w+b");
		if (hdr_fp==NULL)
			return;
		biene();
		show_doing(DO_DELORIGINAL);
		fseek(hdr_fp, (long)((msgnr-1)*sizeof(HDR)), SEEK_SET);
		fread(&thdr, sizeof(HDR), 1, hdr_fp);
		thdr.wFlags |= F_DELETED;
		fseek(hdr_fp, (long)((msgnr-1)*sizeof(HDR)), SEEK_SET);
		fwrite(&thdr, sizeof(HDR), 1, hdr_fp);
		fclose(hdr_fp);
		del_doing();
		mouse_normal();
	}
	else
	{
		biene();
		show_doing(DO_DELORIGINAL);
		hdrseek(win,wi[win].hdrfp, (msgnr-1), SEEK_SET);
		hdrread(win,&thdr, 1, wi[win].hdrfp);
		thdr.wFlags |= F_DELETED;
		wi[win].msgda[msgnr]=FALSE;
		hdrseek(win,wi[win].hdrfp, (msgnr-1), SEEK_SET);
		hdrwrite(win,&thdr, 1, wi[win].hdrfp);
		del_doing();
		mouse_normal();
	}
	wi[win].aktmsg=msgnr;
	need_update_msglistwindow(win,RETURN);
}
