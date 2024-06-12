#include <aes.h>
#include <vdi.h>
#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "avalonio.h"
#include "avallang.h"
#include "define.h"
#include "diverse.h"
#include "transkey.h"
#include "form_run.h"
#include "crc32.h"
#include "vars.h"
#include "version.h"

extern void smallhelp(int feld);

void	hatch(void);
static void	erstelle_mail(int num,char *file,char *replaces,char *sendarea,
										char *sendtxt,long size,int zone,int net,int node,
										int point,int mode,time_t xtime,char *filename,
										char *sysopname,char *fromaddr);

void hatch(void)
	{	int xwahl,wahl=0,len4d,mode,zone,net,node,point,nx,ny,nw,nh,
				myzone,mynet,mynode,mypoint,wahltemp,i,j,htic;
		time_t	xtime;
		long	crc32,len,size;
		char	filename[66],replaces[15],file[15],sendtxt[210],
					ticfilename[MAX_STR],temp2[210],holdpath[MAX_STR],*pointer,
					dum[210],ddomain[21],
					snet[10],snode[10],spoint[10],*copy_buffer;
		FILE *ftic;
		DOSTIME	dostime;
		static char ptext1[] ="12345678901234",
								ptmplt1[]="Node: _____/_____.____",
								pvalid1[]="99999999999999",
								ptext2[] ="1234567890",
								ptmplt2[]="Node: _____/_____",
								pvalid2[]="9999999999";
 
		if (divflags & D_USE4D)
			{	put_template(tree22,SENDNODE,ptext1,ptmplt1,pvalid1);
				len4d = 14;
			}
		else
			{	put_template(tree22,SENDNODE,ptext2,ptmplt2,pvalid2);
				len4d = 10;
			}
		for (i=0; i<=MAX_HATCH-1; i++)
			put_text(tree22,SENDDEF1+i,hatchdefbutton[i],11);
		put_text(tree22,SENDREP1,mailname[1],8);
		put_text(tree22,SENDREP2,mailname[2],8);
		put_text(tree22,SENDAREA,hatcharea,35);
		put_text(tree22,SENDNAME,hatchsysop,25);
		put_text(tree22,SENDPASS,hatchpassword,12);
		put_text(tree22,SENDNODE,hatchnode,len4d);
		undo_xstate(tree22,SELECTED,SENDTOUC,SENDTIC,SENDNONE,SENDATTA,
																SENDMAIL,SENDREP1,SENDREP2,-1);

		if (hatchflags & D_TOUCH	 )	do_state(tree22,SENDTOUC,SELECTED);

		if (hatchflags & D_TIC		 )	{	do_state(tree22,SENDTIC ,SELECTED);
																		wahl++;
																	}
		if (wahl==0 &&
				hatchflags & D_MAILFILE)	{	do_state(tree22,SENDATTA,SELECTED);
																		wahl++;
																	}
		if (wahl==0 &&
				hatchflags & D_NONE		 )	{	do_state(tree22,SENDNONE,SELECTED);
																		wahl++;
																	}
		if (wahl==0)	do_state(tree22,SENDNONE,SELECTED);

		if (hatchflags & D_NETMAIL )	do_state(tree22,SENDMAIL,SELECTED);
		if (hatchflags & D_REPORT1 )	do_state(tree22,SENDREP1,SELECTED);
		if (hatchflags & D_REPORT2 )	do_state(tree22,SENDREP2,SELECTED);

		do
			{	put_text(tree22,SENDFILE,"",0);
				put_text(tree22,SENDTXT1,"",0);
				put_text(tree22,SENDTXT2,"",0);
				put_text(tree22,SENDTXT3,"",0);
				put_text(tree22,SENDREPL,"",0);
				wahl=0;
				xform_dial(tree22,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				do
					{	do
							{	xwahl=xform_do(handle,tree22,NULL,wahl,0,trans_key5,&wnx,&wny,&wnw,&wnh);
								if (xwahl & 0x4000)
									xwahl = SENDCANC;
								wahl = xwahl & 0x7ff;
								if (wahl!=SENDMAIL && wahl!=SENDREP1 && wahl!=SENDREP2 &&
										wahl<1000)
									undo_state(tree22,wahl,1);
								if (wahl==1051)
									smallhelp(HELPSND);
								if (xwahl & 0x8000)
									{	switch(wahl)
											{	case SENDFILE	: {	do_select("*.*",SENDFILE,tree22,65);
																					redraw_one_objc(tree22,wahl,handle);
																				}	break;
												case SENDREP2	:
												case SENDREP1	:
												case SENDMAIL	:	{	form_center(tree24,&nx,&ny,&nw,&nh);
																					form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
																					form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
																					switch(wahl)
																						{	case SENDMAIL	: put_text(tree24,EDITTEXT,"Netmail      ",13);
																															mode=0;
																															break;
																							case SENDREP1	: put_text(tree24,EDITTEXT,"1.Reportarea ",13);
																															mode=1;
																															break;
																							case SENDREP2	: put_text(tree24,EDITTEXT,"2.Reportarea ",13);
																															mode=2;
																															break;
																						}
																					put_text(tree24,EDITMAIL,netmail[mode],50);
																					if (mode==0)
																						{	OB_FLAGS(tree24,EDITNAME) |= HIDETREE;
																							OB_FLAGS(tree24,EDITNAME) &= ~EDITABLE;
																						}
																					else
																						{	OB_FLAGS(tree24,EDITNAME) &= ~HIDETREE;
																							OB_FLAGS(tree24,EDITNAME) |= EDITABLE;
																							put_text(tree24,EDITNAME,mailname[mode],8);
																						}
																					objc_draw(tree24,ROOT,MAX_DEPTH,nx,ny,nw,nh);
																					do
																						{	wahltemp=form_do(tree24,0) & 0x8fff;
																							if (wahltemp & 0x8000)
																								{	do_select("*.*",EDITMAIL,tree24,50);
																									delete_extension(tree24,EDITMAIL);
																									redraw_one_objc(tree24,EDITMAIL,-1);
																									undo_state(tree24,wahltemp & 0x7ff,SELECTED);
																									wahltemp=EDITOK;
																								}
																							undo_state(tree24,wahltemp,SELECTED);
																						}
																					while (wahltemp!=EDITOK);
																					if (mode!=0)
																						{	strcpy(mailname[mode],get_text(tree24,EDITNAME));
																							put_text(tree22,wahl,mailname[mode],8);
																						}
																					strcpy(netmail[mode],get_text(tree24,EDITMAIL));
																					form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
																					form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
																					redraw_one_objc(tree22,wahl,handle);
																					wahl=0;
																					changes |= C_HATCHMAIL;
																				}	break;
											}
										if (wahl>=SENDDEF1 && wahl<=SENDDEF1+MAX_HATCH-1)
											{	form_center(tree23,&nx,&ny,&nw,&nh);
												form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
												form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
												put_text(tree23,DEFNAME,hatchdefbutton[wahl-SENDDEF1],11);
												put_text(tree23,DEFSYSOP,hatchdefsysop[wahl-SENDDEF1],25);
												put_text(tree23,DEFNODE,hatchdefnode[wahl-SENDDEF1],14);
												put_text(tree23,DEFAREA,hatchdefarea[wahl-SENDDEF1],35);
												put_text(tree23,DEFPASS,hatchdefpw[wahl-SENDDEF1],12);
												objc_draw(tree23,ROOT,MAX_DEPTH,nx,ny,nw,nh);
												do
													{	wahltemp=form_do(tree23,0) & 0x7ff;
														undo_state(tree23,wahltemp,SELECTED);
													}
												while (wahltemp!=DEFOK);
												strcpy(hatchdefbutton[wahl-SENDDEF1],get_text(tree23,DEFNAME));
												strcpy(hatchdefnode[wahl-SENDDEF1],get_text(tree23,DEFNODE));
												strcpy(hatchdefsysop[wahl-SENDDEF1],get_text(tree23,DEFSYSOP));
												strcpy(hatchdefarea[wahl-SENDDEF1],get_text(tree23,DEFAREA));
												strcpy(hatchdefpw[wahl-SENDDEF1],get_text(tree23,DEFPASS));
												form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
												form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);
												put_text(tree22,wahl,hatchdefbutton[wahl-SENDDEF1],11);
												redraw_one_objc(tree22,SENDNODE,handle);
												redraw_one_objc(tree22,SENDNAME,handle);
												redraw_one_objc(tree22,SENDPASS,handle);
												redraw_one_objc(tree22,SENDAREA,handle);
												redraw_objc(tree22,wahl,handle);
												wahl=0;
												changes |= C_HATCHDEF;
											}
									}
								else	/* Beginn: Einfachklick */
									{	if (wahl>=SENDDEF1 && wahl<=SENDDEF1+MAX_HATCH-1)
											{	put_text(tree22,SENDNODE,hatchdefnode[wahl-SENDDEF1],14);
												put_text(tree22,SENDAREA,hatchdefarea[wahl-SENDDEF1],35);
												put_text(tree22,SENDNAME,hatchdefsysop[wahl-SENDDEF1],25);
												put_text(tree22,SENDPASS,hatchdefpw[wahl-SENDDEF1],12);
												redraw_one_objc(tree22,SENDNODE,handle);
												redraw_one_objc(tree22,SENDNAME,handle);
												redraw_one_objc(tree22,SENDPASS,handle);
												redraw_one_objc(tree22,SENDAREA,handle);
												redraw_objc(tree22,wahl,handle);
											}
										if (wahl==SENDLOAD)
											{	*dum=EOS;
												if ( fileselect(temp2,dum,"*.*",avaltxt[2]) != 0 )
													{	strcat(temp2,dum);
														if (exist(temp2)!=0)
															{	form_alert(1,msg[6]);
															}
														else
															{	pointer = sendtxt;
																i = 0;
																ftic=fopen(temp2,"r");
																while (!feof(ftic) && i<195)
																	{	j = getc(ftic);
																		if (j==EOF)
																			break;
																		if (j=='\n')
																			j = ' ';
																		*pointer++ = j;
																		i++;
																	}
																*pointer = EOS;
																fclose(ftic);
																strcpy(temp2,sendtxt);
																pointer=temp2; trim(&pointer); strcpy(temp2,pointer);
																put_text(tree22,SENDTXT1,"",0);
																put_text(tree22,SENDTXT2,"",0);
																put_text(tree22,SENDTXT3,"",0);
																if (strlen(temp2)>130)
																	{	put_text(tree22,SENDTXT3,temp2+130,65);
																		temp2[130]=EOS;
																	}
																if (strlen(temp2)>65)
																	{	put_text(tree22,SENDTXT2,temp2+65,65);
																		temp2[65]=EOS;
																	}
																put_text(tree22,SENDTXT1,temp2,65);
																redraw_one_objc(tree22,SENDTXT1,handle);
																redraw_one_objc(tree22,SENDTXT2,handle);
																redraw_one_objc(tree22,SENDTXT3,handle);
															}
													}
												redraw_one_objc(tree22,wahl,handle);
											}
										if (wahl!=SENDSAVE && wahl!=SENDCANC && wahl!=SENDSAVZ)
											wahl=0;
									}
								strcpy(filename,get_text(tree22,SENDFILE));
								strcpy(replaces,get_text(tree22,SENDREPL));
								if ((pointer=strrchr(filename,'\\'))!=NULL)
									strcpy(file,pointer+1);
								else if ((pointer=strrchr(filename,':'))!=NULL)
									strcpy(file,pointer+1);
								else
									strcpy(file,filename);
								if (wahl==SENDFILE)
									{	strcpy(replaces,file);
										put_text(tree22,SENDREPL,replaces,12);
										redraw_one_objc(tree22,SENDREPL,handle);
									}
							}
						while ( (wahl!=SENDCANC) && (wahl!=SENDSAVE) && (wahl!=SENDSAVZ));
						redraw_objc(tree22,wahl,handle);
		
						/*
						**	Aktuelle Einstellungen ermitteln
						*/

						strcpy(hatchnode,get_text(tree22,SENDNODE));
						strcpy(hatchsysop,get_text(tree22,SENDNAME));
						strcpy(hatcharea,get_text(tree22,SENDAREA));
						strcpy(hatchpassword,get_text(tree22,SENDPASS));
						hatchflags &= ~(D_TOUCH|D_TIC|D_NONE|D_MAILFILE|
														D_NETMAIL|D_REPORT1|D_REPORT2);
						if (is_state(tree22,SENDTOUC,SELECTED))	hatchflags |= D_TOUCH;
						if (is_state(tree22,SENDTIC ,SELECTED))	hatchflags |= D_TIC;
						if (is_state(tree22,SENDNONE,SELECTED))	hatchflags |= D_NONE;
						if (is_state(tree22,SENDATTA,SELECTED))	hatchflags |= D_MAILFILE;
						if (is_state(tree22,SENDMAIL,SELECTED))	hatchflags |= D_NETMAIL;
						if (is_state(tree22,SENDREP1,SELECTED))	hatchflags |= D_REPORT1;
						if (is_state(tree22,SENDREP2,SELECTED))	hatchflags |= D_REPORT2;

						/*
						**	Wurden alle n”tigen Angaben gemacht?
						*/

						if (wahl==SENDSAVE || wahl==SENDSAVZ)
							{	if (hatchflags & D_TIC)
									{	if (*hatcharea==EOS)
											{	form_alert(1,msg[35]);
												wahl=SENDAREA;
											}
									}
								if (exist(filename)!=0)
									{	form_alert(1,msg[6]);
										wahl=SENDFILE;
									}
								if (strlen(hatchnode)<6)
									{	form_alert(1,msg[15]);
										wahl=SENDNODE;
									}
								if (*netmail[0]==EOS && (hatchflags & D_NETMAIL) ||
									 (*netmail[0]==EOS && (hatchflags & D_MAILFILE) ))
									{	form_alert(1,msg[30]);
										wahl=0;
									}
								if ( (*netmail[1]==EOS && (hatchflags & D_REPORT1) ) ||
										 (*netmail[2]==EOS && (hatchflags & D_REPORT2) ) )
									{	form_alert(1,msg[32]);
										wahl=0;
									}
								if (*replaces!=EOS)
									{	net=FALSE;
										pointer=replaces;
										while (*pointer!=EOS)
											{	if (*pointer=='?' || *pointer=='*')
													net=TRUE;
												pointer++;
											}
										if (net==TRUE)
											{	if (form_alert(2,msg[31])==1)
													wahl=SENDREPL;
											}
									}
							}

						/*
						**	Zone und Domain-Hatch
						*/

						if (wahl==SENDSAVZ)
							{	put_text(tree81,REQZONE,"",0);
								put_text(tree81,REQDOMAI,"",0);
								setze_presets();
								form_center(tree81,&nx,&ny,&nw,&nh);
								form_dial(FMD_START,mid_x,mid_y,1,1,nx,ny,nw,nh);
								form_dial(FMD_GROW,mid_x,mid_y,1,1,nx,ny,nw,nh);
								objc_draw(tree81,ROOT,MAX_DEPTH,nx,ny,nw,nh);
		
								do
									{	i=form_do(tree81,0) & 0x7FFF;
										undo_state(tree81,i,SELECTED);
										if (i>=PRESET0 && i<=PRESET0+MAX_PRESET-1)
											{	*dum = *temp2 = EOS;
												switch(sscanf(get_text(tree81,i),"%s - %s",dum,temp2))
													{	case 0	: *dum=EOS;
														case 1	: *temp2 = EOS;
													}
												put_text(tree81,REQZONE,dum,3);
												put_text(tree81,REQDOMAI,temp2,20);
												redraw_one_objc(tree81,REQZONE,-1);
												redraw_one_objc(tree81,REQDOMAI,-1);
												redraw_one_objc(tree81,i,-1);
											}
									}
								while (i!=REQZSAVE && i!=REQZCAN);
								form_dial(FMD_SHRINK,mid_x,mid_y,1,1,nx,ny,nw,nh);
								form_dial(FMD_FINISH,mid_x,mid_y,1,1,nx,ny,nw,nh);

								/*
								**	Es wurde ein Preset angew„hlt
								*/

								if (i==REQZSAVE)
									{	zone=atoi(get_text(tree81,REQZONE));
										strcpy(dum,get_text(tree81,REQDOMAI));
										if (*dum!=EOS)
											{	if (domain_cnt==-1)
													{	form_alert(1,msg[27]);
														wahl=0;
													}
												else
													{	j=-1;
														for (i=0; i<=domain_cnt; i++)
															{	if (stricmp(dum,domain[i][0])==0 ||
																		stricmp(dum,domain[i][1])==0)
																	{	j=i;
																		break;
																	}
															}
														if (j==-1)
															{	form_alert(1,msg[28]);
																wahl=0;
															}
														else
															{	strcpy(ddomain,domain[j][1]);
															}
													}
											}
									}
								else
									{	wahl=0;
									}
							}
						else
							{	zone=0;
							}
					}
				while ( (wahl!=SENDCANC) && (wahl!=SENDSAVE) && (wahl!=SENDSAVZ));
		
				/*
				**	Daten aufbereiten (Mode,Adresse,etc.)
				*/

				if (wahl==SENDSAVE || wahl==SENDSAVZ)
					{	net =
						node =
						point = 0;
						strncpy(snet,hatchnode,5); snet[5]=EOS;
						if (strlen(hatchnode)>5)
							{	strncpy(snode,hatchnode+5,5);	snode[5]=EOS;	}
						else
							{	strcpy(snode,"0"); }
						if (strlen(hatchnode)>10)
							{	strncpy(spoint,hatchnode+10,4);	spoint[4]=EOS; }
						else
							{	strcpy(spoint,"0"); }
						net = atoi(snet);
						node = atoi(snode);
						point = atoi(spoint);
						strcpy(sendtxt,get_text(tree22,SENDTXT1));
						strcat(sendtxt,get_text(tree22,SENDTXT2));
						strcat(sendtxt,get_text(tree22,SENDTXT3));
						mode=0;
						if (is_state(tree22,SENDCRAS,SELECTED))	mode='C';
						if (is_state(tree22,SENDDIRE,SELECTED))	mode='D';
						if (is_state(tree22,SENDHOLD,SELECTED))	mode='H';
						if (is_state(tree22,SENDNORM,SELECTED))
							{	if (divflags & D_USE4D)	mode='O';
								else										mode='F';
							}
						time(&xtime);

						/*
						**	Dateinamen erzeugen
						*/

						if (wahl==SENDSAVE)											/* holdpath enth„lt PATH */
							{	strcpy(holdpath,hold);
							}
						else
							{	strcpy(dum,get_text(tree81,REQDOMAI));
		
								/* Ist es unser Defaultdomain? */
		
								scanaddr(address[0],&i,&i,&i,&i,temp2);
								if (stricmp(temp2,dum)==0)
									*dum=EOS;
		
								if (*dum!=EOS)
									{	strcpy(dum,hold);
										pointer=strchr(dum,EOS);
										if (pointer>dum)
											*(--pointer)=EOS;
										pointer=strrchr(dum,'\\');
										if (pointer!=NULL)
											*(pointer+1)=EOS;
		/*							pointer=dum+strlen(dum)-1; *pointer=EOS;
										while (pointer>=dum && *pointer!='\\') *pointer--=EOS;
		*/
										strcat(dum,ddomain);

										if (divflags & D_USEOUT36)
										{	strcpy(temp2,"000");
											itoa(zone,temp2+3,36);
											strcpy(temp2,temp2+strlen(temp2)-3);
										}
										else
											sprintf(temp2,"%03.3X",zone);

										sprintf(holdpath,"%s.%3.3s\\",dum,temp2);
									}
								else
									{	pointer=hold+strlen(hold)-1; *pointer=EOS;

										if (divflags & D_USEOUT36)
										{	strcpy(temp2,"000");
											itoa(zone,temp2+3,36);
											strcpy(temp2,temp2+strlen(temp2)-3);
										}
										else
											sprintf(temp2,"%03.3X",zone);

										sprintf(holdpath,"%s.%3.3s\\",hold,temp2);
										*pointer='\\';
									}
							}
		
						/*
						**	Touch-File
						*/

						if (hatchflags & D_TOUCH)
							{	len = Fopen(filename,FO_READ);
								if (len<0)
									{	form_alert(1,msg[34]);
									}
								else
									{	dostime.date = Tgetdate();
										dostime.time = Tgettime();
										Fdatime(&dostime,(int) len,1);
										if (dostime.date==0 && dostime.time==0)
											form_alert(1,msg[34]);
										Fclose((int) len);
									}
							}
						
						htic = (int) Fopen(filename,FO_READ);
						if (htic<0)
							{	fatal_error(filename);
							}
						else
							{	size = len = filelength(htic);
								Fclose(htic);
							}
		
						i=address_cnt+1;
#if 0
						if (wahl==SENDSAVZ)
#endif
						{
							for (i=0; i<=address_cnt; i++)
							{	strcpy(temp2,address[i]);
								if ((pointer=strchr(temp2,'@'))!=NULL)
									*pointer=EOS;
/* Meine Adresse */			scanaddr(temp2,&myzone,&mynet,&mynode,&mypoint,dum);
								if (myzone==zone)
									break;
							}
						}
						if (i>address_cnt)
						{	strcpy(temp2,address[0]);
							if ((pointer=strchr(temp2,'@'))!=NULL)
								*pointer=EOS;
/* Meine Adresse */			scanaddr(temp2,&myzone,&mynet,&mynode,&mypoint,dum);
						}

						/*
						**	Erzeuge TIC-File
						*/

						if (hatchflags & D_TIC)													/* Erstelle TIC */
						{	htic = (int) Fopen(filename,FO_READ);
							if (htic<0)
							{	fatal_error(filename);
							}
							else
							{	copy_buffer = (char *) malloc(BUFFER_SIZE);
								if (copy_buffer==NULL)
								{	memory_error();
								}
								else
								{	crc32 = 0xffffffffL;
									while ((len=Fread(htic,BUFFER_SIZE,copy_buffer)) > 0)
										crc32 = Crc32(crc32,copy_buffer,len);
									crc32 = ~crc32;
									free(copy_buffer);
								}
								Fclose(htic);
		
								sprintf(ticfilename,"%s\AT%06lX.TIC",holdpath,xtime & 0xffffffL);
								ftic=fopen(ticfilename,"w");						/* Erstelle TIC */
								if (ftic==NULL)
								{	fatal_error(ticfilename);
								}
								else
								{	
#if 0
									i=address_cnt+1;
									if (wahl==SENDSAVZ)
									{
										for (i=0; i<=address_cnt; i++)
										{	strcpy(temp2,address[i]);
											if ((pointer=strchr(temp2,'@'))!=NULL)
												*pointer=EOS;
/* Meine Adresse */			scanaddr(temp2,&myzone,&mynet,&mynode,&mypoint,dum);
											if (myzone==zone)
												break;
										}
									}
									if (i>address_cnt)
									{	strcpy(temp2,address[0]);
										if ((pointer=strchr(temp2,'@'))!=NULL)
											*pointer=EOS;
/* Meine Adresse */			scanaddr(temp2,&myzone,&mynet,&mynode,&mypoint,dum);
									}
#endif
									mergeaddr(temp2,myzone,mynet,mynode,mypoint,"");

									fprintf(ftic,"Area %s\n",hatcharea);
									fprintf(ftic,"Origin %s\n",temp2);

									if (divflags & D_USE3DTIC && fakenet[0]!=0)
										mergeaddr(temp2,myzone,fakenet[0],mypoint,0,"");
									fprintf(ftic,"From %s\n",temp2);

									if (wahl==SENDSAVZ)
										mergeaddr(temp2,zone,net,node,point,"");
									else
										mergeaddr(temp2,myzone,net,node,point,"");

									if ( *hatchsysop=='\0' )
										fprintf(ftic,"To Sysop,%s\n",temp2);
									else
										fprintf(ftic,"To %s,%s\n",hatchsysop,temp2);

									fprintf(ftic,"File %s\n",file);
									if (*replaces!=EOS)
										fprintf(ftic,"Replaces %s\n",replaces);
/*											else
										fprintf(ftic,"Replaces %s\n",file);
*/
									fprintf(ftic,"Size %lu\n",size);
									fprintf(ftic,"Desc %s\n",sendtxt);
									fprintf(ftic,"CRC %lX\n",crc32);
									fprintf(ftic,"Created by AVALON V%s by Stephan Slabihoud\n",VERSION);

									if (divflags & D_USE3DTIC && fakenet[0]!=0)
										mergeaddr(temp2,myzone,fakenet[0],mypoint,0,"");
									else
										mergeaddr(temp2,myzone,mynet,mynode,mypoint,"");
									fprintf(ftic,"Path %s %8lu %s",temp2,xtime,ctime(&xtime));
									fprintf(ftic,"Seenby %s\n",temp2);

									if (wahl==SENDSAVZ)
										mergeaddr(temp2,zone,net,node,point,"");
									else
										mergeaddr(temp2,myzone,net,node,point,"");
									fprintf(ftic,"Seenby %s\n",temp2);

									fprintf(ftic,"Pw %s\n",hatchpassword);
									fclose(ftic);
								}
							}
						}
		
						if (hatchflags & D_MAILFILE)	erstelle_mail(100,file,replaces,hatcharea,sendtxt,size,zone,net,node,point,mode,xtime,filename,hatchsysop,mergeaddr(temp2,myzone,mynet,mynode,mypoint,""));
						if (hatchflags & D_NETMAIL )	erstelle_mail(  0,file,replaces,hatcharea,sendtxt,size,zone,net,node,point,mode,xtime,filename,hatchsysop,mergeaddr(temp2,myzone,mynet,mynode,mypoint,""));
						if (hatchflags & D_REPORT1 )	erstelle_mail(  1,file,replaces,hatcharea,sendtxt,size,zone,net,node,point,mode,xtime,filename,hatchsysop,mergeaddr(temp2,myzone,mynet,mynode,mypoint,""));
						if (hatchflags & D_REPORT2 )	erstelle_mail(  2,file,replaces,hatcharea,sendtxt,size,zone,net,node,point,mode,xtime,filename,hatchsysop,mergeaddr(temp2,myzone,mynet,mynode,mypoint,""));
		
						if (divflags & D_USE4D)
							sprintf(temp2,"%s.%cFT",addr2file36(net,node,point),mode);
						else
							sprintf(temp2,"%s.%cLO",addr2file(net,node),mode);
		
						/*
						**	Erstelle FLO-File
						*/

						if (wahl==SENDSAVE || wahl==SENDSAVZ)
							{	if (!(hatchflags & D_MAILFILE))
									{	sprintf(dum,"%s%s",holdpath,temp2);
										ftic=fopen(dum,"a");								/*	Erstelle FLO-File */
										if (ftic==NULL)
											{	if (exist(ticfilename)==0)
													remove(ticfilename);
												fatal_error(dum);
											}
										else
											{	if (hatchflags & D_TIC ||
														hatchflags & D_NONE)
													fprintf(ftic,"%s\n",filename);
												if (hatchflags & D_TIC)
													fprintf(ftic,"^%s\n",ticfilename);
												fclose(ftic);
											}
										pipeflags |= P_SEMP_REREAD;
									}
							}
					}

			}
		while (wahl!=SENDCANC);

		xform_maindialog(XFMD_DIALOG);
	}

