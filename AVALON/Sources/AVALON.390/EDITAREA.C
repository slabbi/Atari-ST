#include <aes.h>
#include <vdi.h>
#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "form_run.h"
#include "vars.h"

void	areas_error(void);
void	areas_error(void);
void	reset_entry(int xcounter);
void	slider_breite(void);
void	redraw_slider(void);
void	trage_areas(void);
void	trage_areas_boss(int counter,int idxaddress,int idxday);
void	insdel_do(int redraw,int cins, int cdel, int cext, int cnode);
void	insdel_undo(int redraw,int cins, int cdel, int cext, int cnode);
void	is_extend(int redraw,int ext, int xcounter);
void	is_np(int redraw,int node, int xcounter);
void	teste_ob_insert(int redraw);
void	auslesen(void);
void	loesche(register int xcounter);
void	einfuege(register int xcounter);
void	insert(int xcounter);
int		msghdrdel(int xcounter,int dareasflag);
void 	areas_dialog(int dareasflag);
void	extend(int xcounter);
void	xnp(int xcounter);

static int	max_areas,areas_cnt,counter;
static char address_bossnode[41],origin[MAX_STR],find[35]="";
static AREAS_BBS *bbs;

/* =============================================================== */
/* =EDIT-AREAS==================================================== */
/* =============================================================== */

void edit_areas(void)
	{	int i,dareasflag=FALSE,defdays;
		long t;
		char dum[512],dum2[512],*pointer;
		FILE *fbbs;

		t = (((long)Malloc(-1L))-65536L)/sizeof(*bbs);
		if (t>MAXAREAS)	t=MAXAREAS;
		if (t<MINAREAS)	t=MINAREAS;
		max_areas = (int) t;
		bbs = (AREAS_BBS *) calloc(max_areas+2,sizeof(*bbs));

		if (divflags & D_USEDBBS)
			if (form_alert(1,msg[37])==2)
				dareasflag=TRUE;

		if (dareasflag)
			strcpy(dum,dareas);
		else
			strcpy(dum,areas);

		if (bbs==NULL)
			{	memory_error();
			}
		else
			{ set_defaultdir();
		
				areas_cnt=1;
				defdays=-1;
				*origin=EOS;

				if ( exist(dum)==0 )
					{	fbbs=fopen( dum ,"r");
						if (fbbs==NULL)
							{	fatal_error( dum );
							}
						else
							{	if (dareasflag)
									strcpy(origin,avaltxt[13]);
								else
									input(origin,MAX_STR,fbbs);
								do
									{
										input(dum,512,fbbs);
										pointer=dum; trim(&pointer); strcpy(dum,pointer);

										if ( *dum!=EOS )
											{	
												if ( strnicmp(dum,"-BOSSNODE ",10)==0 ||
														 strnicmp(dum,"-BOSSNODE=",10)==0 )
													{	pointer += 10; trim(&pointer);
														if (*pointer!=EOS)
															{	strncpy(address_bossnode,pointer,40);
																address_bossnode[40]=EOS;
															}
														else
															{ *address_bossnode=EOS;
															}
														continue;
													}
													
												if ( strnicmp(dum,"-DAYS ",6)==0 ||
														 strnicmp(dum,"-DAYS=",6)==0 )
													{	pointer += 6; trim(&pointer);
														defdays=atoi(pointer);
														continue;
													}
		
												if (strnicmp(dum,"-ORIGIN ",8)==0 ||
														strnicmp(dum,"-ORIGIN=",8)==0 )
													{	pointer += 7;
														if (*pointer!=EOS)
															{	pointer++;
																strcpy(bbs[areas_cnt].origin,pointer);
															}
														else
															{	*bbs[areas_cnt].origin = EOS;
															}
														continue;
													}
		
												if (*dum=='-')
													{	for (t=0; t<=AREAS_OTHER-1; t++)
															{	if (*bbs[areas_cnt].areasbbs[t]==EOS)
																	{	if (strlen(dum)>AREAS_OTHERLEN)
																			dum[AREAS_OTHERLEN]=EOS;
																		strcpy(bbs[areas_cnt].areasbbs[t],dum);
																		break;
																	}
															}
													}
		
												if (*dum=='-' || *dum==';')
													continue;

												memset(dum2,0,512);
												if (dareasflag)
													{	i = sscanf(dum,"%s %[^_]s",bbs[areas_cnt].name,dum2);
														*bbs[areas_cnt].path = EOS;
														i++;
													}
												else
													{	i = sscanf(dum,"%s %s %[^_]s",bbs[areas_cnt].path,bbs[areas_cnt].name,dum2);
													}
													
												switch(i)
													{	case 0	:
														case 1	: form_alert(1,msg[1]);
																		 	break;
														case 2	: *bbs[areas_cnt].address=EOS;
																		 	break;
														case 3 	: {	pointer=strtok(dum2," ");
																				strncpy(bbs[areas_cnt].address,pointer,40);
																				pointer=strtok(NULL,"");
																				if (pointer!=NULL)
																					{	trim(&pointer);
																						strcpy(bbs[areas_cnt].np,pointer);
																					}
																				else
																					{	*bbs[areas_cnt].np=EOS;
																					}
																			}
													}
												bbs[areas_cnt].days=defdays;
												defdays=-1;

												areas_cnt++;
												if (areas_cnt>max_areas)
													{	--areas_cnt;
														areas_error();
														break;
													}
											}
									}
								while (feof(fbbs)==0);
								fclose(fbbs);
								areas_dialog(dareasflag);
							}
					}
				else
					{	form_alert(1,msg[3]);
							areas_dialog(dareasflag);
					}
			}
		if (bbs!=NULL)
			free((AREAS_BBS *) &bbs[0]);
	}

