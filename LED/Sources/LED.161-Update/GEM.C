#include "ccport.h"
#include "doing.rh"
#include "ledgl.h"
#include "ledfc.h"
#include "language.h"

static word drawn = FALSE;
static word hidden = FALSE;

word work_in[11], work_out[57];

word LangAlert(word entry)
{	word  ret;
	BITBLK *image;
	
	switch (msgtxt[entry].icon)
	{	case (0):	image=ImInfo();					break;
		case (1):	image=ImExclamation();	break;
		case (2):	image=ImQuestionMark();	break;
		case (3):	image=ImHand();					break;		
		default:	image=ImInfo();
	}
	ret=DialAlert(image,msgtxt[entry].text,msgtxt[entry].defbut-1,msgtxt[entry].button)+1;
	return(ret);
}

int FormAlert(int defbut, int icon, char *text, char *buttext)
{
/*form_alert(1,"[3][config_init:|Configuration file|not found!][  OK  ]");*/
/*FormAler_(1, 3, "config_init:|Configuration file|not found!", "  [OK  ");*/ 
	int  ret;
	BITBLK *image;
	
	switch (icon)
	{
		case (0):
			image=ImInfo();
			break;
		case (1):
			image=ImExclamation();
			break;
		case (2):
			image=ImQuestionMark();
			break;
		case (3):
			image=ImHand();
			break;		
		default:
			image=ImInfo();
	}
	defbut--;
	ret=DialAlert(image, text, defbut, buttext);
	ret++;
	return(ret);
}

void hide_mouse(void)
{	if (!hidden)
	{	v_hide_c(handle);
		hidden=TRUE;
	}
}

void show_mouse(void)
{	if (hidden)
	{ v_show_c(handle,0);
		hidden=FALSE;
	}
}

word hole_file(byte *file, byte *nurname, byte *ext, byte *titel)
{	word err;
	word exit_bttn;
	byte *cp;
/*	byte name[14];*/
	byte name[MAX_STR];/* Mike 14/7/00 */
	byte path[MAX_STR];     
	
	if (nurname!=NULL && strlen(nurname)<13 && strchr(nurname,'.')!=NULL)
		strcpy(name,nurname);
	else
		*name=EOS;
	
	if (file && *file)
	{
		cp=strrchr(file,'\\');
		if (cp!=NULL)
		{
			if (*(cp+1)!=EOS && nurname==NULL)
				strcpy(name,cp+1);
			*(cp+1)=EOS;
			strcpy(path,file);
		}
		else
			strcpy(path,fselpath);
	}
	else
		strcpy(path,fselpath);
	
	strcat(path,ext);
	
	do
	{
		if (aes_version >= 0x140)
			err=fsel_exinput(path, name, &exit_bttn, titel);
		else
			err=fsel_input(path, name, &exit_bttn);
	} while (err!=0 && (exit_bttn==1) && (*name==EOS));
	
	if (err==0)
		exit_bttn=0;
	
	if (exit_bttn==1)
	{
		strcpy(file,path);
		cp=strrchr(file,'\\');
		if (cp!=NULL)
			*(cp+1)=EOS;
		strcpy(fselpath,file);
		if (nurname!=NULL)
			strcpy(nurname,name);
		strcat(file,name);
	}
	return(exit_bttn);
}   


void draw_dialog(OBJECT *tree, DIALINFO *dial)
{	DialStart(tree, dial);
	DialDraw(dial);
}


void del_dialog(DIALINFO *dial)
{	DialEnd(dial);
}

void redraw_obj(OBJECT *tree, word index)
{	ObjcDraw(tree,index,0,tree->ob_x,tree->ob_y,tree->ob_width,tree->ob_height);
}

int Form_do(DIALINFO *dial, int fo_dostartob )
{	word ret;
	ret=DialDo(dial, fo_dostartob);
	return (ret & 0x7fff);
}

void set_dtext(OBJECT *tree, word index, char *s, word redraw, word len)
{	if (strlen(s)>len)
	{	strncpy(tree[index].ob_spec.tedinfo->te_ptext,s,len);
		tree[index].ob_spec.tedinfo->te_ptext[len]=EOS;
	}
	else
		strcpy(tree[index].ob_spec.tedinfo->te_ptext,s);
	if (redraw)
		redraw_obj(tree,index);
}