static void erstelle_mail(int num,char *file,char *replaces,char *sendarea,
									 char *sendtxt,long size,int zone,int net,int node,
									 int point,int mode,time_t xtime,char *filename,
									 char *sysopname,char *fromaddr)
	{	FILE *ftic,*ftxt;
		char temp2[300],temp3[MAX_STR],seenby[MAX_STR],*pointer;
		int myzone,mynet,mynode,mypoint,i;

		strcpy(seenby,fromaddr);
		if ((pointer=strchr(seenby,'@'))!=NULL)
			*pointer=EOS;
		scanaddr(seenby,&myzone,&mynet,&mynode,&mypoint,temp3);

		if (num==100)
			strcpy(temp2,netmail[0]);
		else
			strcpy(temp2,netmail[num]);
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

				set_defaultdir();
				if (exist("AVALON\\AVALON.HTC")==0)
					{	ftxt=fopen("AVALON\\AVALON.HTC","rb");
						if (ftxt==NULL)
							{	fatal_error("AVALON.HTC");
							}
						else
							{	while (!feof(ftxt))
									{	i = getc(ftxt);
										if ( i!='\r' && i!=EOS && i!=EOF)
											{	putc(i,ftic);
												header.wSize++;
											}
									}
								fclose(ftxt);
							}
					}

				if (num==0)
					{	sprintf(temp2,"\nYou received following file:\n\n");
						fputs(temp2,ftic);
						header.wSize += (int) strlen(temp2);
					}
				else
					{	sprintf(temp2,"\nNew file received:\n\n");
						fputs(temp2,ftic);
						header.wSize += (int) strlen(temp2);
					}

				sprintf(temp2,"  File: %s",file);
				fputs(temp2,ftic);
				header.wSize += (int) strlen(temp2);
				if (*replaces!=EOS && (stricmp(replaces,file)!=0))
					sprintf(temp2,"  (replaces %s)\n",replaces);
				else
					strcpy(temp2,"\n");
				fputs(temp2,ftic);
				header.wSize += (int) strlen(temp2);

				if (*sendarea!=EOS)
					{	sprintf(temp2,"  Area: %s\n",sendarea);
						fputs(temp2,ftic);
						header.wSize += (int) strlen(temp2);
					}

				if (size!=0)
					{	sprintf(temp2,"  Size: %lu\n",size);
						fputs(temp2,ftic);
						header.wSize += (int) strlen(temp2);
					}

				sprintf(temp2,"  From: %s\n",temp3);
				fputs(temp2,ftic);
				header.wSize += (int) strlen(temp2);

				if (*sendtxt!=EOS)
					{	strcpy(temp2,sendtxt);
						pointer = strtok(temp2," ");
						i = (int)strlen(pointer);
						fputs("  Desc:",ftic);
						header.wSize += 7;
						while (pointer!=NULL)
							{	header.wSize += (int)strlen(pointer);
								if (i>=68)
									{	i = (int)strlen(pointer);
										fputs("\n        ",ftic);
										fputs(pointer,ftic);
										header.wSize += 9;
									}
								else
									{	putc(' ',ftic);
										fputs(pointer,ftic);
										header.wSize++;
										i++;
									}
								pointer = strtok(NULL," ");
								if (pointer!=NULL)
									i += (int)strlen(pointer);
							}
					}

				sprintf(temp2,"\n\n+++ Hatched by AVALON\n");
				fputs(temp2,ftic);
				putc(EOS,ftic);
				header.wSize += (int) strlen(temp2)+1;
				fclose(ftic);

				if (num==100)
					strcpy(temp2,netmail[0]);
				else
					strcpy(temp2,netmail[num]);
				strcat(temp2,".HDR");
				ftic=fopen(temp2,"ab");
				if (ftic==NULL)
					{	fatal_error(temp2);
					}
				else
					{	setvbuf(ftic,NULL,_IOFBF,8*sizeof(MSG_HEADER));
						fseek(ftic,0,SEEK_END);
						strcpy(header.szFrom,txtsysop);
						if (num==0 || num==100)
							{	if ( *sysopname=='\0' )
									strcpy(header.szTo,"Sysop");
								else
									strcpy(header.szTo,sysopname);
							}
						else
							{	strcpy(header.szTo,"All");
							}
						if (num==100)
							strcpy(header.szSubject,filename);
						else
							strcpy(header.szSubject,"New file received");
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
						header.wToPoint= point;
						header.lDate = xtime;
						strftime(header.szDate,20,"%d %b %y %X",localtime(&header.lDate) );
						switch(num)
							{	case		0	:	header.wFlags= MF_LOCAL | MF_PRIVATE;
														break;
								case	100	:	header.wFlags= MF_LOCAL | MF_PRIVATE | MF_WITHFILE;
														break;
								default		:	header.wFlags= MF_LOCAL;
							}
						if ((num==0 || num==100) && (divflags & D_KSHATCH))
							header.wFlags |= MF_KILLIFSENT;
						if ((num==0 || num==100) && mode=='C')
							header.wFlags |= MF_CRASH;

						delete_unused_flags(&header);

						fwrite(&header,sizeof(MSG_HEADER),1,ftic);
						fclose(ftic);
					}
			}
	}
