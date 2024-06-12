#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <tos.h>
#include <fido.h>
#include "define.h"
#include "diverse.h"
#include "avalonio.h"
#include "avallang.h"
#include "windmini.h"
#include "version.h"
#include "form_run.h"
#include "scancode.h"
#include "my_scan.h"
#include "vars.h"

static int	count,kbyte,msgs,deleted,fromsysop,tosysop,line;

static void headline(void)
	{	wind_print(&wind," Name                        KB  Msgs   Del  Left    To/From   Boss");
		wind_print(&wind,"------------------------------------------------------------------------------");
	}

static int teste_area(char *pfad, char *area, char *addr)
	{	FILE *fbbs;
		char filename[MAX_STR],
				 dummy1[10],
				 dummy2[10],
				 dummy3[10],
				 dummy4[10],
				 dummy5[10],
				 dummy6[10];
		MSG_HEADER hdr;
		int	c_msgs=0,c_deleted=0,c_kbyte,c_fromsysop=0,c_tosysop=0,
				lensysop,i;

		lensysop=(int) strlen(txtsysop);
		strcpy(filename,pfad);
		strcat(filename,".MSG");

		fbbs=fopen(filename,"rb");
		if (fbbs==NULL)
			{	sprintf("cannot open msgbase of %s",area);
				wind_print(&wind,filename);
				return(TRUE);
			}
		c_kbyte=(int) (filelength(fbbs->Handle) >> 10);
		fclose(fbbs);

		strcpy(filename,pfad);
		strcat(filename,".HDR");
		fbbs=fopen(filename,"rb");
		if (fbbs==NULL)
			{	sprintf("cannot open headerfile of %s",area);
				wind_print(&wind,filename);
				return(TRUE);
			}
		c_kbyte +=(int) (filelength(fbbs->Handle) >> 10);
		kbyte += c_kbyte;
		count++;

		while ( !feof(fbbs) )
			{	fread(&hdr,sizeof(MSG_HEADER),1,fbbs);
				if (feof(fbbs)) break;
				c_msgs++;
				if (hdr.wFlags & MF_DELETED)
					{	c_deleted++;
					}
				else
					{	if ( (strnicmp(hdr.szFrom,"sysop",5)==0) ||
								 (strnicmp(hdr.szFrom,txtsysop,lensysop)==0) )
							{	c_fromsysop++;
								fromsysop++;
							}
						if ( (strnicmp(hdr.szTo,"sysop",5)==0) ||
								 (strnicmp(hdr.szTo,txtsysop,lensysop)==0) )
							{	c_tosysop++;
								tosysop++;
							}
					}
			}
		fclose(fbbs);

		msgs += c_msgs;
		deleted += c_deleted;
		if (strlen(area)>24)
			{	strncpy(filename,area,12);
				strcpy(filename+12,"~~~");
				strcat(filename,area+strlen(area)-9);
				strcpy(area,filename);
			}
		sprintf(filename," %-24s  %4s  %4s  %4s  %4s  %4s/%4s   %s",
																								area,
																								c_kbyte==0 ? "-" : itoa(c_kbyte,dummy1,10),
																								c_msgs==0 ? "-" : itoa(c_msgs,dummy2,10),
																								c_deleted==0 ? "" : itoa(c_deleted,dummy3,10),
																								c_msgs-c_deleted==0 ? "-" : itoa(c_msgs-c_deleted,dummy4,10),
																								c_tosysop==0 ? "-" : itoa(c_tosysop,dummy5,10),
																								c_fromsysop==0 ? "-" : itoa(c_fromsysop,dummy6,10),
																								addr);
		wind_print(&wind,filename);
		if (++line>=wind.max_y-2)
			{	wind_write(&wind,"--- press SPACE to continue (UNDO to exit) ---");
				do
					{	i = wind_pause(&wind);
					}
				while (i != KEY_SPACE && i!=UNDO );
				if ( i==UNDO )
					{	return(FALSE);
					}
				else
					{	wind_putch(&wind,27,VT52);
						wind_putch(&wind,'E',VT52);
						headline();
					}
				line=0;
			}
		return(TRUE);
	}

void msg_area_info(void)
	{	int t,old_drive;
		char dum[512],pfad[MAX_STR],area[MAX_STR],addr[MAX_STR],
				 old_path[MAX_STR],*pointer;
		FILE *fbbs;

		line =
		count =
		kbyte =
		msgs =
		deleted =
		fromsysop =
		tosysop = 0;

		wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
		wind.handle=handle;
		wind_set(wind.handle,WF_NAME," AVALON-MSGBASE-INFO ");
		wind_set(wind.handle,WF_CURRXYWH,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);

		wind_update(BEG_UPDATE);
		wind_cls(&wind);
		wind_putch(&wind,27,VT52);
		wind_putch(&wind,'f',VT52);
		wind_mesag(&wind,MU_KEYBD|MU_MESAG|MU_TIMER);

		old_drive = Dgetdrv();
		Dgetpath(old_path,old_drive+1);
		set_defaultdir();

		if ( exist(areas)==0 )
			{	fbbs=fopen( areas ,"r");
				if (fbbs!=NULL)
					{	setvbuf(fbbs,NULL,_IOFBF,PHYS_BUFFER);
						headline();
						input(dum,MAX_STR,fbbs);
						do
							{	input(dum,512,fbbs);
								trimstr(dum);
								if (*dum==EOS || *dum=='-' || *dum==';')
									continue;
								memset(pfad,0,MAX_STR);
								memset(area,0,MAX_STR);
								memset(addr,0,MAX_STR);
								t = sscanf(dum,"%s %s %s",pfad,area,addr);
								switch(t)
									{	case 0	:
										case 1	: form_alert(1,msg[1]);	break;
										case 2	: *addr=EOS;	/* kein break */
										case 3	:	pointer=strchr(addr,'@');
															if (pointer) *pointer=EOS;
															if (!teste_area(pfad,area,addr))
																goto raus;
									}
							}
						while (feof(fbbs)==0);
raus:
						fclose(fbbs);
						wind_print(&wind,"==============================================================================");
						sprintf(dum," %3u areas                 %4u  %4u  %4u  %4u  %4u/%4u",count,kbyte,msgs,deleted,msgs-deleted,tosysop,fromsysop);
						wind_print(&wind,dum);
					}
			}
		Dsetdrv(old_drive);
		Dsetpath(old_path);

		wind_update(END_UPDATE);
		wind_pause(&wind);
		wind_set(handle,WF_NAME,AVALONNAME);
		xform_maindialog(XFMD_DIALOG);
	}
