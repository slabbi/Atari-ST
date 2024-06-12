#include "ccport.h"
#include <time.h>
#include "userlist.rh"
#include "switches.rh"
#include "killmsg.rh"
#include "header.rh"
#include "headerf.rh"
#include "headers.rh"
#include "comment.rh"
#include "window.rh"
#include "quote.rh" 
#include "freq.rh" 
#include "fkeys.rh"
#include "helpmsg.rh"
#include "msginfo.rh"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "vars.h"
#include "language.h"

byte msgfrequest[8][14];
word msgfreqcount=0;

void msginfo(long win)
{	byte s[MAX_STR],tmp[MAX_STR],tmp2[MAX_STR];
	byte *cp;
	word ext_btn,akt;
	struct ffblk dta;
	long	shdr=-1,smsg=-1;

	akt=wi[win].aktmsg;

	sprintf(s,"%s.%s",gl_area[wi[win].aktarea].path, HEADEREXT);
	if (!findfirst(s,&dta,0))	shdr=dta.ff_fsize;
	sprintf(s,"%s.MSG",gl_area[wi[win].aktarea].path);
	if (!findfirst(s,&dta,0))	smsg=dta.ff_fsize;

	set_dtext(msginfo_tree,MIAREA,gl_area[wi[win].aktarea].name,FALSE,30);

	sprintf(s,"#%d of %d",akt, wi[win].lastmsg);
	set_dtext(msginfo_tree,MIMSG,s,FALSE,20);

	strcpy(tmp,gl_area[wi[win].aktarea].path);
	cp=tmp;
	if (strlen(cp)>40)
	{	while (strlen(cp)>40)
			cp++;
		cp[0]=cp[1]=cp[2]='.';
	}
	set_dtext(msginfo_tree,MIFILE,cp,FALSE,40);
	if (akt>0)
	{	set_dtext(msginfo_tree,MISTART,ltoa(wi[win].hdr->lStart,s,10),FALSE,8);
		set_dtext(msginfo_tree,MISIZE,ltoa(wi[win].hdr->Size,s,10),FALSE,6);
	}
	else
	{	set_dtext(msginfo_tree,MISTART,"-",FALSE,8);
		set_dtext(msginfo_tree,MISIZE,"-",FALSE,6);
	}
	set_dtext(msginfo_tree,MIFHDR,shdr!=-1?ltoa(shdr,s,10):"-",FALSE,8);
	set_dtext(msginfo_tree,MIFMSG,smsg!=-1?ltoa(smsg,s,10):"-",FALSE,8);

	get_flags(win,s,tmp,tmp2);
	set_dtext(msginfo_tree,MIFLAG1,s,FALSE,30);
	set_dtext(msginfo_tree,MIFLAG2,tmp,FALSE,30);
	set_dtext(msginfo_tree,MIFLAG3,tmp2,FALSE,31);

	draw_dialog(msginfo_tree, &msginfo_dial);
	ext_btn=Form_do(&msginfo_dial,0);
	del_dialog(&msginfo_dial);
	del_state(msginfo_tree,ext_btn,SELECTED,FALSE);
}

void helping(OBJECT *tree, DIALINFO *dial)
{
	word ext_btn;
	
	mouse_normal();
	show_mouse();
	draw_dialog(tree, dial);
	ext_btn=Form_do(dial,0);
	del_state(tree,ext_btn,SELECTED,FALSE);
	del_dialog(dial);
	if (tree==hmsg_tree && ext_btn==HMNEXT)
	{
		draw_dialog(hmsg2_tree, &hmsg2_dial);
		ext_btn=Form_do(&hmsg2_dial,0);
		del_state(hmsg2_tree,ext_btn,SELECTED,FALSE);
		del_dialog(&hmsg2_dial);
	}
}
          		
void set_userltext(word obj, word i)
{	byte tmp[14];
	byte *cp;
	
	cp=strrchr(opt.userlist[i],BSLASH);
	if (cp==NULL)
		cp=opt.userlist[i];
	else
		cp++;
	strcpy(tmp,cp);
	set_dtext(user_tree,obj,tmp,FALSE,12);
	set_dtext(user_tree,obj+1,opt.userlistdomain[i],FALSE,12);
}
          		
