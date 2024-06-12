#include "ccport.h"
#include <time.h>
#include <process.h>
#include "led.h"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "winddefs.h"
#include "vars.h"
#include "language.h"
#include <magx.h>

typedef struct
{	byte *info;
	byte mode;
	void *pnt;
} OPT_LIST;

#define Clear_home()    Cconws("\33E")
#define Cur_on()        Cconws("\33e")
#define Cur_off()       Cconws("\33f")

static void set_fwdstr_1(long win,word area,byte *f_from,byte *f_fr,byte *f_to,uword Zone,uword Net,uword Node,uword Point);
static void set_fwdstr_2(long win,byte *f_by);
static void get_memory(long *gesamt,long *largest,word *blocks);
static void load_echolist(void);

word quit(void)
{	long shft_status;
	word endefl;

	endefl=get_optwind(TRUE);
	if (endefl)
	{	shft_status=Kbshift(-1);
		shft_status&=3;
		if (shft_status==0L && opt.saveopt) /* r/l Shift nicht gedrÅckt	*/
		{	show_doing(DO_SAVELEDOPTIONS);
			save_optled();
		}
		show_doing(DO_SAVELEASTREADS);
		save_lread(-1);
		if (*gl_echofile)
		{	show_doing(DO_SAVEECHOLIST);
			save_echolist();
		}
		show_doing(DO_SAVEDIALOGPOS);
		save_dialpos();
		del_doing();
		clear_cutbuff();
	}
	return(endefl);
}

void setze_raster(long i)				/* ssl 161294					*/
{	ulong temp,xmax,ymax;
	xmax=full.w+1;
	ymax=full.y+full.h+1;

	temp=(ulong)opt.winprm[i][0] << 15;
	temp+=xmax-1;
	opt.winprm[i][0]=(uword) (temp/xmax);

	temp=(ulong)opt.winprm[i][1] << 15;
	temp+=ymax-1;
	opt.winprm[i][1]=(uword) (temp/ymax);

	temp=(ulong)opt.winprm[i][2] << 15;
	temp+=xmax-1;
	opt.winprm[i][2]=(uword) (temp/xmax);

	temp=(ulong)opt.winprm[i][3] << 15;
	temp+=ymax-1;
	opt.winprm[i][3]=(uword) (temp/ymax);
}

word  get_optwind(word wclose)		/* ssl 171294 */
{	long win,window_on_top;
	word t,h;
	word endefl;
	word status_msgwindow=0;

	endefl=TRUE;
	for (win=0;win<TOTAL_WIND;win++)
		opt.winprm[win][6] = 0;		/* ssl 91294: Fensterstatus=geschlossen */

	if ((window_on_top=found_wind())>=MAX_WIND)
		window_on_top=-1;

	if (wi[AREAWIND].belegt)		/* ssl 91294: List/Areawindowparameter merken */
	{	wind_get(wi[AREAWIND].handle,WF_CURRXYWH,
				         &opt.winprm[AREAWIND][0],
				         &opt.winprm[AREAWIND][1],
				         &opt.winprm[AREAWIND][2],
				         &opt.winprm[AREAWIND][3]);
		setze_raster(AREAWIND);
		opt.winprm[AREAWIND][4]=wi[AREAWIND].aktarea;
/*		opt.winprm[AREAWIND][5]=wi[AREAWIND].aktmsg; */
		opt.winprm[AREAWIND][6]=1;	/* ssl 91294: Fensterstatus: offen */
		if (wclose)
			close_window(AREAWIND);
	}
	if (wi[MSGSWIND].belegt)		/* ssl 91294: List/Areawindowparameter merken */
	{	wind_get(wi[MSGSWIND].handle,WF_CURRXYWH,
				         &opt.winprm[MSGSWIND][0],
				         &opt.winprm[MSGSWIND][1],
				         &opt.winprm[MSGSWIND][2],
				         &opt.winprm[MSGSWIND][3]);
		setze_raster(MSGSWIND);
		opt.winprm[MSGSWIND][4]=wi[MSGSWIND].aktarea;
/*		opt.winprm[MSGSWIND][5]=wi[MSGSWIND].aktmsg; */
		status_msgwindow=1;					/* Erst spÑter setzen, sonst wird es bei EditMsg->Save wieder geîffnet */
		if (wclose)
			close_window(MSGSWIND);
	}

	for (win=0;win<MAX_WIND;win++)
	{	if (wi[win].belegt)
		{	if (!wi[win].iconify)
			{	wind_get(wi[win].handle,WF_CURRXYWH,
				         &opt.winprm[win][0],	&opt.winprm[win][1],
				         &opt.winprm[win][2],	&opt.winprm[win][3]);
			}
			else
			{	wind_calc(WC_BORDER,wi[win].gadgets,
											wi[win].xb,wi[win].yb,wi[win].wb,wi[win].hb,
											&(word)opt.winprm[win][0],&(word)opt.winprm[win][1],
											&(word)opt.winprm[win][2],&(word)opt.winprm[win][3]);
			}
			setze_raster(win);
			opt.winprm[win][4]=wi[win].aktarea;
/*			opt.winprm[win][5]=wi[win].aktmsg; */
			opt.winprm[win][6]=1;			/* ssl 91294: Fensterstatus: offen */
			if (wclose)
			{	endefl=close_window(win);
				if (endefl==FALSE)
					return endefl; /* 040895 war: break; */
			}
		}
		else
		{	for (t=4;t<MAX_WINOPT;t++)
				opt.winprm[win][t]=0;			/* Die Windowausmaûe beibehalten */
		}
	}

	opt.winprm[MSGSWIND][6]=status_msgwindow;

	if (wclose)
	{	while ((win=found_wind())>=0)
		{	LangAlert(MSG_STUPIDWINDOW);
			endefl=close_window(win);
			break;
		}
	}

	/* window_on_top als Default-Window nach wi[0] */
	if (window_on_top>=1)
		for (t=0;t<MAX_WINOPT;t++)
		{	h=opt.winprm[0][t];
			opt.winprm[0][t]=opt.winprm[window_on_top][t];
			opt.winprm[window_on_top][t]=h;
		}

	return (endefl);
}

void execute(byte *file, byte *cmd, word sh_fl)
{	struct ffblk dta;
	byte *cp;
	long win;
	word ext_btn;
	static byte path[MAX_STR]="\0";
	static byte ttpcmd[MAX_STR]="\0";
	
	win=found_wind();
	if ((file!=NULL && findfirst(file,&dta,0)==0) || 
	    hole_file(path,NULL,"*.*",msgtxt[FILE_STARTPRG].text))
	{
		get_optwind(TRUE);
		if (file!=NULL)
			strcpy(path,file);
		
		if ((cp=strrchr(path, '.'))==NULL)
			return;
		cp++;	
		if (!strnicmp(cp,"TTP",3) || !strnicmp(cp,"TOS",3) ||
				!strnicmp(cp,"PRG",3) || !strnicmp(cp,"APP",3) ||
				!strnicmp(cp,"GTP",3))
		{		
			if (cmd!=NULL)
				strcpy(ttpcmd,cmd);
			else if ((strstr(path,".TTP") || strstr(path,".GTP")) && !sh_fl)
			{
				set_dtext(edline_tree,ELTEXT,msgtxt[DIV_EDITLINECMD].text,FALSE,6);
				set_dtext(edline_tree,ELLINE,ttpcmd,FALSE,66);
				draw_dialog(edline_tree, &edline_dial);
				ext_btn=Form_do(&edline_dial,ELLINE);
				del_state(edline_tree,ext_btn,SELECTED,FALSE);
				del_dialog(&edline_dial);
				if (ext_btn==ELOK)
					get_dedit(edline_tree,ELLINE,ttpcmd);
			}
			else
				*ttpcmd=EOS;
			program(path,ttpcmd);
			mouse_normal();
		}	
		if (win>=0)
			open_defltwind();
	}
}

void program(byte *prg, byte *cmd)
{	struct ffblk dta;
	COMMAND com;
	word err;
	word whndl;
	byte *cp;
	byte tmp[MAX_STR];
	
	biene();
	
	err=findfirst(prg,&dta,0);
	if (err!=0 || (long)Malloc(-1) < dta.ff_fsize+16384L)
	{
		LangAlert(MSG_NOMEMOTHERPRG);
		mouse_normal();
		return;
	}
	
	save_lread(-2);		/* Keine Flags lîschen! */

	if (*gl_echofile)
	{	show_doing(DO_SAVEECHOLIST);
		save_echolist();
	}

	strcpy(tmp,prg);
	strupr(tmp);
	if (*(tmp+1)==':')
		Dsetdrv(*tmp-'A');
	cp=strrchr(tmp,'\\');
	
	if (cp!=NULL)
	{
	 	if (cp-tmp>=3)	/* C:\DIR\SUPER.PRG */
			*cp=EOS;
		else			/* C:\SUPER.PRG */
			*++cp=EOS;
	}
	
	if (Dsetpath(tmp)!=0)
	{
		Dsetdrv(*gl_startpath-'A');
		return;
	}
	
	hide_mouse();
	if (aes_version>=0x140)
		wind_new();
	else
	{
		do
		{
			wind_get(0,WF_TOP,&whndl,&err,&err,&err);
			if (whndl>0)
				wind_close(whndl);
		} while (whndl>0);
	}
	redraw_all();
	strcpy(tmp,prg);
	strupr(tmp);
	if ((cp=strrchr(tmp,'\\'))!=NULL || (cp=strchr(tmp,':'))!=NULL)
	{
		set_dtext(exec_tree,EXETITLE,cp+1,FALSE,12);
		objc_draw(exec_tree,ROOT,MAX_DEPTH,exec_tree->ob_x,exec_tree->ob_y,
		          exec_tree->ob_width,exec_tree->ob_height);
	}
	show_mouse();
	if (strstr(tmp,".TTP")!=NULL || strstr(tmp,".TOS")!=NULL)
	{
		hide_mouse();
		Clear_home();
		Cur_on();
	}
	
	speicher_freigeben();
	if (vq_gdos())
		vst_unload_fonts(handle,0);
	v_clsvwk(handle);
	ende();
	
	com.length=(byte)strlen(cmd);
	strcpy(com.command_tail,cmd);
	Pexec(0,prg,&com,NULL);
	
	while (Cconis())
		Cnecin();
	
	if (open_vwork())
	{
		v_show_c(handle,0);
		biene();
		Cur_off();
		if (aes_version>=0x140)
			wind_new();
		vswr_mode(handle,MD_REPLACE);
		vst_color(handle,BLACK);
		vsf_interior(handle,1);
		vsf_color(handle,WHITE);
		vsf_style(handle,0);
		vsf_perimeter(handle,FALSE);
		set_desktop();
		set_menu(MSG_WIND);
		menu_bar(menu_tree, 1);
		set_msgfnt();
		init_cutbuff();
		s_cutbuff=-1;
		e_cutbuff=-1;
		
		strcpy(tmp,gl_startpath);
		if (*(tmp+1)==':')
			Dsetdrv(*tmp-'A');
		Dsetpath(tmp);

		sprintf(tmp,"%sLED.CFG",gl_ledpath);
		if (findfirst(tmp,&dta,0))
		{	
#if 0
			sprintf(tmp,"%sFIDOSOFT.CFG",gl_fidosoftpath);
			if (findfirst(tmp,&dta,0))
#endif
				sprintf(tmp,"%sBINKLEY.CFG",gl_binkleypath);
		}

		show_doing(DO_READCONFFILE);
		config_init(tmp);

		if (*gl_echofile)
		{	show_doing(DO_LOADECHOLIST);
			load_echolist();
		}

		show_doing(DO_GETLASTREADS);
		load_lread();
		del_doing();
		mouse_normal();

		if (gl_options & OUTBOUND36)
			if (!available(msgtxt[DO_OUTBOUND36].text))
				gl_options &= ~OUTBOUND36;
	}
	else
	{	LangAlert(MSG_GEMHANDLEERROR);
		exit(1);
	}
}

static void get_memory(long *gesamt,long *largest,word *blocks)
{	void *buffer[100];
	long frei;
	word i;
	*blocks=0;
	*gesamt=*largest=0;
	do
	{	frei = (long) Malloc(-1L);
		if (frei > 0)
		{	buffer[(*blocks)++] = Malloc(frei);
			*gesamt += frei;
			*largest=max(*largest,frei);
		}
	}
	while (frei != 0);
	i = *blocks;
	while (i > 0)
		if (Mfree(buffer[--i])!=0)
			LangAlert(MSG_DEALLOCATEMEM);
}

void dia_about(void)
{	char tmp[40];
	word ext_btn, info_btn, blocks;
	long gesamt,largest;

	get_memory(&gesamt,&largest,&blocks);
	
	set_dtext(about_tree,ADATUM,__DATE__,FALSE,11);
	sprintf(tmp,"%ld",gesamt);
	set_dtext(about_tree,AFREE,tmp,FALSE,8);
	sprintf(tmp,"%d BLOCKS, LARGEST BLOCK %lu",blocks,largest);
	set_dtext(about_tree,ABLOCK,tmp,FALSE,33);
	set_dtext(about_tree,AAUTOR1,autor1,FALSE,15);
	set_dtext(about_tree,AAUTOR2,autor2,FALSE,11);
	set_dtext(about_tree,AAUTOR3,autor3,FALSE,17);
	set_dtext(about_tree,AVERSART,versart,FALSE,22);

	test_registered();
	if (opt.led_register!=0)
		sprintf(tmp,"%lX",opt.led_register);
	else
		*tmp=EOS;
	set_dtext(about_tree,ABOUTREG,tmp,FALSE,8);

	sprintf(tmp,"LED Version %s%s",version,
									is_registered ? "" : unreg);
	set_dstring(about_tree,AVERSION,tmp,FALSE,22);
	
#if 0
	ext_btn=(about_tree[AVERSION].ob_width/gl_syswchar) >> 1;
	about_tree[AVERSION].ob_x = (ext_btn+(is_registered?3:0))*gl_syswchar;
#endif
	about_tree[AVERSION].ob_x=(about_tree[ROOT].ob_width-about_tree[AVERSION].ob_width) >> 1;

	strcpy(tmp,akasysop[0]);
	set_dtext(about_tree,ABOUTNAM,tmp,FALSE,28);

	mouse_normal();
	show_mouse();
	draw_dialog(about_tree, &about_dial);
	for (;;)
	{
		ext_btn=Form_do(&about_dial,0);
		if (ext_btn==AINFO)
		{
			draw_dialog(info_tree, &info_dial);
			info_btn=Form_do(&info_dial,0);
			del_state(info_tree,info_btn,SELECTED,FALSE);
			del_dialog(&info_dial);
			del_state(about_tree,ext_btn,SELECTED,TRUE);
		}
		else
			break;
	}
	get_dedit(about_tree,ABOUTREG,tmp);
	if (*tmp!=EOS)
	{	opt.led_register=strtoul(tmp,NULL,16);
		test_registered();
		if (!is_registered)
		{	LangAlert(MSG_WRONGKEY);
			opt.led_register=0;
		}
	}
	else
		opt.led_register=0;

	del_dialog(&about_dial);
	del_state(about_tree,ext_btn,SELECTED,FALSE);
} 

