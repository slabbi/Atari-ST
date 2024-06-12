#include "ccport.h"
#include <time.h>
#include "seluser.rh"
#include "nodenum.rh"
#include "ledscan.h"
#include "ledgl.h"
#include "ledfc.h"
#include "winddefs.h"
#include "vars.h"
#include "nodelist.h"
#include "language.h"

#define MAX_SELUSER 10

word selu_obj[10]={SU0,SU1,SU2,SU3,SU4,SU5,SU6,SU7,SU8,SU9};

static USER *gl_uschar[256];
static USER *gl_usstart;
static USER *gl_uscur;
static long gl_uszahl;
static long gl_usindex;

static FILE *buf_readinit(byte *fname)
{	FILE *fp=NULL;
	char tmp[MAX_STR];
	struct ffblk dta;
	
	if (!findfirst(fname,&dta,0))
	{	fp=fopen(fname,"r");
		if (!fp)
		{	sprintf(tmp,msgtxt[MSG_CANNOTOPEN].text,fname);
			FormAlert(msgtxt[MSG_CANNOTOPEN].defbut,msgtxt[MSG_CANNOTOPEN].icon,tmp,msgtxt[MSG_CANNOTOPEN].button);
		}
		else
		{	if (setvbuf(fp,NULL,_IOFBF,32768UL))
			{	fclose(fp);
				LangAlert(MSG_NOMEMBUFFER);
				fp=NULL;
			}
		}
	}
	return(fp);
}

static word user_store(byte *name, byte *node, word listnr)
{	USER *new, *old, *p;
	word idx, len;
	
	new=(USER *)malloc(sizeof(USER));
	if (new==NULL)
		return(FALSE);
	
	strncpy(new->name,name,35);			new->name[35]=EOS;
	strncpy(new->node,node,24);			new->node[24]=EOS;

	new->userlist_nr=listnr;
	
	idx=(word)name[0];
	if (gl_usstart==NULL)
	{	new->prior=NULL;
		new->next=NULL;
		gl_usstart=new;
		gl_uschar[idx]=new;
		return(TRUE);
	}
	
	len=(word)strlen(name);
	if (gl_uschar[idx]!=NULL)
		p=gl_uschar[idx];
	else
		p=gl_usstart;
	
	if (strnicmp(name, p->name, len)<0)
	{	gl_uschar[idx]=new;
		new->next=p;
		new->prior=p->prior;
		if (p->prior!=NULL)
			p->prior->next=new;
		else
			gl_usstart=new;
		p->prior=new;
		return(TRUE);
	}
	
	old=p;
	while (p && strnicmp(name, p->name, len)>=0)
	{	old=p;
		p=p->next;
	}
	
	if (p && p!=old)
	{	old->next=new;
		p->prior=new;
	}
	else
	{	if (old!=NULL)
		{	old->next=new;
			p=NULL;
		}
		else
			gl_usstart=new;
	}
	new->prior=old;
	new->next=p;
	if (gl_uschar[idx]==NULL)
		gl_uschar[idx]=new;
	return(TRUE);
}		

static byte *del_allblanks(byte *s)
{	byte *nul, *cp;
	cp=s;
	while (isspace(*cp)) cp++;
	nul=strchr(cp,EOS)-1;
	while(nul>cp && isspace(*nul))
		*nul=EOS;
	return cp;
}

