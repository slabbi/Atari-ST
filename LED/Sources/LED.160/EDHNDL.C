#include "ccport.h"
#include <time.h>
#include "menu.rh"
#include "helpcom.rh" 
#include "helpmsg.rh" 
#include "helpedit.rh" 
#include "helpdial.rh" 
#include "nodenum.rh"
#include "editline.rh" 
#include "editflag.rh"
#include "gotomsg.rh"
#include "ledscan.h"
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"
#include "vars.h"
#include "language.h"
#include "nodelist.h"

extern byte oldline[LINELEN+2];

extern byte msgfrequest[8][14];	/* hndlopt.c	*/
extern word msgfreqcount;				/* hndlopt.c	*/

word
title_select(void)
{	word state;
	state=((menu_tree+LED)->ob_state | (menu_tree+AREA)->ob_state |
	       (menu_tree+MSG)->ob_state | (menu_tree+EDIT)->ob_state | 
	       (menu_tree+OPTIONS)->ob_state | (menu_tree+HELP)->ob_state |
	       (menu_tree+FFILE)->ob_state | (menu_tree+BLOCK)->ob_state  |
	       (menu_tree+EXTRAS)->ob_state);
	if (state == SELECTED)
		return (TRUE);
	else
		return (FALSE);
}

word edmenu_event(word title, word item)
{	word flag;
	long win;
	uword zone,net,node,point;
	char tmp[MAX_STR],*cp /*,sz[10],sn[20]*/;

	flag=FALSE;
	win=found_wind();
	if (win>=0)
	{	hide_mouse();
		set_wiclip(win);
		set_wifont(win);
		cursor(win,FALSE);
		show_mouse();
	}
	if (!req_state(menu_tree,item & 0xfff,DISABLED))
	{	switch (item)
		{	case DABOUT:	dia_about();
										break;
			case HSHORT:	show_shortcut(win,NULL);
										break;


			case FWRITE:	if (win>=0)
										{	if (s_cutbuff!=-1 || e_cutbuff!=-1)
												zone=LangAlert(MSG_WRITE_BLOCKTEXT);
											else
												zone=2;
											if (zone==1)
												write_ascii(win,FWRITE,1);
											else
												write_ascii(win,FWRITE,2);
										}
										break;
			case FAPPEND:	if (win>=0)
										{	if (s_cutbuff!=-1 || e_cutbuff!=-1)
												zone=LangAlert(MSG_APPEND_BLOCKTEXT);
											else
												zone=2;
											if (zone==1)
												write_ascii(win,FAPPEND,1);
											else
												write_ascii(win,FAPPEND,2);
										}
										break;
			case FWCLIP:	if (win>=0)
										{	if (s_cutbuff!=-1 || e_cutbuff!=-1)
												zone=LangAlert(MSG_WRITE_BLOCKTEXT);
											else
												zone=2;
											if (zone==1)
												write_ascii(win,FWCLIP,1);
											else
												write_ascii(win,FWCLIP,2);
										}
										break;

			case FBREAD:	if (win>=0)
										{
											if (read_binary(win))
											{	wisetfiles(win,FALSE,NO_CHANGE);
												s_cutbuff=e_cutbuff=-1;
												upded_stat(win,0);
											}
										}
										break;
			case FREAD:
			case FRCLIP:	if (win>=0)
										{	
											if (read_ascii(win,item,NULL))
											{	wisetfiles(win,FALSE,NO_CHANGE);
												s_cutbuff=e_cutbuff=-1;
												upded_stat(win,0);
											}
										}
										break;
			case FEXEC:		execute(NULL,NULL,FALSE);
										break;			               
			case FQUIT:		show_mouse();
										flag=quit();
										break;
			case AOPEN:		/* Open new area */
										if (opt.listawindow)
											openarea_listwindow(-1,-1,FALSE);
										else
							         wiopen();
										break;

			case ACYCLE:	wicycle();
										break;
			case ACLOSE:	if (win>=0)
											close_window(win);
										break;
			case EABORT:	/* edit_end(win,FALSE);	*/
										if (win>=0)
											close_window(win);
										break;
			case 0x1000|EABORT:
										edit_end(win,FALSE);
										break;
			case ESAVE:		edit_end(win,TRUE);
										break;
			case ECC:			if (win>=0)
											dia_carbon();
										break;
			case BBEGIN:
			case BDELMARK:
			case BCUT:
			case BCOPY:
			case BPASTE:
			case BINDENT:
			case BEXTEND:	block_menu(item);
										break;
			case OSWITCH:	dia_optswitches();
										break;
			case OWIND:		dia_optwindow();
										break;
			case OCOMMENT:dia_optcomment();
										break;
			case OQUOTE:	dia_optquote();
										break;
			case OUSER:		dia_optuser();
										break;
			case OFONT:		dia_optfont();
										break;	
			case OFUNC:		dia_fkeys();
										break;
			case OSAVE:		save_options();
										break;	
			case OFREQ:		/* Make file-request file */
										dia_freq(win);
										break;
			case OUSERLST:init_userdia("", addr[0].zone, addr[0].net,
																		 addr[0].node, addr[0].point);	/* ssl 310495: zeile fehlte */
										if (userbox(2,TRUE,NNTO,tmp,&zone,&net,&node,&point) && win>=0)
										{	strcat(tmp," (");
											cp=tmp;
											while(*cp) output(win, *cp++, TRUE);
											strcpy(tmp,mergeaddr(zone,net,node,point,NULL));
											strcat(tmp,")");
											cp=tmp;
											while(*cp) output(win, *cp++, TRUE);
										}
										break;		
			case HMSG:		helping(hmsg_tree,&hmsg_dial);
										break;
			case HEDIT:		helping(hedit_tree,&hedit_dial);
										break;
			case HDIAL:		helping(hdial_tree,&hdial_dial);
										break;
			case HCOMMENT:/* Help for dialogs */
										helping(helpcom_tree,&helpcom_dial);
										break;

			case OFIND:		edkey_event(win,ALT_F);
			case OFSAME:	edkey_event(win,ALT_G);
										break;
		}
	}
	if ((win=found_wind())>=0 && wi[win].editierbar)
	{	
		if (opt.flag & O_FLAG_SHOWPOS)
			upded_stat(win,0);
		
		hide_mouse();
		cursor(win,TRUE);
		show_mouse();
	}
	menu_tnormal(menu_tree,title,TRUE);
	return (flag);
}

void block_menu(word item)
{	long win;

	if ((win=found_wind())>=0 && wi[win].editierbar==TRUE && 
	    wi[win].cy>=TOPMSG_LINE)
	{
		switch(item)
		{
			case BBEGIN:	
#if 0
										clear_cutbuff();		/* ssl 050495 */
#endif
										if (s_cutbuff<0)
										{	s_cutbuff=wi[win].lnr;
											if (e_cutbuff<0)
												e_cutbuff=wi[win].lzahl-1;
										}
										else if (s_cutbuff<=wi[win].lnr)
											e_cutbuff=wi[win].lnr;
										else
										{	e_cutbuff=s_cutbuff;
											s_cutbuff=wi[win].lnr;
										}
						 				upded_stat(win,0);
										if (s_cutbuff>=0 && e_cutbuff>=0)
										{	hide_mouse();
											pline(win, TOPMSG_LINE, wi[win].bzeilen, FALSE, FALSE);
											show_mouse();
										}
										break;

			case BDELMARK:del_blkbounds(win,TRUE);
										break;
			case BCUT:		cut_blk(win);
										break;
			case BCOPY:		copy_blk(win, TRUE);		/* OK */
										break;
			case BPASTE:	ins_blk(win);
										break;
			case BINDENT:	if (wi[win].indent<0 /*&& wi[win].cx<70*/)
											wi[win].indent=wi[win].cx;
										else
											wi[win].indent=-1;
										upded_stat(win,0);
										break;
			case BEXTEND:	extend_tab(win);
										break;
			case 8000:		del_blkbounds(win,TRUE);
										clear_cutbuff();
										break;
			case 8001:		del_blkbounds(win,TRUE);
										append_line(win);
										hide_mouse();
										c_dn(win);
										show_mouse();
										break;
		}
	}
}


