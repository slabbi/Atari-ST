#include "ccport.h"
#include "selorig.rh"
#include "ledscan.h"
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"
#include "language.h"

#define MAX_SELORIG 10

static ORIG *gl_orstart;
static ORIG *gl_orcur;
static word gl_orzahl;
static word gl_orindex;

word selo_obj[10]={SO0,SO1,SO2,SO3,SO4,SO5,SO6,SO7,SO8,SO9};

static word orig_store(byte *text)
{	ORIG *new, *old, *p;

	new=(ORIG *)malloc(sizeof(ORIG));
	if (new==NULL)
		return(FALSE);

	new->text = strdup(text);

	if (gl_orstart==NULL)		/* Gibt es schon eine Liste?	*/
	{	new->prior=NULL;			/* ...Nein!										*/
		new->next=NULL;
		gl_orstart=new;
		return(TRUE);
	}
	
	p=gl_orstart;						/* Erste Element der Liste		*/
	old=p;
	while (p)
	{	old=p;
		p=p->next;						/* Ende der Liste suchen			*/
	}
													/* "old" zeigt auf letztes Element!	*/
	old->next=new;
	new->prior=old;
	new->next=NULL;
	return(TRUE);
}		

void selo_vor(void)
{	if (gl_orcur!=NULL && gl_orcur->next!=NULL)
	{	if (gl_orzahl>MAX_SELORIG)
		{	if ( (gl_orzahl-MAX_SELORIG) >=gl_orindex)
			{	gl_orcur=gl_orcur->next;
				gl_orindex++;
			}	
		}
	}	
}

void selo_top(void)
{	if (gl_orstart != NULL)
	{	gl_orcur=gl_orstart;
		gl_orindex=1;
	}	
}

void selo_end(void)
{	word vorzahl, i;
	gl_orcur=gl_orstart;
	gl_orindex=1;
	vorzahl=gl_orzahl-MAX_SELORIG;
	for (i=0;i<vorzahl;i++)
		selo_vor();
}

void selo_zurueck(void)
{	if (gl_orcur!=NULL && gl_orcur->prior!=NULL)
	{	gl_orcur=gl_orcur->prior;
		gl_orindex--;
	}	
}

void free_origlist(void)
{	ORIG *hilf, *tmp;
	hilf = tmp = gl_orstart;
	while (hilf!=NULL)
	{	tmp=hilf->next;
		free(hilf->text);
		free(hilf);
		hilf=tmp;
	}
	gl_orstart=NULL;
}

void selo_anzeigen(word redraw)
{	ORIG *hilf;
	long lgrau;
	word i;
	byte tmp[MAX_STR];
	
	hilf=gl_orcur;
	for (i=0;i<MAX_SELORIG;i++)
	{	
		if (hilf!=NULL)
		{	sprintf(tmp,"%-40.40s",	hilf->text);
			hilf=hilf->next;
		}
		else
			sprintf(tmp,"%-40.40s",	"");
		set_dtext(selorig_tree,selo_obj[i],tmp,redraw,40);
	}

	if (gl_orzahl>MAX_SELORIG)
	{	lgrau= (long) ( (long) (selorig_tree+SOGRAU)->ob_height ) * (long)(gl_orindex-1);
		(selorig_tree+SOSLIDER)->ob_y= (uword) (lgrau / gl_orzahl);
		(selorig_tree+SOSLIDER)->ob_height=((selorig_tree+SOGRAU)->ob_height*MAX_SELORIG)/(gl_orzahl);
		if (redraw)
		{	redraw_obj(selorig_tree,SOGRAU);
			redraw_obj(selorig_tree,SOSLIDER);				          		
		}
	}
	else
	{	(selorig_tree+SOSLIDER)->ob_y=0;
		(selorig_tree+SOSLIDER)->ob_height=(selorig_tree+SOGRAU)->ob_height;
	}
}

