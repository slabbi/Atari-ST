#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <portab.h>
#include "casio_sf.h"
#include "casio_sf.rh"
#include "define.h"
#include "diverse.h"
#include "vars.h"

extern int teste_ob_key_ok(char *name,long key);

void init_vars(void)
	{	int i,wchar,hchar,wbox,hbox,min_x,min_y,max_x,max_y,
				aes_handle;
		char temp[MAX_STR];
		FILE *fin;
		OBJECT *rsc_start;

		graf_mouse( ARROW,NULL );
		Fsetdta(&dta_buffer);

		wind_get(0,WF_WORKXYWH,&min_x,&min_y,&max_x,&max_y);
		max_x += min_x;
		max_y += min_y;
		mid_x = max_x / 2;
		mid_y = max_y / 2;

		aes_handle = graf_handle(&wchar,&hchar,&wbox,&hbox);
		for(i=1; i<10; _VDIParBlk.intin[i++]=1);
		_VDIParBlk.intin[0]=1;			/* SCREEN	*/
		_VDIParBlk.intin[10]=2;			/* RC			*/
		vdi_handle=aes_handle;
		v_opnvwk(_VDIParBlk.intin,&vdi_handle,_VDIParBlk.intout);

		def_drive = Dgetdrv();
		Dgetpath(temp,def_drive+1);
		sprintf(def_path,"%c:%s",(def_drive+65),temp);

		aes_version = _GemParBlk.global[0];
		gemdos_version = Sversion();

		tree1=rs_trindex[DIALOG1];
		tree2=rs_trindex[DIALOG2];
		tree3=rs_trindex[DIALOG3];
		tree4=rs_trindex[DIALOG4];
		tree5=rs_trindex[DIALOG5];
		tree6=rs_trindex[DIALOG6];
		tree7=rs_trindex[DIALOG7];
		tree8=rs_trindex[DIALOG8];
		tree9=rs_trindex[DIALOG9];
		tree10=rs_trindex[DIALOG10];
		tree11=rs_trindex[DIALOG11];
		tree12=rs_trindex[DIALOG12];
		tree13=rs_trindex[DIALOG13];
		tree99=rs_trindex[DIALOG99];
		treepic0=rs_trindex[PICTURE0];
		rsc_start=rs_trindex[0];
		for (i=0; i<=NUM_OBS-1; i++)
			rsrc_obfix(rsc_start,i);

		sprintf(temp,"%s\\CASIO_SF.KEY",def_path);
		if (exist(temp)==0)
			{	fin=fopen(temp,"r");
				input(sysopname,MAX_STR,fin);
				input(temp,MAX_STR,fin);
				sysopkey=strtoul(temp,NULL,16);
				fclose(fin);
				if (teste_ob_key_ok(sysopname,sysopkey))
					vollversion=TRUE;
				else
					vollversion=FALSE;
			}
		else
			{	vollversion=FALSE;
			}
		sprintf(temp,"%s\\CASIO_SF.INF",def_path);
		if (exist(temp)==0)
			{	fin=fopen(temp,"r");
				input(temp,MAX_STR,fin); ebaudcode=atoi(temp);
				input(temp,MAX_STR,fin); sbaudcode=atoi(temp);
				input(temp,MAX_STR,fin); port=atoi(temp);
				input(temp,MAX_STR,fin); konvert=atoi(temp);
				fclose(fin);
			}
	}

int	xFsfirst(const char *filename,int attr)
	{	Fsetdta(&dta_buffer);
		return (Fsfirst(filename,attr));
	}

void cutext(char *env)
	{	char *pointer;
		pointer=strrchr(env+strlen(env)-4,'.');
		if (pointer!=NULL)
			*pointer=EOS;
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

		if (*(cin_path+1)==':')
			Dsetdrv(toupper(*cin_path)-'A');
		Dsetpath(cin_path);

		return(button);
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


void vhide_mouse( void )
	{	v_hide_c(vdi_handle);
	}
void vshow_mouse( int reset )
	{	v_show_c(vdi_handle,reset);
	}

unsigned long julian(int tag,int monat,int jahr)
	{	unsigned long aa,bb,cc,n;
		aa = (long) jahr + (((long) monat+9) / 12);
		aa = (7*aa) / 4;
		bb = jahr + (((long) monat-9) / 7);
		bb = (bb/100) + 1;
		bb = (3*bb) / 4;
		cc = (275*(long) monat) / 9;
		n = 367*(long) jahr - aa - bb + cc + (long) tag + 1721029UL;
		return(n);
	}

char *datum(unsigned long n)
	{	unsigned long i,t,m,j;
		static char tdat[32];
		i = n - 1721119UL;
		j = (4*i-1) / 146097UL;
		i = (4*i-1) % 146097UL;
		t = i / 4;
		i = (4*t+3) / 1461;
		t = (4*t+3) % 1461;
		t = (t+4) / 4;
		m = (5*t-3) / 153;
		t = (5*t-3) % 153;
		t = (t+5) / 5;
		j = 100*j + i;
		if (m<10)
			{	m += 3;
			}
		else
			{	m -= 9;
				j++;
			}
		sprintf(tdat,"%04u-%02u-%02u",(int)j,(int)m,(int)t);
		return(tdat);
	}
