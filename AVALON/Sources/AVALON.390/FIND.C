#include <aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <portab.h>
#include "diverse.h"
#include "form_run.h"
#include "vars.h"
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
	#include "avalsemp.rh"
#else
	#include "avalon.h"
	#include "avalon.rh"
#endif

void	setup2(void);
int		setup3(void);

static int setup3_utilname(char *txt, int index);

/* =============================================================== */
/* =FIND-PROGRAMS================================================= */
/* =============================================================== */

void setup2(void)
	{	int xwahl,wahl=0;
		undo_xstate(tree3,SELECTED,CEXPORT,CIMPORT,CCRUNCH,CSCANNER,
															 CMSGED,CTXTED,CMAILER,CTERM,-1);
		
		if (program3[1] & F_REDIR) do_state(tree3,CEXPORT,SELECTED);
		if (program3[2] & F_REDIR) do_state(tree3,CIMPORT,SELECTED);
		if (program3[3] & F_REDIR) do_state(tree3,CCRUNCH,SELECTED);
		if (program3[4] & F_REDIR) do_state(tree3,CSCANNER,SELECTED);
		if (program3[5] & F_REDIR) do_state(tree3,CMSGED,SELECTED);
		if (program3[6] & F_REDIR) do_state(tree3,CTXTED,SELECTED);
		if (program3[7] & F_REDIR) do_state(tree3,CMAILER,SELECTED);
		if (program3[8] & F_REDIR) do_state(tree3,CTERM,SELECTED);
		put_text(tree3,AEXPORT,program[1],55);
		put_text(tree3,AIMPORT,program[2],55);
		put_text(tree3,ACRUNCH,program[3],55);
		put_text(tree3,ASCANNER,program[4],55);
		put_text(tree3,AMSGED,program[5],55);
		put_text(tree3,ATXTED,program[6],55);
		put_text(tree3,AMAILER,program[7],55);
		put_text(tree3,ATERM,program[8],55);
		put_text(tree3,AVIEW,program[15],55);
		put_text(tree3,ABBSEDIT,program[16],55);
		put_text(tree3,BEXPORT,program2[1],55);
		put_text(tree3,BIMPORT,program2[2],55);
		put_text(tree3,BCRUNCH,program2[3],55);
		put_text(tree3,BSCANNER,program2[4],55);
		put_text(tree3,BMSGED,program2[5],55);
		put_text(tree3,BTXTED,program2[6],55);
		put_text(tree3,BMAILER,program2[7],55);
		put_text(tree3,BTERM,program2[8],55);
		xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	xwahl=xform_do(handle,tree3,NULL,wahl,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
				if (xwahl & 0x4000)
					xwahl = ACANCEL;
				wahl = xwahl & 0x7ff;
				undo_state(tree3,wahl,1);
				if (xwahl & 0x8000)
					{	switch(wahl)
							{	case AEXPORT	: do_select("EXPORT.TTP",AEXPORT,tree3,55);		break;
								case AIMPORT	: do_select("IMPORT.TTP",AIMPORT,tree3,55);		break;
								case ACRUNCH	: do_select("CRUNCH.TTP",ACRUNCH,tree3,55);		break;
								case ASCANNER	: do_select("SCANNER.TTP",ASCANNER,tree3,55);	break;
								case AMSGED		: do_select("LED.PRG",AMSGED,tree3,55);				break;
								case ATXTED		: do_select("TEMPUS.PRG",ATXTED,tree3,55);		break;
								case AMAILER	: do_select("BINKLEY.TTP",AMAILER,tree3,55);	break;
								case ATERM		: do_select("BINKLEY.TTP",ATERM,tree3,55);		break;
								case AVIEW		: do_select("GUCK.TTP",AVIEW,tree3,55);				break;
								case ABBSEDIT	: do_select("AREAMAST.PRG",ABBSEDIT,tree3,55);
							}
						redraw_one_objc(tree3,wahl,handle);
					}
			}
		while ( (wahl!=AOK) && (wahl!=ACANCEL) );
		redraw_objc(tree3,wahl,handle);
		if (wahl==AOK)
			{	strcpy(program[1],get_text(tree3,AEXPORT));
				strcpy(program[2],get_text(tree3,AIMPORT));
				strcpy(program[3],get_text(tree3,ACRUNCH));
				strcpy(program[4],get_text(tree3,ASCANNER));
				strcpy(program[5],get_text(tree3,AMSGED));
				strcpy(program[6],get_text(tree3,ATXTED));
				strcpy(program[7],get_text(tree3,AMAILER));
				strcpy(program[8],get_text(tree3,ATERM));
				strcpy(program[15],get_text(tree3,AVIEW));
				strcpy(program[16],get_text(tree3,ABBSEDIT));

				strcpy(program2[1],get_text(tree3,BEXPORT));
				strcpy(program2[2],get_text(tree3,BIMPORT));
				strcpy(program2[3],get_text(tree3,BCRUNCH));
				strcpy(program2[4],get_text(tree3,BSCANNER));
				strcpy(program2[5],get_text(tree3,BMSGED));
				strcpy(program2[6],get_text(tree3,BTXTED));
				strcpy(program2[7],get_text(tree3,BMAILER));
				strcpy(program2[8],get_text(tree3,BTERM));

				for (wahl=1; wahl<=8; wahl++)
					program3[wahl] &= ~F_REDIR;
				if (is_state(tree3,CEXPORT,SELECTED))		program3[1] |= F_REDIR;
				if (is_state(tree3,CIMPORT,SELECTED))		program3[2] |= F_REDIR;
				if (is_state(tree3,CCRUNCH,SELECTED))		program3[3] |= F_REDIR;
				if (is_state(tree3,CSCANNER,SELECTED))	program3[4] |= F_REDIR;
				if (is_state(tree3,CMSGED,SELECTED))		program3[5] |= F_REDIR;
				if (is_state(tree3,CTXTED,SELECTED))		program3[6] |= F_REDIR;
				if (is_state(tree3,CMAILER,SELECTED))		program3[7] |= F_REDIR;
				if (is_state(tree3,CTERM,SELECTED))			program3[8] |= F_REDIR;
				changes |= C_PROGRAMS;
			}
		xform_maindialog(XFMD_DIALOG);
	}

