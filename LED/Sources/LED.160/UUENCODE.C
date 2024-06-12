#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"
#include "ledscan.h"
#include "language.h"

static word uue2binary(long win,byte *file,word z,word fastflag);
static word mime2binary(long win,byte *file,word z,word code,word fastflag);

/*
**	MIME-Tabellen (quoted-printable)
*/

#define hexchar(c)	ascii2hex(c)

/*
**	MIME-Tabellen (base64)
*/

static char index_64[128] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};

#define char64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])

/*
**	UUE-Tabellen
*/

static char set[] =
	" !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
static char decset[] =
	" !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
static char table[256];

#define ENC(c)	( set[ (c) & 077 ] )
#define DEC(c)	( table[ (c) & 0177 ] )

/*
** Funktionen
*/

static word fr(FILE *fd, byte *buf, int cnt)
{	word c, i;
	for (i=0; i<cnt; i++)
	{	c = getc(fd);
		if (c == EOF)
			return(i);
		buf[i] = (byte)c;
	}
	return (cnt);
}

static void outdec(byte *p, byte **cp)
{	word c1, c2, c3, c4;
	c1 = *p >> 2;
	c2 = ((p[0] << 4) & 060) | ((p[1] >> 4) & 017);
	c3 = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	c4 = p[2] & 077;
	*(*cp)++=ENC(c1);
	*(*cp)++=ENC(c2);
	*(*cp)++=ENC(c3);
	*(*cp)++=ENC(c4);
}

static void decoutdec(byte *p, FILE *f, word n)
{	word c1, c2, c3;
	c1 = (DEC( *p ) << 2) | (DEC(p[1]) >> 4);
	c2 = (DEC(p[1]) << 4) | (DEC(p[2]) >> 2);
	c3 = (DEC(p[2]) << 6) | (DEC(p[3]));
	if (n>=1) putc(c1, f);
	if (n>=2)	putc(c2, f);
	if (n>=3)	putc(c3, f);
}

word read_binary(long win)
{	FILE *fp;
	static byte file[MAX_STR];
	byte line_buf[80],buf[80],*cp;
	word n,z,i,mode;
  struct stat sbuf;

	if (!available(msgtxt[DO_READBINARY].text))
		return(FALSE);

	if (hole_file(file,NULL,"*.*",msgtxt[FILE_READBINARY].text)==FALSE)
		return(FALSE);
			
	biene();
	if ((fp=fopen(file,"rb"))==NULL)
	{	mouse_normal();
		LangAlert(MSG_BINARYOPENERROR);
		return(FALSE);
	}	

	if (filelength(fileno(fp))>48000ul)
	{	mouse_normal();
		LangAlert(MSG_BINARYFILELENGTH);
		return(FALSE);
	}	

	z=wi[win].lnr;

  fstat(fileno(fp), &sbuf);
  mode = sbuf.st_mode & 0777;
	
	if (!ins_line(win,z++,"--start of uuecoded part--\n",FALSE,FALSE))
		return(FALSE);
	sprintf(line_buf,"--original length:%5lub--\n",filelength(fileno(fp)));
	if (!ins_line(win,z++,line_buf,FALSE,FALSE))
		return(FALSE);

	if (!ins_line(win,z++,"table\n",FALSE,FALSE))
		return(FALSE);
	strcpy(line_buf,set);
	line_buf[32]='\n';
	line_buf[33]=EOS;
	if (!ins_line(win,z++,line_buf,FALSE,FALSE))
		return(FALSE);
	strcpy(line_buf,set+32);
	line_buf[32]='\n';
	line_buf[33]=EOS;
	if (!ins_line(win,z++,line_buf,FALSE,FALSE))
		return(FALSE);
	
	cp=strrchr(file,'\\');
	if (cp)
		strcpy(file,cp+1);
  sprintf(line_buf,"begin %o %s\n", mode, file);
	if (!ins_line(win,z++,line_buf,FALSE,FALSE))
		return(FALSE);

	for (;;)
	{	memset(buf,EOS,80);
		n = fr(fp, buf, 45);
		if (n<=0) break;

		cp=line_buf;
		*cp++=ENC(n);
		for (i=0; i<n; i+=3)
			outdec(&buf[i], &cp);

/*
**	Hier evtl. Last-Character-Check-Counter "z->a" einbauen!
*/

		*cp++='\n';
		*cp=EOS;
		if (!ins_line(win,z++,line_buf,FALSE,FALSE))
			break;

		if (n<=0) break;
	}

	if (!ins_line(win,z++,"end\n",FALSE,FALSE))
		return(FALSE);

	if (!ins_line(win,z,"--end of uuecoded part--\n",FALSE,FALSE))
		return(FALSE);

	if (wi[win].cy>=TOPMSG_LINE)
		set_cxpos(win);
	fclose(fp);
	mouse_normal();
	return(TRUE);
}

