#include <ext.h>
#include <tos.h>
#include <aes.h>
#include <vdi.h>
#include <string.h>
#include <scancode.h>
#include <my_scan.h>
#include <stddef.h>
#include "windmini.h"
#include "linea.h"

#define SCREENBUFFER	40
#define	D_USEBUFFER		(1LU << 20)		/* Auch in DEFINE.H definiert */
#define CON			0
#define RAWCON	1

static char	buf[SCREENBUFFER+1];
static int	chrcnt=0,buffertype=-1;

extern long	divflags;

WIND wind;

static int  up_rc_intersect(int *x1,int *y1,int *w1,int *h1,int *x2,int *y2,int *w2,int *h2);
static void up_wind_scrollup(WIND *wind,int y);
static void up_wind_scrolldown(WIND *wind,int y);
static void up_wind_delline(WIND *wind,int y);
static void up_wind_putch(WIND *wind,int c);
static void up_wind_cls(WIND *wind);
static void ext_v_gtext(WIND *wind,int x,int y,char *txt);
static void up_wind_redraw(WIND *wind,int x,int y,int w,int h);
static void cdecl newcon(int dev,int chr);
static void cdecl newrawcon(int dev,int chr);
static long set_it(void);
static long reset_it(void);
static void up_ansi_attr(WIND *wind,int x);
static void up_cls_down(WIND *wind);
static void up_cls_up(WIND *wind);
static void up_cls_eol(WIND *wind);
static void up_cls_sol(WIND *wind);
static void up_wind_vt52(WIND *wind,int c);
static void up_wind_vt100(WIND *wind,int c);
static void up_wind_vt100_par(WIND *wind,int c);
static void	up_wind_vt100_esc(WIND *wind,int c);
static void up_wind_vt100_quest(WIND *wind,int c);

static function old_con			= NULL,
								old_rawcon	= NULL;
static int save_cur[2] = { 0,0 };
#define MAX_ESC 40
static int esclist[MAX_ESC+1],esccount=0;
static int linea = FALSE;
static int num,cnum;
static char outchar[]=" ";

void wind_init_full(WIND *wind,int vdi_handle,int kind,int fontpnt,int center,int snap,int clear,int nx,int ny,int nw,int nh)
	{	int xx,yy,ww,hh,x,y,w,h;
		wind_get(0,WF_WORKXYWH,&xx,&yy,&ww,&hh);
		wind_init(wind,vdi_handle,kind,fontpnt,snap,TRUE,clear,0,0,WIND_MAX_X,WIND_MAX_Y);

		if (ww<wind->b[2])
			{	if (nx+nw <= xx+ww)						/* Window mit 80x25 pažt nicht.	*/
					x=0;												/* Nicht zentrieren. 						*/
/*				x=nx; */
				else
					x=(ww-nw)/2+xx;							/* Zentrieren !!!								*/
				w=wind->b[2];
				if (w>ww) w=ww;
			}
		else
			{	if (nx+wind->b[2] <= xx+ww)		/* Window 80x25 pažt.						*/
					x=nx;												/* Gewnschte Koordinate.				*/
				else
					x=(ww-wind->b[2])/2+xx;			/* Es muž zentriert werden.			*/
				w=wind->b[2];

				if (center)
					x=(ww-wind->b[2])/2+xx;			/* Es muž zentriert werden.			*/
			}

		if (hh<wind->b[3])
			{	if (ny+nh <= yy+hh)
					y=yy;
/*				y=ny; */
				else
					y=(hh-nh)/2+yy;
				h=wind->b[3];
				if (h>hh) h=hh;
			}
		else
			{	if (ny+wind->b[3] <= yy+hh)
					y=ny;
				else
					y=(hh-wind->b[3])/2+yy;
				h=wind->b[3];

				if (center)
					y=(hh-wind->b[3])/2+yy;
			}

		if (y<yy) y=yy;				/* Reicht das Window in die Menleiste? */

		wind_init(wind,wind->vdi_handle,kind,fontpnt,snap,FALSE,clear,x,y,w,h);
	}

