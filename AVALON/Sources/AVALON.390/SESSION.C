#include <aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <portab.h>
#include "define.h"
#include "diverse.h"
#include "form_run.h"
#include "avallang.h"
#include "vars.h"
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
	#include "avalsemp.rh"
#else
	#include "avalon.h"
	#include "avalon.rh"
#endif

int		setup1(void);
void	ersatz_str(int csess, int cxx, char *txt);
void	trage_ein(int csess, int cflag);
void	bestimme_sflags(int csess);

/* =============================================================== */
/* =SESSION-SETUP================================================= */
/* =============================================================== */

int setup1(void)
	{	int sess=0,t,button,xx,retflag=FALSE,xwahl,wahl,wahl2,nx,ny,nw,nh;
		char in_path[PATH_MAX],sel_file[14]="",temp[MAX_STR];

		do_state(tree2,SESSION1,1);
		undo_xstate(tree2,SELECTED,SESSION2,SESSION3,SESSION4,
															 SESSION5,SESSION6,-1);

		put_text(tree2,XUTIL1,txtutil[0],9);
		put_text(tree2,XUTIL2,txtutil[1],9);
		put_text(tree2,XUTIL3,txtutil[2],9);
		put_text(tree2,XUTIL4,txtutil[3],9);
		put_text(tree2,XUTIL5,txtutil[4],9);
		put_text(tree2,XUTIL6,txtutil[5],9);
		put_string(tree2,SESSION1,txtsess[0],9);
		put_string(tree2,SESSION2,txtsess[1],9);
		put_string(tree2,SESSION3,txtsess[2],9);
		put_string(tree2,SESSION4,txtsess[3],9);
		put_string(tree2,SESSION5,txtsess[4],9);
		put_string(tree2,SESSION6,txtsess[5],9);
		put_text(tree2,XMAIN,txtname[0],11);
		put_text(tree2,XSET1,txtname[1],11);
		put_text(tree2,XSET2,txtname[2],11);
		put_text(tree2,XSET3,txtname[3],11);
		put_text(tree2,XSET4,txtname[4],11);
		put_text(tree2,XSET5,txtname[5],11);

		if (mailer==BINKLEY)
			{	undo_xstate(tree2,DISABLED,XMAIN,XSET1,XSET2,
																	 XSET3,XSET4,XSET5,XSETNO,-1);
			}
		else
			{	do_xstate(tree2,DISABLED,XMAIN,XSET1,XSET2,
																 XSET3,XSET4,XSET5,XSETNO,-1);
				for (t=0; t<=MAXSESSION-1;  t++)
					sflags[t][0] &= ~(F_MAIN|F_SET1|F_SET2|F_SET3|F_SET4|F_SET5);
			}

		trage_ein(sess,0);

		xform_dial(tree2,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	xwahl=xform_do(handle,tree2,NULL,0,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
				if (xwahl & 0x4000)
					xwahl = XOK;
				wahl = xwahl & 0x7ff;
				
				if (xwahl & 0x8000)
					{	objc_offset(tree2,wahl,&nx,&ny);
						OB_X(tree21,0) = nx;
						OB_Y(tree21,0) = ny;
						nx -= 3;
						ny -= 3;
						nw = OB_W(tree21,0) + 6;
						nh = OB_H(tree21,0) + 6;

						put_text(tree21,SESSTEXT,txtsess[wahl-SESSION1],9);
						form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
						form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
						objc_draw(tree21,ROOT,MAX_DEPTH,nx,ny,nw,nh);
						wahl2=form_do(tree21,0) & 0x7FFF;
						undo_state(tree21,wahl2,SELECTED);
						form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
						form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
						strcpy(temp,get_text(tree21,SESSTEXT));
						put_string(tree2,wahl,temp,9);
						if (strcmp(temp,txtsess[wahl-SESSION1])!=0)
							{	strcpy(txtsess[wahl-SESSION1],temp); retflag=TRUE; }
						redraw_one_objc(tree2,wahl,handle);
					}
				bestimme_sflags(sess);
				switch(wahl)
					{	case SESSION1 : {	sess=0;	trage_ein(0,2);	} break;
						case SESSION2 : { sess=1;	trage_ein(1,2);	} break;
						case SESSION3 :	{	sess=2;	trage_ein(2,2);	} break;
						case SESSION4 : {	sess=3;	trage_ein(3,2);	}	break;
						case SESSION5 : {	sess=4;	trage_ein(4,2);	}	break;
						case SESSION6 : {	sess=5;	trage_ein(5,2);	}	break;
						default				:	{ undo_state(tree2,wahl,SELECTED);
															redraw_objc(tree2,wahl,handle);
															if ((wahl!=XSETNO)	&& 
																	(wahl!=XOK)			&&
																	(wahl!=XDELONE) &&
																	(wahl!=XDELALL) &&
																	(wahl<XPROG1 || wahl>XPROG12))
																{	i_session[sess]++;

																	xx=0;
																	for (t=1; t<=MIN(MAXPRGSESS,i_session[sess]); t++)
																		if (*s_session[sess][t][0]==EOS)
																			{	xx=t; break; }

																	if ( xx>0 )
																		{	if (xx<i_session[sess] &&
																					i_session[sess]<=MAXPRGSESS)
																				i_session[sess]--;
																			switch(wahl)
																				{	case XEXPORT	:	ersatz_str(sess,xx,"###1"); break;
																					case XIMPORT	:	ersatz_str(sess,xx,"###2"); break;
																					case XCRUNCH	:	ersatz_str(sess,xx,"###3"); break;
																					case XSCANNER	:	ersatz_str(sess,xx,"###4"); break;
																					case XMSGED		:	ersatz_str(sess,xx,"###5"); break;
																					case XMAILER	:	ersatz_str(sess,xx,"###7"); break;
																					case XTERM		:	ersatz_str(sess,xx,"###8"); break;
																					case XUTIL1		:	ersatz_str(sess,xx,"###9"); break;
																					case XUTIL2		:	ersatz_str(sess,xx,"###10"); break;
																					case XUTIL3		:	ersatz_str(sess,xx,"###11"); break;
																					case XUTIL4		:	ersatz_str(sess,xx,"###12"); break;
																					case XUTIL5		:	ersatz_str(sess,xx,"###13"); break;
																					case XUTIL6		:	ersatz_str(sess,xx,"###14"); break;
																					case XBATCH		: {	button=fileselect(in_path,sel_file,"*.BAT",avaltxt[6]);
																														strcat(in_path,sel_file);
																														if (button!=0)
																															{	strcpy(s_session[sess][xx][0],in_path);
																																*s_session[sess][xx][1]=EOS;
																																w_session[sess][xx][0] = F_BATCH;
																																w_session[sess][xx][1] = 0;
																															}
																														else
																															{	*s_session[sess][xx][0]=
																																*s_session[sess][xx][1]=EOS;
																																w_session[sess][xx][0]=
																																w_session[sess][xx][1]=0;
																																if (xx==i_session[sess])
																																	i_session[sess]--;
																															}
																														break;
																													}
																					case XOTHER		:	{	button=fileselect(in_path,sel_file,"*.*",avaltxt[7]);
																														strcat(in_path,sel_file);
																														if (button!=0)
																															{	strcpy(s_session[sess][xx][0],in_path);
																																put_text(tree5,XCMD,"",0);
																																form_center(tree5,&nx,&ny,&nw,&nh);
																																form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																																form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																																objc_draw(tree5,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																																wahl2=form_do(tree5,0) & 0x7FFF;
																																undo_state(tree5,wahl2,SELECTED);
																																form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																																form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
																																strcpy(s_session[sess][xx][1],get_text(tree5,XCMD));
																															}
																														else
																															{	*s_session[sess][xx][0]=
																																*s_session[sess][xx][1]=EOS;
																																w_session[sess][xx][0]=
																																w_session[sess][xx][1]=0;
																																if (xx==i_session[sess])
																																	i_session[sess]--;
																															}
																														break;
																													}
																				}
																			trage_ein(sess,1);
																		}
																	if ( i_session[sess]>MAXPRGSESS )
																		i_session[sess]--;

																}
															switch(wahl)
																{	case XSETNO		:	{ sflags[sess][0] &= ~(F_MAIN|F_SET1|F_SET2|F_SET3|F_SET4|F_SET5);
																										trage_ein(sess,3);
																										break;
																									}
																	case XDELONE	:	{	if ( i_session[sess]>0 )
																											{	xx=i_session[sess];
																												*s_session[sess][xx][0]=
																												*s_session[sess][xx][1]=EOS;
																												w_session[sess][xx][0]=
																												w_session[sess][xx][1]=0;
																												i_session[sess]--;
																												while (i_session[sess]>0 &&
																															 *s_session[sess][i_session[sess]][0]==EOS)
																													i_session[sess]--;
																											}
																										trage_ein(sess,1);
																										break;
																									}
																	case XDELALL	:	{	i_session[sess]=0;
																										for (t=1; t<=MAXPRGSESS; t++)
																											{	*s_session[sess][t][0]=
																												*s_session[sess][t][1]=EOS;
																												w_session[sess][t][0]=
																												w_session[sess][t][1]=0;
																											}
																										trage_ein(sess,1);
																										break;
																									}
																	case XPROG1		:
																	case XPROG2		:
																	case XPROG3		:
																	case XPROG4		:
																	case XPROG5		:
																	case XPROG6		:
																	case XPROG7		:
																	case XPROG8		:
																	case XPROG9		:
																	case XPROG10	:
																	case XPROG11	:
																	case XPROG12	:	{	strcpy(temp,s_session[sess][wahl-XPROG1+1][1]);
																										if ( strncmp(temp,"###",3)==0 )		/* Es k”nnte Global sein */
																											{	t = atoi(temp+3);
																												strcpy(temp,program2[t]);
																												put_text(tree51,XCMD1,temp,55);
																												form_center(tree51,&nx,&ny,&nw,&nh);
																												form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												objc_draw(tree51,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																												wahl2=form_do(tree51,0) & 0x7FFF;
																												undo_state(tree51,wahl2,SELECTED);
																												form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												strcpy(temp,get_text(tree51,XCMD1));
																												if (wahl2==XCMDOK1)
																													{	if (form_alert(1,msg[26])==1)
																															{	/* if (strcmp(temp,program2[t])!=0) */
																																	{	strcpy(s_session[sess][wahl-XPROG1+1][0],program[t]);
																																		strcpy(s_session[sess][wahl-XPROG1+1][1],temp);
																																		trage_ein(sess,1);
																																	}
																															}
																													}
																												if (wahl2==XCMDGL1)
																													if (strcmp(temp,program2[t])!=0)
																														strcpy(program2[t],temp);
																												if (wahl2==XCMDINS)
																													wahl2=1001;
																												if (wahl2==XCMDDEL)
																													wahl2=1000;
																											}
																										else
																											{	put_text(tree52,XCMD2,temp,55);
																												form_center(tree52,&nx,&ny,&nw,&nh);
																												form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												objc_draw(tree52,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																												wahl2=form_do(tree52,0) & 0x7FFF;
																												undo_state(tree52,wahl2,SELECTED);
																												form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
																												strcpy(s_session[sess][wahl-XPROG1+1][1],get_text(tree52,XCMD2));
																												if (wahl2==XCMDINS2)
																													wahl2=1001;
																												if (wahl2==XCMDDEL2)
																													wahl2=1000;
																											}
																										if (wahl2==1001)
																											{	t=wahl-XPROG1+1;
																												if (t<=i_session[sess])
																													{	for (xx=MAXPRGSESS-1; xx>=t; xx--)
																															{	strcpy(s_session[sess][xx+1][0],s_session[sess][xx][0]);
																																strcpy(s_session[sess][xx+1][1],s_session[sess][xx][1]);
																																w_session[sess][xx+1][0]=w_session[sess][xx][0];
																																w_session[sess][xx+1][1]=w_session[sess][xx][1];
																															}
																														if (i_session[sess]<MAXPRGSESS)
																															i_session[sess]++;
																													}
																												*s_session[sess][t][0]=
																												*s_session[sess][t][1]=EOS;
																												w_session[sess][t][0]=
																												w_session[sess][t][1]=0;
																												trage_ein(sess,1);
																											}

																										if (wahl2==1000)
																											{	if ( i_session[sess]>0)
																													{	t=wahl-XPROG1+1;
																														if (t<=i_session[sess])
																															{	if (t<i_session[sess])
																																	{	for (xx=t; xx<i_session[sess]; xx++)
																																			{	strcpy(s_session[sess][xx][0],s_session[sess][xx+1][0]);
																																				strcpy(s_session[sess][xx][1],s_session[sess][xx+1][1]);
																																				w_session[sess][xx][0]=w_session[sess][xx+1][0];
																																				w_session[sess][xx][1]=w_session[sess][xx+1][1];
																																			}
																																	}
																																*s_session[sess][i_session[sess]][0]=
																																*s_session[sess][i_session[sess]][1]=EOS;
																																w_session[sess][i_session[sess]][0]=
																																w_session[sess][i_session[sess]][1]=0;
																																i_session[sess]--;
																															}
																													}
																												trage_ein(sess,1);
																											}
																										break;
																									}
																}
														}
					}
			}
		while (wahl!=XOK);
		changes |= C_SESSION;
		bestimme_sflags(sess);
		xform_maindialog(XFMD_DIALOG);
		return(retflag);
	}

void ersatz_str(int csess, int cxx, char *txt)
	{	strcpy(s_session[csess][cxx][0],txt);
		strcpy(s_session[csess][cxx][1],txt);
	}

void trage_ein(int csess, int cflag)
	{	int t,xx,arrow;
		char *pointer;
		char txt[PATH_MAX],temp[PATH_MAX];

		if (i_session[csess]==0)
			{	if (sflags[csess][0]+sflags[csess][1]!=0)
					{	sflags[csess][0] =
						sflags[csess][1] = 0;
						*s_timesess[csess][0] =
						*s_timesess[csess][1] = EOS;
						cflag=2;
					}
			}
		sprintf(txt,"%u",sflags[csess][1]);
		put_text(tree2,XLOOPCNT,txt,4);

		undo_xstate(tree2,SELECTED,XWAIT,XNOMAIL,XAUTOPOL,XLOOP,XSELECT,
															 XMAIN,XSET1,XSET2,XSET3,XSET4,XSET5,-1);
		
		if ( sflags[csess][0] & F_WAITALL		)	do_state(tree2,XWAIT,SELECTED);
		if ( sflags[csess][0] & F_NOMAIL		)	do_state(tree2,XNOMAIL,SELECTED);
		if ( sflags[csess][0] & F_AUTOPOLL	)	do_state(tree2,XAUTOPOL,SELECTED);
		if ( sflags[csess][0] & F_LOOP			)	do_state(tree2,XLOOP,SELECTED);
		if ( sflags[csess][0] & F_SELECTED	)	do_state(tree2,XSELECT,SELECTED);
		if ( sflags[csess][0] & F_MAIN			)	do_state(tree2,XMAIN,SELECTED);
		if ( sflags[csess][0] & F_SET1			)	do_state(tree2,XSET1,SELECTED);
		if ( sflags[csess][0] & F_SET2			)	do_state(tree2,XSET2,SELECTED);
		if ( sflags[csess][0] & F_SET3			)	do_state(tree2,XSET3,SELECTED);
		if ( sflags[csess][0] & F_SET4			)	do_state(tree2,XSET4,SELECTED);
		if ( sflags[csess][0] & F_SET5			)	do_state(tree2,XSET5,SELECTED);

		put_text(tree2,TIMEONE,s_timesess[csess][0],8);
		put_text(tree2,TIMETWO,s_timesess[csess][1],8);

		for(t=1; t<=MAXPRGSESS; t++)
			{	if ( *s_session[csess][t][0]!=EOS )
					{	arrow=FALSE;
						xx = -1;
						strcpy(txt,s_session[csess][t][0]);
						if ( strncmp(txt,"###",3)==0 )
							{	xx = atoi(txt+3);
								strcpy(txt,program[xx]);
								arrow=TRUE;
							}

						pointer=strrchr(txt,'\\');
						if (pointer!=NULL)
							strcpy(txt,pointer+1);

/*					pointer = strchr(txt,'.');
						if ( pointer!=NULL )
							{	*pointer=EOS;
								sprintf(temp,"%-8s.%-3s",txt,++pointer);
							}
						else
							{	strcpy(temp,avaltxt[8]);
							}
*/
						pointer = strchr(txt,'.');
						if ( pointer!=NULL )
							{	*pointer=EOS;
								sprintf(temp,"%-8s.%-3s",txt,++pointer);
							}
						else
							{	sprintf(temp,"%-8s    ",txt);
							}
						if (*txt==EOS)
							{	switch(xx)
									{ case  1 : strcpy(temp,"[EXPORTER ?]" ); break;
										case  2 : strcpy(temp,"[IMPORTER ?]" ); break;
										case  3 : strcpy(temp,"[CRUNCHER ?]" ); break;
										case  4 : strcpy(temp,"[SCANNER  ?]" ); break;
										case  5 : strcpy(temp,"[MSGEDITOR?]" ); break;
										case  6 : strcpy(temp,"[TXTEDITOR?]" ); break;
										case  7 : strcpy(temp,"[MAILER   ?]" ); break;
										case  8 : strcpy(temp,"[TERMINAL ?]" ); break;
										case  9 : strcpy(temp,"[UTILITY1 ?]" ); break;
										case 10 : strcpy(temp,"[UTILITY2 ?]" ); break;
										case 11 : strcpy(temp,"[UTILITY3 ?]" ); break;
										case 12 : strcpy(temp,"[UTILITY4 ?]" ); break;
										case 13 : strcpy(temp,"[UTILITY5 ?]" ); break;
										case 14 : strcpy(temp,"[UTILITY6 ?]" ); break;
										case 15 : strcpy(temp,"[UTILITY7 ?]" ); break;
										default : strcpy(temp,avaltxt[8]);
									}
							}

							

						if (arrow==TRUE)
							{	sprintf(txt,"\x03%s",temp);
							}
						else
							{	if (w_session[csess][t][0] & F_BATCH)
									sprintf(txt,"+%s",temp);
								else
									sprintf(txt," %s",temp);
							}
						put_text(tree2,XPROG1+t-1,txt,13);
					}
				else
					{	put_text(tree2,XPROG1+t-1," ________.___",13);
						w_session[csess][t][0] = 0;
					}
			}
		for (t=1; t<=MAXPRGSESS; t++)
			{	if ( is_state(tree2,XWAIT1+t-1,SELECTED) != (w_session[csess][t][0] & F_PRGWAIT) )
					{	if (w_session[csess][t][0] & F_PRGWAIT)
							do_state(tree2,XWAIT1+t-1,SELECTED);
						else
							undo_state(tree2,XWAIT1+t-1,SELECTED);
					}
				if ( is_state(tree2,XRED1+t-1,SELECTED) != (w_session[csess][t][0] & F_REDIR) )
					{	if (w_session[csess][t][0] & F_REDIR)
							do_state(tree2,XRED1+t-1,SELECTED);
						else
							undo_state(tree2,XRED1+t-1,SELECTED);
					}
				if (t>i_session[csess])
					{	OB_FLAGS(tree2,XWAIT1+t-1) |= HIDETREE;
						OB_FLAGS(tree2,XERR1 +t-1) |= HIDETREE;
						OB_FLAGS(tree2,XERR1 +t-1) &= ~EDITABLE;
						OB_FLAGS(tree2,XRED1 +t-1) |= HIDETREE;
						w_session[csess][t][1]=0;
					}
				else
					{	OB_FLAGS(tree2,XWAIT1+t-1) &= ~HIDETREE;
						OB_FLAGS(tree2,XERR1 +t-1) &= ~HIDETREE;
						OB_FLAGS(tree2,XERR1 +t-1) |= EDITABLE;
						OB_FLAGS(tree2,XRED1 +t-1) &= ~HIDETREE;
/* Wenn ber Find-1/2 installiert, dann ist hier keine "Redirection"
	 m”glich.
*/
						if ( *get_text(tree2,XPROG1+t-1)=='\x03' )
							OB_FLAGS(tree2,XRED1 +t-1) |= HIDETREE;
					}
				sprintf(txt,"%u",w_session[csess][t][1]);
				put_text(tree2,XERR1+t-1, w_session[csess][t][1]>0 ? txt : "",3);
			}

		if ( cflag==1 || cflag==2 )
			{	redraw_one_objc(tree2,XBOX,handle);
				redraw_one_objc(tree2,TIMEONE,handle);
				redraw_one_objc(tree2,TIMETWO,handle);
				redraw_objc(tree2,XBOX3,handle);
			}
		if ( cflag==2 )
			{	redraw_one_objc(tree2,XBOX1,handle);
				redraw_one_objc(tree2,XBOX2,handle);
			}
		if ( cflag==3 )
			{	redraw_one_objc(tree2,XBOX1,handle);
			}
	}

void bestimme_sflags(int csess)
	{	register int t;
		sflags[csess][0]=
		sflags[csess][1]=0;
		sflags[csess][1]=atoi(get_text(tree2,XLOOPCNT));
		if (is_state(tree2,XWAIT,1))		sflags[csess][0] |= F_WAITALL;
		if (is_state(tree2,XNOMAIL,1))	sflags[csess][0] |= F_NOMAIL;
		if (is_state(tree2,XAUTOPOL,1))	sflags[csess][0] |= F_AUTOPOLL;
		if (is_state(tree2,XLOOP,1))		sflags[csess][0] |= F_LOOP;
		if (is_state(tree2,XSELECT,1))	sflags[csess][0] |= F_SELECTED;
		if (is_state(tree2,XMAIN,1))		sflags[csess][0] |= F_MAIN;
		if (is_state(tree2,XSET1,1))		sflags[csess][0] |= F_SET1;
		if (is_state(tree2,XSET2,1))		sflags[csess][0] |= F_SET2;
		if (is_state(tree2,XSET3,1))		sflags[csess][0] |= F_SET3;
		if (is_state(tree2,XSET4,1))		sflags[csess][0] |= F_SET4;
		if (is_state(tree2,XSET5,1))		sflags[csess][0] |= F_SET5;
		for (t=1; t<=MAXPRGSESS; t++)
			{	if (is_state(tree2,XWAIT1+t-1,SELECTED))
					w_session[csess][t][0] |= F_PRGWAIT;
				else
					w_session[csess][t][0] &= ~F_PRGWAIT;
				if (is_state(tree2,XRED1+t-1,SELECTED))
					w_session[csess][t][0] |= F_REDIR;
				else
					w_session[csess][t][0] &= ~F_REDIR;
				if (t>i_session[csess])
					w_session[csess][t][1] = 0;
				else
					w_session[csess][t][1] = atoi(get_text(tree2,XERR1+t-1));
			}
		strcpy(s_timesess[csess][0],get_text(tree2,TIMEONE));
		if (*s_timesess[csess][0]!=EOS &&
				strlen(s_timesess[csess][0])<8)
			{	while (strlen(s_timesess[csess][0])<8)
					strcat(s_timesess[csess][0],"0");
				put_text(tree2,TIMEONE,s_timesess[csess][0],8);
				redraw_one_objc(tree2,TIMEONE,handle);
			}
		strcpy(s_timesess[csess][1],get_text(tree2,TIMETWO));
		if (*s_timesess[csess][1]!=EOS &&
				strlen(s_timesess[csess][1])<8)
			{	while (strlen(s_timesess[csess][1])<8)
					strcat(s_timesess[csess][1],"0");
				put_text(tree2,TIMETWO,s_timesess[csess][1],8);
				redraw_one_objc(tree2,TIMETWO,handle);
			}
	}

