#include "ccport.h"
#include <time.h>
#include "menu.rh"
#include "carbon.rh"
#include "nodenum.rh"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "winddefs.h"
#include "vars.h"
#include "language.h"

#define DEBUG FALSE

extern byte old_msgtime[];
extern byte loc_address[];

byte cc_lines[MAX_CC+2][40];
byte cc_node[MAX_CC+2][25];
byte cc_nof[MAX_CC+2][2];

word ccto_obj[10] = {CCTO1,CCTO2,CCTO3,CCTO4,CCTO5,CCTO6,CCTO7,
                     CCTO8,CCTO9,CCTO10};
word ccnode_obj[10] = {CCNODE1,CCNODE2,CCNODE3,CCNODE4,CCNODE5,
                       CCNODE6,CCNODE7,CCNODE8,CCNODE9,CCNODE10};                     
word ccnof_obj[10] = {CCNF1,CCNF2,CCNF3,CCNF4,CCNF5,CCNF6,CCNF7,
                      CCNF8,CCNF9,CCNF10};
word ccn_obj[10] = {CCN1,CCN2,CCN3,CCN4,CCN5,CCN6,CCN7,
                    CCN8,CCN9,CCN10};

void free_allmsglines(long win)
{	word i;
	for (i=0;i<MAX_LINES;i++)
		if (wi[win].lines[i]!=NULL)
			free(wi[win].lines[i]);
	memset(wi[win].lines,0,sizeof(wi[win].lines));
}

void prep_topline(long win, word titel)
{	if (wi[win].topline>0)
	{	wi[win].topline=0;
		sys_wisetfiles(win,FALSE);
	}
	set_menu(EDIT_WIND);
	menu_tnormal(menu_tree,titel,TRUE);
}

void edit_msg(long win, word newmsg)
{	word redraw;
	RECHTECK curr;

	gl_options |= EDITWINDOW_USED;

	if (newmsg)
	{	clear_hdr(win);
		if (wi[win].hdr==NULL)
		{	nomemalert(1);
			return;
		}	

		set_orignode(wi[win].hdr,wi[win].aktarea);
		strcpy(wi[win].hdr->sFrom,gl_ptowner);
		strcpy(wi[win].hdr->sDate,ifna_date());
		time(&wi[win].hdr->lDate);

		store_msghdr(win);
		
		free_allmsglines(win);
		
		wi[win].lzahl=0;
		wi[win].lnr=0;
		wi[win].cx =HDRCOL;
		wi[win].cy =TO_LINE;
		wi[win].pos=wi[win].edline=wi[win].hdr->sTo;
		
		if (!is_netmail(wi[win].aktarea))
		{	wi[win].hdr->wToZone=gl_area[wi[win].aktarea].zone;
			wi[win].hdr->wToNet=gl_area[wi[win].aktarea].net;
			wi[win].hdr->wToNode=gl_area[wi[win].aktarea].node;
			wi[win].hdr->wToPoint=0;
		}		
	}
	wi[win].newmsg=FALSE /*newmsg*/;
	
	wi[win].lastmsg++;
	wi[win].oldmsg=wi[win].aktmsg;
	wi[win].aktmsg=wi[win].lastmsg;
	
	if ((word)opt.indent>=0 && (word)opt.indent<255)
		wi[win].indent=0;
	else
		wi[win].indent=-1;
	wi[win].insert=opt.insert;

	wi[win].firstc=wi[win].firstl=0;
	wi[win].oldcx=MSGCOL;
	wi[win].cursor=FALSE;
	s_cutbuff=e_cutbuff=-1;
	
	wi[win].belegt = TRUE;
	wi[win].editierbar=TRUE;
	wi[win].maxshow =
	wi[win].topline = 0;
	
	wi[win].hdr->wFlags |= F_LOCAL;
	wi[win].hdr->XFlags |= XF_READ;
	if (gl_area[wi[win].aktarea].flag & AR_FORCEPVT)
		wi[win].hdr->wFlags |= F_PRIVATE;

	set_if_fixed_address(win,wi[win].aktarea);

	if (wi[win].aktarea != wi[win].oldarea)
		wind_set(wi[win].handle,WF_NAME,(long)wi[win].areaname,0,0);
	
	redraw=TRUE;
	wind_get(wi[win].handle,WF_CURRXYWH,
	         &wi[win].px,&wi[win].py,&wi[win].pw,&wi[win].ph);
	if (!opt.sredraw)
	{
		curr.w=full.w;
		curr.h=full.h;
		get_maxwindcol(win,opt.maxedcol,&curr.w,&curr.h);
		if (wi[win].pw!=curr.w || wi[win].ph!=curr.h)
		{	
			wind_set(wi[win].handle,WF_CURRXYWH,wi[win].px,wi[win].py,curr.w,curr.h);
			if (wi[win].pw<curr.w || wi[win].ph<curr.h)
				redraw=FALSE;
#if 1
			wi[win].pw=curr.w;	/* ssl 110295 */
			wi[win].ph=curr.h;
#endif
		}
	}
	else
		redraw=FALSE;

	redraw=FALSE;				/* Smart redraw praktisch deaktiviert!*/
	
	set_wiclip(win);
	set_wifont(win);
	
	set_menu(EDIT_WIND);
	if (is_netmail(wi[win].aktarea))
	{
		wi[win].hdr->wFlags |= F_PRIVATE;
		menu_ienable(menu_tree,ECC,TRUE);
		init_carbon();
	}
	if (redraw)
		wisetfiles(win,TRUE,NO_CHANGE); 

	upded_stat(win,0);

	hide_mouse();
	cursor(win,TRUE);
	show_mouse();
}

static word is_quote(long win,word lnr)
{	byte *cp,*line;
	if (lnr>=0)
	{	line=wi[win].lines[lnr];
		if (line)
		{	cp=strchr(line,'>');
			if (cp && cp-line<6)
				return TRUE;
		}
	}
	return FALSE;
}