void set_dstring(OBJECT *tree, word index, char *s, word redraw, word len)
{	if (strlen(s)>len)
	{ strncpy(tree[index].ob_spec.tedinfo->te_ptmplt,s,len);
		tree[index].ob_spec.tedinfo->te_ptmplt[len]=EOS;
	}
	else
		strcpy(tree[index].ob_spec.tedinfo->te_ptmplt,s);
	tree[index].ob_width=min((int)strlen(s),len)*gl_syswchar;
	if (redraw)
		redraw_obj(tree,index);
}

void set_dbutton(OBJECT *tree, word index, char *s, word redraw)
{	strcpy(tree[index].ob_spec.free_string,s);
	if (redraw)
		redraw_obj(tree,index);
}

word is_text(OBJECT *tree, word index)
{	if ( tree[index].ob_spec.tedinfo->te_ptext[0]==EOS)
		return FALSE;
	else
		return TRUE;
}
			
void get_dedit(OBJECT *tree, word index, char *s)
{	strcpy(s,tree[index].ob_spec.tedinfo->te_ptext);
}

void get_dbutton(OBJECT *tree, word index, char *s)
{ strcpy(s,tree[index].ob_spec.free_string);
}

void set_state(OBJECT *tree, word index, word state, word redraw)
{	tree[index].ob_state |= state;
	if (redraw)
		redraw_obj(tree,index);
}

void del_state(OBJECT *tree, word index, word state, word redraw)
{	tree[index].ob_state &= ~state;
	if (redraw)
		redraw_obj(tree,index);
}

void set_mstate(OBJECT *tree, word state, word redraw, ...)
{	va_list ap;
	word index;
	va_start(ap, redraw);
	index = va_arg(ap,word);
	while (index != -1)
	{	tree[index].ob_state |= state;
		if (redraw)
			redraw_obj(tree,index);
		index = va_arg(ap,word);
	}
	va_end(ap);
}

void del_mstate(OBJECT *tree, word state, word redraw, ...)
{	va_list ap;
	word index;
	va_start(ap, redraw);
	index = va_arg(ap,word);
	while (index != -1)
	{	tree[index].ob_state &= ~state;
		if (redraw)
			redraw_obj(tree,index);
		index = va_arg(ap,word);
	}
	va_end(ap);
}

word req_state(OBJECT *tree, word index, word state)
{	if ((tree[index].ob_state & state) == state)
		return TRUE;
	else
		return FALSE;
}		 

word req_flag(OBJECT *tree, word index, word flag)
{	if ((tree[index].ob_flags & flag) == flag)
		return TRUE;
	else
		return FALSE;
}

void set_flag(OBJECT *tree, word index, word flag, word redraw)
{	tree[index].ob_flags |= flag;
	if (redraw)
		redraw_obj(tree,index);
}

void del_flag(OBJECT *tree, word index, word flag, word redraw)
{	tree[index].ob_flags &= ~flag;
	if (redraw)
		redraw_obj(tree,index);
}

void set_mflag(OBJECT *tree, word flag, word redraw, ...)
{	va_list ap;
	word index;
	va_start(ap, redraw);
	index = va_arg(ap,word);
	while (index != -1)
	{	tree[index].ob_flags |= flag;
		if (redraw)
			redraw_obj(tree,index);
		index = va_arg(ap,word);
	}
	va_end(ap);
}

void del_mflag(OBJECT *tree, word flag, word redraw, ...)
{	va_list ap;
	word index;
	va_start(ap, redraw);
	index = va_arg(ap,word);
	while (index != -1)
	{	tree[index].ob_flags &= ~flag;
		if (redraw)
			redraw_obj(tree,index);
		index = va_arg(ap,word);
	}
	va_end(ap);
}

