#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ext.h>
#include <tos.h>

#define VERSION "1.4"

#define HEADLINE	"Text splitted by SPLIT-IT (%u)\r\n\r\n"
#define FOOTLINE	"\r\n\r\n=== SPLIT-IT (%u)\r\n"

char		filename[256];
long		max_bytes;
int			max_lines;

void splitting(void);

int main(int argc,char *argv[])
	{	--argc;

		if (argc==3 && strchr(argv[1],'?')==NULL && 
									 strchr(argv[1],'*')==NULL)
			{	strcpy(filename,argv[1]);
				max_bytes = atol(argv[2]);
				max_lines = atoi(argv[3]);
			}
		else
			{	fprintf(stderr,"SPLIT_IT V"VERSION"   Utility for FidoPoints.   (c) Stephan Slabihoud 1993-1994\n");
				fprintf(stderr,"\n");
				fprintf(stderr,"\n");
				fprintf(stderr,"\n");
				fprintf(stderr,"Usage: split_it <file> <bytes> <lines>\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <file>      - file that should be converted (e.g. E:\\STORY.TXT)\n");
				fprintf(stderr,"               No wildcards allowed!\n");
				fprintf(stderr," <bytes>     - each splitted part should have <bytes> bytes (0 if not needed)\n");
				fprintf(stderr," <lines>     - each splitted part should have <lines> lines (0 if not needed)\n");
				fprintf(stderr,"\n\n\n\nThis program is *SHAREWARE*. It is published with AVALON.\n");
				getch();
				exit(2);
			}

		printf("SPLIT_IT V"VERSION"   Utility for FidoPoints.   (c) Stephan Slabihoud 1993-1994\n");
		printf("\n");
		printf("\n");
		printf("\n");
		splitting();
		return(0);
	}

void splitting(void)
	{	int		ist_lines=0,part=1;
		long	ist_bytes=0;
		char	outfile[256],temp[256],*pointer;
		FILE	*fp,*infile;

		infile = fopen(filename,"rb");
		if (infile==NULL)
			{	fprintf(stderr,"File open error!\n");
				getch();
				exit(3);
			}
		setvbuf(infile ,NULL,_IOFBF,32768U);

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
		
		printf("Splitting: %s\n",filename);

		while (!feof(infile))
			{	sprintf(temp,"%s.%03u",outfile,part);
				printf("  Writing: %s\n",temp);
				fp = fopen(temp,"wb");
				setvbuf(fp ,NULL,_IOFBF,32768U);

/*			fprintf(fp,HEADLINE,part); */

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
				fprintf(fp,FOOTLINE,part);
				part++;
				fclose(fp);
			}
		fclose(infile);
	}