void undelete(long win, word msgnr)
{	HDR thdr;

	show_doing(DO_UNDELETEMSG);	
	hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
	hdrread(win,&thdr,1,wi[win].hdrfp);
	thdr.wFlags&=~F_DELETED;
	wi[win].msgda[msgnr]=TRUE;
	hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
	hdrwrite(win,&thdr,1,wi[win].hdrfp);	
	del_doing();
}

void msg_list(long win)
{
	word lmsg;
	word i;
	word oldselobj;
	word ext_btn;
	word msgnr;
	byte exttmp[MAX_STR];
	byte stmp[8];

	lmsg=wi[win].aktmsg;
	if (lmsg>0 || (lmsg==0 && wi[win].lastmsg>0))
	{
		if (lmsg==0)
			lmsg=1;
		else
		{
			if (wi[win].lastmsg-wi[win].aktmsg<MAX_LIST)
				lmsg=wi[win].lastmsg-(MAX_LIST-1);
			else
				lmsg=wi[win].aktmsg;
			if (lmsg<wi[win].firstmsg)
				lmsg=wi[win].firstmsg;
		}
		
		inhalt_listen(win,lmsg,&oldselobj,FALSE);
		set_dtext(listmsg_tree,LAREA,wi[win].areaname,FALSE,15);
		set_dtext(listmsg_tree,LMSGANF,itoa(wi[win].firstmsg,stmp,10),FALSE,4);
		set_dtext(listmsg_tree,LMSGEND,itoa(wi[win].lastmsg,stmp,10),FALSE,4);
		
		draw_dialog(listmsg_tree, &listmsg_dial);
		ext_btn=do_lform(win,oldselobj,lmsg);
		del_dialog(&listmsg_dial);
		del_state(listmsg_tree,ext_btn,SELECTED,FALSE);
		msgnr=wi[win].aktmsg;
		if (ext_btn!=LICANCEL)
		{
			for (i=0;i<MAX_LIST;i++)
			{
				if (ext_btn == gl_list_obj[i])
				{
					get_dedit(listmsg_tree,gl_list_obj[i],exttmp);
					exttmp[4]=EOS;
					msgnr=atoi(exttmp);
					if (req_state(listmsg_tree,gl_list_obj[i],DISABLED))
					{
						undelete(win,msgnr);
						berechne_msgp(win);
		     	}
		          	break;
				}
			}
		}
		if (msgnr!=wi[win].aktmsg)
			select_msg(win,msgnr,TRUE,TRUE);
	}
}


void msg_kill(long win, word heal, word multiple)
{	HDR hdr;
	word i, ext_btn;
	word absolute;
	word anfmsg,endmsg,msgnr;
	uword flags;
#if defined(__NEWFIDO__)
	ulong ext_flags;
#else
	uword ext_flags;
#endif

	byte anftmp[6], endtmp[6];

	if (wi[win].aktmsg>0 || (heal && wi[win].lastmsg>0))
	{	ext_flags = 0;		/* ssl 251294 */
		absolute=FALSE;
		msgnr=wi[win].aktmsg;

		if (msgnr<=0)
			msgnr=1;
		*anftmp=EOS;
		*endtmp=EOS;
		sprintf(anftmp,"%d",msgnr);
		sprintf(endtmp,"%d",msgnr);
		set_dtext(killmsg_tree,ANFKILL,anftmp,FALSE,4);
		set_dtext(killmsg_tree,ENDKILL,endtmp,FALSE,4);
		del_state(killmsg_tree,KABSOLUT,SELECTED,FALSE);

		flags=F_DELETED;
		if (multiple)
		{	flags=0;
			if (heal)
				set_dstring(killmsg_tree,KTITLE1,msgtxt[SYSTEM_DELFLAG].text,FALSE,11);
			else
				set_dstring(killmsg_tree,KTITLE1,msgtxt[SYSTEM_SETFLAG].text,FALSE,11);
			draw_dialog(killmsg_tree, &killmsg_dial);
			ext_btn=Form_do(&killmsg_dial,ANFKILL);
			del_dialog(&killmsg_dial);
			del_state(killmsg_tree,ext_btn,SELECTED,FALSE);
			
			if (req_button(killmsg_tree,KDEL))
				flags|=F_DELETED;
			if (req_button(killmsg_tree,KSENT))
				flags|=F_SENT;
			if (req_button(killmsg_tree,KKSENT))
				flags|=F_KILLSENT;
			if (req_button(killmsg_tree,KCRASH))
				flags|=F_CRASH;
			if (req_button(killmsg_tree,KHOLD))
				flags|=F_HOLD;
			if (req_button(killmsg_tree,KFWD))
				flags|=F_INTRANSIT;
			if (req_button(killmsg_tree,KPVT))
				flags|=F_PRIVATE;
			if (req_button(killmsg_tree,KORPHAN))
				flags|=F_ORPHAN;
			if (req_button(killmsg_tree,KWITHF))
				flags|=F_FILEATTACH;
			if (req_button(killmsg_tree,KLOCAL))
				flags|=F_LOCAL;
			if (req_button(killmsg_tree,KRECEIVE))
				flags|=F_RECEIVED;
			if (req_button(killmsg_tree,KRET))
				flags|=F_RETRECREQ;
			if (req_button(killmsg_tree,KIS))
				flags|=F_ISRETREC;
			if (req_button(killmsg_tree,KAUDI))
				flags|=F_AUDITREQ;
			if (req_button(killmsg_tree,KREAD))
				ext_flags|=XF_READ;	
			if (req_button(killmsg_tree,KARCHIVS))
				ext_flags|=XF_ARCHIVSENT;	
			if (req_button(killmsg_tree,KTRUNCFS))
				ext_flags|=XF_TRUNCFILESENT;	
			if (req_button(killmsg_tree,KKILLFS))
				ext_flags|=XF_KILLFILESENT;	
			if (req_button(killmsg_tree,KDIRECT))
				ext_flags|=XF_DIRECT;	
			if (req_button(killmsg_tree,KZONEG))
				ext_flags|=XF_ZONEGATE;	
			if (req_button(killmsg_tree,KHOSTR))
				ext_flags|=XF_HOSTROUTE;	
			if (req_button(killmsg_tree,KLOCK))
				ext_flags|=XF_LOCK;	
			if (req_button(killmsg_tree,KFIXED))
				ext_flags|=XF_FIXEDADDRESS;	
			if (req_button(killmsg_tree,KMAUSMSG))
				ext_flags|=XF_MAUSMSG;
					
						
			absolute=req_button(killmsg_tree,KABSOLUT);
		}
		if (ext_btn==KOK || !multiple)
		{
			get_dedit(killmsg_tree,ANFKILL,anftmp);
			get_dedit(killmsg_tree,ENDKILL,endtmp);
			anfmsg=atoi(anftmp);
			if (anfmsg<=0)
				anfmsg=1;
			endmsg=atoi(endtmp);
			if (endmsg<=0)
				endmsg=1;
			if (anfmsg > endmsg)
			{
				i=anfmsg;
				anfmsg=endmsg;
				endmsg=i;
			}
			if (anfmsg < wi[win].firstmsg)
				anfmsg=wi[win].firstmsg;
			if (endmsg > wi[win].lastmsg)
				endmsg=wi[win].lastmsg;
			
			while (Cconis())
				Cnecin();
			
			msgnr=anfmsg;
			biene();
			while (msgnr <= endmsg)
			{
				if (multiple)
				{	wi[win].aktmsg=msgnr;
					need_update_msglistwindow(win,RETURN);
				}

				if (Cconis())
				{	while (Cconis())
						Cnecin();
					throw_evntaway();
					break;
				}
				hdrseek(win, wi[win].hdrfp, (msgnr-1), SEEK_SET);
				hdrread(win, &hdr, 1, wi[win].hdrfp);
				if (!(hdr.wFlags & F_DELETED) && !heal)
				{
					/* clear DELETED, MSGHOLD, KILLSEND, MSGFWD, */
					/*       SENT, CRASH, PRIVATE, ORPHAN, READ */
					if (absolute)
					{
						hdr.wFlags &= 0x7D14;
						hdr.XFlags=0;
					}	
					
					hdr.wFlags |= flags;
					hdr.XFlags |= ext_flags;
					if (flags & F_DELETED)
						wi[win].msgda[msgnr]=FALSE;
					hdrseek(win, wi[win].hdrfp,(msgnr-1), SEEK_SET);
					hdrwrite(win, &hdr, 1, wi[win].hdrfp);
				}
				else if (heal)
				{
					if (absolute)
					{
						/* clear DELETED, HOLD, KILLSENT, INTRANSIT, */
						/*       SENT, CRASH, PRIVATE, ORPHAN, READ */
						hdr.wFlags &= 0x7D14;
						hdr.wFlags |= flags;
						hdr.XFlags = ext_flags;
					}
					else
					{
						hdr.wFlags &= ~flags;
						hdr.XFlags &= ~ext_flags;
					}	
					
					if (flags & F_DELETED)
						wi[win].msgda[msgnr]=TRUE;
					hdrseek(win, wi[win].hdrfp,(msgnr-1), SEEK_SET);
					hdrwrite(win, &hdr, 1, wi[win].hdrfp);
				}
				
				msgnr++;
			}

			if (multiple)
			{	wi[win].aktmsg=msgnr-1;
				wi[win].oldmsg=0;
				need_update_msglistwindow(win,RETURN);
			}

			mouse_normal();
			berechne_msgp(win);
#if 0
			wi[win].oldmsg=0;
#endif
			select_msg(win,wi[win].aktmsg,TRUE,TRUE);
			need_update_msglistwindow(win,RETURN);
		}
	}
}


/*
**	Wenn flag=0, dann darf ^CHRS geschrieben werden!
*/

static void del_controlinfo(long win,FILE *msgfp,word flag)
{	word i,write;
	byte *cp;
	i=0;
	cp=wi[win].lines[i];
	while (cp!=NULL && *cp=='\01')
	{	write=FALSE;
		if (!strncmp(cp,chrs_line,6) && flag==0)
			write=TRUE;

		if (write)
		{
			fputs(cp,msgfp);
			if (strchr(cp,'\n')==NULL)
				fputc('\n',msgfp);
			del_line(win,i,FALSE);
			i--;
		}
		else
		{	del_line(win,i,FALSE);
			i--;
		}
		i++;
		cp=wi[win].lines[i];
	}
}

static word waehle_zielarea(word aktmsg,word *start,word *ende,word *area,
														word *keep,word *fwd)
{	byte anftmp[8], endtmp[8];
	word ext_btn,i;
	set_flag(allarea_tree,ATITEL,HIDETREE,FALSE);
	del_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
	del_mstate(allarea_tree,SELECTED,FALSE,AMK,AMF,-1);

	if (opt.movemsg & O_MOVEMSG_KEEP)		set_state(allarea_tree,AMK,SELECTED,FALSE);
	if (opt.movemsg & O_MOVEMSG_FORWD)	set_state(allarea_tree,AMF,SELECTED,FALSE);

	*anftmp=
	*endtmp=EOS;
	sprintf(anftmp,"%d",aktmsg);
	sprintf(endtmp,"%d",aktmsg);
	set_dtext(allarea_tree,AMOVEANF,anftmp,FALSE,4);
	set_dtext(allarea_tree,AMOVEEND,endtmp,FALSE,4);
	ext_btn=do_aform(area,TRUE);
	del_state(allarea_tree,ext_btn,SELECTED,FALSE);
	del_dialog(&allarea_dial);
	opt.movemsg=0;
	if ((*keep  =req_button(allarea_tree,AMK))==TRUE)	opt.movemsg|=O_MOVEMSG_KEEP;
	if ((*fwd   =req_button(allarea_tree,AMF))==TRUE)	opt.movemsg|=O_MOVEMSG_FORWD;
	get_dedit(allarea_tree,AMOVEANF,anftmp);
	get_dedit(allarea_tree,AMOVEEND,endtmp);
	*start=atoi(anftmp);
	*ende =atoi(endtmp);
	if (*start > *ende)
	{	i=*start;
		*start=*ende;
		*ende=i;
	}
	set_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
	del_flag(allarea_tree,ATITEL,HIDETREE,FALSE);
	if (ext_btn!=AMCAN)
		return(TRUE);
	return(FALSE);
}		

static word own_address(uword zone,uword net,uword node,uword point)
{	word i;
	for (i=0;i<=addrcount;i++)
		if (addr[i].zone==zone && addr[i].net==net &&
				addr[i].node==node && addr[i].point==point)
			return(TRUE);
	return(FALSE);
}

