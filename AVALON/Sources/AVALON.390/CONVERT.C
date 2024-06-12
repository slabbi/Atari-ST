#include <stdio.h>
#include <fido.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include <tos.h>
#include <ctype.h>
#include <fido.h>
#include "define.h"
#include "version.h"
#include "avalonio.h"
#include "avallang.h"
#include "windmini.h"
#include "convert.h"
#include "diverse.h"
#include "vars.h"

#define X_FILE					 1
#define X_SUBJ					 2
#define X_AREA					 4
#define X_FLAG					 8
#define X_FROM					16
#define X_CONVERT			 512
#define X_CONVERTTEAR	1024

#define ECHO	if (echo)

static char msgfile[PATH_MAX],
						msgbase[PATH_MAX],
						msgsubj[MAX_STR];

void msg_convert(char *txt)
	{	if ( strnicmp(txt,"ON",2)==0 )
			msg_flag |= X_CONVERT;
		else
			msg_flag &= ~X_CONVERT;
	}

void msg_converttear(char *txt)
	{	if ( strnicmp(txt,"ON",2)==0 )
			msg_flag |= X_CONVERTTEAR;
		else
			msg_flag &= ~X_CONVERTTEAR;
	}

int msg_base(char *base)
	{	if ( exist(base)==0 )
			{	strcpy(msgbase,base);
				msg_flag |= X_AREA;
				return TRUE;
			}
		else
			return FALSE;
	}

void msg_area(char *areaname)
	{	int t,old_drive;
		char dum[512],pfad[512],area[512],old_path[MAX_STR];
		FILE *fbbs;

		old_drive = Dgetdrv();
		Dgetpath(old_path,old_drive+1);
		set_defaultdir();

		msg_flag &= ~X_AREA;

		if ( exist(areas)==0 )
			{	fbbs=fopen( areas ,"r");
				if (fbbs!=NULL)
					{	setvbuf(fbbs,NULL,_IOFBF,PHYS_BUFFER);
						input(dum,MAX_STR,fbbs);
						do
							{	input(dum,512,fbbs);
								trimstr(dum);
								if (*dum==EOS || *dum=='-' || *dum==';')
									continue;
								memset(pfad,0,512);
								memset(area,0,512);
								t = sscanf(dum,"%s %s",pfad,area);
								if (t==2)
									{	if (stricmp(area,areaname)==0)
											{	strcpy(msgbase,pfad);
												msg_flag |= X_AREA;
												break;
											}
									}
							}
						while (feof(fbbs)==0);
						fclose(fbbs);
					}
			}
		Dsetdrv(old_drive);
		Dsetpath(old_path);
	}

void msg_filename(char *filename)
	{	strcpy(msgfile,filename);
		msg_flag |= X_FILE;
	}

int msg_fileselect(void)
	{	char in_path[PATH_MAX], sel_file[13]="";
		int ret=2;
		if ( fileselect(in_path,sel_file,"*.TXT",avaltxt[12]) != 0)
			{	strcat(in_path,sel_file);
				ret=1;
				if (exist(in_path)==0)
					{	strcpy(msgfile,in_path);
						msg_flag |= X_FILE;
						ret=0;
					}
			}
		return(ret);
	}

void msg_subject(char *subject)
	{	strcpy(msgsubj,subject);
		msg_flag |= X_SUBJ;
	}

void msg_from(char *txt)
	{	int zone,net,node,point;
		char temp[MAX_STR],*pointer;
		pointer = strtok(txt," ");
		if (pointer!=NULL)
			{	scanaddr(pointer,&zone,&net,&node,&point,temp);
				header.wFromZone = zone;
				header.wFromNet  = net;
				header.wFromNode = node;
				header.wFromPoint= point;
			}
		pointer = strtok(NULL,"");
		if (pointer!=NULL)
			{	trim(&pointer);
				strncpy(header.szFrom,pointer,35);
				header.szFrom[35] = EOS;
			}
		else
			*header.szFrom = EOS;
		msg_flag |= X_FROM;
	}