word edkey_event(long win, word key)
{	
	word x,y,w;
	word msgbuff[8];
	ubyte c;
	uword i;
	byte *cp;
	byte tmp[MAX_STR];
	static byte srch[LINELEN]="\0";
	
	if (wi[win].cy >= (TOPMSG_LINE-1))
		if ((uword)key==CNTRL_F4)
			key=select_function(1);

	if (key!=ALT_D && key!=ALT_N && key!=ALT_S && key!=ALT_V && 
	    key!=ALT_F && key!=ALT_G &&
	    key!=ALT_Y && key!=ALT_E && key!=SHIFT_ALT_E &&
	    key!=CNTRL_F && key!=CNTRL_K && key!=CNTRL_Y &&
	    key!=ALT_H && key!=ALT_Z)
	{	for (i=0; menu_keys[i]!=-1; i+=3)
		{	if (menu_keys[i+2] == key)
			{	menu_tnormal(menu_tree,menu_keys[i],FALSE);
				return(edmenu_event(menu_keys[i],menu_keys[i+1]));
			}
		}
	}
	
	if (win<0)
		return(FALSE);

	set_wiclip(win);		/* ssl 181294 */
	set_wifont(win);		/* ssl 181294 */

	hide_mouse();
	cursor(win, FALSE);

	if (wi[win].cy < (TOPMSG_LINE-1))
	{	switch (key)      /* Msg-Header */
		{	
			case SHIFT_CNTRL_Z:
											return(edmenu_event(EDIT,EABORT|0x1000));
/*		case ALT_A:			return(edmenu_event(EDIT,EABORT)); */
			case HELPKEY:		show_mouse();
											hypertext(HYPER_LED,NULL);
											hide_mouse();
											break;
			case SHFT_HELPKEY:
											show_mouse();
											hypertext(HYPER_LED_KEYS,NULL);
											hide_mouse();
											break;

			case CUR_LEFT:	if (wi[win].pos>wi[win].edline)
											{	wi[win].cx--;
												wi[win].pos--;
											}
											break;
			case CUR_RIGHT:	if (*wi[win].pos)
											{	wi[win].cx++;
												wi[win].pos++;
											}
											break;
			case SHFT_CU:
			case SHFT_CL:		wi[win].cx=HDRCOL;
											wi[win].pos=wi[win].edline;
						     			break;
			case SHFT_CD:
			case SHFT_CR:		wi[win].cx=(word)strlen(wi[win].edline);
											wi[win].pos=(byte *)(wi[win].edline+(long)wi[win].cx);
											wi[win].cx+=HDRCOL;
											break;
			case BACKSPACE:	if (wi[win].pos>wi[win].edline)
											{	wi[win].cx--;
												wi[win].pos--;
											}
											else
												break;
			case DELETE:		cp=wi[win].pos;
											while (*cp++)
												*(cp-1)=*cp;
											get_xypos(win,&x,&y);
											w = (word)strlen(wi[win].pos) * gl_wbox;
											rcopy(x+gl_wbox, y, w, gl_hbox-1, x, y, S_ONLY);
											RWHITE(x+w,y,gl_wbox,gl_hbox-1);
											break;
			case INSERT:		upded_stat(win,CH_INSMD);
											break;
			case ESC:				wi[win].cx=HDRCOL;
											wi[win].pos=wi[win].edline;
											*wi[win].pos=EOS;
											get_xypos(win,&x,&y);
											w = wi[win].w-HDRCOL*gl_wbox;
											RWHITE(x,y,w,gl_hbox-1);
											break;
			case RETURN:
			case ENTER:			if (wi[win].cy==TO_LINE)
											{	word netmail;

												netmail=is_netmail(wi[win].aktarea);

												i=FALSE;
												if (!netmail && strlen(wi[win].hdr->sTo)==0)
												{
													if (gl_area[wi[win].aktarea].quotetype==QUOTE_NACHRICHT)
														strcpy(wi[win].hdr->sTo,"Alle");
													else
														strcpy(wi[win].hdr->sTo,"All");
													phdrline(win);
												}
												else if (!netmail && strlen(wi[win].hdr->sTo)<=2)
												{	
													xget_user_nobox(wi[win].hdr->sTo,FALSE,TRUE,
														             (uword *)&wi[win].hdr->wToZone,
														             (uword *)&wi[win].hdr->wToNet,
														             (uword *)&wi[win].hdr->wToNode,
														             (uword *)&wi[win].hdr->wToPoint,
														             gl_area[wi[win].aktarea].domain);
													phdrline(win);
													i=TRUE;
												}
							
												else if (netmail && strlen(wi[win].hdr->sTo)==0)
												{
													for (i=0;i<MAX_USERLIST;i++)
														if (strlen(opt.userlist[i])>=12 &&
																!stricmp(opt.userlist[i]+strlen(opt.userlist[i])-12,"PRVTUSER.LST"))
															break;
													if (i<MAX_USERLIST)
														xget_user_nobox(wi[win].hdr->sTo,TRUE,TRUE,
															             (uword *)&wi[win].hdr->wToZone,
															             (uword *)&wi[win].hdr->wToNet,
															             (uword *)&wi[win].hdr->wToNode,
															             (uword *)&wi[win].hdr->wToPoint,
															             gl_area[wi[win].aktarea].domain);
													phdrline(win);
													i=TRUE;
												}
												else if (netmail)
												{
													if (xget_user_nobox(wi[win].hdr->sTo,FALSE,TRUE,
														             (uword *)&wi[win].hdr->wToZone,
														             (uword *)&wi[win].hdr->wToNet,
														             (uword *)&wi[win].hdr->wToNode,
														             (uword *)&wi[win].hdr->wToPoint,
														             gl_area[wi[win].aktarea].domain))
														phdrline(win);
													i=TRUE;
												}
							
												if (i)
												{	
													if (wi[win].hdr->wToZone==0 || wi[win].hdr->wToNet==0)
														init_userdia(wi[win].hdr->sTo,
																				 addr[0].zone,addr[0].net,
																				 addr[0].node,addr[0].point);
													else
														init_userdia(wi[win].hdr->sTo,
														             wi[win].hdr->wToZone,wi[win].hdr->wToNet,
														             wi[win].hdr->wToNode,wi[win].hdr->wToPoint);
							
													if (!netmail)
													{	set_mflag(nodenum_tree,HIDETREE,FALSE,NNON,NNSTRING,-1);
														del_mflag(nodenum_tree,EDITABLE,FALSE,NNSTRING,-1);
														i=userbox(0,TRUE,NNTO,wi[win].hdr->sTo,
																								(uword *)&wi[win].hdr->wToZone,
																								(uword *)&wi[win].hdr->wToNet,
																								(uword *)&wi[win].hdr->wToNode,
																								(uword *)&wi[win].hdr->wToPoint);
														del_mflag(nodenum_tree,HIDETREE,FALSE,NNON,NNSTRING,-1);
														set_mflag(nodenum_tree,EDITABLE,FALSE,NNSTRING,-1);
													}
													else
													{
														i=userbox(0,TRUE,NNSTRING,wi[win].hdr->sTo,
																								(uword *)&wi[win].hdr->wToZone,
																								(uword *)&wi[win].hdr->wToNet,
																								(uword *)&wi[win].hdr->wToNode,
																								(uword *)&wi[win].hdr->wToPoint);
													}

													if ( (!i || strlen(wi[win].hdr->sTo)==0) &&
															netmail )
													{	edit_end(win,FALSE);
														show_mouse();
														return(FALSE);
													}
													else
														phdrline(win);
												}
											}
											c_return(win);
											break;
			case ALT_V:			edbutton_event(win,-1,3,0,0);
											break;
			default:				if ((wi[win].cy == TO_LINE && strlen(wi[win].hdr->sTo) <= 34) ||
											    (wi[win].cy == TOPIC_LINE && strlen(wi[win].hdr->sSubject) <= 68))
											{	c = key & 0xff;
												if (c > 31 && c != 127)
													output(win,c, TRUE);
											}
											break;
		}
	}
	else         /* in Msg-Text */
	{
		switch (key)                   
		{	
			case HELPKEY:		strcpy(tmp,wi[win].lines[wi[win].lnr]);
											if (*tmp==EOS) goto hyperhelp;
											cp=tmp+wi[win].cx;
											if (!isalnum(*cp)) goto hyperhelp;
											while (cp>=tmp && isalnum(*cp)) cp--;
											cp=strcpy(tmp,++cp);
											while (*cp && isalnum(*cp)) cp++;
											*cp=EOS;
											show_mouse();
											hypertext(HYPER_SEARCH,tmp);
											hide_mouse();
											break;							
hyperhelp:						show_mouse();
											hypertext(HYPER_LED,NULL);
											hide_mouse();
											break;
			case SHFT_HELPKEY:
											show_mouse();
											hypertext(HYPER_LED_KEYS,NULL);
											hide_mouse();
											break;

			case CNTRL_F:		block_menu(8000);	/* BFREEBUF */
											break;
			case CNTRL_K:		block_menu(8001);	/* BAPPLINE */
											break;
			case SHIFT_ALT_E:		/* Absatz Format bis Leerzeile */
			case ALT_E:					/* Absatz Format bis <cr>			 */
											FOREVER
											{	x=wi[win].lzahl;
												if (wi[win].lnr>=wi[win].lzahl-1)
													break;
												if (wi[win].lines[wi[win].lnr]==NULL)
													break;
												if (*wi[win].lines[wi[win].lnr]=='\n' ||
														*wi[win].lines[wi[win].lnr]==EOS)
													break;
												if (wi[win].lines[wi[win].lnr+1]==NULL)
													break;

												/* Abbruch bei Leerzeile */
												if (key==SHIFT_ALT_E &&
															(*wi[win].lines[wi[win].lnr+1]=='\n' ||
															 *wi[win].lines[wi[win].lnr+1]==EOS))
													break;

												cp=strchr(wi[win].lines[wi[win].lnr],EOS);

												/* Ein <cr> interessiert nicht */
												if (key==SHIFT_ALT_E && *(cp-1)=='\n')
													*(cp-1)=' ';

												if (*(cp-1)!='\n' && *(cp-1)!=' ')
												{	*cp++=' ';
													*cp=EOS;
												}

												if (*(cp-1)==' ')
												{	wi[win].oldcx=(word)strlen(wi[win].edline);
													set_cxpos(win);
													delete(win,TRUE,TRUE,FALSE);
													pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
													wi[win].oldcx=MSGCOL;
													set_cxpos(win);
													if (x==wi[win].lzahl)
														c_dn(win);
							 					}
							 					else
							 						break;
											}
											break;
			case SHIFT_CNTRL_Y:
			case SHIFT_ALT_Y:		/* Loescht einen kompletten Absatz bis Leerzeile */
											while (wi[win].lines[wi[win].lnr]!=NULL &&
														 *wi[win].lines[wi[win].lnr]!=EOS &&
														 *wi[win].lines[wi[win].lnr]!='\n')
												del_line(win,wi[win].lnr,FALSE);
											set_cxpos(win);
											hide_mouse();
											pline(win, TOPMSG_LINE, wi[win].bzeilen, FALSE, FALSE);
											show_mouse();

											throw_evntaway();
											break;
			case SHIFT_CNTRL_Z:
											return(edmenu_event(EDIT,EABORT|0x1000));
/*		case ALT_A:			if (win>=0)
												return(edmenu_event(EDIT,EABORT));
									  	break; */
			case CUR_UP:
			case CUR_DOWN:	throw_evntaway();
											if (key==CUR_UP) c_up(win);
											else					 	 c_dn(win);
											break;
			case CUR_LEFT:	c_lf(win);
											break;
			case CUR_RIGHT:	c_rt(win);
											break;
			case BACKSPACE:	backsp(win);
											break;
			case DELETE:		delete(win,TRUE,FALSE,TRUE);
											break;
			case SHFT_DEL:	delete(win,TRUE,TRUE,TRUE);
											break;
			case CNTRL_DEL:	if (wi[win].pos)
											{	if (!*wi[win].pos || *wi[win].pos=='\n')
												{	delete(win,TRUE,FALSE,TRUE);
												}
												else if (*wi[win].pos==' ')
												{	while	(*wi[win].pos && *wi[win].pos==' ')
														delete(win,FALSE,FALSE,TRUE);
													if (!*wi[win].pos)
														delete(win,TRUE,FALSE,TRUE);
												}
												else
												{	while	(*wi[win].pos && *wi[win].pos!=' ' && *wi[win].pos!='\n')
														delete(win,FALSE,FALSE,TRUE);
												}
											}
											break;
			case ESC:				cp=wi[win].lines[wi[win].lnr];
											if (cp!=NULL)
											{
												if (strlen(cp)>0 && *cp!='\n')
												{	strcpy(oldline,cp);
													*cp='\n';
													*(cp+1)=EOS;
												}
												else if (*oldline)
													strcpy(cp,oldline);
#if 0
												wi[win].indent=getindent(win,cp,NULL);
												if (wi[win].indent>=0)
													wi[win].oldcx=wi[win].indent;
												else
													wi[win].oldcx=MSGCOL;
#else
												wi[win].oldcx=0;
#endif
												set_firstc(win,wi[win].oldcx);
												set_cxpos(win);
												pline(win,wi[win].cy,wi[win].cy,FALSE,FALSE);
											}
											break;
			case INSERT:		upded_stat(win,CH_INSMD);
											break;
			case TAB:				insert_tab(win,TRUE);
											break;
			case HOME:			wi[win].topline=0;
											wi[win].lnr=0;
											wi[win].cy=TOPMSG_LINE;
											wi[win].oldcx=MSGCOL;
											set_firstc(win,wi[win].oldcx);
											set_cxpos(win);
											pline(win,TOPMSG_LINE,wi[win].bzeilen,TRUE,TRUE);
											break;
			case SHFT_HOME:	wi[win].lnr=wi[win].lzahl-1;
											cp=wi[win].lines[wi[win].lnr];
											while (cp==NULL && wi[win].lnr>0)
												wi[win].lnr--;
											if (cp!=NULL)
											{
												wi[win].topline=wi[win].lnr-(wi[win].bzeilen-1-TOPMSG_LINE);
												if (wi[win].topline<0)
													wi[win].topline=0;
												wi[win].oldcx=MSGCOL;
												set_firstc(win,wi[win].oldcx);
												set_cxpos(win);
												wi[win].cy=TOPMSG_LINE+wi[win].lnr-wi[win].topline;
												pline(win,TOPMSG_LINE,wi[win].bzeilen,TRUE,TRUE);
											}
											break;
			case RETURN:
			case ENTER:			c_return(win);
											break;
			case SHFT_CU:		msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
											msgbuff[4]=0;
											hndl_wind(msgbuff);
											break;
			case SHFT_CD:		msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
										 	msgbuff[4]=1;
											hndl_wind(msgbuff);
											break;
			case SHFT_CL:		wi[win].oldcx=MSGCOL;
											set_cxpos(win);
							     		break;
			case SHFT_CR:		wi[win].oldcx=(word)strlen(wi[win].edline);
											if (*(wi[win].edline+(long)wi[win].oldcx-1L)=='\n')
												wi[win].oldcx--;
											set_cxpos(win);
											break;

			case CNTRL_CU:	/* Seitenanfang */
											wi[win].lnr -= wi[win].cy-TOPMSG_LINE;
											wi[win].cy = TOPMSG_LINE;
											set_cxpos(win);
											break;
											
			case CNTRL_CD:	/* Seitenende */
											wi[win].lnr += wi[win].bzeilen-wi[win].cy-1;
											wi[win].cy = wi[win].bzeilen-1;
											while (!wi[win].lines[wi[win].lnr])
											{	wi[win].lnr--;
												wi[win].cy--;
											}
											set_cxpos(win);
											break;

			case CNTRL_CL:	/* Wortanfang suchen */
											while ((wi[win].lnr>0 || wi[win].pos>wi[win].edline) && 
											       !isspace(*wi[win].pos))
												c_lf(win);
											/* Leerzeichen berspringen */
											while ((wi[win].lnr>0 || wi[win].pos>wi[win].edline) && 
											       isspace(*wi[win].pos))
												c_lf(win);
							   			break;

			case CNTRL_CR:	/* Wortende suchen */
											while ((wi[win].lines[wi[win].lnr+1]!=NULL || 
											       (*wi[win].pos!='\n' && *wi[win].pos!=EOS)) && 
											       !isspace(*wi[win].pos))
												c_rt(win);
											/* Leerzeichen berspringen */
											while ((wi[win].lines[wi[win].lnr+1]!=NULL || 
											       (*wi[win].pos!='\n' && *wi[win].pos!=EOS)) && 
											       isspace(*wi[win].pos))
												c_rt(win);

											if (!(*wi[win].pos))
												c_rt(win);
											break;

			case CNTRL_Y:		del_line(win,wi[win].lnr,TRUE);
											throw_evntaway();
											break;
			case ALT_Y:
			case UNDO:			ins_line(win,wi[win].lnr,NULL,FALSE,TRUE);
											break;
			case ALT_D:			edbutton_event(win,-1,0,0,0);
											break;
			case ALT_N:			edbutton_event(win,-1,1,0,0);
											break;
			case ALT_S:			edbutton_event(win,-1,2,0,0);
											break;
			case ALT_V:			edbutton_event(win,-1,3,0,0);
											break;
#if 0
			case ALT_Z:			set_dtext(edline_tree,ELTEXT,msgtxt[DIV_EDITLINEALERT].text,FALSE,6);
											set_dtext(edline_tree,ELLINE,wi[win].alert,FALSE,66);
											draw_dialog(edline_tree, &edline_dial);
											ext_btn=Form_do(&edline_dial,ELLINE);
											del_state(edline_tree,ext_btn,SELECTED,FALSE);
											del_dialog(&edline_dial);
											if (ext_btn==ELOK)
												get_dedit(edline_tree,ELLINE,wi[win].alert);
											break;
#endif
			case ALT_F:			hide_mouse();
											cursor(win,FALSE);
											show_mouse();
											set_dtext(edline_tree,ELTEXT,msgtxt[SYSTEM_EDITLINEFIND].text,FALSE,6);
											set_dtext(edline_tree,ELLINE,srch,FALSE,66);
											draw_dialog(edline_tree, &edline_dial);
											i=Form_do(&edline_dial,ELLINE);
											del_state(edline_tree,i,SELECTED,FALSE);
											del_dialog(&edline_dial);
											hide_mouse();
											cursor(win,TRUE);
											show_mouse();
											if (i!=ELOK)
												break;
											get_dedit(edline_tree,ELLINE,srch);
			case ALT_G:			if (*srch==EOS)
												break;
											hide_mouse();
											cursor(win,FALSE);
											show_mouse();
											strupr(srch);
											i=wi[win].lnr+1;
											while (wi[win].lines[i]!=NULL)
											{
												strcpy(tmp,wi[win].lines[i]);
												strupr(tmp);
												if ((cp=strstr(tmp,srch))!=NULL)
													break;
												i++;
											}
											if (wi[win].lines[i]!=NULL && cp!=NULL)
											{
												x=(word)(cp-tmp);
												if (x>=MSGCOL && x<=strlen(wi[win].lines[i]))
												{
													wi[win].oldcx=x;
													wi[win].lnr=i;
													if (wi[win].lzahl>wi[win].maxshow)
													{
														wi[win].topline=i+TOPMSG_LINE-wi[win].cy;
														if (wi[win].topline<0)
															wi[win].topline=0;
														else if (wi[win].topline>wi[win].lzahl-wi[win].maxshow)
															wi[win].topline=wi[win].lzahl-wi[win].maxshow;
														wi[win].cy=i-wi[win].topline+TOPMSG_LINE;
													}
													else
													{
														wi[win].topline=0;
														wi[win].cy=i+TOPMSG_LINE;
														if (wi[win].cy>TOPMSG_LINE+wi[win].lzahl-1)
															wi[win].cy=TOPMSG_LINE+wi[win].lzahl-1;
													}
													set_cxpos(win);
													pline(win,TOPMSG_LINE,wi[win].bzeilen,TRUE,TRUE);
												}
											}
											hide_mouse();
											cursor(win,TRUE);
											show_mouse();
											break;
			case F1:				fkey_pressed(win,0);	break;
			case F2:				fkey_pressed(win,1);	break;
			case F3:				fkey_pressed(win,2);	break;
			case F4:				fkey_pressed(win,3);	break;
			case F5:				fkey_pressed(win,4);	break;
			case F6:				fkey_pressed(win,5);	break;
			case F7:				fkey_pressed(win,6);	break;
			case F8:				fkey_pressed(win,7);	break;
			case F9:				fkey_pressed(win,8);	break;
			case F10:				fkey_pressed(win,9);	break;
			case SHFT_F1:		fkey_pressed(win,10);	break;
			case SHFT_F2:		fkey_pressed(win,11);	break;
			case SHFT_F3:		fkey_pressed(win,12);	break;
			case SHFT_F4:		fkey_pressed(win,13);	break;
			case SHFT_F5:		fkey_pressed(win,14);	break;
			case SHFT_F6:		fkey_pressed(win,15);	break;
			case SHFT_F7:		fkey_pressed(win,16);	break;
			case SHFT_F8:		fkey_pressed(win,17);	break;
			case SHFT_F9:		fkey_pressed(win,18);	break;
			case SHFT_F10:	fkey_pressed(win,19);	break;
			default:
#if FALSE
											sprintf(tmp,"[1][You pressed: %04X][  [OK  ]",key);
											form_alert(1,tmp);
#endif
											i=key;
											if (i==ZEHN_ALT_0 || i==ZEHN_ALT_1 || i==ZEHN_ALT_2 ||
													i==ZEHN_ALT_3 || i==ZEHN_ALT_4 || i==ZEHN_ALT_5 ||
													i==ZEHN_ALT_6 || i==ZEHN_ALT_7 || i==ZEHN_ALT_8 ||
													i==ZEHN_ALT_9)
											{	show_mouse();
												set_dbutton(gomsg_tree,GMTEXT,msgtxt[DIV_ASCIICODE].text,FALSE);
												*tmp=key & 0xff;
												tmp[1]=EOS;
												set_dtext(gomsg_tree,GMSGNR,tmp,FALSE,4);
												draw_dialog(gomsg_tree, &gomsg_dial);
												i=Form_do(&gomsg_dial,GMSGNR);
												del_dialog(&gomsg_dial);
												del_state(gomsg_tree,i,SELECTED,FALSE);
												hide_mouse();
												if (i==GOK)
												{	get_dedit(gomsg_tree,GMSGNR,tmp);
													i=atoi(tmp);
													key=i;
												}
												else
													key=0;
											}

											if (graphic_output(win,key))
											{	break;
											}
											else
											{	if ((i & 0xff00)>=0x6700 &&
														(i & 0xff00)<=0x7100)
													i=i-0x80;
											}

											c = (ubyte) (key & 0xff);
											if (c > 31 && c != 127)
												output(win,c, TRUE);
											break;
		}
	}

	if (opt.flag & O_FLAG_SHOWPOS)
		upded_stat(win,0);

	cursor(win, TRUE);
	show_mouse();
	return(FALSE);
}

