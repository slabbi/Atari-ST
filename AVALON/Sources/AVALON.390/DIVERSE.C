#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fido.h>
#include "form_run.h"
#include "avallang.h"
#include "avalonio.h"
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "diverse.h"
#include "define.h"
#include "version.h"
#include "vars.h"

int	xFsfirst(const char *filename,int attr)
	{	Fsetdta(&dta_buffer);
		return (Fsfirst(filename,attr));
	}

int	xFsnext(void)
	{	return (Fsnext());
	}

int fileselect(char *cin_path,char *csel_file,char *cext,char *titel)
	{	int button=0,tdrive;
		char *pointer,tpath[PATH_MAX],dum[PATH_MAX];

		tdrive=Dgetdrv();
		Dgetpath(dum,tdrive+1);
		if ( dum[1]!=':' )
			{	tpath[0]='A'+tdrive;
				tpath[1]=':';
				tpath[2]=EOS;
			}
		else
			{	*tpath=EOS;
			}
		strcat(tpath,dum);

		sprintf(cin_path,"%s\\%s", tpath, cext);	/* <=3.40 def_path */
		if (aes_version < XFSELECT)
			fsel_input(cin_path, csel_file, &button);
		else
			fsel_exinput(cin_path, csel_file, &button, titel);

		pointer=strrchr(cin_path,'\\');
		if (pointer!=NULL)
			*(++pointer)=EOS;
		else
			strcpy(cin_path,"\\");

/* NEU in 3.41 */
		if (*(cin_path+1)==':')
			Dsetdrv(toupper(*cin_path)-'A');
		Dsetpath(cin_path);

		return(button);
	}

void do_select(char *defaultprg, int cindex, OBJECT *ctree, int maxstr)
	{	char in_path[PATH_MAX],sel_file[14];
		strcpy(sel_file,defaultprg);
		if ( fileselect(in_path,sel_file,"*.*",avaltxt[2]) != 0 )
			put_text(ctree,cindex,strcat(in_path,sel_file),maxstr);
	}

void delete_extension(OBJECT *tree,int index)
	{	char *pointer,*ptext;
		ptext = tree[index].ob_spec.tedinfo->te_ptext;
		pointer = strchr(ptext,EOS);
		while (pointer>ptext && *pointer!='.' && *pointer!='\\') --pointer;
		if (*pointer=='.')
			*pointer=EOS;
	}

long julian(int tag,int monat,int jahr)
	{	signed long aa,bb,cc,n;
		aa = (long) jahr + (((long) monat+9) / 12);
		aa = (7*aa) / 4;
		bb = jahr + (((long) monat-9) / 7);
		bb = (bb/100) + 1;
		bb = (3*bb) / 4;
		cc = (275*(long) monat) / 9;
		n = 367*(long) jahr - aa - bb + cc + (long) tag + 1721029L;
		return(n);
	}

/*******************************************************************/
/* GetPfad; ->Pfad/Datei, <-Pfad																	 */
/*******************************************************************/

void getpfad(char *fname, char *xpfad)
	{	register char *pointer;
		pointer = strrchr(fname,'\\');
		if (pointer==NULL)
			*xpfad=EOS;
		else
			{	*pointer=EOS;
				strcpy(xpfad,fname);
				strcat(xpfad,"\\");
				*pointer='\\';
			}
	}

char *trimstr(char *str)
	{	register char *pointer;
		char *old=str;
		while (isspace(*str) && (*str!=EOS)) str++;
		pointer = strchr(str,EOS);
		while (isspace(*(--pointer))!=0 && pointer>str) *pointer = EOS;
		return(strcpy(old,str));
	}

void trim(char **dest)
	{	register char *pointer;
		while (isspace(**dest)!=0 && *((*dest)++)!=EOS);
		pointer = strchr(*dest,EOS);
		while (isspace(*(--pointer))!=0 && pointer>*dest) *pointer = EOS;
	}

void trimright(char *dest)
	{	register char *pointer;
		pointer = strchr(dest,EOS);
		while (isspace(*(--pointer))!=0 && pointer>dest) *pointer=EOS;
	}