void msg_to(char *txt)
	{	int zone,net,node,point;
		char temp[MAX_STR],*pointer;
		pointer = strtok(txt," ");
		if (pointer!=NULL)
			{	scanaddr(pointer,&zone,&net,&node,&point,temp);
				header.wToZone = zone;
				header.wToNet  = net;
				header.wToNode = node;
				header.wToPoint= point;
			}
		pointer = strtok(NULL,"");
		if (pointer!=NULL)
			{	trim(&pointer);
				strncpy(header.szTo,pointer,35);
				header.szTo[35] = EOS;
			}
		else
			*header.szTo = EOS;
	}

void msg_flags(char *flags)
	{	char *pointer;
		pointer = flags;
		header.wFlags = MF_LOCAL;
		while (*pointer!=EOS)
			{	switch(toupper(*pointer))
					{	case 'P'	:	header.wFlags |= MF_PRIVATE;		break;
						case 'C'	:	header.wFlags |= MF_CRASH;			break;
						case 'S'	:	header.wFlags |= MF_SENT;				break;
						case 'W'	:	header.wFlags |= MF_WITHFILE;		break;
						case 'K'	:	header.wFlags |= MF_KILLIFSENT;	break;
						case 'H'	:	header.wFlags |= MF_HOLD;				break;
					}
				pointer++;
			}
		msg_flag |= X_FLAG;
	}

