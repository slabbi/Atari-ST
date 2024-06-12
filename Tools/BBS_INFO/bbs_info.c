#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>
#include <ext.h>
#include <fido.h>

#define MAX_STR			255

char		a_name[61],
				a_path[61];
char		areas[PATH_MAX]="AREAS.BBS",
				sysop[PATH_MAX]="";
int			a_days,
				a_boss[3],
				kbyte=0,msgs=0,deleted=0,fromsysop=0,tosysop=0,lensysop=0;

void head(void)
	{	fprintf(stderr," BBS_INFO V1.30       This program is shareware. It is published with AVALON. \n");
	}

int exist(char *filename)
	{	return Fsfirst(filename,0);										 /* 0 = gefunden */
	}

void input(char *string, int max, FILE *ftype)
	{	long len;
		string[0]='\0';
		fgets(string, max, ftype);
		len = strlen(string);
		if (len>0)
			string[len-1]='\0';
	}

void trim(char **dest)
	{	char *pointer;
		while (**dest==' ' && *((*dest)++)!='\0');
		pointer=*dest;
		while (*pointer!='\0') pointer++;
		while (*(--pointer)==' ' && pointer>*dest) *pointer = '\0';
	}

void reset_entry(void)
	{	a_boss[0]=
		a_boss[1]=
		a_boss[2]=0;
		*a_name=
		*a_path='\0';
	}

void teste_area(void)
	{	FILE *fbbs;
		char filename[PATH_MAX],
				 dummy1[10],
				 dummy2[10],
				 dummy3[10],
				 dummy4[10],
				 dummy5[10],
				 dummy6[10];
		MSG_HEADER hdr;
		int	c_msgs=0,c_deleted=0,c_kbyte,c_fromsysop=0,c_tosysop=0;

		strcpy(filename,a_path);
		strcat(filename,".MSG");

		fbbs=fopen(filename,"rb");
/*	fseek(fbbs,0,SEEK_END);
		c_kbyte=(int) (ftell(fbbs) >> 10);
		fseek(fbbs,0,SEEK_SET); */
		c_kbyte=(int) (filelength(fbbs->Handle) >> 10);
		fclose(fbbs);

		strcpy(filename,a_path);
		strcat(filename,".HDR");
		fbbs=fopen(filename,"rb");
/*	fseek(fbbs,0,SEEK_END);
		c_kbyte+=(int) (ftell(fbbs) >> 10);
		kbyte += c_kbyte;
		fseek(fbbs,0,SEEK_SET); */
		c_kbyte +=(int) (filelength(fbbs->Handle) >> 10);
		kbyte += c_kbyte;

		while ( !feof(fbbs) )
			{	fread(&hdr,sizeof(MSG_HEADER),1,fbbs);
				if (feof(fbbs)) break;
				c_msgs++;
				if (hdr.wFlags & MF_DELETED)
					{	c_deleted++;
					}
				else
					{	if ( (strnicmp(hdr.szFrom,"sysop",5)==0) ||
								 (strnicmp(hdr.szFrom,sysop,lensysop)==0) )
							{	c_fromsysop++;
								fromsysop++;
							}
						if ( (strnicmp(hdr.szTo,"sysop",5)==0) ||
								 (strnicmp(hdr.szTo,sysop,lensysop)==0) )
							{	c_tosysop++;
								tosysop++;
							}
					}
			}
		fclose(fbbs);

		msgs += c_msgs;
		deleted += c_deleted;

		if ((a_boss[2]==0) && (a_boss[0]==0) && (a_boss[1]==0))
			{	*filename='\0';
			}
		else
			{	if (a_boss[2]==0)
					sprintf(filename,"%u/%u",a_boss[0],a_boss[1]);
				else
					sprintf(filename,"%u:%u/%u",a_boss[2],a_boss[0],a_boss[1]);
			}
		fprintf(stdout," %-20s  %-12s  %4s  %4s  %4s  %4s    %3u  %4s/%4s\n",a_name,filename,
																																						c_kbyte==0 ? "-" : itoa(c_kbyte,dummy1,10),
																																						c_msgs==0 ? "-" : itoa(c_msgs,dummy2,10),
																																						c_deleted==0 ? "" : itoa(c_deleted,dummy3,10),
																																						c_msgs-c_deleted==0 ? "-" : itoa(c_msgs-c_deleted,dummy4,10),
																																						a_days,
																																						c_tosysop==0 ? "-" : itoa(c_tosysop,dummy5,10),
																																						c_fromsysop==0 ? "-" : itoa(c_fromsysop,dummy6,10));
	}

