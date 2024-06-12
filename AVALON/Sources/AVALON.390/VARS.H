#include <aes.h>
#include <tos.h>
#include <ext.h>
#include <stdio.h>
#include <fido.h>
#include <magx.h>
#include "define.h"

extern struct date HEUTE;
extern struct time ZEIT;

extern MAGX_COOKIE *MagX;

extern DTA dta_buffer;

extern MSG_HEADER header;

extern
char	def_path[PATH_MAX],
			def_path_slash[PATH_MAX],
			
			program[17][PATH_MAX],
			program2[17][PATH_MAX],

			filelist[FREQSETUPS][10][PATH_MAX],		/* Init in INIT_VARS */
			filetext[FREQSETUPS][10][12],
			filelistnode[FREQSETUPS][10][16],

			getfile[MAX_GETFILE][25],
			
			hold[PATH_MAX],
			statuslog[PATH_MAX],
			areas[PATH_MAX], dareas[PATH_MAX],

			txtsysop[58],
			netmail[3][51],
			mailname[3][9],

			address[MAX_ADDRESS][81],
			domain[MAX_DOMAIN][2][21],		/* 0=Name, 1=Ordnername */

			hatchpassword[13],
			hatcharea[36],
			hatchnode[15],
			hatchsysop[26],
			hatchdefbutton[MAX_HATCH][12],
			hatchdefnode[MAX_HATCH][15],		/* Initialisierung in INIT_VARS */
			hatchdefsysop[MAX_HATCH][26],
			hatchdefpw[MAX_HATCH][13],
			hatchdefarea[MAX_HATCH][36];

extern
long	divflags;

extern
int		ap_id,
			handle,
			has_appl_search,
			gemdos_version,
			aes_version,
			vdi_handle,
			fontpnt,
			def_drive,
			wnx,wny,wnw,wnh,
			mid_x,mid_y,
			max_x,max_y,
			domain_cnt,address_cnt,
			pipeflags,changes,hatchflags,
			redraw_infobox,
			Add_Lines,
			filebutton,fileblock,
			msg_flag,
			mailer,
			fakenet[MAX_ADDRESS];

extern
OBJECT	*tree1, *tree2, *tree3,	*tree5,
				*tree6, *tree7, *tree8, *tree9,	*tree10,
				*tree11,*tree12,*tree13,*tree14,*tree15,
				*tree16,*tree17,*tree18,*tree19,*tree20,
				*tree21,*tree22,*tree23,*tree24,*tree25,
				*tree26,*tree27,*tree28,*tree29,*tree30,
				*tree31,*tree33,*tree34,*tree40,*tree41,*tree51,*tree52,*tree61,
				*tree71,*tree72,*tree81,*tree82,*treepic0,
				*tree90,*tree91,*tree99;

extern
char		s_session[MAXSESSION][MAXPRGSESS+1][2][PATH_MAX],
				s_timesess[MAXSESSION][2][9];
extern
int			w_session[MAXSESSION][MAXPRGSESS+1][2],
				i_session[MAXSESSION],
				sflags[MAXSESSION][2],
				program3[17];
extern
char		txtutil[MAX_UTILS][10],
				txtsess[MAXSESSION][10],
				txtname[MAX_SETUP][13];
extern
char		eprogram[MAX_EPROG][2][66],
				eprogtxt[MAX_EPROG][31];
extern
int			eprogram3[MAX_EPROG];
