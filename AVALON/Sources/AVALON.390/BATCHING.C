/* MODUL: Batchinterpreter		 */
/*														 */
/* (c) St.Slabihoud 1991-1993	 */

/* COPY src dst							OK */
/* MOVE src dst							OK */
/* SCOPY src dst						OK */
/* SMOVE src dst						OK */

/* CLS											OK */
/* TYPE src									OK */
/* PRINT src								OK */
/* ECHO <text>							OK */
/* WECHO <text>							OK */
/* ECHO=FULL|ON|OFF					OK */

/* OPEN/APPEND							OK */
/* CLOSE										OK */
/* WRITE										OK */
/* DEL src									OK */
/* EXEC src cmd							OK */
/* DEXEC dfile src cmd			OK */
/* CHDIR src								OK */
/* X:												OK */
/* RENAME src dest					OK */
/* DIR <src>								OK */
/* DUMMYFILE src						OK */

/* GETKEY										OK */
/* REDIRECT=ON|OFF					OK */
/* PAUSE										OK */
/* REM oder ";"							OK */
/* EXIT											OK */
/* EXIT n										OK */
/* ENDOFAVALON							OK */
/* ERRORENDOFAVALON n				OK */

/* EXIST src								OK */
/* EXISTn src								OK */
/* NOTEXIST src							OK */
/* NOTEXISTn src						OK */
/* JUMP n										OK */
/* :label                   OK */
/* GOTO label								OK */
/* SWITCH n									OK */

/* SHELL										OK */
/* IFERR= level label				OK */
/* IFERR> level label				OK */
/* IFERR< level label				OK */
/* IFERR# level label				OK */
/* CLRERR										OK */
/* SHIP											OK */
/* BELL											OK */
/* GONG											OK */
/* WAITTIME time            OK */
/* WEEKDAY?									OK */
/* DAY?											OK */
/* MONTH?										OK */
/* YEAR?										OK */
/* HOUR?										OK */
/* MINUTE?									OK */
/* TIME?										OK */
/* DATE?										OK */
/* LOG text									OK */
/* LOGn text								OK */

/* parameter %%0...%%9/%%e	OK */
/* FILESELECT	[n]						OK */
/* CUTEXTENSION [n]					OK */
/* CUTPATH [n]							OK */
/* INPUT [n [text]]					OK */
/* PRESET [n [text]]				OK */
/* CONVERT [n [text]]				OK */
/* CONVERT4D [n [text]]			OK */

/* !FILE filename						OK */
/* !SUBJECT subj|*filename*	OK */
/* !AREA areaname						OK */
/* !BASE msgbase						OK */
/* !FLAGS P|C|S|W|K|H				OK */
/* !FROM addr name					OK */
/* !SEND addr name					OK */
/* !CONVERT=ON|OFF					OK */
/* !FILESELECT							OK */
/* !CONVERTTEARLINE=ON/OFF	OK */


/* APPLYDIFF nodelist diff [clean]								*/
/* MAKEUSERLIST listtype nodelist userlist [zone]	*/
/* SPLIT <file> <bytes> <lines>										*/
/* EXTRACT <nodelist> <output> [<regions>]				*/

#include <process.h>
#include <screen.h>
#include <ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aes.h>
#include <errno.h>
#include <ctype.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "avalxhdi.h"
#include "avallang.h"
#include "avalonio.h"
#include "define.h"
#include "windmini.h"
#include "convert.h"
#include "diverse.h"
#include "vars.h"

#define B_MAX_STR		254

#define ECHO	if (echo)
#define FULL	2

extern int	extract_it(char *txt);
extern int	split_it(char *txt);
extern int	applydiff(char *txt);
extern int	makeuserlist(char *txt);
extern void avalonexit(signed int ret);
extern void point_setup(int cwahl);

void ship(void);
void RingBell(void);
void RingGong(void);

signed long batching(char *fname,int redirect);
signed long	parsing(FILE *xcfg);
void	parse(char *txt);
int		do_copy(char *fname);
int		do_scopy(char *fname);
int		copy_it(char *source, char *dest);
void	delete(char *fname, int mode);
void	type_file(char *fname);
void	print(char *fname);
void	do_exec(char *fname);
void	do_dexec(char *fname);
void	do_chdir(char *fname);
void	do_rename(char *fname);
void	do_dir(char *fname);
void	dummyfile(char *fname);
void	xerror(int errorcode);
void	do_shell(void);
void 	waittime(char *tim);
void	logging(int typ, char *text);
int		do_fileselect(char *env);
void	do_cutext(char *env);
void	do_cutpath(char *env);
void	do_input(char *env,int mode);
void	do_convert(char *txt,int mode);

static int line,echo,redirect;
static signed long level;
static LABLE (*lab);												/* DYNAMISCH VERWALTET */

static char	gtemp[512],fselname[10][PATH_MAX];

/*******************************************************************/
/* Batch-Interpreter																							 */
/*******************************************************************/

