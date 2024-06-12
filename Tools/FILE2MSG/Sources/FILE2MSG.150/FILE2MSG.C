#include <stdio.h>
#include <fido.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include <tos.h>

#define FALSE					0
#define TRUE					1
#define B_SIZE				65536L
#define MAX_STR				256

int do_file2msg(int args,char *infile,char *msgbase,char *address,char *from,char *destadr,char *to,char *subject,int mode);
void usage(void);
int convert(char *buffer,long *length);

DTA		*dta_buffer;
char	*buffer,
			version[]="1.50",
			tearline[]="\n--- File2Msg V1.50\n\0";

void usage(void)
	{	fprintf(stderr,"FILE2MSG V%s   Utility for FidoPoints.   (c) Stephan Slabihoud 1993-1994\n",version);
		fprintf(stderr,"\n\n");
		fprintf(stderr,"Usage:\n");
		fprintf(stderr,"file2msg <file> <msgbase> <addr> [<from> <dest-addr> <dest> <subject> <mode>]\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"     <file> - file to convert (e.g. E:\\FIDO\\INBOUND\\*.RSP)\n");
		fprintf(stderr,"  <msgbase> - message base without extension (e.g. E:\\FIDO\\MSGS\\SYSTEM)\n");
		fprintf(stderr,"     <addr> - your 4d-fido-address (e.g. 2:2401/103.6)\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"Optional:\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"     <from> - your name (default: File2Msg V%s)\n",version);
		fprintf(stderr,"<dest-addr> - destination address (default: your address)\n");
		fprintf(stderr,"       <to> - 4d-fido-address of destination (default: Sysop)\n");
		fprintf(stderr,"  <subject> - subject of message (default: <path+file>)\n");
		fprintf(stderr,"                                 (*filename* = use filename as subject)\n");
		fprintf(stderr,"     <mode> - 0=Sent+Local+Private, 1=Local, 2=Local+Pvt, 3=Local+Pvt+K/Sent\n");
		fprintf(stderr,"              4=Local+Pvt+Crash   , 5=Local+Pvt+Crash+K/Sent\n");
		fprintf(stderr,"\n");
		fprintf(stderr,"This program is *SHAREWARE*. It is published with AVALON.\n");
		getch();
		exit(2);
	}

int main(int argc, char *argv[])
	{	char xinfile[MAX_STR],xmsgbase[MAX_STR],xaddress[MAX_STR],
				 xfrom[MAX_STR],xdestadr[MAX_STR],xto[MAX_STR],xsubject[MAX_STR];
		int xmode,i;
	
		dta_buffer = (DTA *) malloc(sizeof(DTA));
		Fsetdta(dta_buffer);

		argc--;
		if ( (argc!=3) && (argc!=8) )
			{	if (argc>0)
					{	fprintf(stderr,"\nFILE2MSG: Error in commandline...\n\nCommandlist:\n\n");
						for (i=1; i<=argc; i++)
							{	switch(i)
									{	case	1	: fprintf(stderr,"        File: ");	break;
										case	2	: fprintf(stderr,"     MsgBase: ");	break;
										case	3	:	fprintf(stderr,"From address: ");	break;
										case	4	: fprintf(stderr,"        From: ");	break;
										case	5	: fprintf(stderr,"Dest address: ");	break;
										case	6	: fprintf(stderr,"          To: ");	break;
										case	7	: fprintf(stderr,"     Subject: ");	break;
										case	8	: fprintf(stderr,"        Mode: ");	break;
										default	:	fprintf(stderr,"     Unknown: ");
									}
								fprintf(stderr,"%s\n",argv[i]);
							}
						fprintf(stderr,"\n\nPress any key to continue...\n");
						getch();
					}
				usage();
			}

		strcpy(xinfile,argv[1]);
		strcpy(xmsgbase,argv[2]);
		strcpy(xaddress,argv[3]);
		if (argc==3)
			{	*xfrom=*xdestadr=*xto=*xsubject='\0';
				xmode=0;
			}
		else
			{	strcpy(xfrom,argv[4]);
				strcpy(xdestadr,argv[5]);
				strcpy(xto,argv[6]);
				strcpy(xsubject,argv[7]);
				xmode=atoi(argv[8]);
			}

		switch ( do_file2msg(argc,xinfile,xmsgbase,xaddress,xfrom,xdestadr,xto,xsubject,xmode) )
			{	case 1 : fprintf(stderr,"FILE2MSG: Not enough memory!\n"); break;
				case 2 : fprintf(stderr,"FILE2MSG: File open error!\n"); break;
				case 3 : fprintf(stderr,"FILE2MSG: File(s) not found %s\n",xinfile); break;
				case 4 : fprintf(stderr,"FILE2MSG: File to big!\n"); break;
				case 5 : fprintf(stderr,"FILE2MSG: Cannot convert NULL-length files!\n");
			}
		return 0;
	}

int do_file2msg(int args,char *infile,char *msgbase,char *address,char *from,char *destadr,char *to,char *subject,int mode)
	{	static FILE *hdr,*msg,*in;
		static MSG_HEADER header;
		static char fhdr[PATH_MAX],fmsg[PATH_MAX],fin[PATH_MAX];
		static char temp[PATH_MAX],temp2[40],*pointer;
		long length;
		int x1,x2,x3,x4;
		signed int error;
  	struct tm *tm_now;					/* Y2K */

		buffer=(char *) malloc(B_SIZE);
		if (buffer==NULL)
			{	return 1;
			}
		else
			{	strcpy(temp,infile);

				pointer=temp;
				while (*pointer!='\0') pointer++;
				if (pointer>temp) pointer--;
				while ( (*pointer!='\\') && (pointer>temp) ) pointer--;
				if (pointer!=temp)
					{	*pointer='\0';
						strcpy(fin,temp);
						strcat(fin,"\\");
						*pointer='\\';
					}
				else
					{	*fin='\0';
					}
				error=Fsfirst(temp,0);
				
				if (error<0)
					{	return 3;
					}
				else
					{	while (error>=0)
							{	strcpy(fhdr,msgbase);
								strcat(fhdr,".HDR");
				
								strcpy(fmsg,msgbase);
								strcat(fmsg,".MSG");
				
								strcpy(temp,fin);
								strcat(temp,dta_buffer->d_fname);
				
								hdr=fopen(fhdr,"ab");
								if (hdr!=NULL)
									setvbuf(hdr,NULL,_IOFBF,sizeof(MSG_HEADER));
								msg=fopen(fmsg,"ab");
								in =fopen(temp,"rb");
				
								if (hdr==NULL || msg==NULL || in==NULL)
									{	return 2;
									}
								else
									{	fprintf(stderr,"FILE2MSG: processing %s",temp);
										
										switch(args)
											{	case 3 	:	{	strcpy(header.szFrom,   "File2Msg V");
																		strcat(header.szFrom,version);
																		strcpy(header.szTo     ,"Sysop");
																		strncpy(header.szSubject,temp,35);
																		sscanf(address,"%u:%u/%u.%u",&x1,&x2,&x3,&x4);
																		header.wFromZone = x1;
																		header.wFromNet  = x2;
																		header.wFromNode = x3;
																		header.wFromPoint= x4;
																		header.wToZone = x1;
																		header.wToNet  = x2;
																		header.wToNode = x3;
																		header.wToPoint= x4;
																	} break;
												case 8	:	{	strcpy(header.szFrom,from);
																		strcpy(header.szTo,to);
																		if (stricmp(subject,"*filename*")==0)
																			{	strcpy(temp2,temp);
																				pointer=strrchr(temp2,'\\');
																				if (pointer!=NULL)
																					strcpy(temp2,pointer+1);
																				strcpy(header.szSubject,temp2);
																			}
																		else
																			{	strcpy(header.szSubject,subject);
																			}
																		if (sscanf(address,"%u:%u/%u.%u",&x1,&x2,&x3,&x4)==3)	x4=0;
																		header.wFromZone = x1;
																		header.wFromNet  = x2;
																		header.wFromNode = x3;
																		header.wFromPoint= x4;
																		if (sscanf(destadr,"%u:%u/%u.%u",&x1,&x2,&x3,&x4)==3)	x4=0;
																		header.wToZone = x1;
																		header.wToNet  = x2;
																		header.wToNode = x3;
																		header.wToPoint= x4;
																	}
											}
										if (header.wToPoint==0)
											fprintf(stderr,"    (To: %-21s %u:%u/%u)\n",header.szTo,
																																	 header.wToZone,
																																	 header.wToNet,
																																	 header.wToNode);
										else
											fprintf(stderr,"    (To: %-21s %u:%u/%u.%u)\n",header.szTo,
																																			header.wToZone,
																																			header.wToNet,
																																			header.wToNode,
																																			header.wToPoint);

										time(&header.lDate);
	
        						tm_now=localtime(&header.lDate);		/* Y2K */
          					tm_now->tm_year %= 100;										/* Y2K */

									strftime(header.szDate,20,"%d %b %y %X",tm_now );
										/*				WrittenTime														Imported time */
				
										switch(mode)
											{	case 1	:	header.wFlags= MF_LOCAL; break;
												case 2	:	header.wFlags= MF_LOCAL | MF_PRIVATE; break;
												case 3	: header.wFlags= MF_LOCAL | MF_PRIVATE | MF_KILLIFSENT; break;
												case 4	: header.wFlags= MF_LOCAL | MF_PRIVATE | MF_CRASH; break;
												case 5	: header.wFlags= MF_LOCAL | MF_PRIVATE | MF_CRASH | MF_KILLIFSENT; break;
												default	:	header.wFlags= MF_SENT | MF_LOCAL | MF_PRIVATE; break;
											}
				
										fseek(in,0,SEEK_END);
										length=ftell(in);
										if (length>B_SIZE)
											{	fclose(in);
												fclose(msg);
												fclose(hdr);
												return 4;
											}
										if (length==0)
											{	fclose(in);
												fclose(msg);
												fclose(hdr);
												return 5;
											}
										fseek(in,0,SEEK_SET);
				
										fread(buffer,length,1,in);
										convert(buffer,&length);

										header.wSize=(int) length;
										header.lStart= ftell(msg);
				
/* Hier wird die PID/MSGID direkt geschrieben */

										sprintf(temp2,"\001MSGID: %s %04x%04x\n",address,(int) Random(), (int) Random());
										fprintf(msg,temp2);
										header.wSize += (int) strlen(temp2);
										sprintf(temp2,"\001PID: FILE2MSG %s\n",version);
										fprintf(msg,temp2);
										header.wSize += (int) strlen(temp2);
										
										header.wUp = header.wReply =
										header.wReadcount = header.wCost = 0;
										for (x1=0; x1<=3; x1++)
											*(long *)(&header.lMsgidcrc+x1) = 0;

										fwrite(buffer,length,1,msg);
										fwrite(&header,sizeof(MSG_HEADER),1,hdr);
				
										fclose(in);
										fclose(msg);
										fclose(hdr);
										if (args==3)
											remove(temp);
									}
								error=Fsnext();
							}
					}
				free(buffer);
			}
		return 0;
	}

int convert(char *buffer, long *length)
	{	long k,newlength;
		char *src;
		int  ret=0;

		newlength=*length;
		src=&buffer[0];

		for (k=0; k<=(*length)-1; k++)
			{	if (buffer[k]!=13)
					*src++=buffer[k];
				else
					newlength--;
				ret += buffer[k];
			}
		strcpy(src,tearline);
		*length = newlength + sizeof(tearline);
		return(ret);
	}