void wind_init(WIND *wind,int vdi_handle,int kind,int fontpnt,int snap,int mode,int clear,int x,int y,int w,int h)
	{	int i,xx,yy,ww,hh;

		wind_get(0,WF_WORKXYWH,&xx,&yy,&ww,&hh);

		wind->vdi_handle = vdi_handle;
		wind->fontpnt = fontpnt;
		wind->kind = kind;
/* Schreibmodus und Fontgr”že setzen */

		vswr_mode(wind->vdi_handle,MD_REPLACE);
		vsf_perimeter(wind->vdi_handle,FALSE);
		vsf_style(wind->vdi_handle,0);
		vsf_color(wind->vdi_handle,0);
		vst_color(wind->vdi_handle,1);
		vst_effects(wind->vdi_handle,0);

		wind->mode = MD_REPLACE;
		wind->effects =
		wind->escape =
		wind->savex =
		wind->savey = 0;
		wind->wrap =
		wind->cursor = FALSE;
		vst_font(wind->vdi_handle,1);
		vst_alignment(wind->vdi_handle,0,5,&i,&i);
		vst_point(wind->vdi_handle,wind->fontpnt,&wind->wchar,&wind->hchar,&wind->wbox,&wind->hbox);
/* Windowgr”že setzen */
		wind->b[0] = x;	/* Wind.Border */
		wind->b[1] = y;
		if (mode==FALSE)
			{	wind->b[2] = w; wind->b[3] = h;
			}
		else
			{	wind->b[2] = w*wind->wbox; wind->b[3] = h*wind->hbox;

				if (wind->b[3]>hh) wind->b[3]=hh;			/* Window zu grož? */
				if (wind->b[2]>ww) wind->b[2]=ww;			/* Window zu grož? */

				wind_calc(WC_BORDER,kind,0,0,w*wind->wbox,h*wind->hbox,
																										 &i,&i,&wind->b[2],&wind->b[3]);
			}

		wind_calc(WC_WORK,kind,wind->b[0],wind->b[1],wind->b[2],wind->b[3],
													 &wind->w[0],&wind->w[1],&wind->w[2],&wind->w[3]);
		if (snap)
			{	if ((i=wind->w[0] & 7) > 0)
					{	wind->b[0] -= i;
						wind->w[0] -= i;
					}
			}

/* Window an Fontgr”že anpassen und BORDER neu ermitteln */
		wind->max_x = (wind->w[2]-wind->wbox)/wind->wbox;
		wind->max_y = (wind->w[3]-wind->hbox)/wind->hbox;

		wind->txy[0] = wind->pxy[0] = wind->w[0];	/* Wind.Work als pxy */
		wind->txy[1] = wind->pxy[1] = wind->w[1];
		wind->pxy[2] = wind->w[2]+wind->w[0]-1;
		wind->pxy[3] = wind->w[3]+wind->w[1]-1;
		wind->txy[2] = (wind->w[2]/wind->wbox)*wind->wbox+wind->w[0]-1;
		wind->txy[3] = (wind->w[3]/wind->hbox)*wind->hbox+wind->w[1]-1;
		if (clear)
			{	wind->x = wind->y = 0;
				for (i=0; i<=WIND_MAX_Y-1; i++)
					{	memset(wind->screen[i],' ',WIND_MAX_X);
						wind->screen[i][WIND_MAX_X]=EOS;
					}
			}
	}

int wind_mesag(WIND *wind,int events)
	{	int msgbuf[8] = { 0,0,0,0,0,0,0,0 },nx,ny,nw,nh,
				which,cont=TRUE,retry,thandle,mx,my,mb,ks,kr,br;
		char *pointer;

		wind->evnt = 0;
		do
			{	which = evnt_multi(events,2,1,1,0,0,0,0,0,0,0,0,0,0,msgbuf,
						 										  0,0,&mx,&my,&mb,&ks,&kr,&br);
				retry=FALSE;

				if (which & MU_MESAG)
					{	wind->evnt |= MU_MESAG;
						for (nx=0; nx<=7; nx++)
							wind->msg[nx] = msgbuf[nx];
						switch (msgbuf[0])
							{	case	0x475A		:	if (msgbuf[3]>0)
																		{	pointer = (char *) (long)msgbuf[4]+((long)msgbuf[5]<<16);
																			while (*pointer!=EOS)
																				wind_putch(wind,*pointer++,TRUE);
																		}
																	retry=TRUE;
																	break;
								case	WM_CLOSED :	cont=FALSE;
																	retry=TRUE;
																	break;
								case	WM_TOPPED	:	wind_set(msgbuf[3],WF_TOP);
																	retry=TRUE;
																	break;
								case	WM_REDRAW	:	wind_update(BEG_UPDATE);
																	wind_update(BEG_MCTRL);
																	thandle=msgbuf[3];
																	vst_effects(wind->vdi_handle,0);
																	vsf_perimeter(wind->vdi_handle,FALSE);
																	vswr_mode(wind->vdi_handle,MD_REPLACE);
																	v_hide_c(wind->vdi_handle);
																	wind_get(thandle,WF_FIRSTXYWH,&nx,&ny,&nw,&nh);
																	while (nw!=0 && nh!=0)
																		{	if (up_rc_intersect(&msgbuf[4],&msgbuf[5],&msgbuf[6],&msgbuf[7],&nx,&ny,&nw,&nh))
																				up_wind_redraw(wind,nx,ny,nw,nh);
																			wind_get(thandle,WF_NEXTXYWH,&nx,&ny,&nw,&nh);
																		}
																	vst_effects(wind->vdi_handle,wind->effects);
																	vswr_mode(wind->vdi_handle,wind->mode);
																	v_show_c(wind->vdi_handle,0);
																	wind_update(END_MCTRL);
																	wind_update(END_UPDATE);
																	retry=TRUE;
																	break;
								case	WM_FULLED	:	thandle=msgbuf[3];
																	wind_get(0,WF_WORKXYWH,&nx,&ny,&nw,&nh);
																	nx = (nw - wind->b[2]) / 2 + nx;
																	ny = (nh - wind->b[3]) / 2 + ny;
																	wind_set(thandle,WF_CURRXYWH,nx,ny,wind->b[2],wind->b[3]);
																	nx = nx - wind->b[0];
																	ny = ny - wind->b[1];
																	wind->b[0] += nx;	wind->b[1] += ny;
																	wind->w[0] += nx;	wind->w[1] += ny;
																	wind->pxy[0] += nx;	wind->pxy[1] += ny;
																	wind->pxy[2] += nx;	wind->pxy[3] += ny;
																	wind->txy[0] += nx;	wind->txy[1] += ny;
																	wind->txy[2] += nx;	wind->txy[3] += ny;
																	retry=TRUE;
																	break;
								case	WM_MOVED	:	thandle=msgbuf[3];
																	wind_get(0,WF_WORKXYWH,&nx,&ny,&nw,&nh);
																	if (msgbuf[4]+msgbuf[6]>nx+nw)
																		msgbuf[4]=nx+nw-msgbuf[6];
																	if (msgbuf[5]+msgbuf[7]>ny+nh)
																		msgbuf[5]=ny+nh-msgbuf[7];
																	if (msgbuf[4]<0) msgbuf[4]=0;
																	if (msgbuf[5]<0) msgbuf[5]=0;
																	nx=msgbuf[4];	ny=msgbuf[5];
																	nw=msgbuf[6];	nh=msgbuf[7];
																	wind_set(thandle,WF_CURRXYWH,nx,ny,nw,nh);
																	nx = nx - wind->b[0];
																	ny = ny - wind->b[1];
																	wind->b[0] += nx;	wind->b[1] += ny;
																	wind->w[0] += nx;	wind->w[1] += ny;
																	wind->pxy[0] += nx;	wind->pxy[1] += ny;
																	wind->pxy[2] += nx;	wind->pxy[3] += ny;
																	wind->txy[0] += nx;	wind->txy[1] += ny;
																	wind->txy[2] += nx;	wind->txy[3] += ny;
																	retry=TRUE;
																	break;
							}
					}
				if (which & MU_KEYBD)
					{	wind->evnt |= MU_KEYBD;
						wind->kstate = ks;
						wind->kscan = kr;
					}
				if (which & MU_BUTTON)
					{	wind->evnt |= MU_BUTTON;
						wind->mx = mx;
						wind->my = my;
						wind->mclicks = br;
						wind->mstate = mb;
					}
				if (which & MU_TIMER)
					wind->evnt |= MU_TIMER;
			} 
		while (retry && cont);
		return(cont);
	}

