#include "ccport.h"
#include <ext.h>
#include "led.h"
#include "ledgl.h"
#include "ledfc.h"
#include "vars.h"
#include "language.h"

#define DEBUG       FALSE

#define TREE_VERSION 10
#define TREES_ANZ    35

extern word betatest;

static OBJECT *trees[TREES_ANZ];

static word appl_xgetinfo(word type,word *out1,word *out2,word *out3,word *out4)
{ word has_agi=FALSE;
  ulong dummy;
  has_agi = (  (_GemParBlk.global[0] == 0x399 && getcookie('MagX', &dummy))
            || (_GemParBlk.global[0] == 0x400 && type < 4)
            || (_GemParBlk.global[0] > 0x400)
            || (appl_find ("?AGI") >= 0));
  if (has_agi)
  	return (appl_getinfo(type,out1,out2,out3,out4));
  else return (0);
}

static word read_nicknames(void)
{	byte file[MAX_STR],buf[MAX_STR],*cp;
	FILE *fp;
	word cnt=0;

	sprintf(file,"%sLED.NIC",gl_ledpath);
	if ((fp=fopen(file, "r"))==NULL)
		return(0);
	else
	{	while (fgets(buf,MAX_STR-1,fp)!=NULL)
		{	cp=skip_blanks(buf);
			if (*cp==';' || !*cp)
				continue;
			else if (strlen(cp)<5)
				continue;
			else 
			{	strcpy(buf,cp);
				if (cnt>=MAX_NICKNAMES)
					return(cnt);

				cp=strtok(buf,",\n\r");
				if (cp)
				{	cut_endspc(buf);
					cp=strtok(NULL,",\n\r");
					if (cp && *cp)
					{	nicknames[cnt][0]=strdup(buf);

						cp=skip_blanks(cp);
						cut_endspc(cp);
						nicknames[cnt][1]=strdup(cp);

						cp=strtok(NULL,",\n\r");
						if (cp)
						{	cp=skip_blanks(cp);
							cut_endspc(cp);
							nicknames[cnt][2]=strdup(cp);
						}
						else
							nicknames[cnt][2]=NULL;

						cnt++;
					}
				}
			}
		}
		fclose(fp);
	}
	return(cnt);
}

void set_dialpos(void)
{	FILE *fp;
	word x, y, w, h, i;
	word screenx, screeny;
	word tree_pos[TREES_ANZ][2];
	byte treeversion;
	byte tmp[MAX_STR]; 	

	treeversion=0;
	sprintf(tmp,"%sLED.DLG",gl_ledpath);
	if ((fp=fopen(tmp, "rb"))!=NULL)
	{	
		fread(&treeversion, sizeof(char), 1, fp);
		fread(&screenx, sizeof(word), 1, fp);
		fread(&screeny, sizeof(word), 1, fp);
		if (treeversion==TREE_VERSION && 			/* is richtige Version ? */
		    screenx>0 && screeny>0)
		{
			fread(&tree_pos, 2*sizeof(word), TREES_ANZ, fp);
			for (i=0;i<TREES_ANZ;i++)
			{
				x=tree_pos[i][0];
				y=tree_pos[i][1];
				w=trees[i]->ob_width;
				h=trees[i]->ob_height;
				
				/* anpassen an aktuelle Auflîsung */
				x=(word)((long)(gl_wdesk+gl_xdesk)*(long)x/(long)screenx);
				y=(word)((long)(gl_hdesk+gl_ydesk)*(long)y/(long)screeny);
				x-=w/2;
				y-=h/2;
				
				if (x<gl_xdesk ||	y<gl_ydesk || x+w>gl_wdesk+gl_xdesk || y+h>gl_hdesk+gl_ydesk)
				{	DialCenter(trees[i]);
				}
				else
				{	if (x<gl_xdesk)	x=gl_xdesk+1;
					if (y<gl_ydesk)	y=gl_ydesk+1;
					if (x+w>gl_wdesk+gl_xdesk)	x=gl_wdesk+gl_xdesk-w-1;
					if (y+h>gl_hdesk+gl_ydesk)	y=gl_hdesk+gl_ydesk-h-1;
					trees[i]->ob_x=x;
					trees[i]->ob_y=y;
				}
				
				ObjcTreeInit(trees[i]);
			}
		}
		else		/* altes DLG-File, alle Dialoge centern */
		{
			for (i=0;i<TREES_ANZ;i++)
			{
				DialCenter(trees[i]);
				ObjcTreeInit(trees[i]);
			}
		}		
		fclose(fp);
	}
	else
	{
		for (i=0;i<TREES_ANZ;i++)
		{
			DialCenter(trees[i]);
			ObjcTreeInit(trees[i]);
		}
	}		
}

