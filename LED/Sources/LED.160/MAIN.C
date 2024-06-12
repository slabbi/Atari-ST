#include "ccport.h"
#include "ledscan.h"
#include "vars.h"
#include "menu.rh"
#include "allareas.rh"
#include "listmsg.rh"
#include "ledgl.h"
#include "ledfc.h"
#include "switches.rh"

byte version[]      = "1.60";
byte pidversion[]   = "1.60";
word dezversion			=	160;
word hexversion			= 0x160;
byte versart[]      = "Official";     /* 'betatest' or 'Official' */
word betatest				= FALSE;

RECHTECK full;
OPT opt;

word handle;

OBJECT *menu_tree, *about_tree;
OBJECT *allarea_tree, *listmsg_tree;
OBJECT *gomsg_tree, *edflag_tree;
OBJECT *doing_tree, *killmsg_tree, *edline_tree;
OBJECT *nodenum_tree;
OBJECT *hmsg_tree, *hedit_tree, *hdial_tree;
OBJECT *freq_tree, *header_tree, *header2_tree, *header3_tree;
OBJECT *find_tree, *fkeys_tree, *fsame_tree, *carbon_tree;
OBJECT *seluser_tree, *font_tree, *exec_tree;
OBJECT *writemsg_tree;
OBJECT *switches_tree, *window_tree, *quote_tree, *user_tree;
OBJECT *username_tree, *helpcom_tree;
OBJECT *comment_tree;
OBJECT *hmsg2_tree, *info_tree;
OBJECT *selorig_tree, *msginfo_tree, *thanks_tree;
OBJECT *icons_tree;

DIALINFO about_dial;
DIALINFO allarea_dial, listmsg_dial;
DIALINFO gomsg_dial, ledopt_dial, edflag_dial;
DIALINFO doing_dial, killmsg_dial, edline_dial;
DIALINFO nodenum_dial;
DIALINFO hmsg_dial, hedit_dial, hdial_dial;
DIALINFO freq_dial, header_dial, header2_dial, header3_dial;
DIALINFO find_dial, fkeys_dial, fsame_dial, carbon_dial;
DIALINFO seluser_dial, font_dial, exec_dial;
DIALINFO writemsg_dial;
DIALINFO switches_dial, window_dial, quote_dial, user_dial;
DIALINFO username_dial, helpcom_dial;
DIALINFO comment_dial;
DIALINFO hmsg2_dial, info_dial;
DIALINFO selorig_dial, msginfo_dial, thanks_dial;

word gl_apid, aes_version;
word gl_hchar, gl_wchar, gl_hbox, gl_wbox;
word gl_syshchar, gl_syswchar, gl_syshbox, gl_syswbox;
word gl_pointsize;

word gl_xdesk, gl_ydesk, gl_hdesk, gl_wdesk, gl_x2desk, gl_y2desk;

byte fselpath[MAX_STR], savename[14];

WIND wi[TOTAL_WIND];
FNT  gl_font[MAX_FONTS];
word gl_fntzahl;

word gl_arzahl;
word gl_mailarea;
/* AREAS gl_area[MAX_AREAS]; */

byte gl_fkeys[20][80];

ADDRESS addr[MAX_ADDRESSES];
word 		addrcount;
byte 		gl_domain[MAX_DOMAINS][2][17];
word 		domaincount;

byte akasysop[MAX_SYSOP][40];
word akasysopcount;
byte pseudonymsysop[MAX_PSEUDONYMS][40];
word pseudonymarea[MAX_PSEUDONYMS];
word pseudonymsysopcount;

byte *nicknames[MAX_NICKNAMES][3];	/* 0-Nickname, 1=Name, 2=Area */
word nicknamecnt;

byte gl_ptowner[40];         			/* name of point operator        */

byte gl_startpath[MAX_STR];
byte gl_ledpath[MAX_STR];
byte gl_outpath[MAX_STR];
byte gl_logfile[MAX_STR];
byte gl_echofile[MAX_STR];
byte gl_areasbbspath[MAX_STR];
byte gl_binkleypath[MAX_STR];
byte gl_lednewpath[MAX_STR];
byte gl_ledprg[MAX_STR];
byte gl_ledrsc[MAX_STR];
byte gl_nodelistpath[MAX_STR];
word gl_nodelisttype;
word gl_options;

#if 0
	byte gl_fidosoftpath[MAX_STR];
#endif

byte *gl_shell[10];
byte *gl_shellcmd[10];

KEYTAB *gl_key_p;
word gl_otherquotefl;
long gl_oldwin=-1L;

