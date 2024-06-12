#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "language.h"

static void read_areabbs(word mode);

extern byte autor3[];

char *skip_blanks(char *string)
{	while (*string && isspace(*string)) ++string;
	return (string);
}

void cut_endspc(byte *string)
{	byte *cp;
	cp=strchr(string,EOS);
	while (isspace(*(--cp))) *cp=EOS;
}

void add_bslash(byte *path)
{	byte *cp;
	cp=strchr(path,EOS);
	if (*(cp-1)!='\\')
	{	*cp++='\\';
		*cp=EOS;
	}
}

void kill_lblank(byte *s, word one_word)
{	byte *cp;
	if (one_word)
	{	cp=strchr(s,' ');
		if (cp==NULL)	cp=strchr(s,'\t');
		if (cp!=NULL)	*cp=EOS;
	}
	else
	{	cp=strchr(s,';');
		if (cp==NULL)	cp=strchr(s,EOS);
		while (*(cp-1)==' ' || *(cp-1)=='\t')	cp--;
		*cp=EOS;
	}	
}

void ctl_string(byte *buf, byte *string, word kill_bl)
{	byte *start;
	byte *cp;
	start=skip_blanks(string);
	if ((cp=strchr(start,'\n'))!=NULL)	*cp=EOS;
	strcpy(buf,start);
	if (cp!=NULL)	*cp='\n';
	kill_lblank(buf, kill_bl);	
}

void speicher_freigeben(void)
{	word i;
	if (gl_area)
	{	for (i=0;i<gl_arzahl;i++)
		{	if (gl_area[i].name) 				free(gl_area[i].name);
			if (gl_area[i].path) 				free(gl_area[i].path);
			if (gl_area[i].desc) 				free(gl_area[i].desc);
			if (gl_area[i].rndfootpath) free(gl_area[i].rndfootpath);
			if (gl_area[i].followup) 		free(gl_area[i].followup);
			if (gl_area[i].topline) 		free(gl_area[i].topline);
			if (gl_area[i].domain)	 		free(gl_area[i].domain);
		}
		free(gl_area);
	}
	for (i=0;i<nicknamecnt;i++)
	{	if (nicknames[i][0]) free(nicknames[i][0]);
		if (nicknames[i][1]) free(nicknames[i][1]);
		if (nicknames[i][2]) free(nicknames[i][2]);
	}
}

AREAS *gl_area;
uword MAX_AREAS;

/*
**	Mode & 1 = Ignore Invisible
*/
word init_areasbbs(word mode)
{	byte line_buf[512];
	byte *bufanf;
	FILE *bbsfp;
	word gl_maxarea=0;
	word skiparea=FALSE;

	sprintf( line_buf, "%sareas.bbs", gl_areasbbspath);
	if ((bbsfp=fopen(line_buf, "r"))==NULL) 
	{	return 0;
	}
	else
	{	gl_maxarea=0;
		while (fgets(line_buf,512,bbsfp)!=NULL)
		{	bufanf=skip_blanks(line_buf);
			if (*bufanf==';')
			{	continue;
			}
			else if (*bufanf=='-')
			{	if (!strnicmp(bufanf,"Passthrough",11))
					skiparea=TRUE;
				else if (!strnicmp(bufanf,"Invisible",9) && !(mode & 1))
					skiparea=TRUE;
				else if (!strnicmp(bufanf,"Visible",7))
					skiparea=FALSE;
				continue;
			}
			else if (strlen(bufanf)<3)
			{	continue;
			}
			else if (!skiparea)
			{	gl_maxarea++;
				skiparea=FALSE;
			}
		}
		fclose(bbsfp);
	}
	return(gl_maxarea);
}