word graphic_output(long win, word key)
{	
	switch (key)
	{
		case ZEHN_SHFT_1:
			output(win,200, TRUE);
			break;
		case ZEHN_SHFT_2:
			output(win,202, TRUE);
			break;
		case ZEHN_SHFT_3:
			output(win,188, TRUE);
			break;
		case ZEHN_SHFT_4:
			output(win,204, TRUE);
			break;
		case ZEHN_SHFT_5:
			output(win,206, TRUE);
			break;
		case ZEHN_SHFT_6:
			output(win,185, TRUE);
			break;
		case ZEHN_SHFT_7:
			output(win,201, TRUE);
			break;
		case ZEHN_SHFT_8:
			output(win,203, TRUE);
			break;
		case ZEHN_SHFT_9:
			output(win,187, TRUE);
			break;
		case ZEHN_SHFT_0:
			output(win,205, TRUE);
			break;
		case ZEHN_SHFT_PT:
			output(win,186, TRUE);
			break;
		case ZEHN_CT_1:
			output(win,192, TRUE);
			break;
		case ZEHN_CT_2:
			output(win,193, TRUE);
			break;
		case ZEHN_CT_3:
			output(win,217, TRUE);
			break;
		case ZEHN_CT_4:
			output(win,195, TRUE);
			break;
		case ZEHN_CT_5:
			output(win,197, TRUE);
			break;
		case ZEHN_CT_6:
			output(win,180, TRUE);
			break;
		case ZEHN_CT_7:
			output(win,218, TRUE);
			break;
		case ZEHN_CT_8:
			output(win,194, TRUE);
			break;
		case ZEHN_CT_9:
			output(win,191, TRUE);
			break;
		case ZEHN_CT_0:
			output(win,196, TRUE);
			break;
		case ZEHN_CT_PT:
			output(win,179, TRUE);
			break;
		default:
			return FALSE;	
	}
	return TRUE;
}

