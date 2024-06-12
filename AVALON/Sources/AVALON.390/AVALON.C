/*******************************************************************/
/* This is AVALON, the greatest shell for fidopoints... Enjoy...	 */
/*******************************************************************/
/* #define _BETA_   for BETA-Version															 */
/*******************************************************************/

#include <aes.h>
#include <vdi.h>
#include <ctype.h>
#include <ext.h>
#include <portab.h>
#include <process.h>
#include <screen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <scancode.h>
#include <my_scan.h>
#include <fido.h>
/* #include <magx.h> */
#include "version.h"
#include "avalonio.h"
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
	#include "avalsemp.rh"
#else
	#include "avalon.h"
	#include "avalon.rh"
#endif
#include "avallang.h"
#include "define.h"
#include "transkey.h"
#include "form_run.h"
#include "crc32.h"
#include "windmini.h"
#include "diverse.h"
#include "vars.h"
#include "cookie.h"

void	save2(int config);
void	load2(int config);
void	load(void);
/* ============================================== Externe Module = */
extern long Tracer(void);
extern signed long batching(char *fname,int redirect);
extern void file_manager(void);
extern void prog_manager(void);
extern void terminal(int baud);
extern void	manager_term(int baud);
extern void	send_pipe(void);
extern void file_request(void);
extern void do_areafix(void);
extern void hatch(void);
extern void edit_areas(void);
extern int	setup1(void);
extern void	setup2(void);
extern int	setup3(void);
/* ============================================= USERDEF-Objects = */
int		cdecl DrawRect(PARMBLK *parmblock);							/* _user1 */
int		cdecl	DrawLine(PARMBLK *parmblock);							/* _user2 */
int		cdecl	CheckBox(PARMBLK *parmblock);							/* _user3 */
USERBLK	_user1,_user2,_user3;
/* ========================================== Spezial-Funktionen = */
long SWAP(long value) 0x4840;
/* ================================================== Funktionen = */
void	add_slash(char *path);
void	autopoll(int cwahl);
void	avalonexit(signed int ret);
void	birthday(void);
int		copy_binkley(int xwahl);
int		copy_files(char *pin,char *pout,char *copy_buffer);
void	desktop(void);
int		doexit(void);
void	file_select(char *ext);
void	help(void);
void	info(void);
void	info_box(void);
void	init_vars(void);
int		installbinksemp(void);
void	installinfo(void);
void	lese_cfg(void);
void	limit_shareware(void);
void	mouse(char *xfile);
int		needhelp(void);
void	notebook(void);
void	pnt_hell(char *x0,int x1,int x2);
void	point_hell(void);
void	pnt_hell2(int x1, int x2);
void	point_setup(int cwahl);
void	poll(int cwahl,int mode);
void	poll_selected(void);
void	prg_util_hell(void);
void	select_pointsetup(int xwahl);
void	sess_hell(int x0,int x1,int x2,int x3,int x4);
void	session_hell(void);
void	setup_neu(char *template,char *keystr,char *string,int item,int mitem,int length);
void	setup_point(void);
void	setup_sess(void);
void	setup_util(void);
void	shareware(void);
void	smallhelp(int feld);
int		spezial(void);
int		spezial_pointname(char *txt,int index);
void	start(int prg);
void	start_it(char *xfile,char *xcmd,int mode,int prg);
void	systemfolder(void);
long	testdate(void);
int		teste(char *ext);
void	teste_code(long code_sysop,long code_application,int found);
void	teste_datei(FILE *xcfg);
void	teste_nomail(void);
void	teste_notebook(void);
void	teste_setup(void);
void	teste_setupflags(int xflag, int xpoint, int xsess);
int 	test_if_autostart_neccessary(int do_poll,int wahl);
int		test_if_running(void);
int		tiny_dialog(OBJECT *tree);
void	wait(void);
void	wartebox(int zeit);
int		whatnow(void);
void	zeige_namen(int add,int max);

void	schreibe(int t,OBJECT *tree,FILE *file);
void	savepositions(void);
void	setzexy(OBJECT *tree,int x,int y);
void	loadpositions(void);
/* ================================================ RSC-Routinen = */
void	rsc_load(void);
int		xform_do3(OBJECT *ctree, int xstart);
/* =============================================================== */

long 		demo_date,bak_demo_date;
char 		demo_dat[15],bak_demo_dat[15],
				dodate[11],dotime[9];

int			wahl1;

int			key,key2,
				aes_handle,bt_version=0,baud=9,
				demoversion=UNKNOWN,
				cmdflags=0,
				serport=0,vt52=TRUE,voll=TRUE;

char		netfile[PATH_MAX],
				knowninbound[PATH_MAX],protinbound[PATH_MAX],
				capture[PATH_MAX],

				pareas[51],
				pbinkley[51], binkley[PATH_MAX],
				psemper[51], semper[PATH_MAX],
				bt300[MAX_INFOBT300 * 2][47];			/* Initialisierung in INIT_VARS */

char 		cmdbatch[PATH_MAX],
				tempchr[4][20];

#define	TEST_REGISTERED		if ( demoversion==UNKNOWN || (demoversion==TEMPORARY && testdate()>demo_date) )

/* =============================================================== */
/* =M A I N======================================================= */
/* =============================================================== */

int main(int argc, const char *argv[])
	{	int cpoll;
		signed int i;

/*	Supexec(Tracer); */
		if (argc-->=2)			/* Es gibt min.1 Parameter */
			{	for (cpoll=1; cpoll<=argc; cpoll++)
					{	if (strnicmp(argv[cpoll],"NoAutoStart",11)==0)
							cmdflags |= D_NOAUTOSTART;
						else if (strnicmp(argv[cpoll],"NoAutoBatch",11)==0)
							cmdflags |= D_NOAUTOBATCH;
						else if (strnicmp(argv[cpoll],"NoDelay",7)==0)
							cmdflags |= D_NODELAY;
						else if (strnicmp(argv[cpoll],"Override",8)==0)
							cmdflags |= D_OVERRIDE;
						else if (strnicmp(argv[cpoll],"Batch:",6)==0)
							{	cmdflags |= D_STARTBATCH;
								strcpy(cmdbatch,argv[cpoll]+6);
							}
					}
			}

		bak_demo_date =
		demo_date = (long) DTAG + ((long) DMONAT*100) + ((long) DJAHR*10000);
		sprintf(demo_dat,"%02u.%02u.%04u",DTAG,DMONAT,DJAHR);
		strcpy(bak_demo_dat,demo_dat);
		
		if ((ap_id=appl_init()) < 0)
			{	form_alert(1,msg[0]);
				exit(1);
			}

		graf_mouse( ARROW,NULL );
		init_vars();

		if (test_if_running())
			if (form_alert(2,msg[46])==2)
				avalonexit(0);

		rsc_load();
		systemfolder();

		if ( exist("AVALON\\AVALON.CFG")==0 )
			{	load();
			}
		else
			{	load2(1);
				load2(2);
			}

		loadpositions();
		teste_setup();
		lese_cfg();

		#ifdef _BETA_
		if ( demoversion<=TEMPORARY )
			{	form_alert(1,msg[24]);
				avalonexit(1);
			}
		#endif

		setup_util();
		setup_sess();
		setup_point();

		menu_icheck(tree12,MPOINT1,1);
		OB_FLAGS(tree1,NOTEBOOK) |= HIDETREE;
		prg_util_hell();

		point_hell();
		session_hell();
		info_box();

		gettime( &ZEIT );
		sprintf(dotime,"%02u:%02u:%02u",(int)ZEIT.ti_hour,(int)ZEIT.ti_min,(int)ZEIT.ti_sec);
		put_text(tree1,DOTIME,dotime,8);

		getdate( &HEUTE );
		sprintf(dodate,"%02u.%02u.%04u",(int)HEUTE.da_day,(int)HEUTE.da_mon,HEUTE.da_year);
		put_text(tree1,DODATE,dodate,10);

		if ( !(cmdflags & D_NOAUTOBATCH) && !((int)Kbshift(-1) & 2))
			if (exist("AVALON\\AUTOEXEC.BAT")==0)
				{	wind_get(0,WF_WORKXYWH,&wnx,&wny,&wnw,&wnh);
					Dsetpath("AVALON");
					batching("AUTOEXEC.BAT",BOOLEAN(divflags & D_REDIRECT));
					set_defaultdir();
				}

		if (cmdflags & D_STARTBATCH)
			if (exist(cmdbatch)==0)
				{	wind_get(0,WF_WORKXYWH,&wnx,&wny,&wnw,&wnh);
					batching(cmdbatch,BOOLEAN(divflags & D_REDIRECT));
				}

		if ( !(cmdflags & D_NOAUTOSTART) && !((int)Kbshift(-1) & 8))
			{	i=wahl1=0;
				for(cpoll=0; cpoll<=MAXSESSION-1; cpoll++)
					if (sflags[cpoll][0] & F_AUTOPOLL)
						i++;
				if (i!=0 && (divflags & D_USEDELAY) && !(cmdflags & D_NODELAY) && test_if_autostart_neccessary(FALSE,-1))
					{	if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
							form_alert(1,msg[23]);
						OB_W(tree19,LEFTTIME)=OB_W(tree19,MAXTIME);
						xform_dial(tree19,XFMD_START,&handle,&wnx,&wny,&wnw,&wnh);
						for (i=OB_W(tree19,MAXTIME); i>0; i-=10)
							{	OB_W(tree19,LEFTTIME)=i;
								wahl1=xform_do(handle,tree19,NULL,0,250L,trans_key3,&wnx,&wny,&wnw,&wnh);
								if (wahl1 & 0x1000)
									{	redraw_one_objc(tree19,MAXTIME,handle);
										wahl1=0;
									}
								if (wahl1 & 0x4000)
									wahl1=1020;
								if (wahl1==1020 || wahl1==1021)
									break;
							}
						xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
						xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh);
					}
				if (wahl1!=1020)
					test_if_autostart_neccessary(TRUE,wahl1);
#if 0
					{	for(cpoll=0; cpoll<=MAXSESSION-1; cpoll++)
							{	if (sflags[cpoll][0] & F_AUTOPOLL)
									{	i=FALSE;
										gettime( &ZEIT );
										sprintf(tempchr[2],"%02d%02d",ZEIT.ti_hour,ZEIT.ti_min);
										if (*s_timesess[cpoll][0]==EOS &&
												*s_timesess[cpoll][1]==EOS)
											i=TRUE;
										if (cmdflags & D_OVERRIDE)
											i=TRUE;
										if (*s_timesess[cpoll][0]!=EOS)
											{	strncpy(tempchr[0],s_timesess[cpoll][0],4);
												strncpy(tempchr[1],s_timesess[cpoll][0]+4,4);
												tempchr[0][4] =
												tempchr[1][4] = EOS;
												if (strncmp(tempchr[0],tempchr[1],4)<=0)
													{	if (strncmp(tempchr[0],tempchr[2],4)<=0 &&
																strncmp(tempchr[2],tempchr[1],4)<=0)
															i=TRUE;
													}
												else
													{	if (strncmp(tempchr[0],tempchr[2],4)<=0 ||
																strncmp(tempchr[2],tempchr[1],4)<=0)
															i=TRUE;
													}
											}
										if (*s_timesess[cpoll][1]!=EOS)
											{	strncpy(tempchr[0],s_timesess[cpoll][1],4);
												strncpy(tempchr[1],s_timesess[cpoll][1]+4,4);
												tempchr[0][4] =
												tempchr[1][4] = EOS;
												if (strncmp(tempchr[0],tempchr[1],4)<=0)
													{	if (strncmp(tempchr[0],tempchr[2],4)<=0 && 
																strncmp(tempchr[2],tempchr[1],4)<=0)
															i=TRUE;
													}
												else
													{	if (strncmp(tempchr[0],tempchr[2],4)<=0 ||
																strncmp(tempchr[2],tempchr[1],4)<=0)
															i=TRUE;
													}
											}
										if (i==TRUE)
											{	switch(cpoll)
													{	case 0 : wahl1=START1; break;
														case 1 : wahl1=START2; break;
														case 2 : wahl1=START3; break;
														case 3 : wahl1=START4; break;
														case 4 : wahl1=START5; break;
														case 5 : wahl1=START6;
													}
												poll(wahl1,FALSE);
											}
									}
							}
					}
#endif
			}

		if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
			form_alert(1,msg[23]);
		xform_maindialog(XFMD_START);

		teste_notebook();

		birthday();

		do
			{	menu_bar(tree12, 1);

				do
					{	if (divflags & D_NOAVALONBOX)
							wahl1=xform_do(handle,tree20,tree12,0,0,trans_key,&wnx,&wny,&wnw,&wnh) & 0x7fff;
						else
							{	wahl1=xform_do(handle,tree1,tree12,0,750L,trans_key,&wnx,&wny,&wnw,&wnh) & 0x7fff;

								if (wahl1 & 0x1000)
									{	gettime( &ZEIT );
										sprintf(tempchr[0],"%02u:%02u:%02u",(int)ZEIT.ti_hour,(int)ZEIT.ti_min,(int)ZEIT.ti_sec);
										if (strcmp(tempchr[0],dotime))
											{	strcpy(dotime,tempchr[0]);
												put_text(tree1,DOTIME,dotime,8);
												redraw_one_objc(tree1,DOTIME,handle);
											}
										getdate( &HEUTE );
										sprintf(tempchr[0],"%02u.%02u.%04u",(int)HEUTE.da_day,(int)HEUTE.da_mon,HEUTE.da_year);
										if (strcmp(tempchr[0],dodate))
											{	strcpy(dodate,tempchr[0]);
												put_text(tree1,DODATE,dodate,10);
												redraw_one_objc(tree1,DODATE,handle);
											}
										wahl1 &= ~0x1000;
									}
							}
					}
				while (wahl1==0);

				if (wahl1 & 0x4000)
					wahl1 = DOQUIT;
				if (wahl1 & 0x800)
					wahl1=trans_message();		/* MsgType direkt aus MSGBUF! */
				if (wahl1 & 0x2000)
					wahl1=trans_menu(wahl1 & 0x7ff);

				menu_bar(tree12, 0);
				if ( (wahl1 > 0) && (wahl1 < 1000) )
					{	undo_state(tree1,wahl1,SELECTED);
						if ( !(divflags & D_NOAVALONBOX) )
							if ( !(bt_version==0 && wahl1==TBOX))
								redraw_objc(tree1,wahl1,handle);
					}
				switch (wahl1)
					{	case	1234			: if (divflags & D_NOAVALONBOX)
																divflags &= ~D_NOAVALONBOX;
															else
																divflags |= D_NOAVALONBOX;
															xform_maindialog(XFMD_DIALOG);
															redraw_infobox=TRUE;
															break;		/* Open window von FORM_RUN */
						case	1001			: manager_term(baud); break;
						case	DOFIX			:	{	limit_shareware();		/* SHAREWARE */
																wartebox(75);
																if (*netmail[0]==EOS)
																	form_alert(1,msg[30]);
																else
																	do_areafix();
															}	break;
						case	STARTSEL	: poll_selected(); break;
						case	DOQUIT 		: {	if (changes)
																	if (!doexit())
																		wahl1=0;
															}	break;
						case	DSAVE			:	if (form_alert(2,msg[40])==2)
																{	save2(1);
																	save2(2);
																	if (exist("AVALON\\AVALON.BAK")==0)
																		remove("AVALON\\AVALON.BAK");
																	if (exist("AVALON\\AVALON.CFG")==0)
																		remove("AVALON\\AVALON.CFG");
																}
															break;
						case	DOINFO		:	info(); break;
						case	POINT0	  :
						case	POINT1  	:
						case	POINT2		:
						case	POINT3		:	
						case	POINT4		:
						case	POINT5		:	if (*psemper!=EOS && exist(semper)==0)
																{	mailer=SEMPER;
																	break;
																}
															else
																{	mailer=BINKLEY;
																	point_setup(wahl1);		/* KEIN break; */
																}
						case	TBOX			:
						case	TBOX2			:	redraw_infobox=TRUE;
															break;
						case	DOEXPORT	: start(1); break;
						case	DOIMPORT	:	start(2); break;
						case	DOCRUNCH	: start(3); break;
						case	DOSCAN		: start(4); break;
						case	DOMSGED		: start(5); break;
						case	DOTXTED		: start(6);	break;
						case	DOMAILER	: start(7); break;
						case	DOTERM		: start(8); break;
						case	DOUTIL1		: start(9); break;
						case	DOUTIL2		: start(10); break;
						case	DOUTIL3		: start(11); break;
						case	DOUTIL4		: start(12); break;
						case	DOUTIL5		: start(13); break;
						case	DOUTIL6		: start(14); break;
						case	DOPRG			: {	file_select("*.PRG");
																limit_shareware();		/* SHAREWARE */
															}	break;
						case	DOTOS			: {	file_select("*.TOS");
																limit_shareware();		/* SHAREWARE */
															}	break;
						case	DOTTP			:	{	file_select("*.TTP");
																limit_shareware();		/* SHAREWARE */
															} break;
						case	DOALL			: {	file_select("*.BAT");
																limit_shareware();		/* SHAREWARE */
															} break;
						case	DOAPP			: {	file_select("*.APP");
																limit_shareware();		/* SHAREWARE */
															} break;
						case	DOSTAR		: {	file_select("*.*");
																limit_shareware();		/* SHAREWARE */
															} break;
						case	START1		:
						case	START2		:
						case	START3		:
						case	START4		:
						case	START5		:
						case	START6		:	{	if (wahl1!=START1)
																	limit_shareware();		/* SHAREWARE */
																menu_bar(tree12,0);
																poll(wahl1,TRUE);
																menu_bar(tree12,1);
															}	break;
						case	DOSETUP2	:	{	setup2();	
																prg_util_hell();
																limit_shareware();		/* SHAREWARE */
															}	break;
						case	DOSETUP3	:	{	if (setup3()==TRUE)
																	setup_util();
																prg_util_hell();
																limit_shareware();		/* SHAREWARE */
															}	break;
						case	DOSETUP1	:	{	if (setup1()==TRUE)
																	setup_sess();
																session_hell();
															}	break;
						case	DOSPEZIA	: {	limit_shareware();		/* SHAREWARE */
																if (spezial()==TRUE)
																	setup_point();
																redraw_infobox=TRUE;
															}	break;
						case 	AUTO1			:
						case 	AUTO2			:
						case 	AUTO3			:
						case 	AUTO4			:
						case 	AUTO5			:
						case 	AUTO6			:	{	TEST_REGISTERED
																	{	if (wahl1==AUTO1 || wahl1==AUTO2)
																			{	menu_bar(tree12,0);
																				autopoll(wahl1);
																				menu_bar(tree12,1);
																			}
																		else
																			{	shareware();
																			}
																	}
																else
																	{	menu_bar(tree12,0);
																		autopoll(wahl1);
																		menu_bar(tree12,1);
																	}
															}	break;
						case	DOAREAS		:	{	if ( *program[16]!=EOS )
																	start(16);
																else
																	edit_areas();
															}	break;
						case	DOFREQ		:	wartebox(75);
															file_request();
															break;
						case	DOHATCH		:	{	TEST_REGISTERED
																	shareware();
																hatch();
															}	break;
						case	DONOTE		: {	notebook();
																teste_notebook();
															}	break;
						case	DMANAGER	: {	if (divflags & D_NOAVALONBOX)
																	form_alert(1,msg[25]);
																else
																	file_manager();
															}	break;
						case	DOPROGS		: {	if (divflags & D_NOAVALONBOX)
																	form_alert(1,msg[25]);
																else
																	prog_manager();
																/* form_alert(1,msg[22]); */
															}	break;
						case	1000			:	help();	break;
					}
				if (redraw_infobox==TRUE)
					{	i=mailer;
						if (bt_version!=0)
							lese_cfg();
						if (i!=mailer && mailer==SEMPER)
							{	setup_util();
								setup_sess();
								session_hell();
								prg_util_hell();
							}
						info_box();
						point_hell();
						if (wahl1!=DOSPEZIA)
							{	if (!(divflags & D_NOAVALONBOX))
									{	redraw_one_objc(tree1,DESKPNT,handle);
										if (bt_version!=0)
											redraw_one_objc(tree1,TBOX,handle);
									}
							}
						redraw_infobox=FALSE;
					}
				if (pipeflags!=0)		/************* PIPE senden *************/
					send_pipe();
			}
		while (wahl1 != DOQUIT);

		if (divflags & D_SAVE)
			savepositions();

		wartebox(0);

		xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
		if (xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
			form_alert(1,msg[23]);
		v_clsvwk(vdi_handle);
		appl_exit();
		return 0;
	}