word config_init(char *filename)
{	FILE *cfgfp;                    /* filepointer for config-file */
	uword i;
	word mode=0;
	byte buffer[256];               /* buffer for 1 line */
	byte *p;
	byte *cmd;
	byte tmp[MAX_STR];
	byte longtmp[256];
	byte netmail[MAX_STR];

	addr[0].zone = addr[0].net = addr[0].node =
	addr[0].point = 0; /* addr[0].ptnet = 0; */

	*addr[0].domain	=
	*gl_echofile		=
	*netmail				= EOS;

	for (i=0;i<10;i++)
	{	if (gl_shell[i]!=NULL)
			free(gl_shell[i]);
		if (gl_shellcmd[i]!=NULL)
			free(gl_shellcmd[i]);
	}
	memset(gl_shell,0,sizeof(gl_shell));
	memset(gl_shellcmd,0,sizeof(gl_shellcmd));

	strcpy(gl_ptowner,"Unknown user");
	gl_nodelisttype=
	addrcount			=
	domaincount		=
	akasysopcount	=
	pseudonymsysopcount = -1;

	if ((cfgfp=fopen(filename, "r"))==NULL)        /* open config-file */
	{	sprintf(longtmp,msgtxt[MSG_CONFIGINITFAILED].text,filename);
		FormAlert(msgtxt[MSG_CONFIGINITFAILED].defbut,
							msgtxt[MSG_CONFIGINITFAILED].icon,
							longtmp,
							msgtxt[MSG_CONFIGINITFAILED].button);
		return FALSE;                           /* not found, back */
	}

	while((fgets(buffer, 255, cfgfp))!=NULL)       /* read a line */
	{	p=skip_blanks(buffer);
		p=strtok(p," \t\n\r");
		cmd=strtok(NULL," \t\n\r");
		if (p==NULL)	continue;
		if (*p==';')	continue;
		
		if (!stricmp(p,"Sysop"))
		{	strcpy(tmp,cmd);
			while ((cmd=strtok(NULL," \t\n\r"))!=NULL)
			{	if (isascii(*cmd))
				{	strcat(tmp," ");
					strcat(tmp,cmd);
				}
				else
					break;
			}
			ctl_string(longtmp,tmp,FALSE); longtmp[36]=EOS;
			strcpy(gl_ptowner,longtmp);
			strcpy(akasysop[0],gl_ptowner);
			akasysopcount=0;
		}

		else if (!stricmp(p,"Application") ||
						 !stricmp(p,"AkaSysop") )
		{	if (p[1]=='p' || p[1]=='P')
			{	if (stricmp(cmd,"AkaSysop")) continue;
				cmd=strtok(NULL," \t\n\r");
				if (!cmd) continue;
			}
			strcpy(tmp,cmd);
			while ((cmd=strtok(NULL," \t\n\r"))!=NULL)
			{	if (isascii(*cmd))
				{	strcat(tmp," ");
					strcat(tmp,cmd);
				}
				else
					break;
			}
			if (akasysopcount<0)
			{	LangAlert(MSG_FIRSTSYSOP);
				continue;
			}
			ctl_string(longtmp,tmp,FALSE); longtmp[36]=EOS;
			akasysopcount++;
			if (akasysopcount>=MAX_SYSOP)
			{	LangAlert(MSG_TOMANYSYSOPS);
				akasysopcount--;
				continue;
			}
#if 0   /* AKASYSOP limit removed SSL 300198 */
			if (strcmp(autor3,gl_ptowner))
				if (strncmp(akasysop[0],longtmp,strlen(akasysop[0])))
				{	LangAlert(MSG_AKASYSOPIGNORED);
					akasysopcount--;
					continue;
				}
#endif
			strcpy(akasysop[akasysopcount],longtmp);
		}

		else if (!stricmp(p,"Address"))
		{	addrcount++;
			if (addrcount>=MAX_ADDRESSES)
			{	LangAlert(MSG_TOMANYADDR);
				addrcount--;
			}
			else
			{	addr[addrcount].zone = addr[addrcount].net=
				addr[addrcount].node = addr[addrcount].point=0;
				*addr[addrcount].domain = EOS;
				scanaddr(cmd,	&addr[addrcount].zone,
											&addr[addrcount].net,
											&addr[addrcount].node,
											&addr[addrcount].point,tmp);
				strncpy(addr[addrcount].domain,tmp,12); addr[addrcount].domain[12]=EOS;
			}
		}

		else if (!stricmp(p,"Domain"))
		{	domaincount++;
			if (domaincount>=MAX_DOMAINS)
			{	domaincount--;
			}
			else
			{	
				strncpy(gl_domain[domaincount][0],cmd,16);
				gl_domain[domaincount][0][16]=EOS;

				if ((cmd=strtok(NULL," \t\n\r"))!=NULL)
				{	strncpy(gl_domain[domaincount][1],cmd,16);
					gl_domain[domaincount][1][16]=EOS;
				}
				else
					domaincount--;
			}
		}

		else if (!stricmp(p,"AREASPath"))
		{	strupr(cmd);
			ctl_string(gl_areasbbspath,cmd,TRUE);
			add_bslash(gl_areasbbspath);
		}
		else if (!stricmp(p,"LEDNEWPath"))
		{	strupr(cmd);
			ctl_string(gl_lednewpath,cmd,TRUE);
			add_bslash(gl_lednewpath);
		}

		else if (!stricmp(p,"Logfile"))
		{	strupr(cmd);
			ctl_string(gl_logfile,cmd,TRUE);
		}

		else if (!stricmp(p,"Nodelist"))
		{	strupr(cmd);
			ctl_string(gl_nodelistpath,cmd,TRUE);
			add_bslash(gl_nodelistpath);
		}
		else if (!stricmp(p,"BTNC"))
		{	gl_nodelisttype=BTNC;
		}
		else if (!stricmp(p,"Outbound36"))
		{	gl_options|=OUTBOUND36;
		}
		else if (!stricmp(p,"CallExporter"))
		{	strupr(cmd);
			ctl_string(tmp,cmd,TRUE);
			switch(atoi(tmp))
			{	case	0	:	gl_options|=CALLEXPORTER_AVALON;	break;
				case	1	:	gl_options|=CALLEXPORTER_SEMPER;	break;
			}
		}
		else if (!stricmp(p,"EchoList"))
		{	strupr(cmd);
			ctl_string(gl_echofile,cmd,TRUE);
		}

		else if (!stricmp(p,"Netmail"))
		{	strupr(cmd);
			ctl_string(netmail,cmd,TRUE);
			strupr(netmail);
			if (netmail[1]!=COLON)
			{	tmp[0]=(byte)(Dgetdrv()+'A');
				tmp[1]=COLON;
				tmp[2]=EOS;
				strcat(tmp,netmail);
				strcpy(netmail,tmp);
			}	
		}

		else if (!stricmp(p,"Hold"))
		{	strupr(cmd);
			ctl_string(gl_outpath,cmd,TRUE);
			add_bslash(gl_outpath);
		}

		else if (!stricmp(p,"Shell") && cmd!=NULL)
		{	i=atoi(cmd);
			if (i>=1 && i<=10 && gl_shell[i-1]==NULL && 
			   (cmd=strtok(NULL," \t\n\r"))!=NULL)
			{	gl_shell[i-1]=calloc(strlen(cmd)+2L,1);
				gl_shellcmd[i-1]=calloc(128L,1);
				if (gl_shell[i-1]!=NULL && gl_shellcmd[i-1]!=NULL)
				{	strcpy(gl_shell[i-1],cmd);
					while ((cmd=strtok(NULL," \t\n\r"))!=NULL)
					{	strcat(gl_shellcmd[i-1],cmd);
						strcat(gl_shellcmd[i-1]," ");
					}
				}
			}
		}
	}
	fclose(cfgfp);

	if (gl_nodelisttype!=BTNC)
		*gl_nodelistpath=EOS;

	if (addrcount<0)
	{	LangAlert(MSG_SPECIFYNODEADDR);
		return(FALSE);			/* ssl 181294: war vorher in read_areasbbs */
	}

	if (akasysopcount==-1)
	{	akasysopcount=0;
		strcpy(akasysop[0],"Unknown user");
	}

	if (Kbshift(-1) & 6)
		mode=1;		/* Ignore -Invisible */

	MAX_AREAS=init_areasbbs(mode)+4;		/* ssl 281294: vier sonstige Areas sollten reichen */
	gl_area = (AREAS *) calloc(MAX_AREAS,sizeof(AREAS));
	if (!gl_area)
	{	LangAlert(MSG_CONFMEMORY);
		return FALSE;
	}

	gl_mailarea=0;
	gl_arzahl=0;

	read_areabbs(mode);

	if (gl_arzahl==0)
	{	gl_arzahl=1;
		gl_area[0].path = strdup(netmail);
		gl_area[0].name = strdup("FidoNetmail");
	}
	if (!gl_area[0].path || !(*gl_area[0].path))
	{	LangAlert(MSG_UNKNOWNMAILPATH);
		return(FALSE);
	}

	return(TRUE);
}	