signed long batching(char *fbatch,int doredir)
	{	FILE *batch;
		signed long retcode;
		int i;
		char temp[MAX_STR];

		for (i=0; i<=9; i++)
			*fselname[i] = EOS;

		echo = TRUE;
		redirect = FALSE;
		msg_flag = 0;

		lab = (LABLE *) calloc(MAX_LABEL+1,sizeof(*lab));
		if (lab==NULL)
			{	memory_error();
				return(0);
			}

		wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
		wind.handle=wind_create(NAME|CLOSER|MOVER|FULLER,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
		if (wind.handle<0)
			{	form_alert(1,msg[23]);
				return(0);
			}
		sprintf(temp," %s ",fbatch);
		wind_set(wind.handle,WF_NAME,temp);
		wind_open(wind.handle,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
		wind_update(BEG_UPDATE);
		wind_cls(&wind);
		if (doredir==TRUE)
			{	redirect=TRUE;
				set_redirect(!(divflags & D_NOLINEA));
			}
		wind_update(END_UPDATE);

		batch=fopen(fbatch, "rb");

		if (batch==NULL)
			{	fatal_error(fbatch);
				retcode=-33L;
			}
		else
			{	setvbuf(batch,NULL,_IOFBF,4096);
				retcode=parsing(batch);
				fclose(batch);
			}
		if (lab!=NULL)
			free((LABLE *) &lab[0]);

		if (doredir==TRUE && redirect==TRUE)
			{	redirect=FALSE;
				reset_redirect();
			}
		wind_close(wind.handle);
		wind_delete(wind.handle);

		return(retcode);
	}

/*******************************************************************/
/* Parser																													 */
/*******************************************************************/

signed long parsing(FILE *xcfg)
	{	char txt[1024],dum[1024],*pointer;
		int i,j,k,lable=0,scan,hiddenmouse=FALSE;
		signed long templong,retcode=0;
		FILE *xfile=NULL;

		line=0;
		do
			{	input(txt, B_MAX_STR, xcfg);
				pointer = txt;
				trim(&pointer);
				line++;
				if ( *pointer==':' )
					{	if (lable<MAX_LABEL)
							{	pointer++;
								trim(&pointer);
								strncpy(lab[++lable].lable,pointer,MAX_LABEL_LEN);
								lab[lable].lable[MAX_LABEL_LEN]=EOS;
								lab[lable].position = ftell(xcfg);
								continue;
							}
						else
							{	sprintf(gtemp,tbmsg[7],MAX_LABEL);
								WPRINT(gtemp)
								pause();
								break;
							}
					}
			}
		while (feof(xcfg)==0);
		rewind(xcfg);

		line=0;
		do
			{	i=TRUE;
				scan=0;
				i = wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER);
				if (wind.evnt & MU_KEYBD)
					scan=wind.kscan;
				if (i==FALSE)
					break;
				
				if (!wind_top(&wind))
					i=FALSE;

				if (i==TRUE)
					{	if ( scan==0x11b )
							{	WPRINT(tbmsg[8])
								i = pause();
								if (i=='Y' || i=='y' || i=='J' || i=='j')
									break;
							}

						input(txt, B_MAX_STR, xcfg);
						pointer=txt; trim(&pointer); strcpy(txt,pointer);

						if (strstr(txt,"%%")!=NULL)
							parse(txt);

						if (*txt!='@')
							{	if (echo==FULL)
									{	WPRINT(txt);
									}
							}
						else
							{	strcpy(txt,txt+1);
							}

						pointer=txt;
						line++;
		
						if ( *txt==EOS || *txt==';' || *txt==':' || strnicmp(txt,"REM",3)==0 )
							continue;
						if ( txt[1]==':' )
							{	i=*txt | 32;
								if ( i>='a' && i<='p' )
									Dsetdrv( toupper(i)-'A' );
								else
									{	sprintf(gtemp,tbmsg[2],txt);
										WPRINT(gtemp)
									}
								continue;
							}

						if ( strnicmp(txt,"EXTRACT",7)==0 )
							{	pointer += 7;	trim(&pointer);	strcpy(txt,pointer);
								level=(long)extract_it(txt);
								continue;
							}
						if ( strnicmp(txt,"SPLIT",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								level=(long)split_it(txt);
								continue;
							}
						if ( strnicmp(txt,"APPLYDIFF",9)==0 )
							{	pointer += 9;	trim(&pointer);	strcpy(txt,pointer);
								level=(long)applydiff(txt);
								continue;
							}
						if ( strnicmp(txt,"MAKEUSERLIST",12)==0 )
							{	pointer += 12;	trim(&pointer);	strcpy(txt,pointer);
								level=(long)makeuserlist(txt);
								continue;
							}

						if ( strnicmp(txt,"!FILESELECT",11)==0 )
							{	level = (long)msg_fileselect();
								continue;
							}
						if ( strnicmp(txt,"!FILE",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								msg_filename(txt);
								continue;
							}
						if ( strnicmp(txt,"!SUBJECT",8)==0 )
							{	pointer += 8;
								if (*(pointer++)!=EOS)
									msg_subject(pointer);
								else
									msg_subject("");
								continue;
							}
						if ( strnicmp(txt,"!AREA",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								msg_area(txt);
								continue;
							}
						if ( strnicmp(txt,"!CONVERT=",9)==0 )
							{	pointer += 9;	trim(&pointer);	strcpy(txt,pointer);
								msg_convert(txt);
								continue;
							}
						if ( strnicmp(txt,"!CONVERTTEARLINE=",17)==0 )
							{	pointer += 17;	trim(&pointer);	strcpy(txt,pointer);
								msg_converttear(txt);
								continue;
							}
						if ( strnicmp(txt,"!BASE",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								if (!msg_base(txt))
									xerror(11);
								continue;
							}
						if ( strnicmp(txt,"!FLAGS",6)==0 )
							{	pointer += 6;	trim(&pointer);	strcpy(txt,pointer);
								msg_flags(txt);
								continue;
							}
						if ( strnicmp(txt,"!FROM",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								msg_from(txt);
								continue;
							}
						if ( strnicmp(txt,"!SEND",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								msg_to(txt);
								switch(msg_send(echo))
									{	case	1	: xerror(13);	break;	/* FILE missing			*/
										case	2	: xerror(7);	break;	/* File open error	*/
										case	3	: xerror(11);	break;	/* File not found		*/
										case	4	: xerror(14); break;	/* AREA missing			*/
										case	5	:	xerror(12);	break;	/* Filelength = 0		*/
										case	6	: xerror(15); break;	/* FROM missing			*/
										case	7	: xerror(16); break;	/* SEND parameter		*/
										case	8	: xerror(17); break;	/* SUBJECT missing	*/
										case	9	: xerror(18); break;	/* FLAGS missing		*/
									}
								continue;
							}

						if ( strnicmp(txt,"PRESET",6)==0 )
							{	pointer += 6;	trim(&pointer);	strcpy(txt,pointer);
								do_input(txt,1);
								continue;
							}
						if ( strnicmp(txt,"CONVERT4D",9)==0 )
							{	pointer += 9;	trim(&pointer);	strcpy(txt,pointer);
								do_convert(txt,1);
								continue;
							}
						if ( strnicmp(txt,"CONVERT",7)==0 )
							{	pointer += 7;	trim(&pointer);	strcpy(txt,pointer);
								do_convert(txt,0);
								continue;
							}
						if ( strnicmp(txt,"INPUT",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								do_input(txt,0);
								continue;
							}
						if ( strnicmp(txt,"CUTEXTENSION",12)==0 )
							{	pointer += 12;	trim(&pointer);	strcpy(txt,pointer);
								do_cutext(txt);
								continue;
							}
						if ( strnicmp(txt,"CUTPATH",7)==0 )
							{	pointer += 7;	trim(&pointer);	strcpy(txt,pointer);
								do_cutpath(txt);
								continue;
							}
						if ( strnicmp(txt,"FILESELECT",10)==0 )
							{	pointer += 10;	trim(&pointer);	strcpy(txt,pointer);
								level = (long)do_fileselect(txt);
								continue;
							}
						if ( strnicmp(txt,"LOG",3)==0 )
							{	pointer += 3;
								i=0;
								if (*pointer!=' ')
									i=*(pointer++) -48;
								trim(&pointer);
								logging(i,pointer);
								continue;
							}
						if ( strnicmp(txt,"RENAME",6)==0 )
							{	pointer += 6;	trim(&pointer);	strcpy(txt,pointer);
								do_rename(txt);
								continue;
							}
						if ( strnicmp(txt,"CHDIR",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								do_chdir(txt);
								continue;
							}
						if ( strnicmp(txt,"REDIRECT=",9)==0 )
							{	pointer += 9;
								if ( strnicmp(pointer,"ON",2)==0 )
									{	if (redirect==FALSE)
											{	redirect=TRUE; 
												set_redirect(!(divflags & D_NOLINEA));
											}
									}
								else
									{	if (redirect==TRUE)
											{	redirect=FALSE;
												reset_redirect();
											}
									}
								continue;
							}
						if ( strnicmp(txt,"ECHO=",5)==0 )
							{	pointer += 5;
								if ( strnicmp(pointer,"FU",2)==0 )
									echo=FULL;
								else if ( strnicmp(pointer,"ON",2)==0 )
									echo=TRUE;
								else
									echo=FALSE;
								continue;
							}
						if ( strnicmp(txt,"ECHO",4)==0 )
							{	pointer += 4;
								if (*(pointer++)!=EOS)
									WPRINT(pointer)
								else
									WPRINT("")
								continue;
							}
						if ( strnicmp(txt,"WECHO",5)==0 )
							{	pointer += 5;
								if (*(pointer++)!=EOS)
									WWRITE(pointer)
								else
									WWRITE("")
								continue;
							}
						if ( strnicmp(txt,"DIR",3)==0 )
							{	pointer += 3;
								if (*(pointer++)!=EOS)
									do_dir(pointer);
								else
									do_dir("*.*");
								continue;
							}
						if ( strnicmp(txt,"COPY",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								do_copy(txt);
								continue;
							}
						if ( strnicmp(txt,"MOVE",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								if (do_copy(txt)==0) delete(txt,0);
								continue;
							}
						if ( strnicmp(txt,"SCOPY",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								do_scopy(txt);
								continue;
							}
						if ( strnicmp(txt,"SMOVE",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								if (do_scopy(txt)==0) delete(txt,0);
								continue;
							}
						if ( strnicmp(txt,"EXEC",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								do_exec(txt);
								continue;
							}
						if ( strnicmp(txt,"DEXEC",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								do_dexec(txt);
								continue;
							}
						if ( strnicmp(txt,"DEL",3)==0 )
							{	pointer += 3;	trim(&pointer);	strcpy(txt,pointer);
								delete(txt,1);
								continue;
							}
						if ( strnicmp(txt,"TYPE",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								type_file(txt);
								continue;
							}
						if ( strnicmp(txt,"PRINT",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								print(txt);
								continue;
							}
						if ( strnicmp(txt,"PAUSE",5)==0 )
							{	WPRINT(tbmsg[9])
								i = pause();
								if (i==27)
									{	WPRINT(tbmsg[10])
										i = pause();
										if (i=='Y' || i=='y' || i=='J' || i=='j')
											break;
									}
								continue;
							}
						if ( strnicmp(txt,"GETKEY",6)==0 )
							{	level = (long) pause();
								continue;
							}
						if ( strnicmp(txt,"CLS",3)==0 )
							{	wind_cls(&wind);
								continue;
							}
						if ( (strnicmp(txt,"EXIST",5)==0) ||
								 (strnicmp(txt,"NOTEXIST",8)==0) )
							{	if (*txt=='n' || *txt=='N')
									{	k=1;
										pointer += 8;
									}
								else
									{	k=0;								/* normales berspringen */
										pointer += 5;
									}
								if (*pointer==' ')
									{	trim(&pointer);
										strcpy(txt,pointer);
										if ( (exist(txt)!=0) ^ k )
											input(dum,B_MAX_STR,xcfg);
									}
								else
									{	i=*(pointer++);
										trim(&pointer);
										strcpy(txt,pointer);
										if ( (exist(txt)!=0) ^ k )
											{	if (i<'0' || i>'9') i='1';
												if (i=='0') i='9'+1;
												for (j=1; j<=i-48; j++)
													input(dum,B_MAX_STR,xcfg);
											}
									}
								continue;
							}
						if ( strnicmp(txt,"JUMP",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								i=atoi(txt);
								if (i==0)
									{	xerror(1);
									}
								else
									{	for (j=1; j<=i; j++)
											input(dum,B_MAX_STR,xcfg);
									}
								continue;
							}
#if !defined( _AVALBATCH_ )
						if ( strnicmp(txt,"SWITCH",6)==0 )
							{	pointer += 6;	trim(&pointer);	strcpy(txt,pointer);
								if (hiddenmouse==TRUE)
									vshow_mouse(0);
								i=atoi(txt);
								ECHO	{	sprintf(gtemp,tbmsg[37],i);
												WPRINT(gtemp)
											}
								switch(i)
									{	case 0 : point_setup(POINT0); break;
										case 1 : point_setup(POINT1); break;
										case 2 : point_setup(POINT2); break;
										case 3 : point_setup(POINT3); break;
										case 4 : point_setup(POINT4); break;
										case 5 : point_setup(POINT5); break;
										default: xerror(4);
									}
								if (hiddenmouse==TRUE)
									vhide_mouse();
								redraw_infobox=TRUE;
								continue;
							}
#endif
						if ( strnicmp(txt,"DUMMYFILE",9)==0 )
							{	pointer += 9;	trim(&pointer);	strcpy(txt,pointer);
								dummyfile(txt);
								continue;
							}
						if ( strnicmp(txt,"EXIT",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								retcode=(signed long) atol(txt);
								break;
							}
#if !defined( _AVALBATCH_ )
						if ( strnicmp(txt,"ENDOFAVALON",11)==0 )
							avalonexit(0);
						if ( strnicmp(txt,"ERRORENDOFAVALON",16)==0 )
							{	pointer += 16; trim(&pointer); strcpy(txt,pointer);
								avalonexit( (signed int) atoi(txt) );
							}
#endif
						if ( strnicmp(txt,"SHELL",5)==0 )
							{	pointer += 5;	trim(&pointer);	strcpy(txt,pointer);
								if (*txt==EOS) strcpy(txt," ");
		            if ( system(NULL) != 0 )
									system(txt);
								else
									{	sprintf(gtemp,tbmsg[12],line,txt);
										WPRINT(gtemp)
									}
								continue;
							}
						if ( strnicmp(txt,"HIDEM",5)==0 )
							{	if (hiddenmouse==FALSE)
									vhide_mouse();
								hiddenmouse=TRUE;
								continue;
							}
						if ( strnicmp(txt,"SHOWM",5)==0 )
							{	if (hiddenmouse==TRUE)
									vshow_mouse(0);
								hiddenmouse=FALSE;
								continue;
							}
						if ( strnicmp(txt,"CLRERR",6)==0 )
							{	level=0;
								continue;
							}
						if ( strnicmp(txt,"GOTO",4)==0 )
							{	pointer += 4;	trim(&pointer);	strcpy(txt,pointer);
								j=FALSE;
								for (i=1; i<=lable; i++)
									{	if ( stricmp(lab[i].lable,txt)==0 )
											{	fseek(xcfg,lab[i].position,SEEK_SET);
												j=TRUE;
												break;
											}
									}
								if (j==FALSE)
									{	sprintf(gtemp,tbmsg[13],txt);
										WPRINT(gtemp)
									}
								continue;
							}
						if ( strnicmp(txt,"IFERR",5)==0 )
							{	pointer += 5;
								i = *(pointer++);
								trim(&pointer);
								strcpy(txt,pointer);
								templong=strtol(txt,&pointer,10);
								j=0;
								switch(i)
									{	case '=' : if (level==templong) j=1; break;
										case '<' : if (level<templong) j=1; break;
										case '>' : if (level>templong) j=1; break;
										case '#' : if (level!=templong) j=1; break;
										default	 : j=2;
									}
								if (j==1)
									{	trim(&pointer);
										strcpy(txt,pointer);
										j=FALSE;
										for (i=1; i<=lable; i++)
											{	if ( stricmp(lab[i].lable,txt)==0 )
													{	fseek(xcfg,lab[i].position,SEEK_SET);
														j=TRUE;
														break;
													}
											}
										if (j==FALSE)
											{	sprintf(gtemp,tbmsg[13],txt);
												WPRINT(gtemp)
											}
									}
								if (j==2)
									{	sprintf(gtemp,tbmsg[14],line);
										WPRINT(gtemp)
									}
								continue;
							}
						if ( strnicmp(txt,"SHIP",4)==0 )
							{	ship();
								continue;
							}
						if ( strnicmp(txt,"BELL",4)==0 )
							{	RingBell();
								continue;
							}
						if ( strnicmp(txt,"GONG",4)==0 )
							{	RingGong();
								continue;
							}
						if ( strnicmp(txt,"WAITTIME",8)==0 )
							{	pointer += 8;	trim(&pointer);	strcpy(txt,pointer);
								waittime(txt);
								continue;
							}
						if ( strnicmp(txt,"WEEKDAY?",8)==0 )
							{	getdate( &HEUTE );
								level = julian(HEUTE.da_day,HEUTE.da_mon,HEUTE.da_year) % 7;
								switch((int) level)
									{	case	0	: sprintf(dum,tbmsg[35],tbmsg[38]);	break;
										case	1	: sprintf(dum,tbmsg[35],tbmsg[39]); break;
										case	2	: sprintf(dum,tbmsg[35],tbmsg[40]); break;
										case	3	: sprintf(dum,tbmsg[35],tbmsg[41]); break;
										case	4	: sprintf(dum,tbmsg[35],tbmsg[42]); break;
										case	5	: sprintf(dum,tbmsg[35],tbmsg[43]); break;
										case	6	: sprintf(dum,tbmsg[35],tbmsg[44]); break;
										default	: xerror(5);
									}
								if (level>=0 && level<=6)
									ECHO WPRINT(dum)
								continue;
							}
						if ( strnicmp(txt,"DAY?",4)==0 )
							{	getdate( &HEUTE );
								level = HEUTE.da_day;
								continue;
							}
						if ( strnicmp(txt,"YEAR?",5)==0 )
							{	getdate( &HEUTE );
								level = HEUTE.da_year;
								continue;
							}
						if ( strnicmp(txt,"MONTH?",6)==0 )
							{	getdate( &HEUTE );
								level = HEUTE.da_mon;
								continue;
							}
						if ( strnicmp(txt,"HOUR?",5)==0 )
							{	gettime( &ZEIT );
								level = ZEIT.ti_hour;
								continue;
							}
						if ( strnicmp(txt,"MINUTE?",7)==0 )
							{	gettime( &ZEIT );
								level = ZEIT.ti_min;
								continue;
							}
						if ( strnicmp(txt,"TIME?",5)==0 )
							{	gettime( &ZEIT );
								level = ZEIT.ti_hour * 10000UL +
												ZEIT.ti_min  * 100UL +
												ZEIT.ti_sec;
								continue;
							}
						if ( strnicmp(txt,"DATE?",5)==0 )
							{	getdate( &HEUTE );
								level = HEUTE.da_year * 10000UL +
												HEUTE.da_mon  * 100UL +
												HEUTE.da_day;
								continue;
							}
						if ( strnicmp(txt,"OPEN",4)==0 || strnicmp(txt,"APPEND",6)==0 )
							{	pointer += 4;
								i=FALSE;
								if (strnicmp(txt,"APPEND",6)==0)
									{	pointer += 2;
										i=TRUE;
									}
								level = 1;
								trim(&pointer);
								strcpy(txt,pointer);
								if (i==TRUE)	{	ECHO	{	sprintf(gtemp,tbmsg[45],txt);
																				WPRINT(gtemp)
																			}
															}
								else					{	ECHO	{	sprintf(gtemp,tbmsg[46],txt);
																				WPRINT(gtemp)
																			}
															}
								if (*txt==EOS)
									xerror(0);
								else if (xfile!=NULL)
									xerror(6);
								else
									{	if (i==TRUE) xfile=fopen(txt,"a");
										else				 xfile=fopen(txt,"w");
										if (xfile==NULL)
											xerror(7);
										else
											level = 0;
									}
								continue;
							}
						if ( strnicmp(txt,"CLOSE",5)==0 )
							{	level = 1;
								if (xfile==NULL)
									xerror(8);
								else
									{	fclose(xfile);
										level = 0;
									}
								xfile=NULL;
								continue;
							}
						if ( strnicmp(txt,"WRITE",5)==0 )
							{	pointer += 5;
								level = 0;
								if (xfile==NULL)
									{	xerror(8);
										level = 1;
									}
								else
									{	if (*(pointer++)!=EOS)
											fprintf(xfile,"%s\n",pointer);
										else
											fprintf(xfile,"\n");
									}
								continue;
							}
		
						trim(&pointer);
						if (*pointer!=EOS)
							{	sprintf(gtemp,tbmsg[15],line,txt);
								WPRINT(gtemp)
							}
					}
			}
		while (feof(xcfg)==0);

		if (xfile!=NULL)
			fclose(xfile);
		if (hiddenmouse==TRUE)
			vshow_mouse(0);
		if (redirect==TRUE)
			{	redirect=FALSE;
				reset_redirect();
			}
		return(retcode);
	}

/*******************************************************************/
/* Parse																													 */
/*******************************************************************/

void parse(char *txt)
	{	char buffer[1024],temp[40],*pointer;
		int i;
		*buffer=EOS;

		pointer=strstr(txt,"%%");
		while (pointer!=NULL)				/* Gibt es einen Parameter %%n? 	*/
			{	*pointer=EOS;
				pointer += 2;						/* JA, dann %% berspringen				*/
				strcat(buffer,txt);			/* Text bis dahin in den Puffer		*/
				if (*pointer!=EOS)
					{	if (*pointer=='E' || *pointer=='e')
							{	strcat(buffer,ltoa(level,temp,10));	/* Errorlevel als Zahl einsetzen	*/
							}
						else
							{	i=*pointer-'0';	/* Parameter n einsetzen					*/
								if (i<0 || i>9)
									i=0;
								strcat(buffer,fselname[i]);
							}
						pointer++;
					}
				else
					{	i=0;								/* Parameter 0 einsetzen					*/
						strcat(buffer,fselname[i]);
					}
				strcpy(txt,pointer);		/* Weiter im Text									*/
				pointer=strstr(txt,"%%");
			}

		if (pointer==NULL)
			strcat(buffer,txt);
		strcpy(txt,buffer);
	}

/*******************************************************************/
/* EXEC																														 */
/*******************************************************************/

void do_exec(char *fname)
	{	char source[PATH_MAX],cmd[PATH_MAX],temp[MAX_STR],*start;
		int prg,drive;
		start = strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(source,start);
		start = strtok(NULL,"");
		if (start==NULL)
			*temp=EOS;
		else
			strcpy(temp,start);

		trimstr(temp);						/* Kommandozeile */
		cmd[0]=strlen(temp);
	  cmd[1]=EOS;
		strcpy(&cmd[1],temp);

		prg = 0;
				 if (stricmp(source,"EXPORT"	)==0)	prg=1;
		else if (stricmp(source,"IMPORT"	)==0)	prg=2;
		else if (stricmp(source,"CRUNCH"	)==0)	prg=3;
		else if (stricmp(source,"SCAN"		)==0)	prg=4;
		else if (stricmp(source,"MSGEDIT"	)==0)	prg=5;
		else if (stricmp(source,"TXTEDIT"	)==0)	prg=6;
		else if (stricmp(source,"MAILER"	)==0)	prg=7;
		else if (stricmp(source,"TERMINAL")==0)	prg=8;
		else if (stricmp(source,"UTIL1"		)==0)	prg=9;
		else if (stricmp(source,"UTIL2"		)==0)	prg=10;
		else if (stricmp(source,"UTIL3"		)==0)	prg=11;
		else if (stricmp(source,"UTIL4"		)==0)	prg=12;
		else if (stricmp(source,"UTIL5"		)==0)	prg=13;
		else if (stricmp(source,"UTIL6"		)==0)	prg=14;

		if (prg != 0)
			{	strcpy(source,strupr(program[prg]));
				if (strnicmp(source+strlen(source)-4,".BAT",4)==0)
					{	prg=-1;
					}
				else
					{	cmd[0]=strlen(program2[prg]);
			  		cmd[1]=EOS;
	  				strcpy(&cmd[1],program2[prg]);
						if (*(source+1)==':')
							drive=toupper(*(source))-'A';
						else
							drive=Dgetdrv();			/* def_drive; */
						getpfad(source,temp);
						Dsetdrv(drive);
						Dsetpath(temp);
					}
	  	}

		if (prg>=0)
			{	if (redirect==FALSE)
					{	wind_close(wind.handle);
						wind_delete(wind.handle);
					}

				level=Pexec(0,source,cmd,NULL);

				if (redirect==FALSE)
					{	wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),FALSE,wnx,wny,wnw,wnh);
						wind.handle=wind_create(NAME|CLOSER,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
						wind_set(wind.handle,WF_NAME," AVALON-BATCH ");
						wind_open(wind.handle,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
						wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER);
					}
				Fsetdta(&dta_buffer);
				if (level<0)
					{	sprintf(gtemp,tbmsg[0],level);
						WPRINT(gtemp)
					}
			}
		else
			{	xerror(19);
			}
	}

/*******************************************************************/
/* DEXEC																													 */
/*******************************************************************/

void do_dexec(char *fname)
	{	char source[PATH_MAX],cmd[PATH_MAX],temp[MAX_STR],
				 file[PATH_MAX],*start;
		int error;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(file,start);
		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(source,start);
		start = strtok(NULL,"");
		if (start==NULL)
			*temp=EOS;
		else
			strcpy(temp,start);
		trimstr(temp);
		cmd[0]=strlen(temp);
	  cmd[1]=EOS;
	  strcpy(&cmd[1],temp);
		getpfad(file,temp);
		error=xFsfirst(file,0);
		if (error<0)
			{	sprintf(gtemp,"!DEXEC %s",file);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
			}
		strcpy(file,temp);
		strcat(file,dta_buffer.d_fname);
		if (redirect==FALSE)
			{	wind_close(wind.handle);
				wind_delete(wind.handle);
			}

		level=Pexec(0,source,cmd,NULL);
		if (redirect==FALSE)
			{	wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),FALSE,wnx,wny,wnw,wnh);
				wind.handle=wind_create(NAME|CLOSER,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
				wind_set(wind.handle,WF_NAME," AVALON-BATCH ");
				wind_open(wind.handle,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
				wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER);
			}
		Fsetdta(&dta_buffer);
		if (level!=0)
			{	sprintf(gtemp,tbmsg[0],error);
				WPRINT(gtemp)
			}
		else
			{	if ( remove(file)!=0 )
					{	sprintf(gtemp,tbmsg[1],file);
						WPRINT(gtemp)
					}
				else
					{	ECHO	{	sprintf(gtemp,"!DEL %s",file);
										WPRINT(gtemp)
									}
					}
			}
	}

/*******************************************************************/
/* COPY																														 */
/*******************************************************************/
	
int do_copy(char *fname)
	{	char source[PATH_MAX],dest[PATH_MAX],spfad[PATH_MAX],
				 dpfad[PATH_MAX],*start;
		signed int error;
		int ret=0;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(2);
				return(1);
			}

		strcpy(source,start);
		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(3);
				return(1);
			}

		strcpy(dest,start);
		getpfad(source,spfad);
		getpfad(dest,dpfad);
		error=xFsfirst(source,0);
		if (error<0)
			{	sprintf(gtemp,"!COPY/MOVE %s %s",source,dest);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
				ret=1;
			}

		while (error>=0 && ret==0)
			{	strcpy(source,spfad);
				strcat(source,dta_buffer.d_fname);
				strcpy(dest,dpfad);
				strcat(dest,dta_buffer.d_fname);
				ret = copy_it(source,dest);
				error=xFsnext();
			}
		return(ret);
	}

/*******************************************************************/
/* SCOPY																													 */
/*******************************************************************/
	
int do_scopy(char *fname)
	{	char source[PATH_MAX],dest[PATH_MAX],spfad[PATH_MAX],*start;
		signed int error;
		int ret=0;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(2);
				return(1);
			}

		strcpy(source,start);
		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(3);
				return(1);
			}

		strcpy(dest,start);
		getpfad(source,spfad);

		error=xFsfirst(source,0);
		if (error<0)
			{	sprintf(gtemp,"!SCOPY/SMOVE %s %s",source,dest);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
				ret=1;
			}
		else
			{	strcpy(source,spfad);
				strcat(source,dta_buffer.d_fname);
				ret=copy_it(source,dest);
			}
		return ret;
	}

int copy_it(char *source, char *dest)				/* 1=Fehler */
	{	int ret=0;
		int hin,hout;
		char *copy_buffer;
		copy_buffer = (char *) malloc(BUFFER_SIZE);
		if (copy_buffer==NULL)
			{	WPRINT(tbmsg[16])
				pause();
				return(1);
			}

		if (stricmp(source,dest)==0)
			{	ECHO	{	sprintf(gtemp,tbmsg[48],source);
								WPRINT(gtemp)
							}
				free(copy_buffer);
				return(1);
			}

		hin = (int) Fopen(source,FO_READ);
		if (hin<0)
			{	sprintf(gtemp,tbmsg[49],source);
				WPRINT(gtemp)
				free(copy_buffer);
				return(1);
			}

		hout = (int) Fcreate(dest,0);
		if (hout<0)
			{	sprintf(gtemp,tbmsg[17],dest);
				WPRINT(gtemp)
				Fclose(hin);
				free(copy_buffer);
				return(1);
			}

		ECHO	{	sprintf(gtemp,tbmsg[50],source,dest);
						WPRINT(gtemp)
					}
		filecopy(hin,hout,2,copy_buffer);

		Fclose(hout);
		Fclose(hin);
		free(copy_buffer);
		return ret;
	}


/*******************************************************************/
/* DELETE																													 */
/*******************************************************************/

void delete(char *fname, int mode)
	{	char source[PATH_MAX],pfad[PATH_MAX],*start;
		signed int error;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(source,start);
		getpfad(source,pfad);
		error=xFsfirst(source,0);
		if (error<0)
			{	sprintf(gtemp,"!DEL %s",source);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
			}
		while (error>=0)
			{	strcpy(source,pfad);
				strcat(source,dta_buffer.d_fname);
				if (Fdelete(source)!=0)
					{	if (mode==1)
							{	sprintf(gtemp,tbmsg[1],source);
								WPRINT(gtemp)
							}
					}
				else
					{	if (mode==1)
							{	ECHO	{	sprintf(gtemp,"!DEL %s",source);
												WPRINT(gtemp)
											}
							}
					}
				error=xFsnext();
			}
	}

/*******************************************************************/
/* TYPE																														 */
/*******************************************************************/

void type_file(char *fname)
	{	char source[PATH_MAX],pfad[PATH_MAX],*start,*pointer;
		signed int error;
		FILE *fin;
		int key=0,cnt=0;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(source,start);
		getpfad(source,pfad);
		error=xFsfirst(source,0);
		if (error<0)
			{	sprintf(gtemp,"!TYPE %s",source);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
			}
		while (error>=0)
			{	strcpy(source,pfad);
				strcat(source,dta_buffer.d_fname);
				if ( (fin=fopen(source, "r"))==NULL )
					{	sprintf(gtemp,tbmsg[49],source);
						WPRINT(gtemp)
					}
				else
					{	ECHO	{	sprintf(gtemp,"!TYPE %s",source);
										WPRINT(gtemp)
									}
						while ( feof(fin)==0 )
							{	fgets(source,PATH_MAX,fin);
								pointer = source+strlen(source)-1;
								while (pointer>=source && isspace(*pointer))
									*pointer--=EOS;
								if (feof(fin)) break;
								WPRINT(source)
								if (++cnt>=wind.max_y-1)
									{	cnt=0;
										key=pause();
									}
								if (key==27) break;
								if ((key=(int)evnt_inkey())==27) break;
							}
						fclose(fin);
					}
				if (key==27) break;
				error=xFsnext();
			}
	}

/*******************************************************************/
/* PRINT																													 */
/*******************************************************************/

void print(char *fname)
	{	char source[PATH_MAX],pfad[PATH_MAX],*start;
		signed int error;
		FILE *fin;
		int key=0;
		long length;

		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(source,start);
		getpfad(source,pfad);
		error=xFsfirst(source,0);
		if (error<0)
			{	sprintf(gtemp,"!PRINT %s",source);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
			}
		while (error>=0)
			{	strcpy(source,pfad);
				strcat(source,dta_buffer.d_fname);
				if ( (fin=fopen(source, "rb"))==NULL )
					{	sprintf(gtemp,tbmsg[49],source);
						WPRINT(gtemp)
					}
				else
					{	ECHO	{	sprintf(gtemp,"!PRINT %s",source);
										WPRINT(gtemp)
									}
						if ( Cprnos() )
							{	length = length_of_file(fin,TRUE);
								while ( length )
									{	putc(getc(fin),stdprn);
										length--;
										if ((key=(int)evnt_inkey())==27) break;
									}
							}
						else
							{	WPRINT(tbmsg[18])
							}
						fclose(fin);
					}
				if (key==27) break;
				error=xFsnext();
			}
	}

/*******************************************************************/
/* CHDIR																													 */
/*******************************************************************/

void do_chdir(char *fname)
	{	char source[PATH_MAX],*start;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}
		strcpy(source,start);

		if (*(source+1)==':')
			Dsetdrv(toupper(*(source))-'A');

		if ( Dsetpath(source)!=0 )
			{	sprintf(gtemp,tbmsg[19],source);
				WPRINT(gtemp)
			}
	}

/*******************************************************************/
/* RENAME																													 */
/*******************************************************************/
	
void do_rename(char *fname)
	{	char source[PATH_MAX],dest[PATH_MAX],spfad[PATH_MAX],
				 dpfad[PATH_MAX],temp[PATH_MAX],*start;
		signed int error;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(2);
				return;
			}

		strcpy(source,start);
		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(3);
				return;
			}

		strcpy(dest,start);
		getpfad(source,spfad);
		getpfad(dest,dpfad);
		error=xFsfirst(source,0);
		if (error<0)
			{	sprintf(gtemp,"!RENAME (MOVE) %s %s",source,dest);
				WPRINT(gtemp)
				WPRINT(tbmsg[47])
			}
		while (error>=0)
			{	strcpy(source,spfad);
				strcat(source,dta_buffer.d_fname);
				if (*(dest+strlen(dest)-1)=='\\')
					{	strcpy(temp,dpfad);
						strcat(temp,dta_buffer.d_fname);
					}
				else
					{	strcpy(temp,dest);
					}
				if (*dpfad==EOS)
					{	strcpy(temp,spfad);
						strcat(temp,dest);
					}

				if (Frename(0,source,temp)!=0)
					{	xerror(10);
						ECHO	{	sprintf(gtemp,tbmsg[20],source,temp);
										WPRINT(gtemp)
									}
					}
				else
					{	ECHO	{	sprintf(gtemp,"!RENAME %s %s",source,temp);
										WPRINT(gtemp)
									}
					}
				error=xFsnext();
			}
	}

/*******************************************************************/
/* DIR																														 */
/*******************************************************************/

void do_dir(char *fname)
	{	char source[PATH_MAX],*start;
		signed int error;
		char flags[7];
		int dflag;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}

		strcpy(source,start);
		error=xFsfirst(source,-1);
		sprintf(gtemp,tbmsg[51],source);
		WPRINT(gtemp)
		WPRINT("")
		if (error<0)
			{	WPRINT(tbmsg[47])
			}
		while (error>=0)
			{	strcpy(flags,"      ");
				dflag=dta_buffer.d_attrib;
				if (dflag & FA_VOLUME) 		flags[0]='v';
				if (dflag & FA_SUBDIR) 		flags[1]='o';
				if (dflag & FA_HIDDEN) 		flags[2]='h';
				if (dflag & FA_READONLY) 	flags[3]='r';
				if (dflag & FA_SYSTEM) 		flags[4]='s';
				if (dflag & FA_ARCHIVE) 	flags[5]='a';
				if (dflag & FA_SUBDIR || dflag & FA_VOLUME)
					sprintf(gtemp," %-12s     <dir>   %s",dta_buffer.d_fname,flags);
				else
					sprintf(gtemp," %-12s   %7ld   %s",dta_buffer.d_fname,dta_buffer.d_length,flags);
				WPRINT(gtemp)
				error=xFsnext();
			}
		WPRINT("")
	}

/*******************************************************************/
/* DUMMYFILE																											 */
/*******************************************************************/

void dummyfile(char *fname)
	{	char source[PATH_MAX],*start;
		FILE *fin;
		start=strtok(fname," ");
		if (start==NULL)
			{	xerror(0);
				return;
			}
		strcpy(source,start);
		if ( (fin=fopen(source,"wb"))==NULL )
			{	sprintf(gtemp,tbmsg[21],source);
				WPRINT(gtemp)
			}
		else
			fclose(fin);
	}

/*******************************************************************/
/* WAITTIME																												 */
/*******************************************************************/

void waittime(char *tim)
	{	char tim1[]="00:00",tim2[10];
		int scan,cont,is_top;
		struct time ZEIT;
		strncpy(tim1,tim,5);
		tim1[2]=':';
		ECHO	{	sprintf(gtemp,tbmsg[52],tim1);
						WPRINT("")
						WPRINT(gtemp)
						WPRINT("")
					}
		level=0;
		for (;;)
			{	scan=0;

				cont = wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER);
				if (wind.evnt & MU_KEYBD)
					scan = wind.kscan;

				if (!cont)
					break;
				if ( (level=scan & 0xff)!=0 )
					break;
				gettime( &ZEIT );
				sprintf(tim2,"%02d:%02d:%02d",ZEIT.ti_hour,ZEIT.ti_min,ZEIT.ti_sec);
				ECHO	{	is_top=TRUE;
								wind_update(BEG_UPDATE);
								is_top = wind_top(&wind);
								wind_update(END_UPDATE);
								if (is_top==TRUE)
									{	sprintf(gtemp," %s ",tim2);
										WWRITE(gtemp)
									}
							}
				if (strncmp(tim1,tim2,5)==0)
					break;
			}
		ECHO	WPRINT("")
	}

void ship(void)
	{	char	device[40];
		int		major,minor;
		long	flags;

		if ( XHGetVersion() >= 0x100)
			{	for (major=0; major<=17; major++)
					for (minor=0; minor<=1; minor++)
						{	if ( XHInqTarget(major,minor,NULL,&flags,device) == 0 )
								if ( (flags & 1) && !(flags & (1<<31)) )
									{	if (major>=18)
											sprintf(gtemp,tbmsg[53],"???",major,minor,device);
										else if (major>=16)
											sprintf(gtemp,tbmsg[53],"IDE",major-16,minor,device);
										else if (major>=8)
											sprintf(gtemp,tbmsg[53],"SCSI",major-8,minor,device);
										else if (major>=0)
											sprintf(gtemp,tbmsg[53],"ACSI",major,minor,device);
										WPRINT(gtemp)
										XHStop(major,minor,TRUE,0);
									}
						}
			}
		else
			{	xerror(9);
			}
	}

void logging(int typ, char *text)
	{	switch(log_it(typ,text))
			{ case	1	:	WPRINT(tbmsg[4])										/* No Statuslog */
									break;
				case	2	:	sprintf(gtemp,tbmsg[17],statuslog);	/* fopen error	*/
									WPRINT(gtemp)
			}
	}

int do_fileselect(char *env)
	{	char in_path[PATH_MAX], sel_file[13]="";
		int	i,ret=2;

		if (*env==EOS)
			{	i=0;
			}
		else
			{	i = atoi(env);
				if (i<0 || i>9)
					{	i=0;
						xerror(4);
					}
			}
		if ( fileselect(in_path,sel_file,"*.*",avaltxt[2]) != 0)
			{	strcat(in_path,sel_file);
				ret=1;
				strcpy(fselname[i],in_path);
				if (exist(in_path)==0)
					ret=0;
			}
		else
			{	*fselname[i] = EOS;
			}
		return(ret);
	}

void do_cutext(char *env)
	{	int i;
		char *pointer;
		if (*env==EOS)
			{	i=0;
			}
		else
			{	i = atoi(env);
				if (i<0 || i>9)
					{	i=0;
						xerror(4);
					}
			}
		pointer=strrchr(fselname[i]+strlen(fselname[i])-4,'.');
		if (pointer!=NULL)
			*pointer=EOS;
	}

void do_cutpath(char *env)
	{	int i;
		char *pointer;
		if (*env==EOS)
			{	i=0;
			}
		else
			{	i = atoi(env);
				if (i<0 || i>9)
					{	i=0;
						xerror(4);
					}
			}
		pointer=strrchr(fselname[i],'\\');
		if (pointer!=NULL)
			strcpy(fselname[i],pointer+1);
	}

void do_input(char *txt,int mode)		/* 0=Input, 1=Preset */
	{	int i;
		char *start,temp[MAX_STR];

		start=strtok(txt," ");
		if (start==NULL)
			{	i=0;
				*temp=EOS;
			}
		else
			{	i = atoi(start);
				if (i<0 || i>9)
					{	i=0;
						xerror(4);
					}
			}

		start=strtok(NULL,"");
		if (start==NULL)
			*temp=EOS;
		else
			strcpy(temp,start);

		if (mode==0)
			wind_input(&wind,temp,fselname[i],WIND_MAX_X);
		else
			strcpy(fselname[i],temp);
	}

void do_convert(char *txt,int mode)		/* 0=3D, 1=4D */
	{	int i,zone,net,node,point;
		char *start,temp[MAX_STR],dum[MAX_STR];

		start=strtok(txt," ");
		if (start==NULL)
			{	i=0;
				*temp=EOS;
			}
		else
			{	i = atoi(start);
				if (i<0 || i>9)
					{	i=0;
						xerror(4);
					}
			}

		start=strtok(NULL,"");
		if (start==NULL)
			*temp=EOS;
		else
			strcpy(temp,start);

		scanaddr(temp,&zone,&net,&node,&point,dum);
		if (mode==0)
			strcpy(fselname[i],addr2file(net,node));
		else
			strcpy(fselname[i],addr2file36(net,node,point));
	}

/*******************************************************************/
/* PRINTERROR																											 */
/*******************************************************************/

void xerror(int errorcode)
	{	switch(errorcode)
			{	case 1	:	sprintf(gtemp,tbmsg[22],line);	break; /* Wrong parameter in line			*/
				case 2	:	sprintf(gtemp,tbmsg[23],line);	break; /* Missing source in line			*/
				case 3	:	sprintf(gtemp,tbmsg[24],line);	break; /*	Missing destination in line */
				case 4	:	sprintf(gtemp,tbmsg[25],line);	break; /*	Parameter out of range			*/
				case 5	:	sprintf(gtemp,tbmsg[26],line);	break; /* Calculation failed in line	*/
				case 6	:	sprintf(gtemp,tbmsg[27],line);	break; /* File already opened in line	*/
				case 7	:	sprintf(gtemp,tbmsg[28],line);	break; /* File open error in line			*/
				case 8	:	sprintf(gtemp,tbmsg[29],line);	break; /* File not open error in line	*/
				case 9	:	sprintf(gtemp,tbmsg[30],line);	break; /* SHIP: No XHDI 1.00 comp.		*/
				case 10	:	sprintf(gtemp,tbmsg[32],line);	break; /* Rename failed in line				*/
				case 11	:	sprintf(gtemp,tbmsg[34],line);	break; /* File not found error in			*/
				case 12	:	sprintf(gtemp,tbmsg[11],line);	break; /* Filelength is zero in line	*/
				case 13	:	sprintf(gtemp,tbmsg[54],line);	break; /* !FILE or parameter missing	*/
				case 14	:	sprintf(gtemp,tbmsg[55],line);	break; /* !AREA or parameter missing	*/
				case 15	:	sprintf(gtemp,tbmsg[56],line);	break; /* !FROM or parameter missing	*/
				case 16	:	sprintf(gtemp,tbmsg[57],line);	break; /* !SEND parameter missing			*/
				case 17	:	sprintf(gtemp,tbmsg[58],line);	break; /* !SUBJECT or parameter miss.	*/
				case 18	:	sprintf(gtemp,tbmsg[59],line);	break; /* !FLAGS or parameter missing	*/
				case 19	: sprintf(gtemp,tbmsg[60],line);	break; /* Cannot recurse batchfiles		*/
				default :	sprintf(gtemp,tbmsg[31],line);				 /* Missing parameter in line		*/
			}
		WPRINT(gtemp)
	}
