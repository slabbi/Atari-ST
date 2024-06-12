#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ext.h>
#include <tos.h>

#define VERSION "1.2"

#define MAX_STR	512
#define EOS			'\0'
#define	FALSE		0
#define TRUE		1

char	*txt[]	=	
	{	"COMPDIFF V"VERSION"   Utility for FidoPoints.   (c) Stephan Slabihoud 1993-1994\n",
		"\n",
		"\n",
		"\n"
	};

DTA		dta;

int  write_file(char *src,char *dst,int clean);
void getpfad(char *fname, char *xpfad);
unsigned int calc_crc(char *pointer,unsigned int crc);

extern unsigned int UpdateCRC(unsigned int c,unsigned int crc);

int main(int argc,char *argv[])
	{	int 	i,error,clean=FALSE;
		char	srcpfad[MAX_STR],diffpfad[MAX_STR],
					src[MAX_STR],diff[MAX_STR],
					listline[MAX_STR],diffline[MAX_STR],
					origdiff[MAX_STR];
		FILE	*fin,*fdiff;

		--argc;
		if (argc!=2 && argc!=3)
			{	for (i=0; i<=3; i++)
					fprintf(stderr,txt[i]);
				fprintf(stderr,"Usage: compdiff <list> <diff> [clear]\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <list>  - Point- or nodelist (e.g. E:\\FIDO\\NODELIST.*)\n");
				fprintf(stderr," <diff>  - Point- or nodediff (e.g. E:\\FIDO\\NODEDIFF.*)\n");
				fprintf(stderr," clear   - Delete ALWAYS the original list and diff!\n");
				fprintf(stderr,"           (Do not recognize crc-errors!)\n");
				fprintf(stderr,"\n\nThis program is *SHAREWARE*. It is published with AVALON.\n");
				getch();
				exit(2);
			}
		strcpy(src,argv[1]);
		strcpy(diff,argv[2]);
		strcpy(origdiff,diff);

		if (argc==3)
			clean=TRUE;

		for (i=0; i<=3; i++)
			printf(txt[i]);

		getpfad(src, srcpfad);
		getpfad(diff, diffpfad);

		error=0;
		Fsetdta(&dta);
		if ((error=Fsfirst(src,0))!=0)		/* Es gibt keine Datei src	*/
			{	printf("Nodelist not found...\n\n");
				return(3);
			}
		else
			{	strcpy(src,srcpfad);
				strcat(src,dta.d_fname);
			}
		i=(int)strlen(src);
		while (error==0 && (stricmp(src+i-4,".$$$")==0	||
												stricmp(src+i-4,".CFG")==0	||
												strnicmp(src+i-4,".A",2)==0) )
			{	error=Fsnext();
				if (error==0)
					{	strcpy(src,srcpfad);
						strcat(src,dta.d_fname);
						error=0;
						break;
					}
			}
		if (error!=0)
			{	printf("Nodelist not found...\n\n");
				return(3);
			}

		do
			{	strcpy(diff,origdiff);
				if (Fsfirst(diff,0)!=0)							/* Es gibt keine Diff-Datei mehr */
					{	printf("Nothing (more) to do...\n\n");
						return(0);
					}
				else
					{	strcpy(diff,diffpfad);
						strcat(diff,dta.d_fname);
					}
		
				fin		= fopen(src,"rb");						/* Welche Liste liegt vor??? */
				fgets(listline, MAX_STR-1, fin);
				fclose(fin);
		
				error=0;
				do
					{	fdiff	= fopen(diff,"rb");
						fgets(diffline, MAX_STR-1, fdiff);
						fclose(fin);
						if (strcmp(diffline,listline)==0)
							{	error=0;								/* Passende Diff-Datei gefunden */
								break;
							}
						error=Fsnext();
						if (error==0)								/* Es gibt noch eine Diff-Datei */
							{	strcpy(diff,diffpfad);
								strcat(diff,dta.d_fname);
							}
					}
				while (error==0);
		
				if (error==0)						/* Es wurde eine Liste mit Diff-Datei ermittelt */
					error=write_file(src,diff,clean);
				else
					{	printf("\nNothing more to do...\n\n");
						break;
					}
			}
		while (error==0 || error==1);

		return(error>=0 ? error : 0);
	}