#define UUECODED		1
#define MIMECODED		2
#define	M_BASE64		4

word write_binary(long win,word fastflag)
{	static byte file[MAX_STR];
	byte buf[80],*cp,*line;
	word z,mode,codetype=0;

	if (!available(msgtxt[DO_WRITEBINARY].text))
		return(FALSE);

	*file=
	*buf=EOS;
	mode=0;
	strcpy(decset,set);
	z=0;
	for (;;)
	{	line=wi[win].lines[z];
		if (!line)
		{	mouse_normal();
			LangAlert(MSG_UUEBEGIN);
			return(FALSE);
		}

		if (*line=='>' && line[1]==' ') line+=2;

/* MIME-Coding */
		if (!strnicmp(line,"Content-Transfer-Encoding: base64",33))
			codetype|=M_BASE64;
		else if (!strnicmp(line,"Content-Transfer-Encoding: ",27))
			codetype&= ~M_BASE64;

		if (!strnicmp(line,"Content-Disposition: inline; filename=",38) ||
			  !strnicmp(line,"Content-Disposition: attachment; filename=",42))
		{	if (codetype & M_BASE64)
			{	codetype|=MIMECODED;
				if (line[21]=='a')
					cp=line+42;
				else
					cp=line+38;
				while (*cp=='\"') cp++;
				strcpy(buf,cp);
				cp=buf;
				while(*cp && *cp!='\"') cp++;
				*cp=EOS;
				break;
			}
		}

/* UUE-Coding */
		if (!strncmp(line,"begin ",6))
		{	codetype|=UUECODED;
			sscanf(line, "begin %o %s", &mode, buf);
			break;
		}
		if (!strncmp(line,"table ",6))
		{	z++;
			strncpy(decset,wi[win].lines[z],32);
			z++;
			strncpy(decset+32,wi[win].lines[z],32);
			decset[64]=EOS;
		}
		z++;
	}

	if (*buf==EOS)
		strcpy(buf,"\\");
	cp=strrchr(buf,'\\');
	if (cp)
		strcpy(buf,cp+1);
	cp=strrchr(file,'\\');
	if (cp)
	{	cp[1]=EOS;
		strcat(file,buf);
	}

	if (hole_file(file,buf,"*.*",msgtxt[FILE_WRITEUUECODEDMSG].text)==FALSE)
		return(FALSE);

	biene();
	switch(codetype)
	{	case	UUECODED					:	strcpy(buf," UUE> "); break;
		case	MIMECODED|M_BASE64:	strcpy(buf,"MIME> "); break;
		default									:	strcpy(buf," ???> "); break;
	}
	sprintf(buf+6,msgtxt[DO_SAVING].text,file);
	show_doingtext(buf);

	switch(codetype)
	{	case	UUECODED					:	uue2binary(win,file,z,fastflag); break;
		case	MIMECODED|M_BASE64:	mime2binary(win,file,z,M_BASE64,fastflag); break;
	}

	if (fastflag)
	{	select_msg(win,wi[win].aktmsg+1,TRUE,TRUE);
		need_update_msglistwindow(win,RETURN);
	}
	return(FALSE);
}