int wind_top(WIND *wind)
	{	int tophandle;
		wind_get(wind->handle,WF_TOP,&tophandle,0,0,0);
		return(tophandle==wind->handle);
	}

/* Ausgabe einer Zeichenkette OHNE Zeilenvorschub */
/* Der Cursor bleibt wo er ist!										*/

void wind_write(WIND *wind,char *txt)
	{	wind_clip(wind,TRUE);
		if (wind->cursor) wind_cursor(wind);
		v_gtext(wind->vdi_handle,wind->txy[0],wind->txy[1]+wind->hbox*wind->y,txt);
		if (wind->cursor) wind_cursor(wind);
		wind_clip(wind,FALSE);
		memset(wind->screen[wind->y],' ',WIND_MAX_X);
		memcpy(wind->screen[wind->y],txt,strlen(txt));
	}

void wind_print(WIND *wind,char *txt)
	{	char *p1,*p2,temp[WIND_MAX_X+1];
		wind_clip(wind,TRUE);
		if (wind->cursor)	wind_cursor(wind);
		if (strchr(txt,27)==NULL)
			{	strncpy(temp,txt,WIND_MAX_X);
				temp[WIND_MAX_X]=EOS;
				v_gtext(wind->vdi_handle,wind->txy[0],wind->txy[1]+wind->hbox*wind->y,temp);
				memset(wind->screen[wind->y],' ',WIND_MAX_X);
				memcpy(wind->screen[wind->y],temp,strlen(temp));
			}
		else
			{	ext_v_gtext(wind,wind->txy[0],wind->txy[1]+wind->hbox*wind->y,txt);
				p1=txt; p2=wind->screen[wind->y];
				while (*p1!=EOS)
					{	if (*p2!=27)
							*p2++=*p1++;
						else
							{	p1++;
								if (*p1!=EOS)
									p1++;
							}
					}
			}

		wind->y++;
		wind->x = 0;
		if (wind->y > wind->max_y)
			{	wind->y = wind->max_y;
				up_wind_scrollup(wind,0);
			}
		if (wind->cursor)	wind_cursor(wind);
		wind_clip(wind,FALSE);
	}

void wind_input(WIND *wind,char *text,char *ret,int length)
	{	char temp[512];
		int pos=0,c,quit=FALSE;

		wind_write(wind,text);
		wind->x = (int)strlen(text);
		
		if (length+wind->x >= wind->max_x)
			length=wind->max_x - wind->x - 1;
		if (length<=0)
			length=1;

		wind_putch(wind,'_',TTY);
		*temp=EOS;

		while (!quit)
			{	c=0;
				if (!wind_mesag(wind,MU_KEYBD|MU_MESAG))
					c=13;
				if (wind->evnt & MU_KEYBD)
					c=wind->kscan & 0xff;
				switch(c)
					{	case	13	:	quit=TRUE;	break;
						case	 8	: if (pos>0)
													{	wind_putch(wind, 27,VT52);
														wind_putch(wind,'D',VT52);
														wind_putch(wind,' ',TTY);
														wind_putch(wind, 27,VT52);
														wind_putch(wind,'D',VT52);
														wind_putch(wind, 27,VT52);
														wind_putch(wind,'D',VT52);
														wind_putch(wind,'_',TTY);
														temp[pos]=EOS;
														pos--;
													}
						default		: if (pos<length)
													{	if (c>=' ')
															{	wind_putch(wind, 27,VT52);
																wind_putch(wind,'D',VT52);
																wind_putch(wind, c ,TTY);
																wind_putch(wind,'_',TTY);
																temp[pos] = c;
																temp[pos+1] = EOS;
																pos++;
															}
													}
					}
			}
		wind_putch(wind, 27,VT52);
		wind_putch(wind,'D',VT52);
		wind_putch(wind,' ',TTY);
		wind_putch(wind, 13,TTY);
		wind_putch(wind, 10,TTY);
		strcpy(ret,temp);
	}

