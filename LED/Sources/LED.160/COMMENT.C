#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "language.h"

typedef struct
{ ulong 	mid;
	word 	msgnr;
	byte    plus;
} ID_TAB;

void comment_start(long win)
{	ulong id;
	word msgnr,foundmsg=0;
	HDR thdr;
	WIND *winp;
	
	if (wi[win].ramhdr==FALSE)
	{	LangAlert(MSG_HDRCACHEFIRST);
		return;
	}
		
	winp=&wi[win];

	if (opt.commailer)
		id=winp->hdr->lReplycrc;
	else	
		id=winp->hdr->rep.repcrc;

	if (!id)
		return;

	biene();

	/* Original-Msg suchen */
	msgnr=winp->firstmsg;
	while (id && msgnr<=winp->lastmsg && msgnr>=winp->firstmsg)
	{	fasthdrseek(winp, winp->hdrfp,(msgnr-1), SEEK_SET);
		fasthdrread(winp, &thdr, 1, winp->hdrfp);

		if (!(thdr.wFlags & F_DELETED))
		{	if (opt.commailer)
			{	if ( thdr.lMsgidcrc==id )
				{	id=thdr.lReplycrc;
					if (!id) foundmsg=msgnr;
					msgnr=winp->firstmsg - 1;
				}
			}
			else
			{	if ( thdr.mid.midcrc==id )
				{	id=thdr.rep.repcrc;
					if (!id) foundmsg=msgnr;
					msgnr=winp->firstmsg - 1;
				}
			}
		}
		msgnr++;
	}

	if (id)		/* Diese Msg ist immer noch ein Reply! */
	{
		msgnr=winp->firstmsg;
		while (msgnr<=winp->lastmsg && msgnr>=winp->firstmsg)
		{	fasthdrseek(winp, winp->hdrfp,(msgnr-1), SEEK_SET);
			fasthdrread(winp, &thdr, 1, winp->hdrfp);

			if (!(thdr.wFlags & F_DELETED))
			{	if (opt.commailer)
				{	if ( thdr.lReplycrc==id )
					{	mouse_normal();
						select_msg(win,msgnr,TRUE,TRUE);
						return;
					}
				}
				else
				{	if ( thdr.rep.repcrc==id )
					{	mouse_normal();
						select_msg(win,msgnr,TRUE,TRUE);
						return;
					}
				}
			}
			msgnr++;
		}
	}
	else
	{	mouse_normal();
		if (foundmsg)
			select_msg(win,foundmsg,TRUE,TRUE);
		return;
	}
	mouse_normal();
}

void comment(long win, word mode)
{	ulong id;
	word msgnr;
	WIND *winp;
	HDR thdr;
	
	winp=&wi[win];
	if (winp->ramhdr==FALSE)
	{	LangAlert(MSG_HDRCACHEFIRST);
		return;
	}
		
#if defined(__NEWFIDO__)
	if (mode==FIRST)	id=winp->hdr->lMsgidcrc;
	else							id=winp->hdr->lReplycrc;
#else
	if (opt.commailer)
	{	if (mode==FIRST)	id=winp->hdr->lMsgidcrc;
		else							id=winp->hdr->lReplycrc;
	}
	else	
	{	if (mode==FIRST)	id=winp->hdr->mid.midcrc;
		else 							id=winp->hdr->rep.repcrc;
	}
#endif

	if (id==0L)
		return;

	biene();
	switch(mode)
	{	case PREV			:	msgnr=winp->aktmsg-1; break;
		case NEXT			:	msgnr=winp->aktmsg+1; break;
		case ORIGINAL :
		case FIRST		:	msgnr=winp->firstmsg;	break;
	}

	while (msgnr<=winp->lastmsg && msgnr>=winp->firstmsg)
	{	hdrseek(win,winp->hdrfp,(msgnr-1),SEEK_SET);
		hdrread(win,&thdr, 1, winp->hdrfp);
		if (!(thdr.wFlags & F_DELETED))
		{	
#if defined(__NEWFIDO__)
			if ( (mode==ORIGINAL && thdr.lMsgidcrc==id) ||
					 (mode!=ORIGINAL && thdr.lReplycrc==id) )
			{	mouse_normal();
				select_msg(win,msgnr,TRUE,TRUE);
				return;
			}
#else
			if (opt.commailer)
			{
				if ( (mode==ORIGINAL && thdr.lMsgidcrc==id) ||
						 (mode!=ORIGINAL && thdr.lReplycrc==id) )
				{	mouse_normal();
					select_msg(win,msgnr,TRUE,TRUE);
					return;
				}
			}
			else
			{	if ( (mode==ORIGINAL && thdr.mid.midcrc==id) ||
						 (mode!=ORIGINAL && thdr.rep.repcrc==id) )
				{	mouse_normal();
					select_msg(win,msgnr,TRUE,TRUE);
					return;
				}
			}
#endif

		}
		if (mode==PREV)	msgnr--;
		else						msgnr++;
	}
	mouse_normal();
}


/*
** ssl 170195:
**	Was man doch mit ein wenig berlegen so alles erreichen kann ;-)
*/

