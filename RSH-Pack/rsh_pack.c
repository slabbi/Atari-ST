/********************************************************************/
/*																																	*/
/*		RSH-PACK V1.1  -  (c) Stephan Slabihoud fÅr MAXON-Computer		*/
/*																																	*/
/*    Entwickelt mit Pure-C																					*/
/********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ext.h>
#include <tos.h>

#define MAX_LINES 10000
#define FALSE 0
#define TRUE 1

char *lines[MAX_LINES],*dupes[MAX_LINES];
char stemplate[9]="X9AaNnFPp";
int	 itemplate[9]={0,0,0,0,0,0,0,0,0};
int	 ilength[9]  ={0,0,0,0,0,0,0,0,0};

void header(void)
	{	printf("\n RSH_PACK V1.1                                    (c) 1992 Stephan Slabihoud\n");
		printf("                                                           Gustav-Ohm-Str.72\n");
		printf("                                                             4250  Bottrop\n\n");
	}

void usage(void)
	{	header();
		printf(" Usage: RSH_PACK file\n\n");
		printf("        file - full path and name of Interface-RSH (e.g. E:\\TEST.RSH)\n\n");
		getch();
		exit(1);
	}

int do_it(char *filename)
	{	FILE *ffile;
		char *buffer,*empty,*pointer,temp[256];
		long length,t,p,counter=0,start=0,spare=0,dup=-1;
		int	 len,change,xnull=FALSE;

		printf("\n Loading: %s ... ",filename);
		ffile=fopen(filename,"rb");
		if (ffile==NULL)
			{	printf("ERROR\n");
				return(1);
			}
		else
			{	fseek(ffile,0,SEEK_END);
				length=ftell(ffile);
				fseek(ffile,0,SEEK_SET);
				buffer=(char *) Malloc(2*length);
				empty =buffer+length+1024;
				if (fread(buffer,1,length,ffile)==length)
					{	printf("OK - %lu Bytes\n",length);
					}
				else
					{	printf("ERROR\n");
						Mfree(&buffer);
						return(2);
					}
				fclose(ffile);

				lines[0]=&buffer[0];
				for (t=0; t<length; t++)
					{	if (buffer[t]=='\r')
							{	buffer[t]='\0';
								buffer[t+1]='\0';
								lines[++counter]=&buffer[t+2];
							}
					}
				buffer[length]='\0';

				for (t=0; t<=counter; t++)
					{	if ( strncmp(lines[t],"TEDINFO rs_tedinfo[]",20)==0 )
							{	start=t;
								break;
							}
					}

				if (start==0)
					{	Mfree(&buffer);
						return(3);
					}
				else
					{	printf("\n Searching TEDINFO template: ");
						for (p=0; p<=8; p++)
							{	printf("%c",stemplate[p]);
								for (t=start+3; t<=counter; t+=4)
									{	if (strncmp(lines[t-2],"};",2)==0) break;
										sprintf(temp,"  \"%c",stemplate[p]);
										if (strncmp(lines[t],temp,4)==0)
											{	len=(int) strlen(lines[t])-5;
												memset(temp,'\0',255);
												memset(temp,stemplate[p],len);
												if (strncmp(lines[t]+3,temp,len)==0)
													{	spare+=len;
														ilength[p]= (ilength[p]>len) ? ilength[p] : len;
														sprintf(empty,"  TEMP%c,",stemplate[p]);
														lines[t] = &empty[0];
														empty+=9;
														itemplate[p]=TRUE;
													}
											}
									}
							}
						printf("\n");

						printf(" Searching NULL strings\n");
						for (t=start+1; t<=counter; t+=4)
							{	if (strncmp(lines[t],"};",2)==0) break;
								for (p=t; p<=t+3; p++)
									{	if (strncmp(lines[p],"  \"\",",5)==0)
											{	strcpy(empty,"  XNULL,");
												lines[p] = &empty[0];
												empty+=9;
												xnull=TRUE;
												spare++;
											}
										if (strncmp(lines[p],"  \"\\0\",",7)==0)
											{	strcpy(empty,"  XNULL,");
												lines[p] = &empty[0];
												empty+=9;
												xnull=TRUE;
												spare++;
											}
									}
							}

						printf(" Searching dupes\n");
						for (t=start+2; t<=counter; t+=4)
							{	if (strncmp(lines[t-1],"};",2)==0) break;
								if (strncmp(lines[t],"  \"",3)==0)
									{	dupes[++dup]=lines[t];
									}
							}
						do
							{	change=FALSE;
								for (t=0; t<dup; t++)
									if (strcmp(dupes[t],dupes[t+1])>0)
										{	pointer=dupes[t];
											dupes[t]=dupes[t+1];
											dupes[t+1]=pointer;
											change=TRUE;
										}
							}
						while (change==TRUE);
						for (t=0; t<dup; t++)
							{	if (strcmp(dupes[t],dupes[t+1])!=0)
									{	for (p=t; p<dup; p++)
											dupes[p]=dupes[p+1];
										dupes[dup]=NULL;
										dup--;
										t--;
									}
							}
						t=0;
						do
							{	if (strcmp(dupes[t],dupes[t+1])==0)
									{	for (p=t; p<dup; p++)
											dupes[p]=dupes[p+1];
										dupes[dup]=NULL;
										dup--;
									}
								else
									{	t++;
									}
							}
						while (t<dup);
						for (p=0; p<=dup; p++)
							{	for (t=start+2; t<=counter; t+=4)
									{	if (strncmp(lines[t-2],"};",2)==0) break;
										if (strcmp(lines[t],dupes[p])==0)
											{	len=(int) strlen(lines[t])-5;
												spare+=len;
												sprintf(empty,"  XTEMP%lu,",p);
												lines[t] = &empty[0];
												empty+=strlen(empty)+1;
											}
									}
							}
						for (p=0; p<=8; p++)
							spare-=ilength[p];

						printf("\n RSH can be compressed ~%lu bytes!\n\n",spare);
						if (xnull==TRUE)
							{	for (t=counter; t>=start; t--)
									lines[t+1]=lines[t];
								strcpy(empty,"char XNULL[]=\"\";");
								lines[start] = &empty[0];
								empty+=17;
								counter++;
							}
						for (p=0; p<=8; p++)
							{	if (itemplate[p]==TRUE)
									{	for (t=counter; t>=start; t--)
											lines[t+1]=lines[t];
										memset(temp,'\0',255);
										memset(temp,stemplate[p],ilength[p]);
										sprintf(empty,"char TEMP%c[]=\"%s\";",stemplate[p],temp);
										lines[start] = &empty[0];
										empty += 17+ilength[p];
										counter++;
									}
							}
						for (p=0; p<=dup; p++)
							{	for (t=counter; t>=start; t--)
									lines[t+1]=lines[t];
								memset(temp,'\0',255);
								strncpy(temp,dupes[p]+2,strlen(dupes[p])-3);
								sprintf(empty,"char XTEMP%lu[]=%s;",p,temp);
								lines[start] = &empty[0];
								empty += strlen(empty)+1;
								counter++;
							}
						printf(" Save compressed RSH (y/*)? ");
						if ( (getch() | 0x20)=='y' )
							{	printf("Yes\n");
								filename[strlen(filename)-1]='P';
								printf("\n Saving: %s\n",filename);
								ffile=fopen(filename,"w");
								for (t=0; t<=counter; t++)
									fprintf(ffile,"%s\n",lines[t]);
								fclose(ffile);
							}
						else
							{	printf("No\n");
							}
					}
				Mfree(&buffer);
				return(0);
			}
	}

int main(int argc, char *argv[])
	{	char filename[PATH_MAX];
		if (argc!=2)
			usage();
		strcpy(filename,argv[1]);		
		strupr(filename);
		if (Fsfirst(filename,0)==0)
			{	switch(do_it(filename))
					{	case 1	: printf(" ERROR: File open error!\n"); break;
						case 2	: printf(" ERROR: File read error!\n"); break;
						case 3	: printf(" ERROR: TEDINFO not found. Cannot compress!\n"); break;
					}
			}
		else
			{	header();
				printf(" ERROR: File %s not found!\n",filename);
				getch();
			}
		return(0);
	}