void save_dialpos(void)
{	FILE *fp;
	word i, w, h;
	word screenx, screeny;
	word tree_pos[TREES_ANZ][2];
	byte treeversion;
	byte tmp[MAX_STR];
	
	if (opt.savedlg)
	{
		treeversion=TREE_VERSION;
		screenx=gl_xdesk+gl_wdesk;
		screeny=gl_ydesk+gl_hdesk;
		sprintf(tmp,"%sLED.DLG",gl_ledpath);
		if ( (fp=fopen(tmp, "wb"))!=NULL )
		{	
			fwrite(&treeversion, sizeof(char), 1, fp);
			fwrite(&screenx, sizeof(word), 1, fp);
			fwrite(&screeny, sizeof(word), 1, fp);
			for (i=0;i<TREES_ANZ;i++)
			{
				tree_pos[i][0]=trees[i]->ob_x;
				tree_pos[i][1]=trees[i]->ob_y;
				w=trees[i]->ob_width;
				h=trees[i]->ob_height;
				/* Dialog-Mitte abspeichern */
				tree_pos[i][0]+=w/2;
				tree_pos[i][1]+=h/2;
			}	
			fwrite(&tree_pos, 2*sizeof(int), TREES_ANZ, fp);
		}
	}
}


/*
** -> Pfad auf gesuchte Datei, oder ""
*/

static byte *test_file(byte *path,byte *filename)
{	static byte retpath[MAX_STR];
	byte tmp[MAX_STR],*cp;
	struct ffblk dta;

	cp=skip_blanks(path);
	strncpy(retpath,cp,MAX_STR);
	cp=strchr(retpath,' ');
	if (cp)	*cp=EOS;
	add_bslash(retpath);
	strcpy(tmp,retpath);
	strcat(tmp,filename);
	if (findfirst(tmp,&dta,0))
		*retpath=EOS;		/* nicht gefunden */
	return(retpath);
}

/*
**	-> Pfad auf gesuchte Datei, oder "."
*/

static byte *search_path(byte *filename)
{	static byte tmp[MAX_STR];
	byte dum[MAX_STR],*line;
	struct ffblk dta;
	
/*
**	Testen, ob es eine Environmentvariable dafÅr gibt:
**	Hîchste PrioritÑt: $FIDOCONFDIR
**										 $BINKLEY
**										 $MAILER
**										 .
*/

	*tmp=EOS;

	line=getenv("FIDOCONFDIR");
	if (!line)
	{	sprintf(dum,"%sCONFIG",fselpath);
		if (!findfirst(dum,&dta,FA_DIREC))
			line=dum;
	}
	if (line)
		strcpy(tmp,test_file(line,filename));

	if (!*tmp)
	{	line=getenv("BINKLEY");
		if (line)
			strcpy(tmp,test_file(line,filename));
	}

	if (!*tmp)
	{	line=getenv("MAILER");
		if (line)
			strcpy(tmp,test_file(line,filename));
	}

	if (!*tmp)
		strcpy(tmp,fselpath);

	return(tmp);
}

