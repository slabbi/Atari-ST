#include "ccport.h"
#include <time.h>
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"
#include "winddefs.h"
#include "language.h"

static byte loc_from[40];
static byte loc_From[40];
static byte loc_cfrom[40];
static byte loc_cto[40];
static byte loc_to[40];
static byte loc_To[40];
static byte loc_subject[80];
static byte loc_date[20];
static byte loc_Date[20];
static byte loc_zeit[20];
byte loc_address[MAX_STR];
byte old_msgtime[20];

void del_allhflags(HDR *hdr_p)
{	hdr_p->wFlags&=F_PRIVATE;
}
                     
void del_mailerfields(long win)
{	wi[win].hdr->lMsgidcrc=
	wi[win].hdr->lReplycrc=0;
	wi[win].hdr->XFlags=
	wi[win].hdr->MFlags=0;
#if !defined(__NEWFIDO__)
	wi[win].hdr->free6=
	wi[win].hdr->wProcessed=0;
#endif
}

byte *ifna_date(void)
{	long sec_now;
	struct tm *tm_now;
	byte s[MAX_STR];
	time(&sec_now);
	tm_now=localtime(&sec_now);
	tm_now->tm_year %= 100;											/* Y2K */
	strftime(s,30,"%d %b %y  %H:%M:%S",tm_now);
	return s;
}	

word store_line(long win, word l, byte *quote, byte *inhalt, word add_blank, word add_lf)
{	wi[win].lines[l]=(char *)calloc(1,LINELEN);
	if (wi[win].lines[l]!=NULL)
	{	if (quote!=NULL)
		{	if (strlen(quote)==0L)
			{	if (*inhalt!=' ' && add_blank)
					strcpy(wi[win].lines[l]," ");
			}
			else
				strcpy(wi[win].lines[l],quote);
		}
		strcat(wi[win].lines[l],inhalt);

		if (add_lf && strchr(wi[win].lines[l],'\n')==NULL)
			strcat(wi[win].lines[l],"\n");
		if (*wi[win].lines[l]==EOS)
			strcpy(wi[win].lines[l],"\n");
		return TRUE;
	}
	else
	{	nomemalert(0);
		return FALSE;
	}
}

word mon2int(byte *m)
{	strupr(m);
	if (!strcmp(m,"JAN"))				return 0;
	else if (!strcmp(m,"FEB"))	return 1;	
	else if (!strcmp(m,"MAR"))	return 2;	
	else if (!strcmp(m,"APR"))	return 3;	
	else if (!strcmp(m,"MAY"))	return 4;	
	else if (!strcmp(m,"JUN"))	return 5;	
	else if (!strcmp(m,"JUL"))	return 6;	
	else if (!strcmp(m,"AUG"))	return 7;	
	else if (!strcmp(m,"SEP"))	return 8;	
	else if (!strcmp(m,"OCT"))	return 9;	
	else if (!strcmp(m,"NOV"))	return 10;	
	else if (!strcmp(m,"DEC"))	return 11;	
	else 												return 0;
}

void conv_msgtime(byte *d, byte *s)
{	word ztag, zmonat;
	byte tag[4], monat[10], jahr[6];
	byte *cp;

	while (!isdigit(*s) && *s) s++;
	strcpy(tag,"00");
	strcpy(monat,"00");
	strcpy(jahr,"00");
	if ( (cp=strtok(s," ")) != NULL ) 
		if (strlen(cp)<=2)
			strcpy(tag,cp);
	if ( (cp=strtok(NULL," ")) != NULL )
		if (strlen(cp)<=3)
			strcpy(monat,cp);	
	if ( (cp=strtok(NULL," ")) != NULL )
		if (strlen(cp)<=2)
			strcpy(jahr,cp);
	ztag=atoi(tag);	
	zmonat=mon2int(monat)+1;
	sprintf(d,"%02d.%02d.%2.2s",ztag,zmonat,jahr);	
}

void add_subjre(long win)
{	byte tmp[MAX_STR],*cp;
	int i;
	if (opt.addre & O_ADDRE)
	{	cp=wi[win].hdr->sSubject;

		if (!strnicmp(cp,"Re:",3))
		{	if (opt.addre & O_ADDREN)
			{	strcpy(tmp,"Re^2: ");
				strcat(tmp,kap_re(cp));
			}
			else
				strcpy(tmp,cp);
		}
		else if (!strnicmp(cp,"Re^",3))
		{	
			i=atoi(cp+3)+1;
			sprintf(tmp,"Re^%u: %s",i,kap_re(cp));
		}
		else
		{
			strcpy(tmp,"Re: ");
			strcat(tmp,cp);
		}
		tmp[71]=EOS;
		strcpy(wi[win].hdr->sSubject,tmp);
	}
}