void get_userltext(word obj, word i)
{	byte tmp[MAX_STR];
	byte *cp;
	
	get_dedit(user_tree,obj+1,opt.userlistdomain[i]);
	get_dedit(user_tree,obj,tmp);
	cp=strrchr(opt.userlist[i],BSLASH);
	if (cp==NULL)
		cp=opt.userlist[i];
	else
		cp++;
	strcpy(cp,tmp);
}          		
          		
          		
void dia_optswitches(void)
{	word i, ext_btn, old_umlaut, old_hdrcache, old_flag;

	old_umlaut = opt.umlaut;
	old_hdrcache = opt.hdrcache;
	old_flag = opt.flag;

	if (gem_statusline)
		set_dtext(switches_tree,LGEMSACT,msgtxt[SYSTEM_GEMINFOACT].text,FALSE,23);
	else
		set_dtext(switches_tree,LGEMSACT,msgtxt[SYSTEM_GEMINFONOT].text,FALSE,323);

	set_dopt(opt.saveopt,			LSAVEOPT);
	set_dopt(opt.hdrcache,		LCACHE);
	set_dopt(opt.savedlg,			LSAVEDLG);
	set_dopt(opt.hideseen,		LHIDESEE);
	set_dopt(opt.hideeid,			LHIDEEID);
	set_dopt(opt.hidepid,			LHIDEPID);
	set_dopt(opt.hideca,			LHIDECA);
	set_dopt(opt.showcr,			LSHOWCR);
	set_dopt(opt.exttab,			LEXTTAB);
	set_dopt(opt.delnewmsg,		LDELNEW);
	set_dopt(opt.sredraw,			LSREDRAW);
	set_dopt(opt.savelread,		LSAVELR);
/*	set_dopt(opt.ownorigin,		LORIGIN); */
	set_dopt(opt.listawindow,	LLISTW);
	set_dopt(opt.listmwindow,	LLISTW2);
/*	set_dopt(opt.protecthdr,	LPROTECT); */
	set_dopt(opt.addrcornet,	LKORRNET);
	set_dopt(opt.always_fixed,LFIXED);
	set_dopt(opt.showdesc,		LDESC);
	set_dopt(opt.usecommenttree,LMSGTREE);
	set_dopt(opt.usereplyto,	LREPTO);
	set_dopt(opt.usereplyaddr,LREPADDR);

	set_dopt(opt.autoclose & O_AUTOCLOSE_AREA,LAUTO);
	set_dopt(opt.autoclose & O_AUTOCLOSE_MSGS,LAUTOMSG);

	set_dopt(opt.printopt & O_PRINTOPT_FF,		LFORMFD);
	set_dopt(opt.printopt & O_PRINTOPT_GDOS,	LGDOS);
	set_dopt(opt.printopt & O_PRINTOPT_INFO,	LINFO);

	set_dopt(opt.texteffects & O_TEXTEFFSCR,	LTEXTEFF);
	set_dopt(opt.texteffects & O_TEXTEFFPRN,	LPRTEFF);

	set_dopt(opt.addre & O_ADDRE,		LADDRE);
	set_dopt(opt.addre & O_ADDREN,	LADDREN);

	set_dopt(opt.umlaut & O_UMLAUT_N, LCONVNET);
	set_dopt(opt.umlaut & O_UMLAUT_E,	LCONVECH);
	set_dopt(opt.umlaut & O_ISOCONV,	LISOCONV);
	set_dopt(opt.umlaut & O_MIMESTYLE,LMIMESTY);
	set_dopt(opt.umlaut & O_ATARIFONT,LATARI);

	set_dopt(opt.flag & O_FLAG_RANDOMTEAR,	LRANDOM);
	set_dopt(opt.flag & O_FLAG_GEMINFOLINE,	LGEMSTAT);
	set_dopt(opt.flag & O_FLAG_SHOWPOS,			LSHOWPOS);
	set_dopt(opt.flag & O_FLAG_AKASYSOP,		LAKASYS);

	set_dopt(opt.deadkeyflag & DKEY_DACH,			LDDACH);
	set_dopt(opt.deadkeyflag & DKEY_SCHLANGE,	LDSCHLAN);
	set_dopt(opt.deadkeyflag & DKEY_A,				LDA);
	set_dopt(opt.deadkeyflag & DKEY_BACKA,		LDBACKA);
	set_dopt(opt.deadkeyflag & DKEY_DPNT,			LDDPNT);
	set_dopt(opt.deadkeyflag & DKEY_LITERAL,	LDLITERA);
	set_dopt(opt.deadkeyflag & DKEY_KREIS,		LDKREIS);
	set_dopt(opt.deadkeyflag & DKEY_KOMMA,		LDKOMMA);
	set_dopt(opt.deadkeyflag & DKEY_SLASH,		LDSLASH);
	set_dopt(opt.deadkeyflag & DKEY_MASTER,		LDDEAD);

	draw_dialog(switches_tree, &switches_dial);
	ext_btn=DialDo(&switches_dial,0);
	
  if (ext_btn==LOK)
	{	opt.saveopt =req_button(switches_tree,LSAVEOPT);
		opt.hdrcache=req_button(switches_tree,LCACHE);
		opt.savedlg =req_button(switches_tree,LSAVEDLG);
		opt.hideeid =req_button(switches_tree,LHIDEEID);
		opt.hidepid =req_button(switches_tree,LHIDEPID);
		opt.hideseen=req_button(switches_tree,LHIDESEE);
		opt.hideca  =req_button(switches_tree,LHIDECA);
		opt.showcr  =req_button(switches_tree,LSHOWCR);
		opt.exttab  =req_button(switches_tree,LEXTTAB);
		opt.delnewmsg		=req_button(switches_tree,LDELNEW);
		opt.sredraw 		=req_button(switches_tree,LSREDRAW);		
		opt.savelread		=req_button(switches_tree,LSAVELR);		
/*		opt.ownorigin		=req_button(switches_tree,LORIGIN); */
		opt.listawindow	=req_button(switches_tree,LLISTW);
		opt.listmwindow	=req_button(switches_tree,LLISTW2);
/*		opt.protecthdr	=req_button(switches_tree,LPROTECT); */
		opt.addrcornet	=req_button(switches_tree,LKORRNET);
		opt.always_fixed=req_button(switches_tree,LFIXED);
		opt.showdesc		=req_button(switches_tree,LDESC);
		opt.usecommenttree=req_button(switches_tree,LMSGTREE);
		opt.usereplyto	=req_button(switches_tree,LREPTO);
		opt.usereplyaddr=req_button(switches_tree,LREPADDR);

		opt.flag=0;
		if (req_button(switches_tree,LRANDOM))	opt.flag |= O_FLAG_RANDOMTEAR;
		if (req_button(switches_tree,LGEMSTAT))	opt.flag |= O_FLAG_GEMINFOLINE;
		if (req_button(switches_tree,LSHOWPOS))	opt.flag |= O_FLAG_SHOWPOS;
		if (req_button(switches_tree,LAKASYS))	opt.flag |= O_FLAG_AKASYSOP;

		opt.autoclose=0;
		if (req_button(switches_tree,LAUTO))		opt.autoclose |= O_AUTOCLOSE_AREA;
		if (req_button(switches_tree,LAUTOMSG))	opt.autoclose |= O_AUTOCLOSE_MSGS;

		opt.printopt=0;
		if (req_button(switches_tree,LFORMFD))	opt.printopt |= O_PRINTOPT_FF;
		if (req_button(switches_tree,LGDOS))		opt.printopt |= O_PRINTOPT_GDOS;
		if (req_button(switches_tree,LINFO))		opt.printopt |= O_PRINTOPT_INFO;

		opt.texteffects=0;
		if (req_button(switches_tree,LTEXTEFF))	opt.texteffects |= O_TEXTEFFSCR;
		if (req_button(switches_tree,LPRTEFF))	opt.texteffects |= O_TEXTEFFPRN;

		opt.addre=0;
		if (req_button(switches_tree,LADDRE)) 	opt.addre |= O_ADDRE;
		if (req_button(switches_tree,LADDREN)) 	opt.addre |= O_ADDREN;

		opt.umlaut=0;
		if (req_button(switches_tree,LCONVNET))	opt.umlaut |= O_UMLAUT_N;
		if (req_button(switches_tree,LCONVECH))	opt.umlaut |= O_UMLAUT_E;
		if (req_button(switches_tree,LISOCONV))	opt.umlaut |= O_ISOCONV;
		if (req_button(switches_tree,LMIMESTY))	opt.umlaut |= O_MIMESTYLE;
		if (req_button(switches_tree,LATARI))		opt.umlaut |= O_ATARIFONT;
		
		opt.deadkeyflag=0;
		if (req_button(switches_tree,LDDACH))		opt.deadkeyflag |= DKEY_DACH;
		if (req_button(switches_tree,LDSCHLAN))	opt.deadkeyflag |= DKEY_SCHLANGE;
		if (req_button(switches_tree,LDA))			opt.deadkeyflag |= DKEY_A;
		if (req_button(switches_tree,LDBACKA))	opt.deadkeyflag |= DKEY_BACKA;
		if (req_button(switches_tree,LDDPNT))		opt.deadkeyflag |= DKEY_DPNT;
		if (req_button(switches_tree,LDLITERA))	opt.deadkeyflag |= DKEY_LITERAL;
		if (req_button(switches_tree,LDKREIS))	opt.deadkeyflag |= DKEY_KREIS;
		if (req_button(switches_tree,LDKOMMA))	opt.deadkeyflag |= DKEY_KOMMA;
		if (req_button(switches_tree,LDSLASH))	opt.deadkeyflag |= DKEY_SLASH;
		if (req_button(switches_tree,LDDEAD))		opt.deadkeyflag |= DKEY_MASTER;


		if ((opt.umlaut & O_UMLAUT_E) && !(old_umlaut & O_UMLAUT_E))
			LangAlert(MSG_UMLAUTEECHOAREA);

		if (opt.umlaut & O_ISOCONV)
			if (!available(msgtxt[DO_CHRRECOGNITION].text))
				opt.umlaut &= ~O_ISOCONV;
		if (opt.umlaut & O_MIMESTYLE)
			if (!available(msgtxt[DO_MIMECONVERTION].text))
				opt.umlaut &= ~O_MIMESTYLE;
		if (opt.texteffects & O_TEXTEFFPRN)
			if (!available(msgtxt[DO_PRINTEFFECTS].text))
				opt.texteffects &= ~O_TEXTEFFPRN;
		if (opt.addre & O_ADDREN)
			if (!available(msgtxt[DO_CNTREPLIES].text))
				opt.addre &= ~O_ADDREN;
		if (opt.usecommenttree)
			if (!available(msgtxt[DO_COMMENTTREEWINDOW].text))
				opt.usecommenttree = FALSE;

	}
	del_state(switches_tree,ext_btn,SELECTED,FALSE);
	del_dialog(&switches_dial);
	
	if (wi[AREAWIND].belegt && !opt.listawindow)
		close_window(AREAWIND);		/* ssl 160296 */
	if (wi[MSGSWIND].belegt && !opt.listmwindow)
		close_window(MSGSWIND);		/* ssl 160296 */

	if (ext_btn==LOK)
	{	if ((!old_hdrcache && opt.hdrcache) ||
				((old_flag & O_FLAG_AKASYSOP) ^ (opt.flag & O_FLAG_AKASYSOP)) )
			for (i=0;i<MAX_WIND;i++)
				if (wi[i].belegt && !wi[i].editierbar && !wi[i].listwind)
					change_area(i, wi[i].aktarea);
		for (i=0;i<MAX_WIND;i++)
			if (wi[i].belegt && !wi[i].editierbar && !wi[i].listwind)
	   		if (wi[i].aktmsg>0)
	   		{	wi[i].oldmsg=0;
					select_msg(i,wi[i].aktmsg,TRUE,TRUE);
				}
		for (i=0;i<MAX_WIND;i++)
			if (wi[i].belegt && wi[i].editierbar && !(opt.flag & O_FLAG_SHOWPOS))
				upded_stat(i,0);
	}
}

void dia_optcomment(void)
{
	word ext_btn;
	long win;

	ext_btn=0;	
	set_treeopt(comment_tree, opt.showcom 			,COSHOW);
	set_treeopt(comment_tree, opt.commailer			,COMAILER);
	set_treeopt(comment_tree, opt.numpadcomment ,CONUMPAD);
	set_treeopt(comment_tree, opt.completescan	,COMSCAN);
		
	draw_dialog(comment_tree, &comment_dial);
	ext_btn=DialDo(&comment_dial,ext_btn);
	
    if (ext_btn==COOK)
	{
		opt.showcom				=req_button(comment_tree,COSHOW);
		opt.commailer			=req_button(comment_tree,COMAILER);
		opt.numpadcomment	=req_button(comment_tree,CONUMPAD);
		opt.completescan	=req_button(comment_tree,COMSCAN);

	 	win=found_wind();
	 	if (win>=0L && !list_window(win) && !edit_window(win))
	 	{
   		set_comstatus(win, wi[win].aktmsg);
			chk_status(win,TRUE);
		}
	} 
	del_state(comment_tree,ext_btn,SELECTED,FALSE);
	del_dialog(&comment_dial);
}

