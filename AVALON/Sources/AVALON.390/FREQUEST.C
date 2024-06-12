#include <aes.h>
#include <vdi.h>
#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
	#include "avalsemp.rh"
#else
	#include "avalon.h"
	#include "avalon.rh"
#endif
#include "avalonio.h"
#include "avallang.h"
#include "define.h"
#include "diverse.h"
#include "scancode.h"
#include "transkey.h"
#include "form_run.h"
#include "windmini.h"
#include "vars.h"

void	auswahl_box(void);
void	delete_one_file(int xwahl);
void	filelist2filename(const char *filelist,char *filename);
void	file_request(void);
char	*freqblock(int num);
void	insert_new_file(int xwahl);
void	get_files(void);
void	put_f(int flag,int max);
void	put_files(void);
void	lade_request(char *xnode,int xflag);
int		lese_18(FILE *xfile);
void	save_request(char *xnode, int xzone);
void	search(FILE *xfile);
void	slider_redraw(int redraw);
int		request_from_msg(char *name);

extern void smallhelp(int feld);

static char	find[35]="",
						password[11]="",
						files[21][2][21],		/* LÑnge Strings=21, sonst Source anpassen */
						ff[FREQLINES+FREQLINESADD+1][MAX_STR],
						**lines;

static long	lof,last_loc;


/* =============================================================== */
/* =FILE-REQUEST================================================== */
/* =============================================================== */

void save_request(char *xnode, int xzone)
	{	int net2,node2,point2,zone,
				t,i,j,wahl,nx,ny,nw,nh;
		char net[10],node[10],point[10],*pointer,
				 filename[20],file[PATH_MAX],
				 dum[MAX_STR],dum2[21],dum3[MAX_STR];
		long len;
		FILE *freq;

		strncpy(net,xnode,5); net[5]=EOS;
		if (strlen(xnode)>5)
			{	strncpy(node,xnode+5,5); node[5]=EOS;	}
		else
			{	strcpy(node,"0"); }
		if (strlen(xnode)>10)
			{	strncpy(point,xnode+10,4); point[4]=EOS; }
		else
			{	strcpy(point,"0"); }
		net2=(int)strtoul(net,NULL,10);
		node2=(int)strtoul(node,NULL,10);
		point2=(int)strtoul(point,NULL,10);

		if (divflags & D_USE4D)
			sprintf(filename,"%s.ORT",addr2file36(net2,node2,point2));
		else
			sprintf(filename,"%s.REQ",addr2file(net2,node2));

		if (xzone==FALSE)
			{	sprintf(file,"%s%s",hold,filename);
				wahl=REQZSAVE;
			}
		else
			{	put_text(tree81,REQZONE,"",0);
				put_text(tree81,REQDOMAI,"",0);

				setze_presets();

				form_center(tree81,&nx,&ny,&nw,&nh);
				form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
				form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
				objc_draw(tree81,ROOT,MAX_DEPTH,nx,ny,nw,nh);
				do
					{	wahl=form_do(tree81,0) & 0x7FFF;
						undo_state(tree81,wahl,SELECTED);
						if (wahl>=PRESET0 && wahl<=PRESET0+MAX_PRESET-1)
							{	*dum = *dum3 = EOS;
								switch(sscanf(get_text(tree81,wahl),"%s - %s",dum,dum3))
									{	case	0	: *dum = EOS;
										case	1	: *dum3 = EOS;
									}
								put_text(tree81,REQZONE,dum,3);
								put_text(tree81,REQDOMAI,dum3,20);
								redraw_one_objc(tree81,REQZONE,-1);
								redraw_one_objc(tree81,REQDOMAI,-1);
								redraw_one_objc(tree81,wahl,-1);
							}
					}
				while (wahl!=REQZSAVE && wahl!=REQZCAN);
				form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
				form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);

				if (wahl==REQZSAVE)
					{	zone=(int)strtoul(get_text(tree81,REQZONE),NULL,10);
						strcpy(dum,get_text(tree81,REQDOMAI));

						/* Ist es unser Defaultdomain? */

						scanaddr(address[0],&i,&i,&i,&i,dum2);
						if (stricmp(dum2,dum)==0)
							*dum=EOS;

						if (*dum!=EOS)
							{	if (domain_cnt==-1)
									{	form_alert(1,msg[27]);
										wahl=0;
									}
								else
									{	j=-1;
										for (i=0; i<=domain_cnt; i++)
											{	if (stricmp(dum,domain[i][0])==0 ||
														stricmp(dum,domain[i][1])==0)
													{	j=i;
														break;
													}
											}
										if (j==-1)
											{	form_alert(1,msg[28]);
												wahl=0;
											}
										else
											{	strcpy(dum2,domain[j][1]);
												strcpy(dum,hold);

												pointer=strchr(dum,EOS);
												if (pointer>dum)
													*(--pointer)=EOS;
												pointer=strrchr(dum,'\\');
												if (pointer!=NULL)
													*(pointer+1)=EOS;

/*											pointer=dum+strlen(dum)-1;
												*pointer=EOS;
												while (pointer>=dum && *pointer!='\\') *pointer--=EOS;
*/
												strcat(dum,dum2);

												if (divflags & D_USEOUT36)
												{	strcpy(dum2,"000");
													itoa(zone,dum2+3,36);
													strcpy(dum2,dum2+strlen(dum2)-3);
												}
												else
													sprintf(dum2,"%03.3X",zone);

												sprintf(file,"%s.%3.3s\\%s",dum,dum2,filename);
											}
									}
							}
						else
							{	pointer=hold+strlen(hold)-1; *pointer=EOS;

								if (divflags & D_USEOUT36)
								{	strcpy(dum,"000");
									itoa(zone,dum+3,36);
									strcpy(dum,dum+strlen(dum)-3);
								}
								else
									sprintf(dum,"%03.3X",zone);

								sprintf(file,"%s.%3.3s\\%s",hold,dum,filename);
								*pointer='\\';
							}
					}
			}

		if (wahl==REQZSAVE)
			{	freq=fopen(file,"w");
				if (freq!=NULL)
					{	for (t=1; t<=20; t++)
							{	if (*files[t][0]!=EOS)
									{	if (*files[t][1]!=EOS)
											{	sprintf(dum,"%s !%s",files[t][0],files[t][1]);
												output(dum,freq);
											}
										else
											{	output(files[t][0],freq);
											}
									}
							}
						fseek(freq,0L,SEEK_END);
						len = ftell(freq);
						fclose(freq);

						if (len==0)
							remove(file);
					}
				else
					{	fatal_error(avaltxt[3]);
					}
				pipeflags |= P_SEMP_REREAD;
			}
	}