void edit_areas(void)
	{	int areas_cnt=0;
		char dum[MAX_STR],dum2[MAX_STR+1],*pointer;
		FILE *fbbs;
		
		if ( exist( areas )==0 )
			{	fbbs=fopen( areas ,"r");
				if (fbbs==NULL)
					{	fprintf(stderr,"\n BBS-INFO: Cannot open AREAS.BBS.\n");
					}
				else
					{	input(dum,MAX_STR,fbbs);

						head();
						fprintf(stdout,"============================================================================== \n");
						fprintf(stdout," Name                  Boss            KB  Msgs   Del  Left   Days    To/From  \n");
						fprintf(stdout,"------------------------------------------------------------------------------ \n");
						reset_entry();

						do
							{	input(dum,MAX_STR,fbbs);
								pointer=dum;
								trim(&pointer);
								if ( strlen(pointer)!=0 )
									{	
										if ( strnicmp(dum,"-DAYS ",6)==0 )
											{	pointer +=6;
												trim(&pointer);
												a_days=atoi(pointer);
												continue;
											}

										if (*dum=='-')
											continue;

										if (*dum==';')
											continue;

										switch( sscanf(dum,"%s %s %s",a_path,a_name,dum2) )
											{	case 0 :
												case 1 : fprintf(stderr,"\n BBS-INFO: Error in AREAS.BBS.\n");
																 break;
												case 2 : a_boss[0]=
																 a_boss[1]=
																 a_boss[2]=0;	break;
												case 3 : {	if ( strchr(dum2,':')==NULL )
																			sscanf(dum2,"%u/%u",&a_boss[0],&a_boss[1]);
																		else
																			sscanf(dum2,"%u:%u/%u",&a_boss[2],&a_boss[0],&a_boss[1]);
																	}
											}
										areas_cnt++;
										teste_area();
										reset_entry();
									}
							}
						while (feof(fbbs)==0);
						fclose(fbbs);

						fprintf(stdout,"------------------------------------------------------------------------------ \n");
						fprintf(stdout," %3u areas                           %4u  %4u  %4u  %4u         %4u/%4u\n",areas_cnt,kbyte,msgs,deleted,msgs-deleted,tosysop,fromsysop);
						fprintf(stdout,"============================================================================== \n");
					}
			}
		else
			{	fprintf(stderr,"\n BBS-INFO: AREAS.BBS not found. File: %s\n",areas);
			}
	}

int main(int argc,char *argv[])
	{	if ((argc==2) || (argc==3))
			{	strcpy(sysop,argv[1]);
				lensysop=(int) strlen(sysop);
				if (argc==2)
					{	strcpy(areas,"AREAS.BBS");
					}
				else
					{	strcpy(areas,argv[2]);
					}
				edit_areas();
			}
		else
			{	head();
				fprintf(stdout,"============================================================================== \n\n");
				fprintf(stderr," Usage: BBS-INFO sysop [file] [>[>]logfile]\n\n");
				fprintf(stderr,"        sysop   -  name of sysop \n");
				fprintf(stderr,"        file    -  path and filename from 'AREAS.BBS' (default: AREAS.BBS) \n");
				fprintf(stderr,"        logfile -  logfile (optional) \n");

				getch();
			}
		return 0;
	}