void wind_cls(WIND *wind)
	{	wind_clip(wind,TRUE);
		if (wind->cursor)	wind_cursor(wind);
		up_wind_cls(wind);
		if (wind->cursor)	wind_cursor(wind);
		wind_clip(wind,FALSE);
	}

void wind_clip(WIND *wind,int mode)
	{	register int vh = wind->vdi_handle;
		if (mode)	{	v_hide_c(vh);
								vs_clip(vh,TRUE,wind->pxy);
							}
		else			{	vs_clip(vh,FALSE,wind->pxy);
								v_show_c(vh,0);
							}
	}

void wind_cursor(WIND *wind)
	{	int pxy[4];
		register int vh = wind->vdi_handle;
		pxy[2]=pxy[0]=wind->txy[0]+wind->wbox*wind->x;
		pxy[3]=pxy[1]=wind->txy[1]+wind->hbox*wind->y;
		pxy[2] += wind->wbox-1;
		pxy[3] += wind->hbox-1;
		vswr_mode(vh,MD_XOR);
		vsf_interior(vh,0);
		vsf_perimeter(vh,TRUE);
		v_bar(vh,pxy);
		vsf_perimeter(vh,FALSE);
		vswr_mode(vh,wind->mode);
	}

void wind_putch(WIND *wind,int c,int vtmode)
	{	if (!(vtmode & 128))
			if (wind->escape<=1)
				{	wind_clip(wind,TRUE);
					if (wind->cursor)	wind_cursor(wind);
				}

		if (wind->escape==101)				/* CSI-CODE ?					*/
			{	if (c>='0' && c<='9')
					num = 10*num + (c - 48);
				else
					up_wind_vt100_quest(wind,c);
			}
		else if (wind->escape==100)		/* CSI-CODE VT-100		*/
			{	if (c>='0' && c<='9')
					num = 10*num + (c - 48);
				else
					{	cnum = num;
						if (cnum==0) cnum=1;
						up_wind_vt100(wind,c);
					}
			}
		else if (wind->escape==5)
			{	wind->escape = 0; /* Kein PAPER */
			}
		else if (wind->escape==4)
			{	wind->escape = 0; /* Kein INK */
			}
		else if (wind->escape==3)
			{	wind->x = MIN(c-' ',wind->max_x);
				wind->x = MAX(wind->x,0);
				wind->escape = 0;
			}
		else if (wind->escape==2)
			{	wind->y = MIN(c-' ',wind->max_y);
				wind->y = MAX(wind->y,0);
				wind->escape = 3;
			}
		else if (wind->escape==1)
			{	switch(vtmode & 0xf)
					{ case VT100	:	if ( c=='[' )
														{	wind->escape = 100;
														}
													else
														{	/* ESC-CODE VT100 		*/
															if (c>='0' && c<='9')
																num = 10*num + (c - 48);
															else
																{	cnum = num;
																	if (cnum==0) cnum=1;
																	up_wind_vt100_esc(wind,c);
																}
														}
													break;
						case VT52		:	up_wind_vt52(wind,c);
					}
			}
		else if (wind->escape==0)
			{	switch (c)
					{	case	7	:	putch_bios(2,7); break;
						case	8	:	if (wind->x > 0)
												{	wind->x--;
													wind->screen[wind->y][wind->x] = ' ';
													up_wind_putch(wind,32);
												}
											break;
						case	9	:	wind->x = MIN((wind->x + 8) & ~7,wind->max_x);
											break;
						case 12	:	up_wind_cls(wind); break;
						case 13	:	wind->x = 0; break;
						case 10	:	if (++wind->y > wind->max_y)
												{	wind->y = wind->max_y;
													up_wind_scrollup(wind,0);
												}
											break;
						case 27	:	if (vtmode & 0xf)
												{	num = 0;
													wind->escape = 1;
													break;
												}
						default	:	up_wind_putch(wind,c);
											if (wind->x < WIND_MAX_X && wind->y < WIND_MAX_Y)
												wind->screen[wind->y][wind->x] = c;
											if (++wind->x > wind->max_x)
												{	if (wind->wrap)
														{	wind->x = 0;
															if (++wind->y > wind->max_y)
																{	wind->y = wind->max_y;
																	up_wind_scrollup(wind,0);
																}
														}
													else
														{	wind->x = wind->max_x;
														}
												}
											break;
					}
			}
		if (!(vtmode & 128))
			if (wind->escape<=1)
				{	if (wind->cursor)	wind_cursor(wind);
					wind_clip(wind,FALSE);
				}
	}

