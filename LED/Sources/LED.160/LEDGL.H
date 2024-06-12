#ifdef __NEWFIDO__
  #include "fido_new.h"
#else
  #include "fido_old.h"
  #include <magx.h>
#endif

typedef struct
{	uword	zeile;
	uword	indent;
	ulong	Msgid;
	ulong	Reply;
} TREE;

typedef struct ADDRESStag
{	uword zone;
	uword net;
	uword node;
	uword point;
	char	domain[16];
} ADDRESS;
#define MAX_ADDRESSES	100
#define MAX_DOMAINS		10

typedef struct or
{	byte	*text;
	struct or *next;
	struct or *prior;
} ORIG;

#define TEON_U	16		/* underline on		*/
#define TEOFF_U 17		/* underline off	*/
#define TEON_B	18		/* bold	on				*/
#define TEOFF_B 19		/* bold	off				*/
#define TEON_C	20		/* cursive on			*/
#define TEOFF_C 21		/* cursive off		*/
#define TEON_I	22		/* inverse on			*/
#define TEOFF_I	23		/* inverse off		*/

#define MAX_SYSOP				10
#define	MAX_PSEUDONYMS	20
#define MAX_NICKNAMES		100

enum	{	FIRST, ORIGINAL, PREV, NEXT };

#define NEWMSG      0x0001
#define UNRDMSG     0x0002
#define TOSYSOP     0x0004

#define LEDVERSION	0x0126

/* #define MAX_AREAS   512 */
extern uword MAX_AREAS;

#define MAXZONE     5
#define AREAS_SEITE 60
#define MAX_COL     (80+5)
#define MAX_EDCOL		(79+5)
#define LINELEN     (82+5)	/* 2 grîûer als MAX_COL */

#define MAX_WIND    4				/* von 0 bis 3 = 4 Windows */
#define	TOTAL_WIND	6				/* von 0 bis 5 = 6 Windows */
#define MSGSWIND		4				/* Nr.4 ist MsgWindow			 */
#define AREAWIND		5				/* Nr.5 ist AreaWindow		 */
#define WI_LIST_MSGS  1
#define WI_LIST_AREAS	2

#define MAX_WINOPT	7 /* ssl 81294 */
#define MAX_WINDS		6

#define MAX_LINES   	5000
#define MAX_MSG     	5000

#define MAX_LIST    	18
#define MAX_CC      	40
#define MAX_FONTS   	100
#define MAX_PNTSIZE		10
#define MAX_USERLIST	12

#define EOS		'\0'
#define CR		'\r'
#define LF		'\n'
#define SLASH	'/'
#define COLON	':'
#define BSLASH	'\\'
#define POINT	'.'

#define MSG_WIND		1
#define EDIT_WIND		2
#define NO_WIND			3
#define DEL_MSG			4
#define NO_MSG			5
#define MLIST_WIND	6
#define ALIST_WIND	7

#define END_OFMSG 		10
#define START_OFMSG 	20
#define NO_MSGINAREA	30
#define ST_BLANK			40
#define NO_CHANGE			50

#define ED_ENTER 1
#define ED_REPLY 2
#define ED_QUOTE 3

#define QUOTE_UNSET			0
#define QUOTE_NON	  		1
#define QUOTE_MSG				2
#define QUOTE_NACHRICHT 3
#define QUOTE_NATIVE    4
#define QUOTE_ENGLISH   5

#define LEER 99

#define W_ALL (NAME|INFO|CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)
#define W_MSG (NAME|CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE)

/* extern TREE tree[MAX_MSG]; */
extern byte fm_sound_dta[];

extern ADDRESS 	addr[MAX_ADDRESSES];
extern word 		addrcount;
extern byte 		gl_domain[MAX_DOMAINS][2][17];
extern word 		domaincount;

extern byte akasysop[MAX_SYSOP][40];
extern word akasysopcount;
extern byte pseudonymsysop[MAX_PSEUDONYMS][40];
extern word pseudonymarea[MAX_PSEUDONYMS];
extern word pseudonymsysopcount;

extern byte *nicknames[MAX_NICKNAMES][3];
extern word nicknamecnt;

extern uword is_registered;

extern byte version[];
extern byte pidversion[];
extern word dezversion;
extern word hexversion;
extern byte versart[];

extern word handle;

