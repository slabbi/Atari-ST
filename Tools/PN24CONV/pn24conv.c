#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ext.h>
#include <tos.h>

#define VERSION "1.4"

#define MAX_STR	512
#define EOS			'\0'

char		l_format[] = "%-42s %5d/%d.%d\r\n",
				s_format[] = "%s %d/%d.%d\r\n",
				*format = l_format,

				l_nformat[] = "%-42s %5d/%d\r\n",
				s_nformat[] = "%s %d/%d\r\n",
				*nformat = l_nformat,

				l_zformat[] = "%-42s %3d:%d/%d.%d\r\n",
				s_zformat[] = "%s %d:%d/%d.%d\r\n",
				*zformat = l_zformat,

				l_znformat[] = "%-42s %3d:%d/%d\r\n",
				s_znformat[] = "%s %d:%d/%d\r\n",
				*znformat = l_znformat;

char		src[MAX_STR],dst[MAX_STR];
int			defzone=0;

char	*txt[]	=	
	{	"PN24CONV V"VERSION"   Utility for FidoPoints.   (c) Stephan Slabihoud 1993-1994\n",
		"\n",
		"\n",
		"\n"
	};

void write_file(void);
void konvert(char *name);

int main(int argc,char *argv[])
	{	int i;
		--argc;

		if (argc!=3 && argc!=4)
			{	for (i=0; i<=3; i++)
					fprintf(stderr,txt[i]);
				fprintf(stderr,"Usage: pn24conv <frm> <src> <dst> [<zone>]\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <frm>  - s=short format\n");
				fprintf(stderr,"        - l=long format\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <src>  - Pointlist (e.g. E:\\FIDO\\POINTS24.*)\n");
				fprintf(stderr," <dst>  - Userlist  (e.g. E:\\FIDO\\PN24CONV.LST)\n");
				fprintf(stderr," <zone> - Add zone <zone> to output (e.g. 2)\n");
				fprintf(stderr,"\n\nThis program is *SHAREWARE*. It is published with AVALON.\n");
				getch();
				exit(2);
			}
			
		switch(*argv[1])
			{	case 's'	:
				case 'S'	:	format	 = s_format;
										nformat  = s_nformat;
										zformat  = s_zformat;
										znformat = s_znformat;	break;
				case 'l'	:
				case 'L'	: format	 = l_format;
										nformat  = l_nformat;
										zformat	 = l_zformat;
										znformat = l_znformat;	break;
			}
		strcpy(src,argv[2]);
		strcpy(dst,argv[3]);
		if (argc==4)
			defzone=atoi(argv[4]);

		for (i=0; i<=3; i++)
			printf(txt[i]);
		write_file();
		return(0);
	}

void write_file(void)
	{	int		zone,net,node,point;
		long	counter=0;
		FILE	*in,*out;
		char	line[MAX_STR],temp[MAX_STR],name[MAX_STR],*pointer;

		in	= fopen(src,"rb");
		out	= fopen(dst,"wb");
		if (in==NULL || out==NULL)
			exit(1);
		setvbuf(in ,NULL,_IOFBF,32768U);
		setvbuf(out,NULL,_IOFBF,32768U);

		printf("Converting... %lu\r",counter);
		fgets(line,MAX_STR-1,in);
		while (!feof(in))
			{	
				if (strnicmp(line,"Host",4)==0 ||
						strnicmp(line,"Boss",4)==0)
					{	zone = defzone;
						if (strnicmp(line,"Boss",4)==0)
							{	sscanf(line,"%[^,],%u:%u/%u",temp,&zone,&net,&node);
/*							konvert(name);
								sprintf(temp,znformat,name,zone,net,node);
								fputs(temp,out);
								counter++;
*/
							}
						else
							{	sscanf(line,"%[^,],%[^,],%u/%u,%[^,],%[^,]",temp,temp,&net,&node,temp,name);
								konvert(name);
								if (zone==0)	sprintf(temp,nformat,name,net,node);
								else					sprintf(temp,znformat,name,zone,net,node);
								fputs(temp,out);
								counter++;
							}

						if (!(counter & 0xff))	printf("Converting... %lu\r",counter);

						fgets(line,MAX_STR-1,in);
						while (strnicmp(line,"Host",4)!=0 &&
									 strnicmp(line,"Region",6)!=0 &&
									 strnicmp(line,"Boss",4)!=0)
							{ if ( *line!=';' )
									{	if (*line!=',')	{	pointer=strchr(line,',');
																			if (pointer==NULL)
																				pointer=line;
																		}
										else						{	pointer=line;
																		}
										sscanf(pointer,",%u,%[^,],%[^,],%[^,]",&point,temp,temp,name);
										konvert(name);

										if (zone==0)	sprintf(temp,format,name,net,node,point);
										else					sprintf(temp,zformat,name,zone,net,node,point);

										fputs(temp,out);
										counter++;
										if (!(counter & 0xff))	printf("Converting... %lu\r",counter);
									}
								fgets(line,MAX_STR-1,in);
								if (feof(in))	break;
							}
					}
				else
					{	fgets(line,MAX_STR-1,in);
					}
			}
		printf("Converted.... %lu names\n",counter);

		fclose(out);
		fclose(in);
	}

void konvert(char *name)
	{	char *pointer,temp[MAX_STR];
		strcpy(temp,name);
		pointer = strrchr(temp,'_');
		if (pointer==NULL)
			{	strcpy(name,temp);
			}
		else
			{	strcpy(name,pointer+1);
				strcat(name,", ");
				*pointer = EOS;
				while ((pointer=strchr(temp,'_'))!=NULL)
					*pointer=' ';
				strcat(name,temp);
			}
	}