int wind_pause(WIND *wind)
	{	int i=TRUE,ret=0;
		do
			{	wind_mesag(wind,MU_KEYBD|MU_MESAG|MU_TIMER);
				if ( (wind->evnt & MU_KEYBD) &&
						 (wind->kscan==UNDO ||
						 	wind->kscan==ALT_Q ||
						 	wind->kscan==CNTRL_Q ||
						 	wind->kscan==RETURN ||
						 	wind->kscan==ENTER ||
						 	wind->kscan==KEY_SPACE) )
					{	i=FALSE;
						ret = wind->kscan;
					}
				if ( (wind->evnt & MU_MESAG) &&
						 (wind->msg[0]==WM_CLOSED) )
					{	i=FALSE;
						ret = UNDO;
					}
			}
		while(i);
		return(ret);
	}

/*******************************************************************/

void set_redirect(int lna)
	{	linea=lna;
/*		v_hide_c(vdi_handle); */
		Supexec(set_it);
	}

void reset_redirect(void)
	{	Supexec(reset_it);
/*		v_show_c(vdi_handle,0); */
		linea=FALSE;
	}

void putch_bios(int dev,int chr)
	{	switch(dev)
			{ case	2	:	if (old_con!=NULL)
										(*old_con)(dev,chr);
									else
										Bconout(dev,chr);
									break;
				case	5	:	if (old_rawcon!=NULL)
										(*old_rawcon)(dev,chr);
									else
										Bconout(dev,chr);
									break;
				default	:	Bconout(dev,chr);
			}
	}

static long set_it(void)
	{	old_con = *((function *)0x586L);
		old_rawcon = *((function *)0x592L);
		*((function *)0x586L) = (function) newcon;
		*((function *)0x592L) = (function) newrawcon;
		if ( linea )
			{	linea_init();
				if ( Vdiesc==NULL )
					{	linea=FALSE;
					}
				else
					{	save_cur[0] = Vdiesc->v_cel_mx;
						save_cur[1] = Vdiesc->v_cel_my;
						Vdiesc->v_cel_mx = wind.max_x;
    				Vdiesc->v_cel_my = wind.max_y;
    			}
    	}
		return(0);
	}

static void zeichen_raus(int mode)
	{	int i;
		wind_clip(&wind,TRUE);
		if (wind.cursor)	wind_cursor(&wind);
		for (i=1; i<=chrcnt; i++)
			{	wind_putch(&wind,buf[i],mode);
				if ( linea )
					{	Vdiesc->v_cur_xy[0]=wind.x;
						Vdiesc->v_cur_xy[1]=wind.y;
					}
			}
		if (wind.cursor)	wind_cursor(&wind);
		wind_clip(&wind,FALSE);
		chrcnt = 0;
	}

static long reset_it(void)
	{	if (divflags & D_USEBUFFER && chrcnt)
			zeichen_raus(buffertype==CON ? VT52|128 : TTY|128);
		*((function *)0x586L) = old_con;
		*((function *)0x592L) = old_rawcon;
		old_con =
		old_rawcon = NULL;
		if ( linea )
			{	linea_init();
				if ( Vdiesc!=NULL )
					{	Vdiesc->v_cel_mx = save_cur[0];
		    		Vdiesc->v_cel_my = save_cur[1];
		    	}
    	}
		return(0);
	}

/* diff = *((long *)0x4BAL) - old200hztimer; */

static void cdecl newcon(int dev,int chr)
	{	if (dev==2)	{	if (divflags & D_USEBUFFER)
										{	if (buffertype==RAWCON)
												zeichen_raus(TTY|128);
											buffertype=CON;
											buf[++chrcnt]=chr;
											if (chrcnt==SCREENBUFFER || chr==13 || chr==27)
												zeichen_raus(VT52|128);
										}
									else
										{	wind_putch(&wind,chr,VT52);
											if ( linea )
							        	{	Vdiesc->v_cur_xy[0]=wind.x;
									        Vdiesc->v_cur_xy[1]=wind.y;
									      }
										}
								}
		else				(*old_con)(dev,chr);
	}

static void cdecl newrawcon(int dev,int chr)
	{	if (dev==5)	{	if (divflags & D_USEBUFFER)
										{	if (buffertype==CON)
												zeichen_raus(VT52|128);
											buffertype=RAWCON;
											buf[++chrcnt]=chr;
											if (chrcnt==SCREENBUFFER || chr==13)
												zeichen_raus(TTY|128);
										}
									else
										{	wind_putch(&wind,chr,TTY);
											if ( linea )
							        	{	Vdiesc->v_cur_xy[0]=wind.x;
									        Vdiesc->v_cur_xy[1]=wind.y;
									      }
										}
								}
		else				(*old_rawcon)(dev,chr);
	}

/* static void cdecl newrawcon(int dev,int chr)
	{	if (dev==5)	{	wind_putch(&wind,chr,TTY);
									if ( linea )
					        	{	Vdiesc->v_cur_xy[0]=wind.x;
							        Vdiesc->v_cur_xy[1]=wind.y;
							      }
**							    else
					        	{	(*old_con)(2,27);
							        (*old_con)(2,'Y');
							        (*old_con)(2,wind.y+' ');
							        (*old_con)(2,wind.x+' ');
										} **
					      }
		else				(*old_rawcon)(dev,chr);
	} */

