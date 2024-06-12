#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ext.h>
#include <tos.h>
#include "define.h"
#include "diverse.h"
#include "crc16.h"
#include "windmini.h"
#include "vars.h"

extern void xerror(int errorcode);

static int write_file(char *src,char *dst,int clean);
static unsigned int calc_crc(char *pointer,unsigned int crc);

int applydiff(char *txt)
	{	int 	i,error,clean=FALSE;
		char	srcpfad[MAX_STR],diffpfad[MAX_STR],
					src[MAX_STR],diff[MAX_STR],
					listline[MAX_STR],diffline[MAX_STR],
					origdiff[MAX_STR],*start;
		FILE	*fin,*fdiff;

		WPRINT("")
		WPRINT("Applying diff-file to node- or pointlist. Please wait...")

		start=strtok(txt," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		strcpy(src,start);

		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		strcpy(diff,start);
		strcpy(origdiff,start);

		clean=FALSE;
		start = strtok(NULL,"");
		if (start!=NULL)
			if (stricmp(start,"CLEAN")==0)
				clean=TRUE;

		getpfad(src, srcpfad);
		getpfad(diff, diffpfad);

		error=0;

		if ((error=xFsfirst(src,0))!=0)		/* Es gibt keine Datei src	*/
			{	WPRINT("Nodelist not found...")
				WPRINT("")
				return(3);
			}
		else
			{	strcpy(src,srcpfad);
				strcat(src,dta_buffer.d_fname);
			}
		i=(int)strlen(src);
		while (error==0 && (stricmp(src+i-4,".$$$")==0	||
												stricmp(src+i-4,".CFG")==0	||
												strnicmp(src+i-4,".A",2)==0) )
			{	error=xFsnext();
				if (error==0)
					{	strcpy(src,srcpfad);
						strcat(src,dta_buffer.d_fname);
						error=0;
						break;
					}
			}
		if (error!=0)
			{	WPRINT("Nodelist not found...")
				WPRINT("")
				return(3);
			}

		do
			{	strcpy(diff,origdiff);
				if (xFsfirst(diff,0)!=0)							/* Es gibt keine Diff-Datei mehr */
					{	WPRINT("Nothing (more) to do...")
						return(0);
					}
				else
					{	strcpy(diff,diffpfad);
						strcat(diff,dta_buffer.d_fname);
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
								strcat(diff,dta_buffer.d_fname);
							}
					}
				while (error==0);
		
				if (error==0)						/* Es wurde eine Liste mit Diff-Datei ermittelt */
					error=write_file(src,diff,clean);
				else
					{	WPRINT("Nothing more to do...")
						break;
					}
			}
		while (error==0 || error==1);

		return(error>=0 ? error : 0);
	}

static int write_file(char *src,char *diff,int clean)
	{	FILE	*fin,*fdiff,*fout;
		long	counter=0;
		unsigned int crc=0,listcrc=0;
		int		number,i,listday,firstlinewritten=FALSE,min,sek,hun;
		char	diffline[MAX_STR],listline[MAX_STR],
					dst[MAX_STR],temp[MAX_STR],
					*pointer;
		clock_t timer;

		strcpy(dst,src);
		pointer=strrchr(dst,'.');
		if (pointer!=NULL)	strcpy(pointer,".$$$");
		else								strcat(dst,".$$$");

		WPRINT("")
		sprintf(temp,"Adding : %s",diff);
		WPRINT(temp)
		sprintf(temp,"To     : %s",src);
		WPRINT(temp)

		fin		= fopen(src,"rb");
		fdiff	= fopen(diff,"rb");
		fout	= fopen(dst ,"wb");

		if (fin==NULL || fdiff==NULL || fout==NULL)
			{	xerror(7);
				return(9);
			}

		setvbuf(fin ,NULL,_IOFBF,32768U);
		setvbuf(fdiff,NULL,_IOFBF,32768U);
		setvbuf(fout,NULL,_IOFBF,32768U);

		counter++;
		fgets(diffline, MAX_STR-1, fdiff);		/* erste Zeile Åberspringen */

		timer = clock();
		while (!feof(fdiff) && !feof(fin))
			{	sprintf(temp,"Converting... %lu",counter);
				WWRITE(temp)

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

		sprintf(temp,"Converted.... %lu lines in %u'%02u\"%02u (mm'ss\"hh)",counter,min,sek,hun);
		WPRINT(temp)

		fputc(26,fout);
		fclose(fdiff);
		fclose(fin);
		fclose(fout);

		fin	= fopen(dst,"rb");						/* Bestimme TAG und CRC */
		if (fin==NULL)
			{	WPRINT("File open error (temporary file)...")
				WPRINT("")
				return(6);
			}
		fgets(listline,MAX_STR-1,fin);
		fclose(fin);
		strupr(listline);
		pointer = strstr(listline,"DAY NUMBER ");
		if (pointer==NULL)
			{	WPRINT("Cannot extract new day number...")
				WPRINT("")
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
		sprintf(temp,"Created: %s",src);
		WPRINT(temp)

		if (listcrc!=crc)
			{	sprintf(temp,"CRC error! New nodelist may be corrupted...(%u, expected: %u)",crc,listcrc);
				WPRINT(temp)
				WPRINT("")
			}
		if (listcrc!=crc && !clean)
			return(1);
		return(0);
	}

static unsigned int calc_crc(char *pointer,unsigned int crc)
	{	while (*pointer)
			crc=UpdateCRC(*pointer++,crc);
		return(crc);
	}