/*
** TRUE: Autostart wir erfolgen
*/

int test_if_autostart_neccessary(int do_poll,int wahl1)
{	int i=FALSE,cpoll;
	for(cpoll=0; cpoll<=MAXSESSION-1; cpoll++)
	{	if (sflags[cpoll][0] & F_AUTOPOLL)
		{	gettime( &ZEIT );
			sprintf(tempchr[2],"%02d%02d",ZEIT.ti_hour,ZEIT.ti_min);
			if (*s_timesess[cpoll][0]==EOS &&
					*s_timesess[cpoll][1]==EOS)
				i=TRUE;
			if (cmdflags & D_OVERRIDE)
				i=TRUE;
			if (*s_timesess[cpoll][0]!=EOS)
			{	strncpy(tempchr[0],s_timesess[cpoll][0],4);
				strncpy(tempchr[1],s_timesess[cpoll][0]+4,4);
				tempchr[0][4] =
				tempchr[1][4] = EOS;
				if (strncmp(tempchr[0],tempchr[1],4)<=0)
				{	if (strncmp(tempchr[0],tempchr[2],4)<=0 &&
							strncmp(tempchr[2],tempchr[1],4)<=0)
						i=TRUE;
				}
				else
				{	if (strncmp(tempchr[0],tempchr[2],4)<=0 ||
							strncmp(tempchr[2],tempchr[1],4)<=0)
						i=TRUE;
				}
			}
			if (*s_timesess[cpoll][1]!=EOS)
			{	strncpy(tempchr[0],s_timesess[cpoll][1],4);
				strncpy(tempchr[1],s_timesess[cpoll][1]+4,4);
				tempchr[0][4] =
				tempchr[1][4] = EOS;
				if (strncmp(tempchr[0],tempchr[1],4)<=0)
				{	if (strncmp(tempchr[0],tempchr[2],4)<=0 && 
							strncmp(tempchr[2],tempchr[1],4)<=0)
						i=TRUE;
				}
				else
				{	if (strncmp(tempchr[0],tempchr[2],4)<=0 ||
							strncmp(tempchr[2],tempchr[1],4)<=0)
						i=TRUE;
				}
			}
			if (i==TRUE && do_poll==TRUE)
			{	switch(cpoll)
				{	case 0 : wahl1=START1; break;
					case 1 : wahl1=START2; break;
					case 2 : wahl1=START3; break;
					case 3 : wahl1=START4; break;
					case 4 : wahl1=START5; break;
					case 5 : wahl1=START6;
				}
				poll(wahl1,FALSE);
			}
		}
	}
	return(i);
}