static word load_rscfile(byte *file)
{	word i, k;
	OBJECT *obj;
	byte xtype, type;

	if (!rsrc_load(file))
		return(FALSE);

	rsrc_gaddr(0, MENU, &menu_tree);
	rsrc_gaddr(0, ABOUT, &about_tree);
	rsrc_gaddr(0, ALLAREAS, &allarea_tree);
	rsrc_gaddr(0, LISTMSG, &listmsg_tree);
	rsrc_gaddr(0, GOTOMSG, &gomsg_tree);
	rsrc_gaddr(0, EDITFLAG, &edflag_tree);
	rsrc_gaddr(0, DOING, &doing_tree);
	rsrc_gaddr(0, KILLMSG, &killmsg_tree);
	rsrc_gaddr(0, EDITLINE, &edline_tree);
	rsrc_gaddr(0, NODENUM, &nodenum_tree);
	rsrc_gaddr(0, HELPMSG, &hmsg_tree);
	rsrc_gaddr(0, HELPEDIT, &hedit_tree);
	rsrc_gaddr(0, HELPDIAL, &hdial_tree);
	rsrc_gaddr(0, FREQ, &freq_tree);
	rsrc_gaddr(0, HEADER, &header_tree);
	rsrc_gaddr(0, HEADERF, &header2_tree);
	rsrc_gaddr(0, HEADERS, &header3_tree);
	rsrc_gaddr(0, FIND, &find_tree);
	rsrc_gaddr(0, FKEYS, &fkeys_tree);
	rsrc_gaddr(0, FSAME, &fsame_tree);
	rsrc_gaddr(0, CARBON, &carbon_tree);
	rsrc_gaddr(0, SELUSER, &seluser_tree);
	rsrc_gaddr(0, FONT, &font_tree);
	rsrc_gaddr(0, EXECDLG, &exec_tree);
	rsrc_gaddr(0, WRITEMSG, &writemsg_tree);
	rsrc_gaddr(0, SWITCHES, &switches_tree);
	rsrc_gaddr(0, WINDOW, &window_tree);
	rsrc_gaddr(0, QUOTE, &quote_tree);
	rsrc_gaddr(0, USERLIST, &user_tree);
	rsrc_gaddr(0, HELPCOM, &helpcom_tree);
	rsrc_gaddr(0, USERNAME, &username_tree);
	rsrc_gaddr(0, COMMENT, &comment_tree);
	rsrc_gaddr(0, DINFO, &info_tree);
	rsrc_gaddr(0, HELPMSG2, &hmsg2_tree);
	rsrc_gaddr(0, SELORIG, &selorig_tree);
	rsrc_gaddr(0, MSGINFO, &msginfo_tree);
	rsrc_gaddr(0, THANXBOX,&thanks_tree);
	rsrc_gaddr(0, LEDICONS,&icons_tree);
	
	trees[0]=about_tree;
	trees[1]=allarea_tree; 
	trees[2]=listmsg_tree;
	trees[3]=gomsg_tree; 
	trees[4]=switches_tree; 
	trees[5]=edflag_tree;
	trees[6]=doing_tree; 
	trees[7]=killmsg_tree; 
	trees[8]=edline_tree; 
	trees[9]=nodenum_tree; 
	trees[10]=hmsg_tree; 
	trees[11]=hedit_tree; 
	trees[12]=hdial_tree; 
	trees[13]=freq_tree; 
	trees[14]=header_tree;
	trees[15]=header2_tree;
	trees[16]=find_tree; 
	trees[17]=fkeys_tree; 
	trees[18]=fsame_tree; 
	trees[19]=carbon_tree; 
	trees[20]=seluser_tree;
	trees[21]=font_tree; 
  trees[22]=writemsg_tree;
  trees[23]=window_tree;
  trees[24]=quote_tree;
  trees[25]=user_tree;
  trees[26]=username_tree;
  trees[27]=helpcom_tree;
  trees[28]=comment_tree;
  trees[29]=hmsg2_tree;
  trees[30]=info_tree;
	trees[31]=selorig_tree;
	trees[32]=msginfo_tree;
	trees[33]=thanks_tree;
	trees[34]=header3_tree;
	
	MenuTune(menu_tree, FALSE);

	for (i=0;i<TREES_ANZ;i++)
	{	k=1;
		do
		{	obj=&trees[i][k];
			type=(byte)obj->ob_type;
			if (type == G_BUTTON)
			{	xtype=obj->ob_type >> 8;
				if (xtype == 18)
					if (obj->ob_flags & SELECTABLE) 
						if ( !(obj->ob_flags & EXIT) &&
						     !(obj->ob_flags & TOUCHEXIT) )
							obj->ob_height-=1;
			}
			k++;
		} while ( !(obj->ob_flags & LASTOB) );
	}

	exec_tree->ob_x = exec_tree->ob_y = 0;
	exec_tree->ob_width = full.w;
	exec_tree->ob_height += 2;
	exec_tree[EXETITLE].ob_x=(full.w-exec_tree[EXETITLE].ob_width)/2;
	exec_tree[EXETITLE].ob_y=2;
	set_menu(MSG_WIND);
	menu_bar(menu_tree, 1);
	return(TRUE);
}