static word uue2binary(long win,byte *file,word z,word fastflag)
{	FILE *fp;
	word n,add=0;
	byte *cp,buf[80];

	memset(table,0,256);
	cp=decset;      /* fill table */
  for(n=64;n;--n)
  	table[*(cp++) & 0177]=64-n;

	if ((fp=fopen(file,"wb"))==NULL)
	{	del_doing();
		mouse_normal();
		LangAlert(MSG_WRITEBINARYERROR);
		return(FALSE);
	}

	for (;;)
	{
		z++;
weiter:
		cp=wi[win].lines[z];

		if (cp && *cp=='>' && cp[1]==' ') {	cp+=2; add=2; }

		if (cp && !strnicmp(cp,"end",3))
			break;

		if (!cp || (DEC(*cp)<=0 && *cp!=' ' && *cp!='`') || !strncmp(cp,"--- ",4))
		{	del_doing();
			mouse_normal();
			if (fastflag)
				n=2;
			else
				n=LangAlert(MSG_UUEEND);
			if (n==2)
			{	n=wi[win].aktmsg;
				select_msg(win,wi[win].aktmsg+1,TRUE,TRUE);
				need_update_msglistwindow(win,RETURN);
				if (n>=wi[win].aktmsg)
				{	LangAlert(MSG_UUEENDOFMSGS);
					fclose(fp);
					return(FALSE);
				}
				strcpy(buf," UUE> ");
				sprintf(buf+6,msgtxt[DO_APPENDING].text,file);
				show_doingtext(buf);
				z=0;
				for (;;)
				{	if (!wi[win].lines[z])
					{	del_doing();
						mouse_normal();
						LangAlert(MSG_NOMOREUUEDATA);
						fclose(fp);
						return(FALSE);
					}
					if ( DEC(*wi[win].lines[z])>0 )
						if ( (DEC(wi[win].lines[z][add])*4)/3==strlen(wi[win].lines[z])-add-2 ||
								 (DEC(wi[win].lines[z][add])*4)/3==strlen(wi[win].lines[z])-add-3 )
							goto weiter;
					z++;
				}
			}
			break;
		}

		n=DEC(*cp);
#if 0
		if (n<=0) break;
#endif

		cp++;
		while (n>0)
		{	decoutdec(cp,fp,n);
			cp += 4;
			n -= 3;
		}
	}

	del_doing();
	fclose(fp);
	mouse_normal();
	return(TRUE);
}

static word mime2binary(long win,byte *file,word z,word code,word fastflag)
{	FILE *fp;
	byte *cp,buf[80];
	word c1,c2,c3,c4;
	word n,chr,finished,linelen=0;

	if ((fp=fopen(file,"wb"))==NULL)
	{	del_doing();
		mouse_normal();
		LangAlert(MSG_WRITEBINARYERROR);
		return(FALSE);
	}

	for (;;)
	{	c1=
		c2=
		c3=
		c4=0;
		z++;
weiter:
		cp=wi[win].lines[z];

		if (cp && !strncmp(cp,"----",4))
			break;

		if (cp && isspace(*cp))
			continue;

		if (!cp || !strncmp(cp,"--- ",4))
		{	del_doing();
			mouse_normal();
			if (fastflag)
				n=2;
			else
				n=LangAlert(MSG_UUEEND);
			if (n==2)
			{	n=wi[win].aktmsg;
				select_msg(win,wi[win].aktmsg+1,TRUE,TRUE);
				need_update_msglistwindow(win,RETURN);
				if (n>=wi[win].aktmsg)
				{	LangAlert(MSG_UUEENDOFMSGS);
					fclose(fp);
					return(FALSE);
				}
				strcpy(buf,"MIME> ");
				sprintf(buf+6,msgtxt[DO_APPENDING].text,file);
				show_doingtext(buf);
				z=0;
				for (;;)
				{	if (!wi[win].lines[z])
					{	del_doing();
						mouse_normal();
						LangAlert(MSG_NOMOREUUEDATA);
						fclose(fp);
						return(FALSE);
					}
					if ( !isspace(*wi[win].lines[z]) )
						if (linelen==strlen(wi[win].lines[z]))
							goto weiter;
					z++;
				}
			}
			break;
		}

		/* eine Zeile schreiben */
		finished=FALSE;
		if (!linelen)
			linelen=(word)strlen(cp);

		if (code==M_BASE64)
		{	while (*cp && !isspace(*cp))
			{	c1=*cp++;
				if (char64(c1)==-1)
					break;
				c2=*cp++;
				c3=*cp++;
				c4=*cp++;
				if (c1=='=')
				{	finished=TRUE;
					break;
				}
	    	c1 = char64(c1);
		    c2 = char64(c2);
				chr = ((c1<<2) | ((c2&0x30)>>4));
	    	putc(chr, fp);
		    if (c3=='=')
	  	  {	finished=TRUE;
	  	  	break;
	  	  }
	    	else
		    { c3 = char64(c3);
			    chr = (((c2&0XF) << 4) | ((c3&0x3C) >> 2));
	    	  putc(chr, fp);
	      	if (c4=='=')
		      {	finished=TRUE;
		      	break;
		      }
	  	    else
	    	  {	c4 = char64(c4);
						chr = (((c3&0x03) <<6) | c4);
		        putc(chr, fp);
	  	    }
				}
			}
    }
#if 0
    else
		{	while (*cp && !isspace(*cp))
			{	if (*cp=='=')
				{	finished=TRUE;
					break;
				}
				c1=hexchar(*cp++);
				c2=hexchar(*cp++);
		    chr = (c1<<4) | c2;
	    	putc(chr, fp);
			}
    }
#endif
		if (finished)
			break;
	}

	del_doing();
	fclose(fp);
	mouse_normal();
	return(TRUE);
}