int doexit(void)
	{	int wahl,nx,ny,nw,nh;
		do_xstate(tree30,DISABLED,CSPECIAL,CSESSION,CPROGRAM,CUTILITY,
															CFILELIS,CHATCHMA,CHATCHDE,CTERMINA,
															CEPROGRA,-1);
		if (changes & C_SPECIAL)		undo_state(tree30,CSPECIAL,DISABLED);
		if (changes & C_SESSION)		undo_state(tree30,CSESSION,DISABLED);
		if (changes & C_PROGRAMS)		undo_state(tree30,CPROGRAM,DISABLED);
		if (changes & C_UTILITIES)	undo_state(tree30,CUTILITY,DISABLED);
		if (changes & C_FILELIST)		undo_state(tree30,CFILELIS,DISABLED);
		if (changes & C_HATCHMAIL)	undo_state(tree30,CHATCHMA,DISABLED);
		if (changes & C_HATCHDEF)		undo_state(tree30,CHATCHDE,DISABLED);
		if (changes & C_TERMINAL)		undo_state(tree30,CTERMINA,DISABLED);
		if (changes & C_EPROGRAMS)	undo_state(tree30,CEPROGRA,DISABLED);
		form_center(tree30,&nx,&ny,&nw,&nh);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		objc_draw(tree30,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahl=form_do(tree30,0) & 0x7FFF;
		undo_state(tree30,wahl,SELECTED);
		form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
		if (wahl==AVALEXIT)
			return(TRUE);
		else
			return(FALSE);
	}

void avalonexit(signed int ret)
	{	v_clsvwk(vdi_handle);
		appl_exit();
		exit(ret);
	}

int test_if_running(void)
	{	int ret,type,id,rc;
		char tempchr[20];
		ret=FALSE;
		if (has_appl_search)
			{ rc = appl_search(0,tempchr,&type,&id );
				while( rc )
					{	if ( !strnicmp(tempchr,"AVALON  ",8) && id!=ap_id )
							ret=TRUE;
						rc = appl_search(1,tempchr,&type,&id);
					}
			}
		else
			{ id = appl_find("AVALON  ");
				if ( id>=0 && id!=ap_id )
					ret=TRUE;
			}
		return(ret);
	}

long testdate(void)
	{	getdate( &HEUTE );
		return( (long) HEUTE.da_year *10000 +
						(long) HEUTE.da_mon  *100 +
						(long) HEUTE.da_day);
	}

void limit_shareware(void)
	{	if ( demoversion!=REGISTERED )
			if ( testdate()>demo_date )
				shareware();
	}

void shareware(void)
	{	form_alert(1,msg[5]);
	}

void wartebox(int zeit)
	{ int millisekunden,i,pp1;
		char *p1;
		TEST_REGISTERED
			{	p1 = tree19[TIMEBOXT].ob_spec.tedinfo->te_ptext;
		    pp1 =tree19[TIMEBOXT].ob_spec.tedinfo->te_txtlen;
				if ( testdate()>demo_date )
					{	if (zeit==0) millisekunden=400; else millisekunden=zeit;	}
				else
					{	if (zeit==0) millisekunden=200; else millisekunden=zeit;	}
				OB_FLAGS(tree19,TIMEBOXS) |= HIDETREE;
				tree19[TIMEBOXT].ob_spec.tedinfo->te_ptext  = avaltxt[0];
		    tree19[TIMEBOXT].ob_spec.tedinfo->te_txtlen = (int)strlen(avaltxt[0])+1;
				OB_W(tree19,LEFTTIME)=OB_W(tree19,MAXTIME);
				xform_dial(tree19,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				for (i=OB_W(tree19,MAXTIME); i>=0; i-=8)
					{	OB_W(tree19,LEFTTIME)=i;
						wahl1=xform_do(handle,tree19,NULL,0,(long) millisekunden,NULLFUNC,&wnx,&wny,&wnw,&wnh);
						if (wahl1 & 0x4000)
							i+=8;
						if (wahl1 & 0x1000)
							redraw_one_objc(tree19,MAXTIME,handle);
					}
				OB_FLAGS(tree19,TIMEBOXS) &= ~HIDETREE;
				tree19[TIMEBOXT].ob_spec.tedinfo->te_ptext = p1;
		    tree19[TIMEBOXT].ob_spec.tedinfo->te_txtlen= pp1;
			}
	}

void setup_neu(char *template,char *keystr,char *string,int item,int mitem,int length)
	{	char menue[MAX_STR];
		put_text(tree1,item,string,length);
		sprintf(menue,template,string,keystr);
		menu_text(tree12,mitem,menue);
	}

void setup_util(void)
	{	setup_neu("  %-9s    %s","^1",txtutil[0],DOUTIL1,MUTIL1,9);
		setup_neu("  %-9s    %s","^2",txtutil[1],DOUTIL2,MUTIL2,9);
		setup_neu("  %-9s    %s","^3",txtutil[2],DOUTIL3,MUTIL3,9);
		setup_neu("  %-9s    %s","^4",txtutil[3],DOUTIL4,MUTIL4,9);
		setup_neu("  %-9s    %s","^5",txtutil[4],DOUTIL5,MUTIL5,9);
		setup_neu("  %-9s    %s","^6",txtutil[5],DOUTIL6,MUTIL6,9);
	}

void setup_sess(void)
	{	setup_neu("  %-9s         %s","1",txtsess[0],START1,MPOLL1,9);
		setup_neu("  %-9s         %s","2",txtsess[1],START2,MPOLL2,9);
		setup_neu("  %-9s         %s","3",txtsess[2],START3,MPOLL3,9);
		setup_neu("  %-9s         %s","4",txtsess[3],START4,MPOLL4,9);
		setup_neu("  %-9s         %s","5",txtsess[4],START5,MPOLL5,9);
		setup_neu("  %-9s         %s","6",txtsess[5],START6,MPOLL6,9);
	}

void setup_point(void)
	{	setup_neu("  %-11s       %s","F1",txtname[0],POINT0,MPOINT1,11);
		setup_neu("  %-11s       %s","F2",txtname[1],POINT1,MPOINT2,11);
		setup_neu("  %-11s       %s","F3",txtname[2],POINT2,MPOINT3,11);
		setup_neu("  %-11s       %s","F4",txtname[3],POINT3,MPOINT4,11);
		setup_neu("  %-11s       %s","F5",txtname[4],POINT4,MPOINT5,11);
		setup_neu("  %-11s       %s","F6",txtname[5],POINT5,MPOINT6,11);
	}

void prg_util_hell(void)
	{	register int prg,obj,obj2;
		for (prg=1; prg<=14; prg++)
			{	switch (prg)
					{	case  1	: obj=DOEXPORT;	obj2=MEXPORT;	break;
						case	2 : obj=DOIMPORT;	obj2=MIMPORT;	break;
						case	3	: obj=DOCRUNCH;	obj2=MCRUNCH;	break;
						case	4	: obj=DOSCAN;		obj2=MSCANNER;break;
						case	5 : obj=DOMSGED;	obj2=MMSGEDIT;break;
						case	6 : obj=DOTXTED;	obj2=MTXTEDIT;break;
						case	7 :	obj=DOMAILER;	obj2=MMAILER;	break;
						case	8 : obj=DOTERM;		obj2=MTERM;		break;
						case	9 : obj=DOUTIL1;	obj2=MUTIL1;	break;
						case 10	: obj=DOUTIL2;	obj2=MUTIL2;	break;
						case 11	: obj=DOUTIL3;	obj2=MUTIL3;	break;
						case 12 : obj=DOUTIL4;	obj2=MUTIL4;	break;
						case 13 : obj=DOUTIL5;	obj2=MUTIL5;	break;
						case 14 : obj=DOUTIL6;	obj2=MUTIL6;
					}
				if ( *program[prg]==EOS )
					{	do_state(tree1,obj,DISABLED);
						menu_ienable(tree12,obj2,0);
					}
				else
					{	undo_state(tree1,obj,DISABLED);
						menu_ienable(tree12,obj2,1);
					}
			}
	}

/* =============================================================== */
/* =SPEZIAL======================================================= */
/* =============================================================== */

int spezial(void)
	{	int retflag=FALSE,wahl=0,xwahl;
		char *pointer,temp[5];
		
		sprintf(temp,"%u",fontpnt);
		put_text(tree15,USEDSIZE,temp,2);

		put_text(tree15,PAREAS,pareas,50);
		put_text(tree15,PBINKLEY,pbinkley,50);
		put_text(tree15,PSEMPER,psemper,50);
		put_text(tree15,PNETMAIL,netmail[0],50);
		put_text(tree15,NAMSET1,txtname[0],11);
		put_text(tree15,NAMSET2,txtname[1],11);
		put_text(tree15,NAMSET3,txtname[2],11);
		put_text(tree15,NAMSET4,txtname[3],11);
		put_text(tree15,NAMSET5,txtname[4],11);
		put_text(tree15,NAMSET6,txtname[5],11);

		undo_xstate(tree15,SELECTED,USE000,USE241,USE999,CHECKON,KEYPON,
																BOXON,SAVEON,USE4DON,USE3DON,USEDELAY,
																USESMART,USEDBBS,KSHATCH,KSFIX,
																REDIRECT,USENOLNA,USESNAP,
																USECENTR,USEMAXI,BOXSMALL,
																USEBUFF,-1);
		switch(bt_version)
			{	case	241 : do_state(tree15,USE241,SELECTED); break;
				case	999 :	do_state(tree15,USE999,SELECTED); break;
				default		:	do_state(tree15,USE000,SELECTED); break;
			}
		if (divflags & D_CHECKING)			do_state(tree15,CHECKON	,SELECTED);
		if (divflags & D_DIRECTWAIT)		do_state(tree15,KEYPON	,SELECTED);
		if (!(divflags & D_NOAVALONBOX))do_state(tree15,BOXON		,SELECTED);
		if (divflags & D_SAVE)					do_state(tree15,SAVEON	,SELECTED);
		if (divflags & D_USE4D)					do_state(tree15,USE4DON	,SELECTED);
		if (divflags & D_USE3DTIC)			do_state(tree15,USE3DON	,SELECTED);
		if (divflags & D_USEDELAY)			do_state(tree15,USEDELAY,SELECTED);
		if (divflags & D_USESMART)			do_state(tree15,USESMART,SELECTED);
		if (divflags & D_USEDBBS)				do_state(tree15,USEDBBS ,SELECTED);
		if (divflags & D_KSHATCH)				do_state(tree15,KSHATCH ,SELECTED);
		if (divflags & D_KSFIX)					do_state(tree15,KSFIX   ,SELECTED);
		if (divflags & D_REDIRECT)			do_state(tree15,REDIRECT,SELECTED);
		if (divflags & D_NOLINEA)				do_state(tree15,USENOLNA,SELECTED);
		if (divflags & D_SNAP)					do_state(tree15,USESNAP ,SELECTED);
		if (divflags & D_CENTER)				do_state(tree15,USECENTR,SELECTED);
		if (divflags & D_MAXIWIND)			do_state(tree15,USEMAXI	,SELECTED);
		if (divflags & D_BOXSMALL)			do_state(tree15,BOXSMALL,SELECTED);
		if (divflags & D_USEBUFFER)			do_state(tree15,USEBUFF	,SELECTED);
		if (divflags & D_USEOUT36)			do_state(tree15,USEOUT36,SELECTED);

		xform_dial(tree15,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	xwahl=xform_do(handle,tree15,NULL,wahl,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
				wahl = xwahl & 0x7ff;
				if (xwahl & 0x4000)
					wahl = POK;
				undo_state(tree15,wahl,SELECTED);

				if (xwahl & 0x8000)
					{	switch(wahl)
							{	case PNETMAIL	: do_select("",PNETMAIL,tree15,50); break;
								case PAREAS		: do_select("",PAREAS	 ,tree15,50); break;
								case PBINKLEY	: do_select("",PBINKLEY,tree15,50); break;
								case PSEMPER	: do_select("",PSEMPER ,tree15,50); break;
							}
						if (wahl==PAREAS || wahl==PBINKLEY || wahl==PSEMPER)
							{	pointer = strrchr(tree15[wahl].ob_spec.tedinfo->te_ptext,'\\');
								if (pointer!=NULL)
									*(pointer+1) = EOS;
							}
						if (wahl==PNETMAIL)
							delete_extension(tree15,wahl);
						redraw_one_objc(tree15,wahl,handle);
					}
			}
		while (wahl!=POK);

		retflag |= spezial_pointname(txtname[0],NAMSET1);
		retflag |= spezial_pointname(txtname[1],NAMSET2);
		retflag |= spezial_pointname(txtname[2],NAMSET3);
		retflag |= spezial_pointname(txtname[3],NAMSET4);
		retflag |= spezial_pointname(txtname[4],NAMSET5);
		retflag |= spezial_pointname(txtname[5],NAMSET6);

		strcpy(temp,get_text(tree15,USEDSIZE));
		xwahl = atoi(temp);
		if (xwahl>=1)
			fontpnt=xwahl;

		strcpy(pareas,get_text(tree15,PAREAS));
		strcpy(pbinkley,get_text(tree15,PBINKLEY));
		strcpy(psemper,get_text(tree15,PSEMPER));
		strcpy(netmail[0],get_text(tree15,PNETMAIL));

		add_slash(pareas);
		add_slash(pbinkley);
		add_slash(psemper);
		sprintf(areas,"%s%s",pareas,"AREAS.BBS");
		sprintf(dareas,"%s%s",pareas,"DAREAS.BBS");
		sprintf(binkley,"%s%s",pbinkley,"BINKLEY.CFG");
		sprintf(semper,"%s%s",psemper,"CONFIG.DAT");

		if ( is_state(tree15,USE000,SELECTED) ) bt_version=000;
		if ( is_state(tree15,USE241,SELECTED) ) bt_version=241;
		if ( is_state(tree15,USE999,SELECTED) ) bt_version=999;
		divflags &= ~(D_CHECKING|D_DIRECTWAIT|D_NOAVALONBOX|D_SAVE|
									D_USE4D|D_USE3DTIC|D_USEDELAY|D_USESMART|
									D_USEDBBS|D_KSHATCH|D_KSFIX|D_REDIRECT|
									D_NOLINEA|D_SNAP|D_CENTER|D_MAXIWIND|D_BOXSMALL|
									D_USEBUFFER|D_USEOUT36);
		if ( is_state(tree15,CHECKON ,SELECTED) )	divflags |= D_CHECKING;
		if ( is_state(tree15,KEYPON	 ,SELECTED) )	divflags |= D_DIRECTWAIT;
		if (!is_state(tree15,BOXON	 ,SELECTED) )	divflags |= D_NOAVALONBOX;
		if ( is_state(tree15,SAVEON	 ,SELECTED) )	divflags |= D_SAVE;
		if ( is_state(tree15,USE4DON ,SELECTED) )	divflags |= D_USE4D;
		if ( is_state(tree15,USE3DON ,SELECTED) )	divflags |= D_USE3DTIC;
		if ( is_state(tree15,USEDELAY,SELECTED) )	divflags |= D_USEDELAY;
		if ( is_state(tree15,USESMART,SELECTED) )	divflags |= D_USESMART;
		if ( is_state(tree15,USEDBBS ,SELECTED) )	divflags |= D_USEDBBS;
		if ( is_state(tree15,KSHATCH ,SELECTED) )	divflags |= D_KSHATCH;
		if ( is_state(tree15,KSFIX	 ,SELECTED) )	divflags |= D_KSFIX;
		if ( is_state(tree15,REDIRECT,SELECTED) )	divflags |= D_REDIRECT;
		if ( is_state(tree15,USENOLNA,SELECTED) )	divflags |= D_NOLINEA;
		if ( is_state(tree15,USESNAP ,SELECTED) )	divflags |= D_SNAP;
		if ( is_state(tree15,USECENTR,SELECTED) )	divflags |= D_CENTER;
		if ( is_state(tree15,USEMAXI ,SELECTED) )	divflags |= D_MAXIWIND;
		if ( is_state(tree15,BOXSMALL,SELECTED) )	divflags |= D_BOXSMALL;
		if ( is_state(tree15,USEBUFF ,SELECTED) )	divflags |= D_USEBUFFER;
		if ( is_state(tree15,USEOUT36,SELECTED) )	divflags |= D_USEOUT36;
		xform_maindialog(XFMD_DIALOG);
		changes |= C_SPECIAL;
		return(retflag);
	}

int spezial_pointname(char *txt, int index)
	{	char temp[15];
		strcpy(temp,get_text(tree15,index));
		if (strcmp(temp,txt)!=0)
			{	strcpy(txt,temp);
				return(TRUE);
			}
		else
			return(FALSE);
	}

void add_slash(char *path)
	{	if ( *path!=EOS )
			if ( *(path+strlen(path)-1)!='\\' )
				strcat(path,"\\");
	}

/* =============================================================== */
/* =RSC-ROUTINEN================================================== */
/* =============================================================== */

void rsc_load(void)
	{	register int t;
		OBJECT *rsc_start;

		treepic0=rs_trindex[PICTURE0];

		tree1=rs_trindex[DIALOG1];
		tree2=rs_trindex[DIALOG2];
		tree3=rs_trindex[DIALOG3];
		tree5=rs_trindex[DIALOG5];
		tree6=rs_trindex[DIALOG6];
		tree7=rs_trindex[DIALOG7];
		tree8=rs_trindex[DIALOG8];
		tree9=rs_trindex[DIALOG9];
		tree10=rs_trindex[DIALOG10];
		tree12=rs_trindex[DIALOG12];
		tree13=rs_trindex[DIALOG13];
		tree14=rs_trindex[DIALOG14];
		tree15=rs_trindex[DIALOG15];
		tree19=rs_trindex[DIALOG19];
		tree20=rs_trindex[DIALOG20];
		tree21=rs_trindex[DIALOG21];
		tree25=rs_trindex[DIALOG25];
		tree26=rs_trindex[DIALOG26];
		tree27=rs_trindex[DIALOG27];
		tree29=rs_trindex[DIALOG29];
		tree30=rs_trindex[DIALOG30];
		tree31=rs_trindex[DIALOG31];
		tree33=rs_trindex[DIALOG33];
		tree34=rs_trindex[DIALOG34];
		tree40=rs_trindex[DIALOG40];
		tree41=rs_trindex[DIALOG41];
		tree51=rs_trindex[DIALOG51];
		tree52=rs_trindex[DIALOG52];
		tree61=rs_trindex[DIALOG61];
		tree71=rs_trindex[DIALOG71];
		tree81=rs_trindex[DIALOG81];
		tree82=rs_trindex[DIALOG82];

		tree11=rs_trindex[DIALOG11];
		tree16=rs_trindex[DIALOG16];
		tree17=rs_trindex[DIALOG17];
		tree18=rs_trindex[DIALOG18];
		tree22=rs_trindex[DIALOG22];
		tree23=rs_trindex[DIALOG23];
		tree24=rs_trindex[DIALOG24];
		tree28=rs_trindex[DIALOG28];
		tree72=rs_trindex[DIALOG72];

		tree90=rs_trindex[DIALOG90];
		tree91=rs_trindex[DIALOG91];
		tree99=rs_trindex[DIALOG99];

		rsc_start=rs_trindex[0];
		for (t=0; t<=NUM_OBS-1; t++)
			{	
/*			if (&rsc_start[t]<&tree12[0] ||
						&rsc_start[t]>=&tree13[0])
					{	
						OB_X(rsc_start,t) *= 2;
						OB_Y(rsc_start,t) *= 2;
						OB_W(rsc_start,t) *= 2;
						OB_H(rsc_start,t) *= 2;
					}
*/
				rsrc_obfix(rsc_start,t);
				if ( ( OB_TYPE(rsc_start,t) & 0xff00 )==0x0100 )
					{	OB_TYPE(rsc_start,t) = G_USERDEF;
						rsc_start[t].ob_spec.userblk = &_user1;
					}
				if ( ( OB_TYPE(rsc_start,t) & 0xff00 )==0x0200 )
					{	OB_TYPE(rsc_start,t) = G_USERDEF;
						rsc_start[t].ob_spec.userblk = &_user2;
					}
				if ( ( OB_TYPE(rsc_start,t) & 0xff00 )==0x0300 )
					{	OB_TYPE(rsc_start,t) = G_USERDEF;
						rsc_start[t].ob_spec.userblk = &_user3;
					}
			}

		OB_X(tree12,ROOT) =
		OB_Y(tree12,ROOT) = 0;

		OB_SPEC(tree20,DUMMYPIC) = OB_SPEC(treepic0,AVALBLD0);
		OB_W(tree20,ROOT) = OB_W(tree20,DUMMYPIC) = OB_W(treepic0,AVALBLD0);
		OB_H(tree20,ROOT) = OB_H(tree20,DUMMYPIC) = OB_H(treepic0,AVALBLD0);

		OB_SPEC(tree1,AVALLOGO) = OB_SPEC(treepic0,AVALBLD0);
		OB_W(tree1,AVALLOGO) = OB_W(treepic0,AVALBLD0);
		OB_H(tree1,AVALLOGO) = OB_H(treepic0,AVALBLD0);

		if ( OB_H(tree1,DOINFO)<=8 )
			{	OB_SPEC(tree20,DUMMYPIC) = OB_SPEC(treepic0,AVALBLD1);
				OB_W(tree20,ROOT) = OB_W(tree20,DUMMYPIC) = OB_W(treepic0,AVALBLD1);
				OB_H(tree20,ROOT) = OB_H(tree20,DUMMYPIC) = OB_H(treepic0,AVALBLD1);

				OB_Y(tree1,AVALLOGO)=(OB_H(tree1,AVALLOGO-1)-OB_H(treepic0,AVALBLD1)) >> 2;
				OB_X(tree1,AVALLOGO)=(OB_W(tree1,AVALLOGO-1)-OB_W(treepic0,AVALBLD1)) >> 1;

				OB_SPEC(tree1,AVALLOGO) = OB_SPEC(treepic0,AVALBLD1);
				OB_W(tree1,AVALLOGO) = OB_W(treepic0,AVALBLD1);
				OB_H(tree1,AVALLOGO) = OB_H(treepic0,AVALBLD1);
				fontpnt = 9;
			}

		_user1.ub_parm =
		_user2.ub_parm =
		_user3.ub_parm = 0L;
		_user1.ub_code = DrawRect;
		_user2.ub_code = DrawLine;
		_user3.ub_code = CheckBox;
	}

int cdecl DrawRect(PARMBLK *parmblock)
	{	int pxy[4];
		pxy[0]=parmblock->pb_x;
		pxy[1]=parmblock->pb_y + (parmblock->pb_h >> 1) - 1;
		pxy[2]=parmblock->pb_x + parmblock->pb_w - 1;
		pxy[3]=parmblock->pb_y + (parmblock->pb_h >> 1);
		vswr_mode(vdi_handle,MD_REPLACE);
		vsf_interior(vdi_handle,FIS_PATTERN);
		vsf_style(vdi_handle,4);
		vsf_color(vdi_handle,1);
		vr_recfl(vdi_handle,pxy);
		return(0);
	}

int cdecl DrawLine(PARMBLK *parmblock)
	{	int pxy[4],cxy[4];
		pxy[0]=parmblock->pb_x;
		pxy[1]=parmblock->pb_y + (parmblock->pb_h >> 1);
		pxy[2]=parmblock->pb_x + parmblock->pb_w - 1;
		pxy[3]=parmblock->pb_y + (parmblock->pb_h >> 1);
		cxy[0]=parmblock->pb_xc;
		cxy[1]=parmblock->pb_yc;
		cxy[2]=parmblock->pb_xc + parmblock->pb_wc;
		cxy[3]=parmblock->pb_yc + parmblock->pb_hc;
		vswr_mode(vdi_handle,MD_REPLACE);
		vsl_color(vdi_handle,1);
		vs_clip(vdi_handle,TRUE,cxy);
		v_pline(vdi_handle,2,pxy);
		vs_clip(vdi_handle,FALSE,cxy);
		return(0);
	}

int cdecl CheckBox(PARMBLK *parmblock)
	{	int pxy[8],cxy[4];
		cxy[0]=parmblock->pb_xc;
		cxy[1]=parmblock->pb_yc;
		cxy[2]=parmblock->pb_xc + parmblock->pb_wc - 1;
		cxy[3]=parmblock->pb_yc + parmblock->pb_hc - 1;
		vs_clip(vdi_handle,1,cxy);
		pxy[0]=parmblock->pb_x + 1;
		pxy[1]=parmblock->pb_y + 1;
		pxy[2]=parmblock->pb_x + parmblock->pb_w - 2;
		pxy[3]=parmblock->pb_y + parmblock->pb_h - 2;
		vswr_mode(vdi_handle,MD_REPLACE);
		vsf_color(vdi_handle,1);
		vsf_interior(vdi_handle,FIS_HOLLOW);
		vsf_perimeter(vdi_handle,TRUE);
		v_bar(vdi_handle,pxy);
		if (parmblock->pb_currstate & SELECTED)
			{	pxy[6] = pxy[0];
				pxy[5] = pxy[1];
				pxy[4] = pxy[2];
				pxy[7] = pxy[3];
				v_pline(vdi_handle,2,pxy);
				v_pline(vdi_handle,2,&pxy[4]);
			}
		vs_clip(vdi_handle,0,cxy);
		return(parmblock->pb_currstate & ~SELECTED);
	}

void init_vars(void)
	{	char temp[MAX_STR];
		int wchar,hchar,wbox,hbox,min_x,min_y,i,j;
		long value;

		wind_get(0,WF_WORKXYWH,&min_x,&min_y,&max_x,&max_y);
		max_x += min_x;
		max_y += min_y;
		mid_x = max_x / 2;
		mid_y = max_y / 2;
		
		Fsetdta(&dta_buffer);

		aes_handle=graf_handle(&wchar,&hchar,&wbox,&hbox);

		for(i=1; i<10; _VDIParBlk.intin[i++]=1);
		_VDIParBlk.intin[0]=1;			/* SCREEN	*/
		_VDIParBlk.intin[10]=2;			/* RC			*/
		vdi_handle=aes_handle;
		v_opnvwk(_VDIParBlk.intin,&vdi_handle,_VDIParBlk.intout);

		def_drive = Dgetdrv();
		Dgetpath(temp,def_drive+1);

		sprintf(def_path,"%c:%s",(def_drive+65),temp);
		strcpy(def_path_slash,def_path);
		strcat(def_path_slash,"\\");

		for (j=0; j<=FREQSETUPS-1; j++)
			for (i=0; i<=9; i++)
				*filelist[j][i] = 
				*filetext[j][i] =
				*filelistnode[j][i] = EOS;
		for (i=0; i<=MAX_GETFILE-1; i++)
			*getfile[i] = EOS;
		for (i=0; i<=MAX_INFOBT300*2-1; i++)
			*bt300[i] = EOS;
		for (j=0; j<=MAXSESSION-1; j++)
			{	for (i=1; i<=MAXPRGSESS; i++)
					{	*s_session[j][i][0] = *s_session[j][i][1] = EOS;
						w_session[j][i][0] = w_session[j][i][1] = 0;
					}
				*s_timesess[j][0] = *s_timesess[j][1]=EOS;
				i_session[j] = sflags[j][0] = sflags[j][1] = 0;
			}
		for (j=0; j<=16; j++)
			{	*program[j] = *program2[j] = EOS;
				program3[j] = 0;
			}
		for (i=0; i<=MAX_HATCH-1; i++)
			*hatchdefnode[i] = *hatchdefpw[i] = *hatchdefarea[i] = 
			*hatchdefsysop[i] = EOS;

		for (i=0; i<=MAX_DOMAIN-1; i++)
			*domain[i][0] = *domain[i][1] = EOS;

		for (i=0; i<=MAX_EPROG-1; i++)
			*eprogram[i][0] =	*eprogram[i][1] = *eprogtxt[i] = EOS;

		for (i=0; i<=MAX_ADDRESS-1; i++)
			{	*address[i] = EOS;
				fakenet[i] = 0;
			}

		for (i=0; i<=2; i++)
			*netmail[i] = EOS;

		for (i=0; i<=MAX_UTILS-1; i++)
			sprintf(txtutil[i],"Utility %u",i+1);
		for (i=0; i<=MAXSESSION-1; i++)
			sprintf(txtsess[i],"*** %u ***",i+1);
		for (i=0; i<=MAX_HATCH-1; i++)
			sprintf(hatchdefbutton[i],"*** %u ***",i+1);

		*hatchpassword = *hatcharea =	*hatchnode = *hatchsysop =
		*pareas = *areas = *pbinkley = *binkley = *psemper = *semper =
		*dodate = *dotime = EOS;
		
		aes_version = _GemParBlk.global[0];
		gemdos_version = Sversion();

		MagX = getcookie( 'MagX', &value ) ? (MAGX_COOKIE *) value : NULL;

		if ( aes_version>=0x400 ||
				 wind_get(0,'WX',&i,&i,&i,&i)=='WX'	||
				 (MagX && MagX->aesvars && MagX->aesvars->version>=0x200) )
			{	if (appl_getinfo(4,&i,&i,&j,&i))
					has_appl_search = j ? TRUE : FALSE;
			}
	}

void systemfolder(void)
	{	char *copy_buffer,temp1[MAX_STR],temp2[MAX_STR];
		int i;
		set_defaultdir();
		if (xFsfirst("AVALON",FA_SUBDIR)!=0)
			{	installinfo();
				if (Dcreate("AVALON")<0)
					form_alert(1,msg[44]);
				else
					{	copy_buffer = (char *) malloc(BUFFER_SIZE);
						if (copy_buffer==NULL)
							{	memory_error();
							}
						else
							{	if (copy_files("AVALON.CFG","AVALON\\AVALON.CFG",copy_buffer)==0)
									remove("AVALON.CFG");
								if (copy_files("AVALON.HTC","AVALON\\AVALON.HTC",copy_buffer)==0)
									remove("AVALON.HTC");
								if (copy_files("AVALON.WPX","AVALON\\AVALON.WPX",copy_buffer)==0)
									remove("AVALON.WPX");
								if (copy_files("AVALON.TNB","AVALON\\AVALON.TNB",copy_buffer)==0)
									remove("AVALON.TNB");
								if (copy_files("AVALON.LOG","AVALON\\AVALON.LOG",copy_buffer)==0)
									remove("AVALON.LOG");
								if (copy_files("AVALON.LST","AVALON\\AVALON.LST",copy_buffer)==0)
									remove("AVALON.LST");
								if (copy_files("AVALON.KEY","AVALON\\AVALON.KEY",copy_buffer)==0)
									remove("AVALON.KEY");
								for (i=0; i<=5; i++)
									{	sprintf(temp1,"BINKLEY.%u",i);
										sprintf(temp2,"AVALON\\%s",temp1);
										if (copy_files(temp1,temp2,copy_buffer)==0)
											remove(temp1);
									}
							}
						free(copy_buffer);
					}
				if (exist("AVALON\\AVALON.CFG")!=0)		/* Gibt es noch nicht */
					{	if (installbinksemp()==USESEMP)
							{	strcpy(temp2,"CONFIG.DAT");
								if ( fileselect(temp1,temp2,"CONFIG.DAT","Where is CONFIG.DAT?") != 0 )
									{	strcpy(psemper,temp1);
										add_slash(psemper);		/* Nur falls etwas 'drin steht */
										sprintf(semper,"%s%s",psemper,"CONFIG.DAT");
									}
							}
						else
							{	strcpy(temp2,"BINKLEY.CFG");
								if ( fileselect(temp1,temp2,"BINKLEY.CFG","Where is BINKLEY.CFG?") != 0 )
									{	strcpy(pbinkley,temp1);
										add_slash(pbinkley);		/* Nur falls etwas 'drin steht */
										sprintf(binkley,"%s%s",pbinkley,"BINKLEY.CFG");
									}
							}

						/* Absolute beginner setup */

						if (needhelp()==INEEDHLP)
							{	form_alert(1,"[0][ Please select the programs | you use! | Bitte w„hle nun die benutzten | Programme!][  OK  ]");

								lese_cfg();

								if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
									form_alert(1,msg[23]);
								xform_dial(tree3,XFMD_START,&handle,&wnx,&wny,&wnw,&wnh);

								undo_xflags(tree3,EDITABLE|TOUCHEXIT,AVIEW,ABBSEDIT,-1);
								undo_xflags(tree3,SELECTABLE|EXIT,ACANCEL,-1);
								do_xstate(tree3,DISABLED,ACANCEL,
																				 ANOTICE,
																				 AVIEW,
																				 ABBSTEXT,
																				 ABBSEDIT,-1);
								setup2();
								do_xflags(tree3,EDITABLE|TOUCHEXIT,AVIEW,ABBSEDIT,-1);
								do_xflags(tree3,SELECTABLE|EXIT,ACANCEL,-1);
								undo_xstate(tree3,DISABLED,ACANCEL,
																					 ANOTICE,
																					 AVIEW,
																					 ABBSTEXT,
																					 ABBSEDIT,-1);

								strcpy(s_session[0][1][0],"###1");	/* Exporter */
								strcpy(s_session[0][1][1],"###1");
								strcpy(s_session[0][2][0],"###7");	/* Mailer		*/
								strcpy(s_session[0][2][1],"###7");
								strcpy(s_session[0][3][0],"###2");	/* Importer */
								strcpy(s_session[0][3][1],"###2");
								strcpy(s_session[0][4][0],"###5");	/* MsgEdi		*/
								strcpy(s_session[0][4][1],"###5");
								
								strcpy(txtsess[0],"Poll");
								sflags[0][0] = F_NOMAIL;
								sflags[0][1] = 0;						/* loops */
								i_session[0] = 4;
								changes |= C_SESSION;

								xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
								if (xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
									form_alert(1,msg[23]);
								
								whatnow();
								set_defaultdir();
							}
					}
			}
	}

void desktop()
	{	int x,y,w,h;
		wind_get(0,WF_WORKXYWH,&x,&y,&w,&h);
		form_dial(FMD_START,x,y,w,h,x,y,w,h);
		form_dial(FMD_FINISH,x,y,w,h,x,y,w,h);
	}

void wait(void)
	{	int wahl,nx,ny,nw,nh;
		nw = OB_W(tree13,ROOT) +16;
		nh = OB_H(tree13,ROOT) +16;
		nx = max_x-nw-20;
		ny = 20;
		OB_X(tree13,ROOT) = nx+4;
		OB_Y(tree13,ROOT) = ny+4;
		form_dial(FMD_START,nx,ny,nw,nh,nx,ny,nw,nh);
		objc_draw(tree13,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahl = xform_do3(tree13,0) & 0x7ff;
		if (wahl<100)
			undo_state(tree13,wahl,SELECTED);
		form_dial(FMD_FINISH,nx,ny,nw,nh,nx,ny,nw,nh);
	}

/* =============================================================== */
/* =MAIN-ROUTINEN================================================= */
/* =============================================================== */

void sess_hell(int x0, int x1, int x2, int x3, int x4)
	{	if (i_session[x0]<=0)
			{	do_state(tree1,x1,DISABLED);
				do_state(tree1,x2,DISABLED);
				menu_ienable(tree12,x3,0);
				menu_ienable(tree12,x4,0);
			}
		else
			{	undo_state(tree1,x1,DISABLED);
				undo_state(tree1,x2,DISABLED);
				menu_ienable(tree12,x3,1);
				menu_ienable(tree12,x4,1);
			}
	}

void session_hell(void)
	{	sess_hell(0,START1,AUTO1,MPOLL1,MAUTO1);
		sess_hell(1,START2,AUTO2,MPOLL2,MAUTO2);
		sess_hell(2,START3,AUTO3,MPOLL3,MAUTO3);
		sess_hell(3,START4,AUTO4,MPOLL4,MAUTO4);
		sess_hell(4,START5,AUTO5,MPOLL5,MAUTO5);
		sess_hell(5,START6,AUTO6,MPOLL6,MAUTO6);
	}

void pnt_hell(char *x0, int x1, int x2)
	{	char dum[PATH_MAX];
		sprintf(dum,"AVALON\\%s",x0);
		if ( exist(dum)!=0 )
			{	do_state(tree1,x1,DISABLED);
				menu_ienable(tree12,x2,0);
			}
		else
			{	undo_state(tree1,x1,DISABLED);
				menu_ienable(tree12,x2,1);
			}
	}

void pnt_hell2(int x1, int x2)
	{	do_state(tree1,x1,DISABLED);
		menu_ienable(tree12,x2,0);
	}

void point_hell(void)
	{	if (*psemper!=EOS && exist(semper)==0)
			{	pnt_hell2(POINT0,MPOINT1);
				pnt_hell2(POINT1,MPOINT2);
				pnt_hell2(POINT2,MPOINT3);
				pnt_hell2(POINT3,MPOINT4);
				pnt_hell2(POINT4,MPOINT5);
				pnt_hell2(POINT5,MPOINT6);
			}
		else
			{	pnt_hell("BINKLEY.0",POINT0,MPOINT1);
				pnt_hell("BINKLEY.1",POINT1,MPOINT2);
				pnt_hell("BINKLEY.2",POINT2,MPOINT3);
				pnt_hell("BINKLEY.3",POINT3,MPOINT4);
				pnt_hell("BINKLEY.4",POINT4,MPOINT5);
				pnt_hell("BINKLEY.5",POINT5,MPOINT6);
			}
	}

void birthday(void)
	{	char temp[128];
		if ( (HEUTE.da_day==21) && (HEUTE.da_mon==10) )
			{	sprintf(temp,tmsg[10],HEUTE.da_year-1970);
				form_alert(1,temp);
			}
	}

void info_box(void)
	{	int i;
		switch( bt_version )
			{	case	999 :
				case	241 : {	for (i=0; i<=MAX_INFOBT300-1; i++)
												{	put_text(tree1,BT3001+i,bt300[i],46);
													OB_FLAGS(tree1,BT3001+i) &= ~HIDETREE;
							
													put_text(tree1,BT3007+i,bt300[6+i],46);
													OB_FLAGS(tree1,BT3007+i) &= ~HIDETREE;
												}
										}	break;
			}

		if (bt_version==0)
			{	OB_FLAGS(tree1,TBOX) |= HIDETREE;
				OB_FLAGS(tree1,TLOGO) &= ~HIDETREE;
			}
		else
			{	OB_FLAGS(tree1,TBOX) &= ~HIDETREE;
				OB_FLAGS(tree1,TLOGO) |= HIDETREE;
			}
	}

/* =============================================================== */
/* =LOAD/SAVE-INFO================================================ */
/* =============================================================== */

void load(void)
	{	char xversion[5], dum[MAX_STR];
		FILE *favalon;
		int t,i,j,ver;

		if ( exist("AVALON\\AVALON.CFG")==0 )
			{	favalon = fopen("AVALON\\AVALON.CFG", "r");
				if (favalon==NULL)
					{	fatal_error("AVALON\\AVALON.CFG");
					}
				else
					{	setvbuf(favalon,NULL,_IOFBF,4096);
						input(xversion, MAX_STR, favalon);
						xversion[3]=EOS;
						ver = atoi(xversion);
						switch ( ver )
							{	case 340	:
								case 350	:
								case 351	:
								case 352	:
								case 353	:
								case 360	:	{	fgets(dum, (int) MAX_STR, favalon);

															for ( t=1; t<=16; t++ )
																{	input(program[t], MAX_STR, favalon);
																	input(program2[t], MAX_STR, favalon);
																	fgets(dum, (int) MAX_STR, favalon);
																	program3[t] = atoi(dum);
																}

															for ( j=0; j<=MAXSESSION-1; j++ )
																{	fgets(dum, (int) MAX_STR, favalon);
																	fgets(dum, (int) MAX_STR, favalon);
																	i_session[j] = atoi(dum);
																	fgets(dum, (int) MAX_STR, favalon);
																	sflags[j][0] = atoi(dum);
																	fgets(dum, (int) MAX_STR, favalon);
																	sflags[j][1] = atoi(dum);
																	for ( t=1; t<=MAXPRGSESS; t++ )
																		{	input(s_session[j][t][0], MAX_STR, favalon);
																			input(s_session[j][t][1], MAX_STR, favalon);
																			fgets(dum, (int) MAX_STR, favalon);
																			w_session[j][t][0] = atoi(dum);
																			fgets(dum, (int) MAX_STR, favalon);
																			w_session[j][t][1] = atoi(dum);
																		}
																	input(s_timesess[j][0], MAX_STR, favalon);
																	input(s_timesess[j][1], MAX_STR, favalon);
																}

															if ( ver>=360 )
																{	fgets(dum, (int) MAX_STR, favalon);
																	for (t=0; t<=MAX_EPROG-1; t++)
																		{	input(eprogram[t][0], MAX_STR, favalon);
																			input(eprogram[t][1], MAX_STR, favalon);
																			input(eprogtxt[t], MAX_STR, favalon);
																			fgets(dum,(int) MAX_STR,favalon);
																			eprogram3[t] = atoi(dum);
																		}
																}

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=5; t++)
																input(txtutil[t], MAX_STR, favalon);

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAXSESSION-1; t++)
																input(txtsess[t], MAX_STR, favalon);

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAX_SETUP-1; t++)
																input(txtname[t], MAX_STR, favalon);

															fgets(dum, (int) MAX_STR, favalon);
															if ( ver>=350 )
																{	fgets(dum, (int) MAX_STR, favalon);
																	i = atoi(dum);
																	for (j=0; j<=i-1; j++)
																		for (t=0; t<=9; t++)
																			{	input(filelist[j][t], MAX_STR, favalon);
																				input(filetext[j][t], MAX_STR, favalon);
																				input(filelistnode[j][t], MAX_STR, favalon);
																			}
																}
															else
																{	for (t=0; t<=9; t++)
																		{	input(filelist[0][t], MAX_STR, favalon);
																			input(filetext[0][t], MAX_STR, favalon);
																			input(filelistnode[0][t], MAX_STR, favalon);
																		}
																}

															fgets(dum, (int) MAX_STR, favalon);
															filebutton = atoi(dum);
															if ( ver>=350 )
																{	fgets(dum, (int) MAX_STR, favalon);
																	fileblock = atoi(dum);
																}

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAX_GETFILE-1; t++)
																input(getfile[t], MAX_STR, favalon);

															fgets(dum, (int) MAX_STR, favalon);
															input(pareas, MAX_STR, favalon);
															input(pbinkley, MAX_STR, favalon);
															if (ver<=352)
																fgets(dum, (int) MAX_STR, favalon);

															if (ver>=353)
																input(psemper, MAX_STR, favalon);
																
															if (ver<=350)
																fgets(dum, (int) MAX_STR, favalon);

															fgets(dum,(int) MAX_STR,favalon);
															bt_version = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															divflags = atol(dum);
															fgets(dum,(int) MAX_STR,favalon);
															hatchflags = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															serport = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															vt52 = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															voll = atoi(dum);
															if (ver >= 352 )
																{	fgets(dum,(int) MAX_STR,favalon);
																	fontpnt = atoi(dum);
																}
															fgets(dum,(int) MAX_STR,favalon);
															input(hatchpassword, MAX_STR, favalon);
															input(hatchnode, MAX_STR, favalon);
															input(hatchsysop, MAX_STR, favalon);
															input(hatcharea, MAX_STR, favalon);
															if ( ver>=350 )
																{	fgets(dum,(int) MAX_STR,favalon);
																	i = atoi(dum);
																}
															else
																{	i = 9;
																}
															for (t=0; t<=i-1; t++)
																{	input(hatchdefbutton[t], MAX_STR, favalon);
																	input(hatchdefnode[t], MAX_STR, favalon);
																	input(hatchdefsysop[t], MAX_STR, favalon);
																	input(hatchdefarea[t], MAX_STR, favalon);
																	input(hatchdefpw[t], MAX_STR, favalon);
																}
															for (t=0; t<=2; t++)
																{	input(mailname[t], MAX_STR, favalon);
																	input(netmail[t], MAX_STR, favalon);
																}
															break;
														}
								default		:	{	form_alert(1,msg[7]);
															break;
														}
							}
						fclose(favalon);
					}
			}
		sprintf(areas,"%s%s",pareas,"AREAS.BBS");
		sprintf(dareas,"%s%s",pareas,"DAREAS.BBS");
		sprintf(binkley,"%s%s",pbinkley,"BINKLEY.CFG");
		sprintf(semper,"%s%s",psemper,"CONFIG.DAT");
	}

void load2(int config)
	{	char xversion[5], dum[MAX_STR];
		FILE *favalon;
		int t,i,j,ver;

		sprintf(dum,"AVALON\\AVALON%u.CFG",config);
		if ( exist(dum)==0 )
			{	favalon = fopen(dum, "r");
				if (favalon==NULL)
					{	fatal_error(dum);
					}
				else
					{	setvbuf(favalon,NULL,_IOFBF,4096);
						input(xversion, MAX_STR, favalon);
						xversion[3]=EOS;
						ver = atoi(xversion);
						if (ver==360)
							{ switch (config)
									{	case	1	:	/* PROGRAMS + UTILITIES */
															fgets(dum, (int) MAX_STR, favalon);
															for ( t=1; t<=16; t++ )
																{	input(program[t], MAX_STR, favalon);
																	input(program2[t], MAX_STR, favalon);
																	fgets(dum, (int) MAX_STR, favalon);
																	program3[t] = atoi(dum);
																}

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=5; t++)
																input(txtutil[t], MAX_STR, favalon);

															/* SESSIONS */

															for ( j=0; j<=MAXSESSION-1; j++ )
																{	fgets(dum, (int) MAX_STR, favalon);
																	fgets(dum, (int) MAX_STR, favalon);
																	i_session[j] = atoi(dum);
																	fgets(dum, (int) MAX_STR, favalon);
																	sflags[j][0] = atoi(dum);
																	fgets(dum, (int) MAX_STR, favalon);
																	sflags[j][1] = atoi(dum);
																	for ( t=1; t<=MAXPRGSESS; t++ )
																		{	input(s_session[j][t][0], MAX_STR, favalon);
																			input(s_session[j][t][1], MAX_STR, favalon);
																			fgets(dum, (int) MAX_STR, favalon);
																			w_session[j][t][0] = atoi(dum);
																			fgets(dum, (int) MAX_STR, favalon);
																			w_session[j][t][1] = atoi(dum);
																		}
																	input(s_timesess[j][0], MAX_STR, favalon);
																	input(s_timesess[j][1], MAX_STR, favalon);
																}

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAXSESSION-1; t++)
																input(txtsess[t], MAX_STR, favalon);

															/* Program-Manager */

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAX_EPROG-1; t++)
																{	input(eprogram[t][0], MAX_STR, favalon);
																	input(eprogram[t][1], MAX_STR, favalon);
																	input(eprogtxt[t], MAX_STR, favalon);
																	fgets(dum,(int) MAX_STR,favalon);
																	eprogram3[t] = atoi(dum);
																}

															/* SPECIAL + TERMINAL */

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAX_SETUP-1; t++)
																input(txtname[t], MAX_STR, favalon);

															fgets(dum,(int) MAX_STR,favalon);
															fgets(dum,(int) MAX_STR,favalon);
															bt_version = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															serport = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															vt52 = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															voll = atoi(dum);

															break;

										case	2	:	/* SPECIAL */
															fgets(dum, (int) MAX_STR, favalon);

															input(pareas, MAX_STR, favalon);
															input(pbinkley, MAX_STR, favalon);
															input(psemper, MAX_STR, favalon);
															fgets(dum,(int) MAX_STR,favalon);
															divflags = atol(dum);
															fgets(dum,(int) MAX_STR,favalon);
															fontpnt = atoi(dum);

															/* FILEREQUEST */

															fgets(dum, (int) MAX_STR, favalon);
															fgets(dum, (int) MAX_STR, favalon);
															i = atoi(dum);
															for (j=0; j<=i-1; j++)
																for (t=0; t<=9; t++)
																	{	input(filelist[j][t], MAX_STR, favalon);
																		input(filetext[j][t], MAX_STR, favalon);
																		input(filelistnode[j][t], MAX_STR, favalon);
																	}
															fgets(dum, (int) MAX_STR, favalon);
															filebutton = atoi(dum);
															fgets(dum, (int) MAX_STR, favalon);
															fileblock = atoi(dum);

															fgets(dum, (int) MAX_STR, favalon);
															for (t=0; t<=MAX_GETFILE-1; t++)
																input(getfile[t], MAX_STR, favalon);

															/* HATCH */

															fgets(dum,(int) MAX_STR,favalon);
															fgets(dum,(int) MAX_STR,favalon);
															hatchflags = atoi(dum);
															fgets(dum,(int) MAX_STR,favalon);
															input(hatchpassword, MAX_STR, favalon);
															input(hatchnode, MAX_STR, favalon);
															input(hatchsysop, MAX_STR, favalon);
															input(hatcharea, MAX_STR, favalon);
															fgets(dum,(int) MAX_STR,favalon);
															i = atoi(dum);
															for (t=0; t<=i-1; t++)
																{	input(hatchdefbutton[t], MAX_STR, favalon);
																	input(hatchdefnode[t], MAX_STR, favalon);
																	input(hatchdefsysop[t], MAX_STR, favalon);
																	input(hatchdefarea[t], MAX_STR, favalon);
																	input(hatchdefpw[t], MAX_STR, favalon);
																}
															for (t=0; t<=2; t++)
																{	input(mailname[t], MAX_STR, favalon);
																	input(netmail[t], MAX_STR, favalon);
																}
															break;
									}
							}
						else
							{	form_alert(1,msg[7]);
							}
						fclose(favalon);
					}
			}
		sprintf(areas,"%s%s",pareas,"AREAS.BBS");
		sprintf(dareas,"%s%s",pareas,"DAREAS.BBS");
		sprintf(binkley,"%s%s",pbinkley,"BINKLEY.CFG");
		sprintf(semper,"%s%s",psemper,"CONFIG.DAT");
	}

