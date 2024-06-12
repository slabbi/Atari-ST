#include "ccport.h"
#include "ledscan.h"
#include "ledgl.h"
#include "ledicons.rh"
#include "ledfc.h"

word handle_kbd_iconify(long win,word convkey,word key,word shf)
{	long i;
	word p[4];
	if (key==0x3900 && !wi[win].iconify)
	{	if (shf==0x0c || shf==0x0e)
		{	for (i=(shf==0x0c?win:0);i<=(shf==0x0c?win:TOTAL_WIND-1);i++)
			{	if (wi[i].belegt && !wi[i].iconify)
				{	wi[i].xb=wi[i].x;	wi[i].yb=wi[i].y;
					wi[i].wb=wi[i].w;	wi[i].hb=wi[i].h;
					wi[i].iconify=1;
					wind_close(wi[i].handle);
					wind_set(wi[i].handle,WF_ICONIFY,-1,-1,-1,-1);
					wind_open(wi[i].handle,-1,-1,-1,-1);
				}
			}
		}
		return(TRUE);
	}
	if (key==0x3900 && wi[win].iconify)
	{	if (shf==0x0c || shf==0x0e)
		{	for (i=(shf==0x0c?win:0);i<=(shf==0x0c?win:TOTAL_WIND-1);i++)
			{	if (wi[i].belegt && wi[i].iconify)
				{	wind_calc(WC_BORDER,wi[i].gadgets,
												wi[i].xb,wi[i].yb,wi[i].wb,wi[i].hb,
												&p[0],&p[1],&p[2],&p[3]);
					wind_set(wi[i].handle,WF_UNICONIFY,p[0],p[1],p[2],p[3]);
					wi[i].iconify=0;
				}
			}
		}
		return(TRUE);
	}
	if (wi[win].iconify && convkey!=CNTRL_Q &&
												 convkey!=CNTRL_W &&
												 convkey!=CNTRL_U)
		return(TRUE);
	return(FALSE);
}

void redraw_iconify(long win)
{	word i,xy[4];
	if (wi[win].listwind==WI_LIST_AREAS)
		i=ICONAREA;
	else if (wi[win].listwind==WI_LIST_MSGS)
		i=ICONMSGS;
	else if (wi[win].editierbar)
		i=ICONEDIT;
	else
		i=ICONMSG;
	get_wiwork(win);
	xy[0]=wi[win].x;
	xy[1]=wi[win].y;
	xy[2]=xy[0]+wi[win].w-1;
	xy[3]=xy[1]+wi[win].h-1;
	vr_recfl(handle,xy);
	xy[0]+=((wi[win].w-icons_tree[i].ob_width )>>1)-1;
	xy[1]+=((wi[win].h-icons_tree[i].ob_height)>>1)-1;
	icons_tree[i].ob_x=xy[0];
	icons_tree[i].ob_y=xy[1];
	ObjcDraw(icons_tree,i,MAX_DEPTH,
													xy[0], xy[1],
													icons_tree[i].ob_width,
													icons_tree[i].ob_height);
#if 0
	get_wiwork(win);
	xy[0]=wi[win].x;
	xy[1]=wi[win].y;
	xy[2]=xy[0]+wi[win].w-1;
	xy[3]=xy[1]+wi[win].h-1;
	vr_recfl(handle,xy);
	xy[0]+=((wi[win].w-64)>>1)-1;
	xy[1]+=((wi[win].h-36)>>1)-1;
	draw_led(xy[0],xy[1],TRUE);
#endif
}