void get_qname(long win, byte *q_name)
{	byte name[MAX_STR],quote[MAX_STR];
	byte *cp, *qp;
	
	if (opt.quote & O_QUOTE_XY)
	{	cp = wi[win].hdr->sTo;
		while (*cp==' ') cp++;
		strncpy(name,cp,MAX_STR);
		name[MAX_STR-1]=EOS;

		if ((cp=strchr(name,'%'))!=NULL) *cp=EOS;
		if ((cp=strchr(name,'@'))!=NULL) *cp=EOS;

		cp=name;
		while(*cp)
		{	if (*cp=='_' || *cp=='.' || *cp=='\"') *cp=' ';
			if (*cp>127) *cp='-';		/* Sonderzeichen ausblenden */
			if (!isspace(*cp) && !isalpha(*cp) && *cp!='-')
			{ *cp=EOS;
				break;
			}
			cp++;
		}

		cp=name;
		while (*cp==' ') cp++;
		qp=quote;
		while(*cp)
		{	*qp++=*cp++;
			while(*cp && *cp!=' ') cp++;
			while(*cp && *cp==' ') cp++;
		}
		*qp=EOS;
		
		qp=quote;
		while (*qp)
		{	if (!isalpha(*qp))
				strcpy(qp,qp+1);
			else
				qp++;
		}

		sprintf(q_name," %s> ",quote);
	}
	else
		strcpy(q_name," > ");
}

void no_blanks(byte *s)
{	byte *nul;
	
	nul=strchr(s,EOS);
	nul--;
	while (*nul==' ')
		nul--;
	*(nul+1)=EOS;	
}

void store_msghdr(long win)
{	byte *cp;
	byte tmp[MAX_STR];

	*loc_from=*loc_From=*loc_to=*loc_To=*loc_date=*loc_Date=*loc_zeit=
	*loc_subject=*loc_cfrom=*loc_cto=EOS;

	strcpy(loc_subject,kap_re(wi[win].hdr->sSubject));

	strcpy(loc_cfrom,wi[win].hdr->sFrom);
	strcpy(tmp,wi[win].hdr->sFrom);
	cp=strchr(tmp,' ');
	if (cp==NULL)
	{	strcpy(loc_from,tmp);
	}
	else
	{	*cp=EOS;
		strcpy(loc_from,tmp);
		cp++;
		strcpy(tmp,cp);
		if ((cp=strchr(tmp,'%'))!=NULL) *cp=EOS;
		if ((cp=strchr(tmp,'@'))!=NULL) *cp=EOS;
		cp=strchr(tmp,EOS);
		while(cp>tmp && *(cp-1)==' ') cp--;
		*cp=EOS;
		strcpy(loc_From,tmp);
	}

	strcpy(loc_cto,wi[win].hdr->sTo);
	strcpy(tmp,wi[win].hdr->sTo);
	cp=strchr(tmp,' ');
	if (cp==NULL)
	{	strcpy(loc_to,tmp);
	}
	else
	{	*cp=EOS;
		strcpy(loc_to,tmp);
		cp++;
		strcpy(tmp,cp);
		if ((cp=strchr(tmp,'%'))!=NULL) *cp=EOS;
		if ((cp=strchr(tmp,'@'))!=NULL) *cp=EOS;
		cp=strchr(tmp,EOS);
		while(cp>tmp && *(cp-1)==' ') cp--;
		*cp=EOS;
		strcpy(loc_To,tmp);
	}

	strcpy(tmp,skip_blanks(wi[win].hdr->sDate));
	if ((cp=strchr(tmp,':'))!=NULL)
	{
		while (isdigit(*(cp-1)))
			cp--;
		strcpy(loc_Date, cp);	
		
		cp--;
		*cp=EOS;
		cut_endspc(tmp);
		strcpy(loc_date,tmp);
	}
	strcpy(tmp,wi[win].hdr->sDate);
	conv_msgtime(loc_zeit, tmp);
	get_origaddr(win,loc_address);
}