void dia_optwindow(void)
{	byte smaxcol[4],sedcol[4],stab[4],tmp[128];
	word ext_btn,rep=FALSE;

	sprintf(smaxcol,"%d",opt.maxcol);
	sprintf(sedcol,"%d",opt.maxedcol);
	sprintf(stab,"%d",opt.tabsize);
	set_dtext(window_tree,LMAXCOL,smaxcol,FALSE,2);
	set_dtext(window_tree,LEDCOL,sedcol,FALSE,2);
	set_dtext(window_tree,LTABSIZE,stab,FALSE,1);

	draw_dialog(window_tree, &window_dial);
	
	do 
	{
		ext_btn=DialDo(&window_dial,LMAXCOL);
		rep=FALSE;
		
		if (ext_btn==WOK)
		{
			get_dedit(window_tree,LMAXCOL,smaxcol);
			get_dedit(window_tree,LEDCOL,sedcol);
   		get_dedit(window_tree,LTABSIZE,stab);
			opt.maxcol=atoi(smaxcol);
			opt.maxedcol=atoi(sedcol);
			opt.tabsize=atoi(stab);
			if (!opt.tabsize)
				opt.tabsize=1;

			if (opt.maxcol>MAX_COL)
			{	sprintf(tmp,msgtxt[MSG_MAXWINDCOLS].text,MAX_COL);
				FormAlert(msgtxt[MSG_MAXWINDCOLS].defbut,msgtxt[MSG_MAXWINDCOLS].icon,tmp,msgtxt[MSG_MAXWINDCOLS].button);
				opt.maxcol=MAX_COL;
				sprintf(smaxcol,"%d",opt.maxcol);
				set_dtext(window_tree,LMAXCOL,smaxcol,TRUE,2);
				rep=TRUE;
			}
			if (opt.maxcol<11)
			{	LangAlert(MSG_MINWINDCOLS);
				opt.maxcol=11;
				sprintf(smaxcol,"%d",opt.maxcol);
				set_dtext(window_tree,LMAXCOL,smaxcol,TRUE,2);
				rep=TRUE;
			}
			if (opt.maxedcol>MAX_EDCOL)
			{	sprintf(tmp,msgtxt[MSG_MAXEDITCOLS].text,MAX_EDCOL);
				FormAlert(msgtxt[MSG_MAXEDITCOLS].defbut,msgtxt[MSG_MAXEDITCOLS].icon,tmp,msgtxt[MSG_MAXEDITCOLS].button);
				opt.maxedcol=MAX_EDCOL;
				sprintf(sedcol,"%d",opt.maxedcol);
				set_dtext(window_tree,LEDCOL,sedcol,TRUE,2);
				rep=TRUE;
			}
			if (opt.maxedcol<11)
			{	LangAlert(MSG_MINEDITCOLS);
				opt.maxedcol=11;
				sprintf(sedcol,"%d",opt.maxedcol);
				set_dtext(window_tree,LEDCOL,sedcol,TRUE,2);
				rep=TRUE;
			}
			if (opt.maxcol<opt.maxedcol)
			{	LangAlert(MSG_MSGWINDEDITWIND);
				rep=TRUE;
			}
			if (rep)
				del_state(window_tree,ext_btn,SELECTED,TRUE);
		}	
	} while (rep);	
	del_dialog(&window_dial);
	del_state(window_tree,ext_btn,SELECTED,FALSE);

	if (ext_btn==WOK)
	{	for (rep=0;rep<MAX_WIND;rep++)
			if (wi[rep].belegt && !wi[rep].editierbar && !wi[rep].listwind)
	   		if (wi[rep].aktmsg>0)
	   		{	wi[rep].oldmsg=0;
					select_msg(rep,wi[rep].aktmsg,TRUE,TRUE);
				}
	}
}

void dia_optquote(void)
{	word ext_btn=0;

	del_mstate(quote_tree,SELECTED,FALSE,LINMSG,LINNACH,LGERDET,LNONE,LQUINIT,LQUNORM,LQSET,LQNCOMP,LQNOTOP,-1);

	set_dtext(quote_tree,QNATTOP,opt.nattop,FALSE,75);
	set_dtext(quote_tree,QENGTOP,opt.engtop,FALSE,75);
	set_dtext(quote_tree,QNAT2TOP,opt.nat2top,FALSE,75);
	set_dtext(quote_tree,QENG2TOP,opt.eng2top,FALSE,75);

	if (opt.quote & O_QUOTE_MSG)
		set_state(quote_tree,LINMSG,SELECTED,FALSE);
	else if (opt.quote & O_QUOTE_NAT)
		set_state(quote_tree,LINNACH,SELECTED,FALSE);
	else if (opt.quote & O_QUOTE_DET)
		set_state(quote_tree,LGERDET,SELECTED,FALSE);
	else
		set_state(quote_tree,LNONE,SELECTED,FALSE);
	
	if (opt.quote & O_QUOTE_FRE)
		set_state(quote_tree,LFREE,SELECTED,FALSE);
	if (opt.quote & O_QUOTE_MSET)
		set_state(quote_tree,LQSET,SELECTED,FALSE);
	if (opt.quote & O_QUOTE_NCOMP)
		set_state(quote_tree,LQNCOMP,SELECTED,FALSE);
	if (opt.quote & O_QUOTE_NOTOP)
		set_state(quote_tree,LQNOTOP,SELECTED,FALSE);

	if (opt.quote & O_QUOTE_XY)
		set_state(quote_tree,LQUINIT,SELECTED,FALSE);
	else
		set_state(quote_tree,LQUNORM,SELECTED,FALSE);

	draw_dialog(quote_tree, &quote_dial);
	ext_btn=DialDo(&quote_dial,ext_btn);

	if (ext_btn==QOK)
	{
		opt.quote=0;
		if (req_state(quote_tree,LQUINIT,SELECTED))	opt.quote |= O_QUOTE_XY;
		if (req_state(quote_tree,LINMSG ,SELECTED)) opt.quote |= O_QUOTE_MSG;
		if (req_state(quote_tree,LINNACH,SELECTED)) opt.quote |= O_QUOTE_NAT;
		if (req_state(quote_tree,LFREE  ,SELECTED)) opt.quote |= O_QUOTE_FRE;
		if (req_state(quote_tree,LGERDET,SELECTED)) opt.quote |= O_QUOTE_DET;
		if (req_state(quote_tree,LQSET  ,SELECTED)) opt.quote |= O_QUOTE_MSET;
		if (req_state(quote_tree,LQNCOMP,SELECTED)) opt.quote |= O_QUOTE_NCOMP;
		if (req_state(quote_tree,LQNOTOP,SELECTED)) opt.quote |= O_QUOTE_NOTOP;
		get_dedit(quote_tree,QNATTOP,opt.nattop);
		get_dedit(quote_tree,QENGTOP,opt.engtop);
		get_dedit(quote_tree,QNAT2TOP,opt.nat2top);
		get_dedit(quote_tree,QENG2TOP,opt.eng2top);
	}	
	del_dialog(&quote_dial);
	del_state(quote_tree,ext_btn,SELECTED,FALSE);
}

void dia_optuser(void)
{	byte userpath[MAX_STR];
	byte tmp[20];
	word i, ext_btn, dclick;
	word ut[] = {LUSER1,LUSER2,LUSER3,LUSER4,LUSER5,LUSER6,
							 LUSER7,LUSER8,LUSER9,LUSER10,LUSER11,LUSER12};
	
	for (i=0;i<MAX_USERLIST;i++)
		set_userltext(ut[i],i);

	draw_dialog(user_tree, &user_dial);
	
	ext_btn=LUSER1;
	do
	{
		ext_btn=DialDo(&user_dial,ext_btn);
		dclick=FALSE;
		if (ext_btn & 0x8000)
		{	ext_btn &= 0x7fff;
			dclick=TRUE;
		}
		
		if (dclick && ext_btn>=LUSER1 && ext_btn<=LUSER12)
		{	get_dedit(user_tree,ext_btn,tmp);
			del_dialog(&user_dial);
			i=(ext_btn-LUSER1)>>1;
			strcpy(userpath,opt.userlist[i]);
			if (hole_file(userpath,tmp,"*.*",msgtxt[FILE_PATHUSERLIST].text))
			{	set_dtext(user_tree,ext_btn,tmp,FALSE,12);
				strcpy(opt.userlist[i],userpath);
			}
			draw_dialog(user_tree, &user_dial);
		}
	} while(ext_btn!=UOK && ext_btn!=UCANCEL);

	if (ext_btn==UOK)
	{	for(i=0;i<MAX_USERLIST;i++)
		{	get_dedit(user_tree,ut[i],tmp);
			if (*tmp==EOS)
				*opt.userlist[i]=EOS;
			get_userltext(ut[i],i);
		}
	}
	del_dialog(&user_dial);
	del_state(user_tree,ext_btn,SELECTED,FALSE);
}

void save_options(void)
{	get_optwind(FALSE);
	save_optled();
}

