#include <ccport.h>
#include "thanxbox.rh"
#include "ledgl.h"
#include "ledfc.h"

static cripple_counter=0;

void draw(word xy[],word offx,word offy)
{	word i,pxy[4],deltax,deltay;
	deltax=xy[3]-xy[1];
	deltay=xy[4]-xy[2];
	for (i=1; i<=16; i++)
	{	pxy[0]=xy[1]+offx;
		pxy[1]=xy[2]+offy;
		pxy[2]=xy[1]+offx+ (i*deltax)/16 ;
		pxy[3]=xy[2]+offy+ (i*deltay)/16 ;
		hide_mouse();
		if (xy[0])
		{	v_pline(handle,2,pxy);
			pxy[0]++; pxy[1]++;	pxy[2]++; pxy[3]++;
			v_pline(handle,2,pxy);
			pxy[0]+=2; pxy[1]--;	pxy[2]+=2; pxy[3]--;
			v_pline(handle,2,pxy);
		}
		show_mouse();
		v_dspcur(handle,pxy[2],pxy[3]);
		evnt_timer(40,0);
	}
}

void drawf(word xy[],word offx,word offy)
{	word pxy[4];
	pxy[0]=xy[1]+offx;
	pxy[1]=xy[2]+offy;
	pxy[2]=xy[3]+offx;
	pxy[3]=xy[4]+offy;
	if (xy[0])
	{	v_pline(handle,2,pxy);
		pxy[0]++; pxy[1]++;	pxy[2]++; pxy[3]++;
		v_pline(handle,2,pxy);
		pxy[0]+=2; pxy[1]--;	pxy[2]+=2; pxy[3]--;
		v_pline(handle,2,pxy);
	}
}

void draw_led(word x,word y,word fast)
{	word i;
	word pled[][5] =
	{	{ 1,0,0,0,32		},	{ 1,0,32,16,32	},	{ 0,16,32,24,0	},
		{ 1,24,0,24,32	},	{	0,24,32,24,0	},	{	1,24,0,40,0		},
		{	0,40,0,24,16	},	{	1,24,16,32,16	},	{	0,32,16,24,32	},
		{	1,24,32,40,32	},	{	0,40,32,48,0	},	{	1,48,0,48,32	},
		{	0,48,32,48,0	},	{	1,48,0,56,0		},	{	1,56,0,64,8		},
		{	1,64,8,64,24	},	{	1,64,24,56,32	},	{	1,56,32,48,32	},
		{	0,48,32,0,36	},	{	1,0,36,64,36	},	{	-1,-1,-1,-1,-1} };
	i=0;
	while (pled[i][0]!=-1)
	{	if (!fast)
			draw(&pled[i++][0],x,y);
		else
			drawf(&pled[i++][0],x,y);
	}
}

void thanks(void)
{	word i,offx,offy,oldx,oldy,clip[4];

	test_registered();
	if (!is_registered)
	{	
		mouse_normal();
		show_mouse();
		WindUpdate(BEG_UPDATE);
		WindUpdate(BEG_MCTRL);
		draw_dialog(thanks_tree, &thanks_dial);
		vq_mouse(handle,&i,&oldx,&oldy);
		ObjcOffset(thanks_tree,THBOX,&offx,&offy);
		ObjcOffset(thanks_tree,0,&clip[0],&clip[1]);
		vswr_mode(handle,MD_REPLACE);
		vsl_color(handle,BLACK);
		clip[2]=clip[0]+thanks_tree[0].ob_width;
		clip[3]=clip[1]+thanks_tree[0].ob_height;
		vs_clip(handle,1,clip);
		draw_led(offx,offy,FALSE);
		v_dspcur(handle,oldx,oldy);
		vs_clip(handle,0,clip);
		evnt_timer(5000,0);
		del_dialog(&thanks_dial);
		WindUpdate(END_MCTRL);
		WindUpdate(END_UPDATE);
	}
}

word cripple(long win)
{	word pxy[4],i;
	if (win>=0 && !wi[win].editierbar)
	{	
		set_wiclip(win);
		set_wifont(win);

		wind_update(BEG_UPDATE);
		hide_mouse();
  	vsf_color(handle,BLACK);
  	vst_color(handle,WHITE);
  	vswr_mode(handle,MD_TRANS);

		wind_get(wi[win].handle,WF_FIRSTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		while (pxy[2] && pxy[3])
		{
 	 		pxy[2] += pxy[0]-1;
		  pxy[3] += pxy[1]-1;
	  	vs_clip(handle,TRUE,pxy);
			vr_recfl(handle,pxy);
			for (i=0;i<=wi[win].h;i+=gl_hbox)
		  	v_gtext(handle,wi[win].x+(i >> 1),gl_hbox+wi[win].y+i,"Please register!");
		  wind_get(wi[win].handle,WF_NEXTXYWH,&pxy[0],&pxy[1],&pxy[2],&pxy[3]);
		}
  	vswr_mode(handle,MD_REPLACE);
  	vsf_color(handle,WHITE);
	 	vst_color(handle,BLACK);
		show_mouse();
		evnt_timer(10000,0);
		wind_update(END_UPDATE);
		set_wiclip(win);
		set_wifont(win);

		wisetfiles(win,TRUE,NO_CHANGE);
		cripple_counter++;
	}
	if (cripple_counter>=16)
		return(TRUE);
	return(FALSE);
}