void edit_end(long win, word e_save)
{	word i;

	set_wiclip(win);
	set_wifont(win);

	cursor(win,FALSE);
	s_cutbuff=e_cutbuff=-1;
	
	if (wi[win].cy==TO_LINE || wi[win].cy==TOPIC_LINE)
		e_save=FALSE;
		
	i=wi[win].lzahl-1;
	if (e_save && is_quote(win,i) && is_quote(win,i-1))
	{	i=LangAlert(MSG_QUOTESTUFF);
		if (i==2)
		{	i=wi[win].lzahl-1;
			while (is_quote(win,i))
			{	del_line(win,i,FALSE);
				i--;
			}
			set_cxpos(win);
			hide_mouse();
			pline(win, TOPMSG_LINE, wi[win].bzeilen, FALSE, FALSE);
			show_mouse();
		}
	}

	if (wi[win].indent>=0)	opt.indent=0;
	else										opt.indent=-1;
	opt.insert=wi[win].insert;

	if (e_save)
	{	gl_area[wi[win].aktarea].flag |= AR_EDITED;
		
		if (is_netmail(wi[win].aktarea))   /* Carbon Copys */
		{	for (i=0;i<MAX_CC;i++)
				if (strlen(cc_lines[i])>1 || *cc_lines[i]!='-')
					break;
			if (i<MAX_CC)
			{	/* wi[win].hdr->flags&=~FILEATCH;	*/
				save_carbon(win);
			}
			else
				save_newmsg(win,NULL,TRUE);
		}
		else
			save_newmsg(win,NULL,TRUE);
			
		if (wi[win].killchange!=FALSE)
		{	killnrmsg(win,wi[win].killchange);
			need_update_msglistwindow(win,RETURN);
		}
	}
	else
		wi[win].killchange=FALSE;
	leave_edit(win);
}

void leave_edit(long win)
{	word select_fl=FALSE;
	long new_win=-1;

	mouse_normal();	
	if (wi[win].killchange!=FALSE)
	{	select_fl=TRUE;	
		new_win=area_msgwind(wi[win].aktarea);
	}	
	wi[win].editierbar=FALSE;

	set_wiclip(win);
	set_wifont(win);
	set_menu(MSG_WIND);		               

	if (select_fl)
		if (new_win>=0L)
		{	select_msg(new_win,wi[new_win].lastmsg,TRUE,TRUE);
			need_update_msglistwindow(new_win,RETURN);
		}

	close_window(win);
}

void store_msgid(long win)
{	byte *cp;
	strcpy(wi[win].dup_replyid,"\001REPLY:");
	cp=strchr(wi[win].dup_msgid,' ');
	if (cp!=NULL)
		strcat(wi[win].dup_replyid,cp);
	else
		*(wi[win].dup_replyid)=EOS;
}




void test_replyto(long win)
{	char *p;
	if (is_netmail(wi[win].aktarea))
	{	if (!strncmp(wi[win].dup_replyto,"\001REPLYTO",8))
		{	p=strchr(wi[win].dup_replyto,' ');
			if (p)
			{	while (*p!=EOS && *p==' ') p++;
				scanaddr(p, &wi[win].hdr->wToZone,
										&wi[win].hdr->wToNet,
										&wi[win].hdr->wToNode,
										&wi[win].hdr->wToPoint,NULL);
				p=strchr(p+1,' ');
				if (p)
				{	while (*p!=EOS && *p==' ') p++;
					strcpy(wi[win].hdr->sTo,p);
					p=strchr(wi[win].hdr->sTo,EOS);
					while(p>wi[win].hdr->sTo && iscntrl(*p)) *p--=EOS;
				}
			}
		}
	}
}

void reply_msg(long win,word arindex)
{	word lneu;
	byte tmp[MAX_STR];
	
	store_msgid(win);
	
	wi[win].hdr->wToZone=wi[win].hdr->wFromZone;
	wi[win].hdr->wToNet=wi[win].hdr->wFromNet;
	wi[win].hdr->wToNode=wi[win].hdr->wFromNode;
	wi[win].hdr->wToPoint=wi[win].hdr->wFromPoint;

	del_mailerfields(win);

	set_orignode(wi[win].hdr,wi[win].aktarea);
	del_allhflags(wi[win].hdr);
	
	strcpy(wi[win].hdr->sTo,wi[win].hdr->sFrom);
	strcpy(wi[win].hdr->sFrom,gl_ptowner);
	strcpy(wi[win].hdr->sDate,ifna_date());
	time(&wi[win].hdr->lDate);

	add_subjre(win);

	wi[win].oldarea=wi[win].aktarea;								/* 280496 */
	gl_area[wi[win].aktarea].lread=wi[win].aktmsg;	/* 280496 */
	wi[win].aktarea=arindex;												/* 280496 */


	if (opt.usereplyto)
		test_replyto(win);

	if (wi[win].lines[0]!=NULL)
		strcpy(tmp,wi[win].lines[0]);
	else
		*tmp=EOS;

	free_allmsglines(win);
	
	wi[win].lines[0]=(char *)calloc(1,LINELEN);
	if (wi[win].lines[0]==NULL)
	{	nomemalert(0);

		wi[win].aktarea=wi[win].oldarea;		/* 280496 */

		leave_edit(win);
	}
	else
	{
		wi[win].lzahl=1;

		lneu=0;

		if (!strncmp(tmp,"\003AREA ",6))
			if (ins_line(win,lneu,tmp,FALSE,FALSE)==TRUE)
				lneu++;

		lneu=add_uucp_to_line(win,lneu);

		wi[win].lnr=0;
		wi[win].cy=TOPMSG_LINE;

		if (opt.quote & O_QUOTE_MSET)
		{	wi[win].lnr=lneu;
			wi[win].cy=TOPMSG_LINE+lneu;
		}

		wi[win].oldcx=MSGCOL;
		set_cxpos(win);

		wi[win].aktmsg=gl_area[arindex].lread+1;					/* 280496 */
		strcpy(wi[win].areaname,gl_area[arindex].name);		/* 280496 */
		strcpy(wi[win].areapath,gl_area[arindex].path);		/* 280496 */
		wi[win].oldmsg = 0;																/* 280496 */

		edit_msg(win,FALSE);
	}
}