word is_sysop(byte *from)
{	word i;
	for (i=0;i<=akasysopcount;i++)
		if (!strcmp(akasysop[i],from))
			return(TRUE);
	return(FALSE);
}

word find_nickname(byte *fullname,byte *area)
{	word i;
	test_registered();
	if (!is_registered)
		return(0);
	for (i=0;i<nicknamecnt;i++)
	{	if (!strcmp(fullname,nicknames[i][1]))
		{	if (nicknames[i][2])
			{	if (!stricmp(area,nicknames[i][2]))
					return(i+1);
			}
			else
				return(i+1);
		}
	}
	return(0);
}

void convert_percent(byte *free, word area, word fkey)
{	byte tmp[1024],tmp2[1024],*cp,*dest;
	word i;

	if (strchr(free,'%'))			/* Gibt es ein % Åberhaupt? */
	{
		strcpy(tmp,free);
		dest=free;
		*dest=EOS;
		cp=tmp;
		while (*cp)
		{
			if (*cp=='%' && cp[1]!=EOS)
			{
				switch( cp[1] )
				{	case 'f':	strcpy(dest,loc_from);		break;
					case 'F':	strcpy(dest,loc_From);		break;
					case 'o': strcpy(dest,loc_cfrom);		break;
					case 'O': strcpy(dest,loc_cto);			break;
					case 't':	strcpy(dest,loc_to);			break;
					case 'T':	strcpy(dest,loc_To);			break;
					case 'i':	i=find_nickname(loc_cfrom,gl_area[area].name);
										if (i) strcpy(dest,nicknames[i-1][0]);
										else	 strcpy(dest,loc_from);
										break;
					case 'I':	i=find_nickname(loc_cto,gl_area[area].name);
										if (i) strcpy(dest,nicknames[i-1][0]);
										else	 strcpy(dest,loc_to);
										break;
					case 'd':	strcpy(dest,loc_date);		break;
					case 'D':	strcpy(dest,loc_Date);		break;
					case 'z':	strcpy(dest,loc_zeit);		break;
					case 'a':	strcpy(dest,loc_address);	break;
					case 's': strcpy(dest,loc_subject); break;
					case 'n':	strcpy(dest,gl_area[area].name);	break;
					case 'v':	strcpy(dest,version);			break;
					case '%': strcpy(dest,"%");					break;
					default:	if (fkey &&
												cp[1]>='0' && cp[1]<='4' &&
												cp[2]>='0' && cp[2]<='9')
										{	i = (cp[1]-'0')*10 + (cp[2]-'0');
											if (i>=1 && i<=20)
											{	strcpy(tmp2,gl_fkeys[i-1]);
												convert_percent(tmp2,area,FALSE);
												strcpy(dest,tmp2);
											}
											else if (i>=30 && i<=35 ||
															 i>=40 && i<=43)
											{	switch(i)
												{	case 30	:
													case 40	:	if (is_sysop(loc_cfrom))	strcpy(dest,i==30 ? "Ich" : "I");
																		else											strcpy(dest,loc_cfrom);
																		break;
													case 31	:	
													case 41	:	if (is_sysop(loc_cfrom))	strcpy(dest,i==31 ? "ich" : "I");
																		else											strcpy(dest,loc_cfrom);
																		break;
													case 32	:	
													case 42	:	if (is_sysop(loc_cto))		strcpy(dest,i==32 ? "Mich" : "Me");
																		else											strcpy(dest,loc_cto);
																		break;
													case 33	:	
													case 43	:	if (is_sysop(loc_cto))		strcpy(dest,i==33 ? "mich" : "me");
																		else											strcpy(dest,loc_cto);
																		break;
													case 34	:	if (is_sysop(loc_cto))		strcpy(dest,"Mir");
																		else											strcpy(dest,loc_cto);
																		break;
													case 35	:	if (is_sysop(loc_cto))		strcpy(dest,"mir");
																		else											strcpy(dest,loc_cto);
																		break;
												}
											}
											else
											{	strcpy(dest,"<Wrong FKey>");
											}
											cp++;
										}
										else
											strcpy(dest,"<Wrong Code>");
				}
				cp+=2;
				dest=strchr(dest,EOS);
			}
			else if (*cp=='%')
			{	strcpy(dest,"<Wrong Code>");
				dest=strchr(dest,EOS);
				cp++;
			}
			else
				*dest++=*cp++;
		}
		*dest=EOS;
	}
}


