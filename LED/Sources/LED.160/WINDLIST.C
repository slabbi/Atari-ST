#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "menu.rh"
#include "ledscan.h"
#include "winddefs.h"
#include "language.h"

static byte aname[MAX_STR]="";
static void show_list(word sline,word osline);

void my_gtext(word x, word y, byte *line, word effect_fl)
{	char tmp[256];
	int i;
	if (line!=NULL)
	{	
		if ((i=(int)strlen(line))<=127)
		{	memset(tmp,' ',127); tmp[127]=EOS;
			strncpy(tmp,line,i);
		}
		else
			strcpy(tmp,line);
		vst_effects(handle,0);
		vswr_mode(handle,MD_REPLACE);
		if (effect_fl)
		{
			if (effect_fl & EFF_INVERS)
			{	vswr_mode(handle,MD_ERASE);
				effect_fl &= ~EFF_INVERS;
			}	
			vst_effects(handle,effect_fl);
		}		
		vst_color(handle,BLACK);
		v_gtext(handle,x,y,tmp);
		vswr_mode(handle,MD_REPLACE);
		vst_effects(handle,0);
	}
}

word printline(long win, word lnr, word sel_fl)			/* ssl 111294 */
{	word x,y,cxy[4],pxy[4];
	word effect_fl;
	word show_lnr;

	if (!wi[win].lines[lnr]) return(0);
	if (wi[win].selline==-1) return(0);

	set_wifont(win);
/*set_wislider(win); */

	if (wi[win].listwind==WI_LIST_AREAS)
		update_arealistwindow(win,lnr);
	else if (wi[win].listwind==WI_LIST_MSGS)
		update_msglistwindow(win, atoi(wi[win].lines[ wi[win].selline ]+4)-1 );

	get_wiwork(win);
	x=wi[win].x+4;
	y=wi[win].y;
	show_lnr=lnr-wi[win].topline;

	if (sel_fl)
	{	cxy[0]=wi[win].x;
		cxy[1]=y + show_lnr*gl_hbox + 2;
		cxy[2]=cxy[0]+wi[win].w-1;
		cxy[3]=cxy[1]+gl_hbox-1;
	}	
	hide_mouse();
		
	x -= wi[win].firstc*gl_wbox;
	y += show_lnr*gl_hbox;
	y += gl_hbox;
	if (lnr < wi[win].lzahl)
	{	effect_fl=0;
		if (sel_fl)
			effect_fl |= EFF_INVERS;

		if (wi[win].lines[lnr][1]=='d')
			effect_fl |= EFF_HELL;
	#if 0
		else if (wi[win].lines[lnr][1]=='x')
			effect_fl |= EFF_KURSIV;
	#endif
/* Hier stand ein else vor dem if, genauso in WINDOW.C */
		if (wi[win].lines[lnr][0]=='\x3')	
			effect_fl |= EFF_FETT;

		wind_update(BEG_UPDATE);
		wind_get(wi[win].handle,WF_FIRSTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		while (pxy[2] && pxy[3])
		{	pxy[2] += pxy[0]-1;
		  pxy[3] += pxy[1]-1;
		  vs_clip(handle,TRUE,pxy);
			if (sel_fl)
				vr_recfl(handle,cxy);
				my_gtext(x,y,wi[win].lines[lnr],effect_fl);
	#if 0
					{	int pxy[4];
						pxy[0]=pxy[2]=wi[win].x;
						pxy[2]+=wi[win].w;
						pxy[1]=pxy[3]=y+1;
						vsl_color(handle,1);
						v_pline(handle,2,pxy);
					}
	#endif

		  wind_get(wi[win].handle,WF_NEXTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		}
		wind_update(END_UPDATE);
	}		
	show_mouse();
	return(0); /* ssl 71294 */
}

void list_edbutton(long win, word x, word y, word clicks)	/* ssl 81294 */
{	
	word ypos;
	word xwork, ywork, wwork, hwork;

	if (win>=0L && wi[win].listwind==WI_LIST_AREAS)
	{	wind_get(wi[win].handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
		if (x>xwork && y>ywork && x<xwork+wwork && y<ywork+hwork)
		{	ypos=(y-ywork-gl_hbox+gl_hchar)/gl_hbox;
			ypos+=wi[win].topline;
			if (ypos>=0 && ypos<wi[win].lzahl)
			{	printline(win,wi[win].selline,FALSE);
				printline(win,ypos,TRUE);
				wi[win].selline=ypos;
				if (clicks /*==2*/)   /* Einfachklick reicht aus */
					list_keybd(win,RETURN);
			}
		}
	}

	if (win>=0L && wi[win].listwind==WI_LIST_MSGS)
	{	wind_get(wi[win].handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
		if (x>xwork && y>ywork && x<xwork+wwork && y<ywork+hwork)
		{	ypos=(y-ywork-gl_hbox+gl_hchar)/gl_hbox;
			ypos+=wi[win].topline;
			if (ypos>=0 && ypos<wi[win].lzahl)
			{	printline(win,wi[win].selline,FALSE);
				printline(win,ypos,TRUE);
				wi[win].selline=ypos;
				if (clicks /*==2*/)   /* Einfachklick reicht aus */
					list_keybd(win,RETURN);
			}
		}
	}
}

word list_keybd(long win,word key)
{	
	long show_win;
	long laufw;
	long oldwin;
	word i,j,k,c,def_infoline=TRUE;
	word msgbuff[8];
	word stat;
	word openarea_fl,start;
	byte *cp;
	
	for (i=0; menu_keys[i] != -1; i+=3)
	{	if (menu_keys[i+2] == key)
		{	menu_tnormal(menu_tree,menu_keys[i],FALSE);
			return(hndl_menu(menu_keys[i],menu_keys[i+1]));
		}
	}
	
	if (win<0)
		return(FALSE);
	
	c=(byte)key;
	if (c>47 && c<58 && num_key(key) && wi[win].listwind==WI_LIST_MSGS)
	/* != SHFT_CD && key != SHFT_CU && key != SHFT_CL && key != SHFT_CR)*/
	{	if (win>=0)
		{	msg_goto(win,c-48);
			return(FALSE);
		}
	}

	hide_mouse();
	
	switch (key)
	{
		case ALT_8:		set_pointsize(win,8);
									def_infoline=FALSE;
						     	break;
		case ALT_9:		set_pointsize(win,9);
									def_infoline=FALSE;
						     	break;
		case ALT_0:		set_pointsize(win,10);
									def_infoline=FALSE;
						     	break;

		case HELPKEY:	show_mouse();
									hypertext(HYPER_LED,NULL);
									hide_mouse();
									def_infoline=FALSE;
									break;
		case SHFT_HELPKEY:
									show_mouse();
									hypertext(HYPER_LED_KEYS,NULL);
									hide_mouse();
									def_infoline=FALSE;
									break;

		case CUR_RIGHT:
		case ZEHN_PLUS:
		case CUR_DOWN:		/* Scroll one line down */
									if (wi[win].selline<wi[win].lzahl-1)	
									{	if (wi[win].selline<wi[win].topline || 
										    wi[win].selline>wi[win].topline+wi[win].maxshow)
										{	wi[win].selline=wi[win].topline;
											printline(win,wi[win].selline,TRUE);
										}
										else
										{ if (wi[win].selline+2>wi[win].topline+wi[win].maxshow)
											{	msgbuff[0]=WM_ARROWED;
											 	msgbuff[3]=wi[win].handle;
											 	msgbuff[4]=1;
											 	hndl_wind(msgbuff);
												printline(win,wi[win].selline,FALSE);
												printline(win,wi[win].selline+1,TRUE);
												wi[win].selline++;
											}
											else
											{	printline(win,wi[win].selline,FALSE);
												printline(win,wi[win].selline+1,TRUE);
												wi[win].selline++;
											}
										}
									}	
									throw_evntaway();
									break;	

		case CUR_LEFT:
		case ZEHN_MINUS:
		case CUR_UP:		/* Scroll one line up */
									if (wi[win].selline>0)
									{	if (wi[win].selline<wi[win].topline || 
										    wi[win].selline>wi[win].topline+wi[win].maxshow)
										{	wi[win].selline=wi[win].topline+wi[win].maxshow-1;
											printline(win,wi[win].selline,TRUE);
										}
										else
										{	if (wi[win].selline-1<wi[win].topline)
											{	msgbuff[0]=WM_ARROWED;
											 	msgbuff[3]=wi[win].handle;
												msgbuff[4]=0;
												hndl_wind(msgbuff);
												printline(win,wi[win].selline,FALSE);
												printline(win,wi[win].selline-1,TRUE);
												wi[win].selline--;
											}
											else
											{	printline(win,wi[win].selline,FALSE);
												printline(win,wi[win].selline-1,TRUE);
												wi[win].selline--;
											}
										}
									}
									throw_evntaway();
									break;	

		case CNTRL_RET:
									if (win==MSGSWIND && wi[win].listwind==WI_LIST_MSGS &&
											wi[win].selline>=0)
									{	HDR thdr;
										i =	wi[win].aktmsg = wi[win].selline+1;
						
										i =	wi[win].aktmsg =
												atoi( wi[win].lines[ wi[win].selline ]+4 );
						
										hdrseek(win,wi[win].hdrfp2,(i-1),SEEK_SET);
										hdrread(win,&thdr,1,wi[win].hdrfp2);
										if (!(thdr.wFlags & F_DELETED))
										{	for (j=0;j<MAX_WIND;j++)
											{	if (wi[j].belegt && !wi[j].editierbar && !wi[j].listwind &&
														wi[j].aktarea==wi[win].aktarea && j!=win)
												{	hdrseek(j,wi[j].hdrfp,(i-1),SEEK_SET);
													hdrread(j,&thdr,1,wi[j].hdrfp);
													if (!(thdr.wFlags & F_DELETED))
													{	thdr.wFlags|=F_DELETED;
														wi[j].msgda[i]=FALSE;
														hdrseek(j,wi[j].hdrfp,(i-1),SEEK_SET);
														hdrwrite(j,&thdr,1,wi[j].hdrfp);	
														berechne_msgp(j);
														need_update_msglistwindow(win,RETURN);
														
														c=-1;
														for (k=wi[win].selline; k<wi[win].lastmsg; k++)
															if (wi[j].msgda[atoi( wi[win].lines[ k ]+4 )]==TRUE)
															{	c=atoi( wi[win].lines[ k ]+4 );
																break;
															}
														if (c==-1)
														{	for (k=wi[win].selline; k>=0; k--)
																if (wi[j].msgda[atoi( wi[win].lines[ k ]+4 )]==TRUE)
																{	c=atoi( wi[win].lines[ k ]+4 );
																	break;
																}
														}
														if (c>=0) i=c;
						
														select_msg(j,i /*wi[j].aktmsg*/,TRUE,TRUE);
														need_update_msglistwindow(j,RETURN);
													}
													break;
												}
											}
										}
									}
									break;

		case ALT_RETURN:
									if (wi[win].selline>=0)
									{	for (j=0;j<MAX_WIND;j++)
										{	if (wi[j].belegt && !wi[j].editierbar && !wi[j].listwind &&
													wi[j].aktarea==wi[win].aktarea && j!=win)
											{	
												select_msg(j,atoi( wi[win].lines[ wi[win].selline ]+4 ),FALSE,TRUE);
												del_rek_subject(j);
											}
										}
									}
									throw_evntaway();
									break;

		case RETURN:
		case ENTER:		if (win==AREAWIND && wi[win].listwind==WI_LIST_AREAS)
									{	
										def_infoline=FALSE;

										wi[win].aktarea=wi[win].selline;
										for (laufw=0;laufw<MAX_WIND;laufw++)
										{	if (wi[laufw].belegt && wi[laufw].aktarea==wi[win].aktarea)
											{	if (wi[laufw].editierbar || wi[laufw].listwind)
													continue;
												if (opt.autoclose && wi[AREAWIND].belegt)
													close_window(AREAWIND);		/* ssl 190195 */
												top_it(laufw);
												return (FALSE);
											}
										}

										openarea_fl=TRUE;
										if (wi[AREAWIND].changearea)
										{	openarea_fl=FALSE;
											if (gl_oldwin>=0L && wi[gl_oldwin].belegt &&	
											    !wi[gl_oldwin].editierbar && !wi[gl_oldwin].listwind)
											{	close_area(gl_oldwin);
												if (open_area(gl_oldwin, wi[win].aktarea))
												{	wind_set(wi[gl_oldwin].handle,WF_NAME,(long)wi[gl_oldwin].areaname,0,0);
													select_msg(gl_oldwin,0,TRUE,TRUE);		/* 0 war wi[gl_oldwin].aktmsg */ 
													top_it(gl_oldwin);
												}
											}
											else /* altes msg-win suchen */ 
											{	if ((oldwin=get_firstmsgwind())>=0L)
												{	close_area(oldwin);
													if (open_area(oldwin, wi[win].aktarea))
													{	wind_set(wi[oldwin].handle,WF_NAME,(long)wi[oldwin].areaname,0,0);
														select_msg(oldwin,0,TRUE,TRUE);		/* 0 war wi[woldwin].aktmsg */ 
														top_it(oldwin);
													}
												}
												else
													openarea_fl=TRUE;
											}
										}
										if (openarea_fl) /* " Open Area " */
										{	show_win=get_windarea(win, wi[win].aktarea);
											if (show_win<0)
											{	biene();
												show_doing(DO_OPENMESSAGEAREA);
												if ((show_win=get_freewind())>=0)
												{	if (open_area(show_win, wi[win].aktarea))
													{	del_doing();

														open_window(show_win,wi[show_win].areaname,TRUE);
														stat=select_msg(show_win,0,FALSE,TRUE);		/* 0 war wi[win].aktmsg */ 

														set_comstatus(show_win,wi[show_win].aktmsg);
/*			chk_status(show_win,TRUE); */
														new_status(show_win,stat);
													}
												}	
												del_doing();
												mouse_normal();
											}
										}
										if ( (opt.autoclose & O_AUTOCLOSE_AREA) && wi[AREAWIND].belegt)
											close_window(AREAWIND);		/* ssl 190195 */
									}
						
									else if (win==MSGSWIND && wi[win].listwind==WI_LIST_MSGS &&
													 wi[win].selline>=0)
									{	HDR thdr;
										i =	wi[win].aktmsg = wi[win].selline+1;
						
										i =	wi[win].aktmsg =
												atoi( wi[win].lines[ wi[win].selline ]+4 );
						
										hdrseek(win,wi[win].hdrfp2,(i-1),SEEK_SET);
										hdrread(win,&thdr,1,wi[win].hdrfp2);
										if (thdr.wFlags & F_DELETED)
										{	for (j=0;j<MAX_WIND;j++)
											{	if (wi[j].belegt && !wi[j].editierbar && !wi[j].listwind &&
														wi[j].aktarea==wi[win].aktarea && j!=win)
												{	hdrseek(j,wi[j].hdrfp,(i-1),SEEK_SET);
													hdrread(j,&thdr,1,wi[j].hdrfp);
													if (thdr.wFlags & F_DELETED)
													{	thdr.wFlags&=~F_DELETED;
														wi[j].msgda[i]=TRUE;
														hdrseek(j,wi[j].hdrfp,(i-1),SEEK_SET);
														hdrwrite(j,&thdr,1,wi[j].hdrfp);	
														berechne_msgp(j);
														need_update_msglistwindow(win,RETURN);
													}
													break;
												}
											}
										}
						
										show_win=get_windarea(win, wi[win].oldarea);
										if (show_win<0)
										{	biene();
											show_doing(DO_OPENMESSAGEAREA);
											if ((show_win=get_freewind())>=0)
											{	if (open_area(show_win, wi[win].aktarea))
												{	del_doing();

													open_window(show_win,wi[show_win].areaname,TRUE);
													stat=select_msg(show_win,wi[win].aktmsg,FALSE,TRUE);

												set_comstatus(show_win,wi[show_win].aktmsg);
						/*					chk_status(show_win,TRUE); */
													new_status(show_win,stat);
												}
											}	
											del_doing();
											mouse_normal();
										}	
										else
										{	
											stat=select_msg(show_win,wi[win].aktmsg,FALSE,TRUE);	/* war: false,true */
											msgbuff[0]=WM_REDRAW;
											msgbuff[3]=wi[show_win].handle;
											msgbuff[4]=wi[show_win].x;
											msgbuff[5]=wi[show_win].y;
											msgbuff[6]=wi[show_win].w;
											msgbuff[7]=wi[show_win].h;
											hndl_wind(msgbuff);
											set_comstatus(show_win,wi[show_win].aktmsg);
											new_status(show_win,stat);
										}
										if ( (opt.autoclose & O_AUTOCLOSE_MSGS) && wi[MSGSWIND].belegt)
										{	close_window(MSGSWIND);
											opt.winprm[MSGSWIND][6]=0;
										}
									}
									break;

		case SHFT_CD: /* Display next page */
									msgbuff[0]=WM_ARROWED;
								 	msgbuff[3]=wi[win].handle;
								 	msgbuff[4]=1;
								 	hndl_wind(msgbuff);
								 	break;

		case SHFT_CU:	/* Display previous page */
									msgbuff[0]=WM_ARROWED;
								 	msgbuff[3]=wi[win].handle;
									msgbuff[4]=0;
									hndl_wind(msgbuff);
									break;	

		case SHFT_CL:	msgbuff[0]=WM_ARROWED;
								 	msgbuff[3]=wi[win].handle;
								 	msgbuff[4]=4;
								 	hndl_wind(msgbuff);
									break;

		case SHFT_CR:	msgbuff[0]=WM_ARROWED;
								 	msgbuff[3]=wi[win].handle;
									msgbuff[4]=5;
									hndl_wind(msgbuff);
									break;	

		case HOME:		if (win>=0 && wi[win].selline>0)
									{	wi[win].topline=
										wi[win].selline=0;
										wisetfiles(win,FALSE,NO_CHANGE);
									}
									break;
		case SHFT_HOME:
									if (win>=0)
						/*			{	if (wi[win].topline<wi[win].lzahl-wi[win].maxshow) */
									{	if (wi[win].selline<wi[win].lzahl-1)
										{	wi[win].topline=wi[win].lzahl-wi[win].maxshow;
											wi[win].selline=wi[win].lzahl-1;
											wisetfiles(win,FALSE,NO_CHANGE);
										}
									}	
									break;
		case UNDO:		/* case CNTRL_U wird in WM_CLOSED bersetzt */
									close_window(win);
									break;
		case ESC:			/* redraw */
									if (win>=0)
									{	if (wi[AREAWIND].belegt)		/* ssl 190395 */
										{	for (i=0;i<wi[AREAWIND].lzahl;i++)
												update_arealistwindow(AREAWIND,i);
										}
										msgbuff[0]=WM_REDRAW;
									 	msgbuff[3]=wi[win].handle;
										msgbuff[4]=wi[win].x;
										msgbuff[5]=wi[win].y;
										msgbuff[6]=wi[win].w;
										msgbuff[7]=wi[win].h;
										hndl_wind(msgbuff);
										def_infoline=FALSE;
									}
									break;

		case ZEHN_STAR:
									if (wi[win].listwind==WI_LIST_AREAS)
									{	k = NEWMSG;
										goto jump;
									}
									break;
		case ZEHN_SLASH:
									if (wi[win].listwind==WI_LIST_AREAS)
									{	k = UNRDMSG;
										goto jump;
									}
									break;
		case DOPPKREUZ:
									if (wi[win].listwind==WI_LIST_AREAS)
									{	k = TOSYSOP;
jump:
										if (win>=0)
										{	printline(win,wi[win].selline,FALSE);
											j=wi[win].selline;
											if (j>=gl_arzahl-1)
												i=j=0;
											else
												i=j+1;
											for (laufw=0;laufw<gl_arzahl;laufw++)
											{	if (gl_area[i].newmsg & k)
													break;
												if (i>=gl_arzahl-1)
													i=0;
												else
													i++;
											}
											if (laufw<gl_arzahl)
											{	show_list(i,-1);
											}
											printline(AREAWIND,wi[AREAWIND].selline,TRUE);
										}
									}
									break;

		default:			def_infoline=FALSE;

									if (wi[win].listwind==WI_LIST_AREAS)
									{	cp=strchr(aname,EOS);
										j=wi[AREAWIND].selline;
										start=0;

										if (key==BACKSPACE)
										{	if (cp>aname)
												*--cp=EOS;
										}
										else if (key==TAB)
										{	start=wi[win].selline+1;
											if (start>=wi[AREAWIND].lzahl)
												start=0;
										}
										else if (isgraph(key & 0xff))
										{	if (strlen(aname)<MAX_STR-1)
											{	*cp++=key & 0xff;
												*cp=EOS;
											}
										}
						
										printline(win,wi[win].selline,FALSE);
										if (*aname)
											if (key==TAB ||
													key==BACKSPACE || isgraph(key & 0xff))
											{	for (i=start;i<wi[AREAWIND].lzahl;i++)
												{	if (!strnicmp(gl_area[i].name,aname,strlen(aname)))
														break;
												}
												if (i>=wi[AREAWIND].lzahl)
												{	for (i=0;i<wi[AREAWIND].lzahl;i++)
													{	if (!strnicmp(gl_area[i].name,aname,strlen(aname)))
															break;
													}
												}
						
												if (i<wi[AREAWIND].lzahl)
													show_list(i,j);
												else
													*--cp=EOS;
											}

										areawind_successiv();

										if (!*aname)
										{	wi[AREAWIND].topline =
											wi[AREAWIND].selline = 0;
											if (j!=0) wisetfiles(AREAWIND,FALSE,NO_CHANGE);
										}
										printline(AREAWIND,wi[AREAWIND].selline,TRUE);
									}
									break;
	}
	if (def_infoline)
		no_areawind_successiv();

	show_mouse();
	return (FALSE);
}

static void show_list(word sline,word osline)
{	WIND *winp;
	winp=&wi[AREAWIND];
	if (sline>=0 && sline<winp->lzahl)
	{	winp->selline = sline;
		if (sline<winp->topline)
		{	winp->topline=
			winp->selline=sline;
			if (sline!=osline) wisetfiles(AREAWIND,FALSE,NO_CHANGE);
		}
		else if (sline>=winp->topline+winp->maxshow)
		{	set_wislider(AREAWIND);
			winp->topline=sline-winp->maxshow+1;
			winp->selline=sline;
			if (sline!=osline) wisetfiles(AREAWIND,FALSE,NO_CHANGE);
		}
	}
}

void no_areawind_successiv(void)
{	*aname=EOS;
	strcpy(wi[AREAWIND].status, wi[AREAWIND].changearea ? msgtxt[SYSTEM_CHANGEAREA].text
																				 							: msgtxt[SYSTEM_OPENAREA].text);
	if (wi[AREAWIND].belegt)
		upd_status(AREAWIND, wi[AREAWIND].status);
}

void areawind_successiv(void)
{	strcpy(wi[AREAWIND].status, wi[AREAWIND].changearea ? msgtxt[SYSTEM_CHANGEAREA].text
																				 							: msgtxt[SYSTEM_OPENAREA].text);
	if (*aname)
	{	strcat(wi[AREAWIND].status, " >");
		strcat(wi[AREAWIND].status, aname);
		strcat(wi[AREAWIND].status, "<");
	}
	if (wi[AREAWIND].belegt)
		upd_status(AREAWIND, wi[AREAWIND].status);
}

byte *get_successiv_areaname(void)
{	return(aname);
}

void top_it(long win)
{	word msgbuff[8];
	msgbuff[0]=WM_TOPPED;
	msgbuff[3]=wi[win].handle;
	hndl_wind(msgbuff);
}




#if 0
									if (wi[win].listwind==WI_LIST_AREAS)
									{	if (isalnum(key & 0xff))
										{	
						/*				for (i=0; i<MAX_WIND; i++)
												if (wi[i].belegt && wi[i].aktarea==wi[win].selline)
													update_arealistwindow(win, i);
						*/
											printline(win,wi[win].selline,FALSE);
											i = find_keyarea(key,wi[win].selline);
show_list:
											if (i>=0 && i<wi[win].lzahl)
											{	wi[win].selline = i;
												if (i<wi[win].topline)
												{	wi[win].topline=
													wi[win].selline=i;
													wisetfiles(win,FALSE,NO_CHANGE);
													break;
												}
												if (i>=wi[win].topline+wi[win].maxshow)
												{	set_wislider(win);
													wi[win].topline=i-wi[win].maxshow+1;
													wi[win].selline=i;
													wisetfiles(win,FALSE,NO_CHANGE);
													break;
												}
											}
											printline(win,wi[win].selline,TRUE);
										}
									}
									break;
#endif