void quote_msg(long win)
{	byte q_name[16];
	
	biene();
	store_msgid(win);
	store_msghdr(win);
	
	wi[win].hdr->wToZone=wi[win].hdr->wFromZone;
	wi[win].hdr->wToNet=wi[win].hdr->wFromNet;
	wi[win].hdr->wToNode=wi[win].hdr->wFromNode;
	wi[win].hdr->wToPoint=wi[win].hdr->wFromPoint;
	del_mailerfields(win);
	
	set_orignode(wi[win].hdr,wi[win].aktarea);
	del_allhflags(wi[win].hdr);
	
	strcpy(wi[win].hdr->sTo,wi[win].hdr->sFrom);
	strcpy(wi[win].hdr->sFrom,gl_ptowner);
	strcpy(old_msgtime,wi[win].hdr->sDate);
	strcpy(wi[win].hdr->sDate,ifna_date());
	time(&wi[win].hdr->lDate);
	
	get_qname(win, q_name);

	if (opt.usereplyto)
		test_replyto(win);

	add_subjre(win);

	wi[win].cy=TOPMSG_LINE;
	wi[win].topline=0;
	wi[win].lnr=0;

	if (quote_wrap(win, q_name, wi[win].aktarea, FALSE))
	{	
		wi[win].oldcx=MSGCOL;

		set_cxpos(win);
		mouse_normal();
		wi[win].oldarea=wi[win].aktarea;
		edit_msg(win,FALSE);
	}
	else
		leave_edit(win);
}

void quotearea_msg(long win, word arindex)
{	HDR hdr_bak;
	byte q_name[20];

	if (wi[win].aktarea==arindex)
	{	quote_msg(win);
		return;
	}

	biene();
	store_msgid(win);
	store_msghdr(win);
	
	wi[win].hdr->wToZone=0;
	wi[win].hdr->wToNet=0;
	wi[win].hdr->wToNode=0;
	wi[win].hdr->wToPoint=0;

	scanaddr(loc_address,	&wi[win].hdr->wToZone,
												&wi[win].hdr->wToNet,
												&wi[win].hdr->wToNode,
												&wi[win].hdr->wToPoint,NULL);
	
	del_mailerfields(win);

	set_orignode(wi[win].hdr,wi[win].aktarea);
	del_allhflags(wi[win].hdr);
	
	strcpy(wi[win].hdr->sTo,wi[win].hdr->sFrom);
	strcpy(wi[win].hdr->sFrom,gl_ptowner);
	strcpy(old_msgtime,wi[win].hdr->sDate);
	strcpy(wi[win].hdr->sDate,ifna_date());
	time(&wi[win].hdr->lDate);
	
	add_subjre(win);
	
	wi[win].oldarea=wi[win].aktarea;
	gl_area[wi[win].aktarea].lread=wi[win].aktmsg;
	
	memcpy(&hdr_bak,&wi[win].hdr,sizeof(HDR));

	wi[win].lastmsg=get_lastmsg(arindex);
	wi[win].cy=TOPMSG_LINE;
	wi[win].topline=0;
	wi[win].lnr=0;
	
	get_qname(win, q_name);
	
	wi[win].aktarea=arindex;		/* ssl 200495 */

	if (quote_wrap(win, q_name, wi[win].oldarea, TRUE))
	{
		wi[win].oldcx=MSGCOL;

		set_cxpos(win);
		mouse_normal();
	    
    wi[win].msgfp=NULL;
    wi[win].hdrfp=NULL;

/*	wi[win].aktarea=arindex; */

		if (opt.usereplyto)
			test_replyto(win);		 /* ssl 291294: Erst hier erlaubt!!! */

		wi[win].aktmsg=gl_area[arindex].lread+1;
		strcpy(wi[win].areaname,gl_area[arindex].name);
		strcpy(wi[win].areapath,gl_area[arindex].path);
		wi[win].oldmsg = 0;
		
		edit_msg(win,FALSE);
	}
	else
	{
		wi[win].aktarea=wi[win].oldarea;		/* ssl 200495 */
		leave_edit(win);
	}
}



