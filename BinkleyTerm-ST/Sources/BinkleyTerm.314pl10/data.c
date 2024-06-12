/*--------------------------------------------------------------------------*/
/*																			*/
/*																			*/
/*		------------		 Bit-Bucket Software, Co.						*/
/*		\ 10001101 /		 Writers and Distributors of					*/
/*		 \ 011110 / 		 Freely Available<tm> Software. 				*/
/*		  \ 1011 /															*/
/*		   ------															*/
/*																			*/
/*	(C) Copyright 1987-90, Bit Bucket Software Co., a Delaware Corporation. */
/*																			*/
/*																			*/
/*			  This module was originally written by Bob Hartman 			*/
/*																			*/
/*																			*/
/*							  BinkleyTerm Data								*/
/*																			*/
/*																			*/
/*	  For complete	details  of the licensing restrictions, please refer	*/
/*	  to the License  agreement,  which  is published in its entirety in	*/
/*	  the MAKEFILE and BT.C, and also contained in the file LICENSE.240.	*/
/*																			*/
/*	  USE  OF THIS FILE IS SUBJECT TO THE  RESTRICTIONS CONTAINED IN THE	*/
/*	  BINKLEYTERM  LICENSING  AGREEMENT.  IF YOU DO NOT FIND THE TEXT OF	*/
/*	  THIS	AGREEMENT IN ANY OF THE  AFOREMENTIONED FILES,	OR IF YOU DO	*/
/*	  NOT HAVE THESE FILES,  YOU  SHOULD  IMMEDIATELY CONTACT BIT BUCKET	*/
/*	  SOFTWARE CO.	AT ONE OF THE  ADDRESSES  LISTED BELOW.  IN NO EVENT	*/
/*	  SHOULD YOU  PROCEED TO USE THIS FILE	WITHOUT HAVING	ACCEPTED THE	*/
/*	  TERMS  OF  THE  BINKLEYTERM  LICENSING  AGREEMENT,  OR  SUCH OTHER	*/
/*	  AGREEMENT AS YOU ARE ABLE TO REACH WITH BIT BUCKET SOFTWARE, CO.		*/
/*																			*/
/*																			*/
/* You can contact Bit Bucket Software Co. at any one of the following		*/
/* addresses:																*/
/*																			*/
/* Bit Bucket Software Co.		  FidoNet  1:104/501, 1:132/491, 1:141/491	*/
/* P.O. Box 460398				  AlterNet 7:491/0							*/
/* Aurora, CO 80046 			  BBS-Net  86:2030/1						*/
/*								  Internet f491.n132.z1.fidonet.org 		*/
/*																			*/
/* Please feel free to contact us at any time to share your comments about	*/
/* our software and/or licensing policies.									*/
/*																			*/
/*--------------------------------------------------------------------------*/

/* 1993 (c) St.Slabihoud  Anpassungen an TT-High Aufl�sung */

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#ifdef LATTICE
#include <stdlib.h>
#else
#include <process.h>
#endif


#include "bink.h"
#include "defines.h"
#include "com.h"
#include "keybd.h"
#include "sched.h"
#include "zmodem.h"
#include "sbuf.h"
#include "vfossil.h"

char **msgtxt;

char *mtext [] = {
				  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
				  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};



struct baud_str btypes[] = {
							{300,	BAUD_300},
							{1200,	BAUD_1200},
							{2400,	BAUD_2400},
							{4800,	BAUD_4800},
							{7200,  BAUD_9600},
							{9600,	BAUD_9600},
							{12000, BAUD_19200},
							{14400, BAUD_19200},
							{16800, BAUD_19200},
							{19200, BAUD_19200},
							{21600, BAUD_38400},
							{24000, BAUD_38400},
							{26400, BAUD_38400},
							{28000, BAUD_38400},
							{38400, BAUD_38400},
                            {57600, BAUD_57600},
							{64000, BAUD_76800},
                            {76800, BAUD_76800},
                            {115200,BAUD_115200},
                            {153600,BAUD_153600},
							{0, 	0},
};

struct baud_str max_baud =	{2400,	BAUD_2400}; /* Max baud value */



int locate_x, locate_y, port_ptr=0, original_port;

int TaskNumber = 0; 							 /* unique user-specified id   */
int needtime = 0;
int share = 0;									 /* if 1, don't uninit FOSSIL  */
int first_block;								 /* 1 = first block/Matrix
												  * packet */
