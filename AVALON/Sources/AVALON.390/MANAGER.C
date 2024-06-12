/* MODUL: Filemanager					 */
/*														 */
/* (c) St.Slabihoud 1991-1993	 */

#include <aes.h>
#include <vdi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>
#include <ext.h>
#include <screen.h>
#include <portab.h>
#include <scancode.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "avalonio.h"
#include "avalxhdi.h"
#include "avallang.h"
#include "define.h"
#include "version.h"
#include "diverse.h"
#include "form_run.h"
#include "windmini.h"
#include "vars.h"

extern int	xform_do3(OBJECT *ctree, int xstart);
extern void	start(int prg);
extern void desktop(void);
extern void systeminfo(void);
extern void outboundinfo(void);
extern void msg_area_info(void);

void	file_manager(void);
void	manager_copy(int mode);
int		copy_file(char *source, char *dest);
void	start_manager(void);
void	end_manager(void);
void	info_manager(char *txt, int mode);
void	manager_del(void);
void	manager_ren(void);
void	manager_inf(void);
void	manager_view(void);
void	manager_ship(void);
void	manager_term(int baud);
void	setport(int serport);
void	show_rate(void);

extern char	capture[],txtsysop[],netfile[];
extern int	serport,vt52,voll,baud;

static int nx,ny,nw,nh;

/* =============================================================== */
/* =FILE-MANAGER================================================== */
/* =============================================================== */