void lade_request(char *xnode,int showalert)
	{	int t,net2,node2,point2;
		char net[10],node[10],point[10],*pointer,
				 filename[PATH_MAX],file[PATH_MAX],dum[MAX_STR];
		FILE *freq;

		for (t=1; t<=20; t++)
			*files[t][0]=*files[t][1]=EOS;

		sprintf(dum,"%-14s",xnode);
		strncpy(net,dum,5); net[5]=EOS;
		if (strlen(dum)>5)
			{	strncpy(node,dum+5,5); node[5]=EOS;	}
		else
			{	strcpy(node,"0"); }
		if (strlen(dum)>10)
			{	strncpy(point,dum+10,4); point[4]=EOS; }
		else
			{	strcpy(point,"0"); }
		net2=(int)strtoul(net,NULL,10);
		node2=(int)strtoul(node,NULL,10);
		point2=(int)strtoul(point,NULL,10);

		if (divflags & D_USE4D)
			sprintf(filename,"%s.ORT",addr2file36(net2,node2,point2));
		else
			sprintf(filename,"%s.REQ",addr2file(net2,node2));

		t=0;
		sprintf(file,"%s%s",hold,filename);
		if (exist(file)==0)
			{	freq=fopen(file,"r");
				while(feof(freq)==0)
					{	input(dum,MAX_STR,freq);
						if (*dum!=';')
							{	t++;

/* Wenn es kein Paûwort gibt, dann kann mit "strrchr" der Pfad
	 gesucht werden, ansonsten ab Paûwort rÅckwÑrts suchen,,,
*/
								pointer=strstr(dum," !");
								if (pointer==NULL)
									{	pointer=strrchr(dum,'\\');
									}
								else
									{	while (--pointer>=dum && *pointer!='\\');
										if (*pointer!='\\')
											pointer=NULL;
									}
								if (pointer!=NULL)
									strcpy(dum,pointer+1);

								pointer=strstr(dum," !");
								if (pointer!=NULL)
									{	*(pointer++)=EOS;
										pointer++;
										strncpy(files[t][0],dum,20);
										strncpy(files[t][1],pointer,20);
									}
								else
									{	strncpy(files[t][0],dum,20);
										*files[t][1]=EOS;
									}
							}
					}
				fclose(freq);
			}
		else
			{	if (showalert)	form_alert(1,msg[13]);
			}
	}

void get_files(void)
	{	register int t;
		for (t=PROG1; t<=PROG10; t+=2)	strcpy(files[(t-PROG1)/2+1][0],strupr(get_text(tree8,t)));
		for (t=PROG11; t<=PROG20; t+=2)	strcpy(files[(t-PROG11)/2+11][0],strupr(get_text(tree8,t)));
		for (t=PASS1; t<=PASS10; t+=2)	strcpy(files[(t-PASS1)/2+1][1],get_text(tree8,t));
		for (t=PASS11; t<=PASS20; t+=2)	strcpy(files[(t-PASS11)/2+11][1],get_text(tree8,t));
	}

void put_files(void)
	{	register int t;
		for (t=PROG1; t<=PROG10; t+=2)	put_text(tree8,t,files[(t-PROG1)/2+1][0],12);
		for (t=PROG11; t<=PROG20; t+=2)	put_text(tree8,t,files[(t-PROG11)/2+11][0],12);
		for (t=PASS1; t<=PASS10; t+=2)	put_text(tree8,t,files[(t-PASS1)/2+1][1],10);
		for (t=PASS11; t<=PASS20; t+=2)	put_text(tree8,t,files[(t-PASS11)/2+11][1],10);
	}

int lese_18(FILE *xfile)
	{	int i,max=0;
		char dum[512];
		if (last_loc!=lof)
			{	while ( (feof(xfile)==0) && (max<FREQLINES+Add_Lines) )
					{	*dum=EOS;
						fgets(dum, 511, xfile);
						i = (int)strlen(dum);
						if (i>1)
							{	if (dum[i-2]=='\r' || dum[i-2]=='\n')
									{	dum[i-2]=EOS;	}
								else
									{	if (dum[i-1]=='\r' || dum[i-1]=='\n')
											{	dum[i-1]=EOS;	}
									}
							}
						else
							{	if (i>0)
									{	if (dum[i-1]=='\r' || dum[i-1]=='\n')
											{	dum[i-1]=EOS;	}
									}
							}
						dum[FREQLEN]=EOS;
						strcpy(ff[++max],dum);
					}
			}
		if (max<FREQLINES+Add_Lines)
			for (i=max+1; i<=FREQLINES+Add_Lines; i++)
				strcpy(ff[i]," ");
		return(max);
	}

void filelist2filename(const char *filelist,char *filename)
	{	char *pointer,*pointer2,temp[MAX_STR+1];
	
		sprintf(temp,"%-*s",FREQLEN,filelist);

/* Alle Leerzeichen und Sonderzeichen Åberspringen */

		pointer=temp;
		while ( *pointer!=EOS && (*pointer<=' ' || *pointer>127 ) )
			pointer++;
		strcpy(temp,pointer);
		temp[12]=EOS;

/* Alle Leerzeichen am Anfang und Ende abschneiden */

		trimstr(temp);

/* Gibt es einen "."? Nein, dann ab Leerzeichen den String beenden */

		if (strchr(temp,'.')==NULL)
			{	pointer=temp;
				while (*pointer!=EOS && *pointer!=' ')
					pointer++;
				*pointer=EOS;
			}

/* Alle Leerzeichen in einem String entfernen */

		pointer = pointer2 = temp;
		do
			{	while (*pointer2==' ')
					pointer2++;
				*pointer = *pointer2++;
			}
		while (*pointer++!=EOS);

/* RÅckgabe des Filenamens */

		strcpy(filename,temp);
	}

void put_f(int flag,int max)
	{	int t,j,l;
		char dum[81];
	
		for (t=FNAME1; t<=FNAME1+FREQLINES+Add_Lines-1; t++)
			{	if (max>=1)
					{	l=FALSE;
						filelist2filename(ff[t-FNAME1+1],dum);
						if (*dum!=EOS)
							{	for (j=1; j<=20; j++)
									{	if (*files[j][0]!=EOS)
											{	if (strnicmp(dum,files[j][0],strlen(files[j][0]))==0)
													{	l=TRUE;
														do_state(tree9,t,SELECTED);
													}
											}
									}
							}
						if (l==FALSE) undo_state(tree9,t,SELECTED);
						put_string(tree9,t,ff[t-FNAME1+1],FREQLEN);
					}
				else
					{	sprintf(dum,"%*s",FREQLEN," ");
						undo_state(tree9,t,SELECTED);
						put_string(tree9,t,dum,FREQLEN);
					}
			}
		if (flag>=1) redraw_one_objc(tree9,FBOX,handle);
	}

char *freqblock(int num)
	{	char *pointer;
		switch(num)
			{	case	0	: pointer="<1..>"; break;
				case	1	: pointer="<.2.>"; break;
				case	2	: pointer="<..3>"; break;
			}
		return(pointer);
	}