word menu_keys[] =								/* Frei: Ctrl+I, ALT+D und ALT+V */
{	/* LED,     DABOUT,   CNTRL_I, */
	FFILE,   FREAD,    ALT_R,
	FFILE,   FWRITE,   ALT_W,
	FFILE,   FAPPEND,  ALT_A,
	FFILE,   FPWRITE,  SHIFT_ALT_W,
	FFILE,   FPAPPEND, SHIFT_ALT_A,
	FFILE,   FRCLIP,   ALT_L,
	FFILE,   FWCLIP,   ALT_B,
	FFILE, 	 FSHELL,   ALT_Z,
	FFILE,   FEXEC,    ALT_E,
	FFILE,   FQUIT,    CNTRL_Q,
	AREA,    AOPEN,    CNTRL_O,
	AREA,    ACHANGE,  CNTRL_A,
	AREA,    ACYCLE,   CNTRL_W,
	AREA,    ACLOSE,   CNTRL_U,
	MSG,     MLIST,    CNTRL_L,
	MSG,     MGOTO,    CNTRL_G,
	MSG,     MKILL,    CNTRL_K,
	MSG,     MHEAL,    CNTRL_H,
	MSG,     MFORWARD, CNTRL_F,
	MSG,     MMOVE,    CNTRL_M,
	MSG,     MPRINT,   CNTRL_P,
	EDIT,    EENTER,   CNTRL_E,
	EDIT,    EREPLY,   CNTRL_R,
	EDIT,    EQUOTE,   CNTRL_T,
	EDIT,    EQUONET,  CNTRL_N,
	EDIT, 	 EQUOAREA, CNTRL_J,
	EDIT,    ECHANGE,  ALT_C,
	EDIT,    ECC,      ALT_X,
	EDIT,    EABORT,   CNTRL_Z,
	EDIT,    ESAVE,    CNTRL_S,
	BLOCK,   BBEGIN,   CNTRL_B,
	BLOCK,   BDELMARK, CNTRL_D,
	BLOCK,   BCUT,     CNTRL_X,
	BLOCK,   BCOPY,    CNTRL_C,
	BLOCK,   BPASTE,   CNTRL_V,
	BLOCK,   BINDENT,  ALT_I,
	BLOCK,   BEXTEND,  ALT_T,
	OPTIONS, OSWITCH,  ALT_O,
	OPTIONS, OWIND,    ALT_J,
	OPTIONS, OQUOTE,   ALT_N,
	OPTIONS, OUSER,    ALT_Y,
	OPTIONS, OFONT,    ALT_M,
	OPTIONS, OFUNC,    ALT_K,
	OPTIONS, OSAVE,    ALT_S,
	EXTRAS,  OFREQ,    ALT_Q,
	EXTRAS,  OMHEADER, SHIFT_ALT_H,
	EXTRAS,  OMHEAD2,  ALT_H,
	EXTRAS,  OPRVTUSE, ALT_P,
	EXTRAS,  OUSERLST, ALT_U,
	EXTRAS,  OFIND,    ALT_F,
	EXTRAS,  OFSAME,   ALT_G,
	HELP,		 HSHORT,	 CNTRL_F1,
	-1
};

word gl_list_obj[MAX_LIST] =
{L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13,L14,L15,L16,L17,L18};

word gl_lown_obj[MAX_LIST] =
{LE1,LE2,LE3,LE4,LE5,LE6,LE7,LE8,LE9,LE10,LE11,LE12,LE13,LE14,LE15,LE16,LE17,LE18};


word gl_area_obj[AREAS_SEITE] = 
{AA0,AA1,AA2,AA3,AA4,AA5,AA6,AA7,AA8,AA9,AA10,AA11,AA12,AA13,AA14,AA15,
 AA16,AA17,AA18,AA19,AA20,AA21,AA22,AA23,AA24,AA25,AA26,AA27,AA28,
 AA29,AA30,AA31,AA32,AA33,AA34,AA35,AA36,AA37,AA38,AA39,AA40,AA41,
 AA42,AA43,AA44,AA45,AA46,AA47,AA48,AA49,AA50,AA51,AA52,AA53,AA54,
 AA55,AA56,AA57,AA58,AA59};

word gl_arnew_obj[AREAS_SEITE] =
{AN0,AN1,AN2,AN3,AN4,AN5,AN6,AN7,AN8,AN9,AN10,AN11,AN12,AN13,AN14,AN15,
 AN16,AN17,AN18,AN19,AN20,AN21,AN22,AN23,AN24,AN25,AN26,AN27,AN28,
 AN29,AN30,AN31,AN32,AN33,AN34,AN35,AN36,AN37,AN38,AN39,AN40,AN41,
 AN42,AN43,AN44,AN45,AN46,AN47,AN48,AN49,AN50,AN51,AN52,AN53,AN54,
 AN55,AN56,AN57,AN58,AN59};

extern word is_deadkey(word deadkey);
extern word convert_deadkey(word deadkey, word key);

