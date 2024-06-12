/* MODUL: Programmanager			 */
/*														 */
/* (c) St.Slabihoud 1994			 */

#include <aes.h>
#include <vdi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>
#include <ext.h>
#include <screen.h>
#include <portab.h>
#include <scancode.h>
#include <my_scan.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "avalonio.h"
#include "avalxhdi.h"
#include "avallang.h"
#include "define.h"
#include "version.h"
#include "diverse.h"
#include "form_run.h"
#include "vars.h"

extern void	start(int prg);

void	prog_manager(void);

static void	edit_entry(char *file,char *cmd,char *name,int *flag);
static int	xform_do4(OBJECT *ctree, int xstart);

/* =============================================================== */
/* =PROGRAM-MANAGER=============================================== */
/* =============================================================== */

void prog_manager(void)
	{	int prg,xwahl,wahl,offx,offy,nx,ny,nw,nh;

		for (prg=0; prg<=MAX_EPROG-1; prg++)
			{	if (*eprogram[prg][0]==EOS)
					put_text(tree33,EPROG1+prg,"---",30);
				else
					put_text(tree33,EPROG1+prg,eprogtxt[prg],30);
			}

		objc_offset(tree1,DOPROGS,&offx,&offy);
		offx -= OB_X(tree33,EPROGREF);
		offy -= OB_Y(tree33,EPROGREF);
		OB_X(tree33,ROOT) = offx;
		OB_Y(tree33,ROOT) = offy;
		nx = OB_X(tree33,ROOT) - 3;
		ny = OB_Y(tree33,ROOT) - 3;
		nw = OB_W(tree33,ROOT) + 6;
		nh = OB_H(tree33,ROOT) + 6;
		prg=-1;
		form_dial(FMD_START,nx,ny,nw,nh,nx,ny,nw,nh);
/*	form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh); */
		objc_draw(tree33,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		xwahl = xform_do4(tree33,0);
		wahl = xwahl & 0x7ff;
		if (wahl>=EPROG1 && wahl<=EPROG1+MAX_EPROG-1)
			{	undo_state(tree33,wahl,SELECTED);
				if ( xwahl & 0x8000 )
					{	prg = wahl - EPROG1;
						edit_entry(eprogram[prg][0],eprogram[prg][1],eprogtxt[prg],&eprogram3[prg]);
						prg=-1;
					}
				else
					{	prg = wahl - EPROG1;
					}
			}
/*	form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh); */
		form_dial(FMD_FINISH,nx,ny,nw,nh,nx,ny,nw,nh);
		if (prg>=0 && *eprogram[prg][0]!=EOS)
			{	strcpy(program[0] ,eprogram[prg][0]);
				strcpy(program2[0],eprogram[prg][1]);
				program3[0] = eprogram3[prg];
				start(0);
			}
	}

static void edit_entry(char *file,char *cmd,char *name,int *flag)
	{	int nx,ny,nw,nh,wahltemp;
		char temp[MAX_STR],*pointer;

		form_center(tree34,&nx,&ny,&nw,&nh);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		put_text(tree34,EPFAD,file,65);
		put_text(tree34,ECMD ,cmd ,65);
		put_text(tree34,ENAME,name,30);

		undo_state(tree34,EREDIR,SELECTED);
		if (*flag & F_REDIR) do_state(tree34,EREDIR,SELECTED);

		objc_draw(tree34,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahltemp=0;
		do
			{	wahltemp=form_do(tree34,wahltemp) & 0x8fff;
				if (wahltemp & 0x8000)
					{	do_select("*.*",EPFAD,tree34,65);
						redraw_one_objc(tree34,EPFAD,-1);
						undo_state(tree34,wahltemp & 0x7ff,SELECTED);
						wahltemp=EPROGOK;
					}
				undo_state(tree34,wahltemp,SELECTED);
			}
		while (wahltemp!=EPROGOK);
		strcpy(file,get_text(tree34,EPFAD));
		strcpy(cmd ,get_text(tree34,ECMD ));
		strcpy(name,get_text(tree34,ENAME));
		if (*name==EOS)
			{	pointer=strrchr(file,'\\');
				if (pointer==NULL)
					strncpy(temp,file,30);
				else
					strncpy(temp,pointer+1,30);
				temp[30]=EOS;
				strcpy(name,temp);
			}
		*flag = 0;
		if (is_state(tree34,EREDIR,SELECTED))	*flag |= F_REDIR;

		form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
		changes |= C_EPROGRAMS;
 	}

static int xform_do4(OBJECT *ctree, int xstart)
	{	int cwahl=0,witch,x,y,k,s,key,clicks,
				msgbuf[8] = { 0,0,0,0,0,0,0,0 };
		xstart=xstart;
		witch = evnt_multi(MU_KEYBD|MU_BUTTON,
											 2,1,1,0,0,0,0,0,0,0,0,0,0,msgbuf,0,0,&x,&y,&k,&s,&key,&clicks);

/*	if (witch & MU_KEYBD)
			{	switch( (key & 0xFF00) >> 8 )
					{	case KEY_1 >> 8 : cwahl=EPROG1; break;
					}
			}
*/

		if (witch & MU_BUTTON)
			{	cwahl = objc_find(ctree,ROOT,MAX_DEPTH,x,y);
				if (cwahl != -1)
					{	form_button(ctree,cwahl,clicks,&cwahl);
						if (clicks == 2) cwahl |= 0x8000;
					}
			}
		return cwahl;
	}