void areas_error(void)
	{	char temp[MAX_STR];
		sprintf(temp,tmsg[3],max_areas);
		form_alert(1,temp);
	}

void reset_entry(int xcounter)
	{	memset(&bbs[xcounter],EOS,sizeof(AREAS_BBS));
	}

void slider_breite(void)
	{	register int ret;
		if ( areas_cnt>2 )
			{	ret = OB_W(tree7,ARROWBK) / (areas_cnt-2);
				if (ret<4)
					OB_W(tree7,ARROWFD) = 4;
				else
					OB_W(tree7,ARROWFD) = ret;
				do_state(tree7,ARINSERT,DISABLED);
				OB_FLAGS(tree7,ARINSERT) &= ~(EXIT|DEFAULT|SELECTABLE);
			}
		else
			{	OB_W(tree7,ARROWFD) = OB_W(tree7,ARROWBK);
				undo_state(tree7,ARINSERT,DISABLED);
				OB_FLAGS(tree7,ARINSERT) |= (EXIT|DEFAULT|SELECTABLE);
			}

		if (areas_cnt>=5 &&													/* ab vier Areas */
				(counter<=max_areas-4) && (counter<=areas_cnt-4) )
			{	undo_state(tree7,ARSEARCH,DISABLED);
				OB_FLAGS(tree7,ARSEARCH) |= (EXIT|SELECTABLE);
			}
		else
			{	do_state(tree7,ARSEARCH,DISABLED);
				OB_FLAGS(tree7,ARSEARCH) &= ~(EXIT|SELECTABLE);
			}
		redraw_one_objc(tree7,ARSEARCH,handle);
	}

void redraw_slider(void)
	{	xredraw_slider(tree7,handle,ARROWFUL,ARROWBK,ARROWFD,areas_cnt-3,counter,3,TRUE);

		trage_areas();
		redraw_one_objc(tree7,AREASBOX,handle);
		if ( ((areas_cnt-2)==counter) || (areas_cnt<=2) )
			{	undo_state(tree7,ARINSERT,DISABLED);
				OB_FLAGS(tree7,ARINSERT) |= (EXIT|DEFAULT|SELECTABLE);
			}
		else
			{	do_state(tree7,ARINSERT,DISABLED);
				OB_FLAGS(tree7,ARINSERT) &= ~(EXIT|DEFAULT|SELECTABLE);
			}
		redraw_one_objc(tree7,ARINSERT,handle);
	}

void trage_areas_boss(int counter,int indexaddress,int indexday)
	{	char dum[80];
		if (*bbs[counter].address==EOS)
			*dum=EOS;
		else
			sprintf(dum,"%-40s",bbs[counter].address);
		put_text(tree7,indexaddress,dum,40);

		if (bbs[counter].days!=-1)
			sprintf(dum,"%d",bbs[counter].days);
		else
			*dum=EOS;
		put_text(tree7,indexday,dum,3);
	}

