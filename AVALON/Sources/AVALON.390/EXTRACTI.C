#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include "define.h"
#include "windmini.h"

extern void xerror(int errorcode);

static int comp(int *x,int *y);
static int extract(int rc, int regions[], char *nl, char *out);

int regions[50];

int extract_it(char *txt)
	{	char *start,temp[MAX_STR],temp2[20],out[PATH_MAX],nl[PATH_MAX];
		int i,rc=0;

		start=strtok(txt," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		strcpy(nl,start);

		start=strtok(NULL," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		strcpy(out,start);

		start=strtok(NULL," ");
		while (start!=NULL)
			{	rc++;
				regions[rc]=atoi(start);
				start=strtok(NULL," ");
			}

		WPRINT("")
		sprintf(temp,"Extracting following regions from %s:",nl);
		WPRINT(temp)
		if (rc>0)
			{	qsort(&regions[1],rc,sizeof(int),comp);
				*temp=EOS;
				for (i=1; i<=rc; i++)
					{	strcat(temp," ");
						strcat(temp,itoa(regions[i],temp2,10));
					}
				WPRINT(temp)
			}
		else
			{	WPRINT(" no regions defined, creating list of all RCs")
			}

		return(extract(rc,regions,nl,out));
	}

static int comp(int *x,int *y)
	{	if (*x<*y) return(-1);
		else if (*x>*y) return(1);
		else return(0);
	}

static int extract(int rc, int regions[], char *nl, char *list)
	{	char	temp[MAX_STR],line[MAX_STR],*pointer;
		long	counter=0,outcnt=0;
		int		region=-1;
		FILE *in,*out;

		in	= fopen(nl  ,"rb");
		out	= fopen(list,"wb");
		if (in==NULL || out==NULL)
			{	xerror(7);
				return(9);
			}
		setvbuf(in ,NULL,_IOFBF,32768U);
		setvbuf(out,NULL,_IOFBF,32768U);

		sprintf(temp,"Processing... %lu (%lu)",counter,outcnt);
		WWRITE(temp)

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
					{	sprintf(temp,"Processing... %lu (%lu)",counter,outcnt);
						WWRITE(temp)
					}

				fgets(line,MAX_STR-1,in);
			}
		sprintf(temp,"Processed %lu lines, extracted %lu lines",counter,outcnt);
		WPRINT(temp)
		WPRINT("")

		fclose(out);
		fclose(in);
		return(0);
	}