void move_msg(long win)
{	FILE *msgfp, *hdrfp;
	long msgfilelen, hdrfilelen, hdrnum;
	word old_aai, i, forwfl, keepcpy, gl_aai;
	word anfmsg, endmsg, aktmsg;
	uword Zone,Net,Node,Point;
	long ltemp;
	byte tmp[MAX_STR];
	byte to_neu[MAX_STR];
	byte f_from[MAX_STR];
	byte f_by[MAX_STR];
	byte f_to[MAX_STR];
	byte f_fr[MAX_STR];
	byte snode[40];
	struct ffblk dta;
	
	if (wi[win].aktmsg > 0)
	{	aktmsg=wi[win].aktmsg;

		/* alte (Quell-)Area in old_aai */
		old_aai=gl_aai=wi[win].aktarea;

		i=waehle_zielarea(aktmsg,&anfmsg,&endmsg,&gl_aai,
											&keepcpy,&forwfl);

		if (i && gl_aai!=old_aai)
		{	
			if (is_netmail(gl_aai) && !forwfl)
			{	
				i=LangAlert(MSG_CHECKADDRESSES);
				if (i==1)
/*<=*/		return;
			}

			if (anfmsg<wi[win].firstmsg)	anfmsg=wi[win].firstmsg;
			if (endmsg>wi[win].lastmsg)		endmsg=wi[win].lastmsg;
			
			if (forwfl)
				if (!forw_dial(gl_aai,to_neu,snode))
					return;
			
			wi[win].oldmsg=0;
			select_msg(win,wi[win].aktmsg,TRUE,TRUE);
			
			biene();
			close_allarea(gl_aai);		/* ZielArea schlieûen */

			sprintf(tmp, "%s.MSG", gl_area[gl_aai].path);
			if ((msgfp=fopen(tmp, "r+b"))==NULL) msgfp=fopen(tmp, "w+b");
			if (!findfirst(tmp,&dta,0))	msgfilelen=dta.ff_fsize;
				
			sprintf(tmp, "%s.%s", gl_area[gl_aai].path, HEADEREXT);
			if (msgfilelen==0 || (hdrfp=fopen(tmp, "r+b"))==NULL)	hdrfp=fopen(tmp, "w+b");
			if (!findfirst(tmp,&dta,0))	hdrfilelen=dta.ff_fsize;
			
			if (msgfp==NULL || hdrfp==NULL)
			{	LangAlert(MSG_MOVEINVALIDAREA);
				if (msgfp!=NULL)	fclose(msgfp);
				if (hdrfp!=NULL)	fclose(hdrfp);
				mouse_normal();
				return;
			}

			wi[win].aktmsg=anfmsg;
			strcpy(wi[win].areaname,gl_area[gl_aai].name);
			
			hdrnum=hdrfilelen/sizeof(HDR);
			fseek(hdrfp,(long)hdrnum*sizeof(HDR),SEEK_SET);
			fseek(msgfp,0L,SEEK_END);

			while (Cconis()) Cnecin();
			while (wi[win].aktmsg<=endmsg && wi[win].aktmsg>=anfmsg)
			{	if (Cconis())
				{	while (Cconis()) Cnecin();
					throw_evntaway();
					break;
				}
				
				if (wi[win].msgda[wi[win].aktmsg]!=TRUE)
				{	wi[win].aktmsg++;
					continue;
				}
				wi[win].oldmsg=0;
				select_msg(win,wi[win].aktmsg,FALSE,TRUE);
				
				if (keepcpy)
				{	if (forwfl)	sprintf(tmp,msgtxt[DO_CFMSG].text,wi[win].aktmsg);
					else				sprintf(tmp,msgtxt[DO_CMSG].text,wi[win].aktmsg);
				}	
				else
				{	if (forwfl)	sprintf(tmp,msgtxt[DO_MFMSG].text,wi[win].aktmsg);
					else				sprintf(tmp,msgtxt[DO_MMSG].text,wi[win].aktmsg);
				}	
				show_doingtext(tmp);
				
				if (!keepcpy)	/* Delete old Msg? */
				{	wi[win].hdr->wFlags |= F_DELETED;
					hdrseek(win,wi[win].hdrfp,(wi[win].aktmsg-1),SEEK_SET);
					if (hdrwrite(win,wi[win].hdr,1,wi[win].hdrfp)!=1L)
/*<=*/			break;
					wi[win].msgda[wi[win].aktmsg]=FALSE;
				}

				gl_area[gl_aai].flag |= AR_EDITED;

/******* Mail wurde ueber "select_msg" gelesen ********/

				wi[win].hdr->wFlags=0;   	 /* Loesche Flags 				*/
				if (is_netmail(gl_aai))   /* Msg kommt in Netmail	*/
					wi[win].hdr->wFlags |= (F_KILLSENT|F_PRIVATE|F_LOCAL);
				else
				{	wi[win].hdr->wFlags |= F_LOCAL;
					if (gl_area[wi[win].aktarea].flag & AR_FORCEPVT)
						wi[win].hdr->wFlags |= F_PRIVATE;
				}
				/* Msgidcrc und Replycrc nicht lîschen! */
				wi[win].hdr->XFlags=wi[win].hdr->MFlags=0;
				wi[win].hdr->free6 =wi[win].hdr->wProcessed =0;
				set_if_fixed_address(win,gl_aai);

/******* MOVE/COPY-MSG ********************************/

				if (!forwfl)
				{	wi[win].msgbuf=(char *)calloc(1,wi[win].hdr->Size+2);
					if (wi[win].msgbuf!=NULL)
					{	fseek(wi[win].msgfp,wi[win].hdr->lStart,SEEK_SET);
						if (fread(wi[win].msgbuf,wi[win].hdr->Size,1L,wi[win].msgfp)!=1L)
						{	LangAlert(MSG_MOVEMSGTEXTERROR);
/*<=*/				break;
						}
					}

					if ((wi[win].hdr->lStart=ftell(msgfp))<0)
					{	LangAlert(MSG_MOVESTARTERROR);
						free(wi[win].msgbuf);
/*<=*/			break;
					}

					if (fwrite(wi[win].msgbuf,wi[win].hdr->Size,1L,msgfp)!=1L)
					{	LangAlert(MSG_MOVEWRITETEXTERROR);
						free(wi[win].msgbuf);
/*<=*/			break;
					}
					free(wi[win].msgbuf);

					if (fwrite(wi[win].hdr,sizeof(HDR),1,hdrfp)!=1L)
					{	LangAlert(MSG_MOVEWRITEHDRERROR);
/*<=*/			break;
					}
				}

/******* MOVE/COPY-MSG WITH FORWARD *******************/

				else
				{	
					if ((wi[win].hdr->lStart=ftell(msgfp))<0)
					{	LangAlert(MSG_MOVESTARTERROR);
/*<=*/			break;
					}

/*************** ORIGIN AUSLESEN UND FWD-TEXT SETZEN ****************/

					time(&wi[win].hdr->lDate);
					if (!is_netmail(old_aai))
						if (!get_origin_address(win,&Zone,&Net,&Node,&Point,TRUE))
							Zone = Net = Node = Point = 0;

					set_fwdstr_1(win,old_aai,f_from,f_fr,f_to,Zone,Net,Node,Point);
					strcpy(wi[win].hdr->sFrom,gl_ptowner);
					strcpy(wi[win].hdr->sTo,to_neu);

/************************ Eigene-Adresse setzen *********************/

#if 0
				 	wi[win].hdr->wFromZone=Zone;
					wi[win].hdr->wFromNet=Net;
					wi[win].hdr->wFromNode=Node;
					wi[win].hdr->wFromPoint=Point;
#endif

					if (strlen(snode)>0L)		/* Nur in Netmail! */
						nnstr2node(snode,(uword *)&wi[win].hdr->wToZone,(uword *)&wi[win].hdr->wToNet,
						 								 (uword *)&wi[win].hdr->wToNode,(uword *)&wi[win].hdr->wToPoint);
					else
					{	wi[win].hdr->wToZone =gl_area[gl_aai].zone;
						wi[win].hdr->wToNet  =gl_area[gl_aai].net;
						wi[win].hdr->wToNode =gl_area[gl_aai].node;
						wi[win].hdr->wToPoint=0;
#if 0
						goto alladdr;
#endif
					}

#if 0
					if ( (opt.addrcornet												 &&  is_netmail(gl_aai)) ||
							 (gl_area[gl_aai].flag & AR_FIXEDADDRESS && !is_netmail(gl_aai)) )
					{	
alladdr:
#endif
						wi[win].hdr->free6=
						passender_absender(to_neu,
															 wi[win].hdr->wToZone,
															 wi[win].hdr->wToNet,
															 wi[win].hdr->wToNode,
															 wi[win].hdr->wToPoint,
												 			 gl_area[gl_aai].domain,
															 &wi[win].hdr->wFromZone,
															 &wi[win].hdr->wFromNet,
															 &wi[win].hdr->wFromNode,
															 &wi[win].hdr->wFromPoint,
															 NULL);
#if 0
					}
					else
					{	wi[win].hdr->wFromZone =addr[0].zone;
						wi[win].hdr->wFromNet	 =addr[0].net;
						wi[win].hdr->wFromNode =addr[0].node;
						wi[win].hdr->wFromPoint=addr[0].point;
					}
#endif

/************************ Jetzt den Header aufbereiten **************/

					/* Hier muû in FROM meine Adresse stehen! */

					set_fwdstr_2(win, f_by);

					wi[win].hdr->lReplycrc=0;
					wi[win].hdr->rep.repcrc=0;
					i = put_msgid(win,wi[win].hdr,NULL);
					fputs(wi[win].dup_msgid,msgfp);
					put_pid(msgfp);

					del_controlinfo(win,msgfp,i);
					del_blanklines(win, FALSE);

					fputs(f_from,msgfp);	/* Originally from area ... 	*/
					fputs(f_fr,msgfp);		/* Originally from ... on ... */
					fputs(f_to,msgfp);		/* Originally to ...					*/
					fputs(f_by,msgfp);		/* Forwarded by ...						*/
					fputc('\n',msgfp);

					save_lines(win,msgfp,TRUE, gl_aai);
								
					ltemp = ftell(msgfp) - wi[win].hdr->lStart;
					if (ltemp>=65534ul)
					{	ltemp=65534ul;
						LangAlert(MSG_MSGTOOBIG);
					}
					wi[win].hdr->Size= (uword)ltemp;

					if (is_netmail(gl_aai))   /* Msg kommt in Netmail	*/
						if (own_address(wi[win].hdr->wToZone,	wi[win].hdr->wToNet,
														wi[win].hdr->wToNode,	wi[win].hdr->wToPoint))
							wi[win].hdr->wFlags &= ~F_KILLSENT;

					if (fwrite(wi[win].hdr,sizeof(HDR),1,hdrfp)!=1L)
					{	LangAlert(MSG_MOVEWRITEHDRERROR);
/*<=*/			break;
					}
				}
				wi[win].aktmsg++;

				gl_options |= EDITWINDOW_USED;
			}
			if (wi[win].aktmsg>endmsg)
				wi[win].aktmsg=endmsg;
			fclose(msgfp);
			fclose(hdrfp);
			del_doing();
			berechne_msgp(win);
			strcpy(wi[win].areaname,gl_area[old_aai].name);
			wind_set(wi[win].handle,WF_NAME,(long)wi[win].areaname,0,0);

			open_allarea(gl_aai);
			mouse_normal();

			wi[win].aktmsg=-1;									/* ssl 251294: Und bist Du nicht willig, ... */
			select_msg(win,aktmsg,TRUE,FALSE);
			wi[MSGSWIND].aktarea=-1;
			need_update_msglistwindow(win,RETURN);
		}
	}
}