void change_msg(long win)
{	word i;
	word ret=TRUE,local=FALSE;
/* byte *msgbuf[MAX_LINES]; */
	byte **msgbuf;
	byte *cp,tmp[1024];
	FILE *fp;

	msgbuf=(byte **)calloc(MAX_LINES,sizeof(byte *));
	if (!msgbuf)
	{	LangAlert(MSG_OUTOFMEMORY);
		return;
	}

	if (wi[win].lines[0]!=NULL)
	{	if (wi[win].hdr->wFlags & F_LOCAL)
			local=TRUE;

		if (is_ptowner(wi[win].hdr->sFrom,wi[win].aktarea))
		{	wi[win].hdr->wFlags &= ~(F_SENT|F_RECEIVED);
			wi[win].hdr->XFlags &= ~(XF_READ|XF_FIXEDADDRESS);
		}
		else
		{	del_allhflags(wi[win].hdr);
			wi[win].hdr->XFlags =
			wi[win].hdr->MFlags = 0;
		}

		wi[win].hdr->lMsgidcrc=
		wi[win].hdr->lReplycrc=0;
#if !defined(__NEWFIDO__)
		wi[win].hdr->free6=
		wi[win].hdr->wProcessed=0;
#endif

		strcpy(wi[win].hdr->sDate,ifna_date());
		time(&wi[win].hdr->lDate);

		strcpy(wi[win].hdr->sFrom,gl_ptowner);
		set_orignode(wi[win].hdr,wi[win].aktarea);

		store_msghdr(win);
		
		memset(msgbuf,0,MAX_LINES*sizeof(byte *));

/*	for (i=0;i<MAX_LINES;msgbuf[i++]=NULL); */
/*	i=0;
		while (wi[win].lines[i]!=NULL)
		{	msgbuf[i]=wi[win].lines[i];
			i++;
		}
		for (i=0;i<MAX_LINES;wi[win].lines[i++]=NULL); */

		memcpy(msgbuf,wi[win].lines,sizeof(wi[win].lines));
		
		i=0;
		while (msgbuf[i]!=NULL && ret)
		{	cp=strchr(msgbuf[i],'>');
			if (cp!=NULL && cp-msgbuf[i]<6)
				ret=store_line(win,i,NULL,msgbuf[i],FALSE,TRUE);
			else
				ret=store_line(win,i,NULL,msgbuf[i],FALSE,FALSE);
			i++;
		}

/*
**	Delete RandomTearline
*/

		*tmp=EOS;
		if (local)
		{	if (!(gl_area[wi[win].aktarea].flag & AR_NORANDOMTEARLINE))
			{	if (gl_area[wi[win].aktarea].rndfootpath)
					strcpy(tmp,gl_area[wi[win].aktarea].rndfootpath);
				else if ((opt.flag & O_FLAG_RANDOMTEAR) ||
								(gl_area[wi[win].aktarea].flag & AR_RANDOMTEARLINE))
					sprintf(tmp,"%sLED.FLN",gl_ledpath);
			}
			if (*tmp)
			{	fp=fopen(tmp,"r");
				if (fp)
				{	fgets(tmp,MAX_STR,fp);
					cp=strchr(tmp,EOS);
					while (cp>tmp && *(cp-1)!=' ' && isspace(*(cp-1)))	cp--;
					*cp=EOS;
					fclose(fp);
				}
				if (*tmp)
				{	convert_percent(tmp,wi[win].aktarea,TRUE);
					cp=strchr(tmp,'|');
					if (cp)
					{	*cp=EOS;
						cut_endspc(tmp);
					}
					if (strlen(tmp)>=2)
					{	i=wi[win].lzahl-1;
						while (i>0 && i>wi[win].lzahl-8)
						{	if (!strncmp(wi[win].lines[i],tmp,strlen(tmp)))
							{	while (wi[win].lines[i]!=NULL &&
											 *wi[win].lines[i]!=EOS &&
											 *wi[win].lines[i]!='\n')
								{	/* del_line(win,i,FALSE); */
									free(wi[win].lines[i]);
#if 0
									while (wi[win].lines[j+1]!=NULL)
									{	wi[win].lines[j]=wi[win].lines[j+1];
										j++;
									}
									wi[win].lines[j]=NULL;
#else
									if (i<wi[win].lzahl-1)
										memcpy(&wi[win].lines[i],&wi[win].lines[i+1],(wi[win].lzahl-i-1)*sizeof(byte *));
									wi[win].lines[wi[win].lzahl-1]=NULL;
#endif
									wi[win].lzahl--;
								}
								break;
							}
							i--;
						}
					}
				}
			}
		}


		i=0;
		while (i<MAX_LINES && wi[win].lines[i]!=NULL)
			i++;
		wi[win].lzahl=i;
		wi[win].lnr=0;
		wi[win].oldcx=MSGCOL;
		set_cxpos(win);
		while (wi[win].lines[wi[win].lnr] != NULL)
		{	cp=strchr(wi[win].lines[wi[win].lnr],0);
			if (*(cp-1)!='\n' && *(cp-1)!=' ')
			{	*cp++=' ';
				*cp=EOS;
			}
			if (strlen(wi[win].lines[wi[win].lnr]) > opt.maxedcol)
				wrap_fwd(win,FALSE);
			wi[win].pos=wi[win].edline=wi[win].lines[++wi[win].lnr];
		}
		wi[win].lnr=0;
		wi[win].cy=TOPMSG_LINE;
		wi[win].oldcx=MSGCOL;
		set_cxpos(win);
		wi[win].killchange=wi[win].aktmsg;
		wi[win].oldarea=wi[win].aktarea;
		edit_msg(win,FALSE);
	}
	free(msgbuf);
}