static word search_userlist(byte *name, byte *retaddr,byte *domain)
{	byte *startsuch,*anf;
	byte *start, *blank;
	byte *cp,*cpkomma;
	word i, slen, svorlen;
	FILE *fp;
	byte snachname[40], svorname[40];
	byte filename[MAX_STR];
	byte tmp[MAX_STR+1];
	byte tname[MAX_STR];
	byte buf[256];
	
	strcpy(tname,name);
	start=tname;
	cp=strrchr(tname,'@');
	if (cp==NULL)
		cp=strchr(tname,EOS);
	
	while (*(--cp)==' ')
		*cp=EOS;
	blank=strrchr(tname,' ');	

	if (blank==NULL)	/* kein Nachname z.B. "Sysop" */
		*snachname=EOS;
	else
	{			
		strcpy(snachname,blank+1);
		*blank=EOS;
	}
	strcpy(svorname,start);

	slen=(word)strlen(snachname);
	svorlen=(word)strlen(svorname);

	for (i=0;i<MAX_USERLIST;i++)
	{	
		if (strlen(opt.userlist[i])>0L)
		{
			strcpy(filename,opt.userlist[i]);
			if ((fp=buf_readinit(filename))!=NULL)
			{
				while (fgets(tmp,MAX_STR,fp))
				{
					if (strnicmp(tmp,snachname,slen)==0)
					{	
						cpkomma=startsuch=strchr(tmp,',');
						if (!startsuch)
						{	startsuch=strchr(tmp,' ');
							if (!startsuch)	continue;
						}

						startsuch++;		/* war +2 */
						while (*startsuch==' ')
							startsuch++;

						if (!strnicmp(startsuch,svorname,svorlen))	/* Name gefunden! */
						{       /* Username gefunden */
							if ((anf=strrchr(tmp,':'))!=NULL)
							{	anf--;
								while (isdigit(*(anf-1))) anf--;
							}
							else        /* keine Zonenummer  */	
							{	anf=strrchr(tmp,'/');
								if (!anf) continue;
								anf--;
								while (isdigit(*anf)) anf--;
								anf++;
							}
								
							if (!anf)
							{	LangAlert(MSG_INVALIDNODE);
								fclose(fp);
								return(FALSE);
							}	

							no_blanks(anf);

							if (domain)
							{	uword x1,x2,x3,x4;
								byte hlp[32];
								scanaddr(anf,&x1,&x2,&x3,&x4,hlp);
								if (!(*hlp))
									strcpy(hlp,opt.userlistdomain[i]);
								if (*hlp)
									if (stricmp(hlp,domain))
										continue;		/* Richtiger Name, falsches Domain! */
							}

							if (!cpkomma)
							{	sprintf(buf,msgtxt[MSG_USERCOMMA].text,filename,tmp);
								FormAlert(msgtxt[MSG_USERCOMMA].defbut,msgtxt[MSG_USERCOMMA].icon,buf,msgtxt[MSG_USERCOMMA].button);
								fclose(fp);
								return(FALSE);
							}
							else
								*cpkomma=EOS;
							cp=strchr(startsuch,' ');
							if (cp) *cp=EOS;
							sprintf(name,"%s %s",startsuch,tmp);
							
							strcpy(retaddr,anf);
							fclose(fp);
							return(TRUE);
						}
					}				
				}
				fclose(fp);
			}
			else
				LangAlert(MSG_USERLISTNOTFOUND);
		}
	}	/* for */
	return(FALSE);
}	

static word search_usernode(byte *name, uword zone, uword net, uword node, uword point)
{	byte *cp, *anf;
	word i;
	uword z,nt,nd,pt;
	byte tmp[MAX_STR];
	byte snachname[40];
	byte filename[MAX_STR],buf[256];
	FILE *fp;
	
	for (i=0;i<MAX_USERLIST;i++)
	{	
		if (strlen(opt.userlist[i])>0L)
		{
			strcpy(filename,opt.userlist[i]);
			if ((fp=buf_readinit(filename))!=NULL)
			{
				z=nt=nd=pt=0;

				while (fgets(tmp,MAX_STR,fp))
				{
					if ((anf=strrchr(tmp,':'))!=NULL)
					{	anf--;
						while ( isdigit(*(anf-1)) )
							anf--;
						sscanf(anf,"%u:%u/%u.%u",&z,&nt,&nd,&pt);
					}	
					else        /* keine Zonenummer  */	
					{	anf=strrchr(tmp,'/');
						if (anf==NULL)
							continue;
						
						anf--;
						while  (isdigit(*anf))
							anf--;
						anf++;
						sscanf(anf,"%u/%u.%u",&nt,&nd,&pt);
					}
						
					/* Node gefunden? */
					if ((z==0 || z==zone) && nt==net && nd==node && pt==point)
					{	cp=strchr(tmp,',');
						if (cp==NULL)
						{	cp=strchr(tmp,' ');
							if (cp==NULL)
							{	sprintf(buf,msgtxt[MSG_USERLISTERROR].text,filename,tmp);
								FormAlert(msgtxt[MSG_USERLISTERROR].defbut,msgtxt[MSG_USERLISTERROR].icon,buf,msgtxt[MSG_USERLISTERROR].button);
								fclose(fp);
								return(FALSE);
							}
							else
							{	*cp=EOS;
								strcpy(name,tmp);
								fclose(fp);
								return(TRUE);
							}
						}
						else
							*cp++=EOS;

						strcpy(snachname,skip_blanks(tmp));
						while (*cp==' ')
							cp++;
						anf=cp;
						cp=strchr(anf,' ');
						if (cp!=NULL)
							*cp=EOS;
						sprintf(name,"%s %s",anf,snachname);
						fclose(fp);
						return(TRUE);
					}
					else
					{	z=nt=nd=pt=0;
					}
				}
				fclose(fp);
			}
			else
				LangAlert(MSG_USERLISTNOTFOUND);
		}
	}	/* for */
	return(FALSE);
}	

static void selu_vor(void)
{
	if (gl_uscur!=NULL && gl_uscur->next!=NULL)
	{
		if (gl_uszahl>MAX_SELUSER)
		{
			if ( (gl_uszahl-MAX_SELUSER) >=gl_usindex)
			{
				gl_uscur=gl_uscur->next;
				gl_usindex++;
			}	
		}
	}	
}