void save2(int config)
	{	char	temp[PATH_MAX];
		int		t,j;
		FILE	*favalon;

		set_defaultdir();

		sprintf(temp,"AVALON\\AVALON%u.CFG", config);
		favalon = fopen(temp, "w");
		if (favalon==NULL)
			{	fatal_error(temp);
			}
		else
			{	output(CFGVERSION, favalon);
				switch(config)
					{	case	1	:	/* PROGRAMS + UTILITIES */
											output("--- Programs and commandlines", favalon);
											for ( t=1; t<=16; t++ )
												{	output(program[t], favalon);
													output(program2[t], favalon);
													sprintf(temp, "%u", program3[t]);
													output(temp, favalon);
												}
											output("--- Utilities", favalon);
											for (t=0; t<=MAX_UTILS-1; t++)
												output(txtutil[t], favalon);

											/* SESSIONS */

											for ( j=0; j<=MAXSESSION-1; j++ )
												{	sprintf(temp, "--- Session #%u", j+1);
													output(temp, favalon);
													sprintf(temp, "%u", i_session[j]);
													output(temp, favalon);
													sprintf(temp, "%u", sflags[j][0]);
													output(temp, favalon);
													sprintf(temp, "%u", sflags[j][1]);
													output(temp, favalon);
													for ( t=1; t<=MAXPRGSESS; t++ )
														{	output(s_session[j][t][0], favalon);
															output(s_session[j][t][1], favalon);
															sprintf(temp, "%u", w_session[j][t][0]);
															output(temp, favalon);
															sprintf(temp, "%u", w_session[j][t][1]);
															output(temp, favalon);
														}
													output(s_timesess[j][0], favalon);
													output(s_timesess[j][1], favalon);
												}
											output("--- Sessions", favalon);
											for (t=0; t<=MAXSESSION-1; t++)
												output(txtsess[t], favalon);

											/* Program-Manager */

											output("--- Programs", favalon);
											for (t=0; t<=MAX_EPROG-1; t++)
												{	output(eprogram[t][0], favalon);
													output(eprogram[t][1], favalon);
													output(eprogtxt[t], favalon);
													sprintf(temp, "%u", eprogram3[t]);
													output(temp, favalon);
												}

											/* SPECIAL + TERMINAL */

											output("--- Setups", favalon);
											for (t=0; t<=MAX_SETUP-1; t++)
												output(txtname[t], favalon);

											output("--- Spezial-Flags", favalon);
											sprintf(temp, "%u", bt_version);
											output(temp, favalon);
											sprintf(temp, "%u", serport);
											output(temp, favalon);
											sprintf(temp, "%u", vt52);
											output(temp, favalon);
											sprintf(temp, "%u", voll);
											output(temp, favalon);

											break;

						case	2	:	/* SPECIAL */

											output("--- Spezial-Paths and Flags", favalon);
											output(pareas, favalon);
											output(pbinkley, favalon);
											output(psemper, favalon);
											sprintf(temp, "%lu", divflags);
											output(temp, favalon);
											sprintf(temp, "%u", fontpnt);
											output(temp, favalon);

											/* FILEREQUEST */

											output("--- FileRequest", favalon);
											sprintf(temp, "%u", FREQSETUPS);
											output(temp, favalon);
											for (j=0; j<=FREQSETUPS-1; j++)
												for (t=0; t<=9; t++)
													{	output(filelist[j][t], favalon);
														output(filetext[j][t], favalon);
														output(filelistnode[j][t], favalon);
													}
											sprintf(temp, "%u", filebutton);
											output(temp, favalon);
											sprintf(temp, "%u", fileblock);
											output(temp, favalon);
											output("--- FileRequest-Defaults", favalon);
											for (t=0; t<=MAX_GETFILE-1; t++)
												output(getfile[t], favalon);

											/* HATCH */

											output("--- HatchFlags", favalon);
											sprintf(temp, "%u", hatchflags);
											output(temp, favalon);
											output("--- FileHatch", favalon);
											output(hatchpassword, favalon);
											output(hatchnode, favalon);
											output(hatchsysop, favalon);
											output(hatcharea, favalon);
											sprintf(temp, "%u", MAX_HATCH);
											output(temp, favalon);
											for (t=0; t<=MAX_HATCH-1; t++)
												{	output(hatchdefbutton[t], favalon);
													output(hatchdefnode[t], favalon);
													output(hatchdefsysop[t], favalon);
													output(hatchdefarea[t], favalon);
													output(hatchdefpw[t], favalon);
												}
											for (t=0; t<=2; t++)
												{	output(mailname[t], favalon);
													output(netmail[t], favalon);
												}
											break;
					}
				fclose(favalon);
				changes = 0;
			}
	}