void trage_areas(void)
	{	trage_areas_boss(counter  ,BOSS1,DAYS1);
		trage_areas_boss(counter+1,BOSS2,DAYS2);
		trage_areas_boss(counter+2,BOSS3,DAYS3);
		put_text(tree7,DNAME1,bbs[counter  ].name,48);
		put_text(tree7,DNAME2,bbs[counter+1].name,48);
		put_text(tree7,DNAME3,bbs[counter+2].name,48);
		put_text(tree7,ORIGIN1,bbs[counter  ].origin,60);
		put_text(tree7,ORIGIN2,bbs[counter+1].origin,60);
		put_text(tree7,ORIGIN3,bbs[counter+2].origin,60);
		put_text(tree7,PATH1,bbs[counter  ].path,60);
		put_text(tree7,PATH2,bbs[counter+1].path,60);
		put_text(tree7,PATH3,bbs[counter+2].path,60);
	}

void insdel_undo(int redraw,int cins, int cdel, int cext, int cnode)
	{	undo_xstate(tree7,DISABLED,cins,cdel,cext,cnode,-1);
		if (redraw)
			{	redraw_one_objc(tree7,cdel,handle);
				redraw_one_objc(tree7,cins,handle);
				redraw_one_objc(tree7,cext,handle);
				redraw_one_objc(tree7,cnode,handle);
			}
	}

void insdel_do(int redraw,int cins, int cdel, int cext, int cnode)
	{	do_xstate(tree7,DISABLED,cins,cdel,cext,cnode,-1);
		if (redraw)
			{	redraw_one_objc(tree7,cdel,handle);
				redraw_one_objc(tree7,cins,handle);
				redraw_one_objc(tree7,cext,handle);
				redraw_one_objc(tree7,cnode,handle);
			}
	}

void is_extend(int redraw,int ext, int xcounter)
	{	int bit,*thickness;
		thickness = &tree7[ext].ob_spec.tedinfo->te_thickness;
		bit = *thickness;
		if (*bbs[xcounter].areasbbs[0]==EOS)
			*thickness = 1;
		else
			*thickness = 3;
		if (redraw)
			if (bit!=*thickness)
				redraw_one_objc(tree7,ext,handle);
	}

void is_np(int redraw,int node, int xcounter)
	{	int bit,*thickness;
		thickness = &tree7[node].ob_spec.tedinfo->te_thickness;
		bit = *thickness;
		if (*bbs[xcounter].np==EOS)
			*thickness = 1;
		else
			*thickness = 3;
		if (redraw)
			if (bit!=*thickness)
				redraw_one_objc(tree7,node,handle);
	}

void teste_ob_insert(int redraw)
	{	int bit;

		is_extend(redraw,EXTEND1,counter  );
		is_extend(redraw,EXTEND2,counter+1);
		is_extend(redraw,EXTEND3,counter+2);
		is_np(redraw,NODE1,counter  );
		is_np(redraw,NODE2,counter+1);
		is_np(redraw,NODE3,counter+2);
		bit=is_state(tree7,INSERT1,DISABLED);
		if (counter<areas_cnt)
			{	if (bit!=0) insdel_undo(redraw,INSERT1,DELETE1,EXTEND1,NODE1);		}
		else
			{	if (bit==0) insdel_do(redraw,INSERT1,DELETE1,EXTEND1,NODE1);			}

		bit=is_state(tree7,INSERT2,DISABLED);
		if ((counter+1)<areas_cnt)
			{	if (bit!=0)	insdel_undo(redraw,INSERT2,DELETE2,EXTEND2,NODE2);		}
		else
			{	if (bit==0)	insdel_do(redraw,INSERT2,DELETE2,EXTEND2,NODE2);			}

		bit=is_state(tree7,INSERT3,DISABLED);
		if ((counter+2)<areas_cnt)
			{	if (bit!=0)	insdel_undo(redraw,INSERT3,DELETE3,EXTEND3,NODE3);		}
		else
			{	if (bit==0)	insdel_do(redraw,INSERT3,DELETE3,EXTEND3,NODE3);			}
	}

