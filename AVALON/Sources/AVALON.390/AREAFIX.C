#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
	#include "avalsemp.rh"
#else
	#include "avalon.h"
	#include "avalon.rh"
#endif
#include "avalonio.h"
#include "avallang.h"
#include "define.h"
#include "diverse.h"
#include "transkey.h"
#include "form_run.h"
#include "crc32.h"
#include "vars.h"

extern void smallhelp(int feld);

void do_areafix(void);

static void slider_redraw2(int redraw);
static void areafix(void);
static void erstelle_fixmail(int startarea,int zone,int net,int node,
											char *domain,char *password,char *fixname);
static void zeige_areas(int add);
static void areafix_error(void);

static int max_manager,areas_cnt;
static MANAGER *man;
static char fixname[11]="Areafix", fixpassword[26]="",
						msgbasepath[PATH_MAX];
static long last_loc;

/* =============================================================== */
/* =AREAFIX======================================================= */
/* =============================================================== */

void do_areafix(void)
	{	int i,dareasflag=FALSE;
		long t;
		char dum[512],dum2[512],temp[512],*pointer;
		FILE *fbbs;

		t = (((long)Malloc(-1L))-65536L)/sizeof(*man);
		if (t>MAXMANAGER)	t=MAXMANAGER;
		if (t<MINMANAGER)	t=MINMANAGER;
		max_manager = (int) t;
		man = (MANAGER *) calloc(max_manager+2,sizeof(*man));

		*msgbasepath=EOS;
		if (divflags & D_USEDBBS)
			if (form_alert(1,msg[37])==2)
				dareasflag=TRUE;

		if (dareasflag)
			strcpy(dum,dareas);
		else
			strcpy(dum,areas);

		if (man==NULL)
			{	memory_error();
			}
		else
			{	set_defaultdir();

				areas_cnt=1;

				if ( exist(dum)==0 )
					{	sprintf(dum2,"---------------- \2\2\2\2\2       %s       \2\2\2\2\2 ----------------",dareasflag ? "DAREAS.BBS" : "AREAS.BBS");
						man[areas_cnt].flag = -1;
						strcpy(man[areas_cnt++].name,dum2);

						fbbs=fopen( dum ,"r");
						if (fbbs==NULL)
							{	fatal_error( dum );
							}
						else
							{	if (!dareasflag)
									input(dum,MAX_STR,fbbs);
								do
									{	input(dum,512,fbbs);
										pointer=dum; trim(&pointer); strcpy(dum,pointer);

										if (*dum!=EOS)
											{	if (*dum=='-')
													{	if (strnicmp(dum,"-FIXNAME ",9)==0)
															{	pointer += 9;
																trim(&pointer);
																strncpy(fixname,pointer,10);
																*(fixname+10) = EOS;
															}
														if (strnicmp(dum,"-FIXPASSWORD ",13)==0)
															{	pointer += 13;
																trim(&pointer);
																strncpy(fixpassword,pointer,25);
																*(fixpassword+25) = EOS;
															}
													}

												if (*dum=='-' || *dum==';')
													continue;

												memset(dum2,0,512);
												if (dareasflag)
													{	i = sscanf(dum,"%s %[^_]s",man[areas_cnt].name,dum2);
														i++;
													}
												else
													{	i = sscanf(dum,"%s %s %[^_]s",temp,man[areas_cnt].name,dum2);
														if (*msgbasepath==EOS)
															{	strcpy(msgbasepath,temp);
																pointer=strrchr(msgbasepath,'\\');
																if (pointer!=NULL)
																	*(pointer+1)=EOS;
																else
																	strcpy(msgbasepath,"\\");
															}
													}
												switch(i)
													{	case 0 :
														case 1 :	form_alert(1,msg[1]);
																			break;
														case 2 :	man[areas_cnt].boss[0] =
																			man[areas_cnt].boss[1] =
																			man[areas_cnt].boss[2] = 0;
																			*man[areas_cnt].domain = EOS;
																			break;
														case 3 : 	{	pointer=strtok(dum2," ");
																				scanaddr(pointer,&man[areas_cnt].boss[2],
																												 &man[areas_cnt].boss[0],
																												 &man[areas_cnt].boss[1],
																												 &i,temp);
																				strncpy(man[areas_cnt].domain,temp,25);
																				man[areas_cnt].domain[25] = EOS;
																			}
													}

												strcpy(man[areas_cnt].fixname,fixname);
												strcpy(man[areas_cnt].fixpassword,fixpassword);
														
												areas_cnt++;
												if (areas_cnt>max_manager)
													{	--areas_cnt;
														areafix_error();
														break;
													}
											}
									}
								while (feof(fbbs)==0);
								fclose(fbbs);
								areafix();
							}
					}
				else
					{	areafix();
					}
			}
		if (man!=NULL)
			free((MANAGER *) &man[0]);
	}