/* Gibt Nummer der Originalmsg zurck!!! */

static word search_msgid(WIND *winp, word msg, ulong id, char *outstr)
{	word i;
	HDR 	thdr;
	byte 	tmp[20];
	FILE	*hdrfp;
	ulong	ltemp;

	hdrfp=winp->hdrfp;

	fasthdrseek(winp, hdrfp, (winp->firstmsg-1), SEEK_SET);
	for (i=winp->firstmsg-1;i<=msg;i++)
	{
		fasthdrread(winp,&thdr, 1, hdrfp);
		if (!(thdr.wFlags & F_DELETED))
		{
			if (opt.commailer)	ltemp = thdr.lMsgidcrc;
			else								ltemp = thdr.mid.midcrc;

			if (ltemp==id)
			{	strcat(outstr," ");
				strcat(outstr,itoa(i+1,tmp,10));
				return(i+1);
			}
		}
	}
	return 0;
}

static word search_reply(WIND *winp, word msg, ulong id, char *outstr)
{	word i,flag=FALSE;
	HDR 	thdr;
	byte 	tmp[20];
	FILE	*hdrfp;
	ulong	ltemp;
	
	hdrfp=winp->hdrfp;

	fasthdrseek(winp,hdrfp,msg,SEEK_SET);
	for (i=msg;i<winp->lastmsg;i++)
	{
		fasthdrread(winp,&thdr, 1, hdrfp);
		if (!(thdr.wFlags & F_DELETED))
		{
			if (opt.commailer)	ltemp = thdr.lReplycrc;
			else								ltemp = thdr.rep.repcrc;

			if (ltemp==id)
			{	strcat(outstr," ");
				if (i+1==winp->aktmsg) strcat(outstr,"*");
				strcat(outstr,itoa(i+1,tmp,10));
				if (i+1==winp->aktmsg) strcat(outstr,"*");
				flag=TRUE;
			}
		}
	}
	return(flag);
}

void set_comstatus(long win, word msg)
{	ulong msgid, reply;
	word	replyto;
	byte 	outstr[4096],tmp[20];
	WIND  *winp;

	winp=&wi[win];

	if (opt.showcom==FALSE)
	{	strcpy(winp->status,"...");
		return;
	}
	if (winp->ramhdr==FALSE && opt.hdrcache && winp->aktmsg!=0)
	{	strcpy(winp->status,msgtxt[SYSTEM_NOMEMFORHC].text);
		return;
	}
	if (winp->ramhdr==FALSE && winp->aktmsg!=0)
	{	strcpy(winp->status,msgtxt[SYSTEM_NOHC].text);
		return;
	}
	if (winp->aktmsg<=0)
	{	*winp->status=EOS;
		return;
	}

	*outstr=EOS;

	if (opt.commailer)
	{	msgid=winp->hdr->lMsgidcrc;
		reply=winp->hdr->lReplycrc;
	}
	else
	{	msgid=winp->hdr->mid.midcrc;
		reply=winp->hdr->rep.repcrc;
	}

	if (reply==0L && msgid==0)
	{	strcpy(winp->status,msgtxt[SYSTEM_NOLINKING].text);
		return;
	}

/***************************************************/

	replyto=0;
	if (reply!=0)					/* Suche Originalmsg dazu */
	{	strcat(outstr,msgtxt[SYSTEM_REPLYTO].text);
		if (opt.completescan)
		{	if ((replyto=search_msgid(winp, winp->lastmsg, reply, outstr))==0)
				strcat(outstr," ?");
		}
		else
		{	if ((replyto=search_msgid(winp, msg, reply, outstr))==0)
				strcat(outstr," ?");
		}
	}
	else
	{ strcat(outstr,msgtxt[SYSTEM_ORIGINALMSG].text);
	}

/***************************************************/

	strcat(outstr," |");
	if (reply!=0)					/* Suche alle gleichen Replys dazu */
	{	if (opt.completescan)
			search_reply(winp, winp->firstmsg-1, reply, outstr);
		else
			search_reply(winp, max(replyto,winp->firstmsg-1), reply, outstr);
	}
	else
	{	strcat(outstr," *");
		strcat(outstr,itoa(msg,tmp,10));
		strcat(outstr,"*");
	}

/***************************************************/

	strcat(outstr,msgtxt[SYSTEM_REPLIES].text);
	if (msgid!=0)					/* Suche alle Replys dazu */
	{	
		if (opt.completescan)
		{	if (!search_reply(winp, winp->firstmsg-1, msgid, outstr))
				strcat(outstr,msgtxt[SYSTEM_REPLYNONE].text);
		}
		else
		{	if (!search_reply(winp, msg, msgid, outstr))
				strcat(outstr,msgtxt[SYSTEM_REPLYNONE].text);
		}
		strcat(outstr," |");
	}
	else
	{	strcat(outstr," ? |");
	}

/***************************************************/

	strncpy(winp->status,outstr,MAX_STATUSLINE-1);
	winp->status[MAX_STATUSLINE-1]=EOS;
}