static void free_userlist(void)
{	USER *hilf, *tmp;
	
	hilf=tmp=gl_usstart;
	while (hilf!=NULL)
	{
		tmp=hilf->next;
		free(hilf);
		hilf=tmp;
	}
	gl_usstart=NULL;
	memset(gl_uschar,0,sizeof(gl_uschar));
}

static void selu_zurueck(void)
{
	if (gl_uscur!=NULL && gl_uscur->prior!=NULL)
	{
		gl_uscur=gl_uscur->prior;
		gl_usindex--;
	}	
}

static void selu_top(void)
{
	if (gl_usstart != NULL)
	{
		gl_uscur=gl_usstart;
		gl_usindex=1;
	}	
}

static void selu_end(void)
{
	long vorzahl, i;

	gl_uscur=gl_usstart;
	gl_usindex=1;
	vorzahl=gl_uszahl-MAX_SELUSER;
	for (i=0;i<vorzahl;i++)
		selu_vor();
}

static void selu_anzeigen(word redraw)
{	USER *hilf;
	long lgrau;
	word i;
	byte *cp;
	byte tmp[MAX_STR];
	
	hilf=gl_uscur;
	for (i=0;i<MAX_SELUSER;i++)
	{
		if (hilf!=NULL)
		{
			sprintf(tmp,"%-36.36s %-23.23s",hilf->name,hilf->node);

			hilf=hilf->next;
		}
		else
		{
			cp=tmp;
			while(cp-tmp < 60)
				*cp++=' ';
			*cp=EOS;
		}		
		set_dtext(seluser_tree,selu_obj[i],tmp,redraw,60);
	} /* for  bis MAX_SELUSER */
	if (gl_uszahl>MAX_SELUSER)
	{
		lgrau= (long) ( (long) seluser_tree[SUGRAU].ob_height ) * (long)(gl_usindex-1);
		seluser_tree[SUSLIDER].ob_y= (uword) (lgrau / gl_uszahl);
		seluser_tree[SUSLIDER].ob_height=(word) (((long)seluser_tree[SUGRAU].ob_height*(long)MAX_SELUSER)/gl_uszahl);
		if (seluser_tree[SUSLIDER].ob_height<2)
			seluser_tree[SUSLIDER].ob_height=2;

		if (redraw)
		{
			redraw_obj(seluser_tree,SUGRAU);
			redraw_obj(seluser_tree,SUSLIDER);				          		
		}
	}
	else
	{
		seluser_tree[SUSLIDER].ob_y=0;
		seluser_tree[SUSLIDER].ob_height=seluser_tree[SUGRAU].ob_height;
	}
}