void msg_forward(long win)
{	word msgnr,flag;
	uword Zone,Net,Node,Point;
	byte to_neu[MAX_STR];
	byte f_from[MAX_STR];
	byte f_by[MAX_STR];
	byte f_to[MAX_STR];
	byte f_fr[MAX_STR];
	byte snode[40];
	ulong ltemp;
	
	if (wi[win].aktmsg>0 && forw_dial(wi[win].aktarea,to_neu,snode))
	{
		wi[win].hdr->wFlags=0;   	 					/* Loesche Flags 				*/
		if (is_netmail(wi[win].aktarea))   /* Msg kommt in Netmail	*/
			wi[win].hdr->wFlags |= (F_KILLSENT|F_PRIVATE|F_LOCAL);
		else
		{	wi[win].hdr->wFlags |= F_LOCAL;
			if (gl_area[wi[win].aktarea].flag & AR_FORCEPVT)
				wi[win].hdr->wFlags |= F_PRIVATE;
		}
		del_mailerfields(win);
		set_if_fixed_address(win,wi[win].aktarea);
		
/*************** ORIGIN AUSLESEN UND FWD-TEXT SETZEN ****************/

		time(&wi[win].hdr->lDate);
		if (!is_netmail(wi[win].aktarea))
		{	if (!get_origin_address(win,&Zone,&Net,&Node,&Point,TRUE))
				Zone = Net = Node = Point = 0;
		}
		else
		{	Zone=wi[win].hdr->wFromZone;			/* 241196 */
			Net=wi[win].hdr->wFromNet;
			Node=wi[win].hdr->wFromNode;
			Point=wi[win].hdr->wFromPoint;
		}

		set_fwdstr_1(win,wi[win].aktarea,f_from,f_fr,f_to,Zone,Net,Node,Point);
		strcpy(wi[win].hdr->sFrom,gl_ptowner);
		strcpy(wi[win].hdr->sTo,to_neu);

/************************ Eigene-Adresse setzen *********************/

#if 0
	 	wi[win].hdr->wFromZone=Zone;
		wi[win].hdr->wFromNet=Net;
		wi[win].hdr->wFromNode=Node;
		wi[win].hdr->wFromPoint=Point;
#endif

		if (strlen(snode)>0L)
			nnstr2node(snode,(uword *)&wi[win].hdr->wToZone,(uword *)&wi[win].hdr->wToNet,
			 							 	 (uword *)&wi[win].hdr->wToNode,(uword *)&wi[win].hdr->wToPoint);
		else
		{	wi[win].hdr->wToZone =gl_area[wi[win].aktarea].zone;
			wi[win].hdr->wToNet  =gl_area[wi[win].aktarea].net;
			wi[win].hdr->wToNode =gl_area[wi[win].aktarea].node;
			wi[win].hdr->wToPoint=0;
#if 0
			goto alladdr2;
#endif
		}

#if 0
		if ( (opt.addrcornet																	&&  is_netmail(wi[win].aktarea)) ||
				 (gl_area[wi[win].aktarea].flag & AR_FIXEDADDRESS && !is_netmail(wi[win].aktarea)) )
		{
alladdr2;
#endif
			wi[win].hdr->free6=
			passender_absender(to_neu,
												 wi[win].hdr->wToZone,
												 wi[win].hdr->wToNet,
												 wi[win].hdr->wToNode,
												 wi[win].hdr->wToPoint,
												 gl_area[wi[win].aktarea].domain,
												 &wi[win].hdr->wFromZone,
												 &wi[win].hdr->wFromNet,
												 &wi[win].hdr->wFromNode,
												 &wi[win].hdr->wFromPoint,
												 NULL);
#if 0
		}
		else
		{	wi[win].hdr->wFromZone =addr[0].zone;
			wi[win].hdr->wFromNet	 =addr[0].net;
			wi[win].hdr->wFromNode =addr[0].node;
			wi[win].hdr->wFromPoint=addr[0].point;
		}
#endif

		set_fwdstr_2(win, f_by);
		
		msgnr=wi[win].lastmsg+1;
		biene();
		show_doing(DO_SAVEMSG);
		
		/* Set correct position */
		if (msgnr>0)
			hdrseek(win,wi[win].hdrfp,(msgnr-1),SEEK_SET);
		else 
			hdrseek(win,wi[win].hdrfp, 0, SEEK_END);
			
		fseek(wi[win].msgfp, 0L, SEEK_END);
	
		wi[win].hdr->lStart=ftell(wi[win].msgfp);
		
		/* save all lines */
		flag = put_msgid(win,wi[win].hdr,wi[win].msgfp);

		del_controlinfo(win,wi[win].msgfp,flag);
		del_blanklines(win, FALSE);
		
#if 0
		fputs(f_from,wi[win].msgfp);	/* "Originally from area..." nicht nîtig */
#endif
		fputs(f_fr,wi[win].msgfp);
		fputs(f_to,wi[win].msgfp);
		fputs(f_by,wi[win].msgfp);
		fputc('\n',wi[win].msgfp);
		
		save_lines(win,wi[win].msgfp,TRUE, wi[win].aktarea);

		ltemp = ftell(wi[win].msgfp) - wi[win].hdr->lStart;
		if (ltemp>=65534ul)
		{	ltemp=65534ul;
			LangAlert(MSG_MSGTOOBIG);
		}
		wi[win].hdr->Size= (uword)ltemp;
		
		if (is_netmail(wi[win].aktarea))   /* Msg kommt in Netmail	*/
			if (own_address(wi[win].hdr->wToZone,	wi[win].hdr->wToNet,
											wi[win].hdr->wToNode,	wi[win].hdr->wToPoint))
				wi[win].hdr->wFlags &= ~F_KILLSENT;

		if (hdrwrite(win, wi[win].hdr, 1, wi[win].hdrfp)!=1)
		{	LangAlert(MSG_FWDWRITEHDRERROR);
		}
		else
		{	wi[win].msgfilelen=ftell(wi[win].msgfp);
			wi[win].lastmsg++;
			wi[win].msgda[wi[win].lastmsg]=TRUE;

			gl_options |= EDITWINDOW_USED;
		}

		del_doing();
		mouse_normal();
		wi[win].oldmsg=0;

		wi[MSGSWIND].aktarea=-1;
    need_update_msglistwindow(win,RETURN);
		select_msg(win,wi[win].aktmsg,TRUE,TRUE);
	}
}

word  forw_dial(word area, byte *to_neu, byte *snode)
{	word ret=FALSE;
	uword zone,net,node,point;
	
	if (!is_netmail(area))
	{	set_mflag(nodenum_tree,HIDETREE,FALSE,NNON,NNSTRING,-1);
		del_flag(nodenum_tree,NNSTRING,EDITABLE,FALSE);
	}

	if (userbox(1,TRUE,NNTO,to_neu,&zone,&net,&node,&point))
	{	if (strlen(to_neu)>0)
		{	if (is_netmail(area))
				sprintf(snode,"%-5u%-5u%-5u%-5u",zone,net,node,point);
			else
				*snode=EOS;
			ret=TRUE;
		}
	}

	if (!is_netmail(area))
	{	del_mflag(nodenum_tree,HIDETREE,FALSE,NNON,NNSTRING,-1);
		set_flag(nodenum_tree,NNSTRING,EDITABLE,FALSE);
	}
	return(ret);
}

word get_origin_address(long win,uword *Zone,uword *Net,uword *Node,uword *Point,word strip)
{	word found=FALSE,i;
	byte *cp,tmp[80];

	i=0;
	cp=wi[win].lines[i];
	while (cp!=NULL)
	{	
    if (strip && !strncmp(cp,"---",3))
    	cp[1]='!';
    if (!strncmp(cp," * Origin:",10))
		{	if (strip)
				cp[1]='#';
			if (copy_origaddr(tmp,cp) || copy_origaddr(tmp,wi[win].lines[i+1]))
			{	scanaddr(tmp,Zone,Net,Node,Point,NULL);
				found=TRUE;
			}
		}
		if (strip)
			if (*cp=='\01' || !strncmp(cp,"SEEN-BY",7))
			{	del_line(win,i,FALSE);
				i--;
			}
		i++;
		cp=wi[win].lines[i];
	}
	return(found);
}

static void set_fwdstr_1(long win,word area,byte *f_from,byte *f_fr,byte *f_to,uword Zone,uword Net,uword Node,uword Point)
{
	if (is_netmail(area))
	{	if (Zone!=0 || Net!=0 || Node!=0 || Point!=0)
		{	sprintf(f_fr,msgtxt[DIV_ORIG_FROMON].text,wi[win].hdr->sFrom,
													mergeaddr(Zone,Net,Node,Point,""));
		}
		else
		{	sprintf(f_fr,msgtxt[DIV_ORIG_FROM].text,wi[win].hdr->sFrom);
		}
		strcat(f_fr,", ");
		strcat(f_fr,wi[win].hdr->sDate);
		sprintf(f_to,msgtxt[DIV_ORIG_TO].text,wi[win].hdr->sTo);
	}
	else
	{	sprintf(f_fr,msgtxt[DIV_ORIG_FROMONDATE].text,wi[win].hdr->sFrom,
												mergeaddr(wi[win].hdr->wFromZone,
																	wi[win].hdr->wFromNet,
																	wi[win].hdr->wFromNode,
																	wi[win].hdr->wFromPoint,""),
																	wi[win].hdr->sDate);
		sprintf(f_to,msgtxt[DIV_ORIG_TOON].text,wi[win].hdr->sTo,
												mergeaddr(wi[win].hdr->wToZone,
																	wi[win].hdr->wToNet,
																	wi[win].hdr->wToNode,
																	wi[win].hdr->wToPoint,""));
	}
	sprintf(f_from,msgtxt[DIV_ORIG_FROMAREA].text,
																		gl_area[area].name);
	strcat(f_fr,"\n");
	strcat(f_to,"\n");
	strcat(f_from,"\n");
}

static void set_fwdstr_2(long win,byte *f_by)
{	byte tmp[40];
	strcpy(tmp,ifna_date());
	sprintf(f_by,msgtxt[DIV_ORIG_FWD].text,
													gl_ptowner,
													mergeaddr(wi[win].hdr->wFromZone,wi[win].hdr->wFromNet,
																		wi[win].hdr->wFromNode,wi[win].hdr->wFromPoint,""),
													tmp);
	strcat(f_by,"\n");
}