static void read_areabbs(word mode)
{	byte line_buf[512];
	byte *bufanf, *cp, *areapath, *areaname, *areanode;
	byte tmpbuf[MAX_STR];
	byte msgout[MAX_STR];
	byte aktdrive;
	byte tmp[MAX_STR];
	word line;
	word skiparea_fl=FALSE,pseudonym=FALSE;
	uword i,j;
	FILE *bbsfp,*out;

	line=0;

	aktdrive=(byte) (Dgetdrv()+'A');
	sprintf( line_buf, "%sAREAS.BBS", gl_areasbbspath);
	if ((bbsfp=fopen(line_buf, "r"))==NULL) 
		LangAlert(MSG_NOAREASBBS);
	else
	{	
		while (fgets(line_buf,512,bbsfp)!=NULL)
		{	line++;
			bufanf=skip_blanks(line_buf);

			if (*bufanf==';')
				continue;
			else
				break;
		}

		while (fgets(line_buf,512,bbsfp)!=NULL)
		{	line++;
			bufanf=skip_blanks(line_buf);
			
			switch (*bufanf)
			{	case ';'	:	break;
				case '-'	:	bufanf++;
										if ( !strnicmp(bufanf,"Desc ",5) )
										{	cp=skip_blanks(bufanf+5);
											if (cp)
											{	gl_area[gl_arzahl].desc = strdup(cp);
												if ((cp=strrchr(gl_area[gl_arzahl].desc,'\n'))!=NULL)
													*cp=EOS;
											}	
										}
										else if (!strnicmp(bufanf,"Pseudonym ",10) ||
														 !strnicmp(bufanf,"Name ",5) )
										{	if (*bufanf=='p' || *bufanf=='P')
												cp=skip_blanks(bufanf+10);
											else
												cp=skip_blanks(bufanf+5);
											cut_endspc(cp);
											pseudonymsysopcount++;
											if (pseudonymsysopcount>=MAX_PSEUDONYMS)
											{	pseudonymsysopcount--;
												continue;
											}
											strncpy(pseudonymsysop[pseudonymsysopcount],cp,39);
											pseudonymsysop[pseudonymsysopcount][39]=EOS;
											pseudonymarea[pseudonymsysopcount]=gl_arzahl;
										}
										else if ( !strnicmp(bufanf,"Netmail ",8) )
										{	ctl_string(tmp,bufanf+8,TRUE);
											if (*tmp)
												gl_area[gl_arzahl].domain = strdup(tmp);
											else
												gl_area[gl_arzahl].domain = NULL;
											gl_area[gl_arzahl].flag |= AR_NETMAIL;
										}
										else if ( !strnicmp(bufanf,"Footlinefile ",13) )
										{	ctl_string(tmp,bufanf+13,TRUE);
											if (*tmp)
												gl_area[gl_arzahl].rndfootpath = strdup(tmp);
										}
										else if ( !strnicmp(bufanf,"Topline ",8) )
										{	cp=skip_blanks(bufanf+8);
											if (cp)
											{	gl_area[gl_arzahl].topline = strdup(cp);
												if ((cp=strrchr(gl_area[gl_arzahl].topline,'\n'))!=NULL)
													*cp=EOS;
												gl_area[gl_arzahl].flag |= AR_OWNTOPLINE;
											}	
										}
										else if ( !strnicmp(bufanf,"FollowUp ",9) )
										{	ctl_string(tmp,bufanf+8,TRUE);
											sprintf(msgout,msgtxt[MSG_FOLLOWUPERROR].text,line,tmp);
											if (*tmp)
												gl_area[gl_arzahl].followup=strdup(tmp);
											else
												FormAlert(msgtxt[MSG_FOLLOWUPERROR].defbut,msgtxt[MSG_FOLLOWUPERROR].icon,msgout,msgtxt[MSG_FOLLOWUPERROR].button);
										}
										else if ( !strnicmp(bufanf,"UseCommentTree",14) )
										{	gl_area[gl_arzahl].flag |= AR_COMMENTTREEON;
										}
										else if ( !strnicmp(bufanf,"NoCommentTree",13) )
										{	gl_area[gl_arzahl].flag |= AR_COMMENTTREEOFF;
										}
										else if ( !strnicmp(bufanf,"NoExport",8) )
										{	gl_area[gl_arzahl].flag |= AR_NOEXPORT;
										}
										else if ( !strnicmp(bufanf,"NoTopline",9) )
										{	gl_area[gl_arzahl].flag |= AR_NOTOPLINE;
										}
										else if ( !strnicmp(bufanf,"ForcePVT",8) )
										{	gl_area[gl_arzahl].flag |= AR_FORCEPVT;
										}
										else if ( !strnicmp(bufanf,"Passthrough",11) )
										{	skiparea_fl=TRUE;
										}
										else if ( !strnicmp(bufanf,"Invisible",9) && !(mode & 1))
										{	skiparea_fl=TRUE;
										}
										else if ( !strnicmp(bufanf,"Visible",7) )
										{	skiparea_fl=FALSE;
										}
										else if ( !strnicmp(bufanf,"Maus",4) )
										{	gl_area[gl_arzahl].flag |= AR_MAUS|AR_UMLAUTE;
										}
										else if ( !strnicmp(bufanf,"FixedAddress",12) )
										{	gl_area[gl_arzahl].flag |= AR_FIXEDADDRESS;
										}
										else if ( !strnicmp(bufanf,"Umlaute",7) )
										{	gl_area[gl_arzahl].flag |= AR_UMLAUTE;
										}
										else if ( !strnicmp(bufanf,"Latin1small",11) )
										{	gl_area[gl_arzahl].flag |= AR_CONVERT_LAT1S;
										}
										else if ( !strnicmp(bufanf,"Latin1",6) )
										{	gl_area[gl_arzahl].flag |= AR_CONVERT_LAT1;
										}
										else if ( !strnicmp(bufanf,"NoNewEcho",9) )
										{	gl_area[gl_arzahl].flag |= AR_NONEWECHO;
										}
										else if ( !strnicmp(bufanf,"IgnoreNewMsgFlag",16) )
										{	gl_area[gl_arzahl].flag |= AR_IGNORENEWMSGFL;
										}
										else if ( !strnicmp(bufanf,"IgnoreUnreadFlag",16) )
										{	gl_area[gl_arzahl].flag |= AR_IGNOREUNREADFL;
										}
										else if ( !strnicmp(bufanf,"RandomFootline",14) )
										{	gl_area[gl_arzahl].flag |= AR_RANDOMTEARLINE;
										}
										else if ( !strnicmp(bufanf,"NoRandomFootline",16) )
										{	gl_area[gl_arzahl].flag |= AR_NORANDOMTEARLINE;
										}
										else if ( !strnicmp(bufanf,"Quote_Non",9) )
										{	gl_area[gl_arzahl].quotetype=QUOTE_NON;
										}
										else if ( !strnicmp(bufanf,"Quote_Msg",9) )
										{	gl_area[gl_arzahl].quotetype=QUOTE_MSG;
										}
										else if ( !strnicmp(bufanf,"Quote_Nachricht",15) )
										{	gl_area[gl_arzahl].quotetype=QUOTE_NACHRICHT;
										}
										else if ( !strnicmp(bufanf,"Quote_Native",12) )
										{	gl_area[gl_arzahl].quotetype=QUOTE_NATIVE;
										}
										else if ( !strnicmp(bufanf,"Quote_English",13) )
										{	gl_area[gl_arzahl].quotetype=QUOTE_ENGLISH;
										}

										if (gl_arzahl==0)
											if ( !strnicmp(bufanf,"name ",5) ||
													 !strnicmp(bufanf,"pseudonym",9) )
											{	pseudonym=TRUE;
											}
										break;

				default:		if (strlen(bufanf)<3L)
											break;
										if (skiparea_fl)
										{	skiparea_fl=FALSE;
											break;
										}
									
										areapath=strtok(bufanf," \t\n\r");
										areaname=strtok(NULL," \t\n\r");
										areanode=strtok(NULL," \t\n\r");
										if (areapath==NULL)
											break;
										if (areaname && *areaname==';') areaname=NULL;
					
										if (areanode)
										{  /* to read "Turbo C" as areaname */
											if (!isdigit(*areanode)&&*areanode!='.')
											{	strcpy(tmp,areaname);
												strcat(tmp," ");
												strcat(tmp,areanode);
												areaname=tmp;
												areanode=strtok(NULL," \t\n\r");
											}
										}

										if (areaname==NULL)
										{	sprintf(msgout,msgtxt[DO_AREASBBSERROR].text,line);
											show_doingtext(msgout);
										}	
											
										if (areapath[1]!=COLON) 		/* no drive specified */
										{	if (areapath[0]==BSLASH) 	/* absolute path without drive */
											{	tmpbuf[0]=aktdrive;
												tmpbuf[1]=COLON;
												tmpbuf[2]=EOS;
												strcat(tmpbuf,areapath);
											}
											else
											{	strcpy(tmpbuf,areapath);	/* relative without drive */
											}
											ctl_string(tmp,tmpbuf,TRUE);
											gl_area[gl_arzahl].path = strdup(tmp);
										}
										else		/* take absolute path */
										{
											ctl_string(tmp,bufanf,TRUE);
											gl_area[gl_arzahl].path = strdup(tmp);
										}
		
										if (areaname==NULL)
										{	LangAlert(MSG_PATHWITHOUTNAME);
											gl_area[gl_arzahl].name = strdup("** Unknown **");
										}
										else
										{	
											ctl_string(tmp,areaname,FALSE);
											gl_area[gl_arzahl].name = strdup(tmp);
										}
										
										if (!areanode)
										{	
#if 0
											gl_area[gl_arzahl].zone=addr[0].zone;
											gl_area[gl_arzahl].net=addr[0].net;
											gl_area[gl_arzahl].node=addr[0].node;
#else
											gl_area[gl_arzahl].zone=
											gl_area[gl_arzahl].net =
											gl_area[gl_arzahl].node=0;
#endif
										}		
										else
										{	if ((cp=strchr(areanode,LF))!=NULL)	*cp=EOS;
											scanaddr(areanode,&gl_area[gl_arzahl].zone,
																				&gl_area[gl_arzahl].net,
																				&gl_area[gl_arzahl].node,
																				&i,tmp);
											if (!gl_area[gl_arzahl].zone)
												gl_area[gl_arzahl].zone = addr[0].zone;
											if (!gl_area[gl_arzahl].net)
											{	gl_area[gl_arzahl].net=addr[0].net;
												gl_area[gl_arzahl].node=addr[0].node;
											}
											if (!is_netmail(gl_arzahl) && *tmp)
												gl_area[gl_arzahl].domain=strdup(tmp);
										}			

										gl_arzahl++;
										if (gl_arzahl > MAX_AREAS-2)
										{	LangAlert(MSG_TOMANYAREAS);
											break;
										}
										break;
			} /* switch */	
		}  /* while */
		fclose(bbsfp);

		for (i=0;i<gl_arzahl;i++)
		{	if (gl_area[i].followup)
			{	for (j=0;j<gl_arzahl;j++)
				{	if (!stricmp(gl_area[i].followup,gl_area[j].name))
					{	gl_area[i].followupnr=j;
						gl_area[i].flag |= AR_FOLLOWUP;
						break;
					}
				}
				if (i>=gl_arzahl)
					free(gl_area[i].followup);
			}
		}
	}

	if (pseudonym)
	{	line=0;
		sprintf(tmpbuf, "%sAREAS.BBS", gl_areasbbspath);
		if ((bbsfp=fopen(tmpbuf, "r"))==NULL) 
			LangAlert(MSG_NOAREASBBS);
		else
		{	sprintf(msgout, "%sAREAS.$$$", gl_areasbbspath);
			if ((out=fopen(msgout, "w"))==NULL)
			{	fclose(bbsfp);
				return;
			}
			else
			{
				while (fgets(line_buf,512,bbsfp)!=NULL)
				{	bufanf=skip_blanks(line_buf);
					if (*bufanf==';')
						fputs(line_buf,out);
					else
						break;
				}
				fputs(line_buf,out);

				while (fgets(line_buf,512,bbsfp)!=NULL)
				{	bufanf=skip_blanks(line_buf);
				
					switch (*bufanf)
					{	case '-'	:	bufanf++;
												if (line==0)
													if ( !strnicmp(bufanf,"name ",5) ||
															 !strnicmp(bufanf,"pseudonym",9) )
														break;
												fputs(line_buf,out);
												break;
						default		:	line++;
												fputs(line_buf,out);
												break;
					} /* switch */	
				}  /* while */
				fclose(out);
			}
			fclose(bbsfp);
			remove(tmpbuf);
			rename(msgout,tmpbuf);
		}
	}
}