void loadpositions(void)
	{	char	temp[PATH_MAX];
		FILE	*favalon;
		int		t,x,y;
		if (exist("AVALON\\AVALON.WPX")==0)
			{	favalon = fopen("AVALON\\AVALON.WPX", "r");
				if (favalon==NULL)
					{	fatal_error("AVALON\\AVALON.WPX");
					}
				else
					{	fgets(temp, (int) PATH_MAX, favalon);
						temp[3]=EOS;
						if ( atoi(temp)==atoi(WPSVERSION) )
							{	fgets(temp, (int) PATH_MAX, favalon);
								while (!feof(favalon))
									{	if (fgets(temp, (int) PATH_MAX, favalon)==NULL)
											break;
										sscanf(temp,"%u,%u,%u",&t,&x,&y);
										switch(t)
											{	case	DIALOG1		:	setzexy(tree1,x,y); break;
												case	DIALOG2		:	setzexy(tree2,x,y); break;
												case	DIALOG3		:	setzexy(tree3,x,y); break;
												case	DIALOG5		:	setzexy(tree5,x,y); break;
												case	DIALOG6		:	setzexy(tree6,x,y); break;
												case	DIALOG7		:	setzexy(tree7,x,y); break;
												case	DIALOG8		:	setzexy(tree8,x,y); break;
		
												case	DIALOG9		:	setzexy(rs_trindex[DIALOG9] ,x,y); break;
												case	DIALOG9B	: setzexy(rs_trindex[DIALOG9B],x,y); break;
		
												case	DIALOG10	:	setzexy(tree10,x,y); break;
												case	DIALOG11	:	setzexy(tree11,x,y); break;
												case	DIALOG12	:	setzexy(tree12,x,y); break;
												case	DIALOG13	:	setzexy(tree13,x,y); break;
												case	DIALOG14	:	setzexy(tree14,x,y); break;
												case	DIALOG15	:	setzexy(tree15,x,y); break;
												case	DIALOG16	:	setzexy(tree16,x,y); break;
												case	DIALOG17	:	setzexy(tree17,x,y); break;
												case	DIALOG18	:	setzexy(tree18,x,y); break;
												case	DIALOG19	:	setzexy(tree19,x,y); break;
												case	DIALOG20	:	setzexy(tree20,x,y); break;
												case	DIALOG21	:	setzexy(tree21,x,y); break;
												case	DIALOG22	:	setzexy(tree22,x,y); break;
												case	DIALOG23	:	setzexy(tree23,x,y); break;
												case	DIALOG24	:	setzexy(tree24,x,y); break;
		
												case	DIALOG25	:	setzexy(rs_trindex[DIALOG25],x,y); break;
												case	DIALO25B	:	setzexy(rs_trindex[DIALO25B],x,y); break;
		
												case	DIALOG26	:	setzexy(tree26,x,y); break;
												case	DIALOG27	:	setzexy(tree27,x,y); break;
												case	DIALOG28	:	setzexy(tree28,x,y); break;
												case	DIALOG29	:	setzexy(tree29,x,y); break;
												case	DIALOG30	:	setzexy(tree30,x,y); break;
												case	DIALOG31	:	setzexy(tree31,x,y); break;
												case	DIALOG33	:	setzexy(tree33,x,y); break;
												case	DIALOG34	:	setzexy(tree34,x,y); break;
												case	DIALOG40	:	setzexy(tree40,x,y); break;
												case	DIALOG41	:	setzexy(tree41,x,y); break;
												case	DIALOG51	:	setzexy(tree51,x,y); break;
												case	DIALOG52	:	setzexy(tree52,x,y); break;
												case	DIALOG61	:	setzexy(tree61,x,y); break;
												case	DIALOG71	:	setzexy(tree71,x,y); break;
												case	DIALOG72	:	setzexy(tree72,x,y); break;
												case	DIALOG81	:	setzexy(tree81,x,y); break;
												case	DIALOG82	:	setzexy(tree82,x,y); break;
												case	DIALOG90	:	setzexy(tree90,x,y); break;
												case	DIALOG91	:	setzexy(tree91,x,y); break;
											}
									}
							}
						fclose(favalon);
					}
			}
	}

void setzexy(OBJECT *tree,int x,int y)
	{	OB_X(tree,0) = x;
		OB_Y(tree,0) = y;
	}