static word do_sform(OBJECT *fm_tree, DIALINFO *fm_dial, word sel_obj)
{
	word fm_edit_obj;
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
	fm_next_obj = selu_obj[sel_obj];
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
			
			if (fm_kr == RETURN || fm_kr == ENTER || fm_kr==ALT_O)
			{
				fm_cont = FALSE;
				fm_next_obj=0;
				for (i=0;i<MAX_SELUSER;i++)
				{
					if (req_state(fm_tree,selu_obj[i],SELECTED))
					{
						fm_next_obj=selu_obj[i];
						break;
					}		
				}
			}
			else if (fm_kr==ALT_A || fm_kr==ALT_C || fm_kr == UNDO)
			{
				fm_cont=FALSE;
				fm_next_obj=SUCANCEL;
				del_state(fm_tree,selu_obj[sel_obj],SELECTED,FALSE);
			}
			
			if (fm_kr == HOME)
			{
				fm_cont = TRUE;
				if (gl_uszahl>MAX_SELUSER)
				{
					if (gl_usindex>1)
					{
						selu_top();
						selu_anzeigen(TRUE);
					}
				}
			}
			else if (fm_kr == SHFT_HOME)
			{
				fm_cont = TRUE;
				if (gl_uszahl>MAX_SELUSER)
				{
					if (gl_uszahl-MAX_SELUSER>=gl_usindex)
					{
						selu_end();
						selu_anzeigen(TRUE);
					}
				}
			}
			else if (fm_kr == CUR_DOWN)
			{
				fm_cont = TRUE;
				del_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);
				if (sel_obj+1<MAX_SELUSER)
				{
					sel_obj++;
					if (req_flag(fm_tree,selu_obj[sel_obj],HIDETREE))
						sel_obj=0;
				}
				else
					sel_obj=0;
				set_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);
				fm_next_obj = selu_obj[sel_obj];
			}
			else if (fm_kr == CUR_UP)                
			{
				fm_cont = TRUE;
				del_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);
				if (sel_obj>0)
					sel_obj--;
				else
				{
					sel_obj=MAX_SELUSER-1;	
					if (gl_uszahl<MAX_SELUSER)
						sel_obj=(word)gl_uszahl-1;
				}	
				set_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);
				fm_next_obj = selu_obj[sel_obj];
			} 
			else if (fm_kr == SHFT_CD)
			{
				fm_cont=TRUE;
				if (gl_uszahl>MAX_SELUSER && 
				    gl_uszahl-MAX_SELUSER>=gl_usindex)
				{
					for (i=0;i<MAX_SELUSER;i++)
						selu_vor();
					selu_anzeigen(TRUE);
				}
				throw_evntaway();
			}
			else if (fm_kr == SHFT_CU)
			{
				fm_cont=TRUE;
				if (gl_uszahl>MAX_SELUSER && gl_usindex>1)
				{
					for (i=0;i<MAX_SELUSER;i++)
						selu_zurueck();
					selu_anzeigen(TRUE);
				}
				throw_evntaway();
			}    
			else if (toupper(fm_kr)>='A' && toupper(fm_kr)<='Z')
			{	USER *sta;
				sta=gl_usstart;
				i=1;

				del_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);
				if (toupper(*sta->name)==toupper(fm_kr))
				{	gl_usindex=i;
					gl_uscur=sta;
					sel_obj=0;
					selu_anzeigen(TRUE);
				}
				else
				{
					while (sta->next)
					{	sta=sta->next;
						i++;
						if (toupper(*sta->name)==toupper(fm_kr))
						{	gl_usindex=i;
							gl_uscur=sta;
							sel_obj=0;
							while (gl_uscur->prior && gl_usindex>gl_uszahl-MAX_SELUSER)
							{	gl_uscur=gl_uscur->prior;
								gl_usindex--;
								sel_obj++;
							}
							set_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);

							selu_anzeigen(TRUE);
							break;
						}
					}
				}
				set_state(fm_tree,selu_obj[sel_obj],SELECTED,TRUE);
			}
			else if (fm_cont)
				fm_cont=FormKeybd(fm_tree,fm_edit_obj,fm_next_obj,fm_kr,0,&fm_next_obj,&fm_kr);

		}
		if (fm_which & MU_BUTTON)
		{
			fm_next_obj = objc_find(fm_tree,ROOT,MAX_DEPTH,fm_mx,fm_my);
			if (fm_next_obj == -1)
			{
				Dosound (fm_sound_dta);
				fm_next_obj = 0;
			}
			else if (fm_next_obj == SUFLYBTN)
			{
				graf_mouse(FLAT_HAND,(void *)0L);
				DialMove(fm_dial,gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
				mouse_normal();
			}
			else if (fm_next_obj == SUGRAU)
			{
				objc_offset(fm_tree, SUSLIDER, &dummy, &yslider);
				if (fm_my>yslider)  /* runterblaettern */
				{
					if (gl_uszahl>MAX_SELUSER && 
						gl_uszahl-MAX_SELUSER>=gl_usindex)
					{
						for (i=0;i<MAX_SELUSER;i++)
							selu_vor();
						selu_anzeigen(TRUE);
					}
				}
				else if (fm_my<yslider)     /* rauf */
				{
					if (gl_uszahl>MAX_SELUSER && gl_usindex>1)
					{
						for (i=0;i<MAX_SELUSER;i++)
							selu_zurueck();
						selu_anzeigen(TRUE);
					}
				}
				fm_cont=TRUE;
			}
			else if (fm_next_obj == SUUP)
			{
				if (gl_uszahl>MAX_SELUSER && gl_usindex>1)
				{
					selu_zurueck();
					selu_anzeigen(TRUE);
				}
			}
			else if (fm_next_obj == SUDOWN)
			{
				if (gl_uszahl>MAX_SELUSER && 
					gl_uszahl-MAX_SELUSER>=gl_usindex)
				{
					selu_vor();
					selu_anzeigen(TRUE);
				}
			}
			else if (fm_next_obj == SUSLIDER)
			{
				if (gl_uszahl>MAX_SELUSER)
				{
					pos=(long)graf_slidebox(seluser_tree,SUGRAU,SUSLIDER,1);
					pos=((long)gl_uszahl*pos)/1000L;
					lmsg=(word)pos;
					gl_uscur=gl_usstart;
					gl_usindex=1;
					for (i=0;i<lmsg;i++)
						selu_vor();	
					selu_anzeigen(TRUE);
				}
			}
			else
				fm_cont = FormButton(fm_tree,fm_next_obj,fm_br,&fm_next_obj);
		}
	}
	wind_update(END_MCTRL);
	return(fm_next_obj & 0x7fff);
}

