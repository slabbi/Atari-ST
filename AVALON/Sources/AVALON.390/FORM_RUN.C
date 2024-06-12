#include <aes.h>
#include <tos.h>
#include <scancode.h>
#include <my_scan.h>
#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "version.h"
#include "fido_msg.h"
#include "vars.h"
#include "cookie.h"
#include "avalon.h"

/*	FORM_RUN   Window-Dialog Routinen    (c) St.Slabihoud

void xredraw_slider(OBJECT *tree,int handle,int back,int front,long max,long count,int page);
	tree	- Tree
	back	- Hintergrund
	front	- Vordergrund
	max		- Gesamtzeilen (1...n)
	count	-	Oberste Zeile (1...max)
	page	-	Fenstergr”že (1...n)

Initialisiert Window-Routinen:
		xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh);
™ffnet Window:
		xform_dial(tree,XFMD_START,&handle,&wnx,&wny,&wnw,&wnh);
Stellt neuen Dialog im Window dar:
		xform_dial(tree,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
Schliežt Window:
		xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
L”scht Window:
		xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh);

Normaler Dialog ohne Key/Menu/Time-Auswertung:
		xform_do(handle,tree,NULL,start_field,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
Dialog mit Key-Auswertung:
		xform_do(handle,tree,NULL,start_field,0,trans_key,&wnx,&wny,&wnw,&wnh);
		int trans_key(int shift,int key);
Dialog mit Menu-Auswertung:
		xform_do(handle,tree,menutree,start_field,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
Dialog mit Time-Auswertung:
		xform_do(handle,tree,NULL,start_field,time_ms,NULLFUNC,&wnx,&wny,&wnw,&wnh);

    Doppelklick	:	0x8000
    Closer			:	0x4000
    Menuanwahl	:	0x2000
    Timer				:	0x1000;
    FidoMessage	:	0x0800;

*/


static int find_object(OBJECT *tree,int start_object,int which);
static int ini_field(OBJECT *tree,int start_field);

int		xform_do(int handle,OBJECT *tree,OBJECT *menutree,int start_field,long time,int (*trans_key)(int,int),int *wnx,int *wny,int *wnw,int *wnh);
int		xform_dial(OBJECT *tree,int mode,int *handle,int *wnx,int *wny,int *wnw,int *wnh);
int		NULLFUNC(int,int);
int		rc_intersect(int *x1,int *y1,int *w1,int *h1,int *x2,int *y2,int *w2,int *h2);
void	xredraw_slider(OBJECT *tree,int handle,int sliderbox,int back,int front,long max,long count,int page,int redraw);
void	redraw_objc(OBJECT *tree, int index,int handle);
void	redraw_one_objc(OBJECT *tree, int index,int handle);

int NULLFUNC()
	{	return 0; };

#define XFMD_INIT			0
#define XFMD_START		1
#define XFMD_GROW			2
#define	XFMD_DIALOG		3
#define	XFMD_SHRINK		4
#define XFMD_FINISH		5
#define XFMD_EXIT			6
#define FMD_BACKWARD  -1
#define FMD_FORWARD   -2
#define FMD_DEFLT     -3

int	formrun_msgbuf[8] = { 0,0,0,0,0,0,0,0 };

static int windowflag=0;
static char *fmsg[] =
	{	"[3][FORM-RUN: XFMD_INIT missing][ OK ]",
		"[3][FORM-RUN: XFMD_START missing][ OK ]",
		"[3][FORM-RUN: XFMD_FINISH missing][ OK ]",
		"[3][FORM-RUN: XFMD_EXIT missing][ OK ]"
	};