word save_newmsg(long win, byte *cc_orig, word openarea)
{	word i;
	word dest_area;
	uword tzone, tnet, tnode, tpoint;
	byte tmp[MAX_STR];
	FILE *hdr_fp, *msg_fp;
	struct ffblk dta;
	long msgfilelen;
	ulong ltemp;
	
	if (wi[win].lines[0]==NULL)
		return(FALSE);

	if (!wi[win].newmsg)		/* Quote,Reply,Change etc. */
	{	if ( (opt.addrcornet  																&&  is_netmail(wi[win].aktarea)) ||
				 (gl_area[wi[win].aktarea].flag & AR_FIXEDADDRESS && !is_netmail(wi[win].aktarea)) )
		{	wi[win].hdr->free6=
			passender_absender(wi[win].hdr->sTo,
												 wi[win].hdr->wToZone,
												 wi[win].hdr->wToNet,
												 wi[win].hdr->wToNode,
												 wi[win].hdr->wToPoint,
												 gl_area[wi[win].aktarea].domain,
												 &wi[win].hdr->wFromZone,
												 &wi[win].hdr->wFromNet,
												 &wi[win].hdr->wFromNode,
												 &wi[win].hdr->wFromPoint,
												 NULL);
			phdrfromline(win);				/* Zeige From-Adresse an */
		}
	}
	wi[win].newmsg=0;
	dest_area=wi[win].aktarea;
	
	biene();
	show_doing(DO_SAVEMSG);
	close_allarea(dest_area);
	
	sprintf(tmp, "%s.MSG", gl_area[dest_area].path);

	if ((msg_fp=fopen(tmp, "ab"))==NULL)
	{	LangAlert(MSG_NOACCESSMSG);
		return(FALSE);
	}

	if (!findfirst(tmp,&dta,0))
		msgfilelen=dta.ff_fsize;

	sprintf(tmp, "%s.%s", gl_area[dest_area].path, HEADEREXT);
	
	if (msgfilelen == 0L)
	{	if ((hdr_fp=fopen(tmp, "wb"))==NULL) /* war w+b, wenn msg 0 wird hdr verworfen */
		{	fclose(msg_fp);
			LangAlert(MSG_NOACCESSHDR);
			return(FALSE);
		}
	}
	else
	{	if ((hdr_fp=fopen(tmp, "ab"))==NULL)
		{	fclose(msg_fp);
			LangAlert(MSG_NOACCESSHDR);
			return(FALSE);
		}
	}

	if (msg_fp!=NULL && hdr_fp!=NULL)
	{
		fseek(hdr_fp, 0L, SEEK_END);
		fseek(msg_fp, 0L, SEEK_END);
	
/*		set_orignode(wi[win].hdr,wi[win].aktarea);  haben wir das nicht schon getan? */
		
		wi[win].hdr->lStart=ftell(msg_fp);
		
		/* save all lines */
		put_msgid(win,wi[win].hdr,msg_fp);
		
		if (is_netmail(wi[win].aktarea))    /* Netmail-Area */
		{	if (cc_orig!=NULL)
			{	fputs(cc_orig,msg_fp);
				fputc('\n',msg_fp);
				for (i=0;i<MAX_CC;i++)
				{	if (strlen(cc_lines[i])>0 && *cc_lines[i]!='-')
					{	nnstr2node(cc_node[i],(uword *)&tzone,(uword *)&tnet,
														      (uword *)&tnode,(uword *)&tpoint);

						sprintf(tmp,msgtxt[DIV_CCON].text,cc_lines[i],
														mergeaddr(tzone,tnet,tnode,tpoint,NULL));
						fputs(tmp,msg_fp);
						fputc('\n',msg_fp);
					}
				}
				fputc('\n',msg_fp);	
			}
		}
		
		del_blanklines(win, TRUE);
		save_lines(win, msg_fp, TRUE, wi[win].aktarea);
		
		ltemp = ftell(msg_fp) - wi[win].hdr->lStart;
		if (ltemp>=65534ul)
		{	ltemp=65534ul;
			LangAlert(MSG_MSGTOOBIG);
		}
		wi[win].hdr->Size= (uword)ltemp;

		if (!is_netmail(wi[win].aktarea))
			wi[win].hdr->wFlags &= ~F_FILEATTACH;
		
		fclose(msg_fp);
		msg_fp=NULL;

		if (fwrite(wi[win].hdr, sizeof(HDR), 1, hdr_fp)!=1)
		{	LangAlert(MSG_ERRORWRITEHDR);
			fclose(hdr_fp);
			return(FALSE);
		}
		del_doing();
		mouse_normal();
	}
	else
		LangAlert(MSG_INVALIDMSGAREA);

	if (hdr_fp) fclose(hdr_fp);
	if (msg_fp) fclose(msg_fp);
	
	if (openarea)
		open_allarea(dest_area);
	return(TRUE);
}

void del_blanklines(long win, word memlines)
{	word i;
	byte *cp;
	
	i=wi[win].lzahl;
	while (i>1)
	{	cp=wi[win].lines[i-1];
		if (cp!=NULL)
		{	
			if (*cp=='\01' && *(cp+1)!='P')
				*cp=EOS;
			while (*cp==' ' || *cp=='\n')
				cp++;
			if (*cp==EOS)
			{	if (memlines)
					free(wi[win].lines[i-1]);
				wi[win].lines[i-1]=NULL;
				wi[win].lzahl--;
			}	
			else
				break;
		}
		i--;
	}
	ins_line(win,wi[win].lzahl,"\n",FALSE,FALSE);
}

void put_pid(FILE *fp)
{	byte tmp[80];
	test_registered();
	sprintf(tmp,"\001PID: LED %s%s\n",
														pidversion,
														is_registered ? pidserial : unreg);
	fputs(tmp,fp);	/* Meine eigene PID schreiben! */
}

/*
** 1 - "CHRS IBMPC 2" wurde geschrieben
*/

static ulong calc_xid(byte *d)
{	static uword count;
	byte ds[MAX_STR];
	ulong t,ret;
	ulong day,mon,year,hour,min,sec;
	strcpy(ds,d);
	ds[2]=
	ds[6]=
	ds[9]=
	ds[13]=
	ds[16]=EOS;
	day	=atoi(ds)-1;
	mon	=mon2int(ds+3);
	year=atoi(ds+7)>=70 ? atoi(ds+7)-70 : atoi(ds+7)+30;
	hour=atoi(ds+11);
	min	=atoi(ds+14);
	sec	=atoi(ds+17);
	t=sec;
	t+=min*60ul;
	t+=hour*3600ul;
	t+=day*86400ul;
	t+=mon*2678400ul;
	t+=year*32140800ul;

	test_registered();
	if (is_registered)
		ret=(((ulong)dezversion << 24) | 0xaa0000ul) ^ t;
	else
		ret=(((ulong)dezversion << 24) | 0xab0000ul) ^ t;
	ret+=count;
	count++;
	return(ret);
}