word deklaration(void)
{	long win;
	word i,j,found,p[4];
	byte *cp;
	byte *line;
	byte tmp[MAX_STR];
	struct ffblk dta;
	FILE *fp;
	
	biene();

	aes_version = _GemParBlk.global[0];

	iconify_possible=FALSE;
	if (appl_xgetinfo(11,&p[0],&p[1],&p[2],&p[3]))
		if ( (p[0] & (128|256))==(128|256) &&
				 (p[2] & (			1))==(			 1) )
			if (appl_xgetinfo(12,&p[0],&p[1],&p[2],&p[3]))
				if ( (p[0] & (128|256|512))==(128|256|512) &&
						 (p[2]) )
					iconify_possible=TRUE;

	unreg[3]='r';
	unreg[5]='g';
	unreg[1]='u';
	unreg[6]=EOS;
	unreg[2]='n';
	unreg[4]='e';
	unreg[0]='/';
	
	printf("\x1bv");   	/* Wrap on */
	
	Pdomain(0);
	
	gl_key_p = Keytbl((void *)-1L,(void *)-1L,(void *)-1L);

	wind_get(0, WF_FULLXYWH, &gl_xdesk, &gl_ydesk, &gl_wdesk, &gl_hdesk);
	wind_get(0,WF_WORKXYWH,&full.x,&full.y,&full.w,&full.h);
	if (full.x < 3) /* 3 */
		full.x = 3;
	
	if (full.w<640)
	{	mouse_normal();
		FormAlert(1,3,"Application needs higher|resolution!"," [Sorry ");
		return(FALSE);
	}
	
	if (is_running("JETMAIL ")>=0)
	{	mouse_normal();
		if (FormAlert(1,3,"JetMail is running...|What now?|"," [Exit | [Start ")==1)
			return(FALSE);
	}

	if (is_running("LED     ")>=0)
	{	mouse_normal();
		if (FormAlert(1,3,"LED is already running...|What now?|"," [Exit | [Start ")==1)
			return(FALSE);
	}

	set_desktop();
	
	fselpath[0]=Dgetdrv()+'A';
	fselpath[1]=':';
	Dgetpath(fselpath+2,0);
	strcpy(gl_startpath,fselpath);
	add_bslash(fselpath);

/*
**	Suchen wir zunÑchst einmal den Pfad zu AREAS.BBS
*/

	strcpy(gl_areasbbspath,search_path("AREAS.BBS"		));
	strcpy(gl_binkleypath, search_path("BINKLEY.CFG"	));
	strcpy(gl_lednewpath,	 search_path("LED.NEW"			));
	strcpy(gl_ledprg,			 search_path("LED.PRG"			));

	add_bslash(gl_ledprg);
	strcat(gl_ledprg,"LED.PRG");
	if (findfirst(gl_ledprg,&dta,FA_UPDATE))
	{	strcpy(gl_ledprg, search_path("LED.APP"			));
		add_bslash(gl_ledprg);
		strcat(gl_ledprg,"LED.APP");
		if (findfirst(gl_ledprg,&dta,FA_UPDATE))
			*gl_ledprg=EOS;
	}

#if 0
	strcpy(gl_fidosoftpath,search_path("FIDOSOFT.CFG"	));
#endif

	found=FALSE;

/*
**	Gibt es die Environmentvariable $LED? Wenn ja, dann ist hier der
**	Konfigurationsordner direkt definiert?
*/

	line=getenv("LED");
	if (line)
	{	cp=skip_blanks(line);
		strncpy(gl_ledpath,cp,MAX_STR);
		strupr(gl_ledpath);
		cp=strchr(gl_ledpath,' ');
		if (cp) *cp=EOS;
		if (!findfirst(gl_ledpath,&dta,FA_DIREC))
			found=TRUE;
	}
	if (!found)
	{
		/*
		**	Wenn kein $LED gefunden, dann sehen wir einmal nach
		**	$FIDOCONFDIR. Und nehmen $FIDOCONFDIR\LED als Ordner.
		**	Wenn kein $FIDOCONFDIR existiert, probieren wir noch
		**	.\CONFIG\LED.
		*/

		line=getenv("FIDOCONFDIR");
		if (!line)
		{	sprintf(tmp,"%sCONFIG",fselpath);
			if (!findfirst(tmp,&dta,FA_DIREC))
				line=tmp;
		}
		if (line)
		{	cp=skip_blanks(line);
			strncpy(gl_ledpath,cp,MAX_STR);
			strupr(gl_ledpath);
			cp=strchr(gl_ledpath,' ');
			if (cp)	*cp=EOS;
			add_bslash(gl_ledpath);
			strcat(gl_ledpath,"LED");
			cp=strchr(gl_ledpath,' ');
			if (cp) *cp=EOS;
			if (!findfirst(gl_ledpath,&dta,FA_DIREC))
				found=TRUE;
		}
	}

	if (!found)
	{
		/*
		**	Gehen wir davon aus, daû der LED-Konfigordner in "."
		**	zu finden ist.
		*/

		strcpy(gl_ledpath,fselpath);
		strcat(gl_ledpath,"LED");
		if (!findfirst(gl_ledpath,&dta,FA_DIREC))
			found=TRUE;
	}

	if (!found)
	{	FormAlert(1,1,"Please create a configuration|folder 'LED' and read the manual!|"," [Abort ");
		return(FALSE);
	}
	add_bslash(gl_ledpath);

/*
**	Das LOGFILE liegt ebenfalls im Konfig-Ordner.
*/

	sprintf(gl_logfile,"%sLED.LOG",gl_ledpath);

	sprintf(gl_ledrsc,"%sLED.RSC",gl_ledpath);
	if (findfirst(gl_ledrsc,&dta,0))
		strcpy(gl_ledrsc,"LED.RSC");
	
	if (!load_rscfile(gl_ledrsc))
	{	mouse_normal();
		FormAlert(1,3,"Can't find RSC-File|'LED.RSC'"," [Sorry ");
		return(FALSE);
	}

	set_dialpos();
	
	vswr_mode(handle,MD_REPLACE);
	vst_color(handle,BLACK);
	vsf_interior(handle,1);
	vsf_color(handle,WHITE);
	vsf_style(handle,0);
	vsf_perimeter(handle,FALSE);
	vst_alignment(handle,0,0,&i,&i);
	vsl_ends(handle,0,0);
	vsl_type(handle,1);
	vsl_width(handle,1);
	vsl_color(handle,BLACK);

	init_cutbuff();
	s_cutbuff=
	e_cutbuff=-1;
	
	sprintf(tmp,"%sLED.LNG",gl_ledpath);
	if (!findfirst(tmp,&dta,0))
	{	if (!read_language(tmp))
			return(FALSE);
	}
	else
	{	FormAlert(1,1,"LED languagefile not found.|Please read the manual!"," [Abort ");
		return(FALSE);
	}

#if KEYS_INCLUDED
	sprintf(tmp,"%sLED.SYS",gl_ledpath);
	if (!findfirst(tmp,&dta,0))
	{	if (!read_keytab(tmp))
		{	LangAlert(MSG_SYSDEMAGE);
			return(FALSE);
		}
	}
	else
	{	LangAlert(MSG_SYSNOTFOUND);
		return(FALSE);
	}
#endif

	memset(wi,0,sizeof(wi));
	for (win=0;win<TOTAL_WIND;win++)
	{	if ( (wi[win].hdr=(HDR *)calloc(1,sizeof(HDR))) == NULL )
		{	nomemalert(1);
			return(FALSE);
		}
		memset(wi[win].msgda,LEER,sizeof(wi[win].msgda));
	}
	memset(gl_shell,0,sizeof(gl_shell));

	biene();
	show_doing(DO_READCONFFILE);

	sprintf(tmp,"%sLED.CFG",gl_ledpath);
	if (findfirst(tmp,&dta,0))
	{
#if 0
		sprintf(tmp,"%sFIDOSOFT.CFG",gl_fidosoftpath);
		if (findfirst(tmp,&dta,0))
#endif
		{	
			sprintf(tmp,"%sBINKLEY.CFG",gl_binkleypath);
			if (findfirst(tmp,&dta,0))
			{	del_doing();
				mouse_normal();
				LangAlert(MSG_CONFIGNOTFOUND);
				return(FALSE);
			}
		}
	}
	
	if (!config_init(tmp))
	{	del_doing();
		mouse_normal();
		return(FALSE);
	}

	memset(prn_divs,0,sizeof(prn_divs));
	for(i=0; i<64; i++)
		prn_options[i]=0;
	j=FALSE;
	sprintf(tmp,"%sLED.PRN",gl_ledpath);
	if (!findfirst(tmp,&dta,0))
	{	fp=fopen(tmp,"rb");
		if (fp)
		{	if (fread(&prn_table,PRN_TAB << 8,1,fp)==1)
			{	j=TRUE;
				fread(&prn_options,64,sizeof(int),fp);
				fread(&prn_divs,PRN_TAB << 5,1,fp);
			}
			fclose(fp);
		}
	}
	if (!j)
	{	for(i=0; i<=255; i++)
		{	prn_table[i][0]=1;
			prn_table[i][1]=i;
		}
	}

	memset(nicknames,0,3*MAX_NICKNAMES*sizeof(byte *));
	nicknamecnt=read_nicknames();

	get_optled();
	if (opt.flag & O_FLAG_GEMINFOLINE)
	{	gem_statusline = 1;
		FROM_LINE			 = 1;
		TO_LINE				 = 2;
		TOPIC_LINE		 = 3;
		TOPMSG_LINE		 = 5;
	}
	else
	{	gem_statusline = 0;
		FROM_LINE			 = 1+1;
		TO_LINE				 = 2+1;
		TOPIC_LINE		 = 3+1;
		TOPMSG_LINE		 = 5+1;
	}
	get_fkeys();
	
	if (!test_registered())
	{	LangAlert(MSG_NOTALLOWED);
		return(FALSE);
	}
	/* Ein paar Zeilen spÑter kommt der Text mit den BetaUsern */

	sprintf(tmp,"%-5u%-5u%-5u",addr[0].zone,addr[0].net,addr[0].node);
	
	set_dtext(nodenum_tree,NNTO,"",FALSE,36);
	set_dtext(nodenum_tree,NNSTRING,tmp,FALSE,20);

	set_dtext(find_tree,FITO,"",FALSE,36);
	set_dtext(find_tree,FIFROM,"",FALSE,36);
	set_dtext(find_tree,FISUBJ,"",FALSE,52);
	set_dtext(find_tree,FITEXT,"",FALSE,52);
	
	if (vq_gdos())	set_dtext(font_tree,FGDOSACT,msgtxt[SYSTEM_GDOSACTIVE].text,FALSE,11);
	else						set_dtext(font_tree,FGDOSACT,"",FALSE,11);

	if (gl_arzahl<=AREAS_SEITE)
		set_mflag(allarea_tree,HIDETREE,FALSE,AASIDE1,AASIDE2,-1);
	set_mflag(allarea_tree,HIDETREE,FALSE,AM1,AM2,AM3,AMOVEANF,AMOVEEND,AMK,AMF,-1);
	del_mstate(killmsg_tree,SELECTED,FALSE,KSENT,KKSENT,KCRASH,KHOLD,KFWD,-1);

	if (betatest && !is_registered)
	{	LangAlert(MSG_UNREGBETA);
		return(FALSE);
	}

	set_msgfnt();
	
	show_doing(DO_GETLASTREADS);
	load_lread();
	del_doing();
	
	mouse_normal();

	if (gl_options & OUTBOUND36)
		if (!available(msgtxt[DO_OUTBOUND36].text))
			gl_options &= ~OUTBOUND36;

	thanks();

	open_defltwind();		/* ssl 141294 */
	return(TRUE);
}