void dia_optfont(void)
{	word msg_index, ed_index, mlist_index, alist_index;
	word msg_pt, ed_pt, mlist_pt, alist_pt;
	word old_mfid, old_efid, old_mlistfid, old_alistfid;
	byte old_mpt, old_ept, old_mlistpt, old_alistpt;

	old_mfid=opt.msgid;	/* Store old values for: SAVE + CANCEL */
	old_mpt=opt.msgpt;
	old_efid=opt.editid;
	old_ept=opt.editpt;
	old_mlistfid=opt.mlistid;
	old_mlistpt=opt.mlistpt;
	old_alistfid=opt.alistid;
	old_alistpt=opt.alistpt;
	msg_index=get_fontindex(opt.msgid);
	ed_index=get_fontindex(opt.editid);
	mlist_index=get_fontindex(opt.mlistid);
	alist_index=get_fontindex(opt.alistid);
	msg_pt=(word)opt.msgpt;
	ed_pt=(word)opt.editpt;
	mlist_pt=(word)opt.mlistpt;
	alist_pt=(word)opt.alistpt;
	
	if (dia_font(&msg_index,&msg_pt,&ed_index,&ed_pt,&mlist_index,&mlist_pt,&alist_index,&alist_pt))
	{	opt.msgid=gl_font[msg_index].fid;
		opt.msgpt=(byte)msg_pt;
		opt.editid=gl_font[ed_index].fid;
		opt.editpt=(byte)ed_pt;
		opt.mlistid=gl_font[mlist_index].fid;
		opt.mlistpt=(byte)mlist_pt;
		opt.alistid=gl_font[alist_index].fid;
		opt.alistpt=(byte)alist_pt;
		redraw_all();
	}
	else	/* Cancel */
	{	opt.msgid=old_mfid;
		opt.msgpt=old_mpt;
		opt.editid=old_efid;
		opt.editpt=old_ept;
		opt.mlistid=old_mlistfid;
		opt.mlistpt=old_mlistpt;
		opt.alistid=old_alistfid;
		opt.alistpt=old_alistpt;
	}
}

void 
set_dopt(word option, word index)
{
	if (option)
		set_button(switches_tree,index,FALSE);
	else
		del_button(switches_tree,index,FALSE);
}

static void 
set_treeopt(OBJECT *tree, word option, word index)
{
	if (option)
		set_button(tree,index,FALSE);
	else
		del_button(tree,index,FALSE);
}

void save_optled(void)
{	FILE *fp;
	byte filename[MAX_STR];

	opt.version=LEDVERSION;
	biene();
	sprintf(filename,"%sLED.OPT",gl_ledpath);
	if ((fp=fopen(filename,"wb"))!=NULL)		/* war w+b */
	{
		fwrite(&opt,sizeof(OPT),1,fp);
		fclose(fp);
	}
	if (!vq_gdos())
	{
		if (opt.msgid!=1)
			opt.msgid=1;
		if (opt.editid!=1)
			opt.editid=1;
	}
	mouse_normal();
}

void def_optled(void)
{	word i,t;
	
	for (i=0;i<MAX_WINOPT;i++)	 				/* ssl 91294: List/Areawindowarray l”schen */
		opt.winprm[MSGSWIND][i] =
		opt.winprm[AREAWIND][i] = 0;

	opt.version =LEDVERSION;
	for (i=0;i<MAX_WIND;i++)
		for (t=0;t<MAX_WINOPT;t++)
			opt.winprm[i][t]=0;

	opt.saveopt			=
	opt.hdrcache		=
	opt.savedlg			=
	opt.hideeid			=
	opt.hidepid			=
	opt.hideca			=
	opt.hideseen		=
	opt.exttab			=
	opt.showcom 		=
	opt.commailer		=
	opt.savelread		=
	opt.monospaced	=
	opt.insert			=TRUE;

/*	opt.ownorigin		= */
	opt.numpadcomment=
	opt.sredraw 		=
	opt.showcr  		=
	opt.delnewmsg		=
/*	opt.skipcs  		= */
	opt.addrcornet	=
	opt.always_fixed=
	opt.listawindow =
	opt.listmwindow =
	opt.showdesc		=
	opt.usecommenttree=
	opt.blockrev		=
	opt.qthick			=
	opt.usereplyaddr=
	opt.usereplyto	=
	opt.completescan=FALSE;

									 /* =========================================================================== */
	strcpy(opt.nattop, "In einer Nachricht vom %d schrieb %f %F (%a) an %t %T:");
	strcpy(opt.nat2top,"In einer Nachricht vom %d schrieb %f %F (%a) an %t %T in der Area '%n':");
	strcpy(opt.engtop, "In a message of %d %f %F (%a) wrote to %t %T:");
	strcpy(opt.eng2top,"In a message of %d %f %F (%a) wrote to %t %T in area '%n':");

	opt._free1 =
	opt._free2 =
	opt._free3 = 0;
	opt.led_register = 0;

	opt.maxcol  	= 76;
	opt.maxedcol	= 75;
	opt.tabsize 	=  2;
	opt.indent		= -1;
	opt.texteffects=
	opt.request		=
	opt.umlaut  	=
	opt.quote    	=
	opt.flag			=
	opt.autoclose	=
	opt.deadkeyflag= 0;
	opt.addre			= O_ADDRE;
	opt.movemsg		= O_MOVEMSG_KEEP|O_MOVEMSG_FORWD;
	opt.printopt	= O_PRINTOPT_FF;

	opt.msgid = opt.editid = opt.mlistid = opt.alistid = 1;
	opt.msgpt = opt.editpt = opt.mlistpt = opt.alistpt = 10;

	for (i=0;i<MAX_USERLIST;i++)
		*opt.userlist[i]=EOS;

	strcpy(opt.userlist[0],gl_ledpath);
	strcat(opt.userlist[0],"PRVTUSER.LST");
}

void get_optled(void)
{	FILE *fp;
	word deflt;
	byte filename[MAX_STR];
	
	def_optled();
	opt.version=0;
	deflt=FALSE;
	show_doing(DO_READLEDOPTIONS);
	sprintf(filename,"%sLED.OPT",gl_ledpath);
	if ((fp=fopen(filename,"rb"))!=NULL)		/* war r+b */
	{
		fread(&opt,sizeof(OPT),1,fp);
		if (opt.version!=LEDVERSION || opt.maxcol<=0 || opt.maxedcol<=0)
			deflt=TRUE;
		fclose(fp);	
	}
	if (deflt)
	{	def_optled();
		show_doing(DO_SAVELEDOPTIONS);
		save_optled();
	}
	if (!vq_gdos())
	{	if (opt.msgid!=1)		opt.msgid=1;
		if (opt.editid!=1)	opt.editid=1;
		if (opt.mlistid!=1)	opt.mlistid=1;
		if (opt.alistid!=1)	opt.alistid=1;
	}
	test_registered();
}