word put_msgid(long win, HDR *hdr_p, FILE *msg_fp)
{	/* static word count; */
	word ret=0;
	byte loc_domain[200];
	byte crc_msgid[200];
	byte *cp;
	
	*loc_domain=EOS;
	*crc_msgid=EOS;

	sprintf(wi[win].dup_msgid,"\001MSGID: %s %8.8lx\n",
						mergeaddr(wi[win].hdr->wFromZone,
											wi[win].hdr->wFromNet,
											wi[win].hdr->wFromNode,
											wi[win].hdr->wFromPoint,loc_domain),
											calc_xid(wi[win].hdr->sDate));
/*		        time(NULL)+count); */

	if (msg_fp!=NULL)
	{	word netmail;

		netmail=is_netmail(wi[win].aktarea);

		if (strnicmp("\001MSGID: 0 ",wi[win].dup_msgid,10))
			fputs(wi[win].dup_msgid,msg_fp);
		put_replyid(win,msg_fp);

    if ((gl_area[wi[win].aktarea].flag & AR_UMLAUTE) 								||
    		(gl_area[wi[win].aktarea].flag & AR_CONVERT_LAT1S)					||
    		(gl_area[wi[win].aktarea].flag & AR_CONVERT_LAT1)						||
        (opt.umlaut & O_ISOCONV) 		||
        (opt.umlaut & O_MIMESTYLE) 	||
        ( netmail && (opt.umlaut & O_UMLAUT_N)) ||
        (!netmail && (opt.umlaut & O_UMLAUT_E)) )
    {
    	strcpy(loc_domain,chrs_line);
    	if (opt.umlaut & O_ATARIFONT)
	    	strcat(loc_domain," ATARIST 2\n");
    	else
	    	strcat(loc_domain," IBMPC 2\n");
			fputs(loc_domain,msg_fp);
			ret|=1;
		}	

		put_pid(msg_fp);
#if 0
		if (*wi[win].alert)
			fprintf(msg_fp,"\001ALERT: %s\n",wi[win].alert);
#endif

		strcpy(crc_msgid,wi[win].dup_msgid+8);
		cp=strchr(crc_msgid,LF);
		if (cp)
		{	*cp=EOS;
			while (cp>crc_msgid && *(cp-1)==' ')
				*(--cp)=EOS;
		}
		if (strlen(crc_msgid)>0 || cp!=NULL)
		{
			if (hdr_p!=NULL)
			{	
				if (opt.commailer)
					hdr_p->lMsgidcrc=crc32str(crc_msgid);
				else	
#if defined(__NEWFIDO__)
					wi[win].hdr->lMsgidcrc=crc32str(crc_msgid);
#else
					hdr_p->mid.midcrc=crc32str(crc_msgid);
#endif
			}	
		}	
	}
/*	count++; */
	return(ret);
}

void put_replyid(long win, FILE *msg_fp)
{	byte *cp;
	byte crc_replyid[MAX_STR];

	*crc_replyid=EOS;
	if (strlen(wi[win].dup_replyid)>0L)
	{
		fputs(wi[win].dup_replyid,msg_fp);
		strcpy(crc_replyid,wi[win].dup_replyid+8);
		cp=strchr(crc_replyid,LF);
		if (cp)
		{	*cp=EOS;
			while (cp>crc_replyid && *(cp-1)==' ')
				*(--cp)=EOS;
		}
		if (strlen(crc_replyid)>0 || cp!=NULL)
		{
			if (opt.commailer)
				wi[win].hdr->lReplycrc=crc32str(crc_replyid);
			else	
#if defined(__NEWFIDO__)
				wi[win].hdr->lReplycrc=crc32str(crc_replyid);
#else
				wi[win].hdr->rep.repcrc=crc32str(crc_replyid);
#endif
		}	
	}	
}

static void save_stuff(byte *txt,FILE *msgfp)
{	while (*txt)
	{	if (*txt=='|')
			fputc('\n',msgfp);
		else
			fputc(*txt,msgfp);
		txt++;
	}
}

static void write_randomtear(long win,FILE *msgfp,byte *fname)
{	FILE *fp;
	byte temp[MAX_STR],buffer[1024],*cp;
	ulong size,pos,minpos;

	store_msghdr(win);

	fp=fopen(fname,"r");
	if (!fp)
	{	sprintf(temp,msgtxt[MSG_NOFOOTLINEFILE].text,fname);
		FormAlert(msgtxt[MSG_NOFOOTLINEFILE].defbut,
							msgtxt[MSG_NOFOOTLINEFILE].icon,
							temp,
							msgtxt[MSG_NOFOOTLINEFILE].button);
	}
	else
	{	size=filelength(fileno(fp));
		if (size<=0)
			return;
		fgets(temp,MAX_STR,fp);
		cp=strchr(temp,EOS);
		while (cp>temp && *(cp-1)!=' ' && isspace(*(cp-1)))	cp--;
		*cp=EOS;
		minpos=ftell(fp);
	nochmal:
		pos=Random() % size;
		fseek(fp,pos,SEEK_SET);
		fgets(buffer,1024,fp);
		if (fgets(buffer,1024,fp)==NULL)
		{	fseek(fp,minpos,SEEK_SET);
			fgets(buffer,1024,fp);
		}
		
		if (*buffer==';' || *buffer==EOS)
			goto nochmal;
		cp=strchr(buffer,EOS);
		while (cp>buffer && isspace(*(cp-1)))	cp--;
		*cp=EOS;
		if (*buffer==EOS)
			goto nochmal;

		if (*temp)
		{	convert_percent(temp,wi[win].aktarea,TRUE);
			save_stuff(temp,msgfp);
		}
	
		store_msghdr(win);
		convert_percent(buffer,wi[win].aktarea,TRUE);
		save_stuff(buffer,msgfp);

		fclose(fp);
	}
}

void save_lines(long win, FILE *msgfp, word filter_chrs, word destarea)
{	word i;
	byte *cp,*ptr,temp[MAX_STR];

	i=0;
	while (wi[win].lines[i]!=NULL && i<MAX_LINES)
	{	cp=wi[win].lines[i];

		if (!strncmp(cp,"\001MSGID:",7) || 
		    !strncmp(cp,"\001REPLY:",7) || 
		    !strncmp(cp,"\001PID:",5)   ||
		    !strncmp(cp,"\001ALERT:",7) ||
		    (!strncmp(cp,chrs_line,6) && filter_chrs) )
		{	i++;
			continue;
		}
		else
		{
			ptr=strchr(cp,EOS);
			while (ptr>cp && *(ptr-1)==' ')	/* alle endspaces l”schen */
				*(--ptr)=EOS;

			if (ptr>cp && *(ptr-1)=='\n')
			{
				fputs(cp,msgfp);
			}
			else if (*cp==' ' /*|| strlen(cp)<65*/)	/* ssl 280195 wofr denn <65??? */
			{
				fputs(cp,msgfp);
				fputc('\n',msgfp);
			}
			else
			{
				ptr=strrchr(cp,'-');
				if (ptr==NULL || *(ptr+1)!=EOS)
				{
					fputs(cp,msgfp);
					fputc(' ',msgfp);
				}
				else
					fputs(cp,msgfp);
			}
		}
		i++;
	}

	if (wi[win].lines[0])
		if (!strncmp(wi[win].lines[0],"\003AREA ",6))
		{	cp=strcpy(temp,wi[win].lines[0]+6);
			while (*cp)
			{	if (*cp=='[' || *cp==']')
					strcpy(cp,cp+1);
				else
					cp++;
			}
			cp=skip_blanks(temp);
			cut_endspc(cp);
			for (i=0;i<gl_arzahl;i++)
			{	if (!stricmp(cp,gl_area[i].name))
				{	destarea=i;
					break;
				}
			}
		}

	if (!(gl_area[destarea].flag & AR_NORANDOMTEARLINE))
	{
		if (gl_area[destarea].rndfootpath)
		{	write_randomtear(win,msgfp,gl_area[destarea].rndfootpath);
		}
		else if ((opt.flag & O_FLAG_RANDOMTEAR) ||
				(gl_area[destarea].flag & AR_RANDOMTEARLINE))
		{	
			sprintf(temp,"%sLED.FLN",gl_ledpath);
			write_randomtear(win,msgfp,temp);
		}
	}
	
	/* terminate message */
	fputc(EOS,msgfp);
}