void file_request(void)
	{	long net2,node2,point2;
		int net,node,point;
		char *pointer,temp[255],temp2[255],file[15];
		int error,t,xwahl,wahl,wahltemp,nx,ny,nw,nh,feld,pos,len4d;
		static char ptext1[] ="12345678901234",
								ptmplt1[]="Node: _____/_____.____",
								pvalid1[]="99999999999999",
								ptext2[] ="1234567890",
								ptmplt2[]="Node: _____/_____",
								pvalid2[]="9999999999";
 
		if (divflags & D_USE4D)
			{	put_template(tree8,REQNODE,ptext1,ptmplt1,pvalid1);
				len4d = 14;
			}
		else
			{	put_template(tree8,REQNODE,ptext2,ptmplt2,pvalid2);
				len4d = 10;
			}

		trimright(filelistnode[fileblock][0]);
		put_text(tree8,REQNODE,filelistnode[fileblock][0],len4d);

		for (t=0; t<=MAX_GETFILE-1; t++)
			put_text(tree8,GETFILE1+t,getfile[t],22);

		lade_request(filelistnode[fileblock][0],0);
		put_files();

		put_text(tree8,REQBLOCK,freqblock(fileblock),5);
		for (t=1; t<=9; t++)
			if (*filetext[fileblock][t]==EOS)
				put_text(tree8,t+FILELST0,"           ",11);
			else
				put_text(tree8,t+FILELST0,filetext[fileblock][t],11);

		for (t=GETNODE1; t<=GETNODE0; t++)
			{	OB_FLAGS(tree8,t) |= HIDETREE;
				OB_STATE(tree8,t) &= ~DISABLED;
			}

		for (t=0; t<=9; t++)
			undo_state(tree8,FILELST0+t,SELECTED);
		do_state(tree8,FILELST0+filebutton,SELECTED);

		xform_dial(tree8,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	for (t=GETNODE1; t<=GETNODE0; t++)
					OB_FLAGS(tree8,t) |= HIDETREE;

				t=GETNODE1;
				sprintf(temp,"%s\*.REQ",hold);
				error=xFsfirst(temp,0);
				while (error>=0)
					{	strcpy(file,dta_buffer.d_fname);
						pointer = strchr(file,'.');
						if ( pointer!=NULL )
							{	file2addr(file,&net,&node);
								sprintf(temp2,"%u/%u",net,node);
								put_text(tree8,t,temp2,11);
								OB_FLAGS(tree8,t) &= ~HIDETREE;
								if (divflags & D_USE4D)
									OB_STATE(tree8,t) |= DISABLED;
								else
									OB_STATE(tree8,t) &= ~DISABLED;
								t++;
								if (t>GETNODE0) break;
								error=xFsnext();
							}
					}

				sprintf(temp,"%s\*.ORT",hold);
				error=xFsfirst(temp,0);
				while (error>=0 && t<=GETNODE0)
					{	strcpy(file,dta_buffer.d_fname);
						pointer = strchr(file,'.');
						if ( pointer!=NULL )
							{	file2addr36(file,&net,&node,&point);
								if (point!=0)
									sprintf(temp2,"%u/%u.%u",net,node,point);
								else
									sprintf(temp2,"%u/%u",net,node);
								put_text(tree8,t,temp2,16);
								OB_FLAGS(tree8,t) &= ~HIDETREE;
								if (divflags & D_USE4D)
									OB_STATE(tree8,t) &= ~DISABLED;
								else
									OB_STATE(tree8,t) |= DISABLED;
								t++;
								if (t>GETNODE0) break;
								error=xFsnext();
							}
					}

				redraw_one_objc(tree8,REQBOXES,handle);
				feld=0;
				do
					{	xwahl=xform_do(handle,tree8,NULL,feld,0,trans_key4,&wnx,&wny,&wnw,&wnh);
						if (xwahl & 0x4000)
							xwahl = REQCANCE;
						wahl = xwahl & 0x7ff;
						if (wahl==1041)
							smallhelp(HELPREQ);

						get_files();

						if ( wahl<1000 )
							if ( (OB_FLAGS(tree8,wahl) & EDITABLE) && (wahl!=feld) )
								feld=wahl;

						if (xwahl & 0x8000)
							{	if (wahl>=GETNODE1 && wahl<=GETNODE0)
									{	undo_state(tree8,wahl,SELECTED);
										for (t=1; t<=20; t++)
											*files[t][0]=*files[t][1]=EOS;
										put_files();
										strcpy(temp,get_text(tree8,wahl));
										point2=0;
										if (strchr(temp,'.')!=NULL)
											{	sscanf(temp,"%lu/%lu.%lu",&net2,&node2,&point2);
												sprintf(filelistnode[fileblock][0],"%5lu%-5lu%-4lu",net2,node2,point2);
											}
										else
											{	sscanf(temp,"%lu/%lu",&net2,&node2);
												sprintf(filelistnode[fileblock][0],"%5lu%-5lu",net2,node2);
											}
										trimright(filelistnode[fileblock][0]);
										put_text(tree8,REQNODE,filelistnode[fileblock][0],len4d);
										redraw_one_objc(tree8,REQNODE,handle);
										wahl=REQSAVE;
									}
								if (wahl>=PROG1 && wahl<=PROG10) 
									{	pos=((wahl-PROG1)/2)+1;
										for (t=pos; t<=19; t++)
											{	strcpy(files[t][0],files[t+1][0]);
												strcpy(files[t][1],files[t+1][1]);
											}
										*files[20][0]=
										*files[20][1]=EOS;
										put_files();
										redraw_one_objc(tree8,REQLEFT,handle);
										redraw_one_objc(tree8,REQRIGHT,handle);
									}
					      if (wahl>=PROG11 && wahl<=PROG20)
					      	{	pos=((wahl-PROG11)/2)+1;
					      		if (pos==10)
					      			{	*files[20][0]=
					      				*files[20][1]=EOS;
					      			}
					      		else
					      			{	for (t=pos; t<=9; t++)
													{	strcpy(files[t+10][0],files[t+11][0]);
														strcpy(files[t+10][1],files[t+11][1]);
													}
												*files[20][0]=
												*files[20][1]=EOS;
											}
										put_files();
										redraw_one_objc(tree8,REQLEFT,handle);
										redraw_one_objc(tree8,REQRIGHT,handle);
					      	}
					      if (wahl>=FILELST1 && wahl<=FILELST9)
					      	{	form_center(tree82,&nx,&ny,&nw,&nh);
										form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
										form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
										put_text(tree82,FILETEXT,filetext[fileblock][wahl-FILELST0],11);
										put_text(tree82,FILEPATH,filelist[fileblock][wahl-FILELST0],65);
										put_text(tree82,FILENODE,filelistnode[fileblock][wahl-FILELST0],14);
										objc_draw(tree82,ROOT,MAX_DEPTH,nx,ny,nw,nh);
										wahltemp=0;
										do
											{	wahltemp=form_do(tree82,wahltemp) & 0x8fff;
												if (wahltemp & 0x8000)
													{	do_select("*.*",FILEPATH,tree82,65);
														redraw_one_objc(tree82,FILEPATH,-1);
														undo_state(tree82,wahltemp & 0x7ff,SELECTED);
														wahltemp=FILEOK;
													}
												undo_state(tree82,wahltemp,SELECTED);
											}
										while (wahltemp!=FILEOK);
										strcpy(filetext[fileblock][wahl-FILELST0],get_text(tree82,FILETEXT));
										if (*filetext[fileblock][wahl-FILELST0]==EOS)
											put_text(tree8,wahl,"           ",11);
										else
											put_text(tree8,wahl,filetext[fileblock][wahl-FILELST0],11);
										strcpy(filelist[fileblock][wahl-FILELST0],get_text(tree82,FILEPATH));
										strcpy(filelistnode[fileblock][wahl-FILELST0],get_text(tree82,FILENODE));

										if ( strlen(filelist[fileblock][wahl-FILELST0])>=4 )
											if (stricmp(filelist[fileblock][wahl-FILELST0]+strlen(filelist[fileblock][wahl-FILELST0])-4,".HDR")==0 ||
													stricmp(filelist[fileblock][wahl-FILELST0]+strlen(filelist[fileblock][wahl-FILELST0])-4,".MSG")==0 )
												{	*(filelist[fileblock][wahl-FILELST0]+strlen(filelist[fileblock][wahl-FILELST0])-4) = EOS;
												}

										form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
										form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
										redraw_one_objc(tree8,REQBOX2,handle);
										changes |= C_FILELIST;
					      	}
					    }
					  else
					  	{	if (wahl<FILELST0 || wahl>FILELST9)
									if (wahl<1000)
										undo_state(tree8,wahl,SELECTED);
								if (wahl>=FILELST0 && wahl<=FILELST9)
									{	OB_STATE(tree8,filebutton+FILELST0) &= ~SELECTED;
										OB_STATE(tree8,wahl) |= SELECTED;
										redraw_one_objc(tree8,filebutton+FILELST0,handle);
									}
							}
						strcpy(filelistnode[fileblock][0],get_text(tree8,REQNODE));
						filelistnode[fileblock][0][len4d]=EOS;
						for (t=0; t<=MAX_GETFILE-1; t++)
							strcpy(getfile[t],get_text(tree8,GETFILE1+t));
						for (t=0; t<=9; t++)
							if (is_state(tree8,FILELST0+t,SELECTED))
								filebutton=t;

						if ( (wahl<PROG1 || wahl>PROG20) && /*wahl!=REQSELEC &&*/ wahl<1000)
							redraw_objc(tree8,wahl,handle);

						switch(wahl)
							{	case REQBLOCK	:	{	if (++fileblock>FREQSETUPS-1)
																		fileblock=0;
																	put_text(tree8,REQBLOCK,freqblock(fileblock),5);
																	for (t=1; t<=9; t++)
																		if (*filetext[fileblock][t]==EOS)
																			put_text(tree8,t+FILELST0,"           ",11);
																		else
																			put_text(tree8,t+FILELST0,filetext[fileblock][t],11);
																	redraw_one_objc(tree8,REQBLOCK,handle);
																	redraw_one_objc(tree8,REQBOX2,handle);
																} break;
								case FILELST1	:
								case FILELST2	:
								case FILELST3	:
								case FILELST4	:
								case FILELST5	:
								case FILELST6	:
								case FILELST7	:
								case FILELST8	:
								case FILELST9	:	{	put_text(tree8,REQNODE,filelistnode[fileblock][wahl-FILELST0],len4d);
																	redraw_one_objc(tree8,REQNODE,handle);
																}	break;
								case GETFILE1	:
								case GETFILE2	:	
								case GETFILE3 :
								case GETFILE4	:	{	pos=0;
																	for (t=1; t<=20; t++)
																		{	if (*files[t][0]==EOS)
																				{	pos=t;
																					break;
																				}
																		}
																	if (pos==0)
																		{	form_alert(1,msg[14]);
																		}
																	else
																		{	strcpy(temp2,getfile[wahl-GETFILE1]);
																			strncpy(temp,temp2,12); temp[12]=EOS;
																			trimstr(temp);
																			strcpy(files[pos][0],temp);
																			strupr(files[pos][0]);
																			if (strlen(temp2)>12)
																				{	pointer = temp2+12;
																					trim(&pointer);
																					strcpy(files[pos][1],pointer);
																				}
																			else
																				{	*files[pos][1]=EOS;
																				}
																			put_files();
																		}
																} break;
								case REQLOAD	:	{	strcpy(filelistnode[fileblock][0],get_text(tree8,REQNODE));
																	if (strlen(filelistnode[fileblock][0])<6)
																		{	form_alert(1,msg[15]);
																		}
																	else
																		{	lade_request(filelistnode[fileblock][0],1);
																			put_files();
																		}
																}	break;
								case REQFILES	:	{	pos=0;
																	for (t=1; t<=20; t++)
																		{	if (*files[t][0]==EOS)
																				{	pos=t;
																					break;
																				}
																		}
																	if (pos==0)
																		{	form_alert(1,msg[14]);
																		}
																	else
																		{	strcpy(files[pos][0],"FILES");
																			*files[pos][1]=EOS;
																			put_files();
																		}
																} break;
								case REQCLEAR	:	{ for (t=1; t<=20; t++)
																		*files[t][0]=*files[t][1]=EOS;
																	put_files();
																}	break;
								case GETNODE1	:
								case GETNODE2	:
								case GETNODE3	:
								case GETNODE4	:
								case GETNODE5	:
								case GETNODE6	:
								case GETNODE7	:
								case GETNODE8	:
								case GETNODE9	:
								case GETNODE0	:	{	strcpy(temp,get_text(tree8,wahl));
																	point2=0;
																	if (strchr(temp,'.')!=NULL)
																		{	sscanf(temp,"%lu/%lu.%lu",&net2,&node2,&point2);
																			sprintf(filelistnode[fileblock][0],"%5lu%-5lu%-4lu",net2,node2,point2);
																		}
																	else
																		{	sscanf(temp,"%lu/%lu",&net2,&node2);
																			sprintf(filelistnode[fileblock][0],"%5lu%-5lu",net2,node2);
																		}
																	trimright(filelistnode[fileblock][0]);
																	put_text(tree8,REQNODE,filelistnode[fileblock][0],len4d);
																	redraw_one_objc(tree8,REQNODE,handle);
																	lade_request(filelistnode[fileblock][0],0);
																	put_files();
																}	break;
								case REQSELEC	:	{	get_files();
																	auswahl_box();
																	if ( strncmp(get_text(tree8,FILELST0+filebutton),"???????????",11)==0)
																		put_text(tree8,FILELST0+filebutton,"? ? ?",11);
																	put_files();
																}	break;
								case REQSAVE	:	{	if (strlen(filelistnode[fileblock][0])<6)
																		{	form_alert(1,msg[15]);
																		}
																	else
																		{	get_files();
																			save_request(filelistnode[fileblock][0],FALSE);
																		}
																}	break;
								case REQSAVEZ	:	{	if (strlen(filelistnode[fileblock][0])<6)
																		{	form_alert(1,msg[15]);
																		}
																	else
																		{	get_files();
																			save_request(filelistnode[fileblock][0],TRUE);
																		}
																}
							}
						if (wahl==GETFILE1 || wahl==GETFILE2 || wahl==GETFILE3 || wahl==GETFILE4 ||
								wahl==REQLOAD	 || wahl==REQFILES || wahl==REQCLEAR ||
								wahl==GETNODE1 || wahl==GETNODE2 || wahl==GETNODE3 ||
								wahl==GETNODE4 || wahl==GETNODE5 || wahl==GETNODE6 ||
								wahl==GETNODE7 || wahl==GETNODE8 || wahl==GETNODE9 ||
								wahl==GETNODE0 || wahl==REQSAVE	 || wahl==REQSAVEZ )
							{	redraw_one_objc(tree8,REQLEFT,handle);
								redraw_one_objc(tree8,REQRIGHT,handle);
							}
/*					if ( (wahl<PROG1 || wahl>PROG20) && wahl<1000)
							redraw_objc(tree8,wahl,handle); */
					}
				while ( (wahl!=REQCANCE) && (wahl!=REQSAVE) );
			}
		while (wahl!=REQCANCE);
		xform_maindialog(XFMD_DIALOG);
	}