void seteditflg(long win, word index, word flag)
{	if (wi[win].hdr->wFlags & flag)
		set_state(edflag_tree,index,SELECTED,FALSE);
	else	
		del_state(edflag_tree,index,SELECTED,FALSE);
}

void geteditflg(long win, word index, word flag)
{	if (req_state(edflag_tree,index,SELECTED))
		wi[win].hdr->wFlags |= flag;
	else
		wi[win].hdr->wFlags &= ~flag;
}

#if defined(__NEWFIDO__)
	void setxeditflg(long win, word index, ulong flag)
	{	if (wi[win].hdr->XFlags & flag)
			set_state(edflag_tree,index,SELECTED,FALSE);
		else	
			del_state(edflag_tree,index,SELECTED,FALSE);
	}
	
	void getxeditflg(long win, word index, ulong flag)
	{	if (req_state(edflag_tree,index,SELECTED))
			wi[win].hdr->XFlags |= flag;
		else
			wi[win].hdr->XFlags &= ~flag;
	}
#else
	void setxeditflg(long win, word index, uword flag)
	{	if (wi[win].hdr->XFlags & flag)
			set_state(edflag_tree,index,SELECTED,FALSE);
		else	
			del_state(edflag_tree,index,SELECTED,FALSE);
	}
	
	void getxeditflg(long win, word index, uword flag)
	{	if (req_state(edflag_tree,index,SELECTED))
			wi[win].hdr->XFlags |= flag;
		else
			wi[win].hdr->XFlags &= ~flag;
	}