static word sel_searchuser(byte *name,byte *retaddr,
														word only_pvtuser,byte *domain)
{	word i;
	word ext_btn, found;
	byte *uname, *unode;
	byte tname[MAX_STR];
	byte snode[40];
	byte tmp[MAX_STR+1];
	byte uvname[40];
	byte *cp;
	byte ini_suche, find_all, vorc;
	word slen;
	byte *reserve;
	byte filename[MAX_STR];
	FILE *fp;

	ini_suche=FALSE;
	find_all=FALSE;

	if (only_pvtuser)
		strcat(name,"*");

	if (*name=='*')   	/* alle User finden */
		find_all=TRUE;
	
	strcpy(tname,name);
	if (strlen(tname)==0L)
		return FALSE;
	
	if (strlen(tname)<=2 && !find_all)
	{
		if (strlen(tname)==2)
		{	ini_suche=TRUE;
			vorc=toupper(tname[0]);
			tname[0]=toupper(tname[1]);
			tname[1]=EOS;
		}
		else
		{	tname[0]=toupper(tname[0]);
			tname[1]=EOS;
			vorc=0;
		}
	}
	else
		vorc=0;

	reserve=(byte *)malloc(32768ul);

	memset(gl_uschar,0,sizeof(gl_uschar));
	gl_usstart=NULL;	
	gl_uszahl=0;
	found=FALSE;
	slen=(word)strlen(tname);
	for (i=0;i<MAX_USERLIST;i++)
	{	
		if ( (strlen(opt.userlist[i])>0L && !only_pvtuser) ||
				 (strlen(opt.userlist[i])>=12 && !stricmp(opt.userlist[i]+strlen(opt.userlist[i])-12,"PRVTUSER.LST")) )
		{
			strcpy(filename,opt.userlist[i]);
			if ((fp=buf_readinit(filename))!=NULL)
			{
				while (fgets(tmp,MAX_STR,fp))
				{
					if (strnicmp(tmp,tname,slen)==0 || find_all)
					{          /* Name gefunden */
						*uvname=EOS;
						unode=NULL;
						
						if (strchr(tmp,',')!=NULL)	cp=strtok(tmp,",");
						else												cp=strtok(tmp,", \r\n");
							
						if (cp==NULL)
							continue;

						uname=cp;
						while ((cp=strtok(NULL," \r\n"))!=NULL)
						{
							if (strchr(cp,'/')!=NULL)
							{	unode=cp;
								break;
							}
							else
							{	/* falls vorname nicht stimmt, abbrechen */
								if (ini_suche && toupper(*cp)!=vorc)
								{	*uvname=*cp;
									break;
								}
								strcat(uvname,cp);
								strcat(uvname," ");
							}
						}
							
						if (unode==NULL || *unode==EOS)
							continue;
						
						if (ini_suche && toupper(*uvname)!=vorc)
							continue;
						
						cut_endspc(uvname);

						if (*uvname)	sprintf(tmp,"%s, %s",uname,uvname);
						else					strcpy(tmp,uname);

						if (domain)
						{	uword x1,x2,x3,x4;
							byte hlp[32];
							scanaddr(unode,&x1,&x2,&x3,&x4,hlp);
							if (!(*hlp))
								strcpy(hlp,opt.userlistdomain[i]);
							if (*hlp)
								if (stricmp(hlp,domain))
									continue;		/* Richtiger Name, falsches Domain! */
/*						cp=strchr(unode,'@');
							if (cp) *cp=EOS; */
						}

						if (!user_store(tmp,unode,i))
						{
							i=MAX_USERLIST;
							/* fclose(fp);		Wieso? Wird doch gleich geschlossen... */
							LangAlert(MSG_NOMEMUSER);
							break;
						}
						gl_uszahl++;
						found=TRUE;
					}				
				}
				fclose(fp);
			}
			else
				LangAlert(MSG_USERLISTNOTFOUND);
		}
	}	/* for */

	if (reserve)
		free(reserve);
	
	if (found)
	{
		set_dtext(seluser_tree,SUPAT,name,FALSE,36);
		gl_uscur=gl_usstart;
		gl_usindex=1;
		selu_anzeigen(FALSE);
		if (gl_uszahl>1)
		{
			mouse_normal();
			show_mouse();

			if (gl_uszahl<MAX_SELUSER)
			{	for (i=(word)gl_uszahl;i<MAX_SELUSER;i++)
					set_flag(seluser_tree,selu_obj[i],HIDETREE,FALSE);
			}

			for (i=0;i<MAX_SELUSER;i++)
				if (req_state(seluser_tree,selu_obj[i],SELECTED))
					del_state(seluser_tree,selu_obj[i],SELECTED,FALSE);

			set_state(seluser_tree,selu_obj[0],SELECTED,FALSE);
			draw_dialog(seluser_tree, &seluser_dial);
			ext_btn=do_sform(seluser_tree,&seluser_dial,0);
			
			for (i=0;i<MAX_SELUSER;i++)
				del_flag(seluser_tree,selu_obj[i],HIDETREE,FALSE);
			if (ext_btn!=SUCANCEL)
			{
				for (i=0;i<MAX_SELUSER;i++)
					if (req_state(seluser_tree,selu_obj[i],SELECTED))
						break;
			}
			del_state(seluser_tree,ext_btn,SELECTED,FALSE);
			del_dialog(&seluser_dial);
		}
		else
		{
			i=0;
			ext_btn=SUOK;
		}


		if (ext_btn!=SUCANCEL)
		{
			get_dedit(seluser_tree,selu_obj[i],tmp);
			if (strchr(tmp,',')!=NULL)
				cp=strtok(tmp,",");
			else				
				cp=strtok(tmp,", \r\n");
			if (cp!=NULL)
			{
				strcpy(tname,cp);
				*name=EOS;
				while ((cp=strtok(NULL,", \r\n"))!=NULL)
				{
					if (strchr(cp,'/')!=NULL)
					{
						strcpy(snode,cp);
						break;
					}
					else
					{
						strcat(name,cp);
						strcat(name," ");
					}
				}
				strcat(name,tname);
				strcpy(retaddr,snode);
			}
			else
				ext_btn=SUCANCEL;
		}
		free_userlist();
		if (ext_btn==SUCANCEL)
			return(FALSE);
		else
			return(TRUE);	
	}
	return(FALSE);
}	

