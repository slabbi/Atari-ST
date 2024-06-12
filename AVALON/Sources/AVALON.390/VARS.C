#include <aes.h>
#include <tos.h>
#include <ext.h>
#include <stdio.h>
#include <fido.h>
#include <magx.h>
#include "define.h"

struct date HEUTE;
struct time ZEIT;

MAGX_COOKIE *MagX;

DTA	dta_buffer;

MSG_HEADER header;												/* AVALON,AREAFIX,HATCH,FREQUEST,CONVERT */


char	def_path[PATH_MAX],									/* AVALON,FREQUEST,DIVERSE */
			def_path_slash[PATH_MAX],						/* AVALON,FREQUEST,AVALONIO */
			
			program[17][PATH_MAX],							/* AVALON,BATCHING */
			program2[17][PATH_MAX],							/* AVALON,BATCHING */

			filelist[FREQSETUPS][10][PATH_MAX],	/* AVALON,FREQUEST */
			filetext[FREQSETUPS][10][12],				/* AVALON,FREQUEST */
			filelistnode[FREQSETUPS][10][16],		/* AVALON,FREQUEST */

			getfile[MAX_GETFILE][25],						/* AVALON,FREQUEST */
			
			hold[PATH_MAX],											/* AVALON,FREQUEST,HATCH */
			statuslog[PATH_MAX],								/* AVALON,BATCHING,DIVERSE */
			areas[PATH_MAX], dareas[PATH_MAX],	/* AVALON,AREAFIX,CONVERT */

			txtsysop[58],												/* AVALON,AREAFIX,MANAGER,SYSINFO,HATCH */
			netmail[3][51],											/* AVALON,AREAFIX,HATCH */
			mailname[3][9] =										/* AVALON,HATCH */
										{ "","Report 1","Report 2" },

			address[MAX_ADDRESS][81],						/* AVALON,AREAFIX,FREQUEST,SYSINFO,HATCH */
			domain[MAX_DOMAIN][2][21],					/* AVALON,FREQUEST,SYSINFO,HATCH */
																					/* 0=Name, 1=Ordnername */

			hatchpassword[13],									/* AVALON,HATCH */
			hatcharea[36],											/* AVALON,HATCH */
			hatchnode[15],											/* AVALON,HATCH */
			hatchsysop[26],											/* AVALON,HATCH */
			hatchdefbutton[MAX_HATCH][12],			/* AVALON,HATCH */
			hatchdefnode[MAX_HATCH][15],				/* AVALON,HATCH */
			hatchdefsysop[MAX_HATCH][26],				/* AVALON,HATCH */
			hatchdefpw[MAX_HATCH][13],					/* AVALON,HATCH */
			hatchdefarea[MAX_HATCH][36];				/* AVALON,HATCH */

long	divflags = 													/* ALL */
					D_CHECKING|D_USE4D|D_USEDELAY|D_MAXIWIND;

int		ap_id,															/* AVALON,MANAGER,FORM_RUN,FIDO_MSG */
			handle=-1,													/* ALL */
			has_appl_search=FALSE,
			gemdos_version,
			aes_version,
			vdi_handle,													/* ALL */
			fontpnt=10,													/* ALL */
			def_drive,													/* AVALON,AVALONIO,BATCHING */
			wnx,wny,wnw,wnh,										/* ALL */
			mid_x,mid_y,												/* ALL */
			max_x,max_y,												/* ALL */
			domain_cnt,													/* AVALON,FREQUEST,SYSINFO,HATCH */
			address_cnt,												/* AVALON,SYSINFO */
			pipeflags=0,												/* AVALON,FREQUEST,FIDO_MSG,HATCH */
			changes=0,													/* AVALON,FREQUEST,MANAGER,HATCH */
			hatchflags=0,												/* AVALON,HATCH */
			redraw_infobox=FALSE,								/* AVALON,BATCHING */
			Add_Lines,													/* AVALON,AREAFIX,FREQUEST,TRANSKEY */
			filebutton=0,fileblock=0,						/* AVALON,FREQUEST */
			msg_flag=0,													/* BATCHING,CONVERT */
			mailer=BINKLEY,											/* AVALON,SYSINFO,FIDO_MSG */
			fakenet[MAX_ADDRESS];								/* AVALON,SYSINFO,HATCH */

OBJECT	*tree1, *tree2, *tree3,	*tree5,		/* ALL */
				*tree6, *tree7, *tree8, *tree9,	*tree10,
				*tree11,*tree12,*tree13,*tree14,*tree15,
				*tree16,*tree17,*tree18,*tree19,*tree20,
				*tree21,*tree22,*tree23,*tree24,*tree25,
				*tree26,*tree27,*tree28,*tree29,*tree30,
				*tree31,*tree33,*tree34,*tree40,*tree41,
				*tree51,*tree52,*tree61,*tree71,*tree72,
				*tree81,*tree82,*treepic0,
				*tree90,*tree91,*tree99;


/* SESSION */

char		s_session[MAXSESSION][MAXPRGSESS+1][2][PATH_MAX],
				s_timesess[MAXSESSION][2][9];
int			w_session[MAXSESSION][MAXPRGSESS+1][2],
				i_session[MAXSESSION],
				sflags[MAXSESSION][2],
				program3[17];

char		txtutil[MAX_UTILS][10],
				txtsess[MAXSESSION][10],
				txtname[MAX_SETUP][13]	=	{	"Main Setup"," 1st Setup"," 2nd Setup",
																		" 3rd Setup"," 4th Setup"," 5th Setup" };

/* Extended programs */

char		eprogram[MAX_EPROG][2][66],
				eprogtxt[MAX_EPROG][31];
int			eprogram3[MAX_EPROG];