void savepositions(void)
	{	FILE	*favalon;
		set_defaultdir();
		favalon = fopen("AVALON\\AVALON.WPX","w");
		if (favalon==NULL)
			{	fatal_error("AVALON\\AVALON.WPX");
			}
		else
			{	output(WPSVERSION, favalon);
				output("--- Windowpositions", favalon);
				schreibe(DIALOG1,tree1,favalon);
				schreibe(DIALOG2,tree2,favalon);
				schreibe(DIALOG3,tree3,favalon);
				schreibe(DIALOG5,tree5,favalon);
				schreibe(DIALOG6,tree6,favalon);
				schreibe(DIALOG7,tree7,favalon);
				schreibe(DIALOG8,tree8,favalon);

				schreibe(DIALOG9 ,rs_trindex[DIALOG9] ,favalon);
				schreibe(DIALOG9B,rs_trindex[DIALOG9B],favalon);

				schreibe(DIALOG10,tree10,favalon);
				schreibe(DIALOG11,tree11,favalon);
				schreibe(DIALOG12,tree12,favalon);
				schreibe(DIALOG13,tree13,favalon);
				schreibe(DIALOG14,tree14,favalon);
				schreibe(DIALOG15,tree15,favalon);
				schreibe(DIALOG16,tree16,favalon);
				schreibe(DIALOG17,tree17,favalon);
				schreibe(DIALOG18,tree18,favalon);
				schreibe(DIALOG19,tree19,favalon);
				schreibe(DIALOG20,tree20,favalon);
				schreibe(DIALOG21,tree21,favalon);
				schreibe(DIALOG22,tree22,favalon);
				schreibe(DIALOG23,tree23,favalon);
				schreibe(DIALOG24,tree24,favalon);

				schreibe(DIALOG25,rs_trindex[DIALOG25],favalon);
				schreibe(DIALO25B,rs_trindex[DIALO25B],favalon);

				schreibe(DIALOG26,tree26,favalon);
				schreibe(DIALOG27,tree27,favalon);
				schreibe(DIALOG28,tree28,favalon);
				schreibe(DIALOG29,tree29,favalon);
				schreibe(DIALOG30,tree30,favalon);
				schreibe(DIALOG31,tree31,favalon);
				schreibe(DIALOG33,tree33,favalon);
				schreibe(DIALOG34,tree34,favalon);
				schreibe(DIALOG40,tree40,favalon);
				schreibe(DIALOG41,tree41,favalon);
				schreibe(DIALOG51,tree51,favalon);
				schreibe(DIALOG52,tree52,favalon);
				schreibe(DIALOG61,tree61,favalon);
				schreibe(DIALOG71,tree71,favalon);
				schreibe(DIALOG72,tree72,favalon);
				schreibe(DIALOG81,tree81,favalon);
				schreibe(DIALOG82,tree82,favalon);
				schreibe(DIALOG90,tree90,favalon);
				schreibe(DIALOG91,tree91,favalon);
			}
	}

void schreibe(int t,OBJECT *tree,FILE *file)
	{	char temp[MAX_STR];
		sprintf(temp,"%u,%u,%u",t,OB_X(tree,0),OB_Y(tree,0));
		output(temp,file);
	}

/* =============================================================== */
/* =INPUT/OUTPUT================================================== */
/* =============================================================== */

/*	0 = OK
		1 = Not found
		2 = Error
*/
int copy_files(char *pin,char *pout,char *copy_buffer)
	{	int hin,hout;
		if (exist(pin)==0)
			{	hin = (int) Fopen(pin,FO_READ);
				if (hin<0)
					{	fatal_error(pin);
						return(2);
					}
				hout = (int) Fcreate(pout,0);
				if (hout<0)
					{ Fclose(hin);
						fatal_error(pout);
						return(2);
					}
				filecopy(hin,hout,0,copy_buffer);
				Fclose(hout);
				Fclose(hin);
			}
		else
			{	return(1);
			}
		return(0);
	}

/* =============================================================== */
/* =TESTE-SETUP=================================================== */
/* =============================================================== */

void teste_setup(void)
	{	int hin,hout;
		char *copy_buffer,dum[PATH_MAX];

		copy_buffer = (char *) malloc(BUFFER_SIZE);

		if (copy_buffer==NULL)
			{	memory_error();
				return;
			}

		if (*psemper!=EOS && exist(semper)==0)
			{	mailer=SEMPER;
				/* SEMPER-SETUP 		*/
				/* Nichts machen!!! */
			}
		else
			{	mailer=BINKLEY;
				/* BINKLEY-SETUP */
			
				sprintf(dum,"AVALON\\%s","BINKLEY.0");
				if (exist(dum)!=0)
					{	/* Es gibt noch kein BINKLEY.0... */

						if (exist(binkley)!=0)
							{	/* ...und es gibt kein BINKLEY.CFG */

								form_alert(1,msg[9]);
							}
						else
							{	/* ... es gibt aber ein BINKLEY.CFG, also		*/
								/* machen wir aus BINKLEY.CFG ein BINKLEY.0	*/

								hin = (int) Fopen(binkley,FO_READ);
								if (hin<0)	
									{	fatal_error(binkley);
									}
								else
									{	hout = (int) Fcreate(dum,0);
										if (hout<0)
											{ fatal_error(dum);
											}
										else
											{	filecopy(hin,hout,1,copy_buffer);
												Fclose(hout);
											}
										Fclose(hin);
									}
							}
					}
				else
					{	/* Es gibt bereits BINKLEY.0, also machen */
						/* wir aus BINKLEY.0 ein BINKLEY.CFG			*/

						hin = (int) Fopen(dum,FO_READ);
						if (hin<0)
							{	fatal_error(dum);
							}
						else
							{	hout = (int) Fcreate(binkley,0);
								if (hout<0)
									{	fatal_error(binkley);
									}
								else
									{	filecopy(hin,hout,1,copy_buffer);
										Fclose(hout);
									}
								Fclose(hin);
							}
					}
			}
		free(copy_buffer);
	}

/* =============================================================== */
/* =LESE-CFG====================================================== */
/* =============================================================== */

void lese_cfg(void)
	{	FILE *fcfg;

		set_defaultdir();
		if (*psemper!=EOS && exist(semper)==0)
			{	mailer=SEMPER;
				fcfg = fopen( semper , "r");
				teste_datei(fcfg);
				fclose(fcfg);
			}
		else if (exist(binkley)==0)
			{	mailer=BINKLEY;
				fcfg = fopen( binkley , "r");
				teste_datei(fcfg);
				fclose(fcfg);
			}
	}

void teste_datei(FILE *xcfg)
	{	char txt[MAX_STR],dum[MAX_STR],*pointer;
		int count=0,pos,i,j;
		long temp;

		strcpy(demo_dat,bak_demo_dat);
		demo_date = bak_demo_date;

		for (i=0; i<=(MAX_INFOBT300*2)-1; i++)
			*bt300[i]=EOS;

		*hold = *netfile = *txtsysop =
		*knowninbound = *protinbound =
		*statuslog = EOS;
		sprintf(capture,"%s%s",def_path_slash,"AVALON\\AVALON.LOG");

		domain_cnt = address_cnt = -1;

		do
			{	input(txt, MAX_STR, xcfg);
				pointer=txt; trim(&pointer);

				if ( *txtsysop==EOS && strnicmp(txt,"SYSOP ",6)==0 )
					{	pointer += 6;	trim(&pointer);
						strcpy(txtsysop,pointer);
					}
				if ( strnicmp(txt,"BAUD ",5)==0 ||
						 strnicmp(txt,"SPEED ",6)==0 )		/* SEMPER */
					{	pointer += 5;
						if (*txt=='S' || *txt=='s') pointer++;
						trim(&pointer);
						temp = atol(pointer);
								 if (temp==153600l)	baud=14;
						else if (temp==115200l)	baud=11;
						else if (temp== 76800l)	baud=15;
						else if (temp== 57600l)	baud=12;
						else if (temp== 38400l)	baud=13;
						else if	(temp== 19200)	baud=0;
						else if (temp==  9600)	baud=1;
						else if (temp==  4800)	baud=2;
						else if (temp==  2400)	baud=4;
						else if (temp==  1200)	baud=7;
						else if (temp==   300)	baud=9;
						else if (temp==   150)	baud=11;
						else if (temp==   134)	baud=12;
						else if (temp==   110)	baud=13;
						else if (temp==    75)	baud=14;
						else if (temp==    50)	baud=15;
					}
				if ( *netmail[0]==EOS && strnicmp(txt,"NETMAIL ",8)==0 )
					{	pointer += 8;	trim(&pointer);
						strncpy(netmail[0],pointer,50);
						netmail[0][50] = EOS;
					}
				if ( *hold==EOS && strnicmp(txt,"HOLD ",5)==0 )
					{	pointer += 5;	trim(&pointer);
						strcpy(hold,pointer);
						add_slash(hold);
					}
				if ( *hold==EOS && strnicmp(txt,"OUTBOUND ",9)==0 )	/* SEMPER */
					{	pointer += 9;	trim(&pointer);
						strcpy(hold,pointer);
						add_slash(hold);
					}
				if ( (strnicmp(txt,"NETFILE ",8)==0 ||
							strnicmp(txt,"INBOUND ",8)==0 ) && *netfile==EOS)
					{	pointer += 8;	trim(&pointer);
						strcpy(netfile,pointer);
						add_slash(netfile);
					}
				if ( strnicmp(txt,"KNOWNINBOUND ",13)==0 && *knowninbound==EOS)
					{	pointer += 13; trim(&pointer);
						strcpy(knowninbound,pointer);
						add_slash(knowninbound);
					}
				if ( strnicmp(txt,"PROTINBOUND ",12)==0 && *protinbound==EOS)
					{	pointer += 12; trim(&pointer);
						strcpy(protinbound,pointer);
						add_slash(protinbound);
					}
				if ( strnicmp(txt,"PROTECTINBOUND ",15)==0 && *protinbound==EOS)
					{	pointer += 15; trim(&pointer);
						strcpy(protinbound,pointer);
						add_slash(protinbound);
					}
				if ( strnicmp(txt,"CAPTUREFILE ",12)==0)
					{	pointer += 12; trim(&pointer);
						strcpy(capture,pointer);
					}
				if ( strnicmp(txt,"STATUSLOG ",10)==0)
					{	pointer += 10; trim(&pointer);
						strcpy(statuslog,pointer);
					}
				if ( strnicmp(txt,"LOGFILE ",8)==0)			/* SEMPER */
					{	pointer += 8; trim(&pointer);
						strcpy(statuslog,pointer);
					}

				if (mailer==BINKLEY)
					{	if ( strnicmp(txt,"ADDRESS ",8)==0 )
							{	pointer += 8;	trim(&pointer);
								address_cnt++;
								if (address_cnt<=MAX_ADDRESS-1)
									{	if (sscanf(pointer,"%s %u",address[address_cnt],
																							 &fakenet[address_cnt])==1)
										fakenet[address_cnt]=0;
									}
								else
									{	--address_cnt;
									}
							}
						if ( strnicmp(txt,"DOMAIN ",7)==0 )
							{	pointer += 7; trim(&pointer);
								domain_cnt++;
								if (domain_cnt<=MAX_DOMAIN-1)
									{	sscanf(pointer,"%s %s",domain[domain_cnt][0],
																					 domain[domain_cnt][1]);
									}
								else
									{	--domain_cnt;
									}
							}
					}
				else		/* SEMPER */
					{	if ( strnicmp(txt,"ADDRESS ",8)==0 )
							{	pointer += 8;	trim(&pointer);
								address_cnt++;
								if (address_cnt<=MAX_ADDRESS-1)
									{	strcpy(address[address_cnt],pointer);
										fakenet[address_cnt]=0;
										do
											{	input(dum, MAX_STR, xcfg);
												pointer=dum; trim(&pointer);
												if ( strnicmp(pointer,"FAKENET ",8)==0 )
													{	pointer += 8; trim(&pointer);
														fakenet[address_cnt] = atoi(pointer);
														pointer=dum;
													}
											}
										while (strnicmp(pointer,"ENDADDRESS",10)!=0);
									}
								else
									{	--address_cnt;
									}
							}
						if ( *program[1]==EOS && strnicmp(txt,"EXPORT ",7)==0 )
							{	pointer += 7; trim(&pointer);
								strcpy(program[1],pointer);
							}
						if ( *program[2]==EOS && strnicmp(txt,"IMPORT ",7)==0 )
							{	pointer += 7; trim(&pointer);
								strcpy(program[2],pointer);
							}
						if ( *program[3]==EOS && strnicmp(txt,"MAINTAIN ",9)==0 )
							{	pointer += 9; trim(&pointer);
								strcpy(program[3],pointer);
							}
						if ( *program[4]==EOS && strnicmp(txt,"SCANNER ",8)==0 )
							{	pointer += 8; trim(&pointer);
								strcpy(program[4],pointer);
							}
						if ( *program[5]==EOS && strnicmp(txt,"READER ",7)==0 )
							{	pointer += 7; trim(&pointer);
								strcpy(program[5],pointer);
							}
						if ( *program[6]==EOS && strnicmp(txt,"EDITOR ",7)==0 )
							{	pointer += 7; trim(&pointer);
								strcpy(program[6],pointer);
							}
						if ( *program[8]==EOS && strnicmp(txt,"TERMINAL ",9)==0 )
							{	pointer += 9; trim(&pointer);
								strcpy(program[8],pointer);
							}

						if ( *program2[1]==EOS && strnicmp(txt,"EXPORTCMD ",10)==0 )
							{	pointer += 10; trim(&pointer);
								strcpy(program2[1],pointer);
							}
						if ( *program2[2]==EOS && strnicmp(txt,"IMPORTCMD ",10)==0 )
							{	pointer += 10; trim(&pointer);
								strcpy(program2[2],pointer);
							}
						if ( *program2[3]==EOS && strnicmp(txt,"MAINTAINCMD ",12)==0 )
							{	pointer += 12; trim(&pointer);
								strcpy(program2[3],pointer);
							}
						if ( *program2[4]==EOS && strnicmp(txt,"SCANNERCMD ",11)==0 )
							{	pointer += 11; trim(&pointer);
								strcpy(program2[4],pointer);
							}
						if ( *program2[5]==EOS && strnicmp(txt,"READERCMD ",10)==0 )
							{	pointer += 10; trim(&pointer);
								strcpy(program2[5],pointer);
							}
						if ( *program2[6]==EOS && strnicmp(txt,"EDITORCMD ",10)==0 )
							{	pointer += 10; trim(&pointer);
								strcpy(program2[6],pointer);
							}
						if ( *program2[8]==EOS && strnicmp(txt,"TERMINALCMD ",12)==0 )
							{	pointer += 12; trim(&pointer);
								strcpy(program2[8],pointer);
							}
					}

				pointer = txt;
				trim(&pointer);

				switch( bt_version )
					{	case	999 : {	if ( count<=(MAX_INFOBT300*2)-1 )
														strncpy(bt300[count++],pointer,46);
												} break;
						case	241 : {	if ( count<=(MAX_INFOBT300*2)-1 )
														if (*pointer!=';' && *pointer!=EOS)
															strncpy(bt300[count++],pointer,46);
												}
					}
			}
		while (feof(xcfg)==0);

		if (mailer==SEMPER)
			{	for (i=0; i<=MAX_DOMAIN-1; i++)	*domain[i][0]=EOS;
				for (i=0; i<=address_cnt; i++)
					{	scanaddr(address[i],&j,&j,&j,&j,dum);
						pos = -1;
						for (j=0; j<=MAX_DOMAIN-1; j++)
							{	if (stricmp(domain[j][0],dum)==0)
									break; /* schon vorhanden */
								if (*domain[j][0]==EOS)
									{	pos = j;
										break;
									}
							} /* in pos steht freier Platz oder -1 */
						if (pos != -1)
							{	domain_cnt++;
								if (domain_cnt<=MAX_DOMAIN-1)
									{	strcpy(domain[pos][0],dum);
										if ((pointer=strchr(dum,'.'))!=NULL) *pointer=EOS;
										strcpy(domain[pos][1],dum);
									}
								else
									{ domain_cnt--;
									}
							}
					}
			}

		demoversion=REGISTERED;
	}

/* =============================================================== */
/* =HELP/INFO===================================================== */
/* =============================================================== */

