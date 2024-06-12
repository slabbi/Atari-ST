#include <aes.h>
#include <scancode.h>
#include <my_scan.h>
#include "define.h"
#include "version.h"
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "transkey.h"
#include "form_run.h"
#include <fido_msg.h>
#include "vars.h"

extern void send_message(int id,int fm_id,long fm_from,long command,int extend);

int trans_message(void)
	{	int cwahl=-1;
		unsigned long value;
		value=((long)HEXVERSION) << 16;

#if defined( _AVALSEMP_ )
		value |= 0x00;
#else
		value |= 0x11;
#endif

		switch (formrun_msgbuf[6])
			{	case AVAL_FIX		: cwahl = DOFIX;		break;
				case AVAL_AREAS	: cwahl = DOAREAS;	break;
				case AVAL_REQ		: cwahl = DOFREQ;		break;
				case AVAL_HATCH	:	cwahl = DOHATCH;	break;
#if !defined( _AVALSEMP_ )
				case AVAL_EXPORT:	cwahl = DOEXPORT; break;
				case AVAL_IMPORT:	cwahl = DOIMPORT; break;
				case AVAL_CRUNCH:	cwahl = DOCRUNCH; break;
				case AVAL_SCAN	:	cwahl = DOSCAN; 	break;
				case AVAL_MSGED	:	cwahl = DOMSGED; 	break;
				case AVAL_TXTED	:	cwahl = DOTXTED; 	break;
				case AVAL_MAILER:	cwahl = DOMAILER; break;
				case AVAL_TERM	:	cwahl = DOTERM; 	break;
#endif
				case AVAL_INFO	:	{	send_message(formrun_msgbuf[1],
																					FIDO_INFO,FM_AVALON,value,0);
													} break;
#if !defined( _AVALSEMP_ )
				case AVAL_POLL	: {	switch(formrun_msgbuf[5])
															{	case	0	:	cwahl = STARTSEL; break;
																case	1 : cwahl = START1;		break;
																case	2 : cwahl = START2;		break;
																case	3 : cwahl = START3;		break;
																case	4 : cwahl = START4;		break;
																case	5 : cwahl = START5;		break;
																case	6 : cwahl = START6;		break;
															}
													} break;
#endif
			}
		if (cwahl!=-1)
			wind_set(handle,WF_TOP);
		return(cwahl);
	}



#if defined( _AVALSEMP_ )

int trans_menu(int obj_nr)
	{	int cwahl=-1;
		switch (obj_nr)
			{	case MAREAS		:	cwahl = DOAREAS;	break;
				case MINFO		: cwahl = DOINFO;		break;
				case MQUIT		: cwahl = DOQUIT;		break;
				case MFILEREQ	: cwahl = DOFREQ;		break;
				case MHATCH		: cwahl = DOHATCH;	break;
				case MFIX			: cwahl = DOFIX;		break;
				case MSAVE		: cwahl = DSAVE;		break;
				case MSPEZIAL :	cwahl = DOSPEZIA; break;
			}
		return(cwahl);
	}

int trans_key(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_A : cwahl = DOAREAS;		break;
											case ALT_E : cwahl = DOHATCH;		break;
											case ALT_I : cwahl = DOINFO;		break;
											case ALT_M : cwahl = DOFIX;			break;
											case ALT_P : cwahl = DOSPEZIA;	break;
											case ALT_Q : cwahl = DOQUIT;		break;
											case ALT_R : cwahl = DOFREQ;		break;
											case ALT_S : cwahl = DSAVE;			break;
										}; break;
				case 4	: switch(key)
										{	case CNTRL_Q : cwahl = DOQUIT;	break;
											case KEY_NUM_CNTRL_STAR	: cwahl = 1234; break;
										}; break;
			}
		return(cwahl);
	}

#else