#if 0
	struct time TIME;
#endif

void multi(void)
{
	word verlassen, key, deadkey=0;
	word state, shf, mx, my, nk, oldkey, old_umlaut, flags;
	word msgbuff[8];
	word ev;
	long win;
	
	verlassen = FALSE;

	flags=MU_MESAG|MU_BUTTON|MU_KEYBD;
	test_registered();
	if (!is_registered)
		flags|=MU_TIMER;

	do
	{
#if 1
		ev=evnt_multi( flags,
			               2, 1, 1,
			               0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		        	       msgbuff, 30000, 0, &mx, &my, &state, &shf,
		          	     &key, &nk);
#else
		ev=evnt_multi( flags|MU_TIMER,
			               2, 1, 1,
			               0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		        	       msgbuff, 1000, 0, &mx, &my, &state, &shf,
		          	     &key, &nk);
#endif
		if (ev & MU_TIMER)
		{	
#if 0
			byte tmp[10];
#endif
			win=found_wind();
			if (cripple(win))
				flags &= ~MU_TIMER;
#if 0
			gettime(&TIME);
			sprintf(tmp,"%02u:%02u:%02u",TIME.ti_hour,TIME.ti_min,TIME.ti_sec);
			vs_clip(handle,FALSE,msgbuff);
			v_gtext(handle,full.x+full.w-(gl_wbox*15),gl_hbox,tmp);
			set_wiclip(win);
			set_wifont(win);
#endif
		}

		if (ev & MU_BUTTON)
		{	win=found_wind();
			if (win>=0 && !wi[win].iconify)
			{	if (edit_window(win))
					edbutton_event(win,mx,my,nk,state);
				else if (list_window(win))
					list_edbutton(win,mx,my,nk);
				else
					msg_button(win,mx,my,nk,shf);
			}
		}


		if (ev & MU_MESAG)
			if (msgbuff[0]==0x5142)
			{	ev=MU_KEYBD;
				key=msgbuff[4];
				shf=msgbuff[3];
			}
		if (ev & MU_MESAG)
			verlassen |= hndl_wind(msgbuff);

		if (ev & MU_KEYBD)
		{	win=found_wind();

			if (iconify_possible && win>=0)
				if (handle_kbd_iconify(win,get_code(key,shf),key,shf))
					continue;

			if (!title_select())
			{	key=get_code(key, shf);
				
				old_umlaut = opt.umlaut & (O_UMLAUT_N|O_UMLAUT_E);

				if (win>=0)
				{	if (!deadkey && edit_window(win) && is_deadkey(key))
					{	deadkey=key;
						continue;
					}

					if (deadkey && edit_window(win))
					{	oldkey=key;
						key=convert_deadkey(deadkey,key);
						if (key==oldkey)
							verlassen |= edkey_event(win, deadkey);
						else
							opt.umlaut |= O_UMLAUT_N|O_UMLAUT_E;
					}
				}
				deadkey=0;

				if (win>=0 && edit_window(win))
					verlassen |= edkey_event(win, key);
				else if (win>=0 && list_window(win))
					verlassen |= list_keybd(win, key);
				else
					verlassen |= hndl_keybd(win, key);

				if (key!=ALT_O)
					opt.umlaut = (opt.umlaut & ~(O_UMLAUT_N|O_UMLAUT_E)) | old_umlaut;
			}
		}
	} while (!verlassen);

	if (gl_options & EDITWINDOW_USED)
	{	if (gl_options & CALLEXPORTER_AVALON)	start_export(0);
		if (gl_options & CALLEXPORTER_SEMPER)	start_export(1);
	}
}

static void birthday(void)
{	char temp[MAX_STR];
	struct date HEUTE;
	getdate(&HEUTE);
	if ( (HEUTE.da_day==21) && (HEUTE.da_mon==10) )
	{	sprintf(temp," Happy birthday |      (%2d)|  dear Stephan",HEUTE.da_year-1970);
		FormAlert(1,0,temp," Yeah! ");
	}
}

word main()
{
	if (open_vwork())
	{
		birthday();
		WindUpdate(BEG_UPDATE);
		if (deklaration())
		{	WindUpdate(END_UPDATE);
			multi();
			thanks();
		}
		else
			WindUpdate(END_UPDATE);

		speicher_freigeben();
		language_speicher_freigeben();
		register_speicher_freigeben();

		if (vq_gdos())
			vst_unload_fonts(handle,0);
		DialExit();

		/* redraw_all(); */
		form_dial(FMD_FINISH,full.x,full.y,full.w,full.h,full.x,full.y,full.w,full.h);
		/* ssl 060195 */
		rsrc_free();
		v_clsvwk(handle);
		ende();
		return(0);
	}
	return(1);
}