void scanaddr(char *addr,int *zone,int *net,int *node,int *point,char *domain)
	{	char *p,*q=NULL,*q1,*q2;
		int chr;
		
		q1 = strchr(addr,'\n');
		q2 = strchr(addr,'\r');
		if (q1!=NULL && q2!=NULL)
			q=MIN(q1,q2);
		else if (q1!=NULL)
			q=q1;
		else if (q2!=NULL)
			q=q2;
		if (q!=NULL)
			{	chr=*q;
				*q=EOS;
			}

		p = strchr(addr, ':');
    if (p)	*zone = atoi(addr);
    else   	*zone = 0;
    p = strchr(addr, '/');
    if (p)	{	p--;
			        while (strchr("0123456789", *p) && (p >= addr)) p--;
      			  p++;
			        *net = atoi(p);
	    			}
    else	*net = 0;
    p = strchr(addr, '/');
    if (p) 	{ p++;
		        *node = atoi(p);
	  			  }
    else	*node = atoi(addr);
    p = strchr(addr, '.');
    if (p) 	{ p++;
			        *point = atoi(p);
	    			}
    else	*point = 0;
    p = strchr(addr, '@');
    if (p)	{ p++;
			        sscanf(p,"%s",domain);
	    			}
    else	*domain = EOS;
    
    if (q!=NULL)
    	*q=chr;
	}

char *mergeaddr(char *addr,int zone,int net,int node,int point,char *domain)
	{ static char tmp[64];
    *addr = EOS;
    if (zone)	{	itoa(zone, tmp, 10);
    						strcat(addr, tmp);
								strcat(addr, ":");
						  }
    if (zone || net)	{	itoa(net, tmp, 10);
												strcat(addr, tmp);
												strcat(addr, "/");
											}
    itoa(node, tmp, 10);
    strcat(addr, tmp);
    if (point && node)	{ strcat(addr, ".");
										      itoa(point, tmp, 10);
													strcat(addr, tmp);
											  }
    if (*domain)	{ strcat(addr, "@");
						        strcat(addr, domain);
							    }
    return(addr);
}

void input(char *string, int max, FILE *ftype)
	{	char temp[512];
		long len;
		if (fgets(temp, max, ftype)!=NULL)
			{	len = (int) strlen(temp);
				if (len>0)
					if (temp[len-1]=='\n')
						temp[len-1] = EOS;
				strcpy(string,temp);
			}
		else
			{	*string = EOS;
			}
	}

void output(char *string, FILE *ftype)
	{	fputs(string,ftype);
		fputc('\n',ftype);
	}

int exist(char *filename)
	{	return xFsfirst(filename,0);								/* 0 = gefunden */
	}

void undo_xstate(OBJECT *tree, int maske, ...)
	{	va_list ap;
		int index;
		va_start(ap, maske);
		index = va_arg(ap,int);
		while (index != -1)
			{	OB_STATE(tree,index) &= ~maske;
				index = va_arg(ap,int);
			}
		va_end(ap);
	}

void do_xstate(OBJECT *tree, int maske, ...)
	{	va_list ap;
		int index;
		va_start(ap, maske);
		index = va_arg(ap,int);
		while (index != -1)
			{	OB_STATE(tree,index) |= maske;
				index = va_arg(ap,int);
			}
		va_end(ap);
	}

void undo_xflags(OBJECT *tree, int maske, ...)
	{	va_list ap;
		int index;
		va_start(ap, maske);
		index = va_arg(ap,int);
		while (index != -1)
			{	OB_FLAGS(tree,index) &= ~maske;
				index = va_arg(ap,int);
			}
		va_end(ap);
	}

void do_xflags(OBJECT *tree, int maske, ...)
	{	va_list ap;
		int index;
		va_start(ap, maske);
		index = va_arg(ap,int);
		while (index != -1)
			{	OB_FLAGS(tree,index) |= maske;
				index = va_arg(ap,int);
			}
		va_end(ap);
	}

void undo_state(OBJECT *ctree, int cindex, int maske)
	{	OB_STATE(ctree,cindex) &= ~maske;
	}

void do_state(OBJECT *ctree, int cindex, int maske)
	{	OB_STATE(ctree,cindex) |= maske;
	}

int is_state(OBJECT *ctree, int cindex, int maske)
	{	return OB_STATE(ctree,cindex) & maske;
	}

