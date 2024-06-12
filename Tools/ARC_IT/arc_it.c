#include <stdio.h>
#include <fido.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>

#define FLAG(x,y)			if (flag & x) fprintf(bak,y)

#define MF_AVALONBAK  (1U << 14)

#define B_SIZE				65536L
#define ECHOAREA			0
#define NETMAILAREA		1
#define FALSE					0
#define TRUE					1

#define F_RESCAN			(1 << 0)
#define F_ALL					(1 << 1)

/*     PFAD/NAME o EXT, E/N-AREA, txtsysop   , TRUE-all; FALSE-new */
int do_arc(char *fname, int area, char *sysop, int mode, char *fbackup);

void print(FILE *bak, MSG_HEADER *header, FILE *msg, int area);
void convert(char *buffer, unsigned int *length);
void flags(FILE *bak, int flag);
void usage(void);

char *buffer;
char version[]="1.21";

void usage(void)
	{	fprintf(stderr,"ARC_IT V%s   Utility for FidoPoints.   (c) Stephan Slabihoud 1991-1994	\n",version);
		fprintf(stderr,"\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"Usage: arc_it <e|n> [-ax] ""sysop"" msgbase [arcfile]\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"  e - <msgbase> is an echomail area\n");
		fprintf(stderr,"  n - <msgbase> is a netmail area\n");
		fprintf(stderr,"\n");
		fprintf(stderr," -a - rescan messages\n");
		fprintf(stderr," -x - arc all messages\n");
		fprintf(stderr,"\n");
		fprintf(stderr," <arcfile> name of arced msgbase (default is <msgbase>.ABK)\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"This program is *SHAREWARE*. It is published with AVALON.\n");
		getch();
		exit(2);
	}

int main(int argc, char *argv[])
	{	int area, argp=2, mode=0;
		char *pointer;

		if (--argc<3)
			usage();

		if ( (*argv[1] & ~32)=='E' )
			area=ECHOAREA;
		else if ( (*argv[1] & ~32)=='N' )
			area=NETMAILAREA;
		else
			usage();

		if ( *argv[2]=='-' )
			{	argp=3;
				pointer=argv[2]+1;
				while (*pointer!='\0')
					{	switch(*(pointer++) & ~32)
							{	case 'A' : mode |= F_RESCAN; break;
								case 'X' : mode |= F_ALL;		 break;
								default  : usage();
							}
					}
			}
		
		switch ( do_arc(argv[argp+1],area,argv[argp],mode,argv[argp+2]) )
			{	case 1 : fprintf(stderr,"ARC IT: Not enough memory!\n"); break;
				case 2 : fprintf(stderr,"ARC IT: File open error!\n");
				getch();
			}
		return 0;
	}

int do_arc(char *fname, int area, char *sysop, int mode, char *fbackup)
	{	FILE *fido,*msg,*bak;
		MSG_HEADER hdr;
		char fhdr[PATH_MAX],fmsg[PATH_MAX],fbak[PATH_MAX];
		long count=0;

		buffer=(char *) malloc(B_SIZE);
		if (buffer==NULL)
			{	return 1;
			}
		else
			{	strcpy(fhdr,fname);
				strcat(fhdr,".HDR");
				strcpy(fmsg,fname);
				strcat(fmsg,".MSG");
				strcpy(fbak,fname);
				strcat(fbak,".ABK");

				if (fbackup!=NULL)
					strcpy(fbak,fbackup);

				fido=fopen(fhdr,"r+b");
				if (fido!=NULL)
					setvbuf(fido,NULL,_IOFBF,sizeof(MSG_HEADER));
				msg =fopen(fmsg,"rb");
				bak =fopen(fbak,"a");
				if (fido==NULL || msg==NULL || bak==NULL)
					{	return 2;
					}
				else
					{	fprintf(stderr,"ARC IT: processing %s -> %s\n",fname,fbak);
						fseek(fido,0L,SEEK_SET);
						while ( !feof(fido) )
							{	fseek(fido,0L,SEEK_CUR);
								fread(&hdr,sizeof(MSG_HEADER),1L,fido);
								if (feof(fido))
									break;
								if ( !(hdr.wFlags & MF_DELETED))
									if (stricmp(hdr.szFrom,sysop)==0 ||
											stricmp(hdr.szFrom,"sysop")==0 ||
											stricmp(hdr.szTo,sysop)==0 ||
											stricmp(hdr.szTo,"sysop")==0 ||
											(mode & F_ALL) )
											if ( !(hdr.wProcessed.Arced) || (mode & F_RESCAN) )
												{ print(bak,&hdr,msg,area);
													hdr.wProcessed.Arced = TRUE;
													fseek(fido,-sizeof(MSG_HEADER),SEEK_CUR);
													fwrite(&hdr,sizeof(MSG_HEADER),1L,fido);
													count++;
													printf("<%3lu> ARC IT: %-20.20s %-20.20s %.21s\n",count,hdr.szFrom,hdr.szTo,hdr.szSubject);
												}
							}
						fclose(bak);
						fclose(msg);
						fclose(fido);
					}
				free(buffer);
			}
		return 0;
	}

void print(FILE *bak, MSG_HEADER *header, FILE *msg, int area)
	{	char string[21],first[80],second[80];
		unsigned int length;

		memset(first,'-',79);
		memset(second,'=',79);
		first[79]='\0';
		second[79]='\0';

		fprintf(bak,"%s\n",second);
		if (area==NETMAILAREA)
			{	fprintf(bak,"From: %s   %u:%u/%u",header->szFrom
																				 ,header->wFromZone
																				 ,header->wFromNet
																				 ,header->wFromNode);
				(header->wFromPoint) ? fprintf(bak,".%u\n",header->wFromPoint) : fprintf(bak,"\n");
				fprintf(bak,"To  : %s   %u:%u/%u",header->szTo
																				 ,header->wToZone
																				 ,header->wToNet
																				 ,header->wToNode);
				(header->wToPoint) ? fprintf(bak,".%u\n",header->wToPoint) : fprintf(bak,"\n");
			}
		else
			{	fprintf(bak,"From: %s\n",header->szFrom);
				fprintf(bak,"To  : %s\n",header->szTo);
			}
		fprintf(bak,"Subj: %s\n",header->szSubject);
		fprintf(bak,"Size: %u",header->wSize);
		if (header->wFlags)
			{	fprintf(bak,"    Flags: ");
				flags(bak, header->wFlags);
				if (header->wProcessed.Arced)
					fprintf(bak,"Arced");
			}
		fprintf(bak,"\n");

		strftime(string, 20, "%d %b %y %X",gmtime(&header->lDate) );
		fprintf(bak,"Written: %-20s    Imported: %-20s\n",header->szDate,string);

		fprintf(bak,"%s\n",first);

		fseek(msg,header->lStart,SEEK_SET);
		length=header->wSize;
		fread(buffer,length,1,msg);
		convert(buffer,&length);
		fwrite(buffer,(long) length-1,1,bak);
		fprintf(bak,"\n");
	}

void convert(char *buffer, unsigned int *length)
	{	unsigned int k,j;
		char *src,*dst;
		for (k=0; k<=(*length)-1; k++)
			{	if (buffer[k]==1)
					{	for (j=k+1; j<=(*length)-1; j++)
							{	if (buffer[j]==10 || buffer[j]==0)
									{	src=&buffer[k];
										dst=&buffer[j]+1;
										while (dst<=(&buffer[0]+ *length)) *src++=*dst++;
										*length -= (int) (&buffer[j]-&buffer[k]+1);
										k=-1;
										break;
									}
							}
					}
			}
	}

void flags(FILE *bak, int flag)
	{	FLAG(MF_PRIVATE   ,"Pvt ");
		FLAG(MF_CRASH     ,"Crash ");
		FLAG(MF_RECEIVED  ,"Recd ");
		FLAG(MF_SENT      ,"Sent ");
		FLAG(MF_WITHFILE  ,"W/File ");
		FLAG(MF_FORWARDED ,"Forwd ");
		FLAG(MF_ORPHAN    ,"Orphan ");
		FLAG(MF_KILLIFSENT,"K/Sent ");
		FLAG(MF_LOCAL     ,"Local ");
		FLAG(MF_HOLD      ,"Hold ");
		FLAG(MF_SIGNATURE ,"Signature ");
		FLAG(MF_DELETED   ,"Deleted ");
/*	FLAG(MF_AVALONBAK ,"AvalonArced"); */
	}

/*		long	lUnused1; */
/*		int		wUnused[8]; 		die ersten beiden Worte werden von ECU	*/
/*													und Llegada zur Dupe-Erkennung benutzt	*/