static void find_usernode(long win, char *tnode)
{	*tnode=EOS;

	if (is_ptowner(wi[win].hdr->sFrom,wi[win].aktarea) && is_netmail(wi[win].aktarea))		/* NETMAIL */
	{	strcpy(tnode,mergeaddr(wi[win].hdr->wToZone, wi[win].hdr->wToNet,
													 wi[win].hdr->wToNode, wi[win].hdr->wToPoint, NULL));
	}
	else
	{	get_origaddr(win,tnode);
	}
}

void save_user(long win)
{	byte tmp[MAX_STR];
	byte buffer[MAX_STR+1];
	byte tname[MAX_STR];
	byte tnode[60];
	byte sput[160];
	byte *cp;
	byte *blank;
	byte *nul;
	byte altfile[MAX_STR], neufile[MAX_STR];
	FILE *altfp, *neufp;
	word saved;
	word ret;
	word lstnr, slen;
	uword zone, net, node, point;
	
	if (wi[win].aktmsg>0)
	{
		if (is_ptowner(wi[win].hdr->sFrom,wi[win].aktarea) && is_netmail(wi[win].aktarea))
			strcpy(tmp,wi[win].hdr->sTo);
		else	
			strcpy(tmp,wi[win].hdr->sFrom);
		
		tmp[31]=EOS;
		cp=strchr(tmp,0);
		while (*(--cp)==' ')
			*cp=EOS;

		if (is_ptowner(tmp,wi[win].aktarea))
		{
			LangAlert(MSG_OWNNAME);
			return;
		}	

		lstnr=0;
		do
		{
			cp=strrchr(opt.userlist[lstnr],'\\');
			if (cp==NULL)
				cp=opt.userlist[lstnr];
			else
				cp++;
			lstnr++;
		} while (lstnr<MAX_USERLIST && *opt.userlist[lstnr-1] && stricmp(cp,"PRVTUSER.LST"));
		lstnr--;
		
		if (*opt.userlist[lstnr] && stricmp(cp,"PRVTUSER.LST"))
		{
			LangAlert(MSG_NOPRVTUSER);
			return;
		}
		
		find_usernode(win, tnode);

		if (gl_area[wi[win].aktarea].domain)
			if (*gl_area[wi[win].aktarea].domain)
			{	strcat(tnode,"@");
				strcat(tnode,gl_area[wi[win].aktarea].domain);
			}

		sprintf(buffer,msgtxt[MSG_SAVENEWUSER].text,tmp,tnode);
			
		if ((ret=FormAlert(msgtxt[MSG_SAVENEWUSER].defbut,msgtxt[MSG_SAVENEWUSER].icon,buffer,msgtxt[MSG_SAVENEWUSER].button))==1) /* Cancel */
			return;
			
		if (ret==2) /* New */
		{	scanaddr(tnode,&zone,&net,&node,&point,NULL);
			init_userdia(tmp,zone,net,node,point);
			if (userbox(0,TRUE,NNTO,tmp,&zone,&net,&node,&point))
				strcpy(tnode,mergeaddr(zone,net,node,point,NULL));
			else
				return;
		}	
		
		blank=strchr(tmp,' ');
		if (blank!=NULL)
		{
			strncpy(tname,blank+1,strlen(blank));
			strcat(tname,", ");
			*blank=EOS;
			strcat(tname,tmp);
		}
		else
		{
			strcpy(tname,tmp);
		}
	
		sprintf(sput,"%-35s %s\n",tname,tnode);

	/* LST File */
		
		if (*opt.userlist[lstnr])
			strcpy(altfile,opt.userlist[lstnr]);
		else
		{
			sprintf(altfile,"%sPRVTUSER.LST",gl_ledpath);
			strcpy(opt.userlist[lstnr],altfile);
		}
		strcpy(neufile,altfile);
		cp=strrchr(neufile,'.');
		if (cp==NULL)
		{
			cp=strchr(neufile,EOS);
			*(cp-1)='$';
		}
		else
			strcpy(cp+1,"$$$");
		
		biene();
		saved=FALSE;
		if ((altfp=fopen(altfile,"r"))==NULL)
		{
			altfp=fopen(altfile,"w");
			fputs(sput,altfp);
			saved=TRUE;
			fclose(altfp);
		}
		else
		{		
			neufp=fopen(neufile,"w");
			fseek(neufp,0L,SEEK_SET);
			fseek(altfp,0L,SEEK_SET);

			slen=(word)strlen(tname);

			while (fgets(buffer,MAX_STR,altfp)!=NULL)
			{
				if (strnicmp(buffer,sput,slen)<0)
				{
					fputs(buffer,neufp);
				}
				else if (strnicmp(buffer,sput,slen)>0)
				{
					if (!saved)
						fputs(sput,neufp);
					fputs(buffer,neufp);
					saved=TRUE;
				} 
				else  /* Name ist bereits im LST */
				{
					fputs(sput,neufp);
					nul=strchr(buffer,EOS);
					while (*(nul-1)!=' ')
						nul--;
					if (strncmp(nul,tnode,strlen(tnode))!=0)	
						fputs(buffer,neufp);
					saved=TRUE;	
				}	
			} /* while */
			
			if (!saved)
			{
				fputs(sput,neufp);
			}	
			
			fclose(altfp);	
			fclose(neufp);
			Fdelete(altfile);
			Frename(0,neufile,altfile);
		}
		mouse_normal();
	}
}