void put_text(OBJECT *ctree, int cindex, char *txt, int maxlen)
	{	char *ptext;
		ptext = ctree[cindex].ob_spec.tedinfo->te_ptext;
		if (maxlen==0)
			{	*ptext=EOS;
			}
		else
			{	strncpy(ptext,txt,MIN(maxlen,-1+ctree[cindex].ob_spec.tedinfo->te_txtlen));
				if (maxlen==strlen(txt))
					*(ptext + maxlen) = EOS;
			}
	}

void put_string(OBJECT *ctree, int cindex, char *txt, int maxlen)
	{	char *string;
		string = ctree[cindex].ob_spec.free_string;
		if (maxlen==0)
			{	*string=EOS;
			}
		else
			{	strncpy(string,txt,maxlen);
				if (maxlen==strlen(txt))
					*(string + maxlen) = EOS;
			}
	}

void put_template(OBJECT *ctree, int cindex, char *text, char *tmplt, char *valid)
	{	TEDINFO *ti;
		ti = ctree[cindex].ob_spec.tedinfo;
		ti->te_pvalid = valid;
		ti->te_ptmplt = tmplt;
		ti->te_ptext  = text;
    ti->te_txtlen = (int) strlen(valid)+1;
    ti->te_tmplen = (int) strlen(tmplt)+1;
	}

char *get_text(OBJECT *ctree, int cindex)
	{	return ctree[cindex].ob_spec.tedinfo->te_ptext;
	}


#if !defined( _AVALSEMP_ )

int log_it(int typ,char *text)
	{	int		ret=0,chr;
		char	temp[MAX_STR];
		time_t	tim;
		FILE	*fp;

		if (*statuslog!=EOS)
			{	if ((fp=fopen(statuslog,"a"))!=NULL)
					{	tim=time(NULL);
						strftime(temp,16,"%d %b %X",localtime(&tim));
						switch(typ)
							{ case	1	:	chr='*';	break;
								case	2	:	chr='+';	break;
								case	3	:	chr=':';	break;
								case	4	:	chr='#';	break;
								case	5	:	chr=' ';	break;
								default	:	chr='!';
							}
            fprintf(fp,"%c %s AVAL %s\n",chr,temp,text);
						fclose(fp);
					}
				else
					{	ret=2;
					}
			}
		else
			{	ret=1;
			}
		return(ret);
	}

#endif


void file2addr(char *file,int *net,int *node)
	{	char filename[9];
		strncpy(filename,file,8);
		filename[8] = EOS;
		*node = (int)strtoul(filename+4,NULL,16);
		filename[4] = EOS;
		*net = (int)strtoul(filename,NULL,16);
	}

void file2addr36(char *file,int *net,int *node,int *point)
	{	char filename[9];
		strncpy(filename,file,8);
		filename[8] = EOS;
		*point = (int)strtoul(filename+6,NULL,36);
		filename[6] = EOS;
		*node = (int)strtoul(filename+3,NULL,36);
		filename[3] = EOS;
		*net = (int)strtoul(filename,NULL,36);
	}

char *addr2file(unsigned int net,unsigned int node)
	{	static char filename[13];
		sprintf(filename,"%04X%04X",net,node);
		return(filename);
	}

char *addr2file36(unsigned int net,unsigned int node,unsigned int point)
	{	static char filename[13];
		char dum[10],*pointer;

		itoa(net	,dum,36);	sprintf(filename	,"%3s",dum);
		itoa(node ,dum,36);	sprintf(filename+3,"%3s",dum);
		itoa(point,dum,36);	sprintf(filename+6,"%2s",dum);
		pointer=filename;
		while (*pointer!=EOS)
			{	if (*pointer==' ')
					*pointer = '0';
				pointer++;
			}
		return(filename);
	}

void delete_unused_flags(MSG_HEADER *header)
	{	int i;
		header->wUp =
		header->wReply =
		header->wReadcount =
		header->wCost = 0;
		for (i=0; i<=3; i++)
		*(long *)(&header->lMsgidcrc+i) = 0;
	}

void write_ids(MSG_HEADER *header,FILE *msg,char *msgaddr)
	{	char temp[MAX_STR];
		sprintf(temp,"\001MSGID: %s %04x%04x\n",msgaddr,(int) Random(), (int) Random());
		fputs(temp,msg);
		header->wSize += (int) strlen(temp);

		sprintf(temp,"\001PID: %s\n",PID);
		fputs(temp,msg);
		header->wSize += (int) strlen(temp);
	}