word area_change(long win, word redraw, word changeflag)
{
	word old_aai, gl_aai;
	word ext_btn;
	word ret,laufw;

	if (win>=MAX_WIND)		/* Ist es kein MsgWindow? */
	{	for (laufw=0;laufw<MAX_WIND;laufw++)
		{	if (wi[laufw].belegt && !wi[laufw].editierbar && wi[laufw].aktarea==wi[win].aktarea)
			{	win=laufw;
				break;
			}
			if (win>=MAX_WIND)	/* Nicht gefunden, jetzt irgendein MsgWindow suchen */
			{	if (wi[laufw].belegt && !wi[laufw].editierbar)
				{	win=laufw;
					break;
				}
			}
		}
	}

	old_aai=gl_aai=wi[win].aktarea;

	set_dtext(allarea_tree,ATITEL,changeflag ? msgtxt[SYSTEM_CHANGEAREADIALOG].text
																					 : msgtxt[SYSTEM_OPENWINDOWAREA].text,
																					 			FALSE,22);

	del_mflag(allarea_tree,SELECTABLE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
	set_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
	ext_btn=do_aform(&gl_aai,TRUE);
	set_mflag(allarea_tree,SELECTABLE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
	del_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);

	del_dialog(&allarea_dial);
	del_state(allarea_tree,ext_btn,SELECTED,FALSE);
	
	biene();
	
	if (ext_btn==AMCAN)
	{
		wi[win].aktarea=gl_aai=old_aai;
		ret=FALSE;
	}	
	else
	{
		wi[win].aktarea=gl_aai;
		ret=TRUE;
	}     
	
	if (redraw)
	{
		if (gl_aai!=old_aai)
		{
			if (!srch_winarea(win,gl_aai))
			{
				close_area(win);
				if (open_area(win, gl_aai))
				{
					wind_set(wi[win].handle,WF_NAME,(long)wi[win].areaname,0,0);
					select_msg(win,wi[win].aktmsg,TRUE,TRUE);
				}
			}
			else
				wi[win].aktarea=old_aai;
		}
	}
	mouse_normal();
	return ret;
}

void msg_goto(long win, word preset)
{
	byte mtmp[6];
	word ext_btn;
	word msgnr;

	if (wi[win].aktmsg>0)
	{
		if (preset>0)
			sprintf(mtmp,"%d",preset);
		else
			*mtmp=EOS;
		set_dbutton(gomsg_tree,GMTEXT,msgtxt[DIV_GOTOMSG].text,FALSE);
		set_dtext(gomsg_tree,GMSGNR,mtmp,FALSE,4);
		draw_dialog(gomsg_tree, &gomsg_dial);
		ext_btn=Form_do(&gomsg_dial,GMSGNR);
		del_dialog(&gomsg_dial);
		del_state(gomsg_tree,ext_btn,SELECTED,FALSE);
		if (ext_btn == GOK)
		{
			get_dedit(gomsg_tree,GMSGNR,mtmp);
			if (wi[win].listwind)
			{	msgnr=atoi(mtmp);
				if (msgnr!=0 && msgnr != wi[win].selline-1)
				{	
					wi[win].aktmsg=msgnr;
					need_update_msglistwindow(win,RETURN);
					list_keybd(win,RETURN);
				}
			}
			else
			{
				msgnr=atoi(mtmp);
				if (msgnr!=0 && msgnr != wi[win].aktmsg)
				{	select_msg(win,msgnr,TRUE,TRUE);
					need_update_msglistwindow(win,RETURN);
				}
			}
		}
	}
}

word lade_wind(long win)
{	word stat;
	long laufw;
	word neuladen;
	
	if (!area_change(win,FALSE,FALSE))
		return(FALSE);
	
	biene();
	neuladen=TRUE;
	for (laufw=0;laufw<MAX_WIND;laufw++)
	{	if (wi[laufw].belegt)
		{	if (wi[laufw].aktarea==wi[win].aktarea)
			{	if (wi[laufw].editierbar)
					continue;
				top_it(laufw);
/*				if (wi[laufw].editierbar)
					set_menu(EDIT_WIND); */
				neuladen=FALSE;
				break;
			}
		}
	}
	if (neuladen)
	{	show_doing(DO_OPENMESSAGEAREA);
		if (open_area(win, wi[win].aktarea))
		{	del_doing();

			open_window(win,wi[win].areaname,TRUE);
			stat=select_msg(win,0,FALSE,TRUE);		/* 0 war wi[win].aktmsg */ 

			set_comstatus(win,wi[win].aktmsg);
/*		chk_status(win,TRUE); */
			new_status(win,stat);
		}
		else
			del_doing();
	}	
	mouse_normal();
	return(TRUE);
}

void write_listascii(long win, word mode)
{	FILE *fp;
	word i, ext_btn;
	byte doingstr[70], tmp[MAX_STR], *cp;
	static byte file[MAX_STR], oldappfile[MAX_STR];
	static word filechoosed = FALSE;
	struct ffblk dta;
	
	if (mode==FWCLIP)			/* init file-name for clipboard */
	{	if (!get_clipname(file))
			return;
	}

	if (mode==FPAPPEND)
	{	if (!filechoosed)
		{	if (hole_file(file,NULL,"*.TXT",msgtxt[FILE_APPENDLISTASCII].text))
			{	filechoosed=TRUE;
				strcpy(oldappfile,file);
			}
			else
			{	return;
			}
		}	
		else
			strcpy(file,oldappfile);		
	}
	else if (mode==FPWRITE)
	{	if (hole_file(file,NULL,"*.TXT",msgtxt[FILE_SAVELISTASCII].text))
		{	filechoosed=TRUE;
			strcpy(oldappfile,file);
			if (!findfirst(file,&dta,0))
			{	ext_btn=LangAlert(MSG_FILEEXISTS);
				if (ext_btn==3)
					return;
				else if (ext_btn==2 && mode==FPWRITE)
					mode=FPAPPEND;
			}
		}
		else
		{	return;
		}
	}
		
	if (mode==FPAPPEND)  /* append */
	{	sprintf(doingstr,msgtxt[DO_APPENDING].text, file);
		show_doingtext(doingstr);
	}
	else if (mode==FPWRITE)
		show_doing(DO_SAVELISTASCII);
	else
		show_doing(DO_SAVELISTCLIPBRD);
		
	biene();
	if (mode==FPAPPEND)
		fp=fopen(file,"a");
	else
		fp=fopen(file,"w");
		
	if (fp==NULL)
	{	LangAlert(MSG_WRASCIIOPENERROR);
		mouse_normal();
		del_doing();
		return;
	}	

	biene();
	i=0;
	if (wi[win].listwind==WI_LIST_AREAS)
	{	fputs(msgtxt[SYSTEM_AREALIST].text,fp);
		fputc('\n',fp);
		fputs("---------------------------------------------------------------------------\n",fp);
	}
	else if (wi[win].listwind==WI_LIST_MSGS)
	{	sprintf(tmp," %s   Max.Msg: %d   File: %s\n",
										wi[win].areaname,
										wi[win].lastmsg,wi[win].areapath);
		fputs(tmp,fp);
		fputs("---------------------------------------------------------------------------\n",fp);
	}
	while (wi[win].lines[i]!=NULL)
	{	strcpy(tmp,wi[win].lines[i]);
		cp=strchr(tmp,EOS);
		while (isspace(*(--cp)) && cp>tmp) *cp=EOS;
		if ((cp=strchr(tmp,'\03'))!=NULL) *cp='>';
		if ((cp=strchr(tmp,'\04'))!=NULL) *cp='<';
		fputs(tmp,fp); fputc('\n',fp);
		i++;
	}
	fputc('\n',fp);
	fclose(fp);
	del_doing();
	mouse_normal();
}

static void write_block(long win, word mode, word block_only)
{	FILE *fp;
	word i,last,ext_btn;
	byte doingstr[70];
	struct ffblk dta;
	static byte file[MAX_STR];
	static word filechoosed = FALSE;
	static byte oldappfile[MAX_STR];

	if (mode==FWCLIP)			/* init file-name for clipboard */
	{	if (!get_clipname(file))
			return;
	}

	if (mode==FAPPEND)			  /* append */
	{	if (!filechoosed)
		{	if (hole_file(file,NULL,"*.TXT",
							msgtxt[FILE_APPENDMSGASCII].text))
			{	filechoosed=TRUE;
				strcpy(oldappfile,file);
			}
			else
			{	return;
			}
		}	
		else
			strcpy(file,oldappfile);		
	}
	else if (mode==FWRITE)					/* write */
	{	if (hole_file(file,NULL,"*.TXT",
						msgtxt[FILE_SAVEMSGASCII].text))
		{	filechoosed=TRUE;
			strcpy(oldappfile,file);
			if (!findfirst(file,&dta,0))
			{	ext_btn=LangAlert(MSG_FILEEXISTS);
				if (ext_btn==3)
					return;
				else if (ext_btn==2 && mode==FWRITE)
					mode=FAPPEND;
			}
		}
		else
		{	return;
		}
	}

	if (mode==FAPPEND)		  /* append */
	{	sprintf(doingstr,
							msgtxt[DO_APPENDING].text,
							file);
		show_doingtext(doingstr);
	}
	else if (mode==FWRITE)
		show_doing(DO_SAVEMSGASCII);
	else
		show_doing(DO_SAVEMSGCLIPBRD);
		
	biene();
	if (mode==FAPPEND)
		fp=fopen(file,"a");
	else
		fp=fopen(file,"w");
		
	if (fp==NULL)
	{	LangAlert(MSG_WRASCIIOPENERROR);
		mouse_normal();
		del_doing();
		return;
	}	

	biene();
	i=0;
	last=MAX_LINES-1;
	if (block_only==1)
	{	if (s_cutbuff>=0)	i=s_cutbuff;
		if (e_cutbuff>=0)	last=e_cutbuff;
	}

	while (wi[win].lines[i]!=NULL && i<=last)
	{	
		fputs(wi[win].lines[i],fp);
		if (strchr(wi[win].lines[i],'\n')==NULL)
			fputc('\n',fp);

		i++;
	}
	
	fclose(fp);
	del_doing();
	mouse_normal();
}

/*
** block_only: 1=Block
**						 2=Text
**						 0=Text (Dialog)
*/
void write_ascii(long win, word mode, word block_only)
{	FILE *fp;
	word i,last;
	word aktmsg;
	word ext_btn;
	word anfmsg,endmsg;
	byte anftmp[6],endtmp[6];
	byte doingstr[70],tmp[MAX_STR];
	long cur_time;
	static byte file[MAX_STR];
	struct ffblk dta;
	static word filechoosed = FALSE;
	static byte oldappfile[MAX_STR];
	struct tm *tm_now;					/* Y2K */
	
	if (wi[win].aktmsg<=0)
		return;

	if (block_only)
	{	write_block(win,mode,block_only);
		return;
	}	
	if (mode==FWCLIP)			/* init file-name for clipboard */
	{	if (!get_clipname(file))
			return;
	}

	aktmsg=wi[win].aktmsg;
	if (wi[win].topline>0 || wi[win].firstc>0)
	{	wi[win].topline=0;
		wi[win].firstc=0;
		wisetfiles(win,FALSE,NO_CHANGE);
	}
	
	sprintf(anftmp,"%d",aktmsg);
	sprintf(endtmp,"%d",aktmsg);
	set_dtext(writemsg_tree,WMANF,anftmp,FALSE,4);
	set_dtext(writemsg_tree,WMEND,endtmp,FALSE,4);

	ext_btn=WMCANCEL;
	if (mode!=FAPPEND && mode!=FPAPPEND)
	{	set_dtext(writemsg_tree,WMTO,msgtxt[SYSTEM_TOAFILE].text,FALSE,17);
		if (mode==FPWRITE)
			set_dtext(writemsg_tree,WMTO,msgtxt[SYSTEM_PLAINTOAFILE].text,FALSE,17);
		if (mode==FWCLIP)
			set_dtext(writemsg_tree,WMTO,msgtxt[SYSTEM_TOCLIPBRD].text,FALSE,17);

		draw_dialog(writemsg_tree, &writemsg_dial);
		ext_btn=Form_do(&writemsg_dial,WMANF);
		del_dialog(&writemsg_dial);
		del_state(writemsg_tree,ext_btn,SELECTED,FALSE);
	}
	
	if (ext_btn==WMOK || mode==FAPPEND || mode==FPAPPEND)
	{	get_dedit(writemsg_tree,WMANF,anftmp);
		get_dedit(writemsg_tree,WMEND,endtmp);
		anfmsg=atoi(anftmp);
		endmsg=atoi(endtmp);
		if (anfmsg > endmsg)
		{	i=anfmsg;
			anfmsg=endmsg;
			endmsg=i;
		}
		if (anfmsg < wi[win].firstmsg)	anfmsg=wi[win].firstmsg;
		if (endmsg > wi[win].lastmsg)		endmsg=wi[win].lastmsg;
		wi[win].aktmsg=anfmsg;
		
		if (mode==FAPPEND || mode==FPAPPEND)  /* append */
		{	if (!filechoosed)
			{	if (hole_file(file,NULL,"*.TXT",
						mode==FAPPEND ? msgtxt[FILE_APPENDMSGASCII].text : 
														msgtxt[FILE_APPENDMSGPLAINASCII].text))
				{	filechoosed=TRUE;
					strcpy(oldappfile,file);
				}
				else
				{	wi[win].oldmsg=0;
					wi[win].aktmsg=aktmsg;
					select_msg(win,aktmsg,TRUE,TRUE);
					return;
				}
			}	
			else
				strcpy(file,oldappfile);		
		}
		else if (mode==FWRITE || mode==FPWRITE)			/* write */
		{	if (hole_file(file,NULL,"*.TXT",
					mode==FWRITE ? msgtxt[FILE_SAVEMSGASCII].text :
												 msgtxt[FILE_SAVEMSGPLAINASCII].text))
			{	filechoosed=TRUE;
				strcpy(oldappfile,file);
				if (!findfirst(file,&dta,0))
				{	ext_btn=LangAlert(MSG_FILEEXISTS);
					if (ext_btn==3)
						return;
					else if (ext_btn==2 && mode==FWRITE)
						mode=FAPPEND;
					else if (ext_btn==2 && mode==FPWRITE)
						mode=FPAPPEND;
				}
			}
			else
			{	wi[win].oldmsg=0;
				wi[win].aktmsg=aktmsg;
				select_msg(win,aktmsg,TRUE,TRUE);
				return;
			}
		}
		
		if (mode==FAPPEND || mode==FPAPPEND)  /* append */
		{	sprintf(doingstr,
							mode==FAPPEND ? msgtxt[DO_APPENDING].text : msgtxt[DO_APPENDTOPLAIN].text,
							file);
			show_doingtext(doingstr);
		}
		else if (mode==FWRITE || mode==FPWRITE)
			show_doing(mode==FWRITE ? DO_SAVEMSGASCII
															: DO_SAVEMSGASCIIPLAIN);
		else
			show_doing(DO_SAVEMSGCLIPBRD);
		
		biene();
		if (mode==FAPPEND || mode==FPAPPEND)
			fp=fopen(file,"a");
		else
			fp=fopen(file,"w");
		
		if (fp==NULL)
		{	LangAlert(MSG_WRASCIIOPENERROR);
			mouse_normal();
			del_doing();
			wi[win].oldmsg=0;
			wi[win].aktmsg=aktmsg;
			select_msg(win,aktmsg,TRUE,TRUE);
			return;
		}	
		while (wi[win].aktmsg<=endmsg)
		{	if (wi[win].msgda[wi[win].aktmsg]!=TRUE)
			{	wi[win].aktmsg++;
				continue;
			}
			wi[win].oldmsg=0;
			select_msg(win,wi[win].aktmsg,FALSE,TRUE);
			
			biene();
			if (mode==FAPPEND || mode==FWRITE || mode==FWCLIP)
			{
				if (opt.printopt & O_PRINTOPT_INFO)
				{	fprintf(fp, msgtxt[DIV_PRINT_MSG].text,	wi[win].aktmsg,
										 			           							wi[win].lastmsg);
				  fputc('\n',fp);
	
					if (wi[win].status)
					{	fprintf(fp,"%s",wi[win].status);
						fputc('\n',fp);
					}
					fprintf(fp, msgtxt[DIV_PRINT_WRITTEN].text, wi[win].hdr->sDate);
				  fputc('\n',fp);
					
					tm_now=localtime(&wi[win].hdr->lDate);		/* Y2K */
					tm_now->tm_year %= 100;										/* Y2K */
					strftime(tmp,20,"%d %b %y  %X",tm_now);		/* Y2K */
					fprintf(fp, msgtxt[DIV_PRINT_IMPORTED].text, tmp);
				  fputc('\n',fp);

					time(&cur_time);
					tm_now=localtime(&cur_time);		/* Y2K */
					tm_now->tm_year %= 100;										/* Y2K */
					strftime(tmp,20,"%d %b %y  %X",tm_now);
					fprintf(fp, msgtxt[DIV_PRINT_PRINTED].text, tmp);
				  fputc('\n',fp);
				}
				else
				{	fprintf(fp, msgtxt[DIV_PRINT_MSGTIME].text,wi[win].aktmsg,
			    			         wi[win].lastmsg,wi[win].hdr->sDate);
				  fputc('\n',fp);
				}

				fprintf(fp,msgtxt[DIV_FROM].text,wi[win].hdr->sFrom);
				if (is_netmail(wi[win].aktarea))
				{	fprintf(fp,msgtxt[DIV_ON].text,mergeaddr(wi[win].hdr->wFromZone,
					        											wi[win].hdr->wFromNet,
					        											wi[win].hdr->wFromNode,
																				wi[win].hdr->wFromPoint,
																				gl_area[wi[win].aktarea].domain));
				}
			  fputc('\n',fp);

				fprintf(fp,msgtxt[DIV_TO].text,wi[win].hdr->sTo);
				if (is_netmail(wi[win].aktarea))
				{	fprintf(fp,msgtxt[DIV_ON].text,mergeaddr(wi[win].hdr->wToZone,
				  	      											wi[win].hdr->wToNet,
				    	    											wi[win].hdr->wToNode,
																				wi[win].hdr->wToPoint,
																				gl_area[wi[win].aktarea].domain));
				}
			  fputc('\n',fp);

				fprintf(fp,msgtxt[DIV_SUBJ].text,wi[win].hdr->sSubject);
			  fputc('\n',fp);

				fprintf(fp,"---------[%s]-----------------------------------------------\n",wi[win].areaname);
			}

			i=0;
			last=MAX_LINES;
			if (mode==FPWRITE || mode==FPAPPEND)
			{	while (wi[win].lines[i]!=NULL && i<MAX_LINES &&
			         (wi[win].lines[i][0]==EOS ||
			          wi[win].lines[i][0]=='\001' ||
			          wi[win].lines[i][0]=='\n') )
					i++;

				last=0;
				while (wi[win].lines[last]!=NULL && last<MAX_LINES)
					last++;
				last--;
				if (last>=0)
				{	while (last>=0 &&
			         	 (wi[win].lines[last][0]==EOS ||
				          wi[win].lines[last][0]=='\001' ||
			         	  wi[win].lines[last][0]=='\n' ||
			         	 	!strncmp(wi[win].lines[i]," * Origin:",10) ||
									!strncmp(wi[win].lines[i],"--- ",4)  || 
				    			!strncmp(wi[win].lines[i],"---\n",4) || 
				    			!strncmp(wi[win].lines[i],"---\0",4)) )
			    	last--;
			    last++;
				}
			}

			while (wi[win].lines[i]!=NULL && i<last)
			{
				if (!strncmp(wi[win].lines[i],"--- ",4)  || 
				    !strncmp(wi[win].lines[i],"---\n",4) || 
				    !strncmp(wi[win].lines[i],"---\0",4) ||
				    !strncmp(wi[win].lines[i]," * Origin:",10))
				{	
					if (mode==FPWRITE || mode==FPAPPEND)
						break;
					else
						wi[win].lines[i][1]='!';
				}
				
				fputs(wi[win].lines[i],fp);
				if (strchr(wi[win].lines[i],'\n')==NULL)
					fputc('\n',fp);

				i++;
			}
			if (mode==FAPPEND || mode==FWRITE)
				fputs("\n\n",fp);
			
			wi[win].aktmsg++;
		}
		wi[win].aktmsg--;
		fclose(fp);
		del_doing();
		mouse_normal();
		
		wi[win].oldmsg=0;
		wi[win].aktmsg=aktmsg;
		select_msg(win,aktmsg,TRUE,TRUE);
	}
}

/*
** mode==-1 : aufgerufen vom Shortcut-Dialog
*/

word read_ascii(long win, word mode, byte *fname)
{	FILE *fp;
	static byte file[MAX_STR];
	byte line_buf[1024],*cp,chr;
	word i,llen;

	if (mode==-1)
		store_msghdr(win);

	if (mode==FREAD)
	{
		if (hole_file(file,NULL,"*.*",msgtxt[FILE_READFILE].text)==FALSE)
			return(FALSE);
	}
	else if (mode==FRCLIP)
	{	if (!get_clipname(file))
			return(FALSE);
	}
	else
		strcpy(file,fname);
			
	biene();
	if ((fp=fopen(file,"r"))==NULL)
	{	LangAlert(MSG_REASCIIOPENERROR);
		mouse_normal();
		return(FALSE);
	}	

	i=wi[win].lnr;
	while (fgets(line_buf,511,fp) && wi[win].lzahl<MAX_LINES-1)
	{	line_buf[511]=EOS;

		if (mode==-1)
			convert_percent(line_buf,wi[win].aktarea,TRUE);

		cp=line_buf;
		while (*cp)
		{	if (*cp<32 && !isspace(*cp)) *cp='.';
			cp++;
		}

		cp=line_buf;
		if (!strncmp(cp,"--- " ,4)	|| !strncmp(cp,"---\n",4) ||
		    !strncmp(cp,"---\0",4)	|| !strncmp(cp," * Origin:",10))
			*(cp+1)='!';

next_line:
		llen=(word)strlen(line_buf);
		if (llen<=opt.maxedcol)
		{	if (!ins_line(win,i,line_buf,FALSE,FALSE))
				break;
			i++;
		}
		else
		{	cp=line_buf+opt.maxedcol;
			while (cp>line_buf && !isspace(*cp)) cp--;
			if (isspace(*cp)) cp++;
			if (cp==line_buf)
				cp=line_buf+opt.maxedcol;
			chr=*cp;
			*cp=EOS;
			if (!ins_line(win,i,line_buf,FALSE,FALSE))
				break;
			i++;
			*cp=chr;
			strcpy(line_buf,cp);
			if (*line_buf)
				goto next_line;
		}
	}

	if (wi[win].cy>=TOPMSG_LINE)
		set_cxpos(win);
	fclose(fp);
	mouse_normal();
	return(TRUE);
}

void prn_inits(byte type)
{	word len,i;
	len=prn_divs[type][0];
	if (len)
		for (i=1;i<=len;i++)
			Cprnout(prn_divs[type][i]);
}

void prn_char(byte chr)
{	word len,i;
	len=prn_table[chr][0];
	if (len)
		for (i=1;i<=len;i++)
			Cprnout(prn_table[chr][i]);
}

void prn_out(byte *line, word addcr, word startofline)
{	byte *cp;
	word cr=FALSE,lf=FALSE,i;

	if (line!=NULL && strlen(line)>0)
	{	if (startofline && prn_options[OPT_LEFTMARGIN])
			for(i=0;i<prn_options[OPT_LEFTMARGIN];i++)
				Cprnout(' ');

		cp=line;
		while (*cp)
		{
					 if (*cp=='\n') lf=TRUE;
			else if (*cp=='\r')	cr=TRUE;

			prn_char(*cp++);
		}
		if (addcr)
		{	if (!cr) prn_char('\r');
			if (!lf) prn_char('\n');
		}
	}
}







static void print_gdos_msg(long win)
{	byte line[MAX_STR],*p,tmp[MAX_STR];
	word i,x,y,save_handle,width,wintype;
	word xwork_in[11], xwork_out[57];
	word pxy[4],ext[8];
	word effect_fl;
	long cur_time;
	struct tm *tm_now;					/* Y2K */

	biene();
	show_doing(DO_PRINTINGGDOS);

	save_handle=handle;

	for (i=1; i<10; xwork_in[i++]=1);
	xwork_in[ 0] = 21;
	xwork_in[10] = 2;

	v_opnwk(xwork_in,&handle,xwork_out);
	if (handle<=0)
	{	handle=save_handle;
		del_doing();
		mouse_normal();
		LangAlert(MSG_GDOSOPENERROR);
		return;
	}

	set_wifont(win);
	x=4;
	y=gl_hbox;

	if (wi[win].listwind==WI_LIST_AREAS)
	{	v_gtext(handle,x,y,msgtxt[DIV_AREALIST].text);
		y+=gl_hbox;
		pxy[1]=pxy[3]=y-(gl_hbox >> 2);
		pxy[3]++;
		pxy[0]=0;
		pxy[2]=xwork_out[0];
		vr_recfl(handle,pxy);
		y+=gl_hbox;
	}
	else if (wi[win].listwind==WI_LIST_MSGS)
	{	sprintf(line,msgtxt[DIV_AREAWINDLIST].text,
										wi[win].areaname,
										wi[win].lastmsg,wi[win].areapath);
		v_gtext(handle,x,y,line);
		y+=gl_hbox;
		pxy[1]=pxy[3]=y-(gl_hbox >> 2);
		pxy[3]++;
		pxy[0]=0;
		pxy[2]=xwork_out[0];
		vr_recfl(handle,pxy);
		y+=gl_hbox;
	}
	else
	{	
		if (opt.printopt & O_PRINTOPT_INFO)
		{	sprintf(line, msgtxt[DIV_PRINT_MSG].text,	wi[win].aktmsg,
								   			           							wi[win].lastmsg);
			v_gtext(handle,x,y,line);	y+=gl_hbox;

			if (wi[win].status)
			{	v_gtext(handle,x,y,wi[win].status);	y+=gl_hbox;
			}
			sprintf(line, msgtxt[DIV_PRINT_WRITTEN].text, wi[win].hdr->sDate);
			v_gtext(handle,x,y,line);	y+=gl_hbox;
					
			tm_now=localtime(&wi[win].hdr->lDate);		/* Y2K */
			tm_now->tm_year %= 100;										/* Y2K */
			strftime(tmp,20,"%d %b %y  %X",tm_now);
			sprintf(line, msgtxt[DIV_PRINT_IMPORTED].text, tmp);
			v_gtext(handle,x,y,line);	y+=gl_hbox;

			time(&cur_time);
			tm_now=localtime(&cur_time);		/* Y2K */
			tm_now->tm_year %= 100;										/* Y2K */
			strftime(tmp,20,"%d %b %y  %X",tm_now);
			sprintf(line, msgtxt[DIV_PRINT_PRINTED].text, tmp);
			v_gtext(handle,x,y,line);	y+=gl_hbox;
		}
		else
		{	sprintf(line, msgtxt[DIV_PRINT_MSGTIME].text,wi[win].aktmsg,
   				          wi[win].lastmsg,wi[win].hdr->sDate);
			v_gtext(handle,x,y,line);	y+=gl_hbox;
		}

		if (is_netmail(wi[win].aktarea))
		{	sprintf(tmp,msgtxt[DIV_ON].text,
														mergeaddr(wi[win].hdr->wFromZone,
																			wi[win].hdr->wFromNet,
																			wi[win].hdr->wFromNode,
																			wi[win].hdr->wFromPoint,
																			gl_area[wi[win].aktarea].domain));
			sprintf(line,msgtxt[DIV_FROM].text,wi[win].hdr->sFrom);
			strcat(line,tmp);
		}
		else
			sprintf(line,msgtxt[DIV_FROM].text,wi[win].hdr->sFrom);
		v_gtext(handle,x,y,line);
		y+=gl_hbox;
		if (is_netmail(wi[win].aktarea))
		{	sprintf(tmp,msgtxt[DIV_ON].text,
															mergeaddr(wi[win].hdr->wToZone,
																				wi[win].hdr->wToNet,
																				wi[win].hdr->wToNode,
																				wi[win].hdr->wToPoint,
																				gl_area[wi[win].aktarea].domain));
			sprintf(line,msgtxt[DIV_TO].text,wi[win].hdr->sTo);
			strcat(line,tmp);
		}
		else
			sprintf(line,msgtxt[DIV_TO].text,wi[win].hdr->sTo);
		v_gtext(handle,x,y,line);
		y+=gl_hbox;
		sprintf(line,msgtxt[DIV_SUBJ].text,wi[win].hdr->sSubject);
		effect_v_gtext(win,x,y,-1,line);
		y+=gl_hbox;

		vqt_extent(handle,"========",ext);
		width=ext[2]-ext[0];

#if 1
		/* Normale Version mit nur einer Linie */
		pxy[1]=pxy[3]=y-(gl_hbox >> 2);
		pxy[3]++;
		pxy[0]=0;
		pxy[2]=width;
		vr_recfl(handle,pxy);
		pxy[0]=pxy[2]+(gl_wbox >> 2);
		v_gtext(handle,pxy[0],y,wi[win].areaname);
		vqt_extent(handle,wi[win].areaname,ext);
		width+=ext[2]-ext[0]+gl_wbox;
		pxy[0]=width;
		pxy[2]=xwork_out[0];
		vr_recfl(handle,pxy);
		y+=gl_hbox;
#else
		/* Die lÑuft nicht! Nur Demonstration! */
		vsf_interior(handle,FIS_SOLID);
		vsf_style(handle,0);
		vsf_color(handle,BLACK);
		vsl_color(handle,BLACK);
		pxy[1]=pxy[3]=y-(gl_hbox >> 1)+1;
		pxy[3]++;
		vr_recfl(handle,pxy);
		if (gl_hbox>=8)
		{	pxy[1]=pxy[3]=y-(gl_hbox >> 1)-1;
			v_pline(handle,2,pxy);
			pxy[1]=pxy[3]=y-(gl_hbox >> 1)+4;
			v_pline(handle,2,pxy);
		}
		vsf_interior(handle,FIS_SOLID);
		vsf_style(handle,0);
		vsf_color(handle,WHITE);
#endif
	}

	wintype=wi[win].listwind;

	for (i=0;i<wi[win].lzahl;i++)
	{
		strcpy(line,wi[win].lines[i]);

		if (wintype)
		{
			if (wintype==WI_LIST_AREAS)
				my_gtext(x,y,line,0);
			else if (wintype==WI_LIST_MSGS)
			{	effect_fl=0;
				if (line[1]=='d')		effect_fl |= EFF_HELL;
				if (line[0]=='\x3')	effect_fl |= EFF_FETT;
					my_gtext(x,y,line,effect_fl);
			}
		}
		else
		{	effect_v_gtext(win,x,y,-1,line);
		}
		y+=gl_hbox;
		if (y>=xwork_out[1])
		{	v_updwk(handle);
			v_clrwk(handle);
			y=gl_hbox;
		}
	}

	test_registered();
	if (!is_registered)
	{	vst_effects(handle,0);
		vst_rotation(handle,45);
		strcpy(line,"\x7a\x70\x70\x70\x7a\x0f\x14\x08\x1f\x1d\x13\x09\x0e\x1f\x08\x1f\x1e\x7a\x70\x70\x70\x7a");
		p=line;
		while(*p) *p++^=0x5a;
		vqt_extent(handle,line,ext);
		width+=ext[2]-ext[0];
		while (y<=xwork_out[1] && xwork_out[0]-width-x>=0)
		{	v_gtext(handle,xwork_out[0]-width-x,y,line);
			v_gtext(handle,x,y,line);
			y+=gl_hbox;
			x+=gl_wbox;
		}
	}


	v_updwk(handle);
	v_clrwk(handle);
	v_clswk(handle);

	handle=save_handle;

	set_wifont(win);
	del_doing();
	mouse_normal();
}


void print_msg(long win, word doformfeed)
{	word i,j,isquote,effects=FALSE,found_effects=FALSE,flag;
	byte line[MAX_STR],tmp[MAX_STR],*p;
	long cur_time;
	struct tm *tm_now;					/* Y2K */

	if (wi[win].editierbar) return;

	if (opt.texteffects & O_TEXTEFFPRN &&
			opt.texteffects & O_TEXTEFFSCR)
	{	if (prn_table[TEON_U][1]!=TEON_U &&
				prn_table[TEON_B][1]!=TEON_B &&
				prn_table[TEON_C][1]!=TEON_C &&
				prn_table[TEON_I][1]!=TEON_I &&
				prn_table[TEOFF_U][1]!=TEOFF_U &&
				prn_table[TEOFF_B][1]!=TEOFF_B &&
				prn_table[TEOFF_C][1]!=TEOFF_C &&
				prn_table[TEOFF_I][1]!=TEOFF_I)
			effects=TRUE;
	}

	if (wi[win].aktmsg>0 || wi[win].listwind)
	{
		if (Cprnos())
		{
			if (opt.printopt & O_PRINTOPT_GDOS)
			{	print_gdos_msg(win);
				return;
			}
			biene();
			show_doing(DO_PRINTINGTEXT);

			prn_inits(DIV_INIT);

			if (wi[win].listwind==WI_LIST_AREAS)
			{	prn_out(msgtxt[DIV_AREALIST].text,TRUE,TRUE);
				prn_out("---------------------------------------------------------------------------\n",TRUE,TRUE);
			}
			else if (wi[win].listwind==WI_LIST_MSGS)
			{	sprintf(line,msgtxt[DIV_AREAWINDLIST].text,
												wi[win].areaname,
												wi[win].lastmsg,wi[win].areapath);
				prn_out(line,TRUE,TRUE);
				prn_out("---------------------------------------------------------------------------\n",TRUE,TRUE);
			}
			else
			{	
				if (opt.printopt & O_PRINTOPT_INFO)
				{	sprintf(line, msgtxt[DIV_PRINT_MSG].text,	wi[win].aktmsg,
										   			           							wi[win].lastmsg);
					prn_out(line,TRUE,TRUE);

					if (wi[win].status)
						prn_out(wi[win].status,TRUE,TRUE);
	
					sprintf(line, msgtxt[DIV_PRINT_WRITTEN].text, wi[win].hdr->sDate);
					prn_out(line,TRUE,TRUE);
					
					tm_now=localtime(&wi[win].hdr->lDate);		/* Y2K */
					tm_now->tm_year %= 100;										/* Y2K */
					strftime(tmp,20,"%d %b %y  %X",tm_now);
					sprintf(line, msgtxt[DIV_PRINT_IMPORTED].text, tmp);
					prn_out(line,TRUE,TRUE);

					time(&cur_time);
					tm_now=localtime(&cur_time);		/* Y2K */
					tm_now->tm_year %= 100;										/* Y2K */
					strftime(tmp,20,"%d %b %y  %X",tm_now);		/* Y2K */
					sprintf(line, msgtxt[DIV_PRINT_PRINTED].text, tmp);
					prn_out(line,TRUE,TRUE);
				}
				else
				{	sprintf(line, msgtxt[DIV_PRINT_MSGTIME].text,wi[win].aktmsg,
		   				          wi[win].lastmsg,wi[win].hdr->sDate);
					prn_out(line,TRUE,TRUE);
				}

				if (is_netmail(wi[win].aktarea))
				{	sprintf(tmp,msgtxt[DIV_ON].text,
																mergeaddr(wi[win].hdr->wFromZone,
																					wi[win].hdr->wFromNet,
																					wi[win].hdr->wFromNode,
																					wi[win].hdr->wFromPoint,
																					gl_area[wi[win].aktarea].domain));
					sprintf(line,msgtxt[DIV_FROM].text,wi[win].hdr->sFrom);
					strcat(line,tmp);
				}
				else
					sprintf(line,msgtxt[DIV_FROM].text,wi[win].hdr->sFrom);
/*			strcat(line,"\n"); */
				prn_out(line,TRUE,TRUE);

				if (is_netmail(wi[win].aktarea))
				{	sprintf(tmp,msgtxt[DIV_ON].text,
																	mergeaddr(wi[win].hdr->wToZone,
																						wi[win].hdr->wToNet,
																						wi[win].hdr->wToNode,
																						wi[win].hdr->wToPoint,
																						gl_area[wi[win].aktarea].domain));
					sprintf(line,msgtxt[DIV_TO].text,wi[win].hdr->sTo);
					strcat(line,tmp);
				}
				else
					sprintf(line,msgtxt[DIV_TO].text,wi[win].hdr->sTo);
				prn_out(line,TRUE,TRUE);

				sprintf(line,msgtxt[DIV_SUBJ].text,wi[win].hdr->sSubject);
				if (effects)
					found_effects=scan_effects(line,line);
				prn_out(line,TRUE,TRUE);

				if (effects && found_effects)
					prn_out("\021\023\025\027",FALSE,FALSE);
				sprintf(line,"---------[%s]-----------------------------------------------\n",wi[win].areaname);
				prn_out(line,TRUE,TRUE);
			}
			
			i=0;
			while (wi[win].lines[i]!=NULL && i<MAX_LINES)
			{	
				flag=TRUE;	/* Wir sind am Anfang der Zeile */

				strcpy(line,wi[win].lines[i]);

				if (effects)
					found_effects=scan_effects(line,line);

				if (effects && opt.qthick)
				{	isquote=FALSE;
					j=0;
					p=line;
					while (*p && j++<6)
					{	if (*p++=='>')
						{	isquote=TRUE;
							break;
						}
					}
					if (isquote)
					{	prn_out("\022",FALSE,flag);
						flag=FALSE;

						p=line;
						while(*p)
						{			 if (*p==TEOFF_B) *p=TEON_B;
							else if (*p==TEON_B)	*p=TEOFF_B;
							p++;
						}
						found_effects=TRUE;
					}
				}

				prn_out(line,TRUE,flag);
				flag=FALSE;

				if (effects && found_effects)
					prn_out("\021\023\025\027",FALSE,FALSE);
				i++;

				if (!(i % 15))
				{	test_registered();
					if (!is_registered)
					{	strcpy(line,"\x7a\x70\x70\x70\x7a\x0f\x14\x08\x1f\x1d\x13\x09\x0e\x1f\x08\x1f\x1e\x7a\x70\x70\x70\x7a");
						p=line;
						while(*p) *p++^=0x5a;
						prn_out(line,TRUE,TRUE);
					}
				}

			}
			if (doformfeed || (opt.printopt & O_PRINTOPT_FF))
				prn_out("\f",FALSE,FALSE);
			else
				prn_out("\r\n",TRUE,FALSE);
			del_doing();
			mouse_normal();
		}
		else
			LangAlert(MSG_PRINTERNOTREADY);
	}
}










/* ssl 070195: Wenn area=-1, dann werden ALLE "newmsg" flags gelîscht!	*/
/* Ansonsten steht in area die momentane Aea, die geschlossen wird.			*/
void save_lread(word area)
{	struct ffblk dta;
	FILE *fp;
	word i;
	byte *tmp,ftemp[MAX_STR],ftemp2[MAX_STR];

	biene();
	sprintf(ftemp ,"%sLED.NEW",gl_ledpath);
	sprintf(ftemp2,"%sLED.BAK",gl_ledpath);
	if (findfirst(ftemp,&dta,0))
	{	sprintf(ftemp ,"%sLED.NEW",gl_lednewpath);
		sprintf(ftemp2,"%sLED.BAK",gl_lednewpath);
	}
	if (!findfirst(ftemp2,&dta,0))
		remove(ftemp2);
	if (!findfirst(ftemp,&dta,0))
		rename(ftemp,ftemp2);
	

	if ((fp=fopen(ftemp,"w"))!=NULL)
	{
		tmp=gl_area[0].name;
		gl_area[0].name="FidoNetmail";
		for (i=0;i<gl_arzahl;i++)
		{
			if (opt.delnewmsg && area==-1)
				gl_area[i].newmsg&=~NEWMSG;  	/* delete new Msgs */
			
			if (stricmp(gl_area[i].name,"Bad_Msgs") && 
			    stricmp(gl_area[i].name,"ExtraExpArea"))
			{
				fprintf(fp,"%s  %d %d\n",gl_area[i].name,
				                         gl_area[i].lread,
				                         gl_area[i].newmsg);
				/* Bit 0 = new Msgs    = * */
				/* Bit 1 = unread Msgs = / */
				/* Bit 2 = to Sysop    = # */
			}
		}
		gl_area[0].name=tmp;
		fclose(fp);
	}
	else
		LangAlert(MSG_LEDNEWOPENERROR);
	mouse_normal();
}

void load_lread(void)
{	struct ffblk dta;
	FILE *fp;
	word i;
	byte *name, *lread, *new;
	byte tmp[MAX_STR];
	byte name_buf[MAX_STR];
	
	biene();
	for (i=0;i<gl_arzahl;i++)
	{
		if (!stricmp(gl_area[i].name,"Bad_Msgs"))
		{
			strcpy(tmp,gl_area[i].path);
			strcat(tmp, HEADEREXT);
			if (findfirst(tmp,&dta,0)==0 && dta.ff_fsize>0)
				gl_area[i].newmsg=3;
			break;
		}
	}
	sprintf(tmp,"%sLED.NEW",gl_ledpath);
	if (findfirst(tmp,&dta,0))
		sprintf(tmp,"%sLED.NEW",gl_lednewpath);
	if ((fp=fopen(tmp,"r"))!=NULL)
	{
		while (fgets(tmp,MAX_STR,fp)!=NULL)
		{
			name=strtok(tmp," \r\n");
			lread=strtok(NULL," \r\n");
			new=strtok(NULL," \r\n");
			if (name==NULL || lread==NULL || new==NULL)
				continue;
			
			if ( !isdigit(*lread) )
			{
				strcpy(name_buf,name);
				strcat(name_buf," ");
				strcat(name_buf,lread);
				name=name_buf;
				lread=new;
				new=strtok(NULL," \r\n");
			}
			
			if (!stricmp(name,"FidoNetmail"))
				i=0;
			else
			{
				for (i=1;i<gl_arzahl;i++)
					if (!stricmp(name, gl_area[i].name))
						break;
			}
			if (i<gl_arzahl)
			{
				gl_area[i].lread=atoi(lread);
				gl_area[i].newmsg=atoi(new);

				if (gl_area[i].flag & AR_IGNORENEWMSGFL)
					gl_area[i].newmsg &= ~NEWMSG;
				if (gl_area[i].flag & AR_IGNOREUNREADFL)
					gl_area[i].newmsg &= ~UNRDMSG;

				/* Bit 0 = new Msgs    = * */
				/* Bit 1 = unread Msgs = / */
				/* Bit 2 = to Sysop    = # */
			}
		}
		fclose(fp);	
	}
	else
	{
		for (i=0;i<gl_arzahl;i++)
		{
			gl_area[i].lread=0;
			gl_area[i].newmsg=0;
		}
	}
	mouse_normal();
}

void save_echolist(void)
{	FILE *fp;
	word i,found;
	byte tmp[MAX_STR+1];

	/* echo-areas edited? */
	for (i=1;i<gl_arzahl;i++)
		if (gl_area[i].flag & AR_EDITED)
			if (!(gl_area[i].flag & AR_NONEWECHO))
				break;
	if (i>=gl_arzahl)
		return;
	
	strcpy(tmp,gl_echofile);

	if ((fp=fopen(tmp,"r+"))==NULL)
		fp=fopen(tmp,"w+");
	
	if (fp!=NULL)
	{
		for (i=1;i<gl_arzahl;i++)
		{
			if (gl_area[i].flag & AR_EDITED &&
					!(gl_area[i].flag & AR_NONEWECHO))
			{
				found=FALSE;
				fseek(fp,0,SEEK_SET);
				while (fgets(tmp,MAX_STR,fp)!=NULL)
				{
					if (!stricmp(gl_area[i].name,tmp))
					{	found=TRUE;
						break;
					}
				}
				if (!found)
				{
					fseek(fp,0,SEEK_END);
					fprintf(fp,"%s\n",gl_area[i].name);
				}
			}
		}
		fclose(fp);
	}
}

static void load_echolist(void)
{	FILE *fp;
	word i;
	byte tmp[MAX_STR+1];

	strcpy(tmp,gl_echofile);

	if ((fp=fopen(tmp,"r"))==NULL)
		return;
	
	while (fgets(tmp,MAX_STR,fp)!=NULL)
	{
		cut_endspc(tmp);
		if (!(*tmp))
			continue;
			
		for (i=1;i<gl_arzahl;i++)
			if (!stricmp(tmp, gl_area[i].name))
				break;
		if (i<gl_arzahl)
			gl_area[i].flag |= AR_EDITED;
	}
	fclose(fp);
}

static ulong drvbits;
static long lpeek_drvbits(void)
{ drvbits=*((ulong *)0x4c2);
	return 0;
}

byte get_fastdrv(void)
{	byte ret;
	word i;
	ulong drv;
	
	Supexec(lpeek_drvbits);
	drv=4;				/* Start bei C: = 3. Bit */
	ret='A';
	for (i=0;i<14;i++)
	{
		if (drvbits & drv)      /* Fastdrive found */
		{	ret='C'+i;
			break;
		}
		drv <<= 1;	
	}
		
	return(ret);
}

word
get_clipname(byte *s)
{
	byte scrp_path[MAX_STR];
	byte *nul;
	struct ffblk dta;
	word setnew_fl=FALSE;

	scrp_read(scrp_path); /* Returnwert ist falsch */

	if (strlen(scrp_path)<=0L)		/* Pfad gibs vordefiniert */
	{	setnew_fl=TRUE;
		sprintf(scrp_path,"%c:\\CLIPBRD",get_fastdrv());
	}
	nul=strchr(scrp_path,EOS);
	nul--;
	if (*nul==BSLASH)
		*nul=EOS;
		
	if (findfirst(scrp_path,&dta,FA_DIREC)) /* not exist */
	{
		if (Dcreate(scrp_path)<0)
		{
			LangAlert(MSG_CREATECLIPBRDERROR);
			set_clipmenu(TRUE);
			return FALSE;
		}	
	}
	strcpy(s,scrp_path);
	nul=strchr(s,EOS);
	if (*(nul-1)!=BSLASH)
	{
		*nul=BSLASH;
		*(nul+1)=EOS;
	}	
	if (setnew_fl)
		scrp_write(s);	

	strcat(s,"SCRAP.TXT");
	return TRUE;
}

/*
**	0 = byte
**	1 = uword
**	2 = byte bitfield
**	3 = word
**	4 = word bitfield
**	5 = string
**	6 = string (convert 'cr' to '|')
*/

OPT_LIST optlist[] =
{	{	"Fonts",							1,	&gl_fntzahl			},
	{	"Save OPT file",			0,	&opt.saveopt		},
	{	"Max columns",				0,	&opt.maxcol			},
	{	"Max edit columns",		0,	&opt.maxedcol		},
	{	"HDR cache",					0,	&opt.hdrcache		},
	{	"Insert",							0,	&opt.insert			},
	{	"Request [DF]",				2,	&opt.request		},
	{	"Save DLG",						0,	&opt.savedlg		},
	{	"Hide MSGID/REPLY",		0,	&opt.hideeid		},
	{	"Hide PID/TID/EID",		0,	&opt.hidepid		},
	{	"Hide other ^A",			0,	&opt.hideca			},
	{	"Hide SEEN-BY",				0,	&opt.hideseen		},
	{	"Show cr",						0,	&opt.showcr			},
	{	"Extend tabs",				0,	&opt.exttab			},
	{	"Mutations [AMIEN]",	2,	&opt.umlaut			},
	{	"Quoting [CTSFDNMX]",	2,	&opt.quote			},
	{	"Flags [APIT]",				2,	&opt.flag				},
	{	"Movemsg [FK]",				2,	&opt.movemsg		},
	{	"Del NewMsgFlags",		0,	&opt.delnewmsg	},
	{	"Texteffects [PS]",		2,	&opt.texteffects},
	{	"Show commenttree",		0,	&opt.showcom		},
	{	"Mailerfields",				0,	&opt.commailer	},
	{	"CompleteTree",				0,	&opt.completescan},
	{	"Smart redraw",				0,	&opt.sredraw		},
	{	"Save lastreadpointer",0,	&opt.savelread	},
	{	"FontID MsgWind",			3,	&opt.msgid			},
	{	"FontID EditWind",		3,	&opt.editid			},
	{	"FontID MsgListWind",	3,	&opt.mlistid		},
	{	"FontID AreaListWind",3,	&opt.alistid		},
	{	"FontPNT MsgWind",		0,	&opt.msgpt			},
	{	"FontPNT EditPNT",		0,	&opt.editpt			},
	{	"FontPNT MsgListWind",0,	&opt.mlistpt		},
	{	"FontPNT AreaListWind",0,	&opt.alistpt		},
	{	"Reverse block",			0,	&opt.blockrev		},
	{	"Thick quotes",				0,	&opt.qthick			},
	{	"Tabsize",						0,	&opt.tabsize		},
	{	"Indent",							0,	&opt.indent			},
	{	"'Re:'-style [NA]",		2,	&opt.addre			},
	{	"NumPad commentmode",	0,	&opt.numpadcomment},
	{	"AreaListWindow",			0,	&opt.listawindow},
	{	"MsgListWindow",			0,	&opt.listmwindow},
	{	"Printing [IGF]",			2,	&opt.printopt		},
	{	"Netmail AC",					0,	&opt.addrcornet	},
	{	"Fixed Address",			0,	&opt.always_fixed},
	{	"Show description",		0,	&opt.showdesc		},
	{	"Use commenttree",		0,	&opt.usecommenttree},
	{	"Use REPLYTO",				0,	&opt.usereplyto	},
	{	"Use REPLYADDR",			0,	&opt.usereplyaddr},
	{	"Monospaced fonts",		0,	&opt.monospaced	},
	{	"Autoclose [MA]",			2,	&opt.autoclose	},
	{	"Deadkeys [/,o\":`\~^]",4,&opt.deadkeyflag},
	{	"Topline native",			6,	&opt.nattop			},
	{	"Topline english",		6,	&opt.engtop			},
	{	"AreaTopline native",	6,	&opt.nat2top		},
	{	"AreaTopline english",6,	&opt.eng2top		},
	{	"Areas detected",			3,	&gl_arzahl			},
	{	"STARTpath",					5,	&gl_startpath		},
	{	"LEDpath",						5,	&gl_ledpath			},
	{	"OUTBOUNDpath",				5,	&gl_outpath			},
	{	"AREAS.BBSpath",			5,	&gl_areasbbspath},
	{	"BINKLEY.CFGpath",		5,	&gl_binkleypath	},
	{	"LED.NEWpath",				5,	&gl_lednewpath	},
	{	"NODELISTpath",				5,	&gl_nodelistpath},
	{	"NODELISTtyp",				3,	&gl_nodelisttype},
	{	"Other options",			2,	&gl_options			},
#if 0
	{	"FIDOSOFT.CFGpath",		5,	&gl_fidosoftpath},
#endif
	{	"Logfile",						5,	&gl_logfile			},
	{	"Echofile",						5,	&gl_echofile		},
	{	"Resourcefile",				5,  &gl_ledrsc			},
	{	NULL,									0,	NULL						}};

static uword bootdev;
static long lpeek_bootdev(void)
{ bootdev=*((uword *)0x446);
	return 0;
}

void write_report(void)
{	FILE *fp;
	static byte file[MAX_STR];
	byte *pointer;
	long gesamt,largest;
	word i, blocks, a,b,c,d;
	MAGX_COOKIE *MagX;
	ulong value;
	struct ffblk dta;
	
	if (hole_file(file,"REPORT.LED","REPORT.LED",msgtxt[FILE_SYSTEMREPORT].text))
	{	
		show_doing(DO_SAVESYSTEMREPORT);
		biene();

		fp=fopen(file,"w");
		if (fp==NULL)
		{	LangAlert(MSG_REPORTOPENERROR);
			mouse_normal();
			del_doing();
			return;
		}	

		test_registered();
		fprintf(fp,"LED %s%s system report\n\n",version,
																						is_registered ? "" : unreg);

		get_memory(&gesamt,&largest,&blocks);
		fprintf(fp,"Free memory: %ld (%d blocks, largest block %lu)\n\n",gesamt,blocks,largest);

		fprintf(fp,"Sysop: %s\n\n",gl_ptowner);

		fputs("AkaSysop:\n",fp);
		for (i=0; i<=akasysopcount; i++)
			fprintf(fp,"%s\n",akasysop[i]);

		fputs("\nAddresses:\n",fp);
		for (i=0; i<=addrcount; i++)
			fprintf(fp,"%s\n",mergeaddr(addr[i].zone,addr[i].net,addr[i].node,addr[i].point,addr[i].domain));

		fputs("\nDomains:\n",fp);
		for (i=0; i<=domaincount; i++)
			fprintf(fp,"%-20s %s\n",gl_domain[i][0],gl_domain[i][1]);

		fputs("\nUserlists:\n",fp);
		for (i=0;i<MAX_USERLIST;i++)
		{	if (strlen(opt.userlist[i])>0L)
			{	fprintf(fp,"%u. %s",i+1,opt.userlist[i]);
				if (*opt.userlistdomain[i])
					fprintf(fp,"   (%s)",opt.userlistdomain[i]);
				fputc('\n',fp);
			}
		}

		fprintf(fp,"\nDesktop  : %d, %d, %d, %d\n",gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
		fprintf(fp,"Workplace: %d, %d, %d, %d\n\n",full.x,full.y,full.w,full.h);

		i=0;
		while(optlist[i].info)
		{	switch(optlist[i].mode)
			{	case 0 :	fprintf(fp,"%-20s  %u\n",	optlist[i].info,
																						(uword) *(byte*)optlist[i].pnt);
									break;
				case 1 :	fprintf(fp,"%-20s  %u\n",	optlist[i].info,
																						*(uword*)optlist[i].pnt);
									break;
				case 2 :	fprintf(fp,"%-20s  %s\n",	optlist[i].info,
																						itoa(*(byte*)optlist[i].pnt,file,2));
									break;
				case 3 :	fprintf(fp,"%-20s  %d\n",	optlist[i].info,
																						*(word*)optlist[i].pnt);
									break;
				case 4 :	fprintf(fp,"%-20s  %s\n",	optlist[i].info,
																						itoa(*(word*)optlist[i].pnt,file,2));
									break;
				case 5 :	fprintf(fp,"%-20s  %s\n",	optlist[i].info,
																						(byte*)optlist[i].pnt);
									break;
				case 6 :	fprintf(fp,"%-20s  ",	optlist[i].info);
									for (pointer=(byte*)optlist[i].pnt;*pointer;pointer++)
										if (*pointer=='\r')
											fputc('|',fp);
										else
											fputc(*pointer,fp);
									fputc('\n',fp);
									break;
			}
			i++;
		}

		sprintf(file,"%sLED.NEW",gl_ledpath);
		if (findfirst(file,&dta,0))
			sprintf(file,"%sLED.NEW",gl_lednewpath);
		fprintf(fp,"\nReading/Writing LED.NEW: %s\n",file);

		sprintf(file,"%sLED.CFG",gl_ledpath);
		if (findfirst(file,&dta,0))
		{	
#if 0
			sprintf(file,"%sFIDOSOFT.CFG",gl_fidosoftpath);
			if (findfirst(file,&dta,0))
#endif
				sprintf(file,"%sBINKLEY.CFG",gl_binkleypath);
		}
		fprintf(fp,"\nUsed config file: %s\n",file);
		fprintf(fp,"\nEnvironments:\n");
		pointer=getenv("FIDOCONFDIR");
		if (pointer) fprintf(fp,"FIDOCONFDIR=%s\n",pointer);
		pointer=getenv("LED");
		if (pointer) fprintf(fp,"LED=%s\n",pointer);
		pointer=getenv("BINKLEY");
		if (pointer) fprintf(fp,"BINKLEY=%s\n",pointer);
		pointer=getenv("MAILER");
		if (pointer) fprintf(fp,"MAILER=%s\n",pointer);
		pointer=getenv("SHELL");
		if (pointer) fprintf(fp,"SHELL=%s\n",pointer);

		a=get_fontindex(opt.msgid);
		b=get_fontindex(opt.editid);
		c=get_fontindex(opt.mlistid);
		d=get_fontindex(opt.alistid);
		fprintf(fp,"\nFont MsgWind     : %s\n",gl_font[a].fname);
		fprintf(fp,"Font EditWind    : %s\n",gl_font[b].fname);
		fprintf(fp,"Font MsgListWind : %s\n",gl_font[c].fname);
		fprintf(fp,"Font AreaListWind: %s\n\n",gl_font[d].fname);
		
		a=(opt.texteffects & O_TEXTEFFPRN && opt.texteffects & O_TEXTEFFSCR);
		b=(prn_table[TEON_U][1]!=TEON_U &&
			 prn_table[TEON_B][1]!=TEON_B &&
			 prn_table[TEON_C][1]!=TEON_C &&
			 prn_table[TEON_I][1]!=TEON_I &&
			 prn_table[TEOFF_U][1]!=TEOFF_U &&
			 prn_table[TEOFF_B][1]!=TEOFF_B &&
			 prn_table[TEOFF_C][1]!=TEOFF_C &&
			 prn_table[TEOFF_I][1]!=TEOFF_I);
		fprintf(fp,"Printing texteffects is... %s\n",a?"enabled":"disabled");
		fprintf(fp,"Printing texteffects is... %s\n\n",b?"possible":"impossible");

		fprintf(fp,"Printercodes:\n");
		for (i=TEON_U; i<=TEOFF_I; i++)
		{	switch(i)
			{	case	TEON_U	:	fputs("U+: ",fp);	break;
				case	TEON_B	:	fputs("B+: ",fp);	break;
				case	TEON_C	:	fputs("C+: ",fp);	break;
				case	TEON_I	:	fputs("I+: ",fp);	break;
				case	TEOFF_U	:	fputs("U-: ",fp);	break;
				case	TEOFF_B	:	fputs("B-: ",fp);	break;
				case	TEOFF_C	:	fputs("C-: ",fp);	break;
				case	TEOFF_I	:	fputs("I-: ",fp);	break;
				default				:	fprintf(fp,"%02x: ",i);
			}
			a=prn_table[i][0];
			if (!a)
				fprintf(fp,"empty\n");
			else if (a==i)
				fprintf(fp,"undefined\n");
			else
			{	for (b=1;b<=a;b++)
					fprintf(fp,"%02x ",(uword)prn_table[i][b]);
				putc('\n',fp);
			}
		}

		fprintf(fp,"Printeroptions:\n");
		fprintf(fp,"Left margin: %u\n",prn_options[OPT_LEFTMARGIN]);

		fprintf(fp,"Printerinits:\n");
		for (i=0; i<32; i++)
		{	if (prn_divs[i][0])
			{	switch(i)
				{	case	DIV_INIT	:	fputs("INIT: ",fp); break;
					default					: fprintf(fp,"(%02x): ",i);
				}
				a=prn_divs[i][0];
				for (b=1;b<=a;b++)
					fprintf(fp,"%02x ",(uword)prn_divs[i][b]);
				putc('\n',fp);
			}
		}
		putc('\n',fp);

		MagX = getcookie( 'MagX', &value ) ? (MAGX_COOKIE *) value : NULL;
		if (MagX && MagX->aesvars /*&& MagX->aesvars->version>=0x200*/)
			fprintf(fp,"MagX %u.%c%c installed\n",
												MagX->aesvars->version >> 8,
												'0'+ ((MagX->aesvars->version >> 4) & 0xf),
												'0'+  (MagX->aesvars->version & 0xf) );
		if (wind_get(0,'WX',&i,&i,&i,&i)=='WX')
			fprintf(fp,"Winx installed\n");
		fprintf(fp,"AES-Version %u.%c%c\n",
										aes_version >> 8,
										'0'+ ((aes_version >> 4) & 0xf),
										'0'+  (aes_version & 0xf) );

		fputs("\nShells:\n",fp);
		for (i=0; i<10; i++)
			if (gl_shell[i])
				fprintf(fp,"%2u %s %s\n",i+1,gl_shell[i],gl_shellcmd[i]);

		Supexec(lpeek_bootdev);
		fprintf(fp,"\nAUTO folder programs on drive %c:\n",65+bootdev);
		sprintf(file,"%c:\\AUTO\\*.PRG",65+bootdev);
		if (!findfirst(file,&dta,0))
		{	fprintf(fp,"%12s %lu\n",dta.ff_name,dta.ff_fsize);
			while(!findnext(&dta))
				fprintf(fp,"%12s %lu\n",dta.ff_name,dta.ff_fsize);
		}

		fputs("\nAreas:\n",fp);
		for (i=0; i<gl_arzahl; i++)
		{	if (gl_area[i].zone || gl_area[i].net || gl_area[i].node)
				fprintf(fp,"%03u. %s   %s\n",i+1,gl_area[i].name,mergeaddr(gl_area[i].zone,gl_area[i].net,gl_area[i].node,0,gl_area[i].domain));
			else
				fprintf(fp,"%03u. %s\n",i+1,gl_area[i].name);
			if (gl_area[i].flag & AR_NETMAIL)
				fprintf(fp,"     Netmail domain: %s\n",gl_area[i].domain ? gl_area[i].domain : "not defined");
			
			if (gl_area[i].path)				fprintf(fp,"     -> %s\n",gl_area[i].path);
			if (gl_area[i].topline)			fprintf(fp,"     Topline: %s\n",gl_area[i].topline);
			if (gl_area[i].followup)		fprintf(fp,"     FollowUp: %s\n",gl_area[i].followup);
			if (gl_area[i].rndfootpath)	fprintf(fp,"     FootlineFile: %s\n",gl_area[i].rndfootpath);
		}

		fclose(fp);
		del_doing();
		mouse_normal();
	}
}

