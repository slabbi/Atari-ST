#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "language.h"

TREE tree[MAX_MSG];			/* Konvertiert "tree[aktmsg]->zeile/selline" */

static char	intree[MAX_MSG];
static int	gl_count;
static char tmp[256];

void get_msgline(HDR *thdr,char *line,word aktmsg,int indent,word area)
{	char spez[]="   ";
	if (is_ptowner(thdr->sTo,area))
		spez[0]='\x3';
	else if (is_ptowner(thdr->sFrom,area))
		spez[0]='\x4';

	if (thdr->wFlags & F_DELETED)			spez[1]='d';
	else if (thdr->XFlags & XF_LOCK)	spez[1]='x';

	if (!(thdr->XFlags & XF_READ))	spez[2]='u';

	sprintf(line,"%s %4d %*s%-18.18s %-18.18s %s ",
							spez,aktmsg+1,
							indent,"",
							thdr->sFrom,thdr->sTo,thdr->sSubject);
}


void update_msglistwindow(long win, word aktmsg)
{	HDR thdr;
	if (wi[win].hdrfp2==NULL)
	{	LangAlert(MSG_POINTERERROR);
		return;
	}
	hdrseek(win,wi[win].hdrfp2,aktmsg,SEEK_SET);
	hdrread(win,&thdr,1,wi[win].hdrfp2);
	get_msgline(&thdr,tmp,aktmsg, tree[aktmsg].indent, wi[win].aktarea);

	free(wi[win].lines[ tree[aktmsg].zeile ]);
	wi[win].lines[ tree[aktmsg].zeile ]=strdup(tmp);
}


void make_msglist(long win,long newwin)
{	int msg;
	HDR thdr;
	hdrseek(win,wi[win].hdrfp,0,SEEK_SET);
	for (msg=0;msg<wi[win].lastmsg;msg++)
	{	
/*	hdrseek(win,wi[win].hdrfp,msg,SEEK_SET); */
		hdrread(win,&thdr,1,wi[win].hdrfp);
		get_msgline(&thdr,tmp,msg,0,wi[win].aktarea);
		tree[msg].indent=0;
		tree[msg].zeile=msg;
		wi[newwin].lines[msg]=strdup(tmp);
	}	
}


/*
** ******************************************************************
*/

void comm_suche_reply(HDR *hdr,WIND *winp,WIND *newwinp,ulong id,int msg,int indent)
{	TREE *treep;

	treep=&tree[msg];

	while (msg<winp->lastmsg)
	{	if (!intree[msg] && treep->Reply==id)
		{	
			fasthdrseek(winp,winp->hdrfp,msg,SEEK_SET);
			fasthdrread(winp,hdr,1,winp->hdrfp);

			get_msgline(hdr,tmp,msg,indent,winp->aktarea);
			treep->indent=indent;
			treep->zeile=gl_count;
			newwinp->lines[gl_count++]=strdup(tmp);
			intree[msg]=TRUE;
			if (treep->Msgid!=0)
			{	
				if (opt.completescan)
					comm_suche_reply(hdr,winp,newwinp,treep->Msgid,0,indent+2);
				else
					comm_suche_reply(hdr,winp,newwinp,treep->Msgid,msg+1,indent+2);
/*			fasthdrseek(winp,winp->hdrfp,pos,SEEK_SET); */
			}
		}
		msg++;
		treep++;
	}
}