word do_origform(OBJECT *fm_tree, DIALINFO *fm_dial, word sel_obj, byte *ret)
{	word fm_edit_obj;
	word fm_next_obj;
	word fm_which;
	word fm_cont;
	word fm_mx, fm_my, fm_mb, fm_ks, fm_kr, fm_br;
	word fm_msg[8];
	word i;
	word yslider, dummy;
	word lmsg;
	long pos;
	
	wind_update(BEG_MCTRL);

	fm_next_obj = selo_obj[sel_obj];
	fm_edit_obj = 0;
	fm_cont = TRUE;
	
	while (fm_cont)
	{	fm_which = evnt_multi(MU_KEYBD|MU_BUTTON|MU_MESAG,
		                      0x02,0x01,0x01,
		                      0,0,0,0,0,
		                      0,0,0,0,0,
		                      fm_msg,
		                      0,0,
		                      &fm_mx,&fm_my,&fm_mb,&fm_ks,&fm_kr,&fm_br);
		if (fm_which & MU_KEYBD)
		{	fm_kr=get_code(fm_kr,fm_ks);
			if (fm_kr == RETURN || fm_kr == ENTER || fm_kr==ALT_O)
			{	fm_cont = FALSE;
			}
			else if (fm_kr==ALT_A || fm_kr==ALT_C || fm_kr == UNDO)
			{	if (!(fm_tree[SOCANCEL].ob_flags & HIDETREE))
				{	fm_cont=FALSE;
					fm_next_obj=SOCANCEL;
					del_state(fm_tree,selo_obj[sel_obj],SELECTED,FALSE);
				}
			}
			
			if (fm_kr == HOME)
			{	fm_cont = TRUE;
				if (gl_orzahl>MAX_SELORIG)
				{	if (gl_orindex>1)
					{	selo_top();
						selo_anzeigen(TRUE);
					}
				}
			}
			else if (fm_kr == SHFT_HOME)
			{	fm_cont = TRUE;
				if (gl_orzahl>MAX_SELORIG)
				{	if (gl_orzahl-MAX_SELORIG>=gl_orindex)
					{	selo_end();
						selo_anzeigen(TRUE);
					}
				}
			}
			else if (fm_kr == CUR_DOWN)
			{	fm_cont = TRUE;
				del_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);
				if (sel_obj+1<MAX_SELORIG)
				{	sel_obj++;
					if (req_flag(fm_tree,selo_obj[sel_obj],HIDETREE))
						sel_obj=0;
				}
				else
					sel_obj=0;
				set_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);
				fm_next_obj = selo_obj[sel_obj];
			}
			else if (fm_kr == CUR_UP)                
			{	fm_cont = TRUE;
				del_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);
				if (sel_obj>0)
					sel_obj--;
				else
				{	sel_obj=MAX_SELORIG-1;	
					if (gl_orzahl<MAX_SELORIG)
						sel_obj=gl_orzahl-1;
				}
				set_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);
				fm_next_obj = selo_obj[sel_obj];
			}
			else if (fm_kr == SHFT_CD)
			{	fm_cont=TRUE;
				if (gl_orzahl>MAX_SELORIG && 
				    gl_orzahl-MAX_SELORIG>=gl_orindex)
				{	for (i=0;i<MAX_SELORIG;i++)
						selo_vor();
					selo_anzeigen(TRUE);
				}
				throw_evntaway();
			}
			else if (fm_kr == SHFT_CU)
			{	fm_cont=TRUE;
				if (gl_orzahl>MAX_SELORIG && gl_orindex>1)
				{	for (i=0;i<MAX_SELORIG;i++)
						selo_zurueck();
					selo_anzeigen(TRUE);
				}
				throw_evntaway();
			}
			else if (isgraph(fm_kr))
			{	ORIG *sta;
				sta=gl_orcur;
				i=gl_orindex;
				del_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);

				while (sta->next)
				{	sta=sta->next;
					i++;
					if (toupper(*sta->text)==toupper(fm_kr))
					{	gl_orindex=i;
						gl_orcur=sta;
						sel_obj=0;
						while (gl_orcur->prior && gl_orindex> gl_orzahl-MAX_SELORIG)
						{	gl_orcur=gl_orcur->prior;
							gl_orindex--;
							sel_obj++;
						}
						if (sel_obj>=MAX_SELORIG)
						{	sel_obj--;
							gl_orcur=gl_orcur->next;
							gl_orindex++;
						}
						set_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);

						selo_anzeigen(TRUE);
						break;
					}
				}
				set_state(fm_tree,selo_obj[sel_obj],SELECTED,TRUE);
			}
			else if (fm_cont)
				fm_cont=FormKeybd(fm_tree,fm_edit_obj,fm_next_obj,fm_kr,0,&fm_next_obj,&fm_kr);
		}

		if (fm_which & MU_BUTTON)
		{	fm_next_obj = objc_find(fm_tree,ROOT,MAX_DEPTH,fm_mx,fm_my);
			if (fm_next_obj == -1)
			{	Dosound (fm_sound_dta);
				fm_next_obj = 0;
			}
			else if (fm_next_obj == SOFLYBTN)
			{	graf_mouse(FLAT_HAND,(void *)0L);
				DialMove(fm_dial,gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
				mouse_normal();
			}
			else if (fm_next_obj == SOGRAU)
			{	objc_offset(fm_tree, SOSLIDER, &dummy, &yslider);
				if (fm_my>yslider)  /* runterblaettern */
				{	if (gl_orzahl>MAX_SELORIG && 
						gl_orzahl-MAX_SELORIG>=gl_orindex)
					{	for (i=0;i<MAX_SELORIG;i++)
							selo_vor();
						selo_anzeigen(TRUE);
					}
				}
				else if (fm_my<yslider)     /* rauf */
				{	if (gl_orzahl>MAX_SELORIG && gl_orindex>1)
					{	for (i=0;i<MAX_SELORIG;i++)
							selo_zurueck();
						selo_anzeigen(TRUE);
					}
				}
				fm_cont=TRUE;
			}
			else if (fm_next_obj == SOUP)
			{	if (gl_orzahl>MAX_SELORIG && gl_orindex>1)
				{	selo_zurueck();
					selo_anzeigen(TRUE);
				}
			}
			else if (fm_next_obj == SODOWN)
			{	if (gl_orzahl>MAX_SELORIG && 
					gl_orzahl-MAX_SELORIG>=gl_orindex)
				{	selo_vor();
					selo_anzeigen(TRUE);
				}
			}
			else if (fm_next_obj == SOSLIDER)
			{	if (gl_orzahl>MAX_SELORIG)
				{	pos=(long)graf_slidebox(selorig_tree,SOGRAU,SOSLIDER,1);
					pos=(long)gl_orzahl*pos/1000L;
					lmsg=(word)pos;
					gl_orcur=gl_orstart;
					gl_orindex=1;
					for (i=0;i<lmsg;i++)
						selo_vor();	
					selo_anzeigen(TRUE);
				}
			}
			else
				fm_cont = FormButton(fm_tree,fm_next_obj,fm_br,&fm_next_obj);
		}
	}
	wind_update(END_MCTRL);
	if (ret)
	{	for (i=0;i<MAX_SELORIG;i++)
		{	if (req_state(fm_tree,selo_obj[i],SELECTED))
			{	if (gl_orcur)
					strcpy(ret, gl_orcur->text);
				break;
			}		
			if (gl_orcur /*->next*/)
				gl_orcur=gl_orcur->next;
		}
	}
	return(fm_next_obj & 0x7fff);
}

