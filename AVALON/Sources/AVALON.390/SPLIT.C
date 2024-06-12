#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ext.h>
#include <tos.h>
#include "define.h"
#include "windmini.h"

extern void xerror(int errorcode);
static int splitting(char *filename,long max_bytes,int max_lines);

int split_it(char *txt)
	{	char *start,filename[MAX_STR],temp[MAX_STR];
		long	max_bytes;
		int		max_lines;


		start=strtok(txt," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		strcpy(filename,start);

		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		max_bytes = atol(start);

		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		max_lines = atoi(start);


		WPRINT("")
		sprintf(temp,"Splitting file: %s",filename);
		WPRINT(temp)
		if (max_lines)
			{	sprintf(temp," max. length (lines): %u",max_lines);
				WPRINT(temp);
			}
		if (max_bytes)
			{	sprintf(temp," max. length (bytes): %lu",max_bytes);
				WPRINT(temp);
			}
		return(splitting(filename,max_bytes,max_lines));
	}

static int splitting(char *filename,long max_bytes,int max_lines)
	{	int		ist_lines=0,part=1;
		long	ist_bytes=0;
		char	outfile[MAX_STR],temp[MAX_STR],dum[MAX_STR],*pointer;
		FILE	*fp,*infile;

		infile = fopen(filename,"rb");
		if (infile==NULL)
			{	xerror(7);
				return(9);
			}
		setvbuf(infile,NULL,_IOFBF,32768U);

		strcpy(outfile,filename);
		pointer=strrchr(outfile,'\\');
		if (pointer==NULL)
			{	pointer=outfile;
				pointer=strchr(pointer,'.');
				if (pointer!=NULL)
					*pointer='\0';
			}
		else
			{	pointer=strchr(pointer,'.');
				if (pointer!=NULL)
					*pointer='\0';
			}
		
		while (!feof(infile))
			{	sprintf(temp,"%s.%03u",outfile,part);

				sprintf(dum,"  Writing: %s",temp);
				WPRINT(dum)

				fp = fopen(temp,"wb");
				setvbuf(fp,NULL,_IOFBF,32768U);

				for (;;)
					{	fgets(temp,255,infile);
						if (feof(infile)) break;
						fputs(temp,fp);

						if (max_lines!=0)
							{	ist_lines++;
								if (ist_lines>=max_lines)
									{	ist_lines=0;
										ist_bytes=0;
										break;
									}
							}
						if (max_bytes!=0)
							{	ist_bytes += strlen(temp);
								ist_bytes++;
								if (ist_bytes>=max_bytes)
									{	ist_lines=0;
										ist_bytes=0;
										break;
									}
							}
					}
				part++;
				fclose(fp);
			}
		fclose(infile);
		return(0);
	}