void dia_freq(long win)
{	struct ffblk dta;
	word ext_btn, i;
	word fr_obj[8] = {FR1,FR2,FR3,FR4,FR5,FR6,FR7,FR8};
	word frp_obj[8] = {FRP1,FRP2,FRP3,FRP4,FRP5,FRP6,FRP7,FRP8};
	word doit_fl=FALSE;
	uword zone,net,node,point;
	byte frbuf[20],folderdomain[41];
	byte frpbuf[10];
	byte fr_fname[MAX_STR];
	byte dum[32], tmp[MAX_STR], *cp;
	byte *mode="w";
	FILE *fp;

	sprintf(tmp,"%-5u",addr[0].zone);
	set_dtext(freq_tree,FRNODE,tmp,FALSE,20);

	if (win>=0 && !wi[win].editierbar && !wi[win].listwind)
	{	i=FALSE;
		if (is_netmail(wi[win].aktarea))
		{	zone	=wi[win].hdr->wFromZone;
			net		=wi[win].hdr->wFromNet;
			node	=wi[win].hdr->wFromNode;
			point	=wi[win].hdr->wFromPoint;
			i=TRUE;
		}
		else
		{	if (get_origin_address(win,&zone,&net,&node,&point,FALSE))
				i=TRUE;
		}
		if (i)
		{	sprintf(dum,"%-5u%-5u%-5u%u",zone,net,node,point);
			set_dtext(freq_tree,FRNODE,dum,FALSE,20);
		}
	}

	for (i=0;i<8;i++)
	{	set_dtext(freq_tree,fr_obj[i],"",FALSE,12);
		set_dtext(freq_tree,frp_obj[i],"",FALSE,8);
	}
	if (msgfreqcount)
	{	for (i=0;i<msgfreqcount;i++)
			set_dtext(freq_tree,fr_obj[i],msgfrequest[i],FALSE,12);
	}

	if (opt.request & O_REQUEST_FLO)	set_button(freq_tree,FRFLO,FALSE);
	else															del_button(freq_tree,FRFLO,FALSE);
	if (opt.request & O_REQUEST_DOMAIN)	set_button(freq_tree,FRDOMAIN,FALSE);
	else																del_button(freq_tree,FRDOMAIN,FALSE);

	if (is_running("AVALON  ")>=0)
		del_state(freq_tree,FRAVALON,DISABLED,FALSE);
	else
		set_state(freq_tree,FRAVALON,DISABLED,FALSE);

	draw_dialog(freq_tree, &freq_dial);
	ext_btn=Form_do(&freq_dial,FRNODE);
	del_dialog(&freq_dial);
	del_state(freq_tree,ext_btn,SELECTED,FALSE);

	opt.request=0;
	if (req_button(freq_tree,FRFLO))		opt.request |= O_REQUEST_FLO;
	if (req_button(freq_tree,FRDOMAIN))	opt.request |= O_REQUEST_DOMAIN;

	if (ext_btn==FRAVALON)
	{	start_avalon();
		return;
	}

	if (ext_btn==FRCANCEL && msgfreqcount!=0)
	{	if (LangAlert(MSG_FREQKEEPDELBUFFER)==1)
			msgfreqcount=0;
	}
	else
		msgfreqcount=0;

	if (ext_btn==FROK)
	{	
		for (i=0;i<8;i++) /* test ob was eingetragen */
		{	get_dedit(freq_tree,fr_obj[i],frbuf);
			if (*frbuf && *frbuf!=' ' && *frbuf!='\n')
			{	doit_fl=TRUE;
				break;
			}	
		}	
		if (doit_fl==FALSE)
		{	LangAlert(MSG_NOTHINGTODO);
			return;
		}

		get_dedit(freq_tree,FRNODE,tmp);
		nnstr2node(tmp,&zone,&net,&node,&point);

		*folderdomain=EOS;
		if (req_button(freq_tree,FRDOMAIN))
		{	if (select_domain(tmp))
			{	for (i=0; i<=domaincount; i++)
					if (!strcmp(tmp,gl_domain[i][0]))
						break;
				if (i>domaincount)
				{	LangAlert(MSG_DOMAINNOTFOUND);
					return;
				}
				strcpy(folderdomain,gl_domain[i][1]);
				if (!i)
					*folderdomain=EOS;		/* Defaultdomain */
			}
			else
				return;
		}

		if (net!=0)
		{
			biene();
			strcpy(tmp, gl_outpath);

			if (*folderdomain)
			{	cp=strchr(tmp,EOS);
				if (*(cp-1)=='\\')
					*(cp-1)=EOS;
				cp=strrchr(tmp,'\\');
				if (cp)
				{	cp++;
					*cp=EOS;
					strcat(tmp,folderdomain);
				}
			}

			if (zone!=addr[0].zone || *folderdomain)
			{
				cp=strchr(tmp, EOS);
				if (*(cp-1)=='\\')
					*(cp-1)=EOS;

				if (gl_options & OUTBOUND36)
				{	strcpy(dum,"000");
					itoa(zone,dum+3,36);
					strcpy(dum,dum+strlen(dum)-3);
				}
				else
					sprintf(dum,"%03.3x",zone);
				strcat(tmp,".");
				strcat(tmp,dum);
				strcat(tmp,"\\");
			}	

			strcpy(fr_fname,tmp);
			cp=strchr(fr_fname, EOS);
			if (*(cp-1)=='\\')
				*(cp-1)=EOS;
			if (findfirst(fr_fname,&dta,FA_DIREC)) /* Pfad nicht vorhanden */
			{	sprintf(tmp,msgtxt[MSG_PATHNOTEXISTS].text,fr_fname);
				FormAlert(msgtxt[MSG_PATHNOTEXISTS].defbut,msgtxt[MSG_PATHNOTEXISTS].icon,tmp,msgtxt[MSG_PATHNOTEXISTS].button);
				return;
			}

			sprintf(fr_fname,"%s%s.ORT",tmp,addr2file36(net,node,point));

			if (!findfirst(fr_fname,&dta,0)) /* REQ vorhanden */
			{	ext_btn=LangAlert(MSG_REQUESTEXISTS);
				if (ext_btn==3) 	/* return */
					return;
				else if (ext_btn==2) /* append */
					mode="a";
				else			     /* overwrite */
					mode="w";
			}
					
			if ( (fp=fopen(fr_fname,mode))!=NULL )
			{
				for (i=0;i<8;i++)
				{
					get_dedit(freq_tree,fr_obj[i],frbuf);
					get_dedit(freq_tree,frp_obj[i],frpbuf);
					if (*frbuf && *frbuf!=' ' && *frbuf!='\n')
					{
						strupr(frbuf);
						fputs(frbuf,fp);
						if (strlen(frpbuf)>0L)
						{
							fputc(' ',fp);
							fputc('!',fp);
							fputs(frpbuf,fp);
						}
						fputc('\n',fp);
					}
				}
				fclose(fp);
				
				if (req_button(freq_tree,FRFLO))
				{
					strcpy(tmp, gl_outpath);
					if (zone!=addr[0].zone)
					{
						cp=strchr(tmp, EOS);
						if (*(cp-1)==BSLASH)
							*(cp-1)=EOS;
							
						if (gl_options & OUTBOUND36)
						{	strcpy(dum,"000");
							itoa(zone,dum+3,36);
							strcpy(dum,dum+strlen(dum)-3);
						}
						else
							sprintf(dum,"%03.3x",zone);
						strcat(tmp,dum);
					}	
					sprintf(fr_fname,"%s%s.OFT",tmp,addr2file36(net,node,point));
					if (findfirst(fr_fname,&dta,0)) /* FLO nicht vorhanden */
						if ( (fp=fopen(fr_fname,"w"))!=NULL )
							fclose(fp);
				}	
			}
			else
			{
				LangAlert(MSG_REQUESTFILEERROR);
			}
			mouse_normal();
		}
	}		
}






void gethdruword(uword *newval, word index)
{	byte tmp[40];
	get_dedit(header_tree,index,tmp);
	*newval=(uword)atoi(tmp);
}
	
void sethdruword(word index, uword num)
{	byte tmp[10];
	sprintf(tmp,"%u",num);
	set_dtext(header_tree,index,tmp,FALSE,5);
}

/* void sethdrulong(word index, ulong num)
{	byte tmp[10];
	sprintf(tmp,"%lu",num);
	set_dtext(header_tree,index,tmp,FALSE,10);
} */
	
void gethdruword_hex(uword *newval, word index)
{	byte tmp[40];
	get_dedit(header_tree,index,tmp);
	*newval=(uword)strtoul(tmp,NULL,16);
}
	
void sethdruword_hex(word index, uword num)
{	byte tmp[10];
	sprintf(tmp,"%x",num);
	set_dtext(header_tree,index,tmp,FALSE,4);
}

void gethdrulong_hex(ulong *newval, word index)
{	byte tmp[40];
	get_dedit(header_tree,index,tmp);
	*newval=(ulong)strtoul(tmp,NULL,16);
}

void sethdrulong_hex(word index, ulong num)
{	byte tmp[10];
	sprintf(tmp,"%lx",num);
	set_dtext(header_tree,index,tmp,FALSE,8);
}