/* FALSE: Keine Auswahl
** TRUE : OK
** -1		: Cancel
*/

static word sel_searchorig(byte *selected)
{	word i;
	word ext_btn;

	gl_orcur=gl_orstart;
	gl_orindex=1;
	selo_anzeigen(FALSE);
	if (gl_orzahl>=1)
	{	mouse_normal();
		show_mouse();

		if (gl_orzahl<MAX_SELORIG)
		{	for (i=gl_orzahl;i<MAX_SELORIG;i++)
			set_flag(selorig_tree,selo_obj[i],HIDETREE,FALSE);
		}

		for (i=0;i<MAX_SELORIG;i++)
			if (req_state(selorig_tree,selo_obj[i],SELECTED))
				del_state(selorig_tree,selo_obj[i],SELECTED,FALSE);

		set_state(selorig_tree,selo_obj[0],SELECTED,FALSE);
		draw_dialog(selorig_tree, &selorig_dial);
		ext_btn=do_origform(selorig_tree,&selorig_dial,0,selected);

		for (i=0;i<MAX_SELORIG;i++)
			del_flag(selorig_tree,selo_obj[i],HIDETREE,FALSE);

		for (i=0;i<MAX_SELORIG;i++)
			if (req_state(selorig_tree,selo_obj[i],SELECTED))
				break;
		del_state(selorig_tree,ext_btn,SELECTED,FALSE);
		del_dialog(&selorig_dial);

		if (ext_btn==SOCANCEL)
		{	selected=EOS;
			return(-1);
		}
		else
		{	return(TRUE);
		}
	}
	selected=EOS;
	return(FALSE);
}	