void auslesen(void)
	{	strncpy(bbs[counter  ].address,get_text(tree7,BOSS1),40);
		strncpy(bbs[counter+1].address,get_text(tree7,BOSS2),40);
		strncpy(bbs[counter+2].address,get_text(tree7,BOSS3),40);
		bbs[counter  ].address[40]=EOS;
		bbs[counter+1].address[40]=EOS;
		bbs[counter+2].address[40]=EOS;
		strcpy(bbs[counter  ].name,get_text(tree7,DNAME1));
		strcpy(bbs[counter+1].name,get_text(tree7,DNAME2));
		strcpy(bbs[counter+2].name,get_text(tree7,DNAME3));

		if ( *get_text(tree7,DAYS1)==EOS )
			bbs[counter  ].days=-1;
		else
			bbs[counter  ].days=atoi(get_text(tree7,DAYS1));

		if ( *get_text(tree7,DAYS2)==EOS )
			bbs[counter+1].days=-1;
		else
			bbs[counter+1].days=atoi(get_text(tree7,DAYS2));

		if ( *get_text(tree7,DAYS3)==EOS )
			bbs[counter+2].days=-1;
		else
			bbs[counter+2].days=atoi(get_text(tree7,DAYS3));

		strcpy(bbs[counter  ].origin,get_text(tree7,ORIGIN1));
		strcpy(bbs[counter+1].origin,get_text(tree7,ORIGIN2));
		strcpy(bbs[counter+2].origin,get_text(tree7,ORIGIN3));
		strcpy(bbs[counter  ].path,get_text(tree7,PATH1));
		strcpy(bbs[counter+1].path,get_text(tree7,PATH2));
		strcpy(bbs[counter+2].path,get_text(tree7,PATH3));
	}

void loesche(register int xcounter)
	{	if (xcounter < max_areas)
			memcpy(&bbs[xcounter],&bbs[xcounter+1],sizeof(AREAS_BBS)*(max_areas-xcounter));
		reset_entry(max_areas);
	}

void einfuege(register int xcounter)
	{	if (xcounter < max_areas)
			memcpy(&bbs[xcounter+1],&bbs[xcounter],sizeof(AREAS_BBS)*(max_areas-xcounter-1));
		reset_entry(xcounter);
	}

void insert(int xcounter)
	{	if (areas_cnt+1<=max_areas)
			{	einfuege(xcounter);
				areas_cnt++;
				slider_breite();
				redraw_slider();
			}
		else
			{	areas_error();
			}
	}

int msghdrdel(int xcounter,int dareasflag)
	{	char temp[MAX_STR],temp2[MAX_STR];
		int xx=0;
		if ((divflags & D_CHECKING) && (!dareasflag) )
			{	sprintf(temp,"%s.MSG",bbs[xcounter].path);
				if ( exist(temp)==0 )
					{	sprintf(temp2,tmsg[7],bbs[xcounter].name);
						if ((xx=form_alert(3,temp2))==3)
							{	if (remove(temp)<0)
									{	sprintf(temp2,tmsg[8],temp);
										form_alert(1,temp2);
									}
								sprintf(temp,"%s.HDR",bbs[xcounter].path);
								if (remove(temp)<0)
									{	sprintf(temp2,tmsg[8],temp);
										form_alert(1,temp2);
									}
							}
					}
			}
		return(xx);
	}