void file_manager(void)
	{	int wahl,offx,offy;
		objc_offset(tree1,DMANAGER,&offx,&offy);
		offx -= OB_X(tree16,MANREF);
		offy -= OB_Y(tree16,MANREF);
		OB_X(tree16,ROOT) = offx;
		OB_Y(tree16,ROOT) = offy;
		nx = OB_X(tree16,ROOT) - 3;
		ny = OB_Y(tree16,ROOT) - 3;
		nw = OB_W(tree16,ROOT) + 6;
		nh = OB_H(tree16,ROOT) + 6;
		form_dial(FMD_START,nx,ny,nw,nh,nx,ny,nw,nh);
/*	form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh); */
		objc_draw(tree16,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahl=xform_do3(tree16,0) & 0x7ff;
		if (wahl<100)
			undo_state(tree16,wahl,SELECTED);
/*	form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh); */
		form_dial(FMD_FINISH,nx,ny,nw,nh,nx,ny,nw,nh);
		switch(wahl)
			{	case	MANCOPY : manager_copy(0);		break;
				case	MANMOVE : manager_copy(1);		break;
				case	MANDEL	: manager_del(); 			break;
				case	MANREN	: manager_ren();			break;
				case	MANINFO : manager_inf();			break;
				case	MANSHIP : manager_ship();			break;
				case	MANTERM	: manager_term(baud);	break;
				case	MANVIEW : manager_view();			break;
				case	MANSYS	:	systeminfo();				break;
				case	MANOUT	:	outboundinfo();			break;
				case	MANMSGS	:	msg_area_info(); 		break;
			}
	}

/* =============================================================== */

void manager_copy(int mode)
	{	char	source[PATH_MAX], dest[PATH_MAX],
					spfad[PATH_MAX], dpfad[PATH_MAX],
					in_path[PATH_MAX], sel_file[20]="",
					temp[MAX_STR];
		int		button,error;

		if (mode==0)
			button=fileselect(in_path,sel_file,"*.*",mtmsg[3]);
		else
			button=fileselect(in_path,sel_file,"*.*",mtmsg[4]);

		if (button!=0)
			{	strcpy(spfad,in_path);
				sprintf(source,"%s%s",in_path,sel_file);

				button=fileselect(in_path,sel_file,"*.*",mtmsg[5]);
				if (button!=0)
					{	strcpy(dpfad,in_path);
						sprintf(dest,"%s%s",in_path,sel_file);

						error=xFsfirst(source,0);

						start_manager();
						if (error<0)
							{	sprintf(temp,mtmsg[0],source);
								info_manager(temp,1);
							}
						while (error>=0)
							{	strcpy(source,spfad);
								strcat(source,dta_buffer.d_fname);
								strcpy(dest,dpfad);
								strcat(dest,dta_buffer.d_fname);
								if (copy_file(source,dest)!=0)
									mode=0;
								if (mode==1)
									{	if (remove(source)==-1)
											{	sprintf(temp,mtmsg[1],source);
												info_manager(temp,1);
											}
									}
								error=xFsnext();
							}
						end_manager();
					}
			}
	}

int copy_file(char *source, char *dest)
	{	int ret=0;
		char temp[MAX_STR+1],*copy_buffer;
		int hin,hout;

		copy_buffer = (char *) malloc(BUFFER_SIZE);
		if (copy_buffer==NULL)
			{	memory_error();
				return(1);
			}

		if (stricmp(source,dest)==0)
			{	sprintf(temp,mtmsg[6],source);
				info_manager(temp,1);
				free(copy_buffer);
				return(1);
			}

		hin = (int) Fopen(source,FO_READ);
		if (hin<0)
			{	sprintf(temp,mtmsg[2],source);
				info_manager(temp,1);
				free(copy_buffer);
				return(1);
			}

		hout = (int) Fcreate(dest,0);
		if (hout<0)
			{	sprintf(temp,mtmsg[2],dest);
				info_manager(temp,1);
				Fclose(hin);
				free(copy_buffer);
				return(1);
			}

		sprintf(temp,mtmsg[7],source,dest);
		info_manager(temp,0);
		filecopy(hin,hout,0,copy_buffer);
		Fclose(hout);
		Fclose(hin);
		free(copy_buffer);
		return ret;
	}

/* =============================================================== */

void manager_del(void)
	{	char	source[PATH_MAX], spfad[PATH_MAX],
					in_path[PATH_MAX], sel_file[14]="",
					temp[MAX_STR];
		int		button,error;

		button=fileselect(in_path,sel_file,"*.*",mtmsg[8]);
		if (button!=0)
			{	strcpy(spfad,in_path);
				sprintf(source,"%s%s",in_path,sel_file);

				error=xFsfirst(source,0);

				start_manager();
				if (error<0)
					{	sprintf(temp,mtmsg[0],source);
						info_manager(temp,1);
					}
				while (error>=0)
					{	strcpy(source,spfad);
						strcat(source,dta_buffer.d_fname);
						sprintf(temp,mtmsg[9],source);
						info_manager(temp,0);
						if (remove(source)==-1)
							{	sprintf(temp,mtmsg[1],source);
								info_manager(temp,1);
							}
						error=xFsnext();
					}
				end_manager();
			}
	}

/* =============================================================== */

void manager_ren(void)
	{	char	source[PATH_MAX], dest[PATH_MAX],
					in_path[PATH_MAX], sel_file[14]="",
					temp[MAX_STR],*pointer;
		int		button;

		button=fileselect(in_path,sel_file,"*.*",mtmsg[10]);
		if (button!=0)
			{	sprintf(source,"%s%s",in_path,sel_file);

				if (aes_version < XFSELECT)
					fsel_input(in_path,sel_file,&button);
				else
					fsel_exinput(in_path,sel_file,&button,mtmsg[11]);

				pointer=strrchr(in_path,'\\');
				if (pointer!=NULL)
					*(++pointer)=EOS;
				else
					strcpy(in_path,"\\");

				if (button!=0)
					{	sprintf(dest,"%s%s",in_path,sel_file);

						start_manager();
						sprintf(temp,mtmsg[12],source,dest);
						info_manager(temp,0);
						if (rename(source,dest)==-1)
							{	sprintf(temp,mtmsg[13],source,dest);
								info_manager(temp,1);
							}
						end_manager();
					}
			}
	}

/* =============================================================== */

void manager_inf(void)
	{	char	source[PATH_MAX], spfad[PATH_MAX],
					in_path[PATH_MAX], sel_file[14]="",
					temp[MAX_STR];
		int		button,error,xdate,xtime;

		button=fileselect(in_path,sel_file,"*.*",mtmsg[14]);
		if (button!=0)
			{	strcpy(spfad,in_path);
				sprintf(source,"%s%s",in_path,sel_file);

				error=xFsfirst(source,0);

				start_manager();
				if (error<0)
					{	sprintf(temp,mtmsg[0],source);
						info_manager(temp,1);
					}
				while (error>=0)
					{ xtime=dta_buffer.d_time;
						xdate=dta_buffer.d_date;
						sprintf(temp,"%-12s    %-8lu    %02u:%02u:%02u    %2u.%2u.%4u    %s",
																								dta_buffer.d_fname,
																								dta_buffer.d_length,
																								(xtime & 0xf800) >> 11,
																								(xtime & 0x07e0) >>  5,
																								(xtime & 0x001f) <<  1,
																								(xdate & 0x001f),
																								(xdate & 0x01e0) >>  5,
																								((xdate & 0xfe00) >> 9) + 1980,
																								(dta_buffer.d_attrib & FA_READONLY) ? mtmsg[27] : "");
						info_manager(temp,1);
						error=xFsnext();
					}
				end_manager();
			}
	}

/* =============================================================== */

void manager_view(void)
	{	char	in_path[PATH_MAX], sel_file[14]="",

		button=fileselect(in_path,sel_file,"*.*",mtmsg[15]);
		if (button!=0)
			{	sprintf(program2[15],"%s%s",in_path,sel_file);
				start(15);
			}
	}

/* =============================================================== */

void manager_ship(void)
	{	char	temp[MAX_STR],device[40];
		int		major,minor;
		long	flags;

		start_manager();
		if ( XHGetVersion() >= 0x100)
			{	for (major=0; major<=17; major++)
					for (minor=0; minor<=1; minor++)
						{	if ( XHInqTarget(major,minor,NULL,&flags,device) == 0 )
								if ( (flags & 1) && !(flags & (1<<31)) )
									{	if (major>=18)
											sprintf(temp,mtmsg[16],"???",major,minor,device);
										else if (major>=16)
											sprintf(temp,mtmsg[16],"IDE",major-16,minor,device);
										else if (major>=8)
											sprintf(temp,mtmsg[16],"SCSI",major-8,minor,device);
										else if (major>=0)
											sprintf(temp,mtmsg[16],"ACSI",major,minor,device);
										info_manager(temp,0);
										XHStop(major,minor,TRUE,0);
									}
						}
			}
		else
			{	info_manager(mtmsg[17],1);
			}
		end_manager();
	}

/* =============================================================== */

void start_manager(void)
	{	form_center(tree17,&nx,&ny,&nw,&nh);
		put_string(tree17,IMANAG1,"",70);
		put_string(tree17,IMANAG2,"",70);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		objc_draw(tree17,ROOT,MAX_DEPTH,nx,ny,nw,nh);
	}

void end_manager(void)
	{	form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
	}

void info_manager(char *txt, int mode)
	{	int rx,ry,rw,rh;
		char temp[MAX_STR];
		strcpy(temp,txt);
		put_string(tree17,IMANAG1,temp,70);
		if (strlen(txt)>70)
			{	strcpy(temp,txt);
				put_string(tree17,IMANAG2,temp+70,70);
			}
		else
			{	put_string(tree17,IMANAG2,"",70);
			}
		objc_offset(tree17,IMANAG1,&rx,&ry);
		rw = OB_W(tree17,IMANAG1);
		rh = OB_H(tree17,IMANAG1) * 2;
		objc_draw(tree17,ROOT,MAX_DEPTH,rx,ry,rw,rh);
		if (mode==1)
			xform_do3(tree17,0);
	}

/*******************************************************************/
/* Terminal																												 */
/*******************************************************************/

void manager_term(int xbaud)
	{	FILE *flog=NULL,*fp;
		long len;
		int logfile=FALSE,t,i,cr=FALSE,chr,multimsg;
		int msgbuf[8] = { 0,0,0,0,0,0,0,0 };
		char temp[MAX_STR],*pointer,sz[]="-sz -M ",rz[]="-rz -M -d -P ",
				 in_path[MAX_STR],*iobuf;
		IOREC *io,old;

		wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
		wind.handle=handle;
		wind_set(wind.handle,WF_NAME," AVALON-TERMINAL ");
		wind_set(wind.handle,WF_CURRXYWH,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
		wind_update(BEG_UPDATE);
		wind_cls(&wind);
		wind_putch(&wind,27,VT52);
		wind_putch(&wind,'e',VT52);
		wind_update(END_UPDATE);

		if (Bconmap(0)==0L)
			setport(serport);
		if (gemdos_version >= XRSCONF)
			{	if (Rsconf(-2,-1,-1,-1,-1,-1)!=xbaud)
					Rsconf(xbaud,2,-1,-1,-1,-1);
			}
		else
			Rsconf(xbaud,2,-1,-1,-1,-1);
		show_rate();

		iobuf = (char *) calloc(32000,sizeof(char));
		if (iobuf!=NULL)
			{	io = Iorec(0);
				old.ibuf		= io->ibuf;
				old.ibufsiz	= io->ibufsiz;
				old.ibufhd	= io->ibufhd;
				old.ibuftl	= io->ibuftl;
				old.ibuflow	= io->ibuflow;
				old.ibufhi	= io->ibufhi;
				io->ibuf		= iobuf;
				io->ibufsiz = 32000;
				io->ibufhd	=
				io->ibuftl	= 0;
				io->ibuflow = 16000;
				io->ibufhi	= 28000;
			}

		do
			{	chr=t=0;
				i=TRUE;
				if (!wind_top(&wind))
					i=FALSE;
				if (i==TRUE)
					{	wind_update(BEG_UPDATE);
						if (Bconstat(1))
							{	wind_clip(&wind,TRUE);
								if (wind.cursor)	wind_cursor(&wind);

								while (Bconstat(1) &&	t++<=512)		/* Zeichen an AUX verfgbar? */
									{	chr=(int) Bconin(1);					/*  einlesen...								*/
										if (vt52 || chr!=27)
											{	wind_putch(&wind,chr,vt52 | 128);
												if (logfile==TRUE)
													{	if (cr==TRUE && chr!=10)
															putc(10,flog);
														cr=FALSE;
														putc(chr,flog);
														if (chr==13)
															cr=TRUE;
													}
											}
									}

								if (wind.cursor)	wind_cursor(&wind);
								wind_clip(&wind,FALSE);
							}
						wind_update(END_UPDATE);
					}

				chr =
				multimsg = 0;
				if (!wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER))
					{	chr = UNDO;
						multimsg = MU_KEYBD;
					}
				if (wind.evnt & MU_KEYBD)
					{	chr = wind.kscan;
						multimsg = MU_KEYBD;
					}

				if (!wind_top(&wind))
					multimsg &= ~MU_KEYBD;

				if (multimsg & MU_KEYBD)
					{	switch (chr)
							{	case UNDO	: break;
								case HELP	: {	WPRINT("");
															for (i=35; i<=46; i++)
																WPRINT(mtmsg[i]);
															for (i=57; i<=58; i++)
																WPRINT(mtmsg[i]);
															WPRINT("");
															show_rate();
															if (Bconmap(0)==0L)
																setport(serport);
															switch(vt52)
																{ case 0 : WPRINT(mtmsg[26]) break;
																	case 1 : WPRINT(mtmsg[25]) break;
																	case 2 : WPRINT(mtmsg[49]) break;
																}
															if (voll)	WPRINT(mtmsg[23])
															else			WPRINT(mtmsg[24])
															if (wind.wrap)	WPRINT(mtmsg[55])
															else						WPRINT(mtmsg[56])
															if (logfile)	sprintf(temp,mtmsg[51],mtmsg[52]);
															else					sprintf(temp,mtmsg[51],mtmsg[53]);
															WPRINT(temp);
														}	break;
							case SHFT_F1:	{	WPRINT("")
															if (wind.wrap==FALSE)
																{	wind.wrap=TRUE;
																	WPRINT(mtmsg[55])
																}
															else
																{	wind.wrap=FALSE;
																	WPRINT(mtmsg[56])
																}
														}	break;
							case SHFT_F6:	{	WPRINT("")
															if (gemdos_version >= XRSCONF)
																{	xbaud = (int)Rsconf(-2,-1,-1,-1,-1,-1);
																}
															if (--xbaud<0) xbaud=15;
															Rsconf(xbaud,2,-1,-1,-1,-1);
															show_rate();
														}	break;
								case F1		: {	WPRINT("")
															if (logfile==FALSE)
																{	flog = fopen(capture,"ab");
																	if (flog==NULL)
																		{	sprintf(temp,mtmsg[28],capture);
																			WPRINT(temp)
																		}
																	else
																		{	logfile=TRUE;
																			WPRINT(mtmsg[18])
																		}
																}
															else
																{	WPRINT(mtmsg[19])
																}
														}	break;
								case F2		:	{	WPRINT("")
															if (logfile==TRUE)
																{	fclose(flog);
																	logfile=FALSE;
																	WPRINT(mtmsg[20])
																}
															else
																{	WPRINT(mtmsg[21])
																}
														}	break;
								case F3		:	{	WPRINT("")
															if (logfile==TRUE)
																{	WPRINT(mtmsg[22])
																}
															else
																{	if (exist(capture)==0)
																		{	WPRINT(mtmsg[50])
																			remove(capture);
																		}
																	else
																		{	WPRINT(mtmsg[21])
																		}
																}
														}	break;
								case F4		:	{	WPRINT("")
															if (voll==FALSE)
																{	voll=TRUE;
																	WPRINT(mtmsg[23])
																}
															else
																{	voll=FALSE;
																	WPRINT(mtmsg[24])
																}
														}	break;
								case F5		:	{	vt52++;
															if (vt52>VT100)	vt52=TTY;
															WPRINT("")
															switch(vt52)
																{ case 0 : WPRINT(mtmsg[26]) break;
																	case 1 : WPRINT(mtmsg[25]) break;
																	case 2 : WPRINT(mtmsg[49]) break;
																}
														}	break;
								case	F6	:	{	WPRINT("")
															if (Bconmap(0)==0L)
																{	serport++;
																	if (serport>=4)
																		serport=0;
																	changes |= C_TERMINAL;
																	setport(serport);
																}
															else
																{	WPRINT(mtmsg[30])
																}
															show_rate();
														}	break;
								case	F7	:	{	WPRINT("")
															i=appl_find("GSZRZACC");
															if (i>=0)
																{	*temp=EOS;
																	vshow_mouse(0);
																	t=fileselect(in_path,temp,"*.*",avaltxt[12]);
																	vhide_mouse();
																	strcat(in_path,temp);
																	sprintf(temp,"%s%s",sz,in_path);
																	if (t!=0)
																		{	msgbuf[0]=0x475A;
																			msgbuf[1]=ap_id;
																			msgbuf[2]=0;
																			msgbuf[3]=(int) strlen(temp);
																			msgbuf[4]=(int) &temp;
																			msgbuf[5]=(int) ((long)&temp >> 16);
																			msgbuf[6]=
																			msgbuf[7]=0;
																			appl_write(i,16,msgbuf);
																		}
																}
															else
																{	WPRINT(mtmsg[29])
																}
														} break;
								case	F8	:	{	WPRINT("")
															i=appl_find("GSZRZACC");
															if (i>=0)
																{	sprintf(temp,"%s%s",rz,netfile);
																	msgbuf[0]=0x475A;
																	msgbuf[1]=ap_id;
																	msgbuf[2]=0;
																	msgbuf[3]=(int) strlen(temp);
																	msgbuf[4]=(int) &temp;
																	msgbuf[5]=(int) ((long)&temp >> 16);
																	msgbuf[6]=
																	msgbuf[7]=0;
																	appl_write(i,16,msgbuf);
																}
															else
																{	WPRINT(mtmsg[29])
																}
														} break;
								case	F9	:	{	*temp=EOS;
															vshow_mouse(0);
															t=fileselect(in_path,temp,"*.*",avaltxt[14]);
															vhide_mouse();
															strcat(in_path,temp);
															wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER);
															WPRINT("")
															if (exist(in_path)==0)
																{	fp=fopen(in_path,"rb");
																	if (fp==NULL)
																		{	sprintf(temp,mtmsg[2],in_path);
																			WPRINT(temp)
																		}
																	else
																		{	len=length_of_file(fp,TRUE);
																			while (len>0)
																				{	t = 0;
																					wind_putch(&wind,getc(fp),vt52);
																					wind_mesag(&wind,MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER);
																					if (wind.evnt & MU_KEYBD)
																						t = wind.kscan & 0xff;
																					if (t==19) pause();
																					if (t== 3) break;
																					len--;
																				}
																			vswr_mode(wind.vdi_handle,MD_REPLACE);
																			wind.mode = MD_REPLACE;
																			vst_effects(wind.vdi_handle,0);
																			wind.effects = 0;
																			fclose(fp);
																		}
																}
															else
																{	sprintf(temp,mtmsg[54],in_path);
																	WPRINT(temp)
																}
														} break;
								case F10	:	{	pointer=txtsysop;
															for (i=1; i<=strlen(txtsysop); i++)
																{	for (t=1; t<=30; t++)
																		{	if (Bcostat(1))						/* Ausgabe AUX m”glich? */
																				{	if (voll==FALSE)
																						wind_putch(&wind,*pointer,vt52);
																					Bconout(1,(int) *pointer++);	/*  ausgeben...					*/
																					break;
																				}
																			delay(10);
																		}
																}
														} break;
								case CCUR_UP:
								case CCUR_DOWN:
								case CCUR_LEFT:
								case CCUR_RIGHT:
														{ if (vt52==VT52)
																{	Bconout(1,27);
																}
															if (vt52==VT100)
																{	Bconout(1,27);
																	Bconout(1,'[');
																}
															if (vt52)
																switch(chr)
																	{	case CCUR_UP		: Bconout(1,'A'); break;
																		case CCUR_DOWN	: Bconout(1,'B'); break;
																		case CCUR_RIGHT	:	Bconout(1,'C'); break;
																		case CCUR_LEFT	:	Bconout(1,'D'); break;
																	}
														}	break;
								default		:	{	chr &= 0xff;
															if (voll==FALSE)
																if (vt52 || chr!=27)
																	wind_putch(&wind,chr,vt52);
															for (t=1; t<=30; t++)
																{	if (Bcostat(1))		/* Ausgabe AUX m”glich? */
																		{	Bconout(1,chr);				/*  ausgeben...					*/
																			break;
																		}
																	delay(10);
																}
														}
							}
					}
			}
		while (chr!=UNDO || !(multimsg & MU_KEYBD));
		if (logfile==TRUE)
			fclose(flog);

		wind_set(handle,WF_NAME,AVALONNAME);
		xform_maindialog(XFMD_DIALOG);

		if (iobuf!=NULL)
			{	io->ibuf		= old.ibuf;
				io->ibufsiz = old.ibufsiz;
				io->ibufhd	= old.ibufhd;
				io->ibuftl	= old.ibuftl;
				io->ibuflow = old.ibuflow;
				io->ibufhi	= old.ibufhi;
				free(iobuf);
			}
	}