extern OBJECT *menu_tree, *about_tree, *msginfo_tree;
extern OBJECT *allarea_tree, *listmsg_tree;
extern OBJECT *gomsg_tree, *edflag_tree;
extern OBJECT *doing_tree, *killmsg_tree, *edline_tree;
extern OBJECT *nodenum_tree;
extern OBJECT *hmsg_tree, *hedit_tree, *hdial_tree;
extern OBJECT *freq_tree, *header_tree, *header2_tree, *header3_tree;
extern OBJECT *find_tree, *fkeys_tree, *fsame_tree, *carbon_tree;
extern OBJECT *seluser_tree, *font_tree, *exec_tree;
extern OBJECT *writemsg_tree;
extern OBJECT *switches_tree, *window_tree, *quote_tree, *user_tree;
extern OBJECT *username_tree, *helpcom_tree;
extern OBJECT *comment_tree;
extern OBJECT *hmsg2_tree, *info_tree;
extern OBJECT *selorig_tree, *thanks_tree;
extern OBJECT *icons_tree;

extern DIALINFO about_dial;
extern DIALINFO allarea_dial, listmsg_dial;
extern DIALINFO gomsg_dial, ledopt_dial, edflag_dial;
extern DIALINFO doing_dial, killmsg_dial, edline_dial;
extern DIALINFO nodenum_dial;
extern DIALINFO hmsg_dial, hedit_dial, hdial_dial;
extern DIALINFO freq_dial, header_dial, header2_dial, header3_dial;
extern DIALINFO find_dial, fkeys_dial, fsame_dial, carbon_dial;
extern DIALINFO seluser_dial, font_dial, exec_dial;
extern DIALINFO writemsg_dial;
extern DIALINFO switches_dial, window_dial, quote_dial, user_dial;
extern DIALINFO username_dial, helpcom_dial;
extern DIALINFO comment_dial;
extern DIALINFO hmsg2_dial, info_dial;
extern DIALINFO selorig_dial, msginfo_dial, thanks_dial;

extern word gl_apid, aes_version;
extern word gl_hchar, gl_wchar, gl_hbox, gl_wbox;
extern word gl_syshchar, gl_syswchar, gl_syshbox, gl_syswbox;
extern word gl_fntzahl;
extern word gl_xdesk, gl_ydesk, gl_hdesk, gl_wdesk, gl_x2desk, gl_y2desk;

extern byte fselpath[], savename[];

typedef struct RECHTECK
{	word x;
	word y;
	word w;
	word h;
} RECHTECK;	
extern RECHTECK full;
	
extern word menu_keys[];

#define MAX_STATUSLINE	256

typedef struct wind
{
	byte belegt;			/* Flag, Window belegt									 				*/
	byte iconify;			/* Flag, Window ikonifiziert										*/
	byte editierbar;	/* Flag, Edit-Window 										 				*/
	byte listwind;		/* Flag, List-Window 														*/
	byte newmsg;			/* Flag, ob Enter=TRUE, Quote,Reply etc.=FALSE	*/
	byte changearea;	/* Flag, ob OPEN_AREA oder CHANGE_AREA					*/

	byte ramhdr;			/* Flag, TRUE wenn HDR-Cache aktiv 							*/
	byte insert;			/* Insert-Modus (EditWindow)										*/
	byte cursor;			/* Flag, Cursor da 															*/

	word handle;			/* Window-Handle 																*/
	word gadgets;			/* Window-Gadgets																*/
	word x;						/* Pixel, Start Window-Workarea 								*/
	word y;						/* Pixel, Start Window-Workarea 								*/
	word h;						/* Pixel, Hoehe Window-Workarea 								*/
	word w;						/* Pixel, Breite Window-Workarea 								*/
	word xb;					/* VOR Iconify																	*/
	word yb;					/* VOR Iconify																	*/
	word wb;					/* VOR Iconify																	*/
	word hb;					/* VOR Iconify																	*/
	word px;					/* Pixel, Previous Window-Border 								*/
	word py;					/* Pixel, Previous Window-Border 								*/
	word pw;					/* Pixel, Previous Window-Border						 		*/
	word ph;					/* Pixel, Previous Window-Border						 		*/
	word firstc;			/* Zeichen, erste Spalte in Wind-Work					 	*/
	word firstl;			/* Zeichen, erste Zeile in Wind-Work					 	*/
	word oldcx;				/* Zeichen, Alte Cursorposition x 							*/
	word cx;					/* Zeichen, Cursorposition x 										*/
	word cy;					/* Zeichen, Cursorposition y 										*/
	long msgfilelen;	/* LÑnge des MSG-Files 													*/
	long hdrlen;			/* Laenge des HDR-Files 												*/
	FILE *msgfp;			/* FILE, Handle fuer Messagefile						 		*/
	FILE *hdrfp;			/* FILE, Handle fuer Headerfile							 		*/
	FILE *hdrfp2;			/* FILE, dito. gebraucht fuer MsgWindowupdate		*/

	word indent;			/* Auto Indent im Edit-Window 									*/
	word topmsg_line;
	word selline;
	word lnr;					/* Nummer der akt. Zeile 												*/
	word maxshow;			/* Zahl (Zeichen) der max. Zeilen in Window 		*/
	word lzahl;				/* Zahl der Zeilen in Msg-Puffer 								*/
	word bspalten;		/* Zahl der Spalten in Window (Edit) 						*/
	word bzeilen;			/* Zahl der Zeilen in Window (Edit) 						*/
	word topline;			/* Index der 1. Zeile im Window aus Msg-Puffer 	*/

	word firstmsg;		/* Nummer erster Msg in Area 										*/
	word lastmsg;			/* Nummer letzter Msg in Area 									*/
	word aktmsg;			/* Nummer der akt. Msg 													*/
	word oldmsg;			/* Nummer der ehemals akt. Msg 									*/
	word killchange;	/* Nr der zu lîschenden alten Msg 							*/
	word aktarea;			/* Nummer der akt. Area 												*/
	word oldarea;			/* Nr der alten Area (z.B. quotenet_msg)				*/

	byte msgda[MAX_MSG];/* Feld mit Flags fuer jede Msg (Index) 			*/
	byte areaname[82];	/* akt. Area-Name 														*/
	byte areapath[MAX_STR];	/* akt. Area-Pfad 												*/

	byte *hdrbuf;			/* Pointer auf Buffer im HDR-Cache 							*/
	byte *hdrptr;			/* Pointer auf akt. HDR im HDR-Cache 						*/
	byte *pos;				/* Pointer auf aktuelles Zeichen, Edit  				*/
	byte *msgbuf;			/* pointer auf Message-Puffer 									*/
	byte *edline;			/* pointer auf akt. Editier-Zeile 							*/
	byte *lines[MAX_LINES]; /* pointer-vektor auf Zeilen in Msg-Puffer*/

	HDR  *hdr;				/* Pointer auf Header-Struktur 									*/

	byte status[MAX_STATUSLINE];	/* Window-Statuszeile 							*/
#if 0
	byte alert[128];							/* Alert-Text for Msgs							*/
#endif
	byte dup_msgid[256];
	byte dup_replyid[256];
	byte dup_replyto[256];
	byte dup_replyaddr[256];
} WIND;