/* Hole free editable quote topline */

static void get_freetopline(byte *s, word native_fl, word area, word to_other_area)
{	
	if (native_fl) /* deutsch */
	{
		if (area && to_other_area && *opt.nat2top)	strcpy(s,opt.nat2top);
		else																				strcpy(s,opt.nattop);
	}
	else /* english */
	{
		if (area && to_other_area && *opt.eng2top)	strcpy(s,opt.eng2top);
		else																				strcpy(s,opt.engtop);
	}
	convert_percent(s, area, TRUE);
}

word add_uucp_to_line(long win,word lneu)
{	byte tmp[256],*cp;
	if (opt.usereplyaddr && is_netmail(wi[win].aktarea))
	{	if (!strncmp(wi[win].dup_replyaddr,"\001REPLYADDR",10))
		{	strcpy(tmp,"To: ");
			if ((cp=strchr(wi[win].dup_replyaddr+10,'<'))!=NULL &&
					(strchr(cp,'>')!=NULL))
			{	cp++;
				strcat(tmp,cp);
				cp=strchr(tmp,'>');
				*cp=EOS;
			}
			else
			{	cp=wi[win].dup_replyaddr+10;
				if (*cp==':') cp++;
				strcat(tmp,skip_blanks(cp));
			}
			if (strchr(tmp,'\n')==NULL)
				strcat(tmp,"\n");
			if (ins_line(win,lneu, tmp,FALSE,FALSE)==TRUE)	lneu++;
			if (ins_line(win,lneu,"\n",FALSE,FALSE)==TRUE)	lneu++;
		}
	}
	return(lneu);
}