/* kann zu GRECT werden */
int rc_intersect(int *x1,int *y1,int *w1,int *h1,int *x2,int *y2,int *w2,int *h2)
	{	int tx,ty,tw,th;

		if (divflags & D_USESMART)
			if ( (*x1<*x2) && (*y1<*y2) && (*x1+*w1>*x2+*w2) && (*y1+*h1>*y2+*h2) )
				return (FALSE);

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

int xform_dial(OBJECT *tree,int mode,int *handle,int *wnx,int *wny,int *wnw,int *wnh)
	{	int nx,ny,nw,nh,deskx,desky,deskw,deskh,ok=TRUE,d,pipe[8];
		switch (mode)
			{	case	XFMD_INIT		:	if (windowflag==2)	form_alert(1,fmsg[2]);
														if (windowflag>=1)	form_alert(1,fmsg[3]);
														wind_get(0,WF_WORKXYWH,wnx,wny,wnw,wnh);
														if ( (*handle=wind_create(NAME|CLOSER|MOVER|FULLER,*wnx,*wny,*wnw,*wnh))<0)
															ok=FALSE;
														else
															windowflag=1;
														break;

				case	XFMD_START	:	if (windowflag==0)	form_alert(1,fmsg[0]);
														if (windowflag==2)	form_alert(1,fmsg[2]);

														wind_get(0,WF_WORKXYWH,&deskx,&desky,&deskw,&deskh);
														if (OB_X(tree,0)>deskx+deskw || OB_Y(tree,0)>desky+deskh)
															{	OB_X(tree,0) =
																OB_Y(tree,0) = 0;
															}

														if (OB_X(tree,0)==0 && OB_Y(tree,0)==0)
															{	nx=OB_X(tree,0); ny=OB_Y(tree,0);
																nw=OB_W(tree,0); nh=OB_H(tree,0);
																wind_calc(WC_BORDER,NAME|CLOSER|MOVER|FULLER,nx,ny,nw,nh,&d,&d,wnw,wnh);

/*															wind_get(0,WF_WORKXYWH,&deskx,&desky,&deskw,&deskh);
*/

																*wnx = (deskw - *wnw) / 2 + deskx;
																*wny = (deskh - *wnh) / 2 + desky;
																wind_calc(WC_WORK,NAME|CLOSER|MOVER|FULLER,*wnx,*wny,*wnw,*wnh,&nx,&ny,&d,&d);
																OB_X(tree,0)=nx;	OB_Y(tree,0)=ny;
															}

														nx=OB_X(tree,0); ny=OB_Y(tree,0);
														nw=OB_W(tree,0); nh=OB_H(tree,0);
														wind_calc(WC_BORDER,NAME|CLOSER|MOVER|FULLER,nx,ny,nw,nh,wnx,wny,wnw,wnh);

/* Probleme unter WINX!			if (wind_set(*handle,WF_CURRXYWH,*wnx,*wny,*wnw,*wnh)==0)
															ok=FALSE;
*/

														if (wind_set(*handle,WF_NAME,AVALONNAME)==0)
															ok=FALSE;
														if (wind_open(*handle,*wnx,*wny,*wnw,*wnh)==0)
															ok=FALSE;
														else
															windowflag=2;

														form_dial(FMD_START,nx,ny,nw,nh,nx,ny,nw,nh);
														form_dial(FMD_FINISH,nx,ny,nw,nh,nx,ny,nw,nh);

														break;

				case	XFMD_FINISH	:	if (windowflag==0)	form_alert(1,fmsg[0]);
														if (windowflag<=1)	form_alert(1,fmsg[1]);
														wind_close(*handle);
														windowflag=1;
														break;

				case	XFMD_EXIT		:	if (windowflag==0)	form_alert(1,fmsg[0]);
														if (windowflag==2)	form_alert(1,fmsg[2]);
														wind_delete(*handle);
														windowflag=0;
														break;

				case	XFMD_DIALOG	:	if (windowflag==0)	form_alert(1,fmsg[0]);
														if (windowflag==1)	form_alert(1,fmsg[1]);

														if (OB_X(tree,0)==0 && OB_Y(tree,0)==0)
															{	nx=OB_X(tree,0); ny=OB_Y(tree,0);
																nw=OB_W(tree,0); nh=OB_H(tree,0);
																wind_calc(WC_BORDER,NAME|CLOSER|MOVER|FULLER,nx,ny,nw,nh,&d,&d,wnw,wnh);

																wind_get(0,WF_WORKXYWH,&deskx,&desky,&deskw,&deskh);
																*wnx = (deskw - *wnw) / 2 + deskx;
																*wny = (deskh - *wnh) / 2 + desky;
																wind_calc(WC_WORK,NAME|CLOSER|MOVER|FULLER,*wnx,*wny,*wnw,*wnh,&nx,&ny,&d,&d);
																OB_X(tree,0)=nx;	OB_Y(tree,0)=ny;
															}

														nx=OB_X(tree,0); ny=OB_Y(tree,0);
														nw=OB_W(tree,0); nh=OB_H(tree,0);
														wind_calc(WC_BORDER,NAME|CLOSER|MOVER|FULLER,nx,ny,nw,nh,wnx,wny,wnw,wnh);
														wind_set(*handle,WF_CURRXYWH,*wnx,*wny,*wnw,*wnh);
														if (divflags & D_USESMART)
															{	pipe[0] = WM_REDRAW;
																pipe[3] = *handle;
																pipe[4] = nx;
																pipe[5] = ny;
																pipe[6] = nw;
																pipe[7] = nh;
																appl_write(ap_id,16,pipe);
															}
														else
															{	form_dial(FMD_START,nx,ny,nw,nh,nx,ny,nw,nh);
																form_dial(FMD_FINISH,nx,ny,nw,nh,nx,ny,nw,nh);
															}
														break;
				default						: ok=FALSE;
			}
		return(ok);
	}

static int find_object(OBJECT *tree,int start_object,int which)
	{	int	object=0,flag=EDITABLE,theflag,increment=1;
    switch (which)
			{	case FMD_BACKWARD	:	increment = -1;
        case FMD_FORWARD	:	object=start_object+increment; break;
        case FMD_DEFLT		:	flag=DEFAULT; break;
    	}
    while (object>=0)
			{	theflag = tree[object].ob_flags;
        if (theflag & flag)
					return (object);
				if (which!=FMD_BACKWARD)
					{	if (theflag & LASTOB)
							object = -1;
						else
							object += increment;
					}
				else
					{	object += increment;
					}
			}
		return start_object;
	}

static int ini_field(OBJECT *tree,int start_field)
	{	if (start_field==0 || start_field==-1)
    	start_field=find_object(tree,0,FMD_FORWARD);
    else
    	{	start_field=find_object(tree,start_field-1,FMD_FORWARD);
    		if (start_field==-1)
    			start_field=find_object(tree,start_field,FMD_BACKWARD);
    	}
    return start_field;
	}

int xform_do(int handle,OBJECT *tree,OBJECT *menutree,int start_field,long time,int (*trans_key)(int,int),int *wnx,int *wny,int *wnw,int *wnh)
	{	int	edit_object=0,next_object,which,cont=TRUE,idx,thandle,flags,
				mx,my,mb,ks,kr,br,
				dflag=FALSE, cflag=FALSE,
				mflag=FALSE, tflag=FALSE,
				fflag=FALSE, d;
		int pipe[8];
		int nx,ny,nw,nh,deskx,desky,deskw,deskh,tophandle;	/* Hilfsvariablen */

		next_object = ini_field(tree,start_field);
    while (cont)
	    {	if ((next_object!=0) && (edit_object!=next_object))
					{	edit_object = next_object;
						next_object = 0;
						objc_edit(tree,edit_object,0,&idx,ED_INIT);
					}
				flags = MU_KEYBD|MU_BUTTON|MU_MESAG;
				if (time!=0)
					flags |= MU_TIMER;

#if !defined( _AVALSEMP_ )
				if (trans_key==NULLFUNC && tree==tree19)
					flags &= ~MU_KEYBD;
#endif

        which = evnt_multi(flags,2,1,1,0,0,0,0,0,0,0,0,0,0,formrun_msgbuf,
        									 (int) (time & 0xffffL),(int) ((time & 0xffff0000L) >> 16),
        									 &mx,&my,&mb,&ks,&kr,&br);

				wind_get(handle,WF_TOP,&tophandle,0,0,0);

			  if (tree!=tree99 && handle==tophandle)
			  	{	if ( ((which & MU_KEYBD) && ((ks & 4) || (ks & 6) || (ks & 5)) && (kr==CNTRL_SPACE)) ||
			  				 ((which & MU_MESAG) && (formrun_msgbuf[0]==WM_CLOSED) && (ks!=0)) )
					  	{	int cdecl (*server)(int f,...);
					  		int x,y,w,h,npos;
			  				if (getcookie('ICFS',(long *)&server))
			  					{	which &= ~(MU_KEYBD|MU_MESAG);
										npos=(*server)(1,&x,&y,&w,&h);
										wind_calc(WC_WORK,NAME,x,y,w,h,&x,&y,&w,&h);
										OB_X(tree99,ROOT)=x; OB_Y(tree99,ROOT)=y;
										OB_W(tree99,ROOT)=w; OB_H(tree99,ROOT)=h;

										OB_SPEC(tree99,ICFSPIC) = OB_SPEC(treepic0,ICFS0);
										OB_W(tree99,ICFSPIC) = OB_W(treepic0,ICFS0);
										OB_H(tree99,ICFSPIC) = OB_H(treepic0,ICFS0);

										OB_Y(tree99,ICFSPIC)=(OB_H(tree99,ROOT)-OB_H(treepic0,ICFS0)) >> 1;
										OB_X(tree99,ICFSPIC)=(OB_W(tree99,ROOT)-OB_W(treepic0,ICFS0)) >> 1;

										if ( OB_H(tree1,DOINFO)<=8 )
											{	OB_SPEC(tree99,ICFSPIC) = OB_SPEC(treepic0,ICFS1);
												OB_W(tree99,ICFSPIC) = OB_W(treepic0,ICFS1);
												OB_H(tree99,ICFSPIC) = OB_H(treepic0,ICFS1);

												OB_Y(tree99,ICFSPIC)=(OB_H(tree99,ROOT)-OB_H(treepic0,ICFS1)) >> 1;
												OB_X(tree99,ICFSPIC)=(OB_W(tree99,ROOT)-OB_W(treepic0,ICFS1)) >> 1;
											}
										xform_dial(tree99,XFMD_DIALOG,&handle,&x,&y,&w,&h);
							      which=xform_do(handle,tree99,NULL,0,0,NULLFUNC,&x,&y,&w,&h) & 0x7ff;
							      OB_STATE(tree99,which) &= ~SELECTED;
										(*server)(2,npos);
										xform_dial(tree,XFMD_DIALOG,&handle,wnx,wny,wnw,wnh);
										which=0;
/* xform_do(handle,tree,NULL,0,1,NULLFUNC,wnx,wny,wnw,wnh); */
					  			}
					  	}
			  	}

				if (which & MU_MESAG)
					{	switch (formrun_msgbuf[0])
							{	case	WM_CLOSED :	cont=0;	cflag=TRUE;
																	break;
								case	WM_NEWTOP	:
								case	WM_TOPPED	:	wind_set(formrun_msgbuf[3],WF_TOP);
																	break;

/*							case	WM_UNTOPPED:if (handle==formrun_msgbuf[3])
																		objc_edit(tree,edit_object,0,&idx,ED_END);
																	break;
								case	WM_ONTOP	:	if (handle==formrun_msgbuf[3])
																		objc_edit(tree,edit_object,0,&idx,ED_INIT);
																	break;
*/
								case	WM_REDRAW	:	wind_update(BEG_UPDATE);
																	wind_update(BEG_MCTRL);
																	thandle=formrun_msgbuf[3];
																	if (edit_object!=0)
																		objc_edit(tree,edit_object,0,&idx,ED_END);
																	wind_get(0,WF_WORKXYWH,&deskx,&desky,&deskw,&deskh);

																	wind_get(thandle,WF_FIRSTXYWH,&nx,&ny,&nw,&nh);
																	while (nw!=0 && nh!=0)
																		{	/* if (rc_intersect(&deskx,&desky,&deskw,&deskh,&nx,&ny,&nw,&nh)) */
																				if (rc_intersect(&formrun_msgbuf[4],&formrun_msgbuf[5],&formrun_msgbuf[6],&formrun_msgbuf[7],&nx,&ny,&nw,&nh))
																					objc_draw(tree,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																			wind_get(thandle,WF_NEXTXYWH,&nx,&ny,&nw,&nh);
																		}
																	if (edit_object!=0)
																		objc_edit(tree,edit_object,0,&idx,ED_INIT);
																	wind_update(END_MCTRL);
																	wind_update(END_UPDATE);
																	break;

								case	WM_FULLED	:	if (tree==tree20 || tree==tree1)
																		{	next_object = 1234;
																			cont = 0;
																		}
																	else
																		{	thandle=formrun_msgbuf[3];
																			wind_get(0,WF_WORKXYWH,&deskx,&desky,&deskw,&deskh);
																			*wnx = (deskw - *wnw) / 2 + deskx;
																			*wny = (deskh - *wnh) / 2 + desky;
																			wind_calc(WC_WORK,NAME|CLOSER|MOVER|FULLER,*wnx,*wny,*wnw,*wnh,&nx,&ny,&d,&d);
																			OB_X(tree,0)=nx;	OB_Y(tree,0)=ny;

																			nw=OB_W(tree,0);	nh=OB_H(tree,0);
																			wind_calc(WC_BORDER,NAME|CLOSER|MOVER|FULLER,nx,ny,nw,nh,wnx,wny,wnw,wnh);

																			wind_set(thandle,WF_CURRXYWH,*wnx,*wny,*wnw,*wnh);
																			if (divflags & D_USESMART)
																				{	pipe[0] = WM_REDRAW;
																					pipe[3] = thandle;
																					pipe[4] = nx;
																					pipe[5] = ny;
																					pipe[6] = nw;
																					pipe[7] = nh;
																					appl_write(ap_id,16,pipe);
																				}
																		}
																	break;
								case	WM_MOVED	:	thandle=formrun_msgbuf[3];
																	*wnx=formrun_msgbuf[4];	*wny=formrun_msgbuf[5];
																	*wnw=formrun_msgbuf[6];	*wnh=formrun_msgbuf[7];
																	wind_calc(WC_WORK,NAME|CLOSER|MOVER,*wnx,*wny,*wnw,*wnh,&nx,&ny,&nw,&nh);
																	OB_X(tree,0)=nx;	OB_Y(tree,0)=ny;
																	wind_set(thandle,WF_CURRXYWH,*wnx,*wny,*wnw,*wnh);
																	if (divflags & D_USESMART)
																		{	pipe[0] = WM_REDRAW;
																			pipe[3] = thandle;
																			pipe[4] = nx;
																			pipe[5] = ny;
																			pipe[6] = nw;
																			pipe[7] = nh;
																			appl_write(ap_id,16,pipe);
																		}
																	break;
								case	MN_SELECTED:next_object = formrun_msgbuf[4];
																	menu_tnormal(menutree,formrun_msgbuf[3],1);
																	cont = 0;
																	mflag = TRUE;
																	break;
								case	FIDO_MSG	:	next_object = formrun_msgbuf[6];
																	cont = 0;
																	fflag = TRUE;
																	break;
							}
					}

			  if ((which & MU_KEYBD) &&
			  		(handle==tophandle) )
			  	{	d = -1;
			  		if (trans_key!=NULLFUNC)
    		  		{	d = (*trans_key)(ks,kr);
      					if (d!=-1)
      						{	next_object = d;
      							cont = 0;
		      				}
    		  		}

  	      	if (d==-1)
	  	      	{	cont = form_keybd(tree,edit_object,next_object,kr,&next_object,&d);
								if (edit_object==next_object)
				    			{	if (kr==CCUR_DOWN || kr==TAB)
					    				next_object=find_object(tree,0,FMD_FORWARD);
					    			if (kr==CCUR_UP)
					    				{	for (mx=0; !(tree[mx].ob_flags & LASTOB); mx++);
							    				next_object=find_object(tree,mx+1,FMD_BACKWARD);
							    			if (next_object==mx+1)
							    				next_object=edit_object;
					    				}
				    			}
	  	      		kr=d;
		            if (kr)
									objc_edit(tree,edit_object,kr,&idx,ED_CHAR);
  	      		}
	        }
				if (which & MU_TIMER)
					{	cont = 0;
						tflag = TRUE;
					}
        if (which & MU_BUTTON && (handle==tophandle))
					{	next_object = objc_find(tree,0,MAX_DEPTH,mx,my);
            if (next_object==-1)
            	{ Bconout(2,7);
                next_object = 0;
	            }
            else
              {	cont = form_button(tree,next_object,br,&next_object);
								if (br==2)
									{	if ((d=objc_find(tree,ROOT,MAX_DEPTH,mx,my))!=-1)
											if (OB_FLAGS(tree,d) & TOUCHEXIT)
												dflag=TRUE;
									}
							}
  	      }
        if (!cont || (next_object!=0) && (next_object!=edit_object))
					objc_edit(tree,edit_object,0,&idx,ED_END);
    	}
    if (dflag==TRUE)	next_object |= 0x8000;
    if (cflag==TRUE)	next_object |= 0x4000;
    if (mflag==TRUE)	next_object |= 0x2000;
    if (tflag==TRUE)	next_object |= 0x1000;
    if (fflag==TRUE)	next_object |= 0x0800;
    return next_object;
	}

void xredraw_slider(OBJECT *tree,int handle,int sliderbox,int back,int front,long max,long count,int page,int redraw)
	{	int ret;
		if ( max>0 )
			{	ret = (int) (((long)page*OB_W(tree,back)) / (max+page)) ;
				if (ret<8)
					ret=8;
				OB_W(tree,front) = ret;
				while (max>2000000L)
				{	max>>=1;
					count>>=1;
				}
				OB_X(tree,front) = (int) ((count-1)*(OB_W(tree,back)-ret)/max);
			}
		else
			{	OB_W(tree,front) = OB_W(tree,back);
				OB_X(tree,front) = 0;
			}
		if (redraw==TRUE)
			redraw_one_objc(tree,sliderbox,handle);
	}

void redraw_objc(OBJECT *tree, int index, int handle)
	{	int rx,ry,nx,ny,nw,nh;
		objc_offset(tree,index,&rx,&ry);
		wind_update(BEG_UPDATE);
		if (handle==-1)
			objc_draw(tree,ROOT,MAX_DEPTH,rx,ry,OB_W(tree,index),OB_H(tree,index));
		else
			{	wind_get(handle,WF_FIRSTXYWH,&nx,&ny,&nw,&nh);
				while (nw!=0 && nh!=0)
					{	if (rc_intersect(&rx,&ry,&OB_W(tree,index),&OB_H(tree,index),&nx,&ny,&nw,&nh))
							objc_draw(tree,ROOT,MAX_DEPTH,nx,ny,nw,nh);
						wind_get(handle,WF_NEXTXYWH,&nx,&ny,&nw,&nh);
					}
			}
		wind_update(END_UPDATE);
	}

void redraw_one_objc(OBJECT *tree, int index, int handle)
	{	int rx,ry,nx,ny,nw,nh;
		objc_offset(tree,index,&rx,&ry);
		wind_update(BEG_UPDATE);
		if (handle==-1)
			objc_draw(tree,index,MAX_DEPTH,rx,ry,OB_W(tree,index),OB_H(tree,index));
		else
			{	wind_get(handle,WF_FIRSTXYWH,&nx,&ny,&nw,&nh);
				while (nw!=0 && nh!=0)
					{	if (rc_intersect(&rx,&ry,&OB_W(tree,index),&OB_H(tree,index),&nx,&ny,&nw,&nh))
							objc_draw(tree,index,MAX_DEPTH,nx,ny,nw,nh);
						wind_get(handle,WF_NEXTXYWH,&nx,&ny,&nw,&nh);
					}
			}
		wind_update(END_UPDATE);
	}

#pragma warn -use
static char copyright[]=" WindowDialog-Library V1.3 (c) Stephan Slabihoud ";
