#include "ccport.h"
#include "ledscan.h"
#include "menu.rh"
#include "allareas.rh"
#include "nodenum.rh"
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"
#include "vars.h"
#include "fido_msg.h"
#include "magx.h"
#include "language.h"

static word nextmsg_fl;

word nkeys[10]=
 {0x7030,0x6d31,0x6e32,0x6f33,0x6a34,0x6b35,0x6c36,0x6737,0x6838,0x6939};

static word mlist_wind[]=
 {FWCLIP,FSHELL,FEXEC,FQUIT,
  FPWRITE,
  AOPEN,ACHANGE,ACYCLE,ACLOSE,
  MLIST,MGOTO,MPRINT,
  /*EENTER,*/
  OSWITCH,OWIND,OQUOTE,OCOMMENT,OUSER,OFONT,OFUNC,OSAVE
  /*,OMHEADER,OMHEAD2*/,OFREQ,OUSERLST,OFIND,OFSAME,
  HMSG,HEDIT,HDIAL,HCOMMENT,HSHORT,HREPORT,-1};

static word alist_wind[]=
 {FWCLIP,FSHELL,FEXEC,FQUIT,
  FPWRITE,
  AOPEN,ACHANGE,ACYCLE,ACLOSE,
  MLIST,MPRINT,
  OSWITCH,OWIND,OQUOTE,OCOMMENT,OUSER,OFONT,OFUNC,OSAVE,
  OFREQ,OUSERLST,
  HMSG,HEDIT,HDIAL,HCOMMENT,HSHORT,HREPORT,-1};

static word msg_wind[]=
 {FWRITE,FAPPEND,FWCLIP,FBWRITE,FSHELL,FEXEC,FQUIT,
  FPWRITE,FPAPPEND,
  AOPEN,ACHANGE,ACYCLE,ACLOSE,
  MLIST,MKILL,MHEAL,MFORWARD,MGOTO,MMOVE,MPRINT,
  EENTER,EREPLY,EQUOTE,EQUONET,EQUOAREA,ECHANGE,
  OSWITCH,OWIND,OQUOTE,OCOMMENT,OUSER,OFONT,OFUNC,OSAVE,
  OMHEADER,OMHEAD2,OMHEAD3,OFREQ,OPRVTUSE,OUSERLST,OFIND,OFSAME,
  HMSG,HEDIT,HDIAL,HCOMMENT,HSHORT,HREPORT,-1};
  
static word edit_wind[]=
 {FREAD,FRCLIP,FBREAD,FQUIT,
 	FWRITE,FAPPEND,FWCLIP,
  AOPEN,ACYCLE,ACLOSE,
  EABORT,ESAVE,
  BBEGIN,BDELMARK,BCUT,BCOPY,BPASTE,BINDENT,BEXTEND,
  OSWITCH,OWIND,OQUOTE,OCOMMENT,OUSER,OFONT,OFUNC,OSAVE,
  OFREQ,OUSERLST,OFIND,OFSAME,
  HMSG,HEDIT,HDIAL,HCOMMENT,HSHORT,-1};
  
static word no_wind[]=
 {FSHELL,FEXEC,FQUIT,
  AOPEN,
  EENTER,
  OSWITCH,OWIND,OQUOTE,OCOMMENT,OUSER,OFONT,OFUNC,OSAVE,
  OFREQ,OUSERLST,
  HMSG,HEDIT,HDIAL,HCOMMENT,HSHORT,HREPORT,-1};

static word no_msg[]=
 {FSHELL,FEXEC,FQUIT,
  ACHANGE,AOPEN,ACYCLE,ACLOSE,
  EENTER,
  OSWITCH,OWIND,OQUOTE,OCOMMENT,OUSER,OFONT,OFUNC,OSAVE,
  OFREQ,OUSERLST,
  HMSG,HEDIT,HDIAL,HCOMMENT,HSHORT,HREPORT,-1};


void set_menudisabled(void)
{	word index;
	index=FREAD-1;
	do
	{	index++;
		if (menu_tree[index].ob_type==G_STRING)
			menu_tree[index].ob_state|=DISABLED;
	} while ( !(menu_tree[index].ob_flags & LASTOB) );
	menu_tree[FQUIT].ob_state&=~DISABLED;
}

void set_mmenu(word index[])
{	word i=0;
	while (index[i]!=-1)
	{	(menu_tree+index[i])->ob_state&=~DISABLED;
		i++;
	}
}

void set_menu(word art)
{	long win;
	set_menudisabled();
	switch (art)
	{	case (ALIST_WIND):
			set_mmenu(alist_wind);
			break;
		case (MLIST_WIND):
			set_mmenu(mlist_wind);
			break;
		case (MSG_WIND):
			set_mmenu(msg_wind);
			break;
		case (EDIT_WIND):
			set_mmenu(edit_wind);
			if ((win=found_wind())>=0)
			{	if (is_netmail(wi[win].aktarea))
					menu_ienable(menu_tree,ECC,TRUE);
			}	
			break;
		case (NO_WIND):
			set_mmenu(no_wind);
			break;
		case (NO_MSG):
		case (DEL_MSG):
			set_mmenu(no_msg);
			if (art==DEL_MSG)
			{	menu_ienable(menu_tree,MLIST,TRUE);
				menu_ienable(menu_tree,MHEAL,TRUE);
			}
			break;	
	}
	set_clipmenu(FALSE);
}	


void set_clipmenu(word set_fl)
{	static word disable_fl=FALSE;
	if (set_fl)
		disable_fl=TRUE;
	if (disable_fl)
	{	menu_ienable(menu_tree,FRCLIP,FALSE);
		menu_ienable(menu_tree,FWCLIP,FALSE);
	}
}


long new_editwindow(long win, word arindex)
{	long new_win;
	long i;
	HDR  *hp;

	new_win=-1;	
	for (i=0;i<MAX_WIND;i++)
	{	if (wi[i].belegt)
		{	if (wi[i].aktarea==arindex && wi[i].editierbar)
			{	top_it(i);
				/* wind_set(wi[i].handle,WF_TOP,0,0,0,0); */
				set_menu(EDIT_WIND);
				return (-1);
			}
		}
	}
	for (i=MAX_WIND-1;i>=0;i--)		/* Edit-Window wenn m”glich als letztes */
	{	if (wi[i].belegt==FALSE)
		{	new_win=i;
			break;
		}		
	}
	if (new_win<0L)
	{	LangAlert(MSG_NOWIND);
		return (-1);
	}
	memcpy(&wi[new_win],&wi[win],sizeof(WIND));
	hp=calloc(1,sizeof(HDR));
	if (hp!=NULL)
	{	memcpy(hp,wi[win].hdr,sizeof(HDR));
		wi[new_win].hdr=hp;	
	}
	else
	{	LangAlert(MSG_NOMEMNEWHEADER);
		return(-1);	
	}
	wi[new_win].ramhdr=FALSE;
	wi[new_win].hdrptr=wi[new_win].hdrbuf=NULL;
	wi[new_win].hdrlen=0L;

	open_window(new_win,wi[new_win].areaname,TRUE);	
	for (i=0;i<wi[new_win].lzahl;i++)
		wi[new_win].lines[i]=strdup(wi[win].lines[i]);

	wi[new_win].oldarea=wi[new_win].aktarea;
	wi[new_win].msgfp=wi[new_win].hdrfp=NULL;
	return (new_win);
}
	