word quote_wrap(long win, byte *q_name, word area, word to_other_area)
{	long maxllen;
	word qlen;
	word ret,i;
	word lnr,lneu;
	word newline;
	word innach,inmsg,infree;
	byte *cp, *ptr, *nul;
	byte newqt[128],oldqt[128];
	byte qline[256],tmp[1024],tmp2[256];
	byte norm_time[20];

/*	byte *msglines[MAX_LINES]; */
	byte **msglines;
	
	msglines=(byte **)calloc(MAX_LINES,sizeof(byte *));
	if (!msglines)
	{	LangAlert(MSG_OUTOFMEMORY);
		return FALSE;
	}

	ret=FALSE;
	
	if (wi[win].lines[0]!=NULL)
	{
		biene();
		ret=TRUE;
		lnr=lneu=0;
		maxllen=(long)(opt.maxedcol-1);

		memcpy(msglines,wi[win].lines,wi[win].lzahl*sizeof(byte *));

		/* Ermittel ob deutsch/englisch Topline */
		innach=
		inmsg =
		infree=FALSE;
		if (opt.quote & O_QUOTE_DET)
		{	if ((cp=strrchr(wi[win].areaname,'.'))!=NULL)
			{	cp++;
				if ( !strnicmp(cp,"GER",3) || !strnicmp(cp,"AUS",3) || !strnicmp(cp,"CH",2) )
					innach=TRUE;
				else
					inmsg=TRUE;	
			}
			else
				inmsg=TRUE;
		}
		if (opt.quote & O_QUOTE_NAT) innach=TRUE;
		if (opt.quote & O_QUOTE_MSG) inmsg=TRUE;

/* innach=TRUE: Deutscher Quote		*/
/* inmsg =TRUE: Englischer Quote	*/
		
		if (to_other_area && !innach && !inmsg)
			inmsg=TRUE;	/* Bei "quote to netmail/area", Zeile benutzen */
			
		/* Toggle Topline wenn mit anderer Tastenkombination gestartet */

#if 0
		if (gl_otherquotefl)
		{	if (innach)	{	inmsg=TRUE;
										innach=FALSE;
									}
			else				{	inmsg=FALSE;
										innach=TRUE;
									}
		}
		gl_otherquotefl=FALSE;
#endif

/* infree: Freier Quote						*/

		infree=opt.quote & O_QUOTE_FRE;

		if (gl_area[area].quotetype!=QUOTE_UNSET)
		{
			switch(gl_area[area].quotetype)
			{	case QUOTE_NON:
					inmsg=innach=infree=FALSE;
					break;
				case QUOTE_MSG:
					inmsg=TRUE;
					innach=FALSE;
					infree=FALSE;
					break;	
				case QUOTE_NACHRICHT:
					inmsg=FALSE;
					innach=TRUE;
					infree=FALSE;
					break;	
				case QUOTE_NATIVE:
					inmsg=FALSE;
					innach=TRUE;
					infree=TRUE;
					break;	
				case QUOTE_ENGLISH:
					inmsg=TRUE;
					innach=FALSE;
					infree=TRUE;
					break;	
			}
		}

#if 1
		if (gl_otherquotefl)
		{	gl_otherquotefl=inmsg;
			inmsg=innach;
			innach=gl_otherquotefl;
			gl_otherquotefl=FALSE;
		}
#endif

		memset(wi[win].lines,0,sizeof(wi[win].lines));
		wi[win].lzahl=0;
		
		if (!to_other_area && !strncmp(msglines[0],"\003AREA ",6))
			if ((ret=ins_line(win,lneu,msglines[0],FALSE,FALSE))==TRUE)
			{	lneu++;
				lnr++;
			}
		
		lneu=add_uucp_to_line(win,lneu);

		if (infree && innach && !*opt.nattop)	infree=FALSE;
		if (infree && inmsg  && !*opt.engtop)	infree=FALSE;

		if ((innach || inmsg) &&
				!((opt.quote & O_QUOTE_NOTOP) && is_netmail(wi[win].aktarea)) &&
				!(gl_area[wi[win].aktarea].flag & AR_NOTOPLINE) )
		{	
			no_blanks(wi[win].hdr->sTo);
			no_blanks(old_msgtime);
			cp=old_msgtime;
			while (*cp==' ') cp++;
			strcpy(old_msgtime,cp);

/*
**	"Kommentar zu"-Zeile nur dann erzeugen, wenn:
**		- Mail aus Echoarea kommt (area!=0)
**		& Mail in eine andere Area geschoben wird (to_other_area)
**		& Mail aus einem MausEcho kam (AR_MAUS)
*/
			if (!is_netmail(area) && to_other_area && gl_area[area].flag & AR_MAUS)
			{	strcpy(qline,wi[win].dup_replyid);
				cp=strchr(qline,' ');
				if (cp!=NULL)
				{	cp++;
					nul=strchr(cp,LF);
					if (nul!=NULL)
						*nul=EOS;
					strcpy(tmp2,gl_area[area].name);
					strupr(tmp2);
					sprintf(tmp,"Kommentar zu %s in der Gruppe %s",cp,tmp2);
					if ((ret=ins_line(win,lneu, tmp,FALSE,FALSE))==TRUE) lneu++;
					if ((ret=ins_line(win,lneu,"\n",FALSE,FALSE))==TRUE) lneu++;
				}
			}

			if (gl_area[area].flag & AR_OWNTOPLINE && !to_other_area)
			{	
				strcpy(tmp,gl_area[area].topline);
				for(cp=tmp;*cp;cp++)
					if (*cp=='|') *cp='\r';
				convert_percent(tmp, area, TRUE);
			}
			else if (infree)
			{	
				get_freetopline(tmp, innach, area, to_other_area);
			}
			else
			{
				conv_msgtime(norm_time,old_msgtime);
				cp=norm_time;
				
				if (inmsg)
				{	sprintf(tmp2," in area '%s'",gl_area[area].name);

					if (strlen(loc_address)>0L)
						sprintf(tmp,"In a message of <%s>, %s (%s) writes%s:\n",
															cp,wi[win].hdr->sTo,loc_address,
															to_other_area ? tmp2 : "");
					else
						sprintf(tmp,"In a message of <%s>, %s writes%s:\n",
															cp,wi[win].hdr->sTo,
															to_other_area ? tmp2 : "");
				}
				else
				{	sprintf(tmp2," in Area '%s'",gl_area[area].name);

					if (strlen(loc_address)>0L)
						sprintf(tmp,"In einer Nachricht vom %s schrieb %s (%s)%s:\n",
															cp,wi[win].hdr->sTo,loc_address,
															to_other_area ? tmp2 : "");
					else	
						sprintf(tmp,"In einer Nachricht vom %s schrieb %s%s:\n",
															cp,wi[win].hdr->sTo,
															to_other_area ? tmp2 : "");
				}
			}


			cp=strchr(tmp,0);								/* Ende suchen */
			while (cp>tmp && (*(cp-1)=='\n' || *(cp-1)==' '))
				cp--;
			*cp=EOS;												/* ' ' und '\n' abschneiden */

			while (cp>tmp)	/* ssl 080195 war vorher 78, quote in schleife */
			{	
				if (cp-tmp >= opt.maxedcol)
				{	while (cp-tmp>=opt.maxedcol || (*cp!=' ' && *cp!='\r') )
						cp--;
				}
				else
				{	cp--;
				}

				i=*(++cp);
				*cp=EOS;

				if ((ptr=strchr(tmp,'\r'))!=NULL)
				{	*cp=i;					/* rÅckgÑngig machen		*/
					cp=ptr;					/* Wo steht das '\r'?		*/
					*cp='\n';				/* ersetzen durch '\n'	*/
					i=*(++cp);			/* Zeichen nach '\r' alias '\n' merken */
					*cp=EOS;				/* abschneiden					*/
				}

				if ((ret=ins_line(win,lneu,tmp,FALSE,FALSE))==TRUE)	lneu++;

				if (!ret)
				{	LangAlert(MSG_QUOTINGCANCELLED);
					break;
				}

				*cp=i;
				strcpy(tmp,cp);
				cp=strchr(tmp,0);
			}
			if ((ret=ins_line(win,lneu,"\n",FALSE,FALSE))==TRUE) lneu++;
		}

		if (opt.quote & O_QUOTE_MSET)
		{	wi[win].lnr=lneu;
			wi[win].cy=TOPMSG_LINE+lneu;
		}

		*tmp=EOS;
		newline=TRUE;
		while (msglines[lnr]!=NULL && ret)
		{
			if (newline)
			{
				if (strlen(tmp)<maxllen && strchr(tmp,'\n')==NULL)
				{
					if (*tmp==EOS)
						cp=msglines[lnr];
					else
						cp=skip_blanks(msglines[lnr]);
					strcat(tmp,cp);
					newline=FALSE;
				}
			}
			
			cp=getquote(tmp,qline);
			if (*qline)
			{
				qlen=(word)strlen(qline);
				strcat(qline,cp);		/* add end of line */
			}
			else	/* no quote */
			{
				*newqt=EOS;
				if (msglines[lnr]!=NULL)
					getquote(msglines[lnr],qline);
				if (msglines[lnr+1]!=NULL)
					cp=getquote(msglines[lnr+1],newqt);
				if (*qline)
				{
					qlen=(word)strlen(qline);
					i=strcmp(newqt,qline);
					strcat(qline,tmp);
					if (i==0 && strchr(qline,'\n')==NULL)
					{
						addendspc(qline);
						while (*cp==' ')
							cp++;
						strcat(qline,cp);
						lnr++;
					}
				}
				else
				{
					strcpy(qline,q_name);	/* add my quote */

					if (opt.quote & O_QUOTE_NCOMP)
					{	qlen=(word)strlen(qline);
						strcat(qline,tmp);	/* add line */
					}
					else
					{	qlen=(word)strlen(qline);
						i=qlen;					/* skip blanks for quote */
						ptr=tmp;
						while (--i>=0 && *ptr==' ') ptr++;
						strcat(qline,ptr);	/* add line */
					}
				}
			}
			cp=strchr(qline,EOS);
			if (cp>=qline+maxllen)
			{
				cp=qline+maxllen;
				ptr=qline+qlen;
				while ((*cp!=' ' && *cp!='-' && *cp!=',') && cp>ptr)
					cp--;
				if (cp>ptr)	/* found space? */
				{
					strcpy(tmp,++cp);
					if ((ptr=strchr(tmp,'\n'))!=NULL)	/* no \n */
						*ptr=' ';
					addendspc(tmp);
				}
				else
				{
					cp=qline+maxllen;
					strcpy(tmp,cp);
					addnewl(tmp);
				}
				*cp=EOS;
			}
			else
				*tmp=EOS;
			
			if ((ret=ins_line(win,lneu,qline,FALSE,FALSE))==TRUE) lneu++;
			
			if (!newline)
			{
				if (*tmp==EOS)
				{
					newline=TRUE;
					lnr++;
				}
				else if (strlen(tmp)<maxllen)
				{
					if (msglines[lnr+1]!=NULL)
					{
						getquote(msglines[lnr],oldqt);
						getquote(msglines[lnr+1],newqt);
						if (*newqt==EOS && *oldqt==EOS)
						{
							newline=TRUE;
							lnr++;
						}
					}
				}
			}
		}	/* while */

		if (!ret)
		{
			free_allmsglines(win);

			i=0;
			while (msglines[i]!=NULL)
			{
				wi[win].lines[i]=msglines[i];
				i++;
			}
			wi[win].lzahl=i;
		}
		else
		{
			for (i=0;i<MAX_LINES;i++)
				if (msglines[i]!=NULL)
					free(msglines[i]);
		}
		mouse_normal();
	}
	free(msglines);
	return(ret);
}