void comm_make_msglist(long win,long newwin)
{	int msg;
	HDR hdr;
	WIND *winp,*newwinp;
	FILE *hdrfp;
	TREE *treep;

	winp=&wi[win];
	newwinp=&wi[newwin];
	hdrfp=winp->hdrfp;

	fasthdrseek(winp,hdrfp,0,SEEK_SET);

	treep=&tree[0];
	for (msg=0; msg<winp->lastmsg; msg++)
	{	
		fasthdrread(winp,&hdr,1,hdrfp);
		if (opt.commailer)
		{	treep->Msgid=hdr.lMsgidcrc;
			treep->Reply=hdr.lReplycrc;
		}
		else
		{	treep->Msgid=hdr.mid.midcrc;
			treep->Reply=hdr.rep.repcrc;
		}
		treep++;
	}

	gl_count=0;
	treep=&tree[0];

	for (msg=0; msg<winp->lastmsg; msg++)
	{	if (intree[msg]==0)
		{	fasthdrseek(winp,hdrfp,msg,SEEK_SET);
			fasthdrread(winp,&hdr,1,hdrfp);

			get_msgline(&hdr,tmp,msg,0,winp->aktarea);
			treep->zeile=gl_count;
			treep->indent=0;
			newwinp->lines[gl_count++]=strdup(tmp);

			intree[msg]=TRUE;
			if (opt.completescan)
			{	if (treep->Msgid!=0)
					comm_suche_reply(&hdr,winp,newwinp,treep->Msgid,0,2);
				if (treep->Reply!=0)
					comm_suche_reply(&hdr,winp,newwinp,treep->Reply,0,0);
			}
			else
			{	if (treep->Msgid!=0)
					comm_suche_reply(&hdr,winp,newwinp,treep->Msgid,msg,2);
				if (treep->Reply!=0)
					comm_suche_reply(&hdr,winp,newwinp,treep->Reply,msg,0);
			}
		}
		treep++;
	}
}


/*
** ******************************************************************
*/


long msg_listwindow(long win)
{	HDR  *hp;
	byte win_titel[MAX_STR];
	long c_on,c_off;

	biene();
	if (wi[MSGSWIND].belegt && wi[MSGSWIND].listwind==WI_LIST_MSGS 
	    && wi[MSGSWIND].aktarea==wi[win].aktarea)
	{
		mouse_normal();
		top_it(MSGSWIND);
		return TRUE;
	}
	if (wi[MSGSWIND].belegt && wi[MSGSWIND].listwind==WI_LIST_MSGS)
		close_window(MSGSWIND);
	if (wi[win].lastmsg<=0)
	{	mouse_normal();
		return FALSE;
	}

	memcpy(&wi[MSGSWIND],&wi[win],sizeof(WIND));
	hp=calloc(1,sizeof(HDR));
	if (hp!=NULL)
	{	memcpy(hp,wi[win].hdr,sizeof(HDR));
		wi[MSGSWIND].hdr=hp;	
	}
	else
	{	LangAlert(MSG_NOMEMNEWHEADER);
		return(-1);	
	}

	memset(wi[MSGSWIND].lines,0,sizeof(wi[MSGSWIND].lines));
	memset(intree,0,sizeof(intree));
	memset(tree,0,sizeof(tree));

#if 0
	if (opt.winprm[MSGSWIND][0]>=0 && opt.winprm[MSGSWIND][1]>=0 &&		/* ssl 91294 */
			opt.winprm[MSGSWIND][2]>=0 && opt.winprm[MSGSWIND][3]>=0)
#endif
		wi[MSGSWIND].px=-1;

	sprintf(win_titel,msgtxt[DIV_AREA].text,wi[MSGSWIND].areaname);
	open_window(MSGSWIND,win_titel,FALSE);	

	c_on =gl_area[wi[win].aktarea].flag & AR_COMMENTTREEON;
	c_off=gl_area[wi[win].aktarea].flag & AR_COMMENTTREEOFF;
	if ((opt.usecommenttree && !c_off) || c_on)
		comm_make_msglist(win,MSGSWIND);
	else
		make_msglist(win,MSGSWIND);

	wi[MSGSWIND].lzahl=wi[win].lastmsg;
	wi[MSGSWIND].ramhdr=FALSE;
	wi[MSGSWIND].hdrptr=wi[MSGSWIND].hdrbuf=NULL;
	wi[MSGSWIND].hdrlen=0L;

	wi[MSGSWIND].listwind=WI_LIST_MSGS;
	wi[MSGSWIND].topmsg_line=0;
	wi[MSGSWIND].oldarea=wi[MSGSWIND].aktarea;
	wi[MSGSWIND].msgfp=
	wi[MSGSWIND].hdrfp=NULL;
	wi[MSGSWIND].hdrfp2=wi[win].hdrfp;
	wi[MSGSWIND].selline=-1;
	wi[MSGSWIND].topline=0;

	if (wi[win].aktmsg>0 &&
			wi[win].aktmsg<=wi[MSGSWIND].lzahl+1)
		wi[MSGSWIND].selline=wi[MSGSWIND].topline=
													tree[ wi[win].aktmsg-1 ].zeile;

	wi[MSGSWIND].cy=
	wi[MSGSWIND].cx=0;
	sprintf(tmp," List-Window    Max.Msg: %d",wi[MSGSWIND].lastmsg);
	upd_status(MSGSWIND, tmp);
	mouse_normal();	
	return (MSGSWIND);
}