#define O_UMLAUT_N		1		/* fÅr opt.umlaut */
#define O_UMLAUT_E		2
#define O_ISOCONV			4
#define O_MIMESTYLE		8
#define O_ATARIFONT	 16

#define O_QUOTE_XY		(1U << 0)
#define O_QUOTE_MSG		(1U << 1)
#define O_QUOTE_NAT		(1U << 2)
#define O_QUOTE_DET		(1U << 3)
#define O_QUOTE_FRE		(1U << 4)
#define O_QUOTE_MSET	(1U << 5)
#define O_QUOTE_NOTOP	(1U << 6)
#define O_QUOTE_NCOMP	(1U << 7)	/* No Compress quote */

#define O_REQUEST_FLO			1
#define	O_REQUEST_DOMAIN	2

#define O_ADDRE		1
#define O_ADDREN	2

#define O_TEXTEFFSCR	1
#define O_TEXTEFFPRN	2

#define O_MOVEMSG_KEEP	1
#define O_MOVEMSG_FORWD	2

#define O_PRINTOPT_FF			1
#define O_PRINTOPT_GDOS		2
#define O_PRINTOPT_INFO		4

#define O_AUTOCLOSE_AREA	1
#define O_AUTOCLOSE_MSGS	2

#define O_FLAG_RANDOMTEAR		1
#define O_FLAG_GEMINFOLINE	2
#define O_FLAG_SHOWPOS			4
#define	O_FLAG_AKASYSOP			8

#define DKEY_DACH			(1 << 0)
#define DKEY_SCHLANGE	(1 << 1)
#define DKEY_A				(1 << 2)
#define DKEY_BACKA		(1 << 3)
#define	DKEY_DPNT			(1 << 4)
#define DKEY_LITERAL	(1 << 5)
#define DKEY_KREIS		(1 << 6)
#define DKEY_KOMMA		(1 << 7)
#define DKEY_SLASH		(1 << 8)
#define DKEY_MASTER   (1 << 9)