/*******************************************************************/

static void up_wind_redraw(WIND *wind,int x,int y,int w,int h)
	{	int i,pxy[4];
		pxy[2] = pxy[0] = x;
		pxy[3] = pxy[1] = y;
		pxy[2] += w-1;
		pxy[3] += h-1;
		vs_clip(wind->vdi_handle,TRUE,pxy);
		v_bar(wind->vdi_handle,pxy);
		for (i=(y - wind->txy[1]) /wind->hbox; i<=wind->max_y; i++)
			v_gtext(wind->vdi_handle,wind->txy[0],wind->txy[1]+wind->hbox * i,wind->screen[i]);
		if (wind->cursor)	wind_cursor(wind);
		vs_clip(wind->vdi_handle,FALSE,pxy);
	}	

static void up_wind_putch(WIND *wind,int c)
	{	static int pxy[4];
		register int vh = wind->vdi_handle;
		*outchar = c;
		pxy[0]=wind->txy[0]+wind->wbox*wind->x;
		pxy[1]=wind->txy[1]+wind->hbox*wind->y;
		if (wind->mode==MD_ERASE)
			{	pxy[2] = pxy[0] + wind->wbox - 1;
				pxy[3] = pxy[1] + wind->hbox - 1;
				vswr_mode(vh,MD_REPLACE);
				v_bar(vh,pxy);
				vswr_mode(vh,MD_ERASE);
			}
		v_gtext(vh,pxy[0],pxy[1],outchar);
	}

static void ext_v_gtext(WIND *wind,int x,int y,char *txt)
	{	char *ps,*pesc,xtxt[512];
		int l,pxy[4];
		strcpy(xtxt,txt);
		ps=xtxt;
		do
			{	pesc=strchr(ps,27);
				if (pesc==NULL)
					{	v_gtext(wind->vdi_handle,x,y,ps);
						break;
					}
				*(pesc++) = EOS;
				l = (int) strlen(ps);

				if (wind->mode==MD_ERASE)
					{	pxy[2]=pxy[0]=x;
						pxy[3]=pxy[1]=y;
						pxy[2] += wind->wbox*l-1;
						pxy[3] += wind->hbox-1;
						vswr_mode(wind->vdi_handle,MD_REPLACE);
						v_bar(wind->vdi_handle,pxy);
						vswr_mode(wind->vdi_handle,MD_ERASE);
					}
				v_gtext(wind->vdi_handle,x,y,ps);
				x += l*wind->wbox;
				switch(*pesc)
					{ case	'p'	:	vswr_mode(wind->vdi_handle,MD_ERASE);
												wind->mode = MD_ERASE;
												break;
						case	'q'	:	vswr_mode(wind->vdi_handle,MD_REPLACE);
												wind->mode = MD_REPLACE;
					}
				ps = pesc+1;
			}
		while (*ps!=EOS);
	}

static void up_wind_cls(WIND *wind)
	{	int i;
		v_bar(wind->vdi_handle,wind->pxy);
		wind->x =	wind->y = 0;
		for (i=0; i<=WIND_MAX_Y-1; i++)
			{	memset(wind->screen[i],' ',WIND_MAX_X);
				wind->screen[i][WIND_MAX_X]=EOS;
			}
	}

static void up_wind_delline(WIND *wind,int y)
	{	int pxy[4];
		pxy[0] = wind->txy[0];
		pxy[3] = pxy[1] = wind->txy[1] + wind->hbox*MIN(y,wind->max_y);
		pxy[2] = wind->txy[2];
		pxy[3] += wind->hbox - 1;
		v_bar(wind->vdi_handle,pxy);
		memset(wind->screen[y],' ',WIND_MAX_X);
		wind->screen[y][WIND_MAX_X]=EOS;
	}

/* Scroll_up ab (einschl.) Zeile y */

static void up_wind_scrollup(WIND *wind,int y)
	{	MFDB s,d;
		register int i;
		int	pxy[8];
		if (wind->max_y > 1)
			{	pxy[4]=pxy[0]=wind->txy[0];
				pxy[5]=pxy[1]=wind->txy[1] + wind->hbox*y;
				pxy[6]=pxy[2]=wind->txy[2];
				pxy[7]=pxy[3]=wind->txy[3];
				pxy[1] += wind->hbox;
				pxy[7] -= wind->hbox;
				s.fd_addr = d.fd_addr = NULL;
				vro_cpyfm(wind->vdi_handle,S_ONLY,pxy,&s,&d);
				for (i=y; i<wind->max_y; i++)
					memcpy(wind->screen[i],wind->screen[i+1],WIND_MAX_X);
			}
		up_wind_delline(wind,wind->max_y);
	}

/* Scroll_down ab (einschl.) Zeile y */

static void up_wind_scrolldown(WIND *wind,int y)
	{	MFDB s,d;
		register int i;
		int	pxy[8];
		if (wind->max_y > 1)
			{	pxy[4]=pxy[0]=wind->txy[0];
				pxy[5]=pxy[1]=wind->txy[1] + wind->hbox*y;
				pxy[6]=pxy[2]=wind->txy[2];
				pxy[7]=pxy[3]=wind->txy[3];
				pxy[5] += wind->hbox;
				pxy[3] -= wind->hbox;
				s.fd_addr = d.fd_addr = NULL;
				vro_cpyfm(wind->vdi_handle,S_ONLY,pxy,&s,&d);
				if (y < wind->max_y)
					for (i=wind->max_y; i>y; i--)
						memcpy(wind->screen[i],wind->screen[i-1],WIND_MAX_X);
			}
		up_wind_delline(wind,y);
	}