void open_msglistwindow(long win)
{	word laufw;
	if (opt.listmwindow)
	{	msg_listwindow(win);

		for (laufw=0;laufw<MAX_WIND;laufw++)
		{	if (wi[laufw].belegt && !wi[laufw].editierbar &&
					!wi[laufw].listwind && wi[laufw].handle>0 &&
					wi[MSGSWIND].aktarea==wi[laufw].aktarea)
			{	top_it(laufw);
				break;
			}
		}
	}
}

void need_update_msglistwindow(long win,word mode)
{	word xmsgbuff[8];

	if (!wi[MSGSWIND].belegt)
		return;
	if (wi[MSGSWIND].selline==-1 /*|| wi[win].aktmsg<=0*/)
		return;

	if (wi[win].aktarea!=wi[MSGSWIND].aktarea)			/* ssl 171294: Andere Area als dargestellt? */
	{	if (wi[MSGSWIND].belegt)
			close_window(MSGSWIND);
		open_msglistwindow(win);
	}
	else
	{	if (wi[MSGSWIND].belegt && wi[MSGSWIND].listwind==WI_LIST_MSGS)
		{	switch(mode)
			{ case RETURN:					/* Setze Cursor */
					printline(MSGSWIND,wi[MSGSWIND].selline,FALSE);


					if (wi[win].aktmsg<=0)
					{	printline(MSGSWIND,wi[MSGSWIND].selline,TRUE);
						break;
					}


					wi[MSGSWIND].selline=tree[wi[win].aktmsg-1].zeile;

					if (wi[MSGSWIND].selline<wi[MSGSWIND].topline || 
					    wi[MSGSWIND].selline>=wi[MSGSWIND].topline+wi[MSGSWIND].maxshow)
					{	wi[MSGSWIND].selline=
						wi[MSGSWIND].topline=tree[wi[win].aktmsg-1].zeile;
						xmsgbuff[0]=WM_REDRAW;
						xmsgbuff[3]=wi[MSGSWIND].handle;
						xmsgbuff[4]=wi[MSGSWIND].x;
						xmsgbuff[5]=wi[MSGSWIND].y;
						xmsgbuff[6]=wi[MSGSWIND].w;
						xmsgbuff[7]=wi[MSGSWIND].h;
						hndl_wind(xmsgbuff);
					}
					else
					{	printline(MSGSWIND,wi[MSGSWIND].selline,FALSE);
						wi[MSGSWIND].selline=tree[wi[win].aktmsg-1].zeile;
						printline(MSGSWIND,wi[MSGSWIND].selline,TRUE);
					}
					break;
			}
		}
	}
}

#define ASC2BIN(x) (((x)[0]-48)*10 + ((x)[1]-48))
int compare(TREE *e1,TREE *e2)
{			 if (e1->Msgid < e2->Msgid) return -1;
	else if (e1->Msgid > e2->Msgid) return 1;
	else return 0;
}
int compare2(TREE *e1,TREE *e2)
{			 if (e1->zeile < e2->zeile) return -1;
	else if (e1->zeile > e2->zeile) return 1;
	else return 0;
}