word open_vwork(void)
{	word i,phys_handle;
	if ((gl_apid = appl_init()) != -1)
	{
#if 0		/* so sieht's in AVALON aus */
		for(i=1; i<10; _VDIParBlk.intin[i++]=1);
		_VDIParBlk.intin[0]=1;			/* SCREEN	*/
		_VDIParBlk.intin[10]=2;			/* RC			*/
#endif

		for (i=1; i<10; work_in[i++]=0);
		work_in[ 0] = 1;
		work_in[10] = 2;
		phys_handle = graf_handle(&gl_syswchar, &gl_syshchar, &gl_syswbox, &gl_syshbox);
/*	gl_wchar=gl_syswchar;
		gl_hchar=gl_syshchar;
		gl_wbox =gl_syswbox;
		gl_hbox =gl_syshbox; */
		work_in[0] = 1;
		handle = phys_handle;
		v_opnvwk(work_in, &handle, work_out);
		DialInit(malloc, free);
		load_fonts();
		return(TRUE);
	}
	else 
		return(FALSE);
}

void ende(void)
{	menu_bar(menu_tree,0);
	appl_exit();
}

void set_desktop(void)
{	wind_get (0, WF_FULLXYWH, &gl_xdesk, &gl_ydesk, &gl_wdesk, &gl_hdesk); 
	form_dial(FMD_START,0,0,0,0,gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
	
#if FALSE
	screen_tree->ob_x = gl_xdesk;
	screen_tree->ob_y = gl_ydesk;
	screen_tree->ob_width = gl_wdesk;
	screen_tree->ob_height = gl_hdesk;
#endif	
	gl_x2desk=gl_xdesk+gl_wdesk;
	gl_y2desk=gl_ydesk+gl_hdesk;
	
/*	wind_set(0,WF_NEWDESK,screen_tree,0);*/

	form_dial(FMD_FINISH,0,0,0,0,gl_xdesk,gl_ydesk,gl_wdesk,gl_hdesk);
}

void show_doing(word s)
{	if (!drawn)
	{	set_dtext(doing_tree,DOTEXT,msgtxt[s].text,FALSE,43);
		drawn=TRUE;
		draw_dialog(doing_tree, &doing_dial);
	}	
	else
	{	set_dtext(doing_tree,DOTEXT,"                                         ",TRUE,43); /* 43 */
		set_dtext(doing_tree,DOTEXT,msgtxt[s].text,TRUE,43);
	}	
}

void show_doingtext(byte *s)
{	byte tmp[MAX_STR],*cp1,*cp2;
	word rlen;

	strcpy(tmp,s);

	if (strlen(tmp)>43)
	{	cp2=strchr(tmp,'\\');		/* Hat nur Sinn, wenn mindestens zwei Backslashes vorhanden! */
		cp1=strrchr(tmp,'\\');
		if (cp1 && cp2 && cp1!=cp2)
		{	cp2=cp1;
			rlen=(word)(strchr(cp1,EOS)-cp2);
			while(cp2>=tmp && (cp2-tmp+1 + rlen>40) )
			{	cp2--;
				while (cp2>=tmp && *cp2!='\\')
					cp2--;
			}
			if (cp2>=tmp)
			{	*(cp2+1)=EOS;
				strcat(tmp,"~~~");
				strcat(tmp,cp1);
			}
		}
	}
	
	if (!drawn)
	{	set_dtext(doing_tree,DOTEXT,tmp,FALSE,43);
		drawn=TRUE;
		draw_dialog(doing_tree, &doing_dial);
	}	
	else
	{	set_dtext(doing_tree,DOTEXT,"                                         ",TRUE,43); /* 43 */
		set_dtext(doing_tree,DOTEXT,tmp,TRUE,43);
	}	
}

void del_doing(void)
{	if (drawn)
	{	drawn=FALSE;
		del_dialog(&doing_dial);
	}
}


word req_button(OBJECT *tree, word index)
{	if ((tree[index].ob_state & SELECTED) == SELECTED)
		return TRUE;
	else
		return FALSE;
}

void del_button(OBJECT *tree, word index, word redraw)
{	tree[index].ob_state &= ~SELECTED;
	if (redraw)
		redraw_obj(tree,index);
}

void set_button(OBJECT *tree, word index, word redraw)
{	tree[index].ob_state |= SELECTED;
	if (redraw)
		redraw_obj(tree,index);
}