void init_carbon(void)
{	word i;

	for (i=0;i<MAX_CC;i++)
	{	*cc_lines[i]=
		*cc_node[i] = EOS;
		strcpy(cc_nof[i]," ");
	}
}

void show_ccpage(word page, word redraw)
{	word i;
	byte tmp[6];
	
	for (i=0;i<10;i++)
	{	sprintf(tmp,"%2u.",i+page+1);
		set_dbutton(carbon_tree,ccn_obj[i],tmp,redraw);
	}

	for (i=0;i<10;i++)
	{	set_dtext(carbon_tree,ccto_obj[i],cc_lines[page+i],FALSE,35);
		set_dtext(carbon_tree,ccnode_obj[i],cc_node[page+i],FALSE,20);
		set_dtext(carbon_tree,ccnof_obj[i],cc_nof[page+i],FALSE,1);
	}
	if (redraw)
	{	objc_draw(carbon_tree, CCNAMES, MAX_DEPTH, 
		          carbon_tree->ob_x, carbon_tree->ob_y, 
		          carbon_tree->ob_width, carbon_tree->ob_height);
	}
	if (page<10)
	{	set_state(carbon_tree,CCPREV,DISABLED,redraw);
		del_state(carbon_tree,CCNEXT,DISABLED,redraw);
	}
	else if (page>=MAX_CC-10)
	{	del_state(carbon_tree,CCPREV,DISABLED,redraw);
		set_state(carbon_tree,CCNEXT,DISABLED,redraw);
	}
	else
	{	del_state(carbon_tree,CCPREV,DISABLED,redraw);
		del_state(carbon_tree,CCNEXT,DISABLED,redraw);
	}
}

void get_ccpage(word page)
{	word i;
	
	for (i=0;i<10;i++)
	{	get_dedit(carbon_tree,ccto_obj[i],cc_lines[page+i]);
		get_dedit(carbon_tree,ccnode_obj[i],cc_node[page+i]);
		get_dedit(carbon_tree,ccnof_obj[i],cc_nof[page+i]);
	}
}

static void usern_tostr(byte *sn, byte *sstr)
{		      	   /* "2:241/3.14" -> "2    241  3    14  " */   
	byte tmp[MAX_STR];
	uword szone,snet,snode,spoint;
	
	szone = snet = snode = spoint = 0;
	strcpy(tmp,sn);
	cut_endspc(tmp);
	scanaddr(tmp,&szone,&snet,&snode,&spoint,NULL);
	if (!szone)
		szone=addr[0].zone;
	if (!snet)
	{	snet=addr[0].net;
		snode=addr[0].node;
	}
	sprintf(sstr,"%-5u%-5u%-5u%-5u",szone,snet,snode,spoint);
} 