static int up_rc_intersect(int *x1,int *y1,int *w1,int *h1,int *x2,int *y2,int *w2,int *h2)
	{	register int tx,ty,tw,th;
		tw = MIN (*x2 + *w2, *x1 + *w1);
		th = MIN (*y2 + *h2, *y1 + *h1);
		tx = MAX (*x2, *x1);
		ty = MAX (*y2, *y1);
		*x2 = tx;
		*y2 = ty;
		*w2 = tw - tx;
		*h2 = th - ty;
		return ((tw > tx) && (th > ty));
	}

static void up_cls_sol(WIND *wind)
	{	int i,pxy[4];
		if (wind->x > 0)
			{	for (i=wind->x-1; i>=0; i--)
					wind->screen[wind->y][i]=' ';
				pxy[2] = pxy[0] = wind->txy[0];
				pxy[3] = pxy[1] = wind->txy[1] + wind->hbox*MIN(wind->y,wind->max_y);
				pxy[2] += wind->wbox*MIN(wind->x,wind->max_x)-1;
				pxy[3] += wind->hbox - 1;
				v_bar(wind->vdi_handle,pxy);
			}
	}

static void up_cls_eol(WIND *wind)
	{ int i,pxy[4];
		for (i=wind->x; i<=wind->max_x; i++)
			wind->screen[wind->y][i]=' ';
		pxy[0] = wind->txy[0] + wind->wbox*MIN(wind->x,wind->max_x);
		pxy[3] = pxy[1] = wind->txy[1] + wind->hbox*MIN(wind->y,wind->max_y);
		pxy[2] = wind->txy[2];
		pxy[3] += wind->hbox - 1;
		v_bar(wind->vdi_handle,pxy);
	}

static void up_cls_up(WIND *wind)
	{	int i,pxy[4];
		if (wind->y > 0)
			{	for (i=0; i<=wind->y-1; i++)
					{	memset(wind->screen[i],' ',WIND_MAX_X);
						wind->screen[i][WIND_MAX_X]=EOS;
					}
				pxy[0] = wind->txy[0];
				pxy[3] = pxy[1] = wind->txy[1];
				pxy[2] = wind->txy[2];
				pxy[3] += wind->hbox*MIN(wind->y,wind->max_y)-1;
				v_bar(wind->vdi_handle,pxy);
			}
	}

static void up_cls_down(WIND *wind)
	{	int i,pxy[4];
		if (wind->y < wind->max_y)
			{	for (i=wind->y+1; i<=wind->max_y; i++)
					{	memset(wind->screen[i],' ',WIND_MAX_X);
						wind->screen[i][WIND_MAX_X]=EOS;
					}
				pxy[0] = wind->txy[0];
				pxy[1] = wind->txy[1] + wind->hbox*MIN(wind->y+1,wind->max_y);
				pxy[2] = wind->txy[2];
				pxy[3] = wind->txy[3];
				v_bar(wind->vdi_handle,pxy);
			}
	}

static void up_ansi_attr(WIND *wind,int x)
	{	switch(x)
			{ case 0	: wind->mode = MD_REPLACE;
									wind->effects = 0;
									break;
				case 1	:	wind->effects |= 1;		break;
				case 21	:	wind->effects &= ~1;	break;
				case 3	:	wind->effects |= 4;		break;
				case 23	:	wind->effects &= ~4;	break;
				case 4	:	wind->effects |= 8;		break;
				case 24	:	wind->effects &= ~8;	break;
				case 5	:	wind->effects |= 2;		break;
				case 25	:	wind->effects &= ~2;	break;
				case 7	:	wind->mode = MD_ERASE;		break;
				case 27	:	wind->mode = MD_REPLACE;	break;
				case 8	:	wind->effects |= 16;	break;
				case 28	:	wind->effects &= ~16;	break;
			}
		vst_effects(wind->vdi_handle,wind->effects);
		vswr_mode(wind->vdi_handle,wind->mode);
	}