void setport(int serport)
	{	switch(serport)
			{	case	0	:	{	Bconmap(6);
										WPRINT(mtmsg[31])
									}	break;
				case	1	:	{	Bconmap(7);
										WPRINT(mtmsg[32])
									}	break;
				case	2	:	{	Bconmap(8);
										WPRINT(mtmsg[33])
									}	break;
				case	3	:	{	Bconmap(9);
										WPRINT(mtmsg[34])
									}
			}
	}

void show_rate(void)
	{	int i;
		char temp[MAX_STR],temp2[MAX_STR];
		if (gemdos_version >= XRSCONF)
			{	switch((int)Rsconf(-2,-1,-1,-1,-1,-1))
					{	case	0	:	i=19200; break;
						case	1	: i=9600; break;
						case	2	:	i=4800; break;
						case	3 : i=3600; break;
						case	4	: i=2400; break;
						case	5 : i=2000; break;
						case	6 : i=1800; break;
						case	7	:	i=1200; break;
						case	8 : i=600; break;
						case	9	:	i=300; break;
						case 10	: i=200; break;
						case 11	: i=150; break;
						case 12	: i=134; break;
						case 13	: i=110; break;
						case 14	: i=75; break;
						case 15	: i=50; break;
						default	:	i=-1;
					}
				if (i==-1)	strcpy(temp,mtmsg[47]);
				else				sprintf(temp,mtmsg[48],i);
				if (i==150 || i==134 || i==110 || i==75 || i==50 )
					{	switch(i)
							{	case 150	:	strcpy(temp2,"115200"); break;
								case 134	: strcpy(temp2,"57600");	break;
								case 110	:	strcpy(temp2,"38400");	break;
								case	75	: strcpy(temp2,"153600");	break;
								case	50	: strcpy(temp2,"76800");	break;
							}
						strcat(temp," (with HSMODEM: ");
						strcat(temp,temp2);
						strcat(temp,")");
					}
				WPRINT(temp);
			}
	}