void sort_header(long win)
{	int msg,nochmal,x;
	HDR hdr1,hdr2;
	ulong time;
	WIND 	*winp;
	FILE	*hdrfp;
	TREE	*treep,*treex;
	word	msgpos,pos;

	biene();
	show_doing(DO_SORTHEADER);

	winp=&wi[win];
	hdrfp=winp->hdrfp;
	treep=&tree[0];

	fasthdrseek(winp,hdrfp,0,SEEK_SET);
	for (msg=0;msg<wi[win].lastmsg;msg++)
	{	fasthdrread(winp,&hdr1,1,hdrfp);
		treep->zeile=msg;
		time	=	ASC2BIN(&hdr1.sDate[17]);
		time +=	ASC2BIN(&hdr1.sDate[14]) 		* 60UL;
		time += ASC2BIN(&hdr1.sDate[11]) 		* 3600UL; 			/* 60*60 */
		time += (ASC2BIN(&hdr1.sDate[0])-1)	* 86400UL;			/* 60*60*24 */
		hdr1.sDate[6]=EOS;
		time += mon2int(&hdr1.sDate[3])			* 2678400UL;		/* 60*60*24*31 */
		time += ASC2BIN(&hdr1.sDate[7])  		* 32140800UL;		/* 60*60*24*31*12 */
		treep->Msgid=time;
		treep++;
	}
	qsort(&tree[0],winp->lastmsg,sizeof(TREE),compare);

	do
	{	nochmal=FALSE;

		treep=&tree[0];
		msgpos=0;
		for (msg=0;msg<wi[win].lastmsg;msg++)
		{	
			if (treep->zeile!=msg)
			{	nochmal=TRUE;

				pos=treep->zeile;
				fasthdrseek(winp,hdrfp,pos,SEEK_SET);
				fasthdrread(winp,&hdr1,1,hdrfp);

				fasthdrseek(winp,hdrfp,msgpos,SEEK_SET);
				fasthdrread(winp,&hdr2,1,hdrfp);

				fasthdrseek(winp,hdrfp,msgpos,SEEK_SET);
				hdrwrite(win,&hdr1,1,hdrfp);

				fasthdrseek(winp,hdrfp,pos,SEEK_SET);
				hdrwrite(win,&hdr2,1,hdrfp);

				treex=treep;
				for (x=msg;x<msg<winp->lastmsg;x++)
				{	if (treex->zeile==msg)
						break;
					treex++;
				}
				treex->zeile=treep->zeile;
				treep->zeile=msg;
			}
			msgpos++;
			treep++;
		}
	} while (nochmal);

	hole_msgp(win);
	del_doing();
	mouse_normal();

	wi[MSGSWIND].aktarea=-1;
	select_msg(win,winp->lastmsg,TRUE,TRUE);
	need_update_msglistwindow(win,RETURN);
}

#if 0
Vorher
  0:				0
  1:				1
  2:				2
  3:				3

Qsort aufrufen

da muž     diese Msg stehen
	0: zeile: 2
	1:        1
	2:        3
	3:        0

	do
	{	nochmal=FALSE;

		for (msg=0;msg<wi[win].lastmsg;msg++)
		{	
			if (tree[msg].zeile!=msg)		/* Ist die Msg an der falschen Position? */
			{	nochmal=TRUE;							/* ...ja */

Die 2. Msg lesen (sie geh”rt nach 0!)
				fasthdrseek(winp,hdrfp,tree[msg].zeile,SEEK_SET);	/* Msg steht hier */
				fasthdrread(winp,&hdr1,1,hdrfp);

0. Msg lesen
				fasthdrseek(winp,hdrfp,msg,SEEK_SET);			/* und dort geh”rt sie hin */
				fasthdrread(winp,&hdr2,1,hdrfp);

				fasthdrseek(winp,hdrfp,msg,SEEK_SET);
				hdrwrite(win,&hdr1,1,hdrfp);

				fasthdrseek(winp,hdrfp,tree[msg].zeile,SEEK_SET);					/* austauschen */
				hdrwrite(win,&hdr2,1,hdrfp);

0: zeile 2	-> hier muž nun eine 0 draus gemacht werden (die 2. geh”rt nach 0)
1:       1
2:       3
3:       0

Die 0.Msg habe ich an 2 Stelle geschrieben!

msg=0: Wo ist momentan die 0.Msg?
				for (x=msg; msg<winp->lastmsg;x++)
				{	if (tree[x].zeile==msg)
						break;
				}

Aha, im 3.Tabellenplatz. Hier steht momentan noch, daž die 0.nach 3
geh”rt. Richtig w„re jetzt: Die 2.geh”rt nach 3.

				tree[x].zeile=tree[msg].zeile;
0: zeile 2
1:       1
2:       3
3:       2	-> Die 2. geh”rt nach 3

				tree[msg].zeile=msg;
0: zeile 0	-> Die 0 ist schon korrekt.
1:       1
2:       3
3:       2
			}
		}
	} while (nochmal);
#endif