static void up_wind_vt52(WIND *wind,int c)
	{	wind->escape = 0;
		switch(c)
			{	case 'A'	:	if (--wind->y < 0) wind->y=0; break;
				case 'B'	: if (++wind->y > wind->max_y) --wind->y; break;
				case 'C'	: if (++wind->x > wind->max_x) --wind->x; break;
				case 'D'	: if (--wind->x < 0) wind->x=0; break;
				case 'E'	:	up_wind_cls(wind); break;
				case 'H'	: wind->x=wind->y=0; break;
				case 'I'	: if (--wind->y < 0)
											{	wind->y=0;
												up_wind_scrolldown(wind,0);
											}
										break;
				case 'J'	: up_cls_down(wind);	/* KEIN BREAK */
				case 'K'	: up_cls_eol(wind); break;
				case 'L'	: up_wind_scrolldown(wind,wind->y);
										wind->x=0;
										break;
				case 'M'	: up_wind_scrollup(wind,wind->y);
										wind->x=0;
										break;
				case 'Y'	: wind->escape=2; break;
				case 'b'	: wind->escape=4; break;
				case 'c'	: wind->escape=5; break;
				case 'e'	: wind->cursor=TRUE; break;
				case 'f'	: wind->cursor=FALSE; break;
				case 'j'	: wind->savex=wind->x;
										wind->savey=wind->y;
										break;
				case 'k'	: wind->x=wind->savex;
										wind->y=wind->savey;
										break;
				case 'l'	: up_wind_delline(wind,wind->y);
										wind->x = 0;
										break;
				case 'd'	: up_cls_up(wind);	/* KEIN BREAK */
				case 'o'	: up_cls_sol(wind); break;
				case 'p'	: vswr_mode(wind->vdi_handle,MD_ERASE);
										wind->mode = MD_ERASE;
										break;
				case 'q'	:	vswr_mode(wind->vdi_handle,MD_REPLACE);
										wind->mode = MD_REPLACE;
										break;
				case 'v'	: wind->wrap = TRUE; break;
				case 'w'	: wind->wrap = FALSE; break;
			}
	}

static void up_wind_vt100(WIND *wind,int c)
	{	int i;
		wind->escape = 0;
		switch(c)
			{	case 'R'	:	for (i=1; i<=cnum; i++)	up_wind_scrollup(wind,0);
										break;
				case 'S'	: for (i=1; i<=cnum; i++)	up_wind_scrolldown(wind,0);
										break;
				case 'C'	: wind->x = MIN(wind->max_x,wind->x+cnum);
										break;
				case 'D'	: wind->x = MAX(0,wind->x-cnum);
										break;
				case 'B'	: wind->y = MIN(wind->max_y,wind->y+cnum);
										break;
				case 'A'	: wind->y = MAX(0,wind->y-cnum);
										break;
				case 'M'	:	for (i=1; i<=cnum; i++)	up_wind_scrollup(wind,wind->y);
										wind->x = 0;
										break;
				case 'L'	:	for (i=1; i<=cnum; i++)	up_wind_scrolldown(wind,wind->y);
										wind->x = 0;
										break;
				case 'J'	:	switch(num)
											{	case 2	:	up_wind_cls(wind);
																	wind->x = wind->y = 0;
																	break;
												case 1	: up_cls_sol(wind);
																	up_cls_up(wind);
																	break;
												case 0	: up_cls_eol(wind);
																	up_cls_down(wind);
											}
										break;
				case 'K'	: switch(num)
											{ case 2	:	up_wind_delline(wind,wind->y);
																	wind->x = 0;
																	break;
												case 1	: up_cls_sol(wind); break;
												case 0	:	up_cls_eol(wind);
											}
										break;
				case 's'	: wind->savex=wind->x;
										wind->savey=wind->y;
										break;
				case 'u'	: wind->x=wind->savex;
										wind->y=wind->savey;
										break;

				case ';'	: esclist[esccount++] = num;
										if (esccount>MAX_ESC)
											esccount=MAX_ESC;
										wind->escape = 100;
										num = 0;
										break;
				case 'm'	:	if (esccount>0)
											{	for (i=0; i<esccount; i++)
													up_ansi_attr(wind,esclist[i]);
											}
										up_ansi_attr(wind,num);
										break;
				case 'f'	:
				case 'H'	:	if (esccount==0)	{	wind->x =
																				wind->y = 0;
																			}
										else							{	wind->y = esclist[0];
																				wind->x = num;
																			}
										if (wind->x > wind->max_x)	wind->x = wind->max_x;
										if (wind->y > wind->max_y)	wind->y = wind->max_y;
										break;
				case '?'	: wind->escape = 101;
										num = 0;
										break;
			}
		if ( c!=';' )
			esccount = 0;
	}

static void up_wind_vt100_esc(WIND *wind,int c)
	{	int i;
		wind->escape = 0;
		switch(c)
			{	case 'M'	: if (--wind->y < 0)
											{	wind->y=0;
												up_wind_scrolldown(wind,0);
											}
										break;
				case 'E'	:	wind->x = 0;
				case 'D'	:	for (i=1; i<=cnum; i++)
											{	if (++wind->y > wind->max_y)
													{	wind->y = wind->max_y;
														up_wind_scrollup(wind,0);
													}
											}
										break;
				case '7'	: wind->savex=wind->x;
										wind->savey=wind->y;
										break;
				case '8'	: wind->x=wind->savex;
										wind->y=wind->savey;
										break;
				case 'c'	:	vswr_mode(wind->vdi_handle,MD_REPLACE);
										wind->mode = MD_REPLACE;
										vst_effects(wind->vdi_handle,0);
										wind->effects = 0;
										wind->x = wind->y = 0;
										break;
			}
	}

static void up_wind_vt100_quest(WIND *wind,int c)
	{	wind->escape = 0;
		if (c=='h')
			{	switch(num)
					{	case 7	:	wind->wrap=TRUE;	break;
					}
			}
		else if (c=='l')
			{	switch(num)
					{	case 7	:	wind->wrap=FALSE;	break;
					}
			}
	}

#pragma warn -use
static char copyright[]=" WindMini-Library V1.8 (c) Stephan Slabihoud ";