void addendspc(byte *line)
{	byte *cp;
	if (line!=NULL)
	{	cp=strchr(line,EOS)-1L;
		if (cp>=line && *cp!='\n' && *cp!=' ' && *cp!='-')
		{	cp++;
			*cp++=' ';
			*cp=EOS;
		}
	}
}

void addnewl(byte *line)
{	byte *cp;
	if (line!=NULL)
	{	cp=strchr(line,EOS);
		if (*(cp-1)!='\n')
		{	*cp++='\n';
			*cp=EOS;
		}
	}
}

byte *getquote(byte *line, byte *quote)
{	byte *cp;

	*quote=EOS;
	cp=strchr(line,'>');
	if (cp!=NULL && cp-line < 6
#if 0
		&& (*(cp-1)==' ' || isalpha(*(cp-1)))
#endif
										)	/* found qoute? */
	{
		while (*(cp+1)=='>') cp++;
		*cp=EOS;
		
		if (*line!=' ')
		{	strcpy(quote," ");
			strcat(quote,line);
		}
		else
			strcpy(quote,line);	
		
		*cp='>';
		strcat(quote,">> ");		/* multiple '>' */
		while (*cp=='>') cp++;
		while (*cp==' ') cp++;
		return(cp);
	}
	else
		return(line);
}