int passender_absender(char *touser,
											 uword toZone,uword toNet,uword toNode,uword toPoint,byte *toDomain,
											 uword *fZone,uword *fNet,uword *fNode,uword *fPoint,byte *fDomain)
{	word i,foundcount,lastindex;
	uword zone,net,node,point;
	byte tmp[MAX_STR+1],domain[32];

	foundcount=0;
	lastindex=-1;
	for (i=0;i<=addrcount;i++)
	{	if (!toDomain && addr[i].zone==toZone ||
				 toDomain && !stricmp(toDomain,addr[i].domain))
		{	foundcount++;
			lastindex=i;
		}
	}

	if (foundcount==1)
	{	*fZone=addr[lastindex].zone;
		*fNet=addr[lastindex].net;
		*fNode=addr[lastindex].node;
		*fPoint=addr[lastindex].point;
		if (fDomain)
			*fDomain=EOS;
/* 		strcpy(fDomain,addr[lastindex].domain); */
		return(0 /*lastindex+1*/);
	}

	/* Die gefundenen anzeigen! */

	if (foundcount>1)
	{	gl_orstart=NULL;
		gl_orzahl=0;
		for (i=0;i<=addrcount;i++)
		{	if (!toDomain && addr[i].zone==toZone ||
					 toDomain && !stricmp(toDomain,addr[i].domain))
			{	if (!orig_store(mergeaddr(addr[i].zone,
																	addr[i].net,
																	addr[i].node,
																	addr[i].point,
																	addr[i].domain)))
				{	LangAlert(MSG_NOMEMORIGIN);
					break;
				}
				else
					gl_orzahl++;
			}
		}
	}

	if (foundcount<=0)
	{	gl_orstart=NULL;
		gl_orzahl=0;
		for (i=0;i<=addrcount;i++)
		{	if (!orig_store(mergeaddr(addr[i].zone,
																addr[i].net,
																addr[i].node,
																addr[i].point,
																addr[i].domain)))
			{	LangAlert(MSG_NOMEMORIGIN);
				break;
			}
			else
				gl_orzahl++;
		}
	}

	set_dtext(selorig_tree,SOTO,touser,FALSE,36);
	if (toZone!=0 || toNet!=0 || toNode!=0 || toPoint!=0)
		set_dtext(selorig_tree,SOADDR,mergeaddr(toZone,
																						toNet,
																						toNode,
																						toPoint,
																						toDomain),FALSE,36);
	else
		set_dtext(selorig_tree,SOADDR,"",FALSE,36);

	set_dstring(selorig_tree,SOTITEL,msgtxt[SYSTEM_SELECTORIGINADDR].text,FALSE,21);
	del_mflag(selorig_tree,HIDETREE,FALSE,SOTO1,SOADDR1,SOTO,SOADDR,-1);
	set_mflag(selorig_tree,HIDETREE,FALSE,SOCANCEL,-1);

	i=sel_searchorig(tmp);

	if (i==FALSE)
		LangAlert(MSG_CHOOSEORIGIN);
/*	else if (i==-1)
		FormAler_(1,3,"orig_address: CANCEL?","  [OK  "); */

	scanaddr(tmp,&zone,&net,&node,&point,domain);
	*fZone =zone;
	*fNet	 =net;
	*fNode =node;
	*fPoint=point;
	if (fDomain)
		*fDomain=EOS;
/*		strcpy(fDomain,domain); */

	free_origlist();
	del_mflag(selorig_tree,HIDETREE,FALSE,SOCANCEL,-1);

	lastindex=-1;
	for (i=0;i<=addrcount;i++)
	{	if (addr[i].zone ==zone &&
				addr[i].net  ==net  &&
				addr[i].node ==node &&
				addr[i].point==point &&
				!strcmp(addr[i].domain,domain))
		{	lastindex=i;
			break;
		}
	}
	return(0 /*lastindex+1*/);
}

void set_if_fixed_address(long win,word area)
{	if ( (opt.always_fixed 										 &&  is_netmail(area)) ||
			 (gl_area[area].flag & AR_FIXEDADDRESS && !is_netmail(area)) )
		wi[win].hdr->XFlags |= XF_FIXEDADDRESS;
}