void nnstr2node(byte *s, uword *zone, uword *net, uword *node, uword *point)
{	byte *cp;
	byte szone[6], snet[6], snode[6], spoint[6];
	word i;
	long lzone, lnet, lnode, lpoint;

	*zone = 0;
	*net = 0;
	*node = 0;
	*point = 0;
	strcpy(szone, "     ");
	strcpy(snet,  "     ");
	strcpy(snode, "     ");
	strcpy(spoint,"     ");
	
	/* s = <2    507  414  1   5> */
	
	cp=s;
	i=0;
	while (*cp!=EOS && i < 5)
		szone[i++]=*cp++;
	i=0;
	while (*cp!=EOS && i < 5)
		snet[i++]=*cp++;
	i=0;
	while (*cp!=EOS && i < 5)
		snode[i++]=*cp++;
	i=0;
	while (*cp!=EOS && i < 5)
		spoint[i++]=*cp++;

	lzone=atol(szone);
	lnet=atol(snet);
	lnode=atol(snode);
	lpoint=atol(spoint);

	*zone=(uword)lzone;
	*net=(uword)lnet;
	*node=(uword)lnode;
	*point=(uword)lpoint;

	if (lzone < 0L || lzone > 65535L)
		*zone=addr[0].zone;
	if (lnet < 0L || lnet > 65535L)
		*net=addr[0].net;
	if (lnode < 0L || lnode > 65535L)
		*node=addr[0].node;
	if (lpoint < 0L || lpoint > 65535L)
		*point=addr[0].point;
}

void init_userdia(byte *s, uword zone, uword net, uword node, uword point)
{	byte tmp[50];
	get_dedit(nodenum_tree,NNTO,tmp);
	sprintf(tmp,"%-5u%-5u%-5u%-5u",zone,net,node,point);
	set_dtext(nodenum_tree,NNSTRING,tmp,FALSE,20);
	set_dtext(nodenum_tree,NNTO,s,FALSE,36);
	set_dtext(nodenum_tree,NNFOUND,"              ",FALSE,14);
}

/*
** 0 = Nicht gefunden
** 1 = Gefunden
*/

word xsrch_user(word only_pvtuser,byte *name,byte *domain, 
							 uword *zone,uword *net,uword *node,uword *point)
{	word found=FALSE;
	byte tname[60],tnode[30];
	byte *cp;
	
	strcpy(tname,name);
	cp=del_allblanks(tname);
	strcpy(tname,cp);

	if (strlen(tname)>0L || only_pvtuser)
	{
		cp=strchr(tname,' ');
		if (cp!=NULL)
			found=search_userlist(tname,tnode,domain);
		else
			found=sel_searchuser(tname,tnode,only_pvtuser,domain);

		if (found)
		{	cp=del_allblanks(tname);
			strcpy(name,cp);
			scanaddr(tnode,zone,net,node,point,NULL);
		}
		else
		{	*zone=addr[0].zone;
			*net=addr[0].net;
			*node=addr[0].node;
			*point=addr[0].point;
			strcpy(name,tname);
		}
	}

	else	/* suche nach nodenummer */
	{
		if (*zone!=0 && *net!=0)
		{	
			found=FALSE;

			if (*gl_nodelistpath)
			{	NLSYSTEM buf;
				found=search_bnl_usernode(*zone,*net,*node,*point,&buf,NULL);
				strcpy(tname,buf.operator);
			}

			if (!found)
				found=search_usernode(tname,*zone,*net,*node,*point);

			if (found)
			{	cp=del_allblanks(tname);
				strcpy(name,cp);
			}
		}
	}
	return(found);
}