word hndl_menu(word titel, word item)
{	byte *sh;
	word flag;
	long win, new_win;
	word ext_btn, gl_aai, i;
	word msgbuff[8];
	
	flag = FALSE;
	win=found_wind();
	
	if (!req_state(menu_tree,item,DISABLED))
	{
		switch (item)
		{
			case DABOUT:	dia_about();
										break;
			case FWRITE:	if (win>=0 && !wi[win].listwind)
											write_ascii(win,FWRITE,FALSE);
										break;
			case FAPPEND:	if (win>=0 && !wi[win].listwind)
											write_ascii(win,FAPPEND,FALSE);
										break;	
			case FPWRITE:	if (win>=0)
										{	if (!wi[win].listwind)
												write_ascii(win,FPWRITE,FALSE);
											else
												write_listascii(win,FPWRITE);
										}
										break;
			case FBWRITE:	if (win>=0 && !wi[win].listwind)
										{	write_binary(win,FALSE);
											need_update_msglistwindow(win,RETURN);
										}
										break;
			case FPAPPEND:if (win>=0 && !wi[win].listwind)
											write_ascii(win,FPAPPEND,FALSE);
										break;	
			case FWCLIP:	if (win>=0)
										{	if (!wi[win].listwind)
												write_ascii(win,FWCLIP,FALSE);
											else
												write_listascii(win,FWCLIP);
										}
										break;
			case FSHELL:	sh=getenv("SHELL");
										if (sh)
											execute(sh, NULL,TRUE);
										break;	
			case FEXEC:		execute(NULL,NULL,FALSE);
										break;
			case FQUIT:		show_mouse();
										flag=quit();
										break;

			case AOPEN:		if (opt.listawindow)
											openarea_listwindow(-1,-1,FALSE);
										else	
							        wiopen();
										break;
			case ACHANGE:	if (win>=0)
										{	gl_oldwin=win;				/* Diese Area „ndern */
											if (opt.listawindow)
												openarea_listwindow(win,wi[win].aktarea,TRUE);
											else	
												area_change(win,TRUE,TRUE);
										}
										else
										{	
											if (opt.listawindow)
												openarea_listwindow(-1,-1,TRUE);
											else
												wiopen();
										}
										break;
			case ACYCLE:	wicycle();
										break;
			case ACLOSE:	if (win>=0)		/* ssl 131294 */
										{	msgbuff[0]=WM_CLOSED;
											msgbuff[3]=wi[win].handle;
											flag=hndl_wind(msgbuff);
										}
										break;			               
			case MLIST:		if (win>=0)
										{	
											if (wi[win].listwind)
											{	if (wi[MSGSWIND].belegt)
													top_it(MSGSWIND);
											}
											else
											{	if (opt.listmwindow)
													msg_listwindow(win);
												else
													msg_list(win);
											}
										}	
										break;          
			case MKILL:		if (win>=0 && !wi[win].listwind)
											msg_kill(win,FALSE,TRUE);
										break;
			case MHEAL:		if (win>=0 && !wi[win].listwind)
											msg_kill(win,TRUE,TRUE);
										break;
			case MMOVE:		if (win>=0 && !wi[win].listwind)
											move_msg(win);
										break;
			case MFORWARD:if (win>=0 && !wi[win].listwind)
											msg_forward(win);
										break;
			case MGOTO:		if (win>=0 &&
										(!wi[win].listwind || wi[win].listwind==WI_LIST_MSGS))
											msg_goto(win,0);
										break;
			case MPRINT:	if (win>=0)
											print_msg(win,FALSE);
										break;		

			case EENTER:	/* Enter a new msg */
										for (i=0;i<MAX_WIND;i++)
										{	if (wi[i].belegt && !wi[i].editierbar && !wi[i].listwind)
											{	prep_topline(win,titel);
												break;
											}
										}
										if (i>=MAX_WIND)
										{	win=get_freewind(); /* ssl: 151294: war bisher =0 */
											if (lade_wind(win))
											{	set_menu(EDIT_WIND);
												menu_tnormal(menu_tree,titel,TRUE);
											}
											else
												win=-1;
										}
						
										if (win>=0 && !wi[win].listwind)
										{	i=2;
											if (gl_area[wi[win].aktarea].flag & AR_NOEXPORT)
												i=LangAlert(MSG_NOEXPORT);
											
											if (i==2)
											{	if ((new_win=new_editwindow(win,wi[win].aktarea))>=0L)
												{	*(wi[new_win].dup_replyid)=
													*(wi[new_win].dup_replyto)=
													*(wi[new_win].dup_replyaddr)=EOS;
													edit_msg(new_win,TRUE);
												}
											}
										}
										break;

			case EREPLY:	/* Reply on a msg */
										if (win>=0 && !wi[win].listwind)
										{	gl_aai=wi[win].aktarea;

											if (gl_area[gl_aai].flag & AR_FOLLOWUP)		/* 280496 */
												gl_aai=gl_area[gl_aai].followupnr;

											i=2;
											if (gl_area[gl_aai].flag & AR_NOEXPORT)
												i=LangAlert(MSG_NOEXPORT);
											if (i==2)
											{	prep_topline(win,titel);
												if ((new_win=new_editwindow(win,gl_aai))>=0L)
													reply_msg(new_win,gl_aai);
											}
										}
										break;

			case EQUOTE:	/* Standard reply with quote */
										if (win>=0 && !wi[win].listwind)
										{	if (gl_area[wi[win].aktarea].flag & AR_FOLLOWUP)
											{	gl_aai=gl_area[wi[win].aktarea].followupnr;
												goto standard_quote;
											}
						
											i=2;
											if (gl_area[wi[win].aktarea].flag & AR_NOEXPORT)
												i=LangAlert(MSG_NOEXPORT);
											if (i==2)
											{	prep_topline(win,titel);
												if ((new_win=new_editwindow(win,wi[win].aktarea))>=0L)
													quote_msg(new_win);
											}
										}
										break;		

			case EQUONET:	/* Quote to Netmail */
										if (win>=0 && !wi[win].listwind)
										{	prep_topline(win,titel);
											if ((new_win=new_editwindow(win,0))>=0L)
												quotearea_msg(new_win,0);
										}
										break;		

			case EQUOAREA:	/* Quote to other Echomail-Area */
										if (win>=0 && !wi[win].listwind)
										{	set_flag(allarea_tree,ATITEL,HIDETREE,FALSE);
						
											/* alte Area in old_aai */
											gl_aai=wi[win].aktarea;
											
											del_mflag(allarea_tree,SELECTABLE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
											set_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
											ext_btn=do_aform(&gl_aai,TRUE);
											set_mflag(allarea_tree,SELECTABLE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
											del_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
								
											/* Nach Areawahl */
											del_state(allarea_tree,ext_btn,SELECTED,FALSE);
											del_dialog(&allarea_dial);
								
											if (ext_btn!=AMCAN)
											{	
standard_quote:
												i=2;
												if (!is_netmail(gl_aai) &&
														gl_area[gl_aai].flag & AR_NOEXPORT)
													i=LangAlert(MSG_NOEXPORT);
												if (i==2)
												{	prep_topline(win,titel);
													if ((new_win=new_editwindow(win,gl_aai))>=0L)
														quotearea_msg(new_win,gl_aai);
												}
											}
										}
										break;		

			case ECHANGE:	/* Change msg */
										if (win>=0 && !wi[win].listwind && wi[win].lastmsg>0)
										{	prep_topline(win,titel);
											if ((new_win=new_editwindow(win,wi[win].aktarea))>=0L)
												change_msg(new_win);
										}
										break;		

			case OSWITCH:	/* Many option switches */
										dia_optswitches();
										break;
			case OWIND:		/* Window size options */
										dia_optwindow();
										break;
			case OCOMMENT:dia_optcomment();
										break;
			case OQUOTE:	/* Quote style options */
										dia_optquote();
										break;
			case OUSER:		/* Userlist options */
										dia_optuser();
										break;
			case OFONT:		/* Font change dialog */
										dia_optfont();
										break;	
			case OFUNC:   /* Function key setting */
										dia_fkeys();
										break;
			case OSAVE:		/* Save options */
										save_options();
										break;	
			case OFREQ:   /* Make file-request file */
										dia_freq(win);
										break;
			case OMHEADER:/* Show/Edit msg-header */
										if (win>=0 && !wi[win].listwind)
										{	dia_header(win);
											need_update_msglistwindow(win,RETURN);
										}
										break;
			case OMHEAD2: /* Show/Edit msg-header flags */
										if (win>=0 && !wi[win].listwind)
										{	dia_header2(win);
											need_update_msglistwindow(win,RETURN);
										}
										break;
			case OMHEAD3: /* Show/Edit special msg-header flags */
										if (win>=0 && !wi[win].listwind)
										{	dia_header3(win);
											need_update_msglistwindow(win,RETURN);
										}
										break;
			case OPRVTUSE:/* Add a name to private-userlist */
										if (win>=0)
											save_user(win);
										break;
			case OUSERLST:/* Find a name in private-userlist */
										init_userdia("", addr[0].zone, addr[0].net,
																		 addr[0].node, addr[0].point);	/* ssl 181294: war vorher "\0" */
										userbox(2,TRUE,NNTO,NULL,NULL,NULL,NULL,NULL);
										break;
			case OFIND:   /* Find a string */
										if (win>=0 &&
												(!wi[win].listwind || wi[win].listwind==WI_LIST_MSGS))
						        	dia_find(win);
							      break;
			case OFSAME: 	/* Find same */
										if (win>=0 &&
												(!wi[win].listwind || wi[win].listwind==WI_LIST_MSGS))
											find_same(win,TRUE);
										break;
			case HMSG:		/* Help for msg-window */
										helping(hmsg_tree,&hmsg_dial);
										break;
			case HEDIT:		/* Help for edit-window */
										helping(hedit_tree,&hedit_dial);
										break;
			case HDIAL:		/* Help for dialogs */
										helping(hdial_tree,&hdial_dial);
										break;
			case HCOMMENT:/* Help for dialogs */
										helping(helpcom_tree,&helpcom_dial);
										break;
			case HSHORT:	show_shortcut(win,NULL);
										break;
			case HREPORT:	write_report();
										break;
		}
	}
	win=found_wind();				/* ssl 280295 */
	if (win>=0)
	{	
		if (wi[win].listwind==WI_LIST_AREAS)
			set_menu(ALIST_WIND);
		else if (wi[win].listwind==WI_LIST_MSGS)
			set_menu(MLIST_WIND);
		else if (wi[win].lastmsg==0)
			set_menu(NO_MSG);
		else if (wi[win].aktmsg==0)
			set_menu(DEL_MSG);
		else if (wi[win].editierbar)
			set_menu(EDIT_WIND);	
		else		
			set_menu(MSG_WIND);	
	}
	menu_tnormal(menu_tree,titel,TRUE);
	return(flag);
}

word num_key(word key)
{	word i;
	if (key > 47 && key < 58)
		return(TRUE);

	if (!opt.numpadcomment)
		for (i=0;i<10;i++)
			if (key==nkeys[i])
				return(TRUE);
	return(FALSE);
}				

void throw_evntaway(void)
{	int ev;
	int msgbuff[8];
	int dummy;
	for (;;)
	{	ev=evnt_multi( MU_KEYBD | MU_BUTTON | MU_TIMER,
		               1, 1, 1,
		               0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		               msgbuff, 1, 0, &dummy, &dummy, &dummy, &dummy,
		               &dummy, &dummy);

		if (ev & MU_TIMER)
			return;
	}
}

word search_unreadmsg(long win, word startnr, word forward_fl)
{	HDR	 thdr;
	word msgnr;

	msgnr=startnr;
	while (msgnr<=wi[win].lastmsg && msgnr>=wi[win].firstmsg)
	{
		hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
		hdrread(win,&thdr, 1, wi[win].hdrfp);
		if (!(thdr.XFlags & XF_READ) && !(thdr.wFlags & F_DELETED))
			return msgnr;
		if (forward_fl)
			msgnr++;
		else
			msgnr--;	
	}
	return (-1);
}

void modify_flags(long win,word key)
{	if (wi[win].aktmsg>0)
	{	switch(key)
		{	case	KEY_X	:	wi[win].hdr->XFlags ^= XF_LOCK; break;
		}
		hdrseek(win,wi[win].hdrfp,wi[win].aktmsg-1,SEEK_SET);
		hdrwrite(win,wi[win].hdr,1,wi[win].hdrfp);
#if 0
		wi[win].oldmsg=0;
		select_msg(win,wi[win].aktmsg,TRUE,TRUE);
#endif
#if 1
		wisetfiles(win,TRUE,NO_CHANGE);
#endif
		need_update_msglistwindow(win,RETURN);
	}
}

word hndl_keybd(long win,word key)
{	
	word i, t, ret;
 	word gef, start, count;
	word msgbuff[8];
 	word msgnr=0;
	byte c;
	byte hdrpath[MAX_STR];
	byte tmp[MAX_STR],*cp;
	struct ffblk dta;
	static readnext = TRUE;
	HDR hdr;

	if (win>=0 && key==SHIFT_CNTRL_L)
	{	msg_list(win);
		need_update_msglistwindow(win,RETURN);
		return(FALSE);
	}


	if ((uword)key==CNTRL_F4)
		key=select_function(0);


	ret=FALSE;
	for (i=0; menu_keys[i] != -1; i+=3)
	{	if (menu_keys[i+2] == key)
		{	menu_tnormal(menu_tree,menu_keys[i],FALSE);
			return(hndl_menu(menu_keys[i],menu_keys[i+1]));
		}
	}
	c=(byte)key;
	if (c > 47 && c < 58 && num_key(key)) 
	/* != SHFT_CD && key != SHFT_CU && key != SHFT_CL && key != SHFT_CR)*/
	{	if (win>=0)
		{	msg_goto(win,c-48);
			return(ret);
		}
	}

	if (win>=0)
	{	hide_mouse();
		set_wiclip(win);
		set_wifont(win);
		show_mouse();
	}
	switch(key)
	{	
		case KEY_W:			if (win>=0)
										{	write_binary(win,TRUE);
											need_update_msglistwindow(win,RETURN);
										}
										break;

		case HELPKEY:		hypertext(HYPER_LED,NULL);
										break;							
		case SHFT_HELPKEY:
										hypertext(HYPER_LED_KEYS,NULL);
										break;							

		case UNDO:			redraw_all();
										break;							

		case CUR_DOWN:	if (win>=0)
										{	msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
											msgbuff[4]=3;
											ret=hndl_wind(msgbuff);
											throw_evntaway();
										}
										break;	

		case CUR_UP:		if (win>=0)
										{	msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
											msgbuff[4]=2;
											ret=hndl_wind(msgbuff);
											throw_evntaway();
										}
										break;	

		case SHFT_CD:
		case SPACE:			if (win>=0)
										{	nextmsg_fl=FALSE;
											msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
										 	msgbuff[4]=1;
									 		ret=hndl_wind(msgbuff);
										 	if ( !(key==SPACE && nextmsg_fl) )
												break;
										} /* bitte hier kein break einfgen, danke! */
		case RETURN:
		case ENTER:			if (win>=0)
										{	ret=FALSE;
											if (readnext)
												select_msg(win,wi[win].aktmsg+1,TRUE,TRUE);
											else
												select_msg(win,wi[win].aktmsg-1,TRUE,TRUE);
											need_update_msglistwindow(win,RETURN);
										}
										break;

		case ALT_RETURN:if (win>=0)
										{	del_rek_subject(win);
											throw_evntaway();
										}
										break;

		case CNTRL_RET:	if (win>=0 && wi[win].aktmsg>0)
										{	ret=FALSE;
											need_update_msglistwindow(win,RETURN);
											msg_kill(win,FALSE,FALSE);
										}
										break;

		case SHFT_CU:
		case BACKSPACE:	/* Display previous page */
										if (win>=0)
										{	msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
											msgbuff[4]=0;
											ret=hndl_wind(msgbuff);
										}
										break;	

		case CUR_LEFT:	/* Display previous msg */
										if (win>=0)
										{	ret=FALSE;
											select_msg(win,wi[win].aktmsg-1,TRUE,TRUE);
											throw_evntaway();
											readnext=FALSE;
											need_update_msglistwindow(win,RETURN);
										}
										break;	

		case CUR_RIGHT:	/* Display next msg */
										if (win>=0)
										{	ret=FALSE;
											select_msg(win,wi[win].aktmsg+1,TRUE,TRUE);
											throw_evntaway();
											readnext=TRUE;
											need_update_msglistwindow(win,RETURN);
										}
										break;	

		case ZEHN_1:
		case KEY_p:			if (win>=0)
										{	ret=FALSE;
											msgnr=search_unreadmsg(win,wi[win].aktmsg-1,FALSE);
											if (msgnr>0)
											{	select_msg(win,msgnr,TRUE,TRUE);
												need_update_msglistwindow(win,RETURN);
											}
											throw_evntaway();
											readnext=FALSE;
										}
										break;	

		case ZEHN_3:
		case KEY_n:			if (win>=0)
										{	ret=FALSE;
											msgnr=search_unreadmsg(win,wi[win].aktmsg+1,TRUE);
											if (msgnr>0)
											{	select_msg(win,msgnr,TRUE,TRUE);
												need_update_msglistwindow(win,RETURN);
											}
											throw_evntaway();
											readnext=TRUE;
										}
										break;
		case KEY_X:			if (win>=0)
											modify_flags(win,key);
										break;
		case KEY_T:			if (win>=0)
										{	gl_otherquotefl=TRUE;
											ret=hndl_menu(EDIT,EQUOTE);
										}
										break;
		case KEY_J:			if (win>=0)
										{	gl_otherquotefl=TRUE;
											ret=hndl_menu(EDIT,EQUOAREA);
										}
										break;
		case KEY_N:			if (win>=0)
										{	gl_otherquotefl=TRUE;
											ret=hndl_menu(EDIT,EQUONET);
										}
										break;

		case PLUS:			/* First comment on aktive msg */
		case ZEHN_2:		if (win>=0)
										{	comment(win,FIRST);
											need_update_msglistwindow(win,RETURN);
										}
										break;
		case MINUS:			/* Origin msg */
		case ZEHN_8:		if (win>=0)
										{	comment(win,ORIGINAL);
											need_update_msglistwindow(win,RETURN);
										}
										break;
		case SHFT_GROESSER:	/* next comment */
		case ZEHN_6:		if (win>=0)
										{	comment(win,NEXT);
											need_update_msglistwindow(win,RETURN);
										}
										break;
		case GROESSER:	/* previous comment */
		case ZEHN_4:		if (win>=0)
										{	comment(win,PREV);
											need_update_msglistwindow(win,RETURN);
										}
										break;	
		case ZEHN_5:		/* Start of thread */
										if (win>=0)
										{	comment_start(win);
											need_update_msglistwindow(win,RETURN);
										}
										break;

		case ZEHN_OPKL:	if (win>=0)
										{	prev_subj(win,NULL);
											need_update_msglistwindow(win,RETURN);
										}
										break;
		case ZEHN_CLKL:	if (win>=0)
										{	next_subj(win,NULL);
											need_update_msglistwindow(win,RETURN);
										}
										break;
		case ZEHN_CT_OPKL:
										if (win>=0)
										{	prev_subj(win,wi[win].hdr->sTo);
											need_update_msglistwindow(win,RETURN);
										}
										break;
		case ZEHN_CT_CLKL:
										if (win>=0)
										{	next_subj(win,wi[win].hdr->sFrom);
											need_update_msglistwindow(win,RETURN);
										}
										break;	

		case SHFT_CL:		if (win>=0)
										{	msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
										 	msgbuff[4]=4;
										 	ret=hndl_wind(msgbuff);
										}
										break;
		case SHFT_CR:		if (win>=0)
										{	msgbuff[0]=WM_ARROWED;
											msgbuff[3]=wi[win].handle;
											msgbuff[4]=5;
											ret=hndl_wind(msgbuff);
										}
										break;	

	  case ALT_1:			if (win>=0)
								   	{	opt.umlaut ^= O_MIMESTYLE;
											if (opt.umlaut & O_MIMESTYLE)
												if (!available(msgtxt[DO_MIMECONVERTION].text))
													opt.umlaut &= ~O_MIMESTYLE;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;
		case ALT_2:			if (win>=0)
								   	{	opt.texteffects ^= O_TEXTEFFSCR;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;	
	  case ALT_3:			if (win>=0)
								   	{	opt.umlaut ^= O_ISOCONV;
											if (opt.umlaut & O_ISOCONV)
												if (!available(msgtxt[DO_CHRRECOGNITION].text))
													opt.umlaut &= ~O_ISOCONV;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;
   	case ALT_4:			if (win>=0)
								   	{	opt.hideeid = !opt.hideeid;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;	
	  case ALT_5:			if (win>=0)
								   	{	opt.hidepid = !opt.hidepid;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;	
	  case ALT_6:			if (win>=0)
								   	{	opt.hideca = !opt.hideca;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;
	  case ALT_7:			if (win>=0)
								   	{	opt.hideseen = !opt.hideseen;
								   		wi[win].oldmsg=0;
											select_msg(win,wi[win].aktmsg,TRUE,TRUE);
										}
								   	break;
    case ALT_8:			if (win>=0)
							    	{	opt.showcom = !opt.showcom;
							 	  		set_comstatus(win, wi[win].aktmsg);
											chk_status(win,TRUE);
										}
								   	break;	
		case ALT_9:			if (win>=0)
										{	set_pointsize(win,9);
											ret=FALSE;
										}
							     	break;
		case ALT_0:			if (win>=0)
										{	set_pointsize(win,10);
											ret=FALSE;
										}
							     	break;

		case ZEHN_PLUS:	if (win>=0)
							     	{	i=wi[win].aktarea;
											if (i+1<gl_arzahl)
												i++;
											else
												i=0;
											if (!srch_winarea(win,i))
												change_area(win,i);
											else
												update_areawindow(i /*wi[win].aktarea*/);
											no_areawind_successiv();
							     	}
							     	break;
    case ZEHN_MINUS:if (win>=0)
							     	{	i=wi[win].aktarea;
											if (i>0)
												i--;
											else
												i=gl_arzahl-1;
											if (!srch_winarea(win,i))
												change_area(win,i);
											else
												update_areawindow(i /*wi[win].aktarea*/);
											no_areawind_successiv();
										}
										break;	

		case ZEHN_STAR:		/* next area with new msg */
										t=NEWMSG;
										goto sprung;
		case ZEHN_SLASH:	/* next area with unread msg */
										t=UNRDMSG;
										goto sprung;
		case DOPPKREUZ:		/* next area with msg to sysop */
										t=TOSYSOP;
sprung:
										if (win>=0)
										{	gef=FALSE;
											start=wi[win].aktarea;
											if (start>=gl_arzahl-1)
												i=start=0;
											else	
												i=start+1;
											for (count=0;count<gl_arzahl;count++)
											{	if (gl_area[i].newmsg & t)
												{	gef=TRUE;
													break;
												}
												if (i>=gl_arzahl-1)	i=0;
												else								i++;	
											}	
											if (gef)
												if (!srch_winarea(win,i))
													change_area(win,i);
											no_areawind_successiv();
										}
										break;
		case TAB:				if (win>=0)
										{	cp=get_successiv_areaname();
											if (!*cp)
												break;
											if (wi[AREAWIND].belegt)
												count=wi[AREAWIND].selline+1;
											else
												count=wi[win].aktarea+1;
											if (count>=gl_arzahl)
												count=0;
											for (i=count;i<gl_arzahl;i++)
											{	if (!strnicmp(gl_area[i].name,cp,strlen(cp)))
													break;
											}
											if (i>=gl_arzahl)
											{	for (i=0;i<gl_arzahl;i++)
												{	if (!strnicmp(gl_area[i].name,cp,strlen(cp)))
														break;
												}
											}
											if (i<gl_arzahl)
												if (!srch_winarea(win,i))
													change_area(win,i);
										}
										break;
		case HOME:	    if (win>=0 && wi[win].topline!=0)
										{	wi[win].topline=0;
											wisetfiles(win,FALSE,NO_CHANGE);
										}
										break;
		case SHFT_HOME:	if (win>=0)
										{	if (wi[win].topline<wi[win].lzahl-wi[win].maxshow)
											{	wi[win].topline=wi[win].lzahl-wi[win].maxshow;
												wisetfiles(win,FALSE,NO_CHANGE);
											}
										}	
										break;

		case CNTRL_F1:	/* => Shortcut, Ausgewertet in MAIN <= */
										/* Hier nur wegen function dialog			 */
										if (win>=0)
											show_shortcut(win,NULL);
										break;	

		case CNTRL_F2:	if (win>=0 && !wi[win].listwind && !wi[win].editierbar)
											msginfo(win);
										break;
		case CNTRL_F3:	if (win>=0 && wi[win].aktmsg>0)
										{	HDR hdr;
											t=LangAlert(MSG_DELALLMSG);
											if (t==2)
											{	biene();
												for (i=wi[win].firstmsg;i<=wi[win].lastmsg;i++)
												{	hdrseek(win,wi[win].hdrfp,i-1,SEEK_SET);
													hdrread(win,&hdr,1,wi[win].hdrfp);
													if (!(hdr.wFlags & F_DELETED))
													{	hdr.wFlags |= F_DELETED;
														wi[win].msgda[i]=FALSE;
														hdrseek(win,wi[win].hdrfp,i-1,SEEK_SET);
														hdrwrite(win,&hdr,1,wi[win].hdrfp);
													}
												}
												wi[win].aktmsg=i-1;
												wi[win].oldmsg=0;
												mouse_normal();
												change_area(win, wi[win].aktarea);
											}
										}
										break;

		case CNTRL_F5:	if (select_sysop(tmp))
											strcpy(gl_ptowner,tmp);
										break;
		case CNTRL_F6:	
										if (win>=0)
											if (LangAlert(MSG_SORTHEADER)==2)
												sort_header(win);
										break;
		case CNTRL_F7:	if (win>=0)
											print_msg(win,TRUE);
										break;	
		case CNTRL_F8:	if (win>=0)
										{	t=LangAlert(MSG_LASTREAD);
											if (t==2 || t==3)
											{	biene();
												show_doing(DO_CALCMAXIMUM);
												i=wi[win].aktarea;
												sprintf(hdrpath,"%s.%s",gl_area[i].path, HEADEREXT);
												if (!findfirst(hdrpath,&dta,0))
												{
													gl_area[i].lread=(word)(dta.ff_fsize/sizeof(HDR));
													gl_area[i].newmsg &= ~(UNRDMSG|TOSYSOP);
												}	
												if (t==3)
													for (t=1; t<=wi[win].lastmsg; t++)
													{	wi[win].aktmsg=t;
														need_update_msglistwindow(win,RETURN);
														hdrseek(win, wi[win].hdrfp, (t-1), SEEK_SET);
														hdrread(win, &hdr, 1, wi[win].hdrfp);
														if (!(hdr.XFlags & XF_READ))
														{	
															if (is_ptowner(hdr.sTo,i) ||
															    !stricmp(hdr.sTo,"sysop"))
															{	wi[win].aktmsg=0;
																select_msg(win,t,TRUE,FALSE);
																if (LangAlert(MSG_UNREADMAIL)==1)
																	continue;
															}
															hdr.XFlags |= XF_READ;
															hdrseek(win, wi[win].hdrfp, (t-1), SEEK_SET);
															hdrwrite(win, &hdr, 1, wi[win].hdrfp);
														}
													}

												del_doing();
												mouse_normal();
												wi[win].oldmsg=0;
												select_msg(win,wi[win].lastmsg,TRUE,TRUE);
												update_areawindow(i);
												wi[MSGSWIND].aktarea=-1;
												need_update_msglistwindow(win,RETURN);
											}
										}
										break;
		case CNTRL_F9:	t=LangAlert(MSG_ALLLASTREADS);
										if (t==2 || t==3)
										{	biene();
											show_doing(DO_CALCMAXIMUM);
											for (i=0;i<gl_arzahl;i++)
											{	
												sprintf(hdrpath,"%s.%s",gl_area[i].path, HEADEREXT);
												if (!findfirst(hdrpath,&dta,0))
												{
													gl_area[i].lread=(word)(dta.ff_fsize/sizeof(HDR));
													gl_area[i].newmsg &= ~(UNRDMSG|TOSYSOP);
													if (t==3)
														gl_area[i].newmsg &= ~NEWMSG;
												}
											}	
											del_doing();
											mouse_normal();
											for (i=0; i<MAX_WIND; i++)
												if (wi[i].belegt && i!=win)
													select_msg(i,wi[i].lastmsg,TRUE,TRUE);
											if (win>=0)
												select_msg(win,wi[win].lastmsg,TRUE,TRUE);
											if (wi[AREAWIND].belegt)
											{	for (i=0;i<wi[AREAWIND].lzahl;i++)
													update_arealistwindow(AREAWIND,i);
												wisetfiles(AREAWIND,FALSE,NO_CHANGE);
											}
											wi[MSGSWIND].aktarea=-1;
											if (win>=0)
												need_update_msglistwindow(win,RETURN);
										}
										break;
		case CNTRL_F10:	if (LangAlert(MSG_DELFLAGS)==2)
										{	biene();
											for (i=0;i<gl_arzahl;i++)
												gl_area[i].newmsg &= ~(UNRDMSG|TOSYSOP);
											if (wi[AREAWIND].belegt)
											{	for (i=0;i<gl_arzahl;i++)
													update_arealistwindow(AREAWIND,i);
												wisetfiles(AREAWIND,FALSE,NO_CHANGE);
											}
											mouse_normal();
										}
										break;

		case ALT_F1:
		case ALT_F2:
		case ALT_F3:
		case ALT_F4:
		case ALT_F5:
		case ALT_F6:
		case ALT_F7:
		case ALT_F8:
		case ALT_F9:
		case ALT_F10:		i=(key>>8);
										i-=(ALT_F1>>8);
										if (gl_shell[i]!=NULL)
										{
											strcpy(tmp,gl_shell[i]);
											strupr(tmp);
											if (strstr(tmp,"LED.PRG")==NULL)
											{
												if (strchr(gl_shell[i],'\\')==NULL)
													sprintf(tmp,"%s%s",gl_ledpath,gl_shell[i]);
												else
													strcpy(tmp,gl_shell[i]);
												if (!findfirst(tmp,&dta,0))
													execute(tmp,gl_shellcmd[i],FALSE);
											}
										}
										break;
		case ESC:				if (win>=0)
											change_area(win, wi[win].aktarea);
										break;
		case F1:
#if 0
										{	struct { long t,w; } buf[20];
											appl_trecord(&buf[0],20);
											getch();
										} break;
#endif
										fkey_pressed(win,0);	break;
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
	}
	return(ret);
}

word hndl_wind(word msgbuff[8])
{	WIND *winp;
	long win,winhelp;
	word verlassen,mtitle,red,hspmax;
	word oldw,oldh,i;
	word pxy[4];
	long topwin;
	red=TRUE;
	verlassen = FALSE;

	if (msgbuff[0]==FIDO_MSG)
	{	if (msgbuff[6]==LED_INFO)
		{	word xmsgbuff[8];
			xmsgbuff[0] = FIDO_INFO;
			xmsgbuff[1] = gl_apid;
			xmsgbuff[2] = 0;
			*((long *) &xmsgbuff[3]) = FM_LED;
			xmsgbuff[5] = hexversion;
			xmsgbuff[6] = 0x00;
			xmsgbuff[7] = 0;
			appl_write(msgbuff[1],16,xmsgbuff);
			return(FALSE);
		}
	}

	mtitle=title_select();
	if (!mtitle)
		hide_mouse();

	if (msgbuff[0]!=MN_SELECTED)
	{	for (win=0;win<TOTAL_WIND;win++)
		{	if (msgbuff[3] == wi[win].handle)
				break;
		}
		if (win==TOTAL_WIND)
		{	if (!mtitle)
				show_mouse();
			return (verlassen);
		}
	}

	winp=&wi[win];
	switch(msgbuff[0])
	{
		case WM_ICONIFY:	winp->xb=winp->x;	winp->yb=winp->y;
											winp->wb=winp->w;	winp->hb=winp->h;
											wind_set(msgbuff[3],WF_ICONIFY,msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
											winp->iconify=1;
											break;
		case WM_ALLICONIFY:
											for (i=0;i<TOTAL_WIND;i++)
											{	if (wi[i].belegt && !wi[i].iconify)
												{	wi[i].xb=wi[i].x;	wi[i].yb=wi[i].y;
													wi[i].wb=wi[i].w;	wi[i].hb=wi[i].h;
													wi[i].iconify=1;
													wind_close(wi[i].handle);
													wind_set(wi[i].handle,WF_ICONIFY,-1,-1,-1,-1);
													wind_open(wi[i].handle,-1,-1,-1,-1);
												}
											}
											break;
		case WM_UNICONIFY:if (msgbuff[4]<=0 && msgbuff[5]<=0 &&
													msgbuff[6]<=0 && msgbuff[7]<=0)
												wind_calc(WC_BORDER,winp->gadgets,
														winp->xb,winp->yb,winp->wb,winp->hb,
														&msgbuff[4],&msgbuff[5],&msgbuff[6],&msgbuff[7]);
											wind_set(msgbuff[3],WF_UNICONIFY,msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
											winp->iconify=0;
											break;

		case MN_SELECTED:	topwin=found_wind();
											if (topwin>=0 && edit_window(topwin))
												verlassen |= edmenu_event(msgbuff[3],msgbuff[4]);
											else
												verlassen |= hndl_menu(msgbuff[3],msgbuff[4]);
											break;     			

		case WM_REDRAW:		set_wiclip(win);
											set_wifont(win);
											if (!winp->iconify && winp->editierbar)
											{	hide_mouse();
												cursor(win,FALSE);
											}
											wind_update(BEG_UPDATE);
											wind_get(winp->handle,WF_FIRSTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
											while (pxy[2] && pxy[3])
											{
								     		if (rc_intersect(msgbuff+4,pxy))
								     		{	pxy[2] += pxy[0]-1;
											    pxy[3] += pxy[1]-1;
											    vs_clip(handle,TRUE,pxy);

													if (!winp->iconify)
														sys_wisetfiles(win,TRUE);
													else
														redraw_iconify(win);
											  }
											  wind_get(winp->handle,WF_NEXTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
											}
											wind_update(END_UPDATE);
											set_wiclip(win);
							/*			set_wifont(win); */
											if (!winp->iconify && winp->editierbar)
											{	/* in sys_wisetfiles wird auch show_mouse gemacht */
												hide_mouse();
												cursor(win,TRUE);
												show_mouse();
											}
											set_wiclip(win);
											set_wifont(win);
											break;

		case WM_ONTOP:
											{	MAGX_COOKIE *MagX;
												ulong value;
												int scr_id,cur_id;
												MagX = getcookie( 'MagX', &value ) ? (MAGX_COOKIE *) value : NULL;
												if (MagX && MagX->aesvars && MagX->aesvars->version>=0x200)
												{	wind_update(BEG_UPDATE);
													scr_id=appl_find("SCRENMGR");
													cur_id=menu_bar(0x0l,-1);
													if (cur_id!=gl_apid && scr_id>=0)
													{	word xmsgbuff[8];
														xmsgbuff[0]=101;
														xmsgbuff[1]=gl_apid;
														xmsgbuff[2]=
														xmsgbuff[3]=0;
														xmsgbuff[4]='MA';
														xmsgbuff[5]='GX';
														xmsgbuff[6]=2;
														xmsgbuff[7]=gl_apid;
														appl_write(scr_id,16,xmsgbuff);
													}
													wind_update(END_UPDATE);
												}
											}
											/* Kein BREAK */
		case 33: 					/*WM_BOTTOMED*/
		case WM_TOPPED: 
		case WM_NEWTOP:
											set_wiclip(win);
											set_wifont(win);
											if (!winp->iconify && winp->editierbar)		/* ssl 111294 */
											{	hide_mouse();
												cursor(win,FALSE);
											}
											if (msgbuff[0]==WM_TOPPED || msgbuff[0]==WM_NEWTOP)
												wind_set(msgbuff[3],WF_TOP,0,0,0,0);
											else if (msgbuff[0]==33 /*WM_BOTTOMED*/)
												wind_set(msgbuff[3],WF_BOTTOM,0,0,0,0);
											
											winhelp=found_wind();
											if (winhelp>=0)
											{	
												if (wi[winhelp].listwind==WI_LIST_AREAS)
												{	set_menu(ALIST_WIND);
												}
												else if (wi[winhelp].listwind==WI_LIST_MSGS)
												{	set_menu(MLIST_WIND);
												}
												else if (wi[winhelp].editierbar)
												{	set_menu(EDIT_WIND);
/*													s_cutbuff=e_cutbuff=-1; */
													if (!wi[winhelp].iconify)
													{	upded_stat(winhelp,0);
														cursor(winhelp,TRUE);
													}
												}
												else
													set_menu(MSG_WIND);
											}
											show_mouse();
											break;

		case WM_SIZED: 
		case WM_MOVED:
											if (!winp->iconify && winp->listwind)
												printline(win,winp->selline,FALSE);
											set_wiclip(win);
											set_wifont(win);
											oldw = winp->w;
											oldh = winp->h;
											
											if (!winp->iconify)
											{	if (winp->editierbar)
												{	/* if (!mtitle) */
													cursor(win,FALSE);
													align_edit(win,&msgbuff[4],&msgbuff[5],&msgbuff[6],&msgbuff[7]);
												}
												else
												{	if (msgbuff[4] < 3)	msgbuff[4] = 3;
													else								msgbuff[4] = alignx(msgbuff[4]);
												}
											}
											wind_set(msgbuff[3],WF_CURRXYWH,msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
								
											if (msgbuff[0]==WM_SIZED)
											{	hspmax=winp->firstc;
												winp->firstc=0;
												set_wiclip(win);
												set_wifont(win);
												set_wislider(win);
												if (hspmax!=winp->firstc)
												{	winp->oldcx = winp->cx;
													if (winp->editierbar && winp->oldcx >= winp->bspalten)
														winp->oldcx = winp->bspalten-1;
												}
#if 1
												if (oldw<winp->w || oldh<winp->h)
												{	word xmsgbuff[8];
													xmsgbuff[0]=WM_REDRAW;
													xmsgbuff[3]=winp->handle;
													xmsgbuff[4]=winp->x;
													xmsgbuff[5]=winp->y;
													xmsgbuff[6]=winp->w;
													xmsgbuff[7]=winp->h;
													hndl_wind(xmsgbuff);
												}
#endif
											}
											set_wiclip(win);
											set_wifont(win);
											if (!winp->iconify && winp->editierbar /* && !mtitle */)
												cursor(win,TRUE);
											if (!winp->iconify && winp->listwind)
												printline(win,winp->selline,TRUE);
											break;

		case WM_CLOSED:
											if (win==MSGSWIND && winp->listwind==WI_LIST_MSGS)		/* Msgwindow ist jetzt geschlossen */
												opt.winprm[MSGSWIND][6]=0;
											if (win<MAX_WIND && !winp->editierbar)
											{	if (wi[MSGSWIND].belegt)
													if (winp->aktarea==wi[MSGSWIND].aktarea)
															close_window(MSGSWIND);		/* ssl 161294 */
											}

											close_window(win);		/* ssl 141294 */

											if (win<MAX_WIND && !winp->editierbar)
											{	for (i=0;i<MAX_WIND;i++)
												{	if (wi[i].belegt && !wi[i].editierbar && wi[i].handle>0 &&
								/*neu->*/			wi[MSGSWIND].belegt && wi[MSGSWIND].listwind==WI_LIST_MSGS)
													{	open_msglistwindow(i);
														if (found_wind()!=i)
															top_it(i);
														break;
													}
												}
											}
											break;

		case WM_FULLED:		wifull(win);					/* ssl 111294 */
											break;

		case WM_ARROWED:	set_wiclip(win);
											set_wifont(win);
											if (winp->editierbar)
											{	cursor(win,FALSE);
												hspmax=opt.maxedcol+1-winp->bspalten;
											}
											else
											{	if (winp->listwind)
													hspmax=MAXCOL_LISTWIND-winp->bspalten;
												else
													hspmax=opt.maxcol+1-winp->bspalten;
											}
												
											switch (msgbuff[4])
											{
												case 0:   /* Seite nach oben */
													if (winp->maxshow>0 && winp->topline > winp->maxshow-1)
													{
														if(winp->listwind)
															winp->topline-=(winp->maxshow);
														else	
															winp->topline-=(winp->maxshow-1);
													}	
													else if (winp->topline != 0)
														winp->topline = 0;
													else
														red = FALSE;
													break;
												case 1:   /* Seite nach unten */
													if (winp->maxshow>0 && 
													    winp->topline<winp->lzahl-winp->maxshow)
													{
														if (winp->listwind)
															winp->topline+=winp->maxshow;
														else	
															winp->topline+=winp->maxshow-1;
														if (winp->lzahl-winp->maxshow<winp->topline)
															winp->topline=winp->lzahl-winp->maxshow;
													}
													else
													{
														nextmsg_fl = TRUE;
														red = FALSE;
													}
													break;
												case 2:   /* Zeile nach oben */
													if (winp->topline > 0)
													{	if (win==found_wind())
														{	
															if (winp->listwind)
																printline(win,winp->selline,FALSE);
															set_wiclip(win);
															set_wifont(win);
															scr_down(win);
															if (winp->listwind)
																printline(win,winp->selline,TRUE);
														}
														else
														{	winp->topline--;
															wisetfiles(win,FALSE,NO_CHANGE);
														}
													}
													red=FALSE;
													break;
												case 3:  /* Zeile nach unten */
													if (winp->topline < winp->lzahl-winp->maxshow)
													{	if (win==found_wind())
														{	
															if (winp->listwind)
																printline(win,winp->selline,FALSE);
															set_wiclip(win);
															set_wifont(win);
															scr_up(win);
															if (winp->listwind)
																printline(win,winp->selline,TRUE);
														}
														else
														{	winp->topline++;
															wisetfiles(win,FALSE,NO_CHANGE);
														}
													}
													red=FALSE;
													break;
												case 4:  /* Seite nach links */
													if (winp->firstc > winp->bspalten)
														winp->firstc-=winp->bspalten;
													else if (winp->firstc != 0)
														winp->firstc=0;
													else
														red=FALSE;
													break;
												case 5:   /* Seite nach rechts */
													if (/*!winp->editierbar && */winp->firstc < hspmax)
													{
														winp->firstc+=winp->bspalten;
														if (winp->firstc > hspmax)
															winp->firstc=hspmax;
													}
													else
														red=FALSE;
													break;
												case 6:   /* Spalte nach links */
													if (/*!winp->editierbar && */winp->firstc > 0)
														winp->firstc--;
													else
														red=FALSE;
													break;
												case 7:   /* Spalte nach rechts */
													if (/*!winp->editierbar && */winp->firstc < hspmax)
														winp->firstc++;
													else
														red=FALSE;
													break;
											}
											if (winp->editierbar)
											{	winp->lnr=winp->topline+winp->cy-winp->topmsg_line;
												set_cxpos(win);
												if (red)
													pline(win,winp->topmsg_line,winp->bzeilen,TRUE,TRUE);
												cursor(win,TRUE);
											}
											else if (red)
												wisetfiles(win,FALSE,NO_CHANGE);
										break;

		case WM_VSLID: 	set_wiclip(win);
										set_wifont(win);
										if (winp->lines[0]!=NULL && winp->maxshow>0)
										{	if (winp->editierbar)
												cursor(win,FALSE);
							
											oldh = winp->topline;
											i = (word)((long)msgbuff[4]*(winp->lzahl-winp->maxshow)/1000L);
											if (i==oldh) break;
											winp->topline=i;
							
											if (winp->editierbar)
											{	winp->lnr=winp->topline+winp->cy-winp->topmsg_line;
												set_cxpos(win);
											}
											wisetfiles(win,FALSE,NO_CHANGE);
											if (winp->editierbar)
												cursor(win,TRUE);
										}
										break;

		case WM_HSLID:	set_wiclip(win);
										set_wifont(win);
										if (!winp->editierbar)
										{	oldh=winp->firstc;
											if (winp->listwind)
												i = (word)((long)msgbuff[4]*(MAXCOL_LISTWIND-winp->bspalten)/1000L);
											else
												i = (word)((long)msgbuff[4]*(opt.maxcol+1-winp->bspalten)/1000L);
											if (i==oldh) break;
											winp->firstc=i;
							
											wisetfiles(win,FALSE,NO_CHANGE);
										}
										else
										{	oldh=winp->firstc;
											i = (word)((long)msgbuff[4]*(opt.maxedcol+1-winp->bspalten)/1000L);
											if (i==oldh) break;
											cursor(win,FALSE);
											winp->firstc=i;
											winp->lnr=winp->topline+winp->cy-winp->topmsg_line;
											pline(win,winp->topmsg_line,winp->bzeilen,TRUE,TRUE);
											cursor(win,TRUE);
										}
										break;
	}

	winhelp=found_wind();				/* ssl 280295 */
	if (winhelp>=0)
	{	if (wi[winhelp].iconify)
			set_menudisabled();
		else if (wi[winhelp].listwind==WI_LIST_AREAS)
			set_menu(ALIST_WIND);
		else if (wi[winhelp].listwind==WI_LIST_MSGS)
			set_menu(MLIST_WIND);
		else if (wi[winhelp].lastmsg==0)
			set_menu(NO_MSG);
		else if (wi[winhelp].aktmsg==0)
			set_menu(DEL_MSG);
		else if (wi[winhelp].editierbar)
			set_menu(EDIT_WIND);	
		else		
			set_menu(MSG_WIND);	
	}

	i=0;
	for (winhelp=0;winhelp<TOTAL_WIND;winhelp++)
		if (wi[winhelp].belegt)
			i++;
	if (i<=1)	menu_tree[ACYCLE].ob_state|=DISABLED;
	else			menu_tree[ACYCLE].ob_state&=~DISABLED;
	if (i<=0)	menu_tree[ACLOSE].ob_state|=DISABLED;
	else			menu_tree[ACLOSE].ob_state&=~DISABLED;

	if (!mtitle)
		show_mouse();
	return (verlassen);
}