word select_sysop(char *sysop)
{	word i,ret=TRUE,yadd;
	byte tmp[MAX_STR+1];

	gl_orstart=NULL;
	gl_orzahl=0;

	for (i=0;i<=akasysopcount;i++)
	{	if (!orig_store(akasysop[i]))
		{	LangAlert(MSG_NOMEMSYSOP);
			break;
		}
		else
			gl_orzahl++;
	}

	set_dtext(selorig_tree,SOTO,"",FALSE,36);
	set_dtext(selorig_tree,SOADDR,"",FALSE,36);
	set_mflag(selorig_tree,HIDETREE,FALSE,SOTO1,SOADDR1,SOTO,SOADDR,-1);

	yadd=selorig_tree[SOTO].ob_height * 3;
	selorig_tree[SOBOX].ob_y -= yadd;
	selorig_tree[SOOK].ob_y -= yadd;
	selorig_tree[SOCANCEL].ob_y -= yadd;
	selorig_tree[ROOT].ob_height -= yadd;

	set_dstring(selorig_tree,SOTITEL,msgtxt[SYSTEM_SELECTORIGINNAME].text,FALSE,21);

	i=sel_searchorig(tmp);
	if (i==FALSE)
	{	/* FormAler_(1,3,"Please choose a name!","  [OK  "); */
		ret=FALSE;
	}
	else if (i==-1)
	{	ret=FALSE;
		/* FormAler_(1,3,"select_sysop: CANCEL?","  [OK  "); */
	}
	else
	{	
		kill_lblank(tmp, FALSE);
		strcpy(sysop,tmp);
	}

	free_origlist();

	selorig_tree[SOBOX].ob_y += yadd;
	selorig_tree[SOOK].ob_y += yadd;
	selorig_tree[SOCANCEL].ob_y += yadd;
	selorig_tree[ROOT].ob_height += yadd;
	return ret;
}

word select_domain(char *domain)
{	word i,ret=TRUE,yadd;
	byte tmp[MAX_STR+1];

	gl_orstart=NULL;
	gl_orzahl=0;

	if (domaincount==-1)
	{	LangAlert(MSG_NODOMAINDEFINED);
		return(FALSE);
	}

	for (i=0;i<=domaincount;i++)
	{	if (!orig_store(gl_domain[i][0]))
		{	LangAlert(MSG_NOMEMDOMAIN);
			break;
		}
		else
			gl_orzahl++;
	}

	set_dtext(selorig_tree,SOTO,"",FALSE,36);
	set_dtext(selorig_tree,SOADDR,"",FALSE,36);
	set_mflag(selorig_tree,HIDETREE,FALSE,SOTO1,SOADDR1,SOTO,SOADDR,-1);

	yadd=selorig_tree[SOTO].ob_height * 3;
	selorig_tree[SOBOX].ob_y -= yadd;
	selorig_tree[SOOK].ob_y -= yadd;
	selorig_tree[SOCANCEL].ob_y -= yadd;
	selorig_tree[ROOT].ob_height -= yadd;

	set_dstring(selorig_tree,SOTITEL,msgtxt[SYSTEM_SELECTDOMAIN].text,FALSE,21);

	i=sel_searchorig(tmp);
	if (i==FALSE)
	{	/* FormAler_(1,3,"Please choose a domain!","  [OK  "); */
		ret=FALSE;
	}
	else if (i==-1)
	{	ret=FALSE;
		/* FormAler_(1,3,"select_domain: CANCEL?","  [OK  "); */
	}
	else
	{
		kill_lblank(tmp, FALSE);
		strcpy(domain,tmp);
	}

	free_origlist();

	selorig_tree[SOBOX].ob_y += yadd;
	selorig_tree[SOOK].ob_y += yadd;
	selorig_tree[SOCANCEL].ob_y += yadd;
	selorig_tree[ROOT].ob_height += yadd;
	return ret;
}

