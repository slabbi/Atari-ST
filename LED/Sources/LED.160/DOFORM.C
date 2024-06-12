#include "ccport.h"
#include "allareas.rh"
#include "listmsg.rh"
#include "ledscan.h"
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"

word male_seite(word seite, word redraw)
{	word i;
	word sindex;
	byte stmp[22];
	byte tmp[4];
	
	sindex=seite*AREAS_SEITE;
	
	biene();
	tmp[3]=EOS;
	for (i=0;i<AREAS_SEITE;i++)
	{
		if (i+sindex<gl_arzahl)
		{
			sprintf(stmp,"%-12.12s",gl_area[i+sindex].name);
			set_dtext(allarea_tree,gl_area_obj[i],stmp,FALSE,12);
			
			strcpy(tmp,"   ");
			if (gl_area[i+sindex].newmsg & NEWMSG)	tmp[0]='*';
			if (gl_area[i+sindex].newmsg & UNRDMSG)	tmp[1]='/';
			if (gl_area[i+sindex].newmsg & TOSYSOP)	tmp[2]='#';
			set_dbutton(allarea_tree,gl_arnew_obj[i],tmp,FALSE);
		}
		else
		{
			set_dtext(allarea_tree,gl_area_obj[i],"            ",FALSE,12);
			set_dbutton(allarea_tree,gl_arnew_obj[i],"   ",FALSE);
		}	
 	}
	if (redraw)
	{
		objc_draw(allarea_tree, ALLARNME, MAX_DEPTH, 
		          allarea_tree->ob_x, allarea_tree->ob_y, 
		          allarea_tree->ob_width, allarea_tree->ob_height);
	}
	
	if (gl_arzahl>AREAS_SEITE)
 	{
		set_flag(allarea_tree,AASIDE1,HIDETREE,FALSE);
		set_flag(allarea_tree,AASIDE2,HIDETREE,FALSE);
		if (sindex>0)
			del_flag(allarea_tree,AASIDE1,HIDETREE,FALSE);
		if (sindex+AREAS_SEITE<gl_arzahl)
			del_flag(allarea_tree,AASIDE2,HIDETREE,FALSE);
	}
	if (redraw)
		ObjcDraw(allarea_tree,AASIDEB,1,allarea_tree->ob_x,allarea_tree->ob_y,allarea_tree->ob_width,allarea_tree->ob_height);

	mouse_normal();
	if (sindex+AREAS_SEITE<gl_arzahl)
		return (AREAS_SEITE-1);
	else
		return (gl_arzahl-sindex-1);
}

void show_side(word side, word *oldside, word *area, word *max_sel)
{
	if (gl_arzahl>AREAS_SEITE)
	{
		if (side!=*oldside)
		{	del_state(allarea_tree,gl_area_obj[*area],SELECTED,TRUE);
			*oldside=side;
			*max_sel=male_seite(side,TRUE);
			*area=0;
			set_state(allarea_tree,gl_area_obj[*area],SELECTED,TRUE);
		}
	}
}

void cut_arname(byte *name)
{
	byte *cp, *start;
	
	start=name;
	if (strlen(name) > 12L)
		name[12]=EOS;
	else
	{	
		cp=strchr(name,EOS);
		while(cp-start < 12)
			*cp++=' ';
		*cp=EOS;
	}	
}

void select_side(word *sel_area, word *seite, word *max_sel)
{
	word s;
	word area;
	word arzahl;
	
	s=0;
	area=*sel_area;
	arzahl=gl_arzahl;
	
	if (area>arzahl-1)
		area=arzahl-1;
	while (area>AREAS_SEITE-1)
	{
		area-=AREAS_SEITE;
		arzahl-=AREAS_SEITE;
		s++;
	}
	
	*seite=s;
	*sel_area=area;
	if (arzahl>AREAS_SEITE-1)
		*max_sel=AREAS_SEITE-1;
	else
		*max_sel=arzahl-1;
}

