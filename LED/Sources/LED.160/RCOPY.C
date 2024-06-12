#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"

MFDB src, dst;

word *setxy(word pxy[], word x1, word y1, word x2, word y2)
{
	pxy[0]  = x1;          /* linke obere Ecke */
	pxy[1]  = y1;
	pxy[2]  = x2;          /* rechte untere Ecke */
	pxy[3]  = y2;
	
	return pxy;
}

void rcopy(word xs, word ys, word ws, word hs, word xd, word yd, word mode)
{	word pxy[8];

	src.fd_addr = dst.fd_addr = NULL;
	setxy(&pxy[0], xs, ys, xs+ws, ys+hs);
	setxy(&pxy[4], xd, yd, xd+ws, yd+hs);
	vro_cpyfm(handle, mode, pxy, &src, &dst);
}

void scr_up(long win)
{	word pxy[4];
	word zeilen,line;
	word ytop,y,x2,y2,h,hdiff;
	WIND *winp;
	
	winp=&wi[win];
	if (winp->x+winp->w > gl_x2desk)
		winp->w=gl_x2desk-winp->x;
	if (winp->y+winp->h > gl_y2desk)
		winp->h=gl_y2desk-winp->y;
	hdiff=gl_hbox-gl_hchar;
	zeilen=(winp->h-hdiff)/gl_hbox;
	if (zeilen-winp->topmsg_line >= 1)		/* war >1 */
	{
		line=zeilen+winp->topline-winp->topmsg_line;
		ytop=winp->topmsg_line*gl_hbox;
		y=winp->y+hdiff;
		x2=winp->x+winp->w-1;
		y2=winp->y+winp->h-1;
		h=(zeilen-1)*gl_hbox;
		
		if (winp->lines[line]!=NULL)
		{
			winp->topline++;
			
			pxy[0]=winp->x;

			if (zeilen-winp->topmsg_line>1)
			{
				pxy[1]=winp->y+ytop+gl_hbox+hdiff;
				pxy[2]=x2-pxy[0];
				pxy[3]=h-ytop;
				if (pxy[1]+pxy[3]>y2)
					pxy[3]=y2-pxy[1];
				rcopy(pxy[0],pxy[1],pxy[2],pxy[3],pxy[0],pxy[1]-gl_hbox,S_ONLY);
			}
			
			pxy[1]=y+h;
			pxy[2]=x2;
			pxy[3]=y2;
			vr_recfl(handle,pxy);
			
			winp->maxshow = zeilen-winp->topmsg_line;
			if (winp->maxshow < 0)
				winp->maxshow = 0;
			if (winp->lzahl <= winp->maxshow)
				wind_set(winp->handle,WF_VSLIDE,1,0,0,0);
			else 
				wind_set(winp->handle,WF_VSLIDE,
				  (int)((long)winp->topline*1000L/(winp->lzahl-winp->maxshow)),0,0,0);
			
			pxy[0]+=4-winp->firstc*gl_wbox;
			pxy[1]+=gl_hchar;	/* gl_hbox-hdiff = gl_hchar */
/*		myv_gtext(pxy[0],pxy[1],line,winp->lines[line],winp->lines[line],FALSE); */
			if (winp->listwind)
				printline(win,line,FALSE);
			else
				effect_v_gtext(win,pxy[0],pxy[1],line,winp->lines[line]);
			line++;
			pxy[1]+=gl_hbox;
/*		myv_gtext(pxy[0],pxy[1],line,winp->lines[line],winp->lines[line],FALSE); */
			if (winp->listwind)
				printline(win,line,FALSE);
			else
				effect_v_gtext(win,pxy[0],pxy[1],line,winp->lines[line]);
		}
	}
}

void scr_down(long win)
{ word pxy[4];
	word zeilen;
	word ytop,x2,y2,h,hdiff;
	WIND *winp;
	
	winp=&wi[win];
	if (winp->x+winp->w > gl_x2desk)
		winp->w=gl_x2desk-winp->x;
	if (winp->y+winp->h > gl_y2desk)
		winp->h=gl_y2desk-winp->y;
	hdiff=gl_hbox-gl_hchar;
	zeilen=(winp->h-hdiff)/gl_hbox;
	if (zeilen-winp->topmsg_line >= 1)		/* war >1 */
	{
		ytop=winp->topmsg_line*gl_hbox;
		x2=winp->x+winp->w-1;
		y2=winp->y+winp->h-1;
		h=zeilen*gl_hbox;
		
		if (winp->topline>0)
		{
			winp->topline--;
			
			pxy[0]=winp->x;
			pxy[1]=winp->y+ytop+hdiff;

			if (zeilen-winp->topmsg_line>1)
			{
				pxy[2]=x2-pxy[0];
				pxy[3]=h-ytop;
				h=pxy[1]+gl_hbox+pxy[3];
				if (h>y2)
					pxy[3]-=(h-y2);
				rcopy(pxy[0],pxy[1],pxy[2],pxy[3],pxy[0],pxy[1]+gl_hbox,S_ONLY);
			}
	
			pxy[2]=x2;
			pxy[3]=pxy[1]+gl_hbox-1;
			vr_recfl(handle,pxy);
			
			winp->maxshow = zeilen-winp->topmsg_line;
			if (winp->maxshow < 0)
				winp->maxshow = 0;
			if (winp->lzahl <= winp->maxshow)
				wind_set(winp->handle,WF_VSLIDE,1,0,0,0);
			else 
				wind_set(winp->handle,WF_VSLIDE,
				  (int)((long)winp->topline*1000L/(winp->lzahl-winp->maxshow)),0,0,0);
			
			pxy[0]+=4-winp->firstc*gl_wbox;
			pxy[1]+=gl_hchar;

/*		myv_gtext(pxy[0],pxy[1],winp->topline,winp->lines[winp->topline],winp->lines[winp->topline],FALSE); */
			if (winp->listwind)
				printline(win,winp->topline,FALSE);
			else
				effect_v_gtext(win,pxy[0],pxy[1],winp->topline,winp->lines[winp->topline]);
		}
	}
}

void lines_up(long win, word lnr)
{
	word y,h;
	
	y=lnr*gl_hbox+wi[win].y+gl_hbox-gl_hchar;
	h=wi[win].y+wi[win].h-y-1;
	rcopy(wi[win].x, y, wi[win].w, h, wi[win].x, y-gl_hbox, S_ONLY);
	pline(win,wi[win].bzeilen-1,wi[win].bzeilen,TRUE,FALSE);
}

void lines_down(long win, word lnr)
{
	word y,h;
	
	y=lnr*gl_hbox+wi[win].y+gl_hbox-gl_hchar;
	h=wi[win].y+wi[win].h-y-gl_hbox-1;
	if (h>=gl_hbox)
		rcopy(wi[win].x, y, wi[win].w, h, wi[win].x, y+gl_hbox, S_ONLY);
	if (lnr==wi[win].bzeilen-1)
		pline(win,lnr,lnr+1,TRUE,FALSE);
	else
		pline(win,lnr,lnr,TRUE,FALSE);
}