word show_shortcut(long win,byte *filter)
{	word ret=TRUE,yadd,i;
	byte filename[MAX_STR],buffer[1024],*p;
	FILE *fp;
	struct ffblk dta;

	gl_orstart=NULL;
	gl_orzahl=0;
	if (filter)
		i=(word)strlen(filter);

	sprintf(filename,"%sLED.SCT",gl_ledpath);
	if ((fp=fopen(filename,"r"))!=NULL)
	{	while((fgets(buffer, MAX_STR, fp))!=NULL)
		{	if (*buffer==';' || *buffer==EOS)
				continue;
			p=strchr(buffer,EOS);
			while (p>buffer && isspace(*(p-1)))	p--;
			*p=EOS;
			if (*buffer==EOS)
				continue;
			if (filter)
				if (strnicmp(buffer,filter,i))
					continue;
#if 0
			while (strlen(buffer)>40)
			{	strncpy(filename,buffer,40);
				filename[40]=EOS;
				orig_store(filename);
				strcpy(buffer,buffer+40);
			}
#endif
			if (!orig_store(buffer))
			{	LangAlert(MSG_NOMEMSHORTCUT);
				break;
			}
			else
				gl_orzahl++;
		}
		fclose(fp);
	}
	else
	{	orig_store(msgtxt[DIV_SORRY].text);
		orig_store(msgtxt[DIV_SCTNOTFOUND].text);
		gl_orzahl=2;
	}

	set_dtext(selorig_tree,SOTO,"",FALSE,36);
	set_dtext(selorig_tree,SOADDR,"",FALSE,36);
	set_mflag(selorig_tree,HIDETREE,FALSE,SOTO1,SOADDR1,SOTO,SOADDR,-1);

	if (win>=0 && wi[win].editierbar)
		del_flag(selorig_tree,SOCANCEL,HIDETREE,FALSE);
	else
		set_flag(selorig_tree,SOCANCEL,HIDETREE,FALSE);

	yadd=selorig_tree[SOTO].ob_height * 3;
	selorig_tree[SOBOX].ob_y -= yadd;
	selorig_tree[SOOK].ob_y -= yadd;
	selorig_tree[SOCANCEL].ob_y -= yadd;
	selorig_tree[ROOT].ob_height -= yadd;

	set_dstring(selorig_tree,SOTITEL,msgtxt[SYSTEM_SELECTSCT].text,FALSE,21);

	i=sel_searchorig(buffer);
	if (i>0)
	{	if (win>=0 && wi[win].editierbar)
		{	p=strchr(buffer,EOS);
			while (p>buffer && isspace(*(p-1)))	p--;
			*p=EOS;
			if (*buffer=='-')
			{	p=strchr(buffer,',');
				if (!p)
				{	sprintf(filename,msgtxt[MSG_SHORTCUTFORMAT].text,buffer);
					FormAlert(msgtxt[MSG_SHORTCUTFORMAT].defbut,msgtxt[MSG_SHORTCUTFORMAT].icon,filename,msgtxt[MSG_SHORTCUTFORMAT].button);
				}
				else
				{	strcpy(filename,skip_blanks(p+1));
					cut_endspc(filename);
					if (!findfirst(filename,&dta,0))
					{	if (read_ascii(win,-1,filename))
						{	wisetfiles(win,FALSE,NO_CHANGE);
							s_cutbuff=e_cutbuff=-1;
							upded_stat(win,0);
						}
					}
					else
					{	sprintf(buffer,msgtxt[MSG_FILENOTFOUND].text,filename);
						FormAlert(msgtxt[MSG_FILENOTFOUND].defbut,msgtxt[MSG_FILENOTFOUND].icon,buffer,msgtxt[MSG_FILENOTFOUND].button);
					}
				}
			}
			else
			{	test_registered();
				if (!is_registered)
				{	p="*** You are using an unregistered version! ***";
					while(*p) output(win,*p++,TRUE);
					c_return(win);
					c_return(win);
				}
				p=strchr(buffer,'|');
				if (p)
				{	*p=EOS;
					cut_endspc(buffer);
				}

				convert_percent(buffer,wi[win].aktarea,TRUE);

				p=buffer;
				while (*p)
				{	if (*p=='\n' || *p=='\r')	c_return(win);
					else											output(win,*p,TRUE);
					p++;
				}
			}
		}
	}
#if 0
	if (!sel_searchorig(tmp))
	{	FormAler_(1,3,"Please choose a shortcut","  [OK  ");
		ret=FALSE;
	}
#endif

	free_origlist();

	del_flag(selorig_tree,SOCANCEL,HIDETREE,FALSE);

	selorig_tree[SOBOX].ob_y += yadd;
	selorig_tree[SOOK].ob_y += yadd;
	selorig_tree[SOCANCEL].ob_y += yadd;
	selorig_tree[ROOT].ob_height += yadd;
	return ret;
}