int xform_maindialog(register int mode)
	{	if (divflags & D_NOAVALONBOX)
			{	if (divflags & D_BOXSMALL)
					{	OB_FLAGS(tree20,ROOT) = LASTOB;
						OB_W(tree20,ROOT) = 128;
						OB_H(tree20,ROOT) = 0;
					}
				else
					{	OB_FLAGS(tree20,ROOT) = NONE;
						OB_W(tree20,ROOT) = OB_W(tree20,DUMMYPIC) = OB_W(treepic0,AVALBLD0);
						OB_H(tree20,ROOT) = OB_H(tree20,DUMMYPIC) = OB_H(treepic0,AVALBLD0);

						if ( OB_H(tree1,DOINFO)<=8 )
							{	OB_W(tree20,ROOT) = OB_W(tree20,DUMMYPIC) = OB_W(treepic0,AVALBLD1);
								OB_H(tree20,ROOT) = OB_H(tree20,DUMMYPIC) = OB_H(treepic0,AVALBLD1);
								OB_H(tree20,ROOT) += 8;
							}
					}
				return xform_dial(tree20,mode,&handle,&wnx,&wny,&wnw,&wnh);
			}
		else
			return xform_dial(tree1,mode,&handle,&wnx,&wny,&wnw,&wnh);
	}

void memory_error(void)
	{	form_alert(1,msg[33]);
	}

void fatal_error(char *ftxt)
	{	char temp[MAX_STR];
		sprintf(temp,tmsg[4],ftxt);
		form_alert(1,temp);
	}

void file_not_found(void)
	{	form_alert(1,msg[6]);
	}

void vhide_mouse( void )
	{	v_hide_c(vdi_handle);
	}
void vshow_mouse( int reset )
	{	v_show_c(vdi_handle,reset);
	}

/*	mode = 0 => keine Ausgabe						*/
/*	mode = 1 => GEM-Ausgabe zugelassen	*/
/*	mode = 2 => TOS-Ausgabe zugelassen	*/

void filecopy(int xhin,int xhout,int mode,char *copy_buffer)
	{	long xlen;
		xlen = Fseek(0,xhin,2);
		Fseek(0,xhin,0);

		if ( (mode==1) && (xlen==0) )
			form_alert(1,msg[8]);

#if !defined( _AVALSEMP_ )
		if ( (mode==2) && (xlen==0) )
			printf(tbmsg[36]);
#endif

		if ( xlen!=0 )
			{	if (copy_buffer==NULL)
					{	if (mode==1)
							{	form_alert(1,msg[36]);
							}
#if !defined( _AVALSEMP_ )
						if (mode==2)
							{	printf(tbmsg[5]);
								pause();
							}
#endif
					}
				else
					{	while (xlen>BUFFER_SIZE)
							{	Fread (xhin ,BUFFER_SIZE,copy_buffer);
								Fwrite(xhout,BUFFER_SIZE,copy_buffer);
								xlen -= BUFFER_SIZE;
							}
						Fread (xhin ,xlen,copy_buffer);
						Fwrite(xhout,xlen,copy_buffer);
					}
			}
	}

/* =============================================================== */
/* =HATCH/FREQUEST================================================ */
/* =============================================================== */

void setze_presets(void)
	{	int i,j,zone,dummy,pos;
		char dum[MAX_STR],dum1[MAX_STR];

		for (i=0; i<=MAX_PRESET-1; i++)
			{	OB_FLAGS(tree81,PRESET0+i) = NONE;
				put_text(tree81,PRESET0+i,"---",26);
			}
		for (i=0; i<=address_cnt; i++)
			{	scanaddr(address[i],&zone,&dummy,&dummy,&dummy,dum1);
				sprintf(dum,"%3u - %-20s",zone,dum1);
				pos = -1;
				for (j=0; j<=MAX_PRESET-1; j++)
					{	if (stricmp(get_text(tree81,PRESET0+j),dum)==0)
							break; /* schon vorhanden */
						if (stricmp(get_text(tree81,PRESET0+j),"---")==0)
							{	pos = j;
								break;
							}
					} /* in pos steht freier Platz oder -1 */
				if (pos != -1)
					{	OB_FLAGS(tree81,PRESET0+pos) = SELECTABLE|EXIT;
						put_text(tree81,PRESET0+pos,dum,26);
					}
			}
	}