int tiny_dialog(OBJECT *tree)
	{	int wahl,nx,ny,nw,nh;
		form_center(tree,&nx,&ny,&nw,&nh);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		objc_draw(tree,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahl=form_do(tree,0) & 0x7FFF;
		undo_state(tree,wahl,SELECTED);
		form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
		return(wahl);
	}

void help(void)
	{	tiny_dialog(tree11);
	}

void smallhelp(int feld)
	{	undo_xstate(tree28,SELECTED,HELPLST,HELPFIX,HELPREQ,HELPSND,-1);
		do_state(tree28,feld,SELECTED);
		tiny_dialog(tree28);
	}

void installinfo(void)
	{	wind_update(BEG_UPDATE);
		tiny_dialog(tree40);
		wind_update(END_UPDATE);
	}

int installbinksemp(void)
	{	int wahl;
		wind_update(BEG_UPDATE);
		wahl=tiny_dialog(tree41);
		wind_update(END_UPDATE);
		return(wahl);
	}

int needhelp(void)
	{	int wahl;
		wind_update(BEG_UPDATE);
		wahl=tiny_dialog(tree90);
		wind_update(END_UPDATE);
		return(wahl);
	}

int whatnow(void)
	{	int wahl;
		wind_update(BEG_UPDATE);
		wahl=tiny_dialog(tree91);
		wind_update(END_UPDATE);
		return(wahl);
	}

void info(void)
	{	int xwahl,wahl=0,nx,ny,nw,nh,add,max,loc,i;
		void *buffer[100];
		long frei,gesamt=0;
		char emptylines[3][FREQLEN+1];
	#ifdef _BETA_
		char temp[MAX_STR];
		sprintf(temp,tmsg[11],__DATE__,__TIME__);
		form_alert(1,temp);
	#endif		

		do
			{	frei = (long) Malloc(-1L);
				if (frei > 0)
					{	buffer[++wahl] = Malloc(frei);
						gesamt += frei;
					}
			}
		while (frei != 0);
		i = wahl;
		while (i > 0)
			if (Mfree(buffer[i--])!=0)
				form_alert(1,msg[33]);

		sprintf(emptylines[0],"%*sVersion %s",FREQLEN-13,"",VERSION);
		switch(demoversion)
			{ case REGISTERED :	sprintf(emptylines[1],"This is a registered version for %s.",txtsysop);
													break;
				case TEMPORARY	:	sprintf(emptylines[1],"This version runs with a temporary key. No restrictions until %s",demo_dat);
													break;
				case UNKNOWN		:	sprintf(emptylines[1],"This version runs without any restrictions until %s",demo_dat);
													break;
			}
		sprintf(emptylines[2],"Total free memory: %8lu bytes in %2u blocks",gesamt,wahl);
		registered[INFO_VERSION] = emptylines[0];
		registered[INFO_WITHOUT] = emptylines[1];
		registered[INFO_MEMORY ] = emptylines[2];

/*	Dsetdrv(def_drive);
		Dsetpath(def_path);
		frei=-1;
		if (exist("AVALON.X32")==0)
			{	wahl=(int) Fopen("AVALON.X32",0);
				frei = Fseek(0,wahl,2);
				Fseek(0,wahl,0);
				buffer[0] = calloc(frei,sizeof(char));
				Fread(wahl,frei,buffer[0]);
				Fclose(wahl);
				Dosound(buffer[0]);
			}
		if (frei!=-1)
			{	Bconout(2,7);
				free(buffer[0]);
			}
*/

		if ((divflags & D_MAXIWIND) && (max_y>=672))
			{	Add_Lines=FREQLINESADD;
				tree9=rs_trindex[DIALOG9B];
			}
		else
			{	Add_Lines=0;
				tree9=rs_trindex[DIALOG9];
			}
		add = loc = max = 0;
		while (registered[++max]!=NULL);

		OB_FLAGS(tree9,REQINFO+Add_Lines) |= HIDETREE;
		for (i=FNAME1; i<=FNAME1+FREQLINES+Add_Lines-1; i++)
			{	OB_FLAGS(tree9,i) &= ~(SELECTABLE|TOUCHEXIT);
				undo_state(tree9,i,SELECTED);
			}

		xredraw_slider(tree9,handle,FILEFULL+Add_Lines,
																FILEBK+Add_Lines,
																FILEFD+Add_Lines,max,loc+1,
																FREQLINES+Add_Lines,FALSE);
		zeige_namen(add,max);
		xform_dial(tree9,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	xwahl=xform_do(handle,tree9,NULL,0,0,trans_key1,&wnx,&wny,&wnw,&wnh);
				if (xwahl & 0x4000)
					xwahl = FOK+Add_Lines;
				wahl = xwahl & 0x7ff;
				if (wahl<1000)
					undo_state(tree9,wahl,1);

				if (wahl==FILEBK+Add_Lines)
					{	objc_offset(tree9,FILEFD+Add_Lines,&nx,&ny);
						vq_mouse(vdi_handle,&i,&nw,&nh);
						if (nw<nx)
							xwahl=wahl=FILEDNDN+Add_Lines;
						if (nw>nx+OB_W(tree9,FILEFD+Add_Lines))
							xwahl=wahl=FILEUPUP+Add_Lines;
					}

				if (Add_Lines==0)
					{	switch(wahl)
							{	case FILEDN		:	wahl=1070; break;
								case FILEUP		:	wahl=1071; break;
								case FILEDNDN	:	wahl=1072; break;
								case FILEUPUP	:	wahl=1073; break;
								case FILEFD		:	wahl=1074; break;
							}
					}
				else
					{	switch(wahl)
							{	case FILEDN	 +FREQLINESADD	:	wahl=1070; break;
								case FILEUP	 +FREQLINESADD	:	wahl=1071; break;
								case FILEDNDN+FREQLINESADD	:	wahl=1072; break;
								case FILEUPUP+FREQLINESADD	:	wahl=1073; break;
								case FILEFD	 +FREQLINESADD	:	wahl=1074; break;
							}
					}

				switch(wahl)
					{	case 1003			: add=0; break;
						case 1004			: add=max; break;
						case 1071			:	{	add++;
															if (add>max)
																add--;
														}	break;
						case 1070			:	{	add--;
															if (add<0)
																add = 0;
														}	break;
						case 1073			:	{	add += FREQLINES+Add_Lines;
															if (add>max)
																add = max;
														}	break;
						case 1072			:	{	add -= FREQLINES+Add_Lines;
															if (add<0)
																add = 0;
														}	break;
						case 1074			:	{	loc = (int) ((long)max*(long)graf_slidebox(tree9,FILEBK+Add_Lines,FILEFD+Add_Lines,0)/1001);
															if (loc<0) loc = 0;
															if (loc>max) loc = max;
															add = loc;
														}
					}

				if (wahl>=1070 && wahl<=1074)
					{	loc = add;
						xredraw_slider(tree9,handle,FILEFULL+Add_Lines,
																				FILEBK+Add_Lines,
																				FILEFD+Add_Lines,max,loc+1,
																				FREQLINES+Add_Lines,TRUE);
						zeige_namen(add,max);
						redraw_one_objc(tree9,FBOX,handle);
					}

			}
		while (wahl!=FOK+Add_Lines);

		OB_FLAGS(tree9,REQINFO+Add_Lines) &= ~HIDETREE;
		for (i=FNAME1; i<=FNAME1+FREQLINES+Add_Lines-1; i++)
			OB_FLAGS(tree9,i) |= (SELECTABLE|TOUCHEXIT);
		xform_maindialog(XFMD_DIALOG);
	}

void zeige_namen(int add,int max)
	{	int i;
		for (i=1; i<=FREQLINES+Add_Lines; i++)
			{	if (i+add<=max)
					put_string(tree9,FNAME1+i-1,registered[add+i-1],FREQLEN);
				else
					put_string(tree9,FNAME1+i-1,"",0);
			}
	}

void notebook(void)
	{	register int t;
		int wahl,nx,ny,nw,nh;
		char dum[MAX_STR];
		FILE *fbbs;
		set_defaultdir();
		if (exist("AVALON\\AVALON.TNB")==0)
			{	fbbs=fopen("AVALON\\AVALON.TNB","r");
				t = NOTE1;
				while ( !feof(fbbs) && (t<=NOTEC) )
					{	input(dum,62,fbbs);
						put_text(tree18,t++,dum,60);
					}
				fclose(fbbs);
			}
		else
			{	for (t=NOTE1; t<=NOTEC; t++)
					*tree18[t].ob_spec.tedinfo->te_ptext = EOS;
			}
		form_center(tree18,&nx,&ny,&nw,&nh);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		objc_draw(tree18,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		do
			{	wahl=form_do(tree18,0) & 0x7FFF;
				undo_state(tree18,wahl,SELECTED);
				switch(wahl)
					{	case NOTELOAD	:	{	if (exist("AVALON\\AVALON.TNB")==0)
																{	fbbs=fopen("AVALON\\AVALON.TNB","r");
																	t = NOTE1;
																	while ( !feof(fbbs) && (t<=NOTEC) )
																		{	input(dum,62,fbbs);
																			put_text(tree18,t++,dum,60);
																		}
																	fclose(fbbs);
																}
															redraw_one_objc(tree18,NOTEBOX,-1);
															break;
														}
						case NOTEFF		:	{	if ( Cprnos() )
																fprintf(stdprn,"\f");
															else
																form_alert(1,msg[20]);
															break;
														}
						case NOTEPRN	:	{	if ( Cprnos() )
																{	for (t=NOTE1; t<=NOTEC; t++)
																		fprintf(stdprn,"%s\n", get_text(tree18,t) );
																}
															else
																{	form_alert(1,msg[20]);
																}
															break;
														}
						case NOTECLR	:	{	for (t=NOTE1; t<=NOTEC; t++)
																*tree18[t].ob_spec.tedinfo->te_ptext = EOS;
															redraw_one_objc(tree18,NOTEBOX,-1);
														}
					}
				redraw_one_objc(tree18,wahl,-1);
			}
		while ( (wahl!=NOTEEXIT) && (wahl!=NOTECANC) );
		if (wahl==NOTEEXIT)
			{	fbbs=fopen("AVALON\\AVALON.TNB","w");
				for (t=NOTE1; t<=NOTEC; t++)
					output(get_text(tree18,t),fbbs);
				fclose(fbbs);
			}
		form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
	}

void teste_notebook(void)
	{	set_defaultdir();
		if (exist("AVALON\\AVALON.TNB")==0)
			{	if (xFsfirst("AVALON\\AVALON.TNB",-1)==0)
					{	if (dta_buffer.d_length<=24)
							OB_FLAGS(tree1,NOTEBOOK) |= HIDETREE;
						else
							OB_FLAGS(tree1,NOTEBOOK) &= ~HIDETREE;
						if (!(divflags & D_NOAVALONBOX))
							redraw_objc(tree1,NOTEBOOK,handle);
					}
			}
	}

/* =============================================================== */
/* =POINT-SETUP=================================================== */
/* =============================================================== */

void point_setup(int cwahl)
	{	int tt;
		set_defaultdir();
		tt=copy_binkley(cwahl);		/* FALSE = KEIN FEHLER */
		if (tt==0)
			select_pointsetup(cwahl);
	}

int copy_binkley(int xwahl)
	{	int hin,hout;
		int tt=0;
		char *copy_buffer,temp[PATH_MAX];

		copy_buffer = (char *) malloc(BUFFER_SIZE);
		if (copy_buffer==NULL)
			{	memory_error();
			}
		else
			{	*temp=EOS;
				switch(xwahl)
					{	case POINT0	: tt=1;	break;
						case POINT1	: tt=2;	break;
						case POINT2	: tt=3;	break;
						case POINT3	: tt=4;	break;
						case POINT4	: tt=5;	break;
						case POINT5	: tt=6;	break;
					}
				sprintf(temp,"AVALON\\BINKLEY.%u",tt-1);
				if (exist(temp)==0)
					{	hin = (int) Fopen(temp,FO_READ);
						hout = (int) Fcreate(binkley,0);
						filecopy(hin,hout,1,copy_buffer);
						Fclose(hout);
						Fclose(hin);
						tt = 0;		/* KEIN FEHLER */
					}
				else
					{	sprintf(temp,tmsg[5],tt-1);
						form_alert(1,temp);
					}
				free(copy_buffer);
			}
		return(tt);
	}

void select_pointsetup(int xwahl)
	{
		menu_icheck(tree12,MPOINT1,0);
		menu_icheck(tree12,MPOINT2,0);
		menu_icheck(tree12,MPOINT3,0);
		menu_icheck(tree12,MPOINT4,0);
		menu_icheck(tree12,MPOINT5,0);
		menu_icheck(tree12,MPOINT6,0);
		switch(xwahl)
			{	case POINT0	:	menu_icheck(tree12,MPOINT1,1); break;
				case POINT1	:	menu_icheck(tree12,MPOINT2,1); break;
				case POINT2	:	menu_icheck(tree12,MPOINT3,1); break;
				case POINT3	:	menu_icheck(tree12,MPOINT4,1); break;
				case POINT4	:	menu_icheck(tree12,MPOINT5,1); break;
				case POINT5	:	menu_icheck(tree12,MPOINT6,1); break;
			}
		undo_xstate(tree1,CHECKED,POINT0,POINT1,POINT2,
															POINT3,POINT4,POINT5,-1);
		do_state(tree1,xwahl,CHECKED);
	}

/* =============================================================== */
/* =START-PROGRAM================================================= */
/* =============================================================== */

void start(int prg)
	{	char file[PATH_MAX],cmd[MAX_STR];
	
		strcpy(file,strupr(program[prg]));

		if ( *file==EOS )
			{	form_alert(1,msg[10]);
			}
		else
			{	set_defaultdir();
				if (exist(file)!=0)
					{	form_alert(1,msg[11]);
					}
			  else
			  	{	if (prg==16)
			  			{	strcpy(program2[16],areas);
			  				if (divflags & D_USEDBBS)
									if (form_alert(1,msg[37])==2)
										strcpy(program2[16],dareas);
			  			}
			  		cmd[0]=strlen(program2[prg]);
					  cmd[1]=EOS;
					  strcat(&cmd[1],program2[prg]);

 						if (strnicmp(file+strlen(file)-4,".BAT",4)==0)
							start_it(file,cmd,1,prg);
						else
				  		start_it(file,cmd,0,prg);
					}
			}
	}

void start_it(char *xfile, char *xcmd, int mode, int prg)
	{	int drive=0,i,j,window_is_open=FALSE;
		signed long level;
		char path[PATH_MAX],temp[255],
				 xxcmd[MAX_STR],xxfile[PATH_MAX];
	
		strcpy(xxfile,xfile);
		strcpy(xxcmd,xcmd);

		menu_bar(tree12,0);

		if (*(xfile+1)==':')
			drive=toupper(*(xfile))-'A';
		else
			drive=def_drive;
		getpfad(xfile,path);
		
		Dsetdrv(drive);
		Dsetpath(path);

		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
		if (xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
			form_alert(1,msg[23]);

		desktop();
		
		level=0;
		if (mode==0)
			{	if ((i=(int) strlen(xfile))<4)
					i=4;

				if ((((prg==-1) && (divflags & D_REDIRECT)) ||
						(program3[prg] & F_REDIR)) &&
						((strstr(xfile+i-4,".TOS")!=NULL) ||
						 (strstr(xfile+i-4,".TTP")!=NULL)) )
					{	wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
						wind.handle=wind_create(NAME,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
						wind_set(wind.handle,WF_NAME," AVALON-WINDOW ");
						wind_open(wind.handle,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
						wind_cls(&wind);
						wind_update(BEG_UPDATE);
						wind_update(BEG_MCTRL);
						set_redirect(!(divflags & D_NOLINEA));
						window_is_open=TRUE;
					}
				else
					{ mouse(xfile);
					}


				if (Kbshift(-1) & 8)	/* ALT */
					{	j=FALSE;
						if ( (aes_version>=0x400 && getcookie('MiNT',&level)) ||
								 (MagX && MagX->aesvars && MagX->aesvars->version>=0x200) )
							{	j=TRUE;
							}

						if (j && (strstr(xfile+i-4,".APP")!=NULL ||
											strstr(xfile+i-4,".PRG")!=NULL) )
							j=shel_write(1,1,100,xxfile,xxcmd);
						else
							level=Pexec(0,xxfile,xxcmd,NULL);
					}
				else
					level=Pexec(0,xxfile,xxcmd,NULL);


				if (level==-39)	form_alert(1,msg[41]);
				if (level==-33)	form_alert(1,msg[45]);

				if ( (divflags & D_DIRECTWAIT) && (prg!=15))
					{	if ( strnicmp(xfile+i-4,".TOS",4)==0 ||
								 strnicmp(xfile+i-4,".TTP",4)==0 )
							{	printf(tbmsg[6]);
								if ( window_is_open )
									pause();
								else
									while(inkey()==0);
							}
					}
				if (window_is_open)
					{	reset_redirect();
						wind_update(END_MCTRL);
						wind_update(END_UPDATE);
						wind_close(wind.handle);
						wind_delete(wind.handle);
					}
				else
					{	Cconws(CUR_OFF);
					}
			}
		else
			{	desktop();
				i=FALSE;
				if (prg==-1)	{	if (divflags & D_REDIRECT)
													i=TRUE;
											}
				else					{	if (program3[prg] & F_REDIR)
													i=TRUE;
											}
				level=batching(xfile,i);
			}

		Fsetdta(&dta_buffer);
		graf_mouse(ARROW,NULL);
		vshow_mouse(0);

		desktop();

		key=0;
		if (level<0)						/* "No mail imported." entfernt */
			{	sprintf(temp,tmsg[2],level);
				form_alert(1,temp);
			}
		set_defaultdir();
		if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
			form_alert(1,msg[23]);
		xform_maindialog(XFMD_START);
		menu_bar(tree12,1);
	}

void mouse(char *xfile)
	{	int i;
		if ((i=(int) strlen(xfile))<4)
			i=4;
		if ( (strstr(xfile+i-4,".TOS")!=NULL) ||
	       (strstr(xfile+i-4,".TTP")!=NULL) )
			{	vhide_mouse();
				Vsync();
				Clear_home();
			}
		else
			{	desktop();
			}
	}

/* =============================================================== */
/* =TESTE-NOMAIL================================================== */
/* =============================================================== */

void teste_nomail(void)
	{	register int l;
		l = teste("*.PKT");
		if (l==0) l += teste("*.MO?");
		if (l==0) l += teste("*.TU?");
		if (l==0) l += teste("*.WE?");
		if (l==0) l += teste("*.TH?");
		if (l==0) l += teste("*.FR?");
		if (l==0) l += teste("*.SA?");
		if (l==0) l += teste("*.SU?");
		if (l==0)
			{	graf_mouse(ARROW,NULL);
				if (form_alert(2,msg[12])==2)
					key=27;
			}
	}

int teste(char *ext)
	{	char temp[PATH_MAX];
		register int ret=0;
		sprintf(temp,"%s%s",netfile,ext);
		if (exist(temp)==0)	ret++;
		if (*knowninbound!=EOS)
			{	sprintf(temp,"%s%s",knowninbound,ext);
				if (exist(temp)==0)	ret++;
			}
		if (*protinbound!=EOS)
			{	sprintf(temp,"%s%s",protinbound,ext);
				if (exist(temp)==0)	ret++;
			}
		return(ret);
	}

/* =============================================================== */
/* =FILESELECT==================================================== */
/* =============================================================== */

void file_select(char *ext)
	{	char in_path[PATH_MAX], sel_file[13]="", cmd[255];
		int button,wahl,nx,ny,nw,nh;
		if (stricmp(ext,"*.BAT")==0)
			button = fileselect(in_path, sel_file, ext, avaltxt[10]);
		else
			button = fileselect(in_path, sel_file, ext, avaltxt[11]);
		if (button!=0)
			{	strcat(in_path, sel_file);
				if (exist(in_path)==0)
					{	if ( stricmp(ext,"*.TTP")==0 )
							{	put_text(tree5,XCMD,"",0);
								form_center(tree5,&nx,&ny,&nw,&nh);
								form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
								form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
								objc_draw(tree5,ROOT,MAX_DEPTH,nx,ny,nw,nh);
								wahl = form_do(tree5,0) & 0x7FFF;
								undo_state(tree5,wahl,SELECTED);
								form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
								form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);

						  	cmd[0]= strlen(get_text(tree5,XCMD));
						  	cmd[1]= EOS;
						  	strcat(&cmd[1], get_text(tree5,XCMD) );
							}
						else
							{	cmd[0]=
								cmd[1]=EOS;
							}
						if (stricmp(ext,"*.BAT")==0)
							start_it(in_path,cmd,1,-1);
						else
							start_it(in_path,cmd,0,-1);
					}
				else
					{	file_not_found();
					}
			}
	}

/* =============================================================== */
/* =AUTOPOLL====================================================== */
/* =============================================================== */

void autopoll(int cwahl)
	{	char temp[MAX_STR],tim1[11],tim2[9];
		int sess,wahl,wahltemp,nx,ny,nw,nh;
		
		switch(cwahl)
			{	case AUTO1	:	sess=0;	cwahl=START1;	break;
				case AUTO2	:	sess=1;	cwahl=START2;	break;
				case AUTO3	:	sess=2;	cwahl=START3;	break;
				case AUTO4	:	sess=3;	cwahl=START4;	break;
				case AUTO5	:	sess=4;	cwahl=START5;	break;
				case AUTO6	:	sess=5;	cwahl=START6;
			}
		if ( i_session[sess]<=0 )
			{	sprintf(temp,tmsg[1],sess+1);
				form_alert(1,temp);
			}
		else
			{	put_text(tree6,XPOLLTIM,"",0);
				form_center(tree6,&nx,&ny,&nw,&nh);
				form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
				form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
				objc_draw(tree6,ROOT,MAX_DEPTH,nx,ny,nw,nh);
				wahl=form_do(tree6,0) & 0x7FFF;
				undo_state(tree6,wahl,SELECTED);
				form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
				form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);

				if ( wahl==XPOLLOK )
					{	strcpy(tim1,get_text(tree6,XPOLLTIM));
						strcat(tim1,"0000");
						tim1[5]=EOS;

						sprintf(temp,"%u%s",sess+1,tim1);
						put_text(tree61,APOLLTIM,temp,5);
						gettime( &ZEIT );
						sprintf(tim2,"%02d%02d%02d",ZEIT.ti_hour,ZEIT.ti_min,ZEIT.ti_sec);
						put_text(tree61,ATIME,tim2,6);

						xform_dial(tree61,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

						key=0;
						do
							{	wahltemp=xform_do(handle,tree61,NULL,0,500L,trans_key2,&wnx,&wny,&wnw,&wnh) & 0x5fff;
								if (wahltemp & 0x4000)
									wahltemp=1010;
								if (wahltemp & 0x1000)
									{	gettime( &ZEIT );
										sprintf(tim2,"%02d%02d%02d",ZEIT.ti_hour,ZEIT.ti_min,ZEIT.ti_sec);
										put_text(tree61,ATIME,tim2,6);
										redraw_one_objc(tree61,ATIME,handle);
										wahltemp=0;
									}
								switch(wahltemp)
									{	case	1010	: key=27;	break;
										case	1011	:	key=32;	break;
										default			: key=0;
									}
								if (key==27) break;
								if ( strncmp(tim1,tim2,5)==0 || key==32)
									{	poll(cwahl,TRUE);
										key=26;
									}
							}
						while (key!=26);

						if (key==27)
							{	graf_mouse(ARROW,NULL);
								set_defaultdir();
								xform_maindialog(XFMD_DIALOG);
							}
					}
			}
	}

/* =============================================================== */
/* =Poll selected================================================= */
/* =============================================================== */

void	poll_selected(void)
	{	int cpoll,count=0;
		char temp[MAX_STR];
		for(cpoll=0; cpoll<=MAXSESSION-1; cpoll++)
			if (sflags[cpoll][0] & F_SELECTED)
				count++;
		if (count==0)
			{	form_alert(1,msg[21]);
			}
		else
			{	sprintf(temp,tmsg[9],count);
				if (form_alert(2,temp)==2)
					{	for(cpoll=0; cpoll<=MAXSESSION-1; cpoll++)
							{	if ( sflags[cpoll][0] & F_SELECTED )
									{	switch(cpoll)
											{	case 0 : wahl1=START1; break;
												case 1 : wahl1=START2; break;
												case 2 : wahl1=START3; break;
												case 3 : wahl1=START4; break;
												case 4 : wahl1=START5; break;
												case 5 : wahl1=START6;
											}
										poll(wahl1,TRUE);
									}
							}
					}
			}
	}

/* =============================================================== */
/* =POLL========================================================== */
/* =============================================================== */

void poll(int cwahl,int mode)
	{	int sess,xx,count,i,drive,loop,loopcnt,window_is_open;
		signed long level;
		char temp[255],file[PATH_MAX],path[PATH_MAX],cmd[PATH_MAX],
				 f[10],xxfile[PATH_MAX],xxcmd[MAX_STR];
		char *pointer;

		switch (cwahl)
			{	case START1 : sess=0; break;
				case START2 : sess=1; break;
				case START3 : sess=2; break;
				case START4 : sess=3; break;
				case START5 : sess=4; break;
				case START6 : sess=5;
			}

		loop = TRUE;
		if (i_session[sess]>0)
			{	if (mode==TRUE)
					xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				for (count=1; count<=i_session[sess]; count++)
				strcpy(file, s_session[sess][count][0]);
				if (strncmp(file,"###",3)==0)
					{	xx = atoi(file+3);
						strcpy(file, program[xx]);
					}
				if (*file==EOS)
					loop = FALSE;
			}

		if (i_session[sess]<=0 || loop==FALSE)
			{	if (loop==TRUE)
					{	sprintf(temp,tmsg[1],sess+1);
						form_alert(1,temp);
					}
				else
					{	sprintf(temp,tmsg[12],sess+1);
						form_alert(1,temp);
					}
			}
		else
			{	if (mode==TRUE)
					{	xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
						if (xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
							form_alert(1,msg[23]);
					}

				desktop();

				set_defaultdir();
				if (mailer==BINKLEY)
					{	teste_setupflags(F_MAIN,POINT0,sess);
						teste_setupflags(F_SET1,POINT1,sess);
						teste_setupflags(F_SET2,POINT2,sess);
						teste_setupflags(F_SET3,POINT3,sess);
						teste_setupflags(F_SET4,POINT4,sess);
						teste_setupflags(F_SET5,POINT5,sess);
					}
				key=0;

				if (sflags[sess][0] & F_LOOP)
					loop=sflags[sess][1];
				else
					loop=1;
				if (loop==0) loop=1;

				for (loopcnt=1; (loopcnt<=loop) && (key!=27); loopcnt++)
					{	for (count=1; (count<=i_session[sess]) && (key!=27); count++)
							{	strcpy(file, s_session[sess][count][0]);
								if (strncmp(file,"###",3)==0)
									{	xx = atoi(file+3);
										strcpy(file, program[xx]);
/* Wenn in Find-1/2 "Redirect" selektiert, dann dafr sorgen, daž
	 hier auch umgeleitet wird.
*/
										if (program3[xx] & F_REDIR)
											w_session[sess][count][0] |= F_REDIR;
										else
											w_session[sess][count][0] &= ~F_REDIR;
									}
								strcpy(temp, s_session[sess][count][1]);
								if (strncmp(temp,"###",3)==0)
									{	xx = atoi(temp+3);
										strcpy(temp, program2[xx]);
									}
						  	cmd[0]= strlen(temp);
						  	cmd[1]= EOS;
								strcat(&cmd[1], temp);
								
								if ( *(file+1)==':' )
									drive=toupper(*(file))-'A';
								else
									drive=def_drive;
		
								pointer=strrchr(file,'\\');
								if (pointer==NULL)
									{	strncpy(f,file,7);
										f[8] = EOS;
										strcpy(path,def_path);
									}
								else
									{	*pointer=EOS;
										strcpy(path,file);
										*pointer='\\';
										for ( i=1; i<=7; *(f+i-1) = *(pointer+i), i++ );
										*(f+i-1)=EOS;
									}
		
								Dsetdrv(drive);
								Dsetpath(path);
								level=0;
								window_is_open=FALSE;

								if (w_session[sess][count][0] & F_BATCH ||
								 		strnicmp(file+strlen(file)-4,".BAT",4)==0 )
									{	desktop();
										if (w_session[sess][count][0] & F_REDIR)
											level=batching(file,TRUE);
										else
											level=batching(file,FALSE);
									}
								else
									{	if ((i=(int) strlen(file))<4)
											i=4;

										strcpy(xxfile,file);
										strcpy(xxcmd,cmd);
										if ((w_session[sess][count][0] & F_REDIR) &&
												((strstr(file+i-4,".TOS")!=NULL) ||
												 (strstr(file+i-4,".TTP")!=NULL)) )
											{	wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
												wind.handle=wind_create(NAME,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
												wind_set(wind.handle,WF_NAME," AVALON-POLLSESSION ");
												wind_open(wind.handle,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);
												wind_cls(&wind);
												wind_update(BEG_UPDATE);
												wind_update(BEG_MCTRL);
												set_redirect(!(divflags & D_NOLINEA));
												window_is_open=TRUE;
											}
										else
											{ mouse(file);
											}

										level=Pexec(0,xxfile,xxcmd,NULL);

										if (level==-39)	form_alert(1,msg[41]);
										if (level==-33)	form_alert(1,msg[45]);
										if (window_is_open)
											reset_redirect();
										else
											Cconws(CUR_OFF);
										/* Hier wurde das Window frher geschlossen */
									}
								Fsetdta(&dta_buffer);
								graf_mouse(ARROW,NULL);
								vshow_mouse(0);
		
								key=0;
								if ( ((int)Kbshift(-1) & 8) != 0 )
									key=27;
								if ( level<0 )
									{	sprintf(temp,tmsg[2],level);
										if (form_alert(1,temp)==2) key=27;
									}
								else
									{	if ( sflags[sess][0] & F_NOMAIL )
											{	if ( !strnicmp(f,"BINKLEY",7) || 
														 !strnicmp(f,"BT",2) ||
														 !strnicmp(f,"SEMPER",6) )
													teste_nomail();
											}
									}
								if (key==0)
									{	if ( (sflags[sess][0] & F_WAITALL) || 
												 (w_session[sess][count][0] & F_PRGWAIT) )
											{	wait();
												key=key2;
											}
									}

								if (window_is_open)
									{	window_is_open=FALSE;
										wind_update(END_MCTRL);
										wind_update(END_UPDATE);
										wind_close(wind.handle);
										wind_delete(wind.handle);
									}

								if (sflags[sess][0] & F_LOOP)
									if ((level>0) && (level==w_session[sess][count][1]))
										key=27;
							} /* POLL-SCHLEIFE */
					} /* LOOP-SCHLEIFE */

				graf_mouse( ARROW, NULL );
				set_defaultdir();

				desktop();

				if (mode==TRUE)
					{	if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
							form_alert(1,msg[23]);
						xform_maindialog(XFMD_START);
					}
			}
	}

void teste_setupflags(int xflag, int xpoint, int xsess)
	{	int tt;

		if ( sflags[xsess][0] & xflag )
			{	tt=copy_binkley(xpoint);
				if (tt==0)
					{	select_pointsetup(xpoint);
						lese_cfg();
						info_box();
					}
			}
	}

/* =============================================================== */
/* =FORM-DO-ROUTINEN============================================== */
/* =============================================================== */

int xform_do3(OBJECT *ctree, int xstart)
	{	int cwahl=0,witch,x,y,k,s,key,clicks,
				msgbuf[8] = { 0,0,0,0,0,0,0,0 };
		xstart=xstart;
		witch = evnt_multi(MU_KEYBD | MU_BUTTON,
											 2,1,1,0,0,0,0,0,0,0,0,0,0,msgbuf,0,0,&x,&y,&k,&s,&key,&clicks);

		if (witch & MU_KEYBD)
			{	key2 = ( (key & 0xFF00) >> 8 );
				switch(key2)
					{	case 					1	:	key2=27;				break;	/* Gebraucht in wait() */
						case KEY_A >> 8	:	cwahl=MANMSGS;	break;
						case KEY_C >> 8	:	cwahl=MANCOPY;	break;
						case KEY_D >> 8	:	cwahl=MANDEL;		break;
						case KEY_I >> 8	:	cwahl=MANINFO;	break;
						case KEY_M >> 8	:	cwahl=MANMOVE;	break;
						case KEY_R >> 8	:	cwahl=MANREN;		break;
						case KEY_S >> 8	:	cwahl=MANSHIP;	break;
						case KEY_T >> 8	:	cwahl=MANTERM;	break;
						case KEY_O >> 8	:	cwahl=MANOUT;		break;
						case KEY_V >> 8	:	cwahl=MANVIEW;	break;
						case KEY_Y >> 8	:	cwahl=MANSYS;		break;
						default					: cwahl=PRESSKEY+100;
					}
			}

		if (witch & MU_BUTTON)
			{	cwahl = objc_find(ctree,ROOT,MAX_DEPTH,x,y);
				if (cwahl != -1)
					{	form_button(ctree,cwahl,clicks,&cwahl);
						if (clicks == 2) cwahl |= 0x8000;
					}
			}
		return cwahl;
	}