void dia_header(long win)
{	FILE *fp;
	byte s[MAX_STR];
	byte stime[40];
	byte *cp;
	word ext_btn;
	long cur_time, days;

	if (wi[win].aktmsg>0)
	{	fp=wi[win].hdrfp;
		
		time(&cur_time);
		days=(cur_time-wi[win].hdr->lDate)/86400L;
		if (days>3600)
			days=-1;
		strcpy(stime,ctime(&wi[win].hdr->lDate));
		if ((cp=strchr(stime,'\n'))!=NULL)
			*cp=EOS;
		set_dtext(header_tree,MHAREA,gl_area[wi[win].aktarea].name,FALSE,15);
		sprintf(s,msgtxt[DIV_NUMBEROF].text,wi[win].aktmsg,wi[win].lastmsg);
		set_dtext(header_tree,MHMSG,s,FALSE,15);
		
		set_dtext(header_tree,MHFROM,wi[win].hdr->sFrom,FALSE,36);
		set_dtext(header_tree,MHTO,wi[win].hdr->sTo,FALSE,36);
		set_dtext(header_tree,MHTOPIC,wi[win].hdr->sSubject,FALSE,66);
		set_dtext(header_tree,MHTIME,wi[win].hdr->sDate,FALSE,19);
		set_dtext(header_tree,MHCREATE,ltoa(wi[win].hdr->lDate,s,10),FALSE,10);
		set_dtext(header_tree,MHCTIME,stime,FALSE,26);
		set_dtext(header_tree,MHDAYS,ltoa(days,s,10),FALSE,5);

		set_dtext(header_tree,MHMSTART,ltoa(wi[win].hdr->lStart,s,10),FALSE,8);
		sethdruword(MHSIZE,  wi[win].hdr->Size);

		sethdruword(MHUP,    wi[win].hdr->mid.upparent.up);
		sethdruword(MHPARENT,wi[win].hdr->mid.upparent.parent);
		sethdruword(MHREADS, wi[win].hdr->rep.readscost.reads);
		sethdruword(MHCOST,  wi[win].hdr->rep.readscost.cost);
		sethdrulong_hex(MHMID, wi[win].hdr->mid.midcrc);
		sethdrulong_hex(MHREP, wi[win].hdr->rep.repcrc);

		sethdruword_hex(MHFLAGS, wi[win].hdr->wFlags);
		sethdrulong_hex(MHMAIL0, wi[win].hdr->lMsgidcrc);
		sethdrulong_hex(MHMAIL2, wi[win].hdr->lReplycrc);
		sethdruword_hex(MHMAIL4, wi[win].hdr->MFlags);
		sethdruword_hex(MHMAIL5, wi[win].hdr->XFlags);
		sethdruword_hex(MHMAIL6, wi[win].hdr->free6);
		sethdruword_hex(MHMAIL7, wi[win].hdr->wProcessed);

		sethdruword(MHOZONE, wi[win].hdr->wFromZone);
		sethdruword(MHONET,  wi[win].hdr->wFromNet);
		sethdruword(MHONODE, wi[win].hdr->wFromNode);
		sethdruword(MHOPOINT,wi[win].hdr->wFromPoint);
		sethdruword(MHDZONE, wi[win].hdr->wToZone);
		sethdruword(MHDNET,  wi[win].hdr->wToNet);
		sethdruword(MHDNODE, wi[win].hdr->wToNode);
		sethdruword(MHDPOINT,wi[win].hdr->wToPoint);
		
		draw_dialog(header_tree, &header_dial);
		ext_btn=Form_do(&header_dial,0);
		del_dialog(&header_dial);
		del_state(header_tree,ext_btn,SELECTED,FALSE);

		if (ext_btn==MHOK)
		{
			biene();
			show_doing(DO_WRITEMSGHEADER);
			
			get_dedit(header_tree,MHTIME,wi[win].hdr->sDate);
			get_dedit(header_tree,MHFROM,wi[win].hdr->sFrom);
			get_dedit(header_tree,MHTO,wi[win].hdr->sTo);
			get_dedit(header_tree,MHTOPIC,wi[win].hdr->sSubject);

			gethdruword_hex(&(wi[win].hdr->wFlags),MHFLAGS);

			gethdrulong_hex(&(wi[win].hdr->lMsgidcrc),MHMAIL0);
			gethdrulong_hex(&(wi[win].hdr->lReplycrc),MHMAIL2);
			gethdruword_hex(&(wi[win].hdr->MFlags),MHMAIL4);
			gethdruword_hex(&(wi[win].hdr->XFlags),MHMAIL5);
			gethdruword_hex(&(wi[win].hdr->free6),MHMAIL6);
			gethdruword_hex(&(wi[win].hdr->wProcessed),MHMAIL7);

			gethdruword(&(wi[win].hdr->wFromZone),MHOZONE);
			gethdruword(&(wi[win].hdr->wFromNet),MHONET);
			gethdruword(&(wi[win].hdr->wFromNode),MHONODE);
			gethdruword(&(wi[win].hdr->wFromPoint),MHOPOINT);
			
			gethdruword(&(wi[win].hdr->wToZone),MHDZONE);
			gethdruword(&(wi[win].hdr->wToNet),MHDNET);
			gethdruword(&(wi[win].hdr->wToNode),MHDNODE);
			gethdruword(&(wi[win].hdr->wToPoint),MHDPOINT);

			if (wi[win].hdr->wFlags & F_DELETED)
				wi[win].msgda[wi[win].aktmsg]=FALSE;

			if (wi[win].aktmsg>0)
			{	hdrseek(win,fp,(wi[win].aktmsg-1),SEEK_SET);
				if (hdrwrite(win,wi[win].hdr, 1,fp)!=1)
					LangAlert(MSG_WRITEMSGHDRERROR);
			}		
			del_doing();
			mouse_normal();

			wi[win].oldmsg=0;
			select_msg(win, wi[win].aktmsg, TRUE, TRUE);
		}
	}
}






void sethdrflg(long win, word index, word flag)
{	if (wi[win].hdr->wFlags & flag)
		set_state(header2_tree,index,SELECTED,FALSE);
	else	
		del_state(header2_tree,index,SELECTED,FALSE);
}

void gethdrflg(long win, word index, word flag)
{	if (req_state(header2_tree,index,SELECTED))
		wi[win].hdr->wFlags |= flag;
	else
		wi[win].hdr->wFlags &= ~flag;
}

void setexthdrflg(long win, word index, uword flag)
{	if (wi[win].hdr->XFlags & flag)
		set_state(header2_tree,index,SELECTED,FALSE);
	else	
		del_state(header2_tree,index,SELECTED,FALSE);
}
	
void getexthdrflg(long win, word index, uword flag)
{	if (req_state(header2_tree,index,SELECTED))
		wi[win].hdr->XFlags |= flag;
	else
		wi[win].hdr->XFlags &= ~flag;
}
	
void setmaushdrflg(long win, word index, uword flag)
{	if (wi[win].hdr->MFlags & flag)
		set_state(header2_tree,index,SELECTED,FALSE);
	else	
		del_state(header2_tree,index,SELECTED,FALSE);
}
	
void getmaushdrflg(long win, word index, uword flag)
{	if (req_state(header2_tree,index,SELECTED))
		wi[win].hdr->MFlags |= flag;
	else
		wi[win].hdr->MFlags &= ~flag;
}