int msg_send(int echo)
	{	FILE *hdr,*msg,*in;
		unsigned long minipuffer;
		char *pointer,*pointer2,temp[PATH_MAX],temp2[MAX_STR],dum[21],
				 subjlocal[MAX_STR],fromaddress[40],toaddress[40],
				 fin_path[PATH_MAX],fhdr[PATH_MAX],fmsg[PATH_MAX];
		signed int error;
		long length;
		int i;

		if (!(msg_flag & X_FILE))
			return 1;
		if (!(msg_flag & X_AREA))
			return 4;
		if (!(msg_flag & X_FROM))
			return 6;
		if (*header.szTo==EOS)
			return 7;
		if (!(msg_flag & X_SUBJ))
			return 8;
		if (!(msg_flag & X_FLAG))
			return 9;

		mergeaddr(fromaddress,header.wFromZone,
													header.wFromNet,
													header.wFromNode,
													header.wFromPoint,"");
		mergeaddr(toaddress,header.wToZone,
												header.wToNet,
												header.wToNode,
												header.wToPoint,"");

		ECHO	{	sprintf(temp2,tbmsg[3],header.szTo,toaddress);
						WPRINT(temp2);
					}

		strcpy(temp,msgfile);
		pointer=strchr(temp,EOS);
		while ( (*pointer!='\\') && (pointer>temp) ) pointer--;
		if (*pointer=='\\')
			{	*pointer='\0';
				strcpy(fin_path,temp);
				strcat(fin_path,"\\");
				*pointer='\\';
			}
		else
			{	*fin_path = EOS;
			}

		error=xFsfirst(temp,0);
				
		if (error<0)
			{	return 3;
			}
		else
			{	while (error>=0)
					{	strcpy(fhdr,msgbase);	strcat(fhdr,".HDR");
						strcpy(fmsg,msgbase);	strcat(fmsg,".MSG");
				
						strcpy(temp,fin_path);
						strcat(temp,dta_buffer.d_fname);
				
						hdr=fopen(fhdr,"ab");
						if (hdr!=NULL)
							setvbuf(hdr,NULL,_IOFBF,10*sizeof(MSG_HEADER));
						msg=fopen(fmsg,"ab");
						in =fopen(temp,"rb");
				
						if (hdr==NULL || msg==NULL || in==NULL)
							{	return 2;
							}
						else
							{	ECHO	{	sprintf(temp2," - %s",temp);
												WPRINT(temp2);
											}

/* "temp" enth„lt Dateinamen mit Pfad, "temp2" nur den Dateinamen */

								strcpy(temp2,temp);						/* Pfad abschneiden */
								pointer=strrchr(temp2,'\\');
								if (pointer!=NULL)
									strcpy(temp2,pointer+1);

								strcpy(subjlocal,msgsubj);
								i=TRUE;
								
								while(i==TRUE)
									{	i=FALSE;
										if ((pointer=strstr(subjlocal,"%file%"))!=NULL)
											{	*pointer = EOS;
												sprintf(temp,"%s%s%s",subjlocal,temp2,pointer+6);
												strcpy(subjlocal,temp);
												i=TRUE;
											}
										if ((pointer=strstr(subjlocal,"%filename%"))!=NULL)
											{	*pointer = EOS;
												strncpy(dum,temp2,20); dum[20]=EOS;
												pointer2 = strrchr(dum,'.');
												if (pointer2!=NULL)
													*pointer2=EOS;
												sprintf(temp,"%s%s%s",subjlocal,dum,pointer+10);
												strcpy(subjlocal,temp);
												i=TRUE;
											}
										if ((pointer=strstr(subjlocal,"%ext%"))!=NULL)
											{	*pointer = EOS;
												pointer2 = strrchr(temp2+strlen(temp2)-4,'.');
												if (pointer2!=NULL)
													sprintf(temp,"%s%u%s",subjlocal,atoi(pointer2+1),pointer+5);
												else
													sprintf(temp,"%s0%s",subjlocal,pointer+5);
												strcpy(subjlocal,temp);
												i=TRUE;
											}
									}
								strncpy(header.szSubject,subjlocal,71);
								header.szSubject[71] = EOS;

								time(&header.lDate);
								strftime(header.szDate,20,"%d %b %y %X",localtime(&header.lDate) );
								/*				WrittenTime														Imported time */
				
								length=length_of_file(in,TRUE);
								if (length==0)
									{	fclose(in);
										fclose(msg);
										fclose(hdr);
										return 5;
									}

								header.lStart= ftell(msg);
								header.wSize= 0;
								write_ids(&header,msg,fromaddress);
								
								minipuffer=0;
								while (length>0)
									{	i = getc(in);
										if (msg_flag & X_CONVERTTEAR)
											{	minipuffer = minipuffer << 8;
												minipuffer |= i;
												if (minipuffer=='\n---' ||
														minipuffer=='\r---' ||
														minipuffer==  '---')
													i='!';
											}
										if ( i!='\r' && i!=EOS && i!=EOF)
											{	if (msg_flag & X_CONVERT)
													{	switch(i)
															{ case 'Ž'	:	putc('A',msg);
																						putc('e',msg);
																						header.wSize++;
																						break;
																case '™'	:	putc('O',msg);
																						putc('e',msg);
																						header.wSize++;
																						break;
																case 'š'	: putc('U',msg);
																						putc('e',msg);
																						header.wSize++;
																						break;
																case '„'	: putc('a',msg);
																						putc('e',msg);
																						header.wSize++;
																						break;
																case '”'	: putc('o',msg);
																						putc('e',msg);
																						header.wSize++;
																						break;
																case ''	: putc('u',msg);
																						putc('e',msg);
																						header.wSize++;
																						break;
																case 'ž'	: putc('s',msg);
																						putc('s',msg);
																						header.wSize++;
																						break;
																default		:	putc( i ,msg);
															}
														header.wSize++;
													}
												else
													{	putc(i,msg);
														header.wSize++;
													}
											}
										length--;
									}
								sprintf(temp2,"\n\n+++ Created by AVALON\n");
								fputs(temp2,msg);
								putc(EOS,msg);
								header.wSize += (int) strlen(temp2)+1;

								delete_unused_flags(&header);

								fwrite(&header,sizeof(MSG_HEADER),1,hdr);

								fclose(in);
								fclose(msg);
								fclose(hdr);
							}
						error=xFsnext();
					}
			}
		return 0;
	}