#endif


void setdia_flags(long win)
{	seteditflg(win,EPRIVATE,F_PRIVATE);
	seteditflg(win,EWITH,F_FILEATTACH);
	seteditflg(win,EHOLD,F_HOLD);
	seteditflg(win,ECRASH,F_CRASH);
	seteditflg(win,EKILL,F_KILLSENT);
	seteditflg(win,ERETREC,F_RETRECREQ);
	seteditflg(win,EAUDREC,F_AUDITREQ);

	setxeditflg(win,EDIRECT,XF_DIRECT);
	setxeditflg(win,EIMME,XF_IMMEDIATE);
	setxeditflg(win,EKILLFL,XF_KILLFILESENT);
	setxeditflg(win,EARCHIV,XF_ARCHIVSENT);
	setxeditflg(win,EFIXED,XF_FIXEDADDRESS);
	setxeditflg(win,EMULTICC,XF_MULTIPLECC);
}

void getdia_flags(long win)
{	geteditflg(win,EPRIVATE,F_PRIVATE);
	geteditflg(win,EWITH,F_FILEATTACH);
	geteditflg(win,EHOLD,F_HOLD);
	geteditflg(win,ECRASH,F_CRASH);
	geteditflg(win,EKILL,F_KILLSENT);
	geteditflg(win,ERETREC,F_RETRECREQ);
	geteditflg(win,EAUDREC,F_AUDITREQ);

	getxeditflg(win,EDIRECT,XF_DIRECT);
	getxeditflg(win,EIMME,XF_IMMEDIATE);
	getxeditflg(win,EKILLFL,XF_KILLFILESENT);
	getxeditflg(win,EARCHIV,XF_ARCHIVSENT);
	getxeditflg(win,EFIXED,XF_FIXEDADDRESS);
	getxeditflg(win,EMULTICC,XF_MULTIPLECC);
}