void get_origaddr(long win, byte *addr)
{	word i,ori_index;

	*addr=EOS;
	if (!is_netmail(wi[win].aktarea))
	{	i=ori_index=0;
		while (wi[win].lines[i]!=NULL)
		{	if (!strncmp(wi[win].lines[i]," * Origin:",10))
			{	ori_index=i;
				break;
			}
			i++;
		}		
		if (ori_index!=0)
		{	if (!copy_origaddr(addr,wi[win].lines[ori_index]))
				copy_origaddr(addr,wi[win].lines[ori_index+1]);
		}
		else
		{	strcpy(addr,mergeaddr(wi[win].hdr->wFromZone, wi[win].hdr->wFromNet,
														wi[win].hdr->wFromNode, wi[win].hdr->wFromPoint,NULL));
		}
	}
	else
	{	strcpy(addr,mergeaddr(wi[win].hdr->wFromZone, wi[win].hdr->wFromNet,
													wi[win].hdr->wFromNode, wi[win].hdr->wFromPoint,NULL));
	}
}

word copy_origaddr(byte *addr, byte *line)
{	byte *cp;
	
	if (line!=NULL)
		cp=strrchr(line,':');
	else
	{	*addr=EOS;
		return FALSE;
	}

	if (cp!=NULL && isdigit(*(--cp)))
	{
		while (isdigit(*(cp-1))) cp--;
		while (*cp!=')' && *cp!=' ' && *cp!='\n' && *cp!=EOS)
			*addr++=*cp++;
		*addr=EOS;
		return TRUE;
	}
	else
	{	*addr=EOS;
		return FALSE;
	}
}

void set_orignode(HDR *hdr,word area)
{	/* ###ADDR### */
	if ((gl_area[area].flag & AR_FIXEDADDRESS && !is_netmail(area)) ||
			(opt.addrcornet												&&  is_netmail(area)) )
	{
		hdr->wFromZone=
		hdr->wFromNet =
		hdr->wFromNode=
		hdr->wFromPoint=0;
		hdr->free6=0;
	}
	else
	{
		hdr->wFromZone=addr[0].zone;
		hdr->wFromNet  =addr[0].net;
		hdr->wFromNode =addr[0].node;
		hdr->wFromPoint=addr[0].point;

		hdr->free6=0 /*1*/;
	}
}
