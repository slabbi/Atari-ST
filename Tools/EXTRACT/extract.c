#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>

#define VERSION	"1.0"

#define FALSE			0
#define TRUE			1
#define MAX_STR		256
#define EOS				'\0'
#define MAX(x,y)	((x)>(y)) ? (x) : (y)
#define MIN(x,y)	((x)<(y)) ? (x) : (y)

static int comp(int *x,int *y);
static int extract(int rc, int regions[], char *nl, char *out);

char	*txt[]	=	
	{	"EXTRACT V"VERSION"      Utility for FidoPoints.      (c) Stephan Slabihoud 1994\n",
		"\n",
		"\n"
	};

int regions[50],rc=0;

int main(int argc,char *argv[])
	{	int i;
		char out[PATH_MAX],nl[PATH_MAX];
		--argc;

		if (argc<2)
			{	for (i=0; i<=2; i++)
					fprintf(stderr,txt[i]);
				fprintf(stderr,"Usage: extract <nodelist> <output> [<region1> [<region2> ...]]\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <nodelist> - Nodelist\n");
				fprintf(stderr,"   <output> - New (region) nodelist\n");
				fprintf(stderr,"\n");
				fprintf(stderr,"   <region> - Region(s) to extract\n");
				fprintf(stderr,"              When this parameter is missing a list of\n");
				fprintf(stderr,"              all RCs is created\n");
				fprintf(stderr,"\n");
				fprintf(stderr,"\n\nThis program is *SHAREWARE*. It is published with AVALON.\n");
				getch();
				exit(2);
			}
		for (i=0; i<=2; i++)
			printf(txt[i]);

		strcpy(nl ,argv[1]);
		strcpy(out,argv[2]);

		if (argc!=2)
			{	for (i=3; i<=argc; i++)
					regions[i-2]=atoi(argv[i]);
				rc=argc-2;
				qsort(&regions[1],rc,sizeof(int),comp);
			}
		else
			rc=0;

		return(extract(rc,regions,nl,out));
	}

int comp(int *x,int *y)
	{	if (*x<*y) return(-1);
		else if (*x>*y) return(1);
		else return(0);
	}

int extract(int rc, int regions[], char *nl, char *list)
	{	char	line[MAX_STR],*pointer;
		long	counter=0,outcnt=0;
		int		region=-1;
		FILE *in,*out;

		in	= fopen(nl  ,"rb");
		out	= fopen(list,"wb");
		if (in==NULL || out==NULL)
			{	fprintf(stderr,"File open error\n");
				return(9);
			}
		setvbuf(in ,NULL,_IOFBF,32768U);
		setvbuf(out,NULL,_IOFBF,32768U);

		printf("\nProcessing... %lu (%lu)\r",counter,outcnt);

		fgets(line,MAX_STR-1,in);
		while (!feof(in))
			{
				if (strnicmp(line,"zone",4)==0)
					{	region=-1;
					}
				else if (strnicmp(line,"region",6)==0)
					{	pointer=strchr(line,',');
						region=atoi(pointer+1);
						if (rc>0)
							{	if (bsearch(&region,&regions[1],rc,sizeof(int),comp)==NULL)
									region=0;
							}
						else
							{	fputs(line,out);
								outcnt++;
								region=0;
							}
					}
				
				if (region!=0)
					{	fputs(line,out);
						outcnt++;
					}

				counter++;
				if (!(counter & 0xff))
					printf("Processing... %lu (%lu)\r",counter,outcnt);

				fgets(line,MAX_STR-1,in);
			}
		printf("Processed %lu lines, extracted %lu lines\n\n",counter,outcnt);

		fclose(out);
		fclose(in);
		return(0);
	}