int net_params = 0;
int un_attended = 0;
int command_line_un;
int overwrite = 0;
int redo_dynam = 0;
int fullscreen = 1;
int signalhandler = 0;
int do_screen_blank = 0;
int screen_blank = 0;
int blank_on_key = 1;
unsigned BBStimeout = 2000;
int gong_allowed = 0;
#ifdef ATARIST
int MAXport = 1;		/* Default to 1 comms port on Atari ST */
#else
int MAXport = 2;
#endif
int poll_tries = 500;							 /* How many tries in a poll? */
int poll_delay = 20;							/* Delay between polls in 10th second */

int com_port = 0;								 /* Default com port = COM1   */
int reset_port = FALSE;							 /* Reset ports on exit ?	  */
unsigned int comm_bits = BITS_8;				 /* Default data bits = 8	  */
unsigned int parity = NO_PARITY;				 /* Default parity = NONE	  */
unsigned int stop_bits = STOP_1;				 /* Default stop bits = 1	  */
unsigned int carrier_mask = 0x80;				 /* Default carrier bit = 80H */
unsigned int handshake_mask =  USE_XON | USE_CTS;/* Force CTS/RTS flow control*/

long baud;										 /* Index to baud table 	  */
unsigned long cur_baud;							 /* Current baud rate		  */

char *modem_firstinit = NULL;						/* Used to init modem (startup sequence only) */
char *modem_init = "|AT|";							/* Used to init modem		 */
char *modem_reset = "|AT|";							/* Used to reset modem after exiting Binkley */
char *term_init = NULL; 							/* Used to init modem in terminal mode */
char *modem_busy = "|AT|";							/* Take modem offhook		 */
char *predial = NULL;								/* Pre-dial setup			 */
char *postdial = NULL;								/* Post-dial cleanup		 */
char *normprefix;
char *normsuffix;
char *dial_setup;
char *init_setup;

unsigned int matrix_mask = TAKE_REQ;			 /* Netmail session abilities */
char *system_name = "Unnamed BinkleyTerm System";	/* Name of system to send	 */
char *sysop = "Unknown SysOp";						/* Name of sysop for yoohoo  */
char *NL_System = NULL;
char *NL_Phone = "-Unpublished-";				/* Number to be added to IDENT */
char *NL_City = NULL;							/* City for IDENT */
char *NL_Flags = NULL;							/* NodeList Flags for IDENT */
char *NL_Baud = NULL;							/* Baud rate for IDENT */

char *net_info = ".\\"; 						 /* Location of nodelist, etc */
char *fax_prg  = NULL;							 /* Location of Fax programm  */
char *afterfax_prg = NULL;						 /* Location of AfterFax programm */
char *fax_connect = NULL;						 /* String to look for faxconnect */
char *fax_inbound = ".\\";						 /* Where to store incoming faxes */
char *time_zone = NULL;						 	 /* Time zone (GMT+x) */
char *hold_area = NULL; 						 /* Path to outbound dir	  */
char *flag_dir = NULL;							 /* Where to find INMAIL.$$$  */
char *domain_area = NULL;						 /* Where to put domain name  */
char *domain_loc = NULL;						 /* Where to put domain name  */
ADDRESS alias[ALIAS_CNT] = { 0 };				/* Array of our addresses */
ADKEY *adkeys = NULL;							/* Linked list of key structures */
int num_addrs = 0;								 /* How many we're using	  */
int assumed = 0;								 /* Alias we're now assuming  */
int last_assumed = 0;							/* Last assumed address for display */

int pvtnet = -1;								/* Default fakenet */
int Zone = 0;									/* Default Zone */
char *BOSSphone = NULL; 						 /* Phone number for boss	  */

BOOLEAN iosmode = FALSE;						/* Use 4D addressing */
BOOLEAN usecallslots = FALSE;					/* Use CallSlots instead of cost */
BOOLEAN FDBugfix = FALSE;						/* Take not other code in ZMODEM loop */

char *CurrentNetFiles;							 /* Pointer to netfiles 	  */
char *CurrentOKFile = NULL; 					 /* This session's OKfile	  */
char *CurrentFILES = NULL;						 /* File list for FILES 	  */
char *CurrentAbout = NULL;						 /* Sent if req fails		  */
char *CurrentReqTemplate = NULL;				 /* Dynamic req failure file  */
int CurrentReqLim = 0;							/* Max number of files		 */
long CurrentByteLim = 0L;						/* Maximum number of bytes	 */
short CurrentTimeLim = 0;						/* Maximum Freq time */