void areas_dialog(int dareasflag)
	{	int tt,feldold,feld=0,xwahl,wahl,firstredraw=FALSE,nx,ny,nw,nh;
		char temp[512],temp2[512];
		FILE *fbbs,*fbbs2;

		if (dareasflag)
			{	do_xstate(tree7,DISABLED,PATH1,PATH2,PATH3,DAYS1,DAYS2,DAYS3,
																 ORIGIN0,ORIGIN1,ORIGIN2,ORIGIN3,
																 NPATH1,NPATH2,NPATH3,
																 NORG1,NORG2,NORG3,-1);
				undo_xflags(tree7,EDITABLE,DAYS1,DAYS2,DAYS3,
																	 ORIGIN0,ORIGIN1,ORIGIN2,ORIGIN3,-1);
				undo_xflags(tree7,EDITABLE|TOUCHEXIT,PATH1,PATH2,PATH3,-1);
			}
		else
			{	undo_xstate(tree7,DISABLED,PATH1,PATH2,PATH3,DAYS1,DAYS2,DAYS3,
																	 ORIGIN0,ORIGIN1,ORIGIN2,ORIGIN3,
																	 NPATH1,NPATH2,NPATH3,
																	 NORG1,NORG2,NORG3,-1);
				do_xflags(tree7,EDITABLE,DAYS1,DAYS2,DAYS3,
																 ORIGIN0,ORIGIN1,ORIGIN2,ORIGIN3,-1);
				do_xflags(tree7,EDITABLE|TOUCHEXIT,PATH1,PATH2,PATH3,-1);
			}

		tree7[EXTEND1].ob_spec.tedinfo->te_thickness =
		tree7[EXTEND2].ob_spec.tedinfo->te_thickness =
		tree7[EXTEND3].ob_spec.tedinfo->te_thickness =
		tree7[NODE1].ob_spec.tedinfo->te_thickness =
		tree7[NODE2].ob_spec.tedinfo->te_thickness =
		tree7[NODE3].ob_spec.tedinfo->te_thickness = 1;

		sprintf(temp,"%-3d",max_areas);
		put_text(tree7,CNTMAX,temp,3);

		put_text(tree7,ORIGIN0,origin,65);

		if (*address_bossnode==EOS)
			*temp=EOS;
		else
			sprintf(temp,"%-40s",address_bossnode);
		put_text(tree7,BOSS0,temp,40);

		slider_breite();
		counter = 1;
		OB_X(tree7,ARROWFD) = 0;
		trage_areas();
		sprintf(temp,"%-3d",areas_cnt-1);
		put_text(tree7,CNTAREAS,temp,3);

		xredraw_slider(tree7,handle,ARROWFUL,ARROWBK,ARROWFD,areas_cnt-3,counter,3,FALSE);
		xform_dial(tree7,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		for (;;)
			{	if ( (counter==1) && (areas_cnt<=4) )
					{	if ( (*get_text(tree7,DNAME1)!=EOS) && (*get_text(tree7,PATH1)!=EOS) )	areas_cnt=2;
						if ( (*get_text(tree7,DNAME2)!=EOS) && (*get_text(tree7,PATH2)!=EOS) )	areas_cnt=3;
						if ( (*get_text(tree7,DNAME3)!=EOS) && (*get_text(tree7,PATH3)!=EOS) )	areas_cnt=4;
					}
				if (firstredraw)
					if ((areas_cnt-1)!=atoi(get_text(tree7,CNTAREAS)))
						{	sprintf(temp,"%-3d",areas_cnt-1);
							put_text(tree7,CNTAREAS,temp,3);
							redraw_one_objc(tree7,CNTAREAS,handle);
						}
				for (tt=DNAME3; tt>=DNAME1; tt-=DNAME2-DNAME1)
					if (*get_text(tree7,tt)==EOS)
						feld=tt;

				teste_ob_insert(firstredraw);
				firstredraw = TRUE;
				
				feldold=feld;
				do
					{	xwahl=xform_do(handle,tree7,NULL,feldold,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
						if (xwahl & 0x4000)
							xwahl = ARCANCEL;
						wahl = xwahl & 0x7ff;
						if (xwahl==PATH1 || xwahl==PATH2 || xwahl==PATH3)
							feldold=xwahl;
					}
				while (xwahl==PATH1 || xwahl==PATH2 || xwahl==PATH3);
				undo_state(tree7,wahl,SELECTED);

				if (wahl==ARROWBK)
					{	objc_offset(tree7,ARROWFD,&nx,&ny);
						vq_mouse(vdi_handle,&tt,&nw,&nh);
						if (nw<nx)
							xwahl=wahl=ARROWDN;
						if (nw>nx+OB_W(tree7,ARROWFD))
							xwahl=wahl=ARROWUP;
					}

				if ( xwahl & 0x8000 )
					{	if ( wahl==PATH1 || wahl==PATH2 || wahl==PATH3 )
							{	switch (wahl)
									{	case PATH1 : do_select("",PATH1,tree7,60); break;
										case PATH2 : do_select("",PATH2,tree7,60); break;
										case PATH3 : do_select("",PATH3,tree7,60); break;
									}
								delete_extension(tree7,wahl);
								redraw_one_objc(tree7,wahl,handle);
							}
					}

				auslesen();
				if ((wahl==AROK) || (wahl==ARCANCEL))		break;
				redraw_objc(tree7,wahl,handle);
				if (divflags & D_CHECKING)
					{	switch(wahl)
							{	case DELETE1	: if (msghdrdel(counter  ,dareasflag)==1)
																	wahl=0;
																break;
								case DELETE2	: if (msghdrdel(counter+1,dareasflag)==1)
																	wahl=0;
																break;
								case DELETE3	: if (msghdrdel(counter+2,dareasflag)==1)
																	wahl=0;
							}
					}

				switch(wahl)
					{	case ARSEARCH	:	put_text(tree14,FINDTEXT,find,30);
														form_center(tree14,&nx,&ny,&nw,&nh);
														form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
														form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
														objc_draw(tree14,ROOT,MAX_DEPTH,nx,ny,nw,nh);
														tt=form_do(tree14,0) & 0x7FFF;
														undo_state(tree14,tt,SELECTED);
														form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
														form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
														if (tt==FINDOK)
															{ strcpy(find,get_text(tree14,FINDTEXT));
																strupr(find);
																for (tt=counter+1; tt<=(areas_cnt-1); tt++)
																	{	counter=tt;
																		if (strstr(bbs[tt].name,find)!=NULL)
																			break;
																	}
																if (tt==areas_cnt)
																	form_alert(1,msg[17]);
																else
																	feld=DNAME1;
																if ( (counter>max_areas-2) ||
																		 (counter>(areas_cnt-2)) )
																	counter--;
slider_breite();
																redraw_slider();
															}
														break;
						case EXTEND1	: extend(counter);		break;
						case EXTEND2	: extend(counter+1);	break;
						case EXTEND3	: extend(counter+2);	break;
						case NODE1		: xnp(counter);				break;
						case NODE2		: xnp(counter+1);			break;
						case NODE3		: xnp(counter+2);			break;
						case INSERT1	:	insert(counter);		break;
						case INSERT2	:	insert(counter+1);	break;
						case INSERT3	:	insert(counter+2);	break;
						case DELETE1	:	{	loesche(counter);
															areas_cnt--;
															if (areas_cnt<1) areas_cnt=1;
															if (counter>=(areas_cnt-1))
																{	counter--;
																	if (counter<1) counter=1;
																}
															slider_breite();
															redraw_slider();
															break;
														}
						case DELETE2	:	{	loesche(counter+1);
															areas_cnt--;
															if (areas_cnt<2) areas_cnt=2;
															if (counter>=(areas_cnt-1))
																{	counter--;
																	if (counter<1) counter=1;
																}
															slider_breite();
															redraw_slider();
															break;
														}
						case DELETE3	:	{	loesche(counter+2);
															areas_cnt--;
															if (areas_cnt<3) areas_cnt=3;
															slider_breite();
															redraw_slider();
															break;
														}
						case ARINSERT	: {	tt=FALSE;
															if (dareasflag)
																{	if ( *get_text(tree7,DNAME3)!=EOS )
																		tt=TRUE;
																}
															else
																{	if ( (*get_text(tree7,DNAME3)!=EOS) &&
																			 (*get_text(tree7,PATH3)!=EOS) )
																		tt=TRUE;
																}

															if (tt)
																{	areas_cnt++;
																	if (areas_cnt<=max_areas)
																		{	counter++;
																			slider_breite();
																			redraw_slider();
																		}
																	else
																		{	--areas_cnt;
																			areas_error();
																		}
																	break;
																}
														}
						case ARROWUP	:	{	counter++;
															if ( (counter>max_areas-2) || (counter>(areas_cnt-2)) )
																counter--;
slider_breite();
															redraw_slider();
															break;
														}
						case ARROWDN	: {	counter--;
															if (counter<1) counter=1;
slider_breite();
															redraw_slider();
															break;
														}
						case ARROWFD	:	{	counter=(int) ((long) (areas_cnt-2)* (long) graf_slidebox(tree7,ARROWBK,ARROWFD,0) / 1001L)+1;
slider_breite();
															redraw_slider();
														}
					}
			}
		if (wahl==AROK)
			{ set_defaultdir();
				if (dareasflag)
					strcpy(temp,dareas);
				else
					strcpy(temp,areas);
				fbbs=fopen( temp ,"w");
				if (fbbs==NULL)
					{	fatal_error( temp );
					}
				else
					{	if (!dareasflag)
							{	strcpy(origin,get_text(tree7,ORIGIN0));
								output(origin,fbbs);
							}

						strncpy(temp,get_text(tree7,BOSS0),40);	temp[40]=EOS;
						if (*temp!=EOS)
							{	sprintf(temp2,"-BOSSNODE=%s",temp);
								output(temp2,fbbs);
							}

						for (tt=1; tt<=(areas_cnt-1); tt++)
							{	if ( bbs[tt].days!=-1 )
									{	sprintf(temp,"-DAYS %d",bbs[tt].days);
										output(temp,fbbs);
									}

								if (*bbs[tt].origin!=EOS)
									{	sprintf(temp,"-ORIGIN %s",bbs[tt].origin);
										output(temp,fbbs);
									}

								for (feld=0; feld<=AREAS_OTHER-1; feld++)
									if (*bbs[tt].areasbbs[feld]!=EOS)
										output(bbs[tt].areasbbs[feld],fbbs);

								if (*bbs[tt].address==EOS)
									{
										if (dareasflag)
											sprintf(temp2,"%-20s %s",bbs[tt].name,bbs[tt].np);
										else
											sprintf(temp2,"%-45s %-20s %s",bbs[tt].path,bbs[tt].name,bbs[tt].np);
									}
								else
									{	trimstr(bbs[tt].address);
										if (dareasflag)
											sprintf(temp2,"%-20s %s %s",bbs[tt].name,bbs[tt].address,bbs[tt].np);
										else
											sprintf(temp2,"%-45s %-20s %s %s",bbs[tt].path,bbs[tt].name,bbs[tt].address,bbs[tt].np);
									}
								trimstr(temp2);
								output(temp2,fbbs);

								if ((divflags & D_CHECKING) && (!dareasflag) )
									{	sprintf(temp,"%s.MSG",bbs[tt].path);
										if ( exist(temp)!=0 )
											{	sprintf(temp2,tmsg[6],bbs[tt].name);
												if (form_alert(2,temp2)==2)
													{	fbbs2=fopen(temp,"wb");
														if (fbbs2!=NULL)
															{	fclose(fbbs2);
																sprintf(temp,"%s.HDR",bbs[tt].path);
																fbbs2=fopen(temp,"wb");
																if (fbbs2!=NULL)
																	fclose(fbbs2);
																else
																	fatal_error(temp);
															}
														else
															{	fatal_error(temp);
															}
													}
											}
									}
							}
						fclose(fbbs);
					}
			}
		xform_maindialog(XFMD_DIALOG);
	}

void extend(int xcounter)
	{	register int tt;
		int wahl,nx,ny,nw,nh;
		form_center(tree71,&nx,&ny,&nw,&nh);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		for (tt=0; tt<=AREAS_OTHER-1; tt++)
			put_text(tree71,AREAS1+tt,bbs[xcounter].areasbbs[tt],AREAS_OTHERLEN);
		objc_draw(tree71,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahl=form_do(tree71,0);
		undo_state(tree71,wahl,SELECTED);
		for (tt=0; tt<=AREAS_OTHER-1; tt++)
			strcpy(bbs[xcounter].areasbbs[tt],get_text(tree71,AREAS1+tt));
		form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
	}

void xnp(int xcounter)
	{	char dum[256];
		int wahl,nx,ny,nw,nh;
		form_center(tree72,&nx,&ny,&nw,&nh);
		form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
		if (*bbs[xcounter].address==EOS)
			strcpy(dum,avaltxt[1]);
		else
			strcpy(dum,bbs[xcounter].address);
		put_string(tree72,XAREA,bbs[xcounter].name,54);
		put_string(tree72,XNODE,dum,41);

		strcpy(dum,bbs[xcounter].np);

		if ( strlen(dum)>120 )
			{	put_text(tree72,NLINE3,dum+120,60);	dum[120]=EOS;	}
		else
			put_text(tree72,NLINE3,"",0);
		if ( strlen(dum)>60 )
			{	put_text(tree72,NLINE2,dum+60,60);	dum[60]=EOS;	}
		else
			put_text(tree72,NLINE2,"",0);
		put_text(tree72,NLINE1,dum,60);

		objc_draw(tree72,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		wahl=form_do(tree72,0);
		undo_state(tree72,wahl,SELECTED);
		strcpy(bbs[xcounter].np,get_text(tree72,NLINE1));
		strcat(bbs[xcounter].np,get_text(tree72,NLINE2));
		strcat(bbs[xcounter].np,get_text(tree72,NLINE3));
		form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
		form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
	}