/* =============================================================== */
/* =FILE-REQUEST-AUSWAHLBOX======================================= */
/* =============================================================== */

int msgbase_exist(char *name)
	{	char temp[MAX_STR];
		int	ret=FALSE;
		strcpy(temp,name);
		if (strrchr(temp+strlen(temp)-4,'.')==NULL)
			{	strcat(temp,".HDR");
				if (exist(temp)==0)
					{	strcpy(temp,name);
						if (strrchr(temp+strlen(temp)-4,'.')==NULL)
							{	strcat(temp,".MSG");
								if (exist(temp)==0)
									ret=TRUE;
							}
					}
			}
		return(ret);
	}

void slider_redraw(int redraw)
	{	xredraw_slider(tree9,handle,FILEFULL+Add_Lines,
																FILEBK+Add_Lines,
																FILEFD+Add_Lines,lof,last_loc+1,
																(Add_Lines+18)*FREQLEN,redraw);
	}

void auswahl_box(void)
	{	char pfad[MAX_STR],datei[MAX_STR],dum[MAX_STR],*pointer;
		int button=1,bit,xwahl,wahl,wahltemp,nx,ny,nw,nh,max=0,
				msgbaseflag;
		FILE *ffile;

		if ((divflags & D_MAXIWIND) && (max_y>=672))
			{	Add_Lines=FREQLINESADD;
				tree9=rs_trindex[DIALOG9B];
			}
		else
			{	Add_Lines=0;
				tree9=rs_trindex[DIALOG9];
			}

		strcpy(dum,filelist[fileblock][filebutton]);
		if (*dum==EOS)
			{	sprintf(dum,"%s*.*",def_path_slash);
				strcpy(pfad,def_path);
				strcpy(datei,"*.*");
			}
		else
			{	pointer=strrchr(dum,'\\');
				if (pointer==NULL)
					{	strcpy(pfad,def_path);
						strcpy(datei,"*.*");
					}
				else
					{	*(pointer++)=EOS;
						strcpy(pfad,dum);
						if (*pointer!=EOS)
							strcpy(datei,pointer);
						else
							*datei=EOS;
						*(--pointer)='\\';
					}
			}

		strcat(pfad,"\\*.*");

		msgbaseflag=msgbase_exist(filelist[fileblock][filebutton]);

		if ( (filebutton==0) ||
				 (exist(filelist[fileblock][filebutton])!=0 && msgbaseflag==FALSE) )
			{	if ( aes_version < XFSELECT )
					fsel_input(pfad, datei, &button);
				else
					fsel_exinput(pfad, datei, &button, avaltxt[4]);
				if (filebutton!=0 && button!=0)
					{	strcpy(filetext[fileblock][filebutton],"? ? ?");
						put_text(tree8,FILELST0+filebutton,"???????????",11);
						changes |= C_FILELIST;
					}
				if (button!=0)
					{	if ( strlen(datei)>=4 )
							if (stricmp(datei+strlen(datei)-4,".HDR")==0 ||
									stricmp(datei+strlen(datei)-4,".MSG")==0 )
								{	*(datei+strlen(datei)-4) = EOS;
									pointer=strrchr(pfad,'\\');
									if (pointer!=NULL)
										*(++pointer)=EOS;
									strcpy(filelist[fileblock][filebutton],pfad);
									strcat(filelist[fileblock][filebutton],datei);
									msgbaseflag=msgbase_exist(filelist[fileblock][filebutton]);
								}
					}
			}

		if (button!=0)
			{	pointer=strrchr(pfad,'\\');
				if (pointer!=NULL)
					*(++pointer)=EOS;
				strcpy(filelist[fileblock][filebutton],pfad);
				strcat(filelist[fileblock][filebutton],datei);

				if (msgbaseflag)
					{	xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
						if (xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
							form_alert(1,msg[23]);
						if (request_from_msg(filelist[fileblock][filebutton]))
							strcpy(filelistnode[fileblock][filebutton],get_text(tree8,REQNODE));
						if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
							form_alert(1,msg[23]);
						xform_dial(tree8,XFMD_START,&handle,&wnx,&wny,&wnw,&wnh);
					}

				if (!msgbaseflag)
					{	if (exist(filelist[fileblock][filebutton])==0)
							{	ffile=fopen(filelist[fileblock][filebutton],"rb");
								if (ffile==NULL)
									{	fatal_error(filelist[fileblock][filebutton]);
									}
								else
									{	setvbuf(ffile,NULL,_IOFBF,PHYS_BUFFER);
										last_loc=0;
										lof = length_of_file(ffile,TRUE);
										max = lese_18(ffile);
										put_f(0,max);
										slider_redraw(FALSE);
		
										xform_dial(tree9,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
										do
											{	xwahl=xform_do(handle,tree9,NULL,0,0,trans_key1,&wnx,&wny,&wnw,&wnh);
												if (xwahl & 0x4000)
													xwahl = FOK+Add_Lines;
												wahl = xwahl & 0x7ff;
				
												if (wahl<100)
													{	bit=is_state(tree9,wahl,SELECTED);
														if ( (wahl<FNAME1) || (wahl>FNAME1+FREQLINES+Add_Lines-1) )
															undo_state(tree9,wahl,SELECTED);
													}
		
												if (wahl==FILEBK+Add_Lines)
													{	objc_offset(tree9,FILEFD+Add_Lines,&nx,&ny);
														vq_mouse(vdi_handle,&wahltemp,&nw,&nh);
														if (nw<nx)
															xwahl=wahl=FILEDN+Add_Lines;
														if (nw>nx+OB_W(tree9,FILEFD+Add_Lines))
															xwahl=wahl=FILEUP+Add_Lines;
													}
		
												if (Add_Lines==0)
													{	switch(wahl)
															{	case FILEUP		: wahl=1010;	break;
																case FILEUPUP	: wahl=1011;	break;
																case FILEDN		: wahl=1012;	break;
																case FILEDNDN	: wahl=1013;	break;
																case FILEFD		: wahl=1014;	break;
															}
													}
												else
													{	switch(wahl)
															{	case FILEUP		+FREQLINESADD	: wahl=1010;	break;
																case FILEUPUP +FREQLINESADD	: wahl=1011;	break;
																case FILEDN   +FREQLINESADD	: wahl=1012;	break;
																case FILEDNDN +FREQLINESADD	: wahl=1013;	break;
																case FILEFD		+FREQLINESADD	: wahl=1014;	break;
															}
													}
		
												switch(wahl)
													{	case 1000			:	{	smallhelp(HELPLST);
																						}	break;
														case 1003			: {	last_loc=0;
																							slider_redraw(TRUE);
																							rewind(ffile);
																							max = lese_18(ffile);
																							put_f(1,max);
																						} break;
														case 1004			:	{	last_loc=lof;
																							fseek(ffile,last_loc,SEEK_SET);
																							slider_redraw(TRUE);
																							max = lese_18(ffile);
																							put_f(1,max);
																						}	break;
														case 1010			:	{	if (last_loc<lof)
																								{	last_loc=ftell(ffile);
																									slider_redraw(TRUE);
																									max = lese_18(ffile);
																									put_f(1,max);
																								}
																						}	break;
														case 1011			:	{	if (last_loc<lof)
																								{	last_loc+=10240;
																									if (last_loc>lof) last_loc=lof;
																									fseek(ffile,last_loc,SEEK_SET);
																									if (last_loc<lof) input(dum,MAX_STR,ffile);
																									slider_redraw(TRUE);
																									max = lese_18(ffile);
																									put_f(1,max);
																								}
																						}	break;
														case 1012			:	{	if (last_loc>0)
																								{	if (last_loc<1024)
																										last_loc=0;
																									else
																										last_loc-=1024;
																									slider_redraw(TRUE);
																									fseek(ffile,last_loc,SEEK_SET);
																									if (last_loc>0) input(dum,MAX_STR,ffile);
																									max = lese_18(ffile);
																									put_f(1,max);
																								}
																						}	break;
														case 1013			:	{	if (last_loc>0)
																								{	if (last_loc<10240)
																										last_loc=0;
																									else
																										last_loc-=10240;
																									slider_redraw(TRUE);
																									fseek(ffile,last_loc,SEEK_SET);
																									if (last_loc>0) input(dum,MAX_STR,ffile);
																									max = lese_18(ffile);
																									put_f(1,max);
																								}
																						}	break;
														case 1014			:	{	if (lof<2000000L)
																								last_loc=(lof*(long)graf_slidebox(tree9,FILEBK+Add_Lines,FILEFD+Add_Lines,0))/1001L+1;
																							else
																								last_loc=(lof/1001L)*(long)graf_slidebox(tree9,FILEBK+Add_Lines,FILEFD+Add_Lines,0)+1;
																							if (last_loc<0) last_loc=0;
																							if (last_loc>lof) last_loc=lof;
																							fseek(ffile,last_loc,SEEK_SET);
																							if ( (last_loc>0) && (last_loc<lof) ) input(dum,MAX_STR,ffile);
																							slider_redraw(TRUE);
																							max = lese_18(ffile);
																							put_f(1,max);
																						}	break;
														case 1001			:	{	put_text(tree14,FINDTEXT,find,30);
																							form_center(tree14,&nx,&ny,&nw,&nh);
																							form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																							form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																							objc_draw(tree14,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																							wahltemp=form_do(tree14,0) & 0x7FFF;
																							undo_state(tree14,wahltemp,SELECTED);
																							form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																							form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
																							if (wahltemp==FINDOK)
																								{ strcpy(find,get_text(tree14,FINDTEXT));
																									strupr(find);
																									if (last_loc==lof)
																										form_alert(1,msg[17]);
																									else
																										search(ffile);
																									slider_redraw(TRUE);
																									max = lese_18(ffile);
																									put_f(1,max);
																								}
																						}	break;
														case 1002			:	{	if (*find==EOS)
																								{	form_alert(1,msg[18]);
																								}
																							else
																								{	if (last_loc>=lof)
																										{	form_alert(1,msg[17]);
																										}
																									else
																										{	search(ffile);
																											slider_redraw(TRUE);
																											max = lese_18(ffile);
																											put_f(1,max);
																										}
																								}
																						}	break;
													}
												if (wahl>=FNAME1 && wahl<=FNAME1+FREQLINES+Add_Lines-1)
													{	*password=EOS;
														if (bit!=0)
															insert_new_file(xwahl);
														else
															delete_one_file(xwahl);
													}
											}
										while (wahl!=FOK+Add_Lines);
										fclose(ffile);
										xform_dial(tree8,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
									}
							}
						else
							{	file_not_found();
							}
					} /* endif (tryfilelist) */
			} /* endif (buton) */
	}

void insert_new_file(int xwahl)
	{	int pos=0,t,wahl,wahltemp,nx,ny,nw,nh;
		char txt[MAX_STR];
		wahl=xwahl & 0x7ff;
		if (xwahl & 0x8000)
			{	put_text(tree10,PASSEDIT,"",10);
				form_center(tree10,&nx,&ny,&nw,&nh);
				form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
				form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
				objc_draw(tree10,ROOT,MAX_DEPTH,nx,ny,nw,nh);
				wahltemp=form_do(tree10,0) & 0x7FFF;
				undo_state(tree10,wahltemp,SELECTED);
				strcpy(password,get_text(tree10,PASSEDIT));
				strupr(password);
				form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
				form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
			}
		for (t=1; t<=20; t++)
			{	if ( *files[t][0]==EOS )
					{	pos=t;
						break;
					}
			}
		if (pos==0)
			{	undo_state(tree9,wahl,SELECTED);
				redraw_objc(tree9,wahl,handle);
				form_alert(1,msg[14]);
			}
		else
			{	filelist2filename(ff[wahl-FNAME1+1],txt);
				strncpy(files[pos][0],txt,20);
				strncpy(files[pos][1],password,20);
			}
	}

void delete_one_file(int xwahl)
	{	char txt[MAX_STR];
		int t,tt,wahl;
		wahl=xwahl & 0x7ff;
		filelist2filename(ff[wahl-FNAME1+1],txt);
		for (t=1; t<=20; t++)
			{	if ( stricmp(files[t][0],txt)==0 )
					{	if (t==20)
							{	*files[20][0]=
								*files[20][1]=EOS;
							}
						else
							{	for (tt=t; tt<=19; tt++)
									{	strcpy( files[tt][0],files[tt+1][0] );
										strcpy( files[tt][1],files[tt+1][1] );
									}
							}
					}
			}
	}

void search(FILE *xfile)
	{	int i=0;
		char dum[512];
		fseek(xfile,last_loc,SEEK_SET);
		if ( feof(xfile)==0 )
			input(dum,MAX_STR,xfile);
		while( feof(xfile)==0 )
			{	if (i++>100)
					{	i=0;
						slider_redraw(TRUE);
					}
				last_loc=ftell(xfile);
				*dum=EOS;
				fgets(dum, 511, xfile);
				strupr(dum);
				if ((int)evnt_inkey()==27) break;
				if (strstr(dum,find)!=NULL) break;
			}
		fseek(xfile,last_loc,SEEK_SET);
	}

/* =============================================================== */
/* =FILE-REQUEST (MSGBASE)======================================== */
/* =============================================================== */

void set_hslide(WIND *wind,int aktmsg,int maxmsg);
void set_vslide(WIND *wind,int aktline,int maxline);
void redraw_screen(WIND *wind,int firstline,int maxline);

#define BPRINT(x) wind_print(&wind,x)
#define BWRITE(x) wind_write(&wind,x)
#define INFOZEILE	{	sprintf(temp," (%u%s) %s%s%s   (%s)", \
																			 msg_on_screen+1, \
																			 header.wFlags & MF_DELETED ? ",DEL" : "", \
																			 header.szFrom, \
																			 *fromaddr!=EOS ? " on " : "", \
																			 fromaddr, \
																			 lastfile); \
										wind_set(wind.handle,WF_INFO,temp); \
									}

int request_from_msg(char *name)
	{	FILE *fhdr,*fmsg;
		register int linescnt;
		int	i,chr,multimsg,xpos,ypos,xend,msg_on_screen,readmsg,maxmsg,
				x1,x2,x3,x4,pos,firstline;
		char *buffer,*pointer,temp[MAX_STR],
				 select[MAX_STR],fromaddr[MAX_STR],
				 name_hdr[PATH_MAX],name_msg[PATH_MAX],
				 lastfile[21];

		sprintf(name_hdr,"%s.HDR",name);
		sprintf(name_msg,"%s.MSG",name);

		wind_init_full(&wind,vdi_handle,VSLIDE|HSLIDE|UPARROW|DNARROW|LFARROW|RTARROW|INFO|NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
		handle=wind_create(VSLIDE|HSLIDE|UPARROW|DNARROW|LFARROW|RTARROW|INFO|NAME|CLOSER|MOVER|FULLER,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
		if (handle<0)
			{	form_alert(1,msg[23]);
				return(FALSE);
			}
		wind.handle=handle;
		wind_set(wind.handle,WF_NAME," AVALON-REQUEST ");
		wind_set(wind.handle,WF_INFO,"");
		wind_open(wind.handle,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
		wind_update(BEG_UPDATE);
		wind_cls(&wind);
		wind_update(END_UPDATE);

		fhdr=fopen(name_hdr,"rb");
		if (fhdr==NULL)
			{	fatal_error(name_hdr);
				wind_close(wind.handle);
				wind_delete(wind.handle);
				return(FALSE);
			}

		fmsg=fopen(name_msg,"rb");
		if (fmsg==NULL)
			{	fclose(fhdr);
				fatal_error(name_msg);
				wind_close(wind.handle);
				wind_delete(wind.handle);
				return(FALSE);
			}

		maxmsg = (int) (length_of_file(fhdr,TRUE) / sizeof(MSG_HEADER))-1;
		if (maxmsg<0)
			{	fclose(fhdr);
				fclose(fmsg);
				form_alert(1,msg[43]);
				wind_close(wind.handle);
				wind_delete(wind.handle);
				return(FALSE);
			}

		lines = (char	**) calloc(MAXLINESMSG,sizeof(char *));
		if (lines==NULL)
			{	fclose(fhdr);
				fclose(fmsg);
				form_alert(1,msg[33]);
				wind_close(wind.handle);
				wind_delete(wind.handle);
				return(FALSE);
			}

		buffer = (char *)malloc(65536L);
		if (buffer==NULL)
			{	fclose(fhdr);
				fclose(fmsg);
				free(lines);
				form_alert(1,msg[33]);
				wind_close(wind.handle);
				wind_delete(wind.handle);
				return(FALSE);
			}

		*lastfile=
		*fromaddr=EOS;
		readmsg = TRUE;
		msg_on_screen = -1;
		linescnt = 0;
		wind_mesag(&wind,MU_MESAG|MU_TIMER);
		do
			{	chr = multimsg = 0;

				if (msg_on_screen!=-1)
					{	if (!wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG))
							{	chr = UNDO;
								multimsg = MU_KEYBD;
							}
						else
							{	if (wind.evnt & MU_KEYBD)
									{	multimsg = MU_KEYBD;
										chr = wind.kscan;
									}
								if (wind.evnt & MU_BUTTON)
									multimsg = MU_BUTTON;
								if (wind.evnt & MU_MESAG)
									{	if (wind.msg[3]==wind.handle)
											{	if (wind.msg[0]==WM_HSLID)
													{	msg_on_screen = (int) (   ((long)(maxmsg+1)*(long)wind.msg[4]) / 1000L  );
														msg_on_screen = MAX(msg_on_screen,0);
														msg_on_screen = MIN(msg_on_screen,maxmsg);
														readmsg=TRUE;
													}
												if (wind.msg[0]==WM_VSLID)
													{	i = MAX(linescnt - wind.max_y,1);
														firstline = 1+ (int) (   ((long)i*(long)wind.msg[4]) / 1000L  );
														firstline = MAX(firstline,1);
														firstline = MIN(firstline,i);
														set_vslide(&wind,firstline-1,linescnt-1);
														redraw_screen(&wind,firstline,linescnt);
													}
												if (wind.msg[0]==WM_ARROWED)
													{	multimsg = MU_KEYBD;
														switch(wind.msg[4])
															{	case	WA_UPLINE	: chr = CCUR_UP;		break;
																case	WA_DNLINE : chr = CCUR_DOWN;	break;
																case	WA_LFLINE : chr = CCUR_LEFT;	break;
																case	WA_RTLINE : chr = CCUR_RIGHT;	break;
																case	WA_UPPAGE : chr = SHFT_CU;		break;
																case	WA_DNPAGE : chr = SHFT_CD;		break;
																case	WA_LFPAGE : chr = SHFT_CL;		break;
																case	WA_RTPAGE : chr = SHFT_CR;		break;
																default					: multimsg = 0;
															}
													}
											}
									}
							}
					}

				if (!wind_top(&wind))
					multimsg = 0;

				if (multimsg & MU_BUTTON)
					{	ypos = wind.my - wind.txy[1];
						if (ypos<0 || ypos>wind.txy[3]-wind.txy[1])
							ypos=-1;
						else
							ypos = ypos / wind.hbox;
						if (firstline+ypos>linescnt)
							ypos=-1;

						xpos = wind.mx - wind.txy[0];
						if (xpos<0 || xpos>wind.txy[2]-wind.txy[0])
							xpos=-1;
						else
							xpos = xpos / wind.wbox;

						if (ypos!=-1)
							if (xpos>=strlen(lines[firstline+ypos]))
								xpos=-1;

						if (xpos!=-1 && ypos!=-1)
							{	while( (xpos>=0) && isspace(lines[firstline+ypos][xpos])==0)
									--xpos;
								xpos++;
								xend=xpos;
								while( (xend<=wind.max_x) && isspace(lines[firstline+ypos][xend])==0)
									xend++;
								xend--;
								if (xend>=xpos && lines[firstline+ypos][xpos]!=EOS)
									{	for (i=xpos; i<=xend; i++)
											select[i-xpos] = lines[firstline+ypos][i];
										select[i-xpos] = EOS;

										wind.x = xpos;	wind.y = ypos;
										wind_putch(&wind,27,1); wind_putch(&wind,'p',1);
										for (pointer=select; *pointer!=EOS; pointer++)
											wind_putch(&wind,*pointer,0);
										wind_putch(&wind,27,1); wind_putch(&wind,'q',1);

										if (wind.mclicks==1)
											{	pos=0;
												for (i=1; i<=20; i++)
													{	if ( *files[i][0]==EOS )
															{	pos=i;
																break;
															}
													}
												if (pos==0)
													{	form_alert(1,msg[14]);
													}
												else
													{	pointer=strncpy(lastfile,select,20);
														lastfile[20]=EOS;
														while(*pointer)
															{	if (*pointer=='[' || *pointer==']' || *pointer=='|')
																	*pointer=' ';
																pointer++;
															}
														sprintf(temp,"[0][ Request: | <%s> |][ Cancel |   OK   ]",lastfile);
														if (form_alert(2,temp)==2)
															{	strncpy(files[pos][0],select,20);
																strncpy(files[pos][1],"",20);
																strncpy(lastfile,select,20);
																lastfile[20]=EOS;
																INFOZEILE
															}
													}
											}

										if (wind.mclicks==2)
											{	pointer=select;
												while ( (*pointer<'0' || *pointer>'9') && *pointer )
													pointer++;
												scanaddr(pointer,&x1,&x2,&x3,&x4,temp);
												mergeaddr(fromaddr,x1,x2,x3,x4,"");
												INFOZEILE
											}

										wind.x = xpos;	wind.y = ypos;
										for (pointer=select; *pointer!=EOS; pointer++)
											wind_putch(&wind,*pointer,0);
									}
							}
					}
				
				if (multimsg & MU_KEYBD)
					{	switch (chr)
							{	case HELP			: form_alert(1,msg[39]);
																break;
								case ALT_Q		:
								case CNTRL_Q	: chr=UNDO;
																break;
								case SHFT_CU	:	firstline = MAX(firstline-wind.max_y,1);
																set_vslide(&wind,firstline-1,linescnt-1);
																redraw_screen(&wind,firstline,linescnt);
																break;
								case SHFT_CD	:	firstline = MIN(firstline+wind.max_y,linescnt-wind.max_y);
																firstline = MAX(firstline,1);
																set_vslide(&wind,firstline-1,linescnt-1);
																redraw_screen(&wind,firstline,linescnt);
																break;
								case CCUR_UP	:	if (firstline>1)
																	{	firstline--;
																		wind_putch(&wind,27,1);
																		wind_putch(&wind,'Y',1);
																		wind_putch(&wind,' ',1);
																		wind_putch(&wind,' ',1);
																		wind_putch(&wind,27,1);
																		wind_putch(&wind,'L',1);
																		wind_print(&wind,lines[firstline]);
																		set_vslide(&wind,firstline-1,linescnt-1);
																	}
																break;
								case CCUR_DOWN:	if (firstline+wind.max_y<linescnt)
																	{	firstline++;
																		wind_putch(&wind,27,1);
																		wind_putch(&wind,'Y',1);
																		wind_putch(&wind,' '+wind.max_y,1);
																		wind_putch(&wind,' ',1);
																		wind_print(&wind,lines[firstline+wind.max_y-1]);
																		wind_write(&wind,lines[firstline+wind.max_y]);
																		set_vslide(&wind,firstline-1,linescnt-1);
																	}
																break;
								case SHFT_CL	:
								case CCUR_LEFT:if (msg_on_screen>0)
																	{	msg_on_screen--;
																		readmsg=TRUE;
																	}
																break;
								case SHFT_CR	:
								case CCUR_RIGHT:if (msg_on_screen<maxmsg)
																	{	msg_on_screen++;
																		readmsg=TRUE;
																	}
																break;
								default				:
												if ( (chr & 0xff)>='0' && (chr & 0xff)<='9')
													{	form_center(tree29,&x1,&x2,&x3,&x4);
														*temp=chr; temp[1]=EOS;
														put_text(tree29,MSGEDIT,temp,3);
														form_dial(FMD_START,mid_x,mid_y,1,1,x1,x2,x3,x4);
														form_dial(FMD_GROW,mid_x,mid_y,1,1,x1,x2,x3,x4);
														objc_draw(tree29,ROOT,MAX_DEPTH,x1,x2,x3,x4);
														i=form_do(tree29,0) & 0x7FFF;
														undo_state(tree29,i,SELECTED);
														form_dial(FMD_SHRINK,mid_x,mid_y,1,1,x1,x2,x3,x4);
														form_dial(FMD_FINISH,mid_x,mid_y,1,1,x1,x2,x3,x4);
														strcpy(temp,get_text(tree29,MSGEDIT));
														if (*temp!=EOS)
															{	i=atoi(temp)-1;
																if (i>maxmsg)
																	i=maxmsg;
																if (i>=0 && i<=maxmsg && i!=msg_on_screen)
																	{	msg_on_screen=i;
																		readmsg=TRUE;
																	}
															}
													}
							}
					}

				if (wind_top(&wind))
					{	if (readmsg)				/* Msg darstellen */
							{	if (msg_on_screen==-1)
									msg_on_screen = maxmsg;
								readmsg = FALSE;
								linescnt = 0;
								firstline = 1;
								*fromaddr = EOS;
								fseek(fhdr,msg_on_screen*sizeof(MSG_HEADER),SEEK_SET);
								fread(&header,sizeof(MSG_HEADER),1,fhdr);

								fseek(fmsg,header.lStart,SEEK_SET);
								fread(buffer,header.wSize,1,fmsg);
								buffer[header.wSize] = EOS;

								pointer=buffer;
								if (*pointer!='\001')
									lines[++linescnt]=pointer;
								else
									{	if (strnicmp(pointer,"\001MSGID:",7)==0)
											{	scanaddr(pointer+7,&x1,&x2,&x3,&x4,temp);
												mergeaddr(fromaddr,x1,x2,x3,x4,"");
											}
									}
								for (i=1; i<=header.wSize-1; i++)
									{	if (*pointer==EOS || *pointer=='\n' || *pointer=='\r')
											{	*pointer++=EOS;
												if (strncmp(pointer,"SEEN-BY:",8)!=0)
													{	if (*pointer!='\001')
															{	if (linescnt<MAXLINESMSG-1)
																	lines[++linescnt]=pointer;
																else
																	lines[linescnt] = avaltxt[15];
															}
														else
															{	if (strnicmp(pointer,"\001MSGID:",7)==0)
																	{	scanaddr(pointer+7,&x1,&x2,&x3,&x4,temp);
																		mergeaddr(fromaddr,x1,x2,x3,x4,"");
																	}
															}
													}
											}
										else
											pointer++;
									}
								lines[linescnt+1] = NULL;

								for (x1=1; x1<=linescnt; x1++)
									{	if (strlen(lines[x1]) > wind.max_x+1)
											{	for (pointer=lines[x1]+wind.max_x; pointer>lines[x1] && *pointer!=' '; pointer--);
												if (pointer>lines[x1])
													{	*pointer=EOS;
														for (x2=linescnt+1; x2>x1; x2--)
															lines[x2] = lines[x2-1];
														linescnt++;
														lines[x1+1]=pointer+1;
													}
												else if (pointer==lines[x1])
													{	lines[x1][wind.max_x+1] = EOS;
													}
											}
									}

								INFOZEILE
								set_hslide(&wind,msg_on_screen,maxmsg);
								set_vslide(&wind,0,linescnt-1);
								redraw_screen(&wind,1,linescnt);
							}
					}

			}
		while (chr!=UNDO || !(multimsg & MU_KEYBD));

		scanaddr(fromaddr,&x1,&x2,&x3,&x4,temp);
		if (divflags & D_USE4D)
			{	if (x4==0)
					sprintf(temp,"%5u%u",x2,x3);
				else
					sprintf(temp,"%5u%-5u%u",x2,x3,x4);
				i=14;
			}
		else
			{	sprintf(temp,"%5u%u",x2,x3);
				i=10;
			}
		put_text(tree8,REQNODE,temp,i);

		free(buffer);
		free(lines);
		fclose(fhdr);
		fclose(fmsg);

		wind_close(wind.handle);
		wind_delete(wind.handle);

		return(TRUE);
	}

void set_hslide(WIND *wind,int aktmsg,int maxmsg)
	{	wind_set(wind->handle,WF_HSLIDE,(int) ( (1000L*(long)aktmsg) / ((long) MAX(maxmsg,1)) ) );
		wind_set(wind->handle,WF_HSLSIZE,1000/(maxmsg+1) );
	}

void set_vslide(WIND *wind,int aktline,int maxline)
	{	int xmaxline;
		xmaxline = MAX(maxline - wind->max_y,1);
		wind_set(wind->handle,WF_VSLIDE,(int) ( (1000L*(long)aktline) / ((long) MAX(xmaxline,1)) ) );
		wind_set(wind->handle,WF_VSLSIZE,1000/xmaxline );
	}

void redraw_screen(WIND *wind,int firstline,int maxline)
	{ int i;
		wind_cls(wind);
		for (i=1; i<=wind->max_y; i++)
			{	if (firstline+i-1<=maxline)
					wind_print(wind,lines[firstline+i-1]);
				else
					wind_print(wind,"");
			}
		if (firstline+i-1<=maxline)
			wind_write(wind,lines[firstline+i-1]);
	}