word xget_user_nobox(byte *name, word only_pvtuser, word waitdialog,
						 uword *rzone,uword *rnet,uword *rnode,uword *rpoint,
						 byte *domain)
{ uword zone,net,node,point;
	word found=FALSE;

	if (waitdialog) show_doing(DO_SEARCHINUSERLIST);

	biene();
	if (xsrch_user(only_pvtuser,name,domain,&zone,&net,&node,&point))
	{	*rzone=zone; *rnet=net; *rnode=node; *rpoint=point;
		found=TRUE;
	}
	mouse_normal();

	if (waitdialog) del_doing();
	return(found);
}

static void xget_user(byte *name, word only_pvtuser, word waitdialog,
						 uword zone,uword net,uword node,uword point)
{ byte *cp,tnode[40];

	if (waitdialog) show_doing(DO_SEARCHINUSERLIST);
	biene();
	
	if (!xsrch_user(only_pvtuser,name,NULL,&zone,&net,&node,&point))
	{	set_dtext(nodenum_tree,NNFOUND,msgtxt[SYSTEM_NOTFOUND].text,FALSE,14);

		sprintf(tnode,"%-5u%-5u%-5u%-5u",addr[0].zone,addr[0].net,addr[0].node,addr[0].point);
		set_dtext(nodenum_tree,NNSTRING,tnode,FALSE,20);
	}
	else
	{	set_dtext(nodenum_tree,NNFOUND,"              ",FALSE,14);
		cp=del_allblanks(name);
		set_dtext(nodenum_tree,NNTO,cp,FALSE,36);

		sprintf(tnode,"%-5u%-5u%-5u%-5u",zone,net,node,point);
		set_dtext(nodenum_tree,NNSTRING,tnode,FALSE,20);
	}

	mouse_normal();

	if (waitdialog) del_doing();
}

/*
** 0 = Cancel
** 1 = OK
** mode = 0 : "Edit address"
** mode = 1 : "Forward Msg"
*/

word userbox(word mode,word waitdialog,word startobj,byte *rname,
						 uword *rzone,uword *rnet,uword *rnode,uword *rpoint)
{
	byte nns[40], name[60];
	word ext_btn, i;
	uword zone,net,node,point;
	
	show_mouse();

	set_dtext(nodenum_tree,NNFOUND,"              ",FALSE,14);

	switch(mode)
	{	case 0	:	set_dstring(nodenum_tree,NNINFO,msgtxt[SYSTEM_EDITADDRESS].text,FALSE,16); break;
		case 1	:	set_dstring(nodenum_tree,NNINFO,msgtxt[SYSTEM_FORWARDADDR].text,FALSE,16); break;
		case 2	:	set_dstring(nodenum_tree,NNINFO,msgtxt[SYSTEM_SEARCHUSER].text ,FALSE,16); break;
		default	:	set_dstring(nodenum_tree,NNINFO,msgtxt[SYSTEM_UNKNOWN].text		 ,FALSE,16); break;
	}

	for(;;)
	{
		draw_dialog(nodenum_tree, &nodenum_dial);

		do
		{	ext_btn=Form_do(&nodenum_dial,startobj);

			if (ext_btn==NNUSERL)
			{	get_dedit(nodenum_tree,NNTO,name);
				get_dedit(nodenum_tree,NNSTRING,nns);

				del_dialog(&nodenum_dial);

				nnstr2node(nns,&zone,&net,&node,&point);

				xget_user(name,FALSE,waitdialog,zone,net,node,point);
				draw_dialog(nodenum_tree, &nodenum_dial);
				del_state(nodenum_tree,NNUSERL,SELECTED,TRUE);

			}
		} while (ext_btn==NNUSERL);
		del_dialog(&nodenum_dial);

		del_state(nodenum_tree,ext_btn,SELECTED,FALSE);
			
		if (ext_btn==NNCANCEL)
			return(FALSE);
			
		get_dedit(nodenum_tree,NNTO,name);

		if (*name==EOS)
		{	ext_btn=0;
			for (i=0;i<MAX_USERLIST;i++)
				if (strlen(opt.userlist[i])>=12 &&
						!stricmp(opt.userlist[i]+strlen(opt.userlist[i])-12,"PRVTUSER.LST"))
					break;
			if (i<MAX_USERLIST)
			{	xget_user(name,TRUE,waitdialog,0,0,0,0);
			}
		}
		if (ext_btn==NNOK)
			break;
	}	/* for(;;) */

	if (rname) get_dedit(nodenum_tree,NNTO,rname);

	get_dedit(nodenum_tree,NNSTRING,nns);

	if (rzone) nnstr2node(nns,rzone,rnet,rnode,rpoint);

	show_mouse();
	return(TRUE);
}