word find_keyarea(word key, word sel_area)
{	word i;

	if (++sel_area>gl_arzahl)
		sel_area=0;

	key=toupper((byte)key);
	for (i=sel_area;i<gl_arzahl;i++)
		if (toupper(gl_area[i].name[0])==key)
			return i;
	for (i=0;i<gl_arzahl;i++)
		if (toupper(gl_area[i].name[0])==key)
			return i;
	return -1;
}

word do_aform(word *sel_anf, word drawdia)
{	word fm_edit_obj;
	word fm_next_obj;
	word fm_which;
	word fm_cont;
	word fm_idx;
	word fm_mx, fm_my, fm_mb, fm_ks, fm_kr, fm_br;
	word fm_msg[8];
	word i;
	word seite;
	word max_sel;
	word sel_area;
	word ret_area;
	word button_fl;
	
	sel_area=*sel_anf;
	select_side(&sel_area,&seite,&max_sel);
	
	/* zum testen */
	set_flag(allarea_tree,3,TOUCHEXIT,FALSE);
	
	if (drawdia)
	{
		male_seite(seite,FALSE);
		draw_dialog(allarea_tree, &allarea_dial);
	}
	
	wind_update(BEG_MCTRL);
	
	if (!req_flag(allarea_tree,AMOVEANF,HIDETREE))
		fm_next_obj = AMOVEANF;
	else
	{
		fm_next_obj = gl_area_obj[sel_area];
		set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
	}
	fm_edit_obj = 0;
	fm_cont = TRUE;
	
	while (fm_cont)
	{
		if (fm_next_obj!=0 && fm_edit_obj!=fm_next_obj && 
		    req_flag(allarea_tree,fm_next_obj,EDITABLE))
		{
			fm_edit_obj=fm_next_obj;
			fm_next_obj=0;
			objc_edit(allarea_tree,fm_edit_obj,0,&fm_idx,ED_INIT);
		}
		
		fm_which = evnt_multi(MU_KEYBD|MU_BUTTON|MU_MESAG,
		                      0x01,0x01,0x01,
		                      0,0,0,0,0,
		                      0,0,0,0,0,
		                      fm_msg,
		                      0,0,
		                      &fm_mx,&fm_my,&fm_mb,&fm_ks,&fm_kr,&fm_br);
		
		if (fm_which & MU_KEYBD)
		{
			fm_kr=get_code(fm_kr,fm_ks);

			if (fm_kr==CNTRL_RET)
				continue;
		
			if (fm_kr==ESC && req_flag(allarea_tree,fm_edit_obj,EDITABLE))
			{	
				objc_edit(allarea_tree,fm_edit_obj,fm_kr,&fm_idx,ED_END);
				set_dtext(allarea_tree,fm_edit_obj,"",TRUE,4);
				objc_edit(allarea_tree,fm_edit_obj,fm_kr,&fm_idx,ED_INIT);
			}
			else if (/*fm_kr==HOME || fm_kr==SHFT_HOME ||*/ fm_kr==TAB)
				fm_next_obj=0;
			else if (fm_kr==ALT_K || fm_kr==ALT_B)
				FormButton(allarea_tree,AMK,fm_br,&fm_next_obj);
			else if (fm_kr==ALT_F || fm_kr==ALT_W)
				FormButton(allarea_tree,AMF,fm_br,&fm_next_obj);
			else if (fm_kr==UNDO || fm_kr==ALT_C || fm_kr==ALT_A)
			{	fm_cont=FALSE;
				fm_next_obj=AMCAN;
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,FALSE);
			}

			else if (req_flag(allarea_tree,fm_edit_obj,EDITABLE))
			{
				if (fm_kr==CUR_DOWN)
				{
					if (fm_edit_obj==AMOVEANF)
						fm_next_obj=AMOVEEND;
					else
					{	sel_area=0;
						fm_next_obj = gl_area_obj[sel_area];
						set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
					}
				}
				else if (fm_kr==CUR_UP)
				{
					if (fm_edit_obj==AMOVEEND)
						fm_next_obj=AMOVEANF;
					else
					{	sel_area=max_sel;	
						fm_next_obj = gl_area_obj[sel_area];
						set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
					}
				}
				else
				{
					fm_cont=FormKeybd(allarea_tree,fm_edit_obj,fm_next_obj,fm_kr,0,&fm_next_obj,&fm_kr);
					if (fm_cont && fm_kr)
						objc_edit(allarea_tree,fm_edit_obj,fm_kr,&fm_idx,ED_CHAR);
				}
			}
			else if (fm_kr==RETURN || fm_kr==ENTER)
			{
				fm_cont=FALSE;
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,FALSE);
			}
			else if (fm_kr==CUR_DOWN || fm_kr==SHFT_CD)
			{
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				if (sel_area<max_sel || req_flag(allarea_tree,AMOVEANF,HIDETREE))
				{
					sel_area++;
					if (sel_area>max_sel)
						sel_area=0;
					fm_next_obj = gl_area_obj[sel_area];
					set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				}
				else
					fm_next_obj=AMOVEANF;
			}
			else if (fm_kr==CUR_UP || fm_kr==SHFT_CU)
			{
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				if (sel_area>0 || req_flag(allarea_tree,AMOVEANF,HIDETREE))
				{
					sel_area--;
					if (sel_area<0)
						sel_area=max_sel;
					fm_next_obj = gl_area_obj[sel_area];
					set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				}
				else
					fm_next_obj=AMOVEEND;
			}
			else if (fm_kr==CUR_RIGHT)
			{
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				sel_area+=15;
				if (sel_area>max_sel)
					sel_area-=(15*((max_sel/15)+1));
				if (sel_area<0)
					sel_area+=15;
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				fm_next_obj = gl_area_obj[sel_area];
			}
			else if (fm_kr==CUR_LEFT)
			{
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				sel_area-=15;
				if (sel_area<0)
					sel_area+=(15*((max_sel/15)+1));
				if (sel_area>max_sel)
					sel_area-=15;
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				fm_next_obj = gl_area_obj[sel_area];
			}
			else if (fm_kr==SHFT_CR) /* to Seite 2 */
			{
				if ((seite+1)*AREAS_SEITE < gl_arzahl)
				{
					show_side(seite+1,&seite,&sel_area,&max_sel);
				}
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (fm_kr==SHFT_CL)  /* to Seite 1 */
			{
				if (seite>0 && gl_arzahl>AREAS_SEITE)
				{
					show_side(seite-1,&seite,&sel_area,&max_sel);
				}
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}     
			else if (fm_kr==HOME)
			{	if (seite>0 && gl_arzahl>AREAS_SEITE)
					show_side(0,&seite,&sel_area,&max_sel);
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				sel_area=0;
				fm_next_obj = gl_area_obj[sel_area];
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (fm_kr==SHFT_HOME)
			{	if ((seite+1)*AREAS_SEITE < gl_arzahl)
					show_side((gl_arzahl-1)/AREAS_SEITE,&seite,&sel_area,&max_sel);
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				sel_area=max_sel;
				fm_next_obj = gl_area_obj[sel_area];
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (isalnum(fm_kr))
			{
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
				ret_area=find_keyarea(fm_kr,sel_area+seite*AREAS_SEITE);
				i=seite;
				if (ret_area!=-1 /*&& ret_area<=max_sel*/ )
				{	sel_area=ret_area;

/*				sel_area=sel_area % AREAS_SEITE;
					seite=ret_area/AREAS_SEITE; */

					select_side(&sel_area,&i,&max_sel);
				}

				if (seite!=i)
				{	seite=i;
					max_sel=male_seite(seite,TRUE);
				}

				fm_next_obj = gl_area_obj[sel_area];
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (fm_cont)
			{
				fm_cont=FormKeybd(allarea_tree,fm_edit_obj,fm_next_obj,fm_kr,0,&fm_next_obj,&fm_kr);
				if (fm_cont && fm_kr && req_flag(allarea_tree,fm_edit_obj,EDITABLE))
					objc_edit(allarea_tree,fm_edit_obj,fm_kr,&fm_idx,ED_CHAR);
			}
		}
		
		if (fm_which & MU_BUTTON)
		{
			fm_next_obj = objc_find(allarea_tree,ROOT,MAX_DEPTH,fm_mx,fm_my);
			button_fl=TRUE;
			if (fm_next_obj == -1)
			{
				Dosound (fm_sound_dta);
				fm_next_obj = 0;
			}
			else if (fm_next_obj==AAFLYBTN)
			{
				button_fl=FALSE;
				graf_mouse(FLAT_HAND,(void *)0L);
				DialMove(&allarea_dial,gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
				mouse_normal();
			}
			else if (fm_next_obj==AASIDE1)
			{
				if (seite>0 && gl_arzahl>AREAS_SEITE)
				{
					show_side(seite-1,&seite,&sel_area,&max_sel);
				}
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (fm_next_obj==AASIDE2)
			{
				if ((seite+1)*AREAS_SEITE < gl_arzahl)
				{
					show_side(seite+1,&seite,&sel_area,&max_sel);
				}
				set_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (fm_next_obj==AMCAN)
			{
				fm_cont=FALSE;
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,FALSE);
			}
			else if (req_flag(allarea_tree,fm_next_obj,EDITABLE))
			{
				del_state(allarea_tree,gl_area_obj[sel_area],SELECTED,TRUE);
			}
			else if (fm_next_obj!=AMK && fm_next_obj!=AMF)
			{
				button_fl=FALSE;
				for (i=0;i<=max_sel;i++)
				{
					if (fm_next_obj==gl_area_obj[i])
					{
						sel_area=i;
						button_fl=TRUE;
						break;
					}
				}
				if (i>max_sel || fm_next_obj!=gl_area_obj[i])
					fm_next_obj=0;
			}
			
			if (button_fl)
				fm_cont = FormButton(allarea_tree,fm_next_obj,fm_br,&fm_next_obj);
		}  /* MU_BUTTON */
			
		if ((!fm_cont || (fm_next_obj!=0 && fm_next_obj!=fm_edit_obj)) && 
		    req_flag(allarea_tree,fm_edit_obj,EDITABLE))
		{
			objc_edit(allarea_tree,fm_edit_obj,0,&fm_idx,ED_END);
			fm_edit_obj=0;
		}
	}  /* while cont */
	
	*sel_anf=sel_area+seite*AREAS_SEITE;
	
	wind_update(END_MCTRL);
	return(fm_next_obj & 0x7fff);
}

void inhalt_listen(long win, word top_nr, word *sel_obj, word redraw)
{	long grauh;
	word i;
	word lmsg, ab_nr;
	uword sldy,sldh,slddiff;
	HDR thdr;
	byte tmp[MAX_STR],owntmp[2];
	
	ab_nr=lmsg=top_nr;

	biene();
	owntmp[1]=EOS;
/*	*sel_obj=0;*/
	hdrseek(win,wi[win].hdrfp,(lmsg-1),SEEK_SET);
	for (i=0;i<MAX_LIST;i++,lmsg++)
	{
		del_state(listmsg_tree,gl_lown_obj[i],SELECTED,FALSE);
		if (lmsg<=wi[win].lastmsg)
		{
			hdrread(win,&thdr,1,wi[win].hdrfp);
			sprintf(tmp,"%4d %-18.18s %-18.18s %-29.29s",lmsg,thdr.sFrom,thdr.sTo,thdr.sSubject);
			
			if (thdr.wFlags & F_DELETED)
				set_state(listmsg_tree,gl_list_obj[i],DISABLED,FALSE);
			else
				del_state(listmsg_tree,gl_list_obj[i],DISABLED,FALSE);
			
			if (lmsg==wi[win].aktmsg || (lmsg==1 && wi[win].aktmsg==0))
			{
				set_state(listmsg_tree,gl_list_obj[i],SELECTED,FALSE);
				*sel_obj=i;
			}
			else
				del_state(listmsg_tree,gl_list_obj[i],SELECTED,FALSE);
			
			tmp[72]=EOS;
			del_flag(listmsg_tree,gl_list_obj[i],HIDETREE,FALSE);
			set_dtext(listmsg_tree,gl_list_obj[i],tmp,FALSE,72);
			
			if (thdr.XFlags & XF_LOCK)
				owntmp[0]='x';
			else if (is_ptowner(thdr.sTo,wi[win].aktarea))
			{
				owntmp[0]='\x3';
				set_state(listmsg_tree,gl_lown_obj[i],SELECTED,FALSE);
			}
			else if (is_ptowner(thdr.sFrom,wi[win].aktarea))
				owntmp[0]='\x4';
			else if (!(thdr.XFlags & XF_READ))
				owntmp[0]='u';
			else
				owntmp[0]=' ';
			set_dtext(listmsg_tree,gl_lown_obj[i],owntmp,FALSE,1);
		}
		else
		{
			set_dtext(listmsg_tree,gl_lown_obj[i]," ",FALSE,1);
			set_flag(listmsg_tree,gl_list_obj[i],HIDETREE,FALSE);
		}
	}
	if (redraw)
	{
		objc_draw(listmsg_tree, LNAMES, MAX_DEPTH, 
		          listmsg_tree->ob_x, listmsg_tree->ob_y, 
		          listmsg_tree->ob_width, listmsg_tree->ob_height);
	}
	
	grauh=(long)((listmsg_tree+LGRAU)->ob_height);
	if (wi[win].lastmsg<=MAX_LIST)
	{
		((listmsg_tree+LSLIDER)->ob_y)=0;
		((listmsg_tree+LSLIDER)->ob_height)=(uword)grauh;
	}
	else
	{
		sldh=(uword)((grauh * MAX_LIST) / wi[win].lastmsg);
		if (sldh<10)
		{
			slddiff=10-sldh;
			sldh=10;
		}
		else
			slddiff=0;
		sldy=(uword)(((grauh-(long)slddiff) * (long)(ab_nr-1)) / wi[win].lastmsg);
		(listmsg_tree+LSLIDER)->ob_y=sldy;
		(listmsg_tree+LSLIDER)->ob_height=sldh;
	}
	
	if (redraw)
	{
		redraw_obj(listmsg_tree,LGRAU);
		redraw_obj(listmsg_tree,LSLIDER);				          		
	}
	mouse_normal();
}

word do_lform(long win, word sel_obj, word top_msg)
{	long sldh, pos;
	word fm_edit_obj;
	word fm_next_obj;
	word fm_which;
	word fm_cont;
	word fm_mx, fm_my, fm_mb, fm_ks, fm_kr, fm_br;
	word fm_msg[8];
	word lmsg;
	word i;
	word yslider, dummy;
	word new_top;
	
	wind_update(BEG_MCTRL);
	fm_next_obj = gl_list_obj[sel_obj];
	fm_edit_obj = 0;
	fm_cont = TRUE;
	
	while (fm_cont)
	{
		fm_which = evnt_multi(MU_KEYBD|MU_BUTTON|MU_MESAG,
		                      0x02,0x01,0x01,
		                      0,0,0,0,0,
		                      0,0,0,0,0,
		                      fm_msg,
		                      0,0,
		                      &fm_mx,&fm_my,&fm_mb,&fm_ks,&fm_kr,&fm_br);
		
		if (fm_which & MU_KEYBD)
		{
			fm_kr=get_code(fm_kr,fm_ks);
			
			if (fm_kr==RETURN || fm_kr==ENTER)
			{	fm_cont = FALSE;
				fm_next_obj=0;
				for (i=0;i<MAX_LIST;i++)
				{	if (req_state(listmsg_tree,gl_list_obj[i],SELECTED))
					{	fm_next_obj=gl_list_obj[i];
						break;
					}
				}
			}
			else if (fm_kr==UNDO || fm_kr==ALT_C || fm_kr==ALT_A)
			{	fm_cont=FALSE;
				fm_next_obj=LICANCEL;
				del_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,FALSE);
			}
			
			if (fm_kr==HOME)
			{
				if (top_msg>=1)
				{	fm_cont = TRUE;
					top_msg=lmsg=1;
					sel_obj=0;
					inhalt_listen(win, lmsg, &sel_obj, TRUE);
					set_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
					fm_next_obj=gl_list_obj[sel_obj];
				}
				throw_evntaway();
			}
			else if (fm_kr == SHFT_HOME)
			{	if ( top_msg < wi[win].lastmsg-(MAX_LIST-1) )
				{ fm_cont = TRUE;
					top_msg=lmsg=wi[win].lastmsg-(MAX_LIST-1);
					if (lmsg<=0)
						top_msg=lmsg=1;
					sel_obj=MAX_LIST-1;
					inhalt_listen(win, lmsg, &sel_obj, TRUE);
					set_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
					fm_next_obj=gl_list_obj[sel_obj];
				}
				throw_evntaway();
			}
			else if (fm_kr==CUR_DOWN)
			{	fm_cont = TRUE;
				/* Ende der Liste -> nix machen  */
				if (sel_obj+1>wi[win].lastmsg || sel_obj+1>wi[win].lastmsg-top_msg)
					continue;
				if (req_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED))
				{
					del_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
					if (sel_obj+1>=MAX_LIST)
					{
						/* Neue korrigierte Version */
						sel_obj=0;
						new_top=top_msg+MAX_LIST;
						if (new_top+MAX_LIST>wi[win].lastmsg)
						{	new_top=wi[win].lastmsg-MAX_LIST;
							sel_obj=MAX_LIST-wi[win].lastmsg+MAX_LIST+top_msg-1;
						}
						fm_kr=SHFT_CD;
					}
					else
						sel_obj++;
				}
				if (fm_kr!=SHFT_CD)
					set_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
				fm_next_obj = gl_list_obj[sel_obj];
			}
			else if (fm_kr == CUR_UP)                
			{	fm_cont = TRUE;
				if (top_msg<=1 && sel_obj<=0)
					continue;
				del_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
				if (sel_obj<=0)
				{	sel_obj=MAX_LIST-1;
					new_top=top_msg-MAX_LIST;
					if (new_top < 1)
						sel_obj=top_msg-2;
					fm_kr=SHFT_CU;	
				}	
				else
				{
					sel_obj--;
				}
				if (fm_kr!=SHFT_CU)
					set_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
				fm_next_obj = gl_list_obj[sel_obj];
			} 
			if (fm_kr==SHFT_CD)
			{	fm_cont=TRUE;
				if (wi[win].lastmsg-wi[win].firstmsg > MAX_LIST-1)
				{	if ( top_msg < wi[win].lastmsg-(MAX_LIST-1) )
					{ lmsg=top_msg+MAX_LIST;
						if (wi[win].lastmsg-lmsg<MAX_LIST)
							lmsg=wi[win].lastmsg-(MAX_LIST-1);
						top_msg=lmsg;
						inhalt_listen(win, lmsg, &sel_obj, TRUE);
					}
				}
				fm_next_obj=gl_list_obj[sel_obj];
				set_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
				throw_evntaway();
			}
			if (fm_kr==SHFT_CU)
			{	fm_cont=TRUE;
				if (wi[win].lastmsg-wi[win].firstmsg>MAX_LIST-1)
				{	if (top_msg>1)
					{	lmsg=top_msg-MAX_LIST;
						if (lmsg<wi[win].firstmsg)
							lmsg=wi[win].firstmsg;
						top_msg=lmsg;	
						inhalt_listen(win, lmsg, &sel_obj, TRUE);
					}
				}
				fm_next_obj=gl_list_obj[sel_obj];
				set_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,TRUE);
				throw_evntaway();
			}    
			else if (fm_cont)
			{	fm_cont=FormKeybd(listmsg_tree,fm_edit_obj,fm_next_obj,fm_kr,0,&fm_next_obj,&fm_kr);
			}                         
		}

		if (fm_which & MU_BUTTON)
		{	fm_next_obj = objc_find(listmsg_tree,ROOT,MAX_DEPTH,fm_mx,fm_my);
			if (fm_next_obj == -1)
			{	Dosound (fm_sound_dta);
				fm_next_obj = 0;
			}
			else if (fm_next_obj==LIFLYBTN)
			{	graf_mouse(FLAT_HAND,(void *)0L);
				DialMove(&listmsg_dial,gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
				mouse_normal();
			}
			else if (fm_next_obj==LGRAU)
			{	objc_offset(listmsg_tree, LSLIDER, &dummy, &yslider);
				if (fm_my>yslider)  /* runterblaettern */
				{	if (wi[win].lastmsg-wi[win].firstmsg > MAX_LIST-1)
					{	if ( top_msg < wi[win].lastmsg-(MAX_LIST-1) )
						{ lmsg=top_msg+MAX_LIST;
							if (wi[win].lastmsg-lmsg<MAX_LIST)
								lmsg=wi[win].lastmsg-(MAX_LIST-1);
							top_msg=lmsg;
							inhalt_listen(win, lmsg, &sel_obj, TRUE);
						}
					}	
				}
				else if (fm_my<yslider)     /* rauf */
				{	if (wi[win].lastmsg-wi[win].firstmsg>MAX_LIST-1)
					{	if (top_msg > 1)
						{	lmsg=top_msg-MAX_LIST;
							if (lmsg<wi[win].firstmsg)
								lmsg=wi[win].firstmsg;
							top_msg=lmsg;	
							inhalt_listen(win, lmsg, &sel_obj, TRUE);
						}
					}
				}
				fm_cont=TRUE;
			}
			else if (fm_next_obj==LUPAR)
			{	if (wi[win].lastmsg-wi[win].firstmsg>MAX_LIST-1)
				{	if (top_msg > 1)
					{	lmsg=top_msg-1;
						if (lmsg<wi[win].firstmsg)
							lmsg=wi[win].firstmsg;
						top_msg=lmsg;	
						inhalt_listen(win, lmsg, &sel_obj, TRUE);
					}
				}
			}
			else if (fm_next_obj==LDOWNAR)
			{	if (wi[win].lastmsg-wi[win].firstmsg>MAX_LIST-1)
				{	if ( top_msg < wi[win].lastmsg-(MAX_LIST-1) )
					{	lmsg=top_msg+1;
						if (wi[win].lastmsg-lmsg<MAX_LIST)
							lmsg=wi[win].lastmsg-(MAX_LIST-1);
						top_msg=lmsg;
						inhalt_listen(win, lmsg, &sel_obj, TRUE);
					}
				}
			}
			else if (fm_next_obj==LSLIDER)
			{	if (wi[win].lastmsg-wi[win].firstmsg>MAX_LIST-1)
				{	pos=(long)graf_slidebox(listmsg_tree,LGRAU,LSLIDER,1);
					sldh=(long)(listmsg_tree+LGRAU)->ob_height - 
					     (((long)(listmsg_tree+LGRAU)->ob_height * MAX_LIST) / (long)wi[win].lastmsg);
					lmsg=(word)( (((sldh*pos)/1000L) * (long)wi[win].lastmsg) / (long)(listmsg_tree+LGRAU)->ob_height )+1;
					if (lmsg < wi[win].firstmsg)
						lmsg=wi[win].firstmsg;
					else if (lmsg > wi[win].lastmsg-(MAX_LIST-1))
						lmsg=wi[win].lastmsg-(MAX_LIST-1);
					top_msg=lmsg;
					inhalt_listen(win, lmsg, &sel_obj, TRUE);
				}
			}
			else if (fm_next_obj==LICANCEL)
			{	fm_cont=FALSE;
				del_state(listmsg_tree,gl_list_obj[sel_obj],SELECTED,FALSE);
			}
			else
				fm_cont = FormButton(listmsg_tree,fm_next_obj,fm_br,&fm_next_obj);
		}
	}
	wind_update(END_MCTRL);
	return(fm_next_obj & 0x7fff);
}