struct secure DEFAULT;							 /* Default paths, etc		  */
struct secure KNOWN;							 /* Sec for KNOWN address	  */
struct secure PROT; 							 /* Sec for PW-PR address	  */

struct req_accum freq_accum;					 /* Accumulator for freq stats*/

char *domain_name[MAXDOMAIN];							/* Known domain names		 */
char *domain_abbrev[MAXDOMAIN]; 						/* Known domain name abbrev  */
char *domain_nodelist[MAXDOMAIN];						/* Known domain nodelists	 */
byte *Txbuf;									 /* Pointer to xmit buffer	  */
byte *Secbuf;									 /* Pointer to recv buffer	  */
#ifdef POPBUF
  byte *popbuf; 								 /* Pointer to popup buffer   */
#endif

char *native_protocols = "XZTS";				 /* Protocols we support	  */
char *extern_protocols = NULL;					 /* Protocols we can call	  */
char *protocols[MAX_EXTERN];					 /* external file protocol	  */
int extern_index = 0;							 /* external proto index	  */
struct prototable protos[MAX_EXTERN];			 /* Table for execution 	  */

char *ext_mail_string[16];						 /* String for UUCP, etc.	  */
int lev_ext_mail[16];							 /* Errorlevel for extmail	  */
int num_ext_mail;								 /* Number of ext mails 	  */

char *BBSopt = NULL;							 /* BBS command options 	  */
char *BBSbanner = NULL; 						 /* BBS banner				  */
char *BBSreader = NULL; 						 /* Message reader			  */
char *download_path = "";						 /* Default download path	  */
char *BBSnote = NULL;							 /* Tell user BBS loading	  */
char *MAILnote = NULL;							 /* Say same for ext mail	  */

char *BINKpath = "";
char capturename[140];
char *config_name = "Binkley.Cfg";

char *scan_list[10];
int  set_loaded = 0;

byte user_exits[6];

char junk[255];

PN_TRNS *pn = NULL;
MDM_TRNS *mm = NULL;
int boxtype = 0;
int modemring = 0;								 /* Modem doesn't differ for
												  * RING and RINGING */
int no_collide = 0; 							 /* No collision detect */
int cursor_col = 79;
int cursor_row = 22;
int leave_dtr_high = 0;
char *ans_str = NULL;
char *keys[N_SHELLS];
char *shells[N_SHELLS];

char *prodcode[] = {
					"Fido",
					"Rover",
					"SEAdog",
					"Unknown",
					"Slick/150",
					"Opus",
					"Dutchie",
					"Unknown",
					"Tabby",
					"Hoster",
					"Wolf/68k",
					"QMM",
					"FrontDoor",
					"Unknown",
					"Unknown",
					"Unknown",
					"Unknown",
					"MailMan",
					"Oops",
					"GS-Point",
					"BGMail",
					"CrossBow",
					"Unknown",
					"Unknown",
					"Unknown",
					"Unknown",
					"D'Bridge",
					"BinkleyTerm",
					"Yankee",
					"FGet/Send",
					"Daisy",
					"Polar Bear",
					"The-Box",
					"Warlock",
					"TMail",
					"TCOMMail",
					"Bananna",
					"RBBSMail",
					"Apple-netmail",
					"Chameleon",
					"Majik Board",
					"QMail",
					"Point and Click",
					"Aurora Three",
					"FourDog",
					"MSG-PACK",
					"AMAX",
					"Domain",
					"LesRobot",
					"Rose",
					"Paragon",
					"Binkley/ST",
					"StarNet",
					"ZzyZx",
					"QuickBBS",
					"BOOM",
					"PBBS",
					"TrapDoor",
					"Welmat",
					"Unknown",
					"Unknown",
					"Unknown",
					"Unknown",
					"Unknown",
					"Unknown",
					"Unknown",
					"TIMS",
					"Isis",
					NULL
};

char ctrlc_ctr;
int old_fossil = 1;


char no_requests = 0;
int who_is_he = 0;

int curmudgeon = 0; 							 /* 1 = Randy Bush			 */
int small_window = 0;
int no_overdrive = 0;
int no_resync = 0;
int no_sealink = 0;
int immed_update = 0;

unsigned long janus_baud = 0;

/* If this is set.. the tx buffer must be empty before sending a new packet */
BOOLEAN slowjanus;

ADDR called_addr;
int mail_only;
int caller;
int more_mail;
int doing_poll;