void dia_carbon(void)
{	FILE *ccfile;
	word page;
	word ext_btn;
	word z, i, hidden;
	uword szone, snet, snode, spoint;
	byte *cp;
	byte tmp[MAX_STR+1];
	byte lines[MAX_CC+2][40];
	byte node[MAX_CC+2][25];
	byte nof[MAX_CC+2][2];

	for (i=0;i<MAX_CC;i++)
	{	strcpy(lines[i],cc_lines[i]);
		strcpy(node[i],cc_node[i]);
		strcpy(nof[i],cc_nof[i]);
	}
	page=0;
	show_ccpage(page,FALSE);
	draw_dialog(carbon_tree, &carbon_dial);
	do
	{
		ext_btn=Form_do(&carbon_dial,CCTO1);
		biene();

		if (ext_btn==CCUSERL)
		{	get_ccpage(page);
			z=0;

			if (page!=0) show_ccpage(0,TRUE);
			while (z<MAX_CC)
			{	for (i=0;i<10;i++)
				{	if ( (*cc_lines[z+i] && !(*cc_node[z+i])) ||
							 (!(*cc_lines[z+i]) && *cc_node[z+i]) )
					{	nnstr2node(cc_node[z+i],&szone,&snet,&snode,&spoint);

						hidden=FALSE;
						if (*cc_lines[z+i]=='-')
						{	hidden=TRUE;
							strcpy(cc_lines[z+i],cc_lines[z+i]+1);
						}
						
						if (xsrch_user(FALSE,cc_lines[z+i],NULL,
																		&szone,&snet,&snode,&spoint))
						{	sprintf(node[z+i],"%-5u%-5u%-5u%-5u",szone,snet,snode,spoint);
							set_dtext(carbon_tree,ccnode_obj[i],node[z+i],TRUE,20);
							set_dtext(carbon_tree,ccnof_obj[i]," ",TRUE,1);
							set_dtext(carbon_tree,ccto_obj[i],cc_lines[z+i],TRUE,36);
						}
						else
							set_dtext(carbon_tree,ccnof_obj[i],"*",TRUE,1);

						if (hidden)
						{	strcpy(tmp,"-");
							strcat(tmp,cc_lines[z+i]);
							strcpy(cc_lines[z+i],tmp);
							set_dtext(carbon_tree,ccto_obj[i],cc_lines[z+i],TRUE,36);
						}
					}

				}
				get_ccpage(z);
				z+=10;
				if (z<MAX_CC) show_ccpage(z,TRUE);
			}
			show_ccpage(page,TRUE);
		}

		else if (ext_btn==CCNEXT)
		{	get_ccpage(page);
			if (page<MAX_CC-10)
			{	page+=10;
				show_ccpage(page,TRUE);
			}
		}

		else if (ext_btn==CCPREV)
		{	get_ccpage(page);
			if (page>=10)
			{	page-=10;
				show_ccpage(page,TRUE);
			}
		}

		else if (ext_btn==CCCLEAR)
		{	init_carbon();
			page=0;
			show_ccpage(page,TRUE);
		}

		else if (ext_btn==CCSAVE)
		{	get_ccpage(page);
			del_dialog(&carbon_dial);
			hole_file(tmp,NULL,"*.CCL",msgtxt[FILE_CARBON].text);
			redraw_all();
			if ((ccfile=fopen(tmp,"w"))!=NULL)
			{	for (i=0;i<MAX_CC;i++)
				{	if (*cc_lines[i])
					{	if (*cc_node[i])
						{	nnstr2node(cc_node[i],&szone,&snet,&snode,&spoint);
							fprintf(ccfile,"%-40s %s\n",cc_lines[i],
															mergeaddr(szone,snet,snode,spoint,NULL));
						}
						else
						{	fprintf(ccfile,"%-40s\n",cc_lines[i]);
						}
					}
				}
				fclose(ccfile);
			}
			show_ccpage(page,FALSE);
			draw_dialog(carbon_tree, &carbon_dial);
		}

		else if (ext_btn==CCLOAD)
		{	z=LangAlert(MSG_LOADCCL);
			if (z==1)
				init_carbon();

			del_dialog(&carbon_dial);
			hole_file(tmp,NULL,"*.CCL",z==1 ? msgtxt[FILE_CARBONLOAD].text :
																				msgtxt[FILE_CARBONMERGE].text);
			redraw_all();
			if ((ccfile=fopen(tmp,"r"))!=NULL)
			{
				i=0;
				if (z==2)
				{	for(i=0;i<MAX_CC;i++)
						if (*cc_lines[i]==EOS)
							break;
				}
				while (fgets(tmp,MAX_STR,ccfile)!=NULL && i<MAX_CC)
				{	strcpy(tmp,skip_blanks(tmp));
					if (!(*tmp)) continue;

					cp=strrchr(tmp,':');
					if (cp==NULL)
					{	
						cut_endspc(tmp);
						strcpy(cc_lines[i],tmp);
						sprintf(cc_node[i],"%-5u%-5u%-5u%-5u",
																		addr[0].zone, addr[0].net, 
																		addr[0].node, addr[0].point);
						i++;
					}
					else
					{	cp--;
						while (isdigit(*cp))
							cp--;
						*(cp-1)=EOS;
						cut_endspc(tmp);
						strcpy(cc_lines[i],tmp);
						usern_tostr(cp,cc_node[i]);
						i++;
					}
				}
				fclose(ccfile);
				page=0;
				show_ccpage(page,FALSE);
			}
			draw_dialog(carbon_tree, &carbon_dial);
		}

		del_state(carbon_tree,ext_btn,SELECTED,TRUE);
		mouse_normal();
	} while (ext_btn!=CCOK && ext_btn!=CCCANCEL);
	if (ext_btn==CCCANCEL)
	{
		for (i=0;i<MAX_CC;i++)
		{
			strcpy(cc_lines[i],lines[i]);
			strcpy(cc_node[i],node[i]);
			strcpy(cc_nof[i],nof[i]);
		}
	}
	else
		get_ccpage(page);
	del_dialog(&carbon_dial);
	del_state(carbon_tree,ext_btn,SELECTED,FALSE);
}

void save_carbon(long win)
{	word i,allhidden;
	byte cc_orig[MAX_STR];
	
	allhidden=TRUE;
	for (i=0;i<MAX_CC;i++)
	{ if (*cc_lines[i]!='-' && strlen(cc_lines[i])>0)
		{	allhidden=FALSE;
			break;
		}
	}

	if (allhidden)
		save_newmsg(win,NULL,FALSE);
	else
	{	if (wi[win].hdr->wToZone==0 && wi[win].hdr->wToNet==0 &&
	    	wi[win].hdr->wToNode==0 && wi[win].hdr->wToPoint==0)
	  	strcpy(cc_orig,wi[win].hdr->sTo);
	  else
			sprintf(cc_orig,msgtxt[DIV_ORIGINALTO].text,
		  	        wi[win].hdr->sTo,mergeaddr(wi[win].hdr->wToZone,
	  	  	      													 wi[win].hdr->wToNet,
	    	  	    													 wi[win].hdr->wToNode,
	      	  	  													 wi[win].hdr->wToPoint,NULL));
		save_newmsg(win,cc_orig,FALSE);
	}
	
	wi[win].hdr->wFlags |= F_KILLSENT;
	for (i=0;i<MAX_CC;i++)
	{
		if (strlen(cc_lines[i])>0)
		{
			if (*cc_lines[i]=='-')
				strcpy(wi[win].hdr->sTo,cc_lines[i]+1);
			else
				strcpy(wi[win].hdr->sTo,cc_lines[i]);

			nnstr2node(cc_node[i],
			   (uword *)&wi[win].hdr->wToZone,(uword *)&wi[win].hdr->wToNet,
			   (uword *)&wi[win].hdr->wToNode,(uword *)&wi[win].hdr->wToPoint);

			phdrline(win);				/* Zeige To-Adresse an */

			if (allhidden)
				save_newmsg(win,NULL,FALSE);
			else
				save_newmsg(win,cc_orig,FALSE);
		}
	}
	wi[win].hdr->wFlags &= ~F_KILLSENT;

/*
**	Wenn cc_orig!=NULL, dann wird KEIN open_allarea in save_newmsg
**	gemacht.
*/
	open_allarea(wi[win].aktarea);
}