typedef struct
{	word	keycode;
	byte	**text;
} FUNCTION_LIST;

FUNCTION_LIST function_list1[] = {
{	HELPKEY,	&msgtxt[FUNC_HELPKEY].text		},
{	UNDO,			&msgtxt[FUNC_UNDO].text 			},
{	KEY_p,		&msgtxt[FUNC_KEY_p].text			},
{	KEY_n,		&msgtxt[FUNC_KEY_n].text			},
{	ZEHN_2,		&msgtxt[FUNC_ZEHN_2].text			},
{	ZEHN_8,		&msgtxt[FUNC_ZEHN_8].text			},
{	ZEHN_4,		&msgtxt[FUNC_ZEHN_4].text			},
{	ZEHN_6,		&msgtxt[FUNC_ZEHN_6].text			},
{	ZEHN_5,		&msgtxt[FUNC_ZEHN_5].text			},
{	ZEHN_OPKL,&msgtxt[FUNC_ZEHN_OPKL].text	},
{	ZEHN_CLKL,&msgtxt[FUNC_ZEHN_CLKL].text	},
{	ZEHN_CT_OPKL,&msgtxt[FUNC_ZEHN_CT_OPKL].text	},
{	ZEHN_CT_CLKL,&msgtxt[FUNC_ZEHN_CT_CLKL].text	},
{	ALT_1,		&msgtxt[FUNC_ALT_1].text			},
{	ALT_2,		&msgtxt[FUNC_ALT_2].text			},
{	ALT_3,		&msgtxt[FUNC_ALT_3].text			},
{	ALT_4,		&msgtxt[FUNC_ALT_4].text			},
{	ALT_5,		&msgtxt[FUNC_ALT_5].text			},
{	ALT_6,		&msgtxt[FUNC_ALT_6].text			},
{	ALT_7,		&msgtxt[FUNC_ALT_7].text			},
{	ALT_8,		&msgtxt[FUNC_ALT_8].text			},
{	ALT_9,		&msgtxt[FUNC_ALT_9].text			},
{	ALT_0,		&msgtxt[FUNC_ALT_0].text			},
{	ZEHN_MINUS,&msgtxt[FUNC_ZEHN_MINUS].text},
{	ZEHN_PLUS,&msgtxt[FUNC_ZEHN_PLUS].text	},
{	ZEHN_STAR,&msgtxt[FUNC_ZEHN_STAR].text	},
{	ZEHN_SLASH,&msgtxt[FUNC_ZEHN_SLASH].text},
{	DOPPKREUZ,&msgtxt[FUNC_DOPPKREUZ].text	},
{	CNTRL_F1,	&msgtxt[FUNC_CNTRL_F1].text		},
{	CNTRL_F2,	&msgtxt[FUNC_CNTRL_F2].text		},
{	CNTRL_F3,	&msgtxt[FUNC_CNTRL_F3].text		},
{	CNTRL_F5,	&msgtxt[FUNC_CNTRL_F5].text		},
{	CNTRL_F6,	&msgtxt[FUNC_CNTRL_F6].text		},
{	CNTRL_F7,	&msgtxt[FUNC_CNTRL_F7].text		},
{	CNTRL_F8,	&msgtxt[FUNC_CNTRL_F8].text		},
{	CNTRL_F9,	&msgtxt[FUNC_CNTRL_F9].text		},
{	CNTRL_F10,&msgtxt[FUNC_CNTRL_F10].text	},
{	ESC,			&msgtxt[FUNC_ESC].text				},
{	CNTRL_E,	&msgtxt[FUNC_CNTRL_E].text		},
{	CNTRL_R,	&msgtxt[FUNC_CNTRL_R].text		},
{	CNTRL_T,	&msgtxt[FUNC_CNTRL_T].text		},
{	CNTRL_J,	&msgtxt[FUNC_CNTRL_J].text		},
{	CNTRL_N,	&msgtxt[FUNC_CNTRL_N].text		},
{	KEY_T,		&msgtxt[FUNC_KEY_T].text			},
{	KEY_J,		&msgtxt[FUNC_KEY_J].text			},
{	KEY_N,		&msgtxt[FUNC_KEY_N].text			},
{	-1,				NULL													}
};