void dia_header2(long win)
{	FILE *fp;
	byte s[MAX_STR];
	word ext_btn,x,y;

	if (wi[win].aktmsg>0)
	{	fp=wi[win].hdrfp;
		
		if ( is_netmail(wi[win].aktarea))
			del_state(header2_tree,MHMAUS,DISABLED,FALSE);
		else
			set_state(header2_tree,MHMAUS,DISABLED,FALSE);

		set_dtext(header2_tree,MHAREA2,gl_area[wi[win].aktarea].name,FALSE,15);
		sprintf(s,msgtxt[DIV_NUMBEROF].text,wi[win].aktmsg,wi[win].lastmsg);
		set_dtext(header2_tree,MHMSG2,s,FALSE,15);
		
		sprintf(s,"%s > %s",wi[win].hdr->sFrom,wi[win].hdr->sTo);
		set_dtext(header2_tree,MHFROM2,s,FALSE,73);
		set_dtext(header2_tree,MHTOPIC2,wi[win].hdr->sSubject,FALSE,66);
		set_dtext(header2_tree,MHTIME2,wi[win].hdr->sDate,FALSE,19);

		sethdrflg(win,MHPVT,F_PRIVATE);
		sethdrflg(win,MHCRASH,F_CRASH);
		sethdrflg(win,MHRECV,F_RECEIVED);
		sethdrflg(win,MHKSENT,F_KILLSENT);
		sethdrflg(win,MHSENT,F_SENT);
		sethdrflg(win,MHFWD,F_INTRANSIT);
		sethdrflg(win,MHWFILE,F_FILEATTACH);
		sethdrflg(win,MHLOCAL,F_LOCAL);
		sethdrflg(win,MHDEL,F_DELETED);
		sethdrflg(win,MHHOLD,F_HOLD);
		sethdrflg(win,MHORPHAN,F_ORPHAN);
		sethdrflg(win,MHRET,F_RETRECREQ);
		sethdrflg(win,MHIS,F_ISRETREC);
		sethdrflg(win,MHAUDI,F_AUDITREQ);
		sethdrflg(win,MHRESERV,F_RESERVED);
		sethdrflg(win,MHFREQ,F_FILEREQ);
		
		setexthdrflg(win,MHIMMED,XF_IMMEDIATE);
		setexthdrflg(win,MHREAD,XF_READ);
		setexthdrflg(win,MHARCHIV,XF_ARCHIVSENT);
		setexthdrflg(win,MHTRUNC,XF_TRUNCFILESENT);
		setexthdrflg(win,MHKILL,XF_KILLFILESENT);
		setexthdrflg(win,MHDIRECT,XF_DIRECT);
		setexthdrflg(win,MHZONEG,XF_ZONEGATE);
		setexthdrflg(win,MHHOSTR,XF_HOSTROUTE);
		setexthdrflg(win,MHLOCK,XF_LOCK);
		setexthdrflg(win,MHFIXED,XF_FIXEDADDRESS);
		setexthdrflg(win,MHMAUS,XF_MAUSMSG);
		setexthdrflg(win,MHGATED,XF_GATED);
		setexthdrflg(win,MHCREAT,XF_CREATEFLOWFILE);
		setexthdrflg(win,MHSIGN,XF_SIGNATURE);
		setexthdrflg(win,MHMULTCC,XF_MULTIPLECC);
		setexthdrflg(win,MHRESX12,XF_RESERVED12);
		
		setmaushdrflg(win,MNICHT,MF_NICHTGELESEN);
		setmaushdrflg(win,MZURUECK,MF_ZURUECK);
		setmaushdrflg(win,MBEANT,MF_BEANTWORTET);
		setmaushdrflg(win,MGELESEN,MF_GELESEN);
		setmaushdrflg(win,MWEITER,MF_WEITER);
		setmaushdrflg(win,MMAUSNET,MF_MAUSNET);
		setmaushdrflg(win,MANGEKOM,MF_ANGEKOMMEN);
		setmaushdrflg(win,MGATEWAY,MF_GATEWAY);
		setmaushdrflg(win,MKOPIERT,MF_KOPIERT);
		setmaushdrflg(win,MUNBEK,MF_UNBEKANNT);
		setmaushdrflg(win,MTRANS,MF_MAUSTAUSCH);
		setmaushdrflg(win,MAINTER1,MF_INTERESTING1);
		setmaushdrflg(win,MAINTER2,MF_INTERESTING2);
		setmaushdrflg(win,MAHERE,MF_HEREDITARY);
		setmaushdrflg(win,MA14,MF_RESERVED14);
		setmaushdrflg(win,MA15,MF_RESERVED15);

		if ((wi[win].hdr->XFlags & XF_MAUSMSG) && is_netmail(wi[win].aktarea) )
			del_flag(header2_tree,MAUSBOX,HIDETREE,FALSE);
		else
			set_flag(header2_tree,MAUSBOX,HIDETREE,FALSE);
				
		draw_dialog(header2_tree, &header2_dial);
		do
		{	ext_btn=Form_do(&header2_dial,0);
			if (ext_btn==MHMAUS)
			{
				if (req_state(header2_tree,MHMAUS,SELECTED) && 
						is_netmail(wi[win].aktarea))
					del_flag(header2_tree,MAUSBOX,HIDETREE,FALSE);
				else
					set_flag(header2_tree,MAUSBOX,HIDETREE,FALSE);
				objc_offset(header2_tree,MAUSBOX,&x,&y);
				ObjcDraw(header2_tree,0,MAX_DEPTH,x,y,
											header2_tree[MAUSBOX].ob_width,
											header2_tree[MAUSBOX].ob_height);
			}
		}
		while (ext_btn!=MHOK2 && ext_btn!=MHCANC2);

		del_dialog(&header2_dial);
		del_state(header2_tree,ext_btn,SELECTED,FALSE);

		if (ext_btn==MHOK2)
		{
			biene();
			show_doing(DO_WRITEMSGHEADER);
			
			gethdrflg(win,MHPVT,F_PRIVATE);
			gethdrflg(win,MHCRASH,F_CRASH);
			gethdrflg(win,MHRECV,F_RECEIVED);
			gethdrflg(win,MHKSENT,F_KILLSENT);
			gethdrflg(win,MHSENT,F_SENT);
			gethdrflg(win,MHFWD,F_INTRANSIT);
			gethdrflg(win,MHWFILE,F_FILEATTACH);
			gethdrflg(win,MHLOCAL,F_LOCAL);
			gethdrflg(win,MHDEL,F_DELETED);
			gethdrflg(win,MHHOLD,F_HOLD);
			gethdrflg(win,MHORPHAN,F_ORPHAN);
			gethdrflg(win,MHRET,F_RETRECREQ);
			gethdrflg(win,MHIS,F_ISRETREC);
			gethdrflg(win,MHAUDI,F_AUDITREQ);
			gethdrflg(win,MHRESERV,F_RESERVED);
			gethdrflg(win,MHFREQ,F_FILEREQ);
			
			getexthdrflg(win,MHIMMED,XF_IMMEDIATE);
			getexthdrflg(win,MHREAD,XF_READ);
			getexthdrflg(win,MHARCHIV,XF_ARCHIVSENT);
			getexthdrflg(win,MHTRUNC,XF_TRUNCFILESENT);
			getexthdrflg(win,MHKILL,XF_KILLFILESENT);
			getexthdrflg(win,MHDIRECT,XF_DIRECT);
			getexthdrflg(win,MHZONEG,XF_ZONEGATE);
			getexthdrflg(win,MHHOSTR,XF_HOSTROUTE);
			getexthdrflg(win,MHLOCK,XF_LOCK);
			getexthdrflg(win,MHFIXED,XF_FIXEDADDRESS);
			getexthdrflg(win,MHMAUS,XF_MAUSMSG);
			getexthdrflg(win,MHGATED,XF_GATED);
			getexthdrflg(win,MHCREAT,XF_CREATEFLOWFILE);
			getexthdrflg(win,MHSIGN,XF_SIGNATURE);
			getexthdrflg(win,MHMULTCC,XF_MULTIPLECC);
			getexthdrflg(win,MHRESX12,XF_RESERVED12);
			
			getmaushdrflg(win,MNICHT,MF_NICHTGELESEN);
			getmaushdrflg(win,MZURUECK,MF_ZURUECK);
			getmaushdrflg(win,MBEANT,MF_BEANTWORTET);
			getmaushdrflg(win,MGELESEN,MF_GELESEN);
			getmaushdrflg(win,MWEITER,MF_WEITER);
			getmaushdrflg(win,MMAUSNET,MF_MAUSNET);
			getmaushdrflg(win,MANGEKOM,MF_ANGEKOMMEN);
			getmaushdrflg(win,MGATEWAY,MF_GATEWAY);
			getmaushdrflg(win,MKOPIERT,MF_KOPIERT);
			getmaushdrflg(win,MUNBEK,MF_UNBEKANNT);
			getmaushdrflg(win,MTRANS,MF_MAUSTAUSCH);
			getmaushdrflg(win,MAINTER1,MF_INTERESTING1);
			getmaushdrflg(win,MAINTER2,MF_INTERESTING2);
			getmaushdrflg(win,MAHERE,MF_HEREDITARY);
			getmaushdrflg(win,MA14,MF_RESERVED14);
			getmaushdrflg(win,MA15,MF_RESERVED15);

			if (wi[win].hdr->wFlags & F_DELETED)
				wi[win].msgda[wi[win].aktmsg]=FALSE;

			if (wi[win].aktmsg>0)
			{
				hdrseek(win,fp,(wi[win].aktmsg-1),SEEK_SET);
				if (hdrwrite(win,wi[win].hdr, 1,fp)!=1)
					LangAlert(MSG_WRITEMSGHDRERROR);
			}		
			
			del_doing();
			mouse_normal();

			wi[win].oldmsg=0;
			select_msg(win, wi[win].aktmsg, TRUE, TRUE);
		}
	}
}

void sethdrproflg(long win, word index, word flag)
{	if (wi[win].hdr->wProcessed & flag)
		set_state(header3_tree,index,SELECTED,FALSE);
	else	
		del_state(header3_tree,index,SELECTED,FALSE);
}

void gethdrproflg(long win, word index, word flag)
{	if (req_state(header3_tree,index,SELECTED))
		wi[win].hdr->wProcessed |= flag;
	else
		wi[win].hdr->wProcessed &= ~flag;
}

void dia_header3(long win)
{	FILE *fp;
	byte s[MAX_STR];
	word ext_btn;

	if (wi[win].aktmsg>0)
	{	fp=wi[win].hdrfp;
		
		set_dtext(header3_tree,MHAREA3,gl_area[wi[win].aktarea].name,FALSE,15);
		sprintf(s,msgtxt[DIV_NUMBEROF].text,wi[win].aktmsg,wi[win].lastmsg);
		set_dtext(header3_tree,MHMSG3,s,FALSE,15);
		
		sprintf(s,"%s > %s",wi[win].hdr->sFrom,wi[win].hdr->sTo);
		set_dtext(header3_tree,MHFROM3,s,FALSE,73);
		set_dtext(header3_tree,MHTOPIC3,wi[win].hdr->sSubject,FALSE,66);
		set_dtext(header3_tree,MHTIME3,wi[win].hdr->sDate,FALSE,19);

		sethdrproflg(win,PRO15,0x8000);
		sethdrproflg(win,PRO14,0x4000);
		sethdrproflg(win,PRO13,0x2000);
		sethdrproflg(win,PRO12,0x1000);
		sethdrproflg(win,PRO11,0x800);
		sethdrproflg(win,PRO10,0x400);
		sethdrproflg(win,PRO9,0x200);
		sethdrproflg(win,PRO8,0x100);
		sethdrproflg(win,PRO7,0x80);
		sethdrproflg(win,PRO6,0x40);
		sethdrproflg(win,PRO5,0x20);
		sethdrproflg(win,PRO4,0x10);
		sethdrproflg(win,PRO3,0x8);
		sethdrproflg(win,PRO2,0x4);
		sethdrproflg(win,PRO1,0x2);
		sethdrproflg(win,PRO0,0x1);

		draw_dialog(header3_tree, &header3_dial);
		do
		{	ext_btn=Form_do(&header3_dial,0);
		}
		while (ext_btn!=MHOK3 && ext_btn!=MHCANC3);

		del_dialog(&header3_dial);
		del_state(header3_tree,ext_btn,SELECTED,FALSE);

		if (ext_btn==MHOK3)
		{
			biene();
			show_doing(DO_WRITEMSGHEADER);
			
			gethdrproflg(win,PRO15,0x8000);
			gethdrproflg(win,PRO14,0x4000);
			gethdrproflg(win,PRO13,0x2000);
			gethdrproflg(win,PRO12,0x1000);
			gethdrproflg(win,PRO11,0x800);
			gethdrproflg(win,PRO10,0x400);
			gethdrproflg(win,PRO9,0x200);
			gethdrproflg(win,PRO8,0x100);
			gethdrproflg(win,PRO7,0x80);
			gethdrproflg(win,PRO6,0x40);
			gethdrproflg(win,PRO5,0x20);
			gethdrproflg(win,PRO4,0x10);
			gethdrproflg(win,PRO3,0x8);
			gethdrproflg(win,PRO2,0x4);
			gethdrproflg(win,PRO1,0x2);
			gethdrproflg(win,PRO0,0x1);

			if (wi[win].hdr->wFlags & F_DELETED)
				wi[win].msgda[wi[win].aktmsg]=FALSE;

			if (wi[win].aktmsg>0)
			{
				hdrseek(win,fp,(wi[win].aktmsg-1),SEEK_SET);
				if (hdrwrite(win,wi[win].hdr, 1,fp)!=1)
					LangAlert(MSG_WRITEMSGHDRERROR);
			}		
			
			del_doing();
			mouse_normal();

			wi[win].oldmsg=0;
			select_msg(win, wi[win].aktmsg, TRUE, TRUE);
		}
	}
}