static void slider_redraw2(int redraw)
	{ xredraw_slider(tree25,handle,FIXFULL+Add_Lines,
																 FIXBK+Add_Lines,
																 FIXFD+Add_Lines,
																 areas_cnt,last_loc+1,
																 FIXLINES+Add_Lines,redraw);
	}

static void areafix(void)
	{	int add=0,i,wahl,xwahl,chr,nomore,start=1,nx,ny,nw,nh,
				myzone,mynet,mynode;
		char temp[MAX_STR],temp2[MAX_STR],*pointer,tfind[31]="",
				 mydomain[26],cmpdomain[26],*pointer2;
		FILE *fp;

		if ((divflags & D_MAXIWIND) && (max_y>=672))
			{	Add_Lines=FIXLINESADD;
				tree25=rs_trindex[DIALO25B];
			}
		else
			{	Add_Lines=0;
				tree25=rs_trindex[DIALOG25];
			}

		if (*msgbasepath==EOS)
			strcpy(msgbasepath,"\\");
		strcpy(temp,address[0]);
		scanaddr(temp,&myzone,&mynet,&mynode,&i,temp2);
		strncpy(mydomain,temp2,25); mydomain[25]=EOS;

		areas_cnt--;

		set_defaultdir();
		if ( exist("AVALON\\AVALON.LST")==0 )
			{	strcpy(man[++areas_cnt].name,"---------------- \2\2\2\2\2       AVALON.LST       \2\2\2\2\2 ----------------");
				man[areas_cnt].flag = -1;

				fp = fopen("AVALON\\AVALON.LST", "r");
				if (fp!=NULL)
					{	setvbuf(fp,NULL,_IOFBF,PHYS_BUFFER);
						while (!feof(fp) && areas_cnt<max_manager)
							{	input(temp, MAX_STR, fp);
								if (feof(fp))
									break;

								if (!strncmp(temp,"               ",15))
									*temp=EOS;

								do
									{	if (isspace(*temp))
											{	pointer=temp;
												trim(&pointer);
												strcpy(temp,pointer);
											}
										if (*temp==';' || *temp==EOS)
											{	*temp=EOS;
												break;
											}
										if (isspace(temp[1]))
											*temp=' ';
										if (*temp<'0' && *temp!='%')
											*temp=' ';
									}
								while (isspace(*temp));

								pointer=temp;
								if (*pointer!=EOS)
									{	if (strnicmp(pointer,"NODE ",5)==0 ||
												strnicmp(pointer,"LINK ",5)==0 )
											{	scanaddr(pointer+5,&myzone,&mynet,&mynode,&i,temp2);
												strncpy(mydomain,temp2,25); mydomain[25]=EOS;
											}
										else if	(strnicmp(pointer,"AREAFIX ",8)==0 )
											{	pointer += 8; trim(&pointer);
												strncpy(fixname,pointer,10);
												*(fixname+10) = EOS;
											}
										else if	(strnicmp(pointer,"PASSWORD ",9)==0 )
											{	pointer += 9; trim(&pointer);
												strncpy(fixpassword,pointer,25);
												*(fixpassword+25) = EOS;
											}
										else if	(strnicmp(pointer,"REM ",4)==0 )
											{	pointer += 4; trim(&pointer);
												if (strlen(pointer)>20)
													*(pointer+20) = EOS;
												i=(20-(int)strlen(pointer)) >> 1;
												if (i>0)
													{	memset(temp2,' ',i);
														strcpy(temp2+i,pointer);
													}
												else
													{	strcpy(temp2,pointer);
													}
												sprintf(man[++areas_cnt].name,"---------------- \2\2\2\2\2  %-20s  \2\2\2\2\2 ----------------",temp2);
												man[areas_cnt].flag = -1;
											}
										else
											{	areas_cnt++;
												pointer = strtok(temp,"\t ");
												strcpy(man[areas_cnt].name,pointer);
												strcpy(man[areas_cnt].fixname,fixname);
												strcpy(man[areas_cnt].fixpassword,fixpassword);
												pointer = strtok(NULL,"");
												if (pointer)
													{	pointer2=strchr(pointer,'/');
													  while (pointer2 && !strchr(pointer2-6,':'))
													  	pointer2=strchr(pointer2+1,'/');
														if (pointer2)
															{ pointer2--;
																while (strchr("0123456789:",*pointer2) &&
																			 (pointer2 >= pointer)) pointer2--;
      			  									pointer2++;
																trim(&pointer2);
																if (*pointer2!=EOS)
																	{	scanaddr(pointer2,&man[areas_cnt].boss[2],&man[areas_cnt].boss[0],
																								 			&man[areas_cnt].boss[1],&i,temp2);
																		temp2[25]=EOS;
																		strcpy(man[areas_cnt].domain,temp2);
																	}
															}
													}
												if (man[areas_cnt].boss[2]==0)
													man[areas_cnt].boss[2] = myzone;
												if (man[areas_cnt].boss[0]==0 &&
														man[areas_cnt].boss[1]==0)
													{	man[areas_cnt].boss[0] = mynet;
														man[areas_cnt].boss[1] = mynode;
													}
												if (*man[areas_cnt].domain==EOS)
													strcpy(man[areas_cnt].domain,mydomain);
											}
									}
							}
						fclose(fp);
					}
			}
		if (areas_cnt==0)
			{	strcpy(man[areas_cnt].name,avaltxt[5]);
				man[areas_cnt].flag = -1;
			}

		sprintf(temp,"%d",max_manager);
		put_text(tree25,FIXMAX+Add_Lines,temp,4);

		OB_X(tree25,FIXFD+Add_Lines) = 0;
		last_loc = 0;
		zeige_areas(add);
		slider_redraw2(FALSE);
		xform_dial(tree25,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	xwahl=xform_do(handle,tree25,NULL,0,0,trans_key6,&wnx,&wny,&wnw,&wnh);
				if (xwahl & 0x4000)
					xwahl = FIXCANC+Add_Lines;
				wahl = xwahl & 0x7ff;
				if (wahl<1000)
					undo_state(tree25,wahl,1);

				if (wahl==FIXBK+Add_Lines)
					{	objc_offset(tree25,FIXFD+Add_Lines,&nx,&ny);
						vq_mouse(vdi_handle,&i,&nw,&nh);
						if (nw<nx)
							xwahl=wahl=FIXUPUP+Add_Lines;
						if (nw>nx+OB_W(tree25,FIXFD+Add_Lines))
							xwahl=wahl=FIXDNDN+Add_Lines;
					}

				if (Add_Lines==0)
					{	switch(wahl)
							{	case FIXDOWN	:	wahl=1070; break;
								case FIXUP		:	wahl=1071; break;
								case FIXDNDN	:	wahl=1072; break;
								case FIXUPUP	:	wahl=1073; break;
								case FIXFD		:	wahl=1074; break;
							}
					}
				else
					{	switch(wahl)
							{	case FIXDOWN+FIXLINESADD	:	wahl=1070; break;
								case FIXUP	+FIXLINESADD	:	wahl=1071; break;
								case FIXDNDN+FIXLINESADD	:	wahl=1072; break;
								case FIXUPUP+FIXLINESADD	:	wahl=1073; break;
								case FIXFD	+FIXLINESADD	:	wahl=1074; break;
							}
					}

				switch(wahl)
					{	case 1061			:	smallhelp(HELPFIX); break;
						case 1064			: add=0; break;
						case 1065			: add=areas_cnt; break;
						case 1063			:	{	if (*tfind==EOS)
																{	form_alert(1,msg[18]);
																	break;
																}
														}
						case 1062			:	{	i=0;
															if (wahl==1062)
																{	put_text(tree14,FINDTEXT,tfind,30);
																	form_center(tree14,&nx,&ny,&nw,&nh);
																	form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																	form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																	objc_draw(tree14,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																	i=form_do(tree14,0) & 0x7FFF;
																	undo_state(tree14,i,SELECTED);
																	form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																	form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
																}
															if (i==FINDOK)
																{ strcpy(tfind,get_text(tree14,FINDTEXT));
																	strupr(tfind);
																}
															if (i==FINDOK || wahl==1063)
																{	if (add>=areas_cnt)
																		form_alert(1,msg[17]);
																	else
																		{	for (i=add+2; i<=areas_cnt; i++)
																				{	if (i % 10 == 0)
																						{	last_loc=i;
																							slider_redraw2(TRUE);
																						}
																					if (strstr(man[i].name,tfind)!=NULL)
																						break;
																				}
																			last_loc = add = i-1;
																			slider_redraw2(TRUE);
																			zeige_areas(add);
																			redraw_one_objc(tree25,FIXBOX,handle);
																		}
																}
														}	break;
						case 1070			:	{	add++;
															if (add>areas_cnt)
																add--;
														}	break;
						case 1071			:	{	add--;
															if (add<0)
																add = 0;
														}	break;
						case 1072			:	{	add += FIXLINES+Add_Lines;
															if (add>areas_cnt)
																add = areas_cnt;
														}	break;
						case 1073			:	{	add -= FIXLINES+Add_Lines;
															if (add<0)
																add = 0;
														}	break;
						case 1074			:	{	last_loc = ((long)areas_cnt)*graf_slidebox(tree25,FIXBK+Add_Lines,FIXFD+Add_Lines,0)/1001;
															if (last_loc<0) last_loc = 0;
															if (last_loc>areas_cnt) last_loc = areas_cnt;
															add = (int) last_loc;
														}
					}

				if ((wahl>=1070 && wahl<=1074) || wahl==1064 || wahl==1065)
					{	last_loc = add;
						slider_redraw2(TRUE);
						zeige_areas(add);
						redraw_one_objc(tree25,FIXBOX,handle);
					}

				if (wahl>=FIX1 && wahl<=FIX1+FIXLINES+Add_Lines)
					{	if (++man[add+wahl-FIX1+1].flag >= 4)
							man[add+wahl-FIX1+1].flag = 0;
						switch ( man[add+wahl-FIX1+1].flag )
							{	case 0	:	chr = ' '; break;
								case 1	: chr = '-'; break;
								case 2	: chr = '+'; break;
								case 3	:	chr = '*';
							}
						*tree25[wahl].ob_spec.free_string = chr;
						redraw_objc(tree25,wahl,handle);
					}
			}
		while ( (wahl!=FIXCANC+Add_Lines) && (wahl!=FIXAREA+Add_Lines) );
		if (wahl==FIXAREA+Add_Lines)
			{	do
					{	nomore=TRUE;
						for (i=start; i<=areas_cnt; i++)
							{	if ( man[i].flag > 0 )
									{	nomore=FALSE;
										start=i;
										sprintf(temp,"%u:%u/%u",man[i].boss[2],
																						man[i].boss[0],
																						man[i].boss[1]);
										put_text(tree26,PASSDOMA,man[i].domain,25);
										put_text(tree26,PASSNODE,temp,20);
										put_text(tree26,FIXNAME,man[i].fixname,10);
										put_text(tree26,PASSSUBJ,man[i].fixpassword,25);
										form_center(tree26,&nx,&ny,&nw,&nh);
										form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
										form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
										objc_draw(tree26,ROOT,MAX_DEPTH,nx,ny,nw,nh);
										chr=form_do(tree26,PASSSUBJ) & 0x7FFF;
										undo_state(tree26,chr,SELECTED);
										form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
										form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
										strcpy(fixpassword,get_text(tree26,PASSSUBJ));
										strcpy(fixname,get_text(tree26,FIXNAME));
										if (chr==FIXOK)
											{	/* Ist Fixname/Fixpassword ge„ndert worden? */
												if (stricmp(fixname,man[start].fixname)!=0 ||
														stricmp(fixpassword,man[start].fixpassword)!=0 )
													{	nx = man[start].boss[2];
														ny = man[start].boss[0];
														nw = man[start].boss[1];
														strcpy(cmpdomain,man[start].domain);
														for (wahl=start+1; wahl<=areas_cnt; wahl++)
															{	if ( man[wahl].flag > 0	&&
																		 man[wahl].boss[2]==nx &&
																		 man[wahl].boss[0]==ny &&
																		 man[wahl].boss[1]==nw &&
																		 stricmp(man[wahl].domain,cmpdomain)==0 &&
																		 stricmp(man[wahl].fixname,man[start].fixname)==0 &&
																		 stricmp(man[wahl].fixpassword,man[start].fixpassword)==0 )
																	{	strcpy(man[wahl].fixname,fixname);
																		strcpy(man[wahl].fixpassword,fixpassword);
																	}
															}
														strcpy(man[start].fixname,fixname);
														strcpy(man[start].fixpassword,fixpassword);
													}
												erstelle_fixmail(	start,man[i].boss[2],man[i].boss[0],man[i].boss[1],
																					man[i].domain,fixpassword,fixname );
											}
										else
											{	nx = man[i].boss[2];
												ny = man[i].boss[0];
												nw = man[i].boss[1];
												strcpy(cmpdomain,man[i].domain);
												for (wahl=start; wahl<=areas_cnt; wahl++)
													{	if ( man[wahl].flag > 0	&&
																 man[wahl].boss[2]==nx &&
																 man[wahl].boss[0]==ny &&
																 man[wahl].boss[1]==nw &&
																 stricmp(man[wahl].domain,cmpdomain)==0 &&
																 stricmp(man[wahl].fixname,fixname)==0 &&
																 stricmp(man[wahl].fixpassword,fixpassword)==0 )
															{	man[wahl].flag = 0;
																*man[wahl].fixname =
																*man[wahl].fixpassword = EOS;
															}
													}
											}
									}
							}
					}
				while (nomore==FALSE);
			}
		xform_maindialog(XFMD_DIALOG);
	}

static void zeige_areas(int add)
	{	char temp[MAX_STR],temp2[MAX_STR];
		int i,chr;
		for (i=1; i<=FIXLINES+Add_Lines; i++)
			{	if (man[i+add].flag == -1)
					OB_FLAGS(tree25,FIX1+i-1) &= ~SELECTED;
				else
					OB_FLAGS(tree25,FIX1+i-1) |= SELECTED;
				if (i+add<=areas_cnt)
					{	switch ( man[i+add].flag )
							{	case 0	:	chr = ' '; break;
								case 1	: chr = '-'; break;
								case 2	: chr = '+'; break;
								case 3	: chr = '*'; break;
								default	: chr = ' ';
							}
						if (man[i+add].boss[0]==0 && man[i+add].boss[1]==0)
							sprintf(temp,"%c %-67s",chr,man[i+add].name);
						else
							sprintf(temp,"%c %-48s %-17s ",chr,man[i+add].name,
																					mergeaddr(temp2,man[i+add].boss[2],
																													man[i+add].boss[0],
																													man[i+add].boss[1],0,""));
						put_string(tree25,FIX1+i-1,temp,69);
						OB_FLAGS(tree25,FIX1+i-1) &= ~HIDETREE;
					}
				else
					{	OB_FLAGS(tree25,FIX1+i-1) |= HIDETREE;
					}
			}
	}

static void erstelle_fixmail(int startarea,int zone,int net,int node,char *xdomain,char *password,char *name)
	{	FILE *ftic,*fbbs,*fbbs2;
		char temp2[MAX_STR],temp3[MAX_STR],seenby[MAX_STR],*pointer,
				 dum[MAX_STR];
		int myzone,mynet,mynode,mypoint,i,t,dareasflag=0,nx,ny,nw,nh,
				tzone,tnet,tnode,wahl;
		time_t xtime;

		time(&xtime);
		strcpy(seenby,address[0]);
		if ((pointer=strchr(seenby,'@'))!=NULL)
			*pointer=EOS;
		scanaddr(seenby,&myzone,&mynet,&mynode,&mypoint,temp3);

		strcpy(temp2,netmail[0]);
		strcat(temp2,".MSG");
		ftic=fopen(temp2,"ab");
		if (ftic==NULL)
			{	fatal_error(temp2);
			}
		else
			{	header.lStart = length_of_file(ftic,FALSE);
				header.wSize = 0;
				mergeaddr(temp3,myzone,mynet,mynode,mypoint,"");

				write_ids(&header,ftic,temp3);

				for (i=startarea; i<=areas_cnt; i++)
					{	if ( man[i].flag > 0			&&
								 man[i].boss[2]==zone &&
								 man[i].boss[0]==net	&&
								 man[i].boss[1]==node &&
								 stricmp(man[i].domain,xdomain)==0 &&
								 stricmp(man[i].fixname,fixname)==0 &&
								 stricmp(man[i].fixpassword,fixpassword)==0 )
							{	switch( man[i].flag )
									{	case	1	:
											sprintf(temp2,"-%s\n",man[i].name);
											break;
										case	3	:
											{	if (divflags & D_USEDBBS)
													{	sprintf(temp2,tmsg[14],man[i].name);
														switch(form_alert(1,temp2))
															{	case	1	: dareasflag=1; break;
																case	2	: dareasflag=2; break;
																case	3	:	dareasflag=0;
															}
													}
												else
													{	sprintf(temp2,tmsg[13],man[i].name);
														switch(form_alert(1,temp2))
															{	case	1	: dareasflag=1; break;
																case	2	:	dareasflag=0;
															}
													}
												switch(dareasflag)
													{	case	1	:	strcpy(temp2,areas); break;
														case	2 :	strcpy(temp2,dareas);
													}
												if (dareasflag!=0)
													{	set_defaultdir();
														fbbs=fopen( temp2 ,"a");
														if (fbbs==NULL)
															{	fatal_error( temp2 );
															}
														else
															{	if (dareasflag==2)
																	{	do_xstate(tree27,DISABLED,AUTODAYS,AUTOORG,AUTOPATH,-1);
																		undo_xflags(tree27,EDITABLE,AUTODAYS,AUTOORG,AUTOPATH,-1);
																		put_text(tree27,AUTODAYS,"",0);
																		put_text(tree27,AUTOORG ,"",0);
																		put_text(tree27,AUTOPATH,"",0);
																	}
																else
																	{	undo_xstate(tree27,DISABLED,AUTODAYS,AUTOORG,AUTOPATH,-1);
																		do_xflags(tree27,EDITABLE,AUTODAYS,AUTOORG,AUTOPATH,-1);
																		put_text(tree27,AUTODAYS,"14",3);
																		put_text(tree27,AUTOORG ,"Default origin created by AVALON...",60);
																		sprintf(temp2,"%sFILE%04X",msgbasepath,
																															 (int)~Crc32(0xffffffffL,man[i].name,strlen(man[i].name)));
																		put_text(tree27,AUTOPATH,temp2,60);
																	}

																put_text(tree27,AUTOAREA,man[i].name,48);
																sprintf(temp2,"%5u%-5u%s",net,node,xdomain);
																put_text(tree27,AUTOBOSS,temp2,35);
																sprintf(temp2,"%u",zone);
																put_text(tree27,AUTOZONE,temp2,3);
						
																form_center(tree27,&nx,&ny,&nw,&nh);
																form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																do
																	{	t=TRUE;
																		objc_draw(tree27,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																		wahl=form_do(tree27,AUTOBOSS) & 0x7FFF;
																		undo_state(tree27,wahl,SELECTED);

																		if ((divflags & D_CHECKING) && (dareasflag==1) )
																			{	strcpy(temp3,get_text(tree27,AUTOPATH));
																				sprintf(dum,"%s.$$$",temp3);
																				fbbs2=fopen(dum,"ab");
																				if (fbbs2==NULL)
																					{	t=FALSE;
																						form_alert(1,msg[42]);
																					}
																				else
																					{	fclose(fbbs2);
																					}
																				if (exist(dum)==0)
																					remove(dum);
																			}
																	}
																while (t==FALSE);
																form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
						
																if (wahl==AUTOOK)
																	{	if (dareasflag==1)
																			{	strcpy(temp2,get_text(tree27,AUTODAYS));
																				wahl = atoi(temp2);
																				fprintf(fbbs,"-DAYS %u\n",wahl);
																				strcpy(temp2,get_text(tree27,AUTOORG));
																				if (*temp2 != EOS)
																					fprintf(fbbs,"-ORIGIN %s\n",temp2);
																			}
																		strcpy(temp2,get_text(tree27,AUTOZONE));
																		tzone = atoi(temp2);
																		strcpy(temp2,get_text(tree27,AUTOBOSS));
																		strncpy(temp3,temp2,5); temp3[5]=EOS;
																		tnet = atoi(temp3);
																		if (strlen(temp2)>5)
																			{	strncpy(temp3,temp2+5,5); temp3[5]=EOS; }
																		else
																			{	strcpy(temp3,"0"); }
																		tnode = atoi(temp3);
																		if (strlen(temp2)>10)
																			strcpy(temp2,temp2+10);
																		else
																			*temp2=EOS;
																		if (dareasflag==1)
																			{	strcpy(temp3,get_text(tree27,AUTOPATH));
																				fprintf(fbbs,"%s %s %u:%u/%u%c%s\n",temp3,man[i].name,tzone,tnet,tnode,(*temp2==EOS) ? ' ' : '@',temp2);
																			}
																		else
																			{	fprintf(fbbs,"%s %u:%u/%u%c%s\n",man[i].name,tzone,tnet,tnode,(*temp2==EOS) ? ' ' : '@',temp2);
																			}
																		
																		if ((divflags & D_CHECKING) && (dareasflag==1) )
																			{	sprintf(dum,"%s.MSG",temp3);
																				if ( exist(dum)!=0 )
																					{	sprintf(dum,tmsg[6],man[i].name);
																						if (form_alert(2,dum)==2)
																							{	sprintf(dum,"%s.MSG",temp3);
																								fbbs2=fopen(dum,"wb");
																								if (fbbs2!=NULL)
																									{	fclose(fbbs2);
																										sprintf(dum,"%s.HDR",temp3);
																										fbbs2=fopen(dum,"wb");
																										if (fbbs2!=NULL)
																											fclose(fbbs2);
																										else
																											fatal_error(dum);
																									}
																								else
																									{	fatal_error(dum);
																									}
																							}
																					}
																			}
																	}
																fclose(fbbs);
															}
													}
											} 				/* KEIN BREAK */
										case	2	:
											sprintf(temp2,"%s\n",man[i].name);
									}
								fputs(temp2,ftic);
								header.wSize += (int) strlen(temp2);
								man[i].flag = 0;
							}
					}
				fputs("---\n",ftic);
				putc(EOS,ftic);
				header.wSize += 5;
				fclose(ftic);

				strcpy(temp2,netmail[0]);
				strcat(temp2,".HDR");
				ftic=fopen(temp2,"ab");
				if (ftic==NULL)
					{	fatal_error(temp2);
					}
				else
					{	setvbuf(ftic,NULL,_IOFBF,8*sizeof(MSG_HEADER));
						fseek(ftic,0,SEEK_END);
						strcpy(header.szFrom,txtsysop);
						strcpy(header.szTo,name);

						strcpy(header.szSubject,password);

						header.wFromZone = myzone;
						header.wFromNet  = mynet;
						header.wFromNode = mynode;
						header.wFromPoint= mypoint;

						if (zone!=0)
							header.wToZone = zone;
						else
							header.wToZone = myzone;
						header.wToNet  = net;
						header.wToNode = node;
						header.wToPoint= 0;
						header.lDate = xtime;
						strftime(header.szDate,20,"%d %b %y %X",localtime(&header.lDate) );
						header.wFlags= MF_LOCAL | MF_PRIVATE;
						if (divflags & D_KSFIX)
							header.wFlags |= MF_KILLIFSENT;

						delete_unused_flags(&header);

						fwrite(&header,sizeof(MSG_HEADER),1,ftic);
						fclose(ftic);
					}
			}
	}

static void areafix_error(void)
	{	char temp[MAX_STR];
		sprintf(temp,tmsg[15],max_manager);
		form_alert(1,temp);
	}
