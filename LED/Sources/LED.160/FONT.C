#include "ccport.h"
#include <time.h>
#include "font.rh"
#include "ledgl.h"
#include "ledfc.h"

void set_pointsize(long win, word point)
{	
	word mode,red;
	if (wi[win].editierbar)
		mode=1;
	else if (wi[win].listwind==WI_LIST_MSGS)
		mode=2;
	else if (wi[win].listwind==WI_LIST_AREAS)
		mode=3;
	else
		mode=0;

	for (win=0;win<TOTAL_WIND;win++)
	{	if (wi[win].belegt)
		{	red=FALSE;
			if (wi[win].editierbar)
			{	if (mode==1) { opt.editpt=point;	red=TRUE; }
			}
			else if (wi[win].listwind==WI_LIST_MSGS)
			{	if (mode==2) { opt.mlistpt=point; red=TRUE; }
			}
			else if (wi[win].listwind==WI_LIST_AREAS)
			{	if (mode==3) { opt.alistpt=point; red=TRUE; }
			}
			else
			{	if (mode==0) { opt.msgpt=point; red=TRUE; }
			}
			if (red)
			{	set_wifont(win);
				wisetfiles(win,TRUE,NO_CHANGE);
			}
		}
	}
}

void set_msgfnt(void)
{	int ret_point;
	if (opt.msgid>=1)
		vst_font(handle,opt.msgid);
	if (opt.msgpt>=1)
		ret_point=vst_point(handle,opt.msgpt,&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
	gl_pointsize=ret_point;
}

void set_editfnt(void)
{	int ret_point;
	if (opt.editid>=1)
		vst_font(handle,opt.editid);
	if (opt.editpt>=1)
		ret_point=vst_point(handle,opt.editpt,&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
/*	gl_pointsize=opt.editpt; */
	gl_pointsize=ret_point;
}

void set_mlistfnt(void)
{	int ret_point;
	if (opt.mlistid>=1)
		vst_font(handle,opt.mlistid);
	if (opt.mlistpt>=1)
		ret_point=vst_point(handle,opt.mlistpt,&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
/*	gl_pointsize=opt.mlistpt; */
	gl_pointsize=ret_point;
}

void set_alistfnt(void)
{	int ret_point;
	if (opt.alistid>=1)
		vst_font(handle,opt.alistid);
	if (opt.alistpt>=1)
		ret_point=vst_point(handle,opt.alistpt,&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
/*	gl_pointsize=opt.alistpt; */
	gl_pointsize=ret_point;
}

void set_wifont(long win)
{	WIND *winp;
	winp=&wi[win];
	if (winp->editierbar)
		set_editfnt();
	else if (winp->listwind==WI_LIST_MSGS)
		set_mlistfnt();
	else if (winp->listwind==WI_LIST_AREAS)
		set_alistfnt();
	else
		set_msgfnt();
	winp->bzeilen = (winp->h-gl_hbox+gl_hchar) / gl_hbox;  /* 20, 40, 54 */
}

void load_fonts(void)  
{	word i,p/*,c1,c2*/,count;
	word ret_name, ret_point, dummy;
	word pti;
	byte fname[32];

/*FONTWORK fw;		ssl 100195: so ginge es mit den Flydials, aber zu langsam!
	fw.handle=handle;
	fw.loaded=0;
	fw.sysfonts=work_out[10];
	FontLoad (&fw);
	i=FontGetList (&fw);
	FontUnLoad (&fw);
*/

	if (vq_gdos())	/* GDOS geladen */
		gl_fntzahl=vst_load_fonts(handle,0);
	else
		gl_fntzahl=0;	/* gibt nur Systemfont */
	
	count=0;
	for (i=0;i<=gl_fntzahl;i++)
	{ 
		ret_name=vqt_name(handle,i+1,fname);

		if (count<MAX_FONTS)
		{
			strncpy(gl_font[count].fname,fname,24);
			gl_font[count].fname[24]=EOS;
			gl_font[count].fid=ret_name;
		
			for (p=0;p<MAX_PNTSIZE;p++)
				gl_font[count].fpoint[p]=0;

			vst_font(handle,ret_name);
			pti=0;
			for (p=1;p<50;p++)
			{	ret_point=vst_point(handle,p,&dummy,&dummy,&dummy,&dummy);
				if (ret_point==p)
				{	gl_font[count].fpoint[pti]=(byte)ret_point;
					if (pti<MAX_PNTSIZE-2)		/* Letzter Entrag ist immer 0! */
						pti++;				
				}
			}
			count++;
		}
	}
	gl_fntzahl=count-1;

	if (vq_gdos())
		vst_font(handle,1);
}


word male_point(word obj_index, word ft_index, word anfpt, word redraw)
{	byte spt[10];
	word i, k;

	k=anfpt;
	for (k=anfpt;k>0;k--)
	{
		for (i=0;i<MAX_PNTSIZE;i++)
		{
			if (gl_font[ft_index].fpoint[i]==k)
			{	sprintf(spt,"%2d",gl_font[ft_index].fpoint[i]);
				set_dbutton(font_tree,obj_index,spt,redraw);
				return i;
			}
		}
	}	
	sprintf(spt,"%2d",gl_font[ft_index].fpoint[0]);
	set_dbutton(font_tree,obj_index,spt,redraw);
	return(0); /* ssl 71294 */
}

word dia_font(word *msg_index,word *msg_pt,word *ed_index,word *ed_pt,
							word *mlist_index,word *mlist_pt,word *alist_index,word *alist_pt)
{	word ext_btn,c1,c2,dummy;
	word sel_mpt, sel_ept, sel_mlistpt, sel_alistpt;
	word ideal_mpt, ideal_ept, ideal_mlistpt, ideal_alistpt;
	byte tmp1m[25], tmp1e[25], tmp1l[25], tmp1a[25];
	byte tmp2m[10], tmp2e[10], tmp2l[10], tmp2a[10];
	
	ideal_mpt=*msg_pt;
	ideal_ept=*ed_pt;
	ideal_mlistpt=*mlist_pt;
	ideal_alistpt=*alist_pt;
	sprintf(tmp1m,"%-24.24s",gl_font[*msg_index].fname);
		set_dbutton(font_tree,FMNAME,tmp1m,FALSE);
	sprintf(tmp1e,"%-24.24s",gl_font[*ed_index].fname);
		set_dbutton(font_tree,FENAME,tmp1e,FALSE);
	sprintf(tmp1l,"%-24.24s",gl_font[*mlist_index].fname);
		set_dbutton(font_tree,FLNAME,tmp1l,FALSE);
	sprintf(tmp1a,"%-24.24s",gl_font[*alist_index].fname);
		set_dbutton(font_tree,FANAME,tmp1a,FALSE);

	sel_mpt=male_point(FMPT,*msg_index, ideal_mpt, FALSE);
	sel_ept=male_point(FEPT,*ed_index, ideal_ept, FALSE);
	sel_mlistpt=male_point(FLPT,*mlist_index, ideal_mlistpt, FALSE);
	sel_alistpt=male_point(FAPT,*alist_index, ideal_alistpt, FALSE);

	del_state(font_tree,FREVERSE,SELECTED,FALSE);
	del_state(font_tree,FHILIGHT,SELECTED,FALSE);
	del_state(font_tree,FQTHICK,SELECTED,FALSE);
	del_state(font_tree,FQNORM,SELECTED,FALSE);
	if (opt.blockrev)		set_state(font_tree,FREVERSE,SELECTED,FALSE);
	else								set_state(font_tree,FHILIGHT,SELECTED,FALSE);
	if (opt.qthick)			set_state(font_tree,FQTHICK,SELECTED,FALSE);
	else								set_state(font_tree,FQNORM,SELECTED,FALSE);
	if (opt.monospaced)	set_state(font_tree,FMONO,SELECTED,FALSE);
	else								del_state(font_tree,FMONO,SELECTED,FALSE);
	
	draw_dialog(font_tree, &font_dial);
	do
	{
		ext_btn=Form_do(&font_dial,0);
		opt.monospaced=req_state(font_tree,FMONO,SELECTED);
		switch (ext_btn)
		{
			case FMNAME:
				*msg_index=-1;
			case FMRIGHT:
msg_loop:
				(*msg_index)++;
				if (*msg_index>gl_fntzahl)
					*msg_index=0;
				sprintf(tmp1m,"%-24.24s",gl_font[*msg_index].fname);
				set_dbutton(font_tree,FMNAME,tmp1m,TRUE);
				if (opt.monospaced)
				{	vst_font(handle,gl_font[*msg_index].fid);
					vqt_width(handle,'M',&c1,&dummy,&dummy);
					vqt_width(handle,'.',&c2,&dummy,&dummy);
					if (c1!=c2)
						goto msg_loop;
				}
				sel_mpt=male_point(FMPT,*msg_index, ideal_mpt, TRUE);
				break;	
			case FENAME:
				*ed_index=-1;

			case FERIGHT:
ed_loop:
				(*ed_index)++;
				if (*ed_index>gl_fntzahl)
					*ed_index=0;
				sprintf(tmp1e,"%-24.24s",gl_font[*ed_index].fname);
				set_dbutton(font_tree,FENAME,tmp1e,TRUE);
				if (opt.monospaced)
				{	vst_font(handle,gl_font[*ed_index].fid);
					vqt_width(handle,'M',&c1,&dummy,&dummy);
					vqt_width(handle,'.',&c2,&dummy,&dummy);
					if (c1!=c2)
						goto ed_loop;
				}
				sel_ept=male_point(FEPT,*ed_index, ideal_ept, TRUE);
				break;	

			case FLNAME:
				*mlist_index=-1;
			case FLRIGHT:
mlist_loop:
				(*mlist_index)++;
				if (*mlist_index>gl_fntzahl)
					*mlist_index=0;
				sprintf(tmp1l,"%-24.24s",gl_font[*mlist_index].fname);
				set_dbutton(font_tree,FLNAME,tmp1l,TRUE);
				if (opt.monospaced)
				{	vst_font(handle,gl_font[*mlist_index].fid);
					vqt_width(handle,'M',&c1,&dummy,&dummy);
					vqt_width(handle,'.',&c2,&dummy,&dummy);
					if (c1!=c2)
						goto mlist_loop;
				}
				sel_mlistpt=male_point(FLPT,*mlist_index, ideal_mlistpt, TRUE);
				break;	

			case FANAME:
				*alist_index=-1;
			case FARIGHT:
alist_loop:
				(*alist_index)++;
				if (*alist_index>gl_fntzahl)
					*alist_index=0;
				sprintf(tmp1a,"%-24.24s",gl_font[*alist_index].fname);
				set_dbutton(font_tree,FANAME,tmp1a,TRUE);
				if (opt.monospaced)
				{	vst_font(handle,gl_font[*alist_index].fid);
					vqt_width(handle,'M',&c1,&dummy,&dummy);
					vqt_width(handle,'.',&c2,&dummy,&dummy);
					if (c1!=c2)
						goto alist_loop;
				}
				sel_alistpt=male_point(FAPT,*alist_index, ideal_alistpt, TRUE);
				break;	

			case FMPTBUT:
				if (gl_font[*msg_index].fpoint[sel_mpt+1]!=0)
					sel_mpt++;
				else
					sel_mpt=0;
				sprintf(tmp2m,"%2d",gl_font[*msg_index].fpoint[sel_mpt]);
				set_dbutton(font_tree,FMPT,tmp2m,TRUE);
				break;
			case FEPTBUT:
				if (gl_font[*ed_index].fpoint[sel_ept+1]!=0)
					sel_ept++;
				else
					sel_ept=0;
				sprintf(tmp2e,"%2d",gl_font[*ed_index].fpoint[sel_ept]);
				set_dbutton(font_tree,FEPT,tmp2e,TRUE);
				break;	
			case FLPTBUT:
				if (gl_font[*mlist_index].fpoint[sel_mlistpt+1]!=0)
					sel_mlistpt++;
				else
					sel_mlistpt=0;
				sprintf(tmp2l,"%2d",gl_font[*mlist_index].fpoint[sel_mlistpt]);
				set_dbutton(font_tree,FLPT,tmp2l,TRUE);
				break;
			case FAPTBUT:
				if (gl_font[*alist_index].fpoint[sel_alistpt+1]!=0)
					sel_alistpt++;
				else
					sel_alistpt=0;
				sprintf(tmp2a,"%2d",gl_font[*alist_index].fpoint[sel_alistpt]);
				set_dbutton(font_tree,FAPT,tmp2a,TRUE);
				break;	
		}
	} while (ext_btn != FOOK && ext_btn != FOCANCEL);
	del_state(font_tree,ext_btn,SELECTED,FALSE);
	del_dialog(&font_dial);

	if (ext_btn == FOOK)
	{
		get_dbutton(font_tree,FMPT,tmp2m);	*msg_pt=atoi(tmp2m);
		get_dbutton(font_tree,FEPT,tmp2e);	*ed_pt=atoi(tmp2e);
		get_dbutton(font_tree,FLPT,tmp2l);	*mlist_pt=atoi(tmp2l);
		get_dbutton(font_tree,FAPT,tmp2a);	*alist_pt=atoi(tmp2a);
		opt.blockrev=req_state(font_tree,FREVERSE,SELECTED);
		opt.qthick=req_state(font_tree,FQTHICK,SELECTED);
		return TRUE;
	}	
	else
		return FALSE;	
}

word get_fontindex(word fid)
{	word i, index;
	index=0;
	for (i=0;i<MAX_FONTS;i++)  /* ermittle Anfangsindex */
	{	if (i<=gl_fntzahl)
			if (fid==gl_font[i].fid)
			{	index=i;
				break;
			}	
	}
	return (index);
}	