FUNCTION_LIST function_list2[] = {
{	HELPKEY,	&msgtxt[FUNCE_HELPKEY].text		},
{	CNTRL_F,	&msgtxt[FUNCE_CNTRL_F].text		},
{	CNTRL_K,	&msgtxt[FUNCE_CNTRL_K].text		},
{	CNTRL_B,	&msgtxt[FUNCE_CNTRL_B].text		},
{	CNTRL_D,	&msgtxt[FUNCE_CNTRL_D].text		},
{	CNTRL_X,	&msgtxt[FUNCE_CNTRL_X].text		},
{	CNTRL_C,	&msgtxt[FUNCE_CNTRL_C].text		},
{	CNTRL_V,	&msgtxt[FUNCE_CNTRL_V].text		},
{	SHIFT_ALT_E,&msgtxt[FUNCE_SHIFT_ALT_E].text	},
{	ALT_E,		&msgtxt[FUNCE_ALT_E].text			},
{	SHFT_DEL,	&msgtxt[FUNCE_SHFT_DEL].text	},
{	CNTRL_DEL,&msgtxt[FUNCE_CNTRL_DEL].text	},
{	ESC,			&msgtxt[FUNCE_ESC].text				},
{	INSERT,		&msgtxt[FUNCE_INSERT].text		},
{	ALT_I,		&msgtxt[FUNCE_ALT_I].text			},
{	ALT_T,		&msgtxt[FUNCE_ALT_T].text			},
{	SHIFT_ALT_Y,&msgtxt[FUNCE_SHIFT_ALT_Y].text	},
{	ALT_Y,		&msgtxt[FUNCE_ALT_Y].text			},
{	ALT_D,		&msgtxt[FUNCE_ALT_D].text			},
{	ALT_N,		&msgtxt[FUNCE_ALT_N].text			},
{	ALT_S,		&msgtxt[FUNCE_ALT_S].text			},
{	ALT_V,		&msgtxt[FUNCE_ALT_V].text			},
{	ALT_F,		&msgtxt[FUNCE_ALT_F].text			},
{	ALT_G,		&msgtxt[FUNCE_ALT_G].text			},
{	-1,				NULL													}
};

/*
** 0=MsgWindow
** 1=EditWindow
*/

word select_function(word mode)
{	word i,ret=0,yadd;
	byte tmp[MAX_STR+1];
	FUNCTION_LIST *function_list;

	if (mode==0)
		function_list=function_list1;
	else
		function_list=function_list2;

	gl_orstart=NULL;
	gl_orzahl=0;

	for (i=0;function_list[i].text;i++)
	{	if (!orig_store(*function_list[i].text))
		{	LangAlert(MSG_NOMEMFUNCTION);
			break;
		}
		else
			gl_orzahl++;
	}

	set_dtext(selorig_tree,SOTO,"",FALSE,36);
	set_dtext(selorig_tree,SOADDR,"",FALSE,36);
	set_mflag(selorig_tree,HIDETREE,FALSE,SOTO1,SOADDR1,SOTO,SOADDR,-1);

	yadd=selorig_tree[SOTO].ob_height * 3;
	selorig_tree[SOBOX].ob_y -= yadd;
	selorig_tree[SOOK].ob_y -= yadd;
	selorig_tree[SOCANCEL].ob_y -= yadd;
	selorig_tree[ROOT].ob_height -= yadd;

	set_dstring(selorig_tree,SOTITEL,msgtxt[SYSTEM_SELECTFUNCTION].text,FALSE,21);

	i=sel_searchorig(tmp);
	if (i==FALSE)
	{	/* FormAler_(1,3,"Please choose a function!","  [OK  "); */
		ret=-1;
	}
	else if (i==-1)
	{	ret=0;
		/* FormAler_(1,3,"select_function: CANCEL?","  [OK  "); */
	}
	else
	{
		kill_lblank(tmp, FALSE);
		for (i=0;function_list[i].text;i++)
		{	if (!strcmp(*function_list[i].text,tmp))
				ret=function_list[i].keycode;
		}
	}

	free_origlist();

	selorig_tree[SOBOX].ob_y += yadd;
	selorig_tree[SOOK].ob_y += yadd;
	selorig_tree[SOCANCEL].ob_y += yadd;
	selorig_tree[ROOT].ob_height += yadd;
	return ret;
}