int trans_menu(int obj_nr)
	{	int cwahl=-1;
		switch (obj_nr)
			{	case MRESTART	: cwahl = STARTSEL;	break;
				case MREREAD	: cwahl = TBOX; 		break;
				case MAUTO1		: cwahl = AUTO1;		break;
				case MAUTO2		: cwahl = AUTO2;		break;
				case MAUTO3		: cwahl = AUTO3;		break;
				case MAUTO4		: cwahl = AUTO4;		break;
				case MAUTO5		: cwahl = AUTO5;		break;
				case MAUTO6		: cwahl = AUTO6;		break;
				case MAREAS		:	cwahl = DOAREAS;	break;
				case MFINDPRG	: cwahl = DOSETUP2;	break;
				case MFINDUTL	: cwahl = DOSETUP3;	break;
				case MPRG			: cwahl = DOPRG;		break;
				case MINFO		: cwahl = DOINFO;		break;
				case MALL			: cwahl = DOALL;		break;
				case MSSETUP	: cwahl = DOSETUP1;	break;
				case MQUIT		: cwahl = DOQUIT;		break;
				case MFILEREQ	: cwahl = DOFREQ;		break;
				case MHATCH		: cwahl = DOHATCH;	break;
				case MFIX			: cwahl = DOFIX;		break;
				case MSAVE		: cwahl = DSAVE;		break;
				case MMANAGER	: cwahl = DMANAGER;	break;
				case MPMANAGE	: cwahl = DOPROGS;	break;
				case MFTERM		: cwahl = 1001;			break;
				case MTOS			: cwahl = DOTOS;		break;
				case MTTP			: cwahl = DOTTP;		break;
				case MAPP			: cwahl = DOAPP;		break;
				case MSTAR		: cwahl = DOSTAR;		break;
				case MMAILER	: cwahl = DOMAILER;	break;
				case MCRUNCH	: cwahl = DOCRUNCH;	break;
				case MEXPORT	: cwahl = DOEXPORT;	break;
				case MIMPORT	: cwahl = DOIMPORT;	break;
				case MMSGEDIT	: cwahl = DOMSGED;	break;
				case MSCANNER	: cwahl = DOSCAN;		break;
				case MTXTEDIT	: cwahl = DOTXTED;	break;
				case MUTIL1		: cwahl = DOUTIL1;	break;
				case MUTIL2		: cwahl = DOUTIL2;	break;
				case MUTIL3		: cwahl = DOUTIL3;	break;
				case MUTIL4		: cwahl = DOUTIL4;	break;
				case MUTIL5		: cwahl = DOUTIL5;	break;
				case MUTIL6		: cwahl = DOUTIL6;	break;
				case MTERM		: cwahl = DOTERM;		break;
				case MPOLL1		: cwahl = START1;		break;
				case MPOLL2		: cwahl = START2;		break;
				case MPOLL3		: cwahl = START3;		break;
				case MPOLL4		: cwahl = START4;		break;
				case MPOLL5		: cwahl = START5;		break;
				case MPOLL6		: cwahl = START6;		break;
		 /* case MHELP		: cwahl = 1000;			break; */
				case MNOTE		: cwahl = DONOTE;		break;
				case MPOINT1	: cwahl = POINT0;		break;
				case MPOINT2	: cwahl = POINT1;		break;
				case MPOINT3	: cwahl = POINT2;		break;
				case MPOINT4	: cwahl = POINT3;		break;
				case MPOINT5	: cwahl = POINT4;		break;
				case MPOINT6	: cwahl = POINT5;		break;
				case MSPEZIAL :	cwahl = DOSPEZIA; break;
			}
		return(cwahl);
	}