char BBSwelcome[1024];
char *aftermail;


int slowmodem = 0;

PN_TRNS *pn_head = NULL;
MDM_TRNS *mm_head = NULL;

unsigned long lock_baud = 0;

char sending_program[32];

char e_input[255];

char *IDUNNO_msg = "???";
char *local_CEOL = "\033K";	/* What the F*** is this doing in here! */

char *wkday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

HISTORY hist;
BTEVENT *e_ptrs[256];
int requests_ok = 1;
int num_events = 0;
int cur_event = -1;
int next_event = 0;
int got_sched = 0;
int noforce = 0;
int no_zones = 0;
int fastscreen = 0;
int nodomains = 0;
int max_connects = 3;
int max_noconnects = 10000;
int server_mode = 0;
char *packer;
char *cleanup;
char *answerback;

long etm = 0L;
long file_length = 0L;

#if 0	/* Unused? */
long TX_FileSize;
long TX_StTime;
int TX_OldETA;

long RX_FileSize;
long RX_StTime;
int RX_OldETA;

byte *XFER_str = (byte *) " pos=        , len=    , ETA=     min.";
#endif

char *script_path;								 /* Where the script files
												  * live */


byte Resume_WaZOO;								 /* Flags resumption of
												  * aborted WaZOO xfer	*/
byte Resume_name[13];							 /* "Real" name of file being
												  * received	  */
byte Resume_info[48];							 /* File size&time info for
												  * rx file 		*/
byte Abortlog_name[PATHLEN];					 /* Path of WaZOO aborted
												  * xfer log	  */

#ifdef NEW /* COLORS  22.09.1989 */
/*	Background, Settings, History, Hold, Call, File, Calling, PopUp, Headers, Borders */
COLORS colors = { 112, 7, 7, 7, 7, 7, 112, 7, 0, 0};
#else
COLORS colors = { 7, 7, 7, 7, 7, 7, 112, 7 };
#endif

#if (!defined(OS_2) && !defined(ATARIST))
struct vfossil_hooks vfossil_funcs;
#endif

int vfossil_installed = 0;			/* Fossil not installed!!!! */

#if 0
VIOMODEINFO vfos_mode;
#endif
char blanks[264];

char *logptr;

MAILP mail_top;
MAILP next_mail;

J_TYPESP j_top;
J_TYPESP j_next;
int janus_OK;
char  mdm_reliable[80];



/* First, the polynomial itself and its table of feedback terms.  The  */
/* polynomial is													   */
/* X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0 */
/* Note that we take it "backwards" and put the highest-order term in  */
/* the lowest-order bit.  The X^32 term is "implied"; the LSB is the   */
/* X^31 term, etc.	The X^0 term (usually shown as "+1") results in    */
/* the MSB being 1. 												   */

/* Note that the usual hardware shift register implementation, which   */
/* is what we're using (we're merely optimizing it by doing eight-bit  */
/* chunks at a time) shifts bits into the lowest-order term.  In our   */
/* implementation, that means shifting towards the right.  Why do we   */
/* do it this way?	Because the calculated CRC must be transmitted in  */
/* order from highest-order term to lowest-order term.	UARTs transmit */
/* characters in order from LSB to MSB.  By storing the CRC this way,  */
/* we hand it to the UART in the order low-byte to high-byte; the UART */
/* sends each low-bit to hight-bit; and the result is transmission bit */
/* by bit from highest- to lowest-order term without requiring any bit */
/* shuffling on our part.  Reception works similarly.				   */

/* The feedback terms table consists of 256, 32-bit entries.  Notes:   */
/*																	   */
/*	   The table can be generated at runtime if desired; code to do so */
/*	   is shown later.	It might not be obvious, but the feedback	   */
/*	   terms simply represent the results of eight shift/xor opera-    */
/*	   tions for all combinations of data and CRC register values.	   */
/*																	   */
/*	   The values must be right-shifted by eight bits by the "updcrc"  */
/*	   logic; the shift must be unsigned (bring in zeroes).  On some   */
/*	   hardware you could probably optimize the shift in assembler by  */
/*	   using byte-swap instructions.								   */