int setup3(void)
	{	int xwahl,wahl=0,retflag=FALSE;
		undo_xstate(tree31,SELECTED,CUTIL1,CUTIL2,CUTIL3,
																CUTIL4,CUTIL5,CUTIL6,-1);
		if (program3[9]  & F_REDIR) do_state(tree31,CUTIL1,SELECTED);
		if (program3[10] & F_REDIR) do_state(tree31,CUTIL2,SELECTED);
		if (program3[11] & F_REDIR) do_state(tree31,CUTIL3,SELECTED);
		if (program3[12] & F_REDIR) do_state(tree31,CUTIL4,SELECTED);
		if (program3[13] & F_REDIR) do_state(tree31,CUTIL5,SELECTED);
		if (program3[14] & F_REDIR) do_state(tree31,CUTIL6,SELECTED);
		put_text(tree31,AUTIL1,program[9],55);
		put_text(tree31,AUTIL2,program[10],55);
		put_text(tree31,AUTIL3,program[11],55);
		put_text(tree31,AUTIL4,program[12],55);
		put_text(tree31,AUTIL5,program[13],55);
		put_text(tree31,AUTIL6,program[14],55);
		put_text(tree31,BUTIL1,program2[9],55);
		put_text(tree31,BUTIL2,program2[10],55);
		put_text(tree31,BUTIL3,program2[11],55);
		put_text(tree31,BUTIL4,program2[12],55);
		put_text(tree31,BUTIL5,program2[13],55);
		put_text(tree31,BUTIL6,program2[14],55);
		put_text(tree31,DUTIL1,txtutil[0],9);
		put_text(tree31,DUTIL2,txtutil[1],9);
		put_text(tree31,DUTIL3,txtutil[2],9);
		put_text(tree31,DUTIL4,txtutil[3],9);
		put_text(tree31,DUTIL5,txtutil[4],9);
		put_text(tree31,DUTIL6,txtutil[5],9);
		xform_dial(tree31,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	xwahl=xform_do(handle,tree31,NULL,wahl,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
				if (xwahl & 0x4000)
					xwahl = BCANCEL;
				wahl = xwahl & 0x7ff;
				undo_state(tree31,wahl,1);

				if (xwahl & 0x8000)
					{	switch(wahl)
							{	case AUTIL1		: do_select("",AUTIL1,tree31,55); break;
								case AUTIL2		: do_select("",AUTIL2,tree31,55); break;
								case AUTIL3		: do_select("",AUTIL3,tree31,55); break;
								case AUTIL4		: do_select("",AUTIL4,tree31,55); break;
								case AUTIL5		: do_select("",AUTIL5,tree31,55); break;
								case AUTIL6		: do_select("",AUTIL6,tree31,55);
							}
						redraw_one_objc(tree31,wahl,handle);
					}
			}
		while ( (wahl!=BOK) && (wahl!=BCANCEL) );
		redraw_objc(tree31,wahl,handle);
		if (wahl==BOK)
			{	strcpy(program[9],get_text(tree31,AUTIL1));
				strcpy(program[10],get_text(tree31,AUTIL2));
				strcpy(program[11],get_text(tree31,AUTIL3));
				strcpy(program[12],get_text(tree31,AUTIL4));
				strcpy(program[13],get_text(tree31,AUTIL5));
				strcpy(program[14],get_text(tree31,AUTIL6));
				strcpy(program2[9],get_text(tree31,BUTIL1));
				strcpy(program2[10],get_text(tree31,BUTIL2));
				strcpy(program2[11],get_text(tree31,BUTIL3));
				strcpy(program2[12],get_text(tree31,BUTIL4));
				strcpy(program2[13],get_text(tree31,BUTIL5));
				strcpy(program2[14],get_text(tree31,BUTIL6));
				for (wahl=9; wahl<=14; wahl++)
					program3[wahl] &= ~F_REDIR;
				if (is_state(tree31,CUTIL1,SELECTED))	program3[9]  |= F_REDIR;
				if (is_state(tree31,CUTIL2,SELECTED))	program3[10] |= F_REDIR;
				if (is_state(tree31,CUTIL3,SELECTED))	program3[11] |= F_REDIR;
				if (is_state(tree31,CUTIL4,SELECTED))	program3[12] |= F_REDIR;
				if (is_state(tree31,CUTIL5,SELECTED))	program3[13] |= F_REDIR;
				if (is_state(tree31,CUTIL6,SELECTED))	program3[14] |= F_REDIR;
				retflag |= setup3_utilname(txtutil[0],DUTIL1);
				retflag |= setup3_utilname(txtutil[1],DUTIL2);
				retflag |= setup3_utilname(txtutil[2],DUTIL3);
				retflag |= setup3_utilname(txtutil[3],DUTIL4);
				retflag |= setup3_utilname(txtutil[4],DUTIL5);
				retflag |= setup3_utilname(txtutil[5],DUTIL6);
				changes |= C_UTILITIES;
			}
		xform_maindialog(XFMD_DIALOG);
		return(retflag);
	}

static int setup3_utilname(char *txt, int index)
	{	char temp[15];
		strcpy(temp,get_text(tree31,index));
		if (strcmp(temp,txt)!=0)
			{	strcpy(txt,temp);
				return(TRUE);
			}
		else
			return(FALSE);
	}