int trans_key(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_1 			: cwahl = AUTO1;		break;
											case KEY_NUM_1	: cwahl = AUTO1;		break;
											case ALT_2 			: cwahl = AUTO2;		break;
											case KEY_NUM_2	: cwahl = AUTO2;		break;
											case ALT_3 			: cwahl = AUTO3;		break;
											case KEY_NUM_3	: cwahl = AUTO3;		break;
											case ALT_4 			: cwahl = AUTO4;		break;
											case KEY_NUM_4	: cwahl = AUTO4;		break;
											case ALT_5 			: cwahl = AUTO5;		break;
											case KEY_NUM_5	: cwahl = AUTO5;		break;
											case ALT_6 			: cwahl = AUTO6;		break;
											case KEY_NUM_6	: cwahl = AUTO6;		break;
											case ALT_A : cwahl = DOAREAS;		break;
											case ALT_B : cwahl = DOALL;			break;
											case ALT_E : cwahl = DOHATCH;		break;
											case ALT_F : cwahl = DOSETUP2;	break;
											case ALT_G : cwahl = DOPRG;			break;
											case ALT_H : cwahl = DOSETUP3;	break;
											case ALT_I : cwahl = DOINFO;		break;
											case ALT_M : cwahl = DOFIX;			break;
											case ALT_O : cwahl = DOSETUP1;	break;
											case ALT_P : cwahl = DOSPEZIA;	break;
											case ALT_Q : cwahl = DOQUIT;		break;
											case ALT_R : cwahl = DOFREQ;		break;
											case ALT_S : cwahl = DSAVE;			break;
											case ALT_T : cwahl = DOTOS;			break;
											case ALT_U : cwahl = DOAPP;			break;
											case ALT_V : cwahl = DOSTAR;		break;
											case ALT_X : cwahl = 1001;			break;
											case ALT_Z : cwahl = DOTTP;			break;
										}; break;
				case 4	: switch(key)
										{	case CNTRL_B : cwahl = DOMAILER;	break;
											case CNTRL_F : cwahl = DMANAGER;	break;
											case CNTRL_C : cwahl = DOCRUNCH;	break;
											case CNTRL_E : cwahl = DOEXPORT;	break;
											case CNTRL_I : cwahl = DOIMPORT;	break;
											case CNTRL_M : cwahl = DOMSGED;		break;
											case CNTRL_P : cwahl = DOPROGS;		break;
											case CNTRL_Q : cwahl = DOQUIT;		break;
											case CNTRL_S : cwahl = DOSCAN;		break;
											case CNTRL_T : cwahl = DOTXTED;		break;
											case CNTRL_1 					: cwahl = DOUTIL1;	break;
											case KEY_NUM_CNTRL_1	: cwahl = DOUTIL1;	break;
											case CNTRL_2 					: cwahl = DOUTIL2;	break;
											case KEY_NUM_CNTRL_2	: cwahl = DOUTIL2;	break;
											case CNTRL_3 					: cwahl = DOUTIL3;	break;
											case KEY_NUM_CNTRL_3	: cwahl = DOUTIL3;	break;
											case CNTRL_4 					: cwahl = DOUTIL4;	break;
											case KEY_NUM_CNTRL_4	: cwahl = DOUTIL4;	break;
											case CNTRL_5 					: cwahl = DOUTIL5;	break;
											case KEY_NUM_CNTRL_5	: cwahl = DOUTIL5;	break;
											case CNTRL_6 					: cwahl = DOUTIL6;	break;
											case KEY_NUM_CNTRL_6	: cwahl = DOUTIL6;	break;
											case CNTRL_X 					: cwahl = DOTERM;		break;
											case KEY_NUM_CNTRL_STAR	: cwahl = 1234;		break;
										}; break;
				case 0 	: switch(key)
										{	case KEY_0			: cwahl = STARTSEL;	break;
											case KEY_NUM_0	: cwahl = STARTSEL;	break;
											case KEY_1 			: cwahl = START1;		break;
											case KEY_NUM_1 	: cwahl = START1; 	break;
											case KEY_2 			: cwahl = START2;		break;
											case KEY_NUM_2 	: cwahl = START2; 	break;
											case KEY_3 			: cwahl = START3;		break;
											case KEY_NUM_3 	: cwahl = START3; 	break;
											case KEY_4 			: cwahl = START4;		break;
											case KEY_NUM_4 	: cwahl = START4; 	break;
											case KEY_5 			: cwahl = START5;		break;
											case KEY_NUM_5 	: cwahl = START5; 	break;
											case KEY_6 			: cwahl = START6;		break;
											case KEY_NUM_6 	: cwahl = START6; 	break;
											case HELP  : cwahl = 1000;			break;
											case UNDO  : cwahl = DONOTE;		break;
											case F1    : cwahl = POINT0;		break;
											case F2    : cwahl = POINT1;		break;
											case F3    : cwahl = POINT2;		break;
											case F4    : cwahl = POINT3;		break;
											case F5    : cwahl = POINT4;		break;
											case F6    : cwahl = POINT5;		break;
											case F10   : cwahl = TBOX;			break;
										}
			}
		return(cwahl);
	}
#endif



int trans_key1(int shift,int key)
	{	int ret=-1;
		switch(key)
			{	
				case HELP				:	ret=1000;								break;
				case KEY_f			:	
				case KEY_F			: ret=1001;								break;
				case KEY_g			:
				case KEY_n			:
				case KEY_G			:
				case KEY_N			: ret=1002;								break;
				case CCUR_DOWN	:	ret=FILEUP+Add_Lines;		break;
				case CCUR_UP		: ret=FILEDN+Add_Lines;		break;
				case CCUR_RIGHT	:	ret=FILEUPUP+Add_Lines;	break;
				case CCUR_LEFT	:	ret=FILEDNDN+Add_Lines;	break;
				case HOME				: ret=1003;								break;
				case SHFT_HOME	: ret=1004;								break;
				case RETURN			:
				case ENTER			:	ret=FOK+Add_Lines;			break;
				case UNDO				:	ret=FOK+Add_Lines;			break;
			}
		if (shift & 4)
			{	switch(key)
					{	case CNTRL_Q: ret=FOK+Add_Lines; break;
					}
			}
		if (shift & 8)
			{	switch(key)
					{	case ALT_Q: ret=FOK+Add_Lines; break;
					}
			}
		return(ret);
	}