typedef struct 
{	word version;
	uword winprm[MAX_WINDS][MAX_WINOPT];	/* [6]=1 Window automatisch îffnen */
	byte saveopt;
	byte maxcol;
	byte maxedcol;
	byte hdrcache;
	byte insert;
	byte request;				/* bitfeld */
	byte savedlg;
	byte hideeid;
	byte hideca;
	byte hideseen;
	byte showcr;
	byte exttab;
	byte umlaut;				/* bitfeld */
	byte quote;					/* bitfeld */
	byte flag;					/* bitfeld */
	byte movemsg;				/* bitfeld */
	byte delnewmsg;
	byte completescan;
	byte texteffects;		/* bitfeld */
	byte showcom;
	byte commailer;
	byte sredraw;
	byte savelread;

	word msgid;
	word editid;
	word mlistid;
	word alistid;

	byte msgpt;
	byte editpt;
	byte mlistpt;
	byte alistpt;

	byte blockrev;
	byte qthick;
	byte tabsize;
	byte indent;
	byte addre;					/* bitfeld */
	byte numpadcomment;
	byte listawindow;
	byte listmwindow;
	byte printopt;			/* bitfeld */
	byte addrcornet;
	byte always_fixed;
	byte showdesc;
	byte usecommenttree;
	byte usereplyto;
	byte hidepid;
	byte monospaced;
	byte usereplyaddr;
	byte autoclose;			/* bitfeld */

	byte 		_free1;
	byte 		_free2;
	byte 		_free3;
	word deadkeyflag;
	ulong	led_register;
	byte nattop[80];
	byte engtop[80];
	byte nat2top[80];
	byte eng2top[80];

	byte userlist[MAX_USERLIST][128];
	byte userlistdomain[MAX_USERLIST][32];
} OPT;

#define AR_MAUS							(1ul << 0)
#define AR_UMLAUTE					(1ul << 1)
#define AR_EDITED						(1ul << 2)
#define AR_FIXEDADDRESS			(1ul << 3)
#define AR_NONEWECHO				(1ul << 4)
#define AR_FORCEPVT					(1ul << 5)
#define AR_CONVERT_LAT1S		(1ul << 6)
#define AR_CONVERT_LAT1			(1ul << 7)
#define AR_IGNORENEWMSGFL		(1ul << 8)
#define AR_IGNOREUNREADFL 	(1ul << 9)
#define AR_RANDOMTEARLINE		(1ul << 10)
#define AR_NORANDOMTEARLINE (1ul << 11)
#define AR_NOEXPORT					(1ul << 12)
#define AR_COMMENTTREEON		(1ul << 13)
#define AR_COMMENTTREEOFF		(1ul << 14)
#define AR_FOLLOWUP					(1ul << 15)
#define AR_NOTOPLINE				(1ul << 16)
#define AR_OWNTOPLINE				(1ul << 17)
#define AR_NETMAIL					(1ul << 18)

typedef struct ar
{	byte *name;
	byte *path;
	byte *desc;
	byte *rndfootpath;
	byte *followup;
	byte *topline;
	byte *domain;
	byte quotetype;
	word followupnr;
	word lread,
			 newmsg;
	long flag;
	uword	zone,
				net,
				node;
} AREAS;

typedef struct us
{	byte name[36];
	byte node[25];
	word userlist_nr;
	struct us *next;
	struct us *prior;
} USER;

typedef struct fnt
{	byte fname[25];
	word fid;
	byte fpoint[MAX_PNTSIZE];
} FNT;

extern OPT   opt;
extern WIND  wi[TOTAL_WIND];
extern FNT   gl_font[MAX_FONTS];

extern AREAS *gl_area;
/* extern AREAS gl_area[MAX_AREAS]; */

extern word gl_pointsize;

extern word gl_area_obj[AREAS_SEITE];
extern word gl_arnew_obj[AREAS_SEITE];

extern word gl_arzahl;

extern word gl_list_obj[MAX_LIST];
extern word gl_lown_obj[MAX_LIST];

extern byte gl_fkeys[20][80];

extern word gl_mailarea;
extern byte gl_ptowner[];         /* name of point operator        */

extern byte gl_startpath[];
extern byte gl_ledpath[];
extern byte gl_outpath[];
extern byte gl_logfile[];
extern byte gl_echofile[];
extern byte gl_areasbbspath[];
extern byte gl_binkleypath[];
extern byte gl_lednewpath[];
extern byte gl_ledprg[];
extern byte gl_ledrsc[];
extern byte gl_nodelistpath[];

extern word gl_nodelisttype;
#define BTNC 				0

extern word gl_options;
#define OUTBOUND36					(1 <<  0)
#define CALLEXPORTER_AVALON	(1 <<  1)
#define CALLEXPORTER_SEMPER	(1 <<  2)
#define EDITWINDOW_USED			(1 << 15)

#if 0
	extern byte gl_fidosoftpath[];
#endif

extern byte *gl_shell[10];
extern byte *gl_shellcmd[10];

extern KEYTAB *gl_key_p;
extern word gl_otherquotefl;
extern long gl_oldwin;