void edbutton_event(long win,word x,word y,word clicks,word state)
{	
	word tolen;
	word cx,cy;
	word ext_btn;
	word xwork, ywork, wwork, hwork;
/* byte fflags1[MAX_STR], fflags2[MAX_STR]; */

	if (win>=0L)
	{	hide_mouse();
		set_wiclip(win);
		set_wifont(win);
		cursor(win,FALSE);
		show_mouse();
		wind_get(wi[win].handle,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
		if ((x>xwork && y>ywork && x<xwork+wwork && y<ywork+hwork) ||
		     x==-1)
		{
			tolen=(word)strlen(wi[win].hdr->sTo);
			if (x!=-1)
			{	cx=(x-xwork-4)/gl_wbox;
				cy=(y-ywork-gl_hbox+gl_hchar)/gl_hbox;
			}
			
			if (wi[win].cy>TO_LINE && 
			   ((cy==TO_LINE && cx>=HDRCOL && cx<tolen+HDRCOL) ||
			    (x==-1 && y==0)))
			{     /* "To: "-Zeile Adressatname */

				if (!is_netmail(wi[win].aktarea))
				{	set_mflag(nodenum_tree,HIDETREE,FALSE,NNON,NNSTRING,-1);
					del_flag(nodenum_tree,NNSTRING,EDITABLE,FALSE);
				}
				init_userdia(wi[win].hdr->sTo,wi[win].hdr->wToZone,wi[win].hdr->wToNet,
				             wi[win].hdr->wToNode,wi[win].hdr->wToPoint);
				userbox(0,TRUE,NNTO,wi[win].hdr->sTo,
													(uword *)&wi[win].hdr->wToZone,
													(uword *)&wi[win].hdr->wToNet,
													(uword *)&wi[win].hdr->wToNode,
													(uword *)&wi[win].hdr->wToPoint);
				phdrline(win);

				del_mflag(nodenum_tree,HIDETREE,FALSE,NNON,NNSTRING,-1);
				set_flag(nodenum_tree,NNSTRING,EDITABLE,FALSE);
			}		
			else if (wi[win].cy>TO_LINE && 
			        ((cy==TO_LINE && cx>tolen+HDRCOL && cx<tolen+HDRCOL+18) ||
			         (x==-1 && y==1)))
			{      /* Nodenumber */
				if (is_netmail(wi[win].aktarea))
				{
					init_userdia(wi[win].hdr->sTo,wi[win].hdr->wToZone,wi[win].hdr->wToNet,
					             wi[win].hdr->wToNode,wi[win].hdr->wToPoint);
					userbox(0,TRUE,NNSTRING,wi[win].hdr->sTo,
														(uword *)&wi[win].hdr->wToZone,
														(uword *)&wi[win].hdr->wToNet,
														(uword *)&wi[win].hdr->wToNode,
														(uword *)&wi[win].hdr->wToPoint);
					phdrline(win);
				}
			}
			else if (wi[win].cy!=TOPIC_LINE && 
			        ((cy==TOPIC_LINE && cx>=HDRCOL) ||
			         (x==-1 && y==2)))
			{     /* "Subj.: " Zeile */
				set_dtext(edline_tree,ELTEXT,msgtxt[DIV_EDITLINESUBJ].text,FALSE,6);
				set_dtext(edline_tree,ELLINE,wi[win].hdr->sSubject,FALSE,66);
				draw_dialog(edline_tree, &edline_dial);
				ext_btn=Form_do(&edline_dial,ELLINE);
				del_state(edline_tree,ext_btn,SELECTED,FALSE);
				del_dialog(&edline_dial);
				if (ext_btn==ELOK)
				{	get_dedit(edline_tree,ELLINE,wi[win].hdr->sSubject);
					hide_mouse();
					psubjline(win);
					show_mouse();
				}
			}
			else if (is_netmail(wi[win].aktarea) && 
			        (( (cy==0 || cy==1) && (cx>45 || cx>=wi[win].bspalten-2)) ||
			        (x==-1 && y==3)))
			{      /*  Flags */
				setdia_flags(win); 
				if (!is_netmail(wi[win].aktarea))
					set_flag(edflag_tree,EWITH,HIDETREE,FALSE);
				
				draw_dialog(edflag_tree, &edflag_dial);
				ext_btn=Form_do(&edflag_dial,0);
				del_dialog(&edflag_dial);
				
				if (!is_netmail(wi[win].aktarea))
					del_flag(edflag_tree,EWITH,HIDETREE,FALSE);
				
				if (ext_btn == EOK)
				{
					getdia_flags(win);
/*				get_flags(win,fflags1,fflags2);
					sprintf(fflags,"%30.30s",flags); */

					if (wi[win].hdr->wFlags & F_FILEATTACH)
					{	if (hole_file(wi[win].hdr->sSubject,NULL,"*.*",msgtxt[FILE_FILETOSEND].text)==1)
						{	if (wi[win].cy==TOPIC_LINE)
							{	wi[win].cx=HDRCOL;
								wi[win].pos=wi[win].edline;
							}
						}
					}

					hide_mouse();
					redraw_all();
					show_mouse();
				}
				del_state(edflag_tree,ext_btn,SELECTED,FALSE);
			}
			else if (wi[win].cy>=TOPMSG_LINE && 
			         cy>=TOPMSG_LINE && cx>=MSGCOL && x!=-1)
			{
				tolen=cy-TOPMSG_LINE+wi[win].topline;

				if (tolen<wi[win].lzahl)
				{
					if (state==3)
					{	if (clicks==1)
						{	s_cutbuff=tolen;
							if (e_cutbuff<0)
								e_cutbuff=wi[win].lzahl-1;
						}
						else if (clicks==2)
						{	e_cutbuff=tolen;
						}
						upded_stat(win,0);
						if (s_cutbuff>=0 && e_cutbuff>=0)
						{	if (s_cutbuff>e_cutbuff)
							{	tolen=s_cutbuff;
								s_cutbuff=e_cutbuff;
								e_cutbuff=tolen;
							}
							hide_mouse();
							pline(win, TOPMSG_LINE, wi[win].bzeilen, FALSE, FALSE);
							show_mouse();
						}
					}
				}

				if (wi[win].lines[tolen]!=NULL)
				{
					hide_mouse();
					wi[win].lnr=tolen;
					wi[win].cy=cy;
					wi[win].oldcx=wi[win].firstc+cx;
					set_cxpos(win);
					show_mouse();
				}
			}
		}

		if (opt.flag & O_FLAG_SHOWPOS)
			upded_stat(win,0);

		hide_mouse();
		cursor(win,TRUE);
		show_mouse();
	}
}

static word get_xpos(long win,byte *text, word x, word column)
{	byte tmp[MAX_STR],*cp,*ct;
	word cx=-1,width=0,ext[8];
	cp=text;
	ct=tmp;
	while (*cp && width<x)
	{	
		*ct=*cp;
		ct[1]=EOS;
		vqt_extent(handle,tmp,ext);
		width=(wi[win].x - gl_wbox*column) + (ext[2]-ext[0]);
		ct++;
		cp++;
		cx++;
	}
	return(cx);
}

static void scan_bnl_flags(uword modemtype,byte *dum)
{	byte *cp;
	*dum=EOS;
	if (modemtype &     1u) strcat(dum,"HST,");
	if (modemtype &	    2u) strcat(dum,"PEP,");
	if (modemtype &	    4u) strcat(dum,"MNP,");
	if (modemtype & 	  8u) strcat(dum,"V32,");
	if (modemtype & 	 16u) strcat(dum,"V32b,");
	if (modemtype & 	 32u) strcat(dum,"V42,");
	if (modemtype &  	 64u) strcat(dum,"V42b,");
	if (modemtype &   128u) strcat(dum,"H16,");
	if (modemtype &   256u) strcat(dum,"ZYX,");
	if (modemtype &   512u) strcat(dum,"Z19,");
	if (modemtype &  1024u) strcat(dum,"V32T,");
	if (modemtype &  2048u) strcat(dum,"V34,");
	if (modemtype &  4096u) strcat(dum,"ISDNA,");
	if (modemtype &  8192u) strcat(dum,"ISDNB,");
	if (modemtype & 16384u) strcat(dum,"ISDNC,");
	if (modemtype & 32768u) strcat(dum,"MAX,");
	cp=strchr(dum,EOS);
	if (cp>dum) *--cp=EOS;
}

#pragma warn -par

/* state=Status der Sondertasten */
void msg_button(long win,word x,word y,word clicks,word state)
{
	word cx,cy,ext[8],width,i,effect_found,cnt=0,found_inverse=FALSE,
			 search4address=FALSE;
	byte *cp,*cp2,*cp3;
	byte tmp[MAX_STR],tmp2[1024],tmp3[MAX_STR];
	struct ffblk dta;

	if (win<0) return;

	set_wiclip(win);
	set_wifont(win);
	get_wiwork(win);

	if (!wi[win].editierbar && !wi[win].listwind)
	{	/* Click in Infoline */

		if (opt.showcom &&
				x>=wi[win].x && x<wi[win].x+wi[win].w &&
				y>=wi[win].y-(gem_statusline*gl_hbox) && y<wi[win].y+gl_hbox)
		{	
			
			if (!gem_statusline)
				cx = get_xpos(win,wi[win].status,x,0);
			else
				cx = ( x - wi[win].x ) / gl_syswchar;

			if (cx>=1 && cx<strlen(wi[win].status))
			{	cp=&wi[win].status[cx];
				while( isdigit(*(cp-1)) )
					cp--;
				cx=atoi(cp);
				{	if (cx>0 && cx!=wi[win].aktmsg)
					{	select_msg(win,cx,TRUE,TRUE);
						need_update_msglistwindow(win,RETURN);
					}
				}
			}
		}
		else if (x>=wi[win].x && x<wi[win].x+wi[win].w &&
		         y>=wi[win].y && y<wi[win].y+wi[win].h)
		{	/* Click in MsgText */

			cy=(y-wi[win].y-gl_hbox+gl_hchar)/gl_hbox;

			if (state & 8)
				if (cy==FROM_LINE || cy==TO_LINE)
					if (is_netmail(wi[win].aktarea) ||
					    !stricmp(gl_area[wi[win].aktarea].name,"ExtraExpArea") ||
		  			  !stricmp(gl_area[wi[win].aktarea].name,"Bad_Msgs"))
					{	sprintf(tmp3,cy==FROM_LINE ? mergeaddr(wi[win].hdr->wFromZone,
																	 								 wi[win].hdr->wFromNet,
																									 wi[win].hdr->wFromNode,
																									 wi[win].hdr->wFromPoint,
																	 								 NULL)
																	 		 : mergeaddr(wi[win].hdr->wToZone,
																									 wi[win].hdr->wToNet,
				        																	 wi[win].hdr->wToNode,
				        																	 wi[win].hdr->wToPoint,NULL));
						if (cy==FROM_LINE)
							sprintf(tmp, msgtxt[DIV_FROM].text,wi[win].hdr->sFrom);
						else
							sprintf(tmp, msgtxt[DIV_TO].text,wi[win].hdr->sTo);

						i=FALSE;
						if (wi[win].hdr->wFromZone!=0 || wi[win].hdr->wFromNet !=0 ||
								wi[win].hdr->wFromNode!=0 || wi[win].hdr->wFromPoint!=0)
							if (cy==FROM_LINE)
								i=TRUE;
						if (wi[win].hdr->wToNet!=0)
							if (cy==TO_LINE)
								i=TRUE;

						if (i)
						{	sprintf(tmp2,msgtxt[DIV_ON].text,
												 cy==FROM_LINE ? mergeaddr(wi[win].hdr->wFromZone,
																	 								 wi[win].hdr->wFromNet,
																									 wi[win].hdr->wFromNode,
																									 wi[win].hdr->wFromPoint,
																	 								 NULL)
																	 		 : mergeaddr(wi[win].hdr->wToZone,
																									 wi[win].hdr->wToNet,
				        																	 wi[win].hdr->wToNode,
				        																	 wi[win].hdr->wToPoint,NULL));
							strcat(tmp,tmp2);
							cp=strrchr(tmp,' ');
							i=*++cp;
							*cp=EOS;
							vqt_extent(handle,tmp,ext);
							*cp=i;
							width=ext[2]-ext[0];
							hide_mouse();
							for (i=1; i<=6; i++)
							{	if (i & 1) vswr_mode(handle,MD_ERASE);
								else			 vswr_mode(handle,MD_REPLACE);
								v_gtext(handle, wi[win].x+width+4, wi[win].y+(cy+1)*gl_hbox,cp);
								evnt_timer(50,0);
							}
							show_mouse();
							goto show_fromaddr;
						}
					}

			cy=cy-TOPMSG_LINE+wi[win].topline;

			if (cy<wi[win].topline || cy>=wi[win].lzahl || wi[win].lines[cy]==NULL)
				return;

			cx = get_xpos(win,wi[win].lines[cy],x,wi[win].firstc);

			if (opt.texteffects & O_TEXTEFFSCR)
			{	effect_found = scan_effects(wi[win].lines[cy],tmp2);
			}
			else
			{	effect_found = FALSE;
				strcpy(tmp2,wi[win].lines[cy]);
			}

/*
** Es gibt einen Effect, wenn effect_found==TRUE
** In "tmp2" befindet sich der compilierte Text!
*/

			cp3=&tmp2[cx];
			if (effect_found)
			{	cp=tmp2;
				while(*cp)
				{	if (*cp>=TEON_U && *cp<=TEOFF_I)
					{	if (cp<=cp3)
							cp3++;
					}
					else
						cnt++;
					cp++;
				}
			}
			else
				cnt=(word)strlen(tmp2);

/*
** "cp3" zeigt auf Klickposition MIT Attributen!
*/
			if (cx<1 || cx>cnt)
				return;

			cp=cp3;
			if (state & 4)		/* CONTROL */
			{	while(isalnum(*cp)
						 	&& cp>=tmp2
							&& (*cp<TEON_U || *cp>TEOFF_I) )
					cp--;
			}
			else
			{	while((isalnum(*cp) || *cp=='.' || *cp=='-' || *cp=='_' ||
													  (state & 8) && (*cp=='/' || *cp==':') )
						 	&& cp>=tmp2
							&& (*cp<TEON_U || *cp>TEOFF_I) )
					cp--;
			}
			cp++;
			strcpy(tmp,cp);
			cp2=cp;					/* Position des Strings merken! */

			cp--;
#if 0
			while(cp>=tmp2 && *cp>=TEON_U && *cp<=TEOFF_I)
#endif
			while(cp>=tmp2 && *cp!=TEOFF_I)
			{	if (*cp==TEON_I)
				{	found_inverse=TRUE;
					break;
				}
				cp--;
			}

/*
** In "tmp" steht der Anfang des Worts OHNE Effekte!
*/

			cp=tmp;
			if (state & 4)		/* CONTROL */
			{	while(*cp
							&& isalnum(*cp)
							&& (*cp<TEON_U || *cp>TEOFF_I) )
					cp++;
			}
			else
			{	while(*cp
							&& (isalnum(*cp) || *cp=='.' || *cp=='-' || *cp=='_' ||
															 (state & 8) && (*cp=='/' || *cp==':') )
							&& (*cp<TEON_U || *cp>TEOFF_I) )
					cp++;
			}
			*cp=EOS;

/*
** In "tmp" steht NUR das Wort OHNE Attribute!
*/

			if (!*tmp)
				return;

			if (state & 8)
				if (strchr(tmp,':') && strchr(tmp,'/'))
					search4address=TRUE;

			if (!effect_found)
			{
				strcpy(tmp2,wi[win].lines[cy]);
				cp=cp2;

				if (cp)
				{	*cp=EOS;
					vqt_extent(handle,tmp2,ext);
					width=(wi[win].x-gl_wbox*wi[win].firstc) + (ext[2]-ext[0]);

					hide_mouse();
					for (i=1; i<=6; i++)
					{	if (i & 1) vswr_mode(handle,MD_ERASE);
						else			 vswr_mode(handle,MD_REPLACE);
						v_gtext(handle, width+4, wi[win].y+(cy+TOPMSG_LINE-wi[win].topline+1)*gl_hbox,tmp);
						evnt_timer(50,0);
					}
					show_mouse();
				}
			}
			else
			{
				strcpy(tmp2,wi[win].lines[cy]);
				cp=cp2;		/* hier steht das Wort im Text */

				if (cp)
				{	
					strcpy(tmp3,cp+strlen(tmp));
					*cp=EOS;

					hide_mouse();
					for (i=1; i<=6; i++)
					{	
						if (i & 1) sprintf(cp,"%c%s%c%s",!found_inverse?TEON_I:TEOFF_I,tmp,!found_inverse?TEOFF_I:TEON_I,tmp3);
						else			 sprintf(cp,"%s%s",tmp,tmp3);
						effect_v_gtext(win,wi[win].x-gl_wbox*wi[win].firstc+4,
															 wi[win].y+(cy+TOPMSG_LINE-wi[win].topline+1)*gl_hbox,
															 -1,
															 tmp2);
						evnt_timer(50,0);
					}
					if (found_inverse)
					{	
						RWHITE(wi[win].x-gl_wbox*wi[win].firstc+4,
									 wi[win].y+(cy+TOPMSG_LINE-wi[win].topline)*gl_hbox+3,
									 cnt*gl_wbox,gl_hbox-1);
						sprintf(cp,"%s%s",tmp,tmp3);
						effect_v_gtext(win,wi[win].x-gl_wbox*wi[win].firstc+4,
															 wi[win].y+(cy+TOPMSG_LINE-wi[win].topline+1)*gl_hbox,
															 -1,
															 tmp2);
					}
					show_mouse();
				}
			}

			cp=strchr(tmp,' ');
			while(cp)
			{	strcpy(cp,cp+1);
				cp=strchr(tmp,' ');
			}

			strcpy(tmp3,tmp);
			tmp[12]=EOS;
			tmp3[41]=EOS;

			if (state & 4)	/* CNTRL */
			{	
				show_shortcut(win,tmp3);
			}
			else if ((state & 8) && search4address)		/* ALTERNATE */
			{	
show_fromaddr:
				if (*gl_nodelistpath)
				{	uword zone,net,node,point;
					byte dum[MAX_STR];
					NLSYSTEM buf;
					scanaddr(tmp3,&zone,&net,&node,&point,NULL);
					if (search_bnl_usernode(zone,net,node,point,&buf,tmp))
					{	scan_bnl_flags(buf.modemtype,dum);
						sprintf(tmp2,msgtxt[MSG_ADDRESSINFO].text,
														tmp3,
														*tmp ? '@' : ' ',
														*tmp ? tmp : "",
														buf.operator,
														buf.sysname,
														buf.location,
														buf.flags & (1U << 8) ? "---Pvt---" : buf.phone,
														buf.flags & (1U << 1) ? " (CM)" : "",
														dum);
						cy=FormAlert(msgtxt[MSG_ADDRESSINFO].defbut,
												 msgtxt[MSG_ADDRESSINFO].icon,
												 tmp2,
												 msgtxt[MSG_ADDRESSINFO].button);
					}
					else
					{	point=0;
						if (search_bnl_usernode(zone,net,node,point,&buf,tmp))
						{	strcpy(tmp3,mergeaddr(zone,net,node,point,NULL));
							scan_bnl_flags(buf.modemtype,dum);
							sprintf(tmp2,msgtxt[MSG_ADDRESSINFO].text,
															tmp3,
															*tmp ? '@' : ' ',
															*tmp ? tmp : "",
															buf.operator,
															buf.sysname,
															buf.location,
															buf.flags & (1U << 8) ? "---Pvt---" : buf.phone,
															buf.flags & (1U << 1) ? " (CM)" : "",
															dum);
							cy=FormAlert(msgtxt[MSG_ADDRESSINFO].defbut,
													 msgtxt[MSG_ADDRESSINFO].icon,
													 tmp2,
													 msgtxt[MSG_ADDRESSINFO].button);
						}
						else
						{	sprintf(tmp2,msgtxt[MSG_ADDRESSNOTFOUND].text,tmp3);
							cy=FormAlert(msgtxt[MSG_ADDRESSNOTFOUND].defbut,
													 msgtxt[MSG_ADDRESSNOTFOUND].icon,
													 tmp2,
													 msgtxt[MSG_ADDRESSNOTFOUND].button);
						}
					}
				}
				else
				{	LangAlert(MSG_NONODELIST);
				}
			}
			else
			{	
				aufrufbar_machen("ST-GUIDE");

				if (msgfreqcount<8)
				{
					sprintf(tmp2,"%sST-GUIDE.APP",gl_ledpath);
					if (appl_find("ST-GUIDE")<0 && findfirst(tmp2,&dta,0))
					{	i=MSG_REQUESTREQ;
						sprintf(tmp2,msgtxt[MSG_REQUESTREQ].text,tmp);
						/* Cancel|Request|Hypertext */
					}
					else
					{	i=MSG_REQUESTREQHYP;
						sprintf(tmp2,msgtxt[MSG_REQUESTREQHYP].text,tmp,tmp3);
						/* Cancel|Request */
					}
					cy=FormAlert(msgtxt[i].defbut,msgtxt[i].icon,tmp2,msgtxt[i].button);
					if (cy==2)
					{	strcpy(msgfrequest[msgfreqcount],tmp);
						msgfreqcount++;
					}
					else if (cy==3)
						hypertext(HYPER_SEARCH,tmp3);
				}
				else
				{	
					sprintf(tmp2,"%sST-GUIDE.APP",gl_ledpath);
					if (appl_find("ST-GUIDE")<0 && findfirst(tmp2,&dta,0))
					{	i=MSG_REQUEST;
						sprintf(tmp2,msgtxt[MSG_REQUEST].text);
						/* Cancel|Hypertext */
					}
					else
					{	i=MSG_REQUESTHYP;
						sprintf(tmp2,msgtxt[MSG_REQUESTHYP].text,tmp3);
						/* Cancel */
					}
					cy=FormAlert(msgtxt[i].defbut,msgtxt[i].icon,tmp2,msgtxt[i].button);
					if (cy==2)
						hypertext(HYPER_SEARCH,tmp3);
				}
			}
		}
	}
}

#pragma warn +par