unsigned long cr3tab[] = {				  /* CRC polynomial 0xedb88320 */
0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL, 0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL,
0x0edb8832UL, 0x79dcb8a4UL, 0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL, 0x90bf1d91UL,
0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL, 0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL,
0x136c9856UL, 0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL, 0xfa0f3d63UL, 0x8d080df5UL,
0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL, 0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL, 0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL,
0x26d930acUL, 0x51de003aUL, 0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL, 0xb8bda50fUL,
0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL, 0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL,
0x76dc4190UL, 0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL, 0x9fbfe4a5UL, 0xe8b8d433UL,
0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL, 0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL, 0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL,
0x65b0d9c6UL, 0x12b7e950UL, 0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL, 0xfbd44c65UL,
0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL, 0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL,
0x4369e96aUL, 0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL, 0xaa0a4c5fUL, 0xdd0d7cc9UL,
0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL, 0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL, 0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL,
0xedb88320UL, 0x9abfb3b6UL, 0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL, 0x73dc1683UL,
0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL, 0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL,
0xf00f9344UL, 0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL, 0x196c3671UL, 0x6e6b06e7UL,
0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL, 0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL, 0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL,
0xd80d2bdaUL, 0xaf0a1b4cUL, 0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL, 0x4669be79UL,
0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL, 0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL,
0xc5ba3bbeUL, 0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL, 0x2cd99e8bUL, 0x5bdeae1dUL,
0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL, 0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL, 0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL,
0x86d3d2d4UL, 0xf1d4e242UL, 0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL, 0x18b74777UL,
0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL, 0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL,
0xa00ae278UL, 0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL, 0x4969474dUL, 0x3e6e77dbUL,
0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL, 0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL, 0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL,
0xb3667a2eUL, 0xc4614ab8UL, 0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL, 0x2d02ef8dUL
};

/*
 *
 * -rev 04-16-87  (abbreviated)
 *	The CRC-16 routines used by XMODEM, YMODEM, and ZMODEM
 *	are also in this file, a fast table driven macro version
 */


/* crctab calculated by Mark G. Mendel, Network Systems Corporation */
unsigned short crctab[256] = {
			 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
			 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
			 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
			 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
			 0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
			 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
			 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
			 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
			 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
			 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
			 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
			 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
			 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
			 0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
			 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
			 0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
			 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
			 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
			 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
			 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
			 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
			 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
			 0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
			 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
			 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
			 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
			 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
			 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
			 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
			 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
			 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
			 0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

char *stat_str =	"%c %02i:%02i:%02i %		 ";
char *script_line = "Script Line %03d: %		 ";
char *BadChars = "\026\021\b\377\376\375\374";

BOOLEAN no_zapzed = FALSE;
char *BBSesc = "\rPress <Escape> to enter BBS.\r";
char *noBBS = "\r\rProcessing Mail. Please hang up.\r\r";
BOOLEAN no_pickup = FALSE;
BOOLEAN no_WaZOO = FALSE;
BOOLEAN no_WaZOO_Session = FALSE;


BOOLEAN SendRSP = FALSE;			/* Set this for old style RSP files */
char seperators[] = "; \t\r\n";


int niceoutbound = 0;
size_t blklen;
int reinit_time = 10;
int reinit_ctr = 10;
int readhold_time = 1440;
int readhold_ctr = 1440;
#if 0
FILE *cost_log = NULL;
char *cost_log_name = NULL;
int ReqTimeLimit = 0;
int SuckerFlag = 0;
int SuckerTimeLimit = 1439;
int cost_unit = 23;
int hstls2400c = 0;
int no_wildcards = 0;
int req_mins = 0;
#endif	/* 0 */

BOOLEAN HoldsOnUs = FALSE;			/* If set then send held mail even on outgoing call! */
STR_LIST *hold_list = NULL;			/* List of nodes for which HoldOnUs is valid */
BOOLEAN no_EMSI = FALSE;			/* Disable EMSI when set */
BOOLEAN no_REQUEST = FALSE;			/* Disable Requests when set */
BOOLEAN no_JANUS = FALSE;			/* Disable Janus when set */
BOOLEAN no_Janus_Session = FALSE;
STR_LIST *emsi_list = NULL;			/* List of Nodes not to use emsi with */
STR_LIST *request_list = NULL;		/* List of Nodes which are not allowed to request */
STR_LIST *janus_list = NULL;		/* List of Nodes not to use janus with */
BOOLEAN on_our_nickel = FALSE;
STR_LIST *reqonus_list = NULL;		/* List of Nodes to allow REQs on us */
STR_LIST *tranx_list = NULL;		/* Nodes to accept tranx's from */

BOOLEAN batch_mode = FALSE;			/* Running in Batch Mode */