int write_file(char *src,char *diff,int clean)
	{	FILE	*fin,*fdiff,*fout;
		long	counter=0;
		unsigned int crc=0,listcrc=0;
		int		number,i,listday,firstlinewritten=FALSE,min,sek,hun;
		char	diffline[MAX_STR],listline[MAX_STR],
					dst[MAX_STR],
					*pointer;
		clock_t timer;

		strcpy(dst,src);
		pointer=strrchr(dst,'.');
		if (pointer!=NULL)	strcpy(pointer,".$$$");
		else								strcat(dst,".$$$");

		printf("\nAdding : %s\n",diff);
		printf("To     : %s\n",src);

		fin		= fopen(src,"rb");
		fdiff	= fopen(diff,"rb");
		fout	= fopen(dst ,"wb");

		if (fin==NULL || fdiff==NULL || fout==NULL)
			{	printf("File open error...\n\n");
				return(9);
			}

		setvbuf(fin ,NULL,_IOFBF,32768U);
		setvbuf(fdiff,NULL,_IOFBF,32768U);
		setvbuf(fout,NULL,_IOFBF,32768U);

		counter++;
		fgets(diffline, MAX_STR-1, fdiff);		/* erste Zeile Åberspringen */

		timer = clock();
		while (!feof(fdiff) && !feof(fin))
			{	printf("Converting... %lu\r",counter);

				if (fgets(diffline, MAX_STR-1, fdiff)==NULL)
					break;

				number = atoi(&diffline[1]);
				switch (toupper(diffline[0]))
					{	case	'A'	:	for (i=1; i<=number; i++)
													{	fgets(diffline,MAX_STR-1,fdiff);
														fputs(diffline,fout);
														if (firstlinewritten)
															crc=calc_crc(diffline,crc);
														else
															firstlinewritten=TRUE;
													}
												break;
						case	'C'	:	for (i=1; i<=number; i++)
													{	fgets(listline,MAX_STR-1,fin);
														fputs(listline,fout);
														if (firstlinewritten)
															crc=calc_crc(listline,crc);
														else
															firstlinewritten=TRUE;
													}
												break;
						case	'D'	:	for (i=1; i<=number; i++)
													{	fgets(listline,MAX_STR-1,fin);
													}
					}
				counter += (long) number;
			}
		timer = clock() - timer;
		hun = (int) ((timer % CLK_TCK) / 2);
		sek = (int) ((timer % (CLK_TCK*60)) / CLK_TCK);
		min = (int) (timer / (CLK_TCK*60));

		printf("Converted.... %lu lines in %u'%02u\"%02u (mm'ss\"hh)\n",counter,min,sek,hun);
		fputc(26,fout);
		fclose(fdiff);
		fclose(fin);
		fclose(fout);

		fin	= fopen(dst,"rb");						/* Bestimme TAG und CRC */
		if (fin==NULL)
			{	printf("File open error (temporary file)...\n\n");
				return(6);
			}
		fgets(listline,MAX_STR-1,fin);
		fclose(fin);
		strupr(listline);
		pointer = strstr(listline,"DAY NUMBER ");
		if (pointer==NULL)
			{	printf("Cannot extract new day number...\n\n");
				return(7);
			}
		pointer += 11;
		listday = atoi(pointer);
		pointer=strchr(pointer,':');
		if (pointer!=NULL)
			listcrc = atoi(pointer+1);

		if (listcrc==crc || clean)
			{	remove(src);
				remove(diff);
			}

		pointer=strrchr(src,'.');					/* Datei bekommt neue Endung! */
		if (pointer!=NULL)
			sprintf(pointer,".%03u",listday);
		else
			strcat(pointer,".XXX");

		rename(dst,src);
		printf("Created: %s\n",src);
		if (listcrc!=crc)
			printf("CRC error! New nodelist may be corrupted...(%u, expected: %u)\n",crc,listcrc);
		if (listcrc!=crc && !clean)
			return(1);
		return(0);
	}

unsigned int calc_crc(char *pointer,unsigned int crc)
	{	while (*pointer)
			crc=UpdateCRC(*pointer++,crc);
		return(crc);
	}

void getpfad(char *fname, char *xpfad)
	{	register char *pointer;
		pointer = strrchr(fname,'\\');
		if (pointer==NULL)
			*xpfad=EOS;
		else
			{	*pointer=EOS;
				strcpy(xpfad,fname);
				strcat(xpfad,"\\");
				*pointer='\\';
			}
	}
