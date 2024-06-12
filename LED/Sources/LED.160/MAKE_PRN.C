#include <stdio.h>
#include <stddef.h>
#include <ext.h>
#include <string.h>
#include <ctype.h>

#define PRN_TAB	12

char	table[256][PRN_TAB];
int		options[64];
char	divers[32][PRN_TAB];

#define OPT_LEFTMARGIN	0
#define	DIV_INIT				0

char *skip_blanks(char *string)
{	while (*string && isspace(*string)) ++string;
	return (string);
}

int main(int argc, char *argv[])
{	FILE *fp;
	char *p,buffer[256];
	int	 i,original,conv;

	if (--argc!=1)
	{	fprintf(stderr,"\n\rUsage: make_prn <file.pn>\n\n'Make Printer 1.2' for LED 1.45 (c) St.Slabihoud 1995,96\n\n\r");
		getch();
		return(1);
	}

	fp=fopen(argv[1],"rb");
	if (!fp)
	{	fprintf(stderr,"\nFile open error!\n\r");
		getch();
		return(2);
	}

	memset(table,0,sizeof(table));
	memset(divers,0,sizeof(divers));

	for(i=0; i<=255; i++)
	{	table[i][0]=1;
		table[i][1]=i;
	}
	for(i=0; i<64; i++)
		options[i]=0;

	while(fgets(buffer, 255, fp))
	{	p=skip_blanks(buffer);
		p=strtok(p," ,\t\n\r");
		if (!p)	continue;
		if (*p==';' || *p=='\0') continue;

		if (!stricmp(p,"LEFTMARGIN"))
		{	if ((p=strtok(NULL," ,\t\n\r"))!=NULL)
				options[OPT_LEFTMARGIN]=(int)strtoul(p,NULL,0);
		}
		else if (!stricmp(p,"INIT"))
		{	i=0;
			while ((p=strtok(NULL," ,\t\n\r"))!=NULL && i<PRN_TAB-1)
			{	conv=(int)strtoul(p,NULL,0);
				divers[DIV_INIT][++i]=conv;
			}
			divers[DIV_INIT][0]=i;
		}
		else
		{	original=(int)strtoul(p,NULL,0);
			i=0;
			while ((p=strtok(NULL," ,\t\n\r"))!=NULL && i<PRN_TAB-1)
			{	conv=(int)strtoul(p,NULL,0);
				table[original][++i]=conv;
			}
			table[original][0]=i;
		}
	}
	fclose(fp);
	fp=fopen("LED.PRN","wb");
	fwrite(&table,PRN_TAB << 8,1,fp);
	fwrite(&options,64,sizeof(int),fp);
	fwrite(&divers,PRN_TAB << 5,1,fp);
	fclose(fp);
	return(0);
}