void dia_fkeys(void)
{	word ext_btn;
	word gl_fk_obj[10] = {FK1,FK2,FK3,FK4,FK5,FK6,FK7,FK8,FK9,FK10};
	byte filename[MAX_STR];
	FILE *fp;
	word i,add=0,x,y;

	strcpy(fkeys_tree[FKNEXT].ob_spec.tedinfo->te_ptmplt,"\3 F11-F20");
	for (i=0;i<10;i++)
	{	sprintf(filename,"F%02u",i+add+1);
		set_dbutton(fkeys_tree,gl_fk_obj[i]-1,filename,FALSE);
		set_dtext(fkeys_tree,gl_fk_obj[i],gl_fkeys[i+add],FALSE,67);
	}

	draw_dialog(fkeys_tree, &fkeys_dial);
	do
	{	ext_btn=Form_do(&fkeys_dial,FK1);

		if (ext_btn==FKNEXT)
		{	for (i=0;i<10;i++)
				get_dedit(fkeys_tree,gl_fk_obj[i],gl_fkeys[i+add]);
			if (add)
			{	add=0;
				strcpy(fkeys_tree[FKNEXT].ob_spec.tedinfo->te_ptmplt,"\3 F11-F20");
			}
			else
			{	add=10;
				strcpy(fkeys_tree[FKNEXT].ob_spec.tedinfo->te_ptmplt,"\3 F 1-F10");
			}
			del_state(fkeys_tree,FKNEXT,SELECTED,TRUE);
			for (i=0;i<10;i++)
			{	sprintf(filename,"F%02u",i+add+1);
				set_dbutton(fkeys_tree,gl_fk_obj[i]-1,filename,FALSE);
				set_dtext(fkeys_tree,gl_fk_obj[i],gl_fkeys[i+add],FALSE,67);
			}
			objc_offset(fkeys_tree,FKBOX,&x,&y);
			ObjcDraw(fkeys_tree,FKBOX,MAX_DEPTH,x,y,
											fkeys_tree[FKBOX].ob_width,
											fkeys_tree[FKBOX].ob_height);
		}

		if (ext_btn==FKOK || ext_btn==FKSAVE)
		{	for (i=0;i<10;i++)
				get_dedit(fkeys_tree,gl_fk_obj[i],gl_fkeys[i+add]);
			if (ext_btn==FKSAVE)
			{	biene();
				sprintf(filename,"%sLED.FKY",gl_ledpath);
				if ((fp=fopen(filename,"wb"))!=NULL)
				{	fwrite(&gl_fkeys,sizeof(gl_fkeys),1L,fp);
					fclose(fp);
				}
				mouse_normal();
				del_state(fkeys_tree,ext_btn,SELECTED,TRUE);
			}	
		}
	} while (ext_btn==FKSAVE || ext_btn==FKNEXT);

	del_state(fkeys_tree,ext_btn,SELECTED,FALSE);
	del_dialog(&fkeys_dial);
}

void get_fkeys(void)
{	FILE *fp;
	byte filename[MAX_STR];
	word i;
	
	for (i=0;i<20;i++)
		*gl_fkeys[i]=EOS;

	show_doing(DO_READFUNCTIONKEYS);
	sprintf(filename,"%sLED.FKY",gl_ledpath);
	if ((fp=fopen(filename,"rb"))!=NULL)		/* war r+b */
	{	fread(&gl_fkeys,sizeof(gl_fkeys),1,fp);
		fclose(fp);	
	}
}

byte *kap_re(byte *s)
{	byte *cp;
	cp=s;
	while (*cp==' ') cp++;
	while (!strnicmp(cp,"Re:",3) ||
				 !strnicmp(cp,"Re^",3) )
	{	if (cp[2]==':')
		{	cp+=3;
		}
		else
		{	cp+=3;
			while (isdigit(*cp)) cp++;
			if (*cp==':')
				cp++;
		}
		while (*cp==' ') cp++;
	}	
	return cp;
}

void
del_ape(byte *s)
{	byte *cp;
	if (s==NULL)
		return;
	if ( (cp=strchr(s,'@'))!=NULL || (cp=strchr(s,'%'))!=NULL )
	{	cp--;
		while (*cp==' ')
			cp--;
		cp++;	
		*cp=EOS;
	}
}

void next_subj(long win, byte *origname)
{	word found, foundnr, msgnr;
	byte subj[MAX_STR];
	byte search[MAX_STR];
	HDR thdr;

	biene();
	strcpy(subj,kap_re(wi[win].hdr->sSubject));

	found=FALSE;
	foundnr=0;
	msgnr=wi[win].aktmsg;
	msgnr++;
	del_ape(origname);

	while (msgnr<=wi[win].lastmsg && msgnr>=wi[win].firstmsg)
	{
		hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
		hdrread(win,&thdr, 1, wi[win].hdrfp);
		if (!(thdr.wFlags & F_DELETED))
		{
			strcpy(search,kap_re(thdr.sSubject));			
			if (stricmp(search,subj)==0L)
			{
				if (origname==NULL) /* normale Suche */
				{
					found=TRUE;
					foundnr=msgnr;
					break;
				}
				else
				{
					del_ape(thdr.sTo);
					if (stricmp(thdr.sTo,origname)==0L)
					{
						found=TRUE;
						foundnr=msgnr;
						break;
					}
				}
			}
		}
		msgnr++;
	}
	mouse_normal();
	if (found)
		select_msg(win,foundnr,TRUE,TRUE);
}

void prev_subj(long win, byte *origname)
{
	word found, foundnr, msgnr;
	byte subj[MAX_STR];
	byte search[MAX_STR];
	HDR thdr;

	biene();
	strcpy(subj,kap_re(wi[win].hdr->sSubject));

	found=FALSE;
	foundnr=0;
	msgnr=wi[win].aktmsg;
	msgnr--;
	del_ape(origname);
	
	while (msgnr<=wi[win].lastmsg && msgnr>=wi[win].firstmsg)
	{
		hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
		hdrread(win,&thdr, 1, wi[win].hdrfp);
		if (!(thdr.wFlags & F_DELETED))
		{
			strcpy(search,kap_re(thdr.sSubject));			
			
			if (stricmp(search,subj)==0L)
			{
				if (origname==NULL) /* normale Suche */
				{
					found=TRUE;
					foundnr=msgnr;
					break;
				}
				else  			/* Suche nach Original-Msg */
				{
					del_ape(thdr.sFrom);					
					if (stricmp(thdr.sFrom,origname)==0L)
					{
						found=TRUE;
						foundnr=msgnr;
						break;
					}
				}
			}
		}
		msgnr--;
	}
	mouse_normal();
	if (found)
		select_msg(win,foundnr,TRUE,TRUE);
}

/*
** mode==2: Subject
**		 ==3: From person
*/
void del_rek_subject(long win)
{	word msgnr,mode;
	byte subj[MAX_STR];
	byte search[MAX_STR];
	HDR thdr;

	mode=LangAlert(MSG_DELSUBJECTPERSON);
	if (mode==1)
		return;

	biene();
	if (mode==2)
		strcpy(subj,kap_re(wi[win].hdr->sSubject));
	else
		strcpy(subj,wi[win].hdr->sFrom);

	msgnr=wi[win].lastmsg;
	if (!msgnr)
		return;
	
	while (msgnr>=wi[win].firstmsg)
	{
		hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
		hdrread(win,&thdr, 1, wi[win].hdrfp);
		if (!(thdr.wFlags & F_DELETED))
		{	if (mode==2)
				strcpy(search,kap_re(thdr.sSubject));
			else
				strcpy(search,thdr.sFrom);
			
			if (!stricmp(search,subj))
			{	thdr.wFlags |= F_DELETED;
				wi[win].msgda[msgnr]=FALSE;
				hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
				hdrwrite(win,&thdr,1,wi[win].hdrfp);
			}
		}
		msgnr--;
	}
	mouse_normal();
	select_msg(win,wi[win].aktmsg,TRUE,TRUE);

	wi[MSGSWIND].aktarea=-1;
	need_update_msglistwindow(win,RETURN);
}