#if !defined( _AVALSEMP_ )

int trans_key2(int shift,int key)
	{	int ret=-1;
		shift=shift;
		switch(key & 0xff)
			{	case	27	: ret=1010;	break;
				case	32	: ret=1011;
			}
		return(ret);
	}

int trans_key3(int shift,int key)
	{	int ret;
		shift=shift;
		switch(key & 0xff)
			{	case	27	:	ret=1021;	break;
				default		: ret=1020;	break;
			}
		return(ret);
	}

#endif



int trans_key4(int shift,int key)
	{	int ret=-1;
		if (key==HELP) ret=1041;
		if (shift & 8)
			{	switch(key)
					{	case ALT_L			:	ret=REQLOAD;	break;
						case ALT_W			: ret=REQBLOCK;	break;
						case ALT_S			: ret=REQSELEC;	break;
						case ALT_F			: ret=REQFILES;	break;
						case ALT_C			: ret=REQCLEAR;	break;
						case ALT_Z			: ret=REQSAVEZ;	break;
						case ALT_Q			:
						case ALT_E			: ret=REQCANCE;	break;
						case KEY_NUM_0	:
						case ALT_0			: ret=FILELST0;	break;
						case KEY_NUM_1	:
						case ALT_1			: ret=FILELST1;	break;
						case KEY_NUM_2	:
						case ALT_2			: ret=FILELST2;	break;
						case KEY_NUM_3	:
						case ALT_3			: ret=FILELST3;	break;
						case KEY_NUM_4	:
						case ALT_4			: ret=FILELST4;	break;
						case KEY_NUM_5	:
						case ALT_5			: ret=FILELST5;	break;
						case KEY_NUM_6	:
						case ALT_6			: ret=FILELST6;	break;
						case KEY_NUM_7	:
						case ALT_7			: ret=FILELST7;	break;
						case KEY_NUM_8	:
						case ALT_8			: ret=FILELST8;	break;
						case KEY_NUM_9	:
						case ALT_9			: ret=FILELST9;	break;
						case RETURN			:
						case ENTER			:	ret=REQSAVE;
					}
			}
		if (shift & 4)
			{	switch(key)
					{	case CNTRL_Q		: ret=REQCANCE; break;
					}
			}
		if (key==UNDO) ret=REQCANCE;
		return(ret);
	}

int trans_key5(int shift,int key)
	{	int ret=-1;
		if (shift & 8)
			{	switch(key)
					{	case ALT_Q			:
						case ALT_E			: ret=SENDCANC;	break;
						case RETURN			:
						case ENTER			:	ret=SENDSAVE;	break;
						case ALT_Z			: ret=SENDSAVZ;	break;
						case ALT_L			: ret=SENDLOAD; break;
						case ALT_S			: ret=SENDFILE|0x8000;	break;
					}
			}
		if (shift & 4)
			{	switch(key)
					{	case CNTRL_Q		: ret=SENDCANC; break;
					}
			}
		if (key==HELP)	ret=1051;
		if (key==UNDO)	ret=SENDCANC;
		return(ret);
	}

int trans_key6(int shift,int key)
	{	int ret=-1;
		switch(key)
			{	
				case HELP				:	ret=1061;								break;
				case HOME				: ret=1064;								break;
				case SHFT_HOME	: ret=1065;								break;
				case CCUR_RIGHT	: ret=FIXDNDN+Add_Lines;	break;
				case CCUR_DOWN	:	ret=FIXDOWN+Add_Lines;	break;
				case CCUR_LEFT	:	ret=FIXUPUP+Add_Lines;	break;
				case CCUR_UP		: ret=FIXUP+Add_Lines;		break;
				case RETURN			:
				case ENTER			:	ret=FIXAREA+Add_Lines;	break;
				case KEY_f			:	
				case KEY_F			: ret=1062;								break;
				case KEY_g			:
				case KEY_n			:
				case KEY_G			:
				case KEY_N			: ret=1063;								break;
				case UNDO				: ret=FIXCANC+Add_Lines;	break;
			}
		if (shift & 4)
			{	switch(key)
					{	case CNTRL_Q: ret=FIXCANC+Add_Lines; break;
					}
			}
		if (shift & 8)
			{	switch(key)
					{	case ALT_Q	:
						case ALT_E	: ret=FIXCANC+Add_Lines;
					}
			}
		return(ret);
	}
