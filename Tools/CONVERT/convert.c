#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ext.h>
#include <tos.h>

#define VERSION	"1.0"

#define FALSE			0
#define TRUE			1
#define MAX_STR		256
#define EOS				'\0'
#define MAX(x,y)	((x)>(y)) ? (x) : (y)
#define MIN(x,y)	((x)<(y)) ? (x) : (y)

static char *mergeaddr(char *addr,int zone,int net,int node,int point,char *domain);
static void scanaddr(char *addr,int *zone,int *net,int *node,int *point,char *domain);
static int write_file(char *src,char *dst,int defzone,int pointlist);
static void konvert(FILE *out,int zone,int net,int node,int point,char *name);
static int makeuserlist(char *txt);

char	*txt[]	=	
	{	"CONVERT V"VERSION"   Utility for FidoPoints.   (c) Stephan Slabihoud 1993-1994\n",
		"\n",
		"\n"
	};

int main(int argc,char *argv[])
	{	int i;
		char cmd[512];
		--argc;

		if (argc!=3 && argc!=4)
			{	for (i=0; i<=2; i++)
					fprintf(stderr,txt[i]);
				fprintf(stderr,"Usage: convert <typ> <lst> <out> [<zone>]\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <typ>  - p = Pointlist (3D or 4D)\n");
				fprintf(stderr,"          n = Nodelist\n");
				fprintf(stderr,"\n");
				fprintf(stderr," <lst>  - 3D-/4D-Pointlist, Nodelist (e.g. E:\\FIDO\\POINTS24.*)\n");
				fprintf(stderr," <out>  - Userlist  (e.g. E:\\FIDO\\PN24CONV.LST)\n");
				fprintf(stderr," <zone> - Add zone <zone> to output (e.g. 2; only pointlist)\n");
				fprintf(stderr,"\n");
				fprintf(stderr,"Errorlevels:\n");
				fprintf(stderr," 0 - OK\n");
				fprintf(stderr," 2 - Missing Parameter\n");
				fprintf(stderr," 9 - File open error\n");
				fprintf(stderr,"\n\nThis program is *SHAREWARE*. It is published with AVALON.\n");
				getch();
				exit(2);
			}
		for (i=0; i<=2; i++)
			printf(txt[i]);
		sprintf(cmd,"%s %s %s",argv[1],argv[2],argv[3]);
		if (argc==4)
			{	strcat(cmd," ");
				strcat(cmd,argv[4]);
			}
		return(makeuserlist(cmd));
	}

static int makeuserlist(char *txt)
	{	int defzone=0,pointlist=FALSE;
		char src[MAX_STR],dst[MAX_STR],fmt[MAX_STR],
				 *start;

		start=strtok(txt," ");
		if (start==NULL)
			{	fprintf(stderr,"Missing parameter\n");
				return 2;
			}
		strcpy(fmt,start);

		start=strtok(NULL," ");
		if (start==NULL)
			{	fprintf(stderr,"Missing parameter\n");
				return 2;
			}
		strcpy(src,start);

		start=strtok(NULL," ");
		if (start==NULL)
			{	fprintf(stderr,"Missing parameter\n");
				return 2;
			}
		strcpy(dst,start);

		start = strtok(NULL,"");
		if (start!=NULL)
			defzone=atoi(start);

		switch(*fmt)
			{	case 'p'	:
				case 'P'	:	pointlist=TRUE; break;
				case 'n'	:
				case 'N'	: pointlist=FALSE; break;
			}

		printf("\n");
		if (pointlist)
			printf("Converting pointlist to userlist. Please wait...\n");
		else
			printf("Converting nodelist to userlist. Please wait...\n");

		return(write_file(src,dst,defzone,pointlist));
	}

static int write_file(char *src,char *dst,int defzone,int pointlist)
	{	int		zone,net,node,point;
		long	counter=0;
		FILE	*in,*out;
		char	line[MAX_STR],temp[MAX_STR],
					*pointer,*pointer2;

		in	= fopen(src,"rb");
		out	= fopen(dst,"wb");
		if (in==NULL || out==NULL)
			{	fprintf(stderr,"File open error\n");
				return(9);
			}
		setvbuf(in ,NULL,_IOFBF,32768U);
		setvbuf(out,NULL,_IOFBF,32768U);

		printf("\nConverting... %lu\r",counter);

		zone=
		net=
		node=
		point=0;

		if (pointlist)
			zone=defzone;

		fgets(line,MAX_STR-1,in);
		while (!feof(in))
			{	if (*line!=';')
					{	
						if (strnicmp(line,"zone",4)==0)
							{	pointer=strchr(line,',');
								zone=atoi(pointer+1);
								node=point=0;
								net=zone;
								konvert(out,zone,net,node,point,pointer);
							}
						else if (strnicmp(line,"region",6)==0)
							{	pointer=strchr(line,',');
								net=atoi(pointer+1);
								node=point=0;
								if (!pointlist)
									konvert(out,zone,net,node,point,pointer);
							}
						else if (strnicmp(line,"host",4)==0)
							{	pointer=strchr(line,',');
								if (pointlist)
									{	pointer2=strchr(pointer+1,',');
										net=atoi(pointer2+1);
										pointer2=strchr(pointer2+1,'/');
										if (pointer2==NULL)
											{	net=atoi(line+5);	/* ERROR:Fakenet nehmen */
												node=0;
											}
										else
											node=atoi(pointer2+1);
									}
								else
									{	net=atoi(pointer+1);
									}
								point=0;
								konvert(out,zone,net,node,point,pointer);
							}
						else if (strnicmp(line,"boss",4)==0)
							{	pointer=strchr(line,',');
								scanaddr(pointer+1,&zone,&net,&node,&point,temp);
								/* konvert(out,zone,net,node,point,pointer); => AUF GAR KEINEN FALL! */
							}
						else if ( *line==','										||
										  strnicmp(line,"hub",3)==0			||
										  strnicmp(line,"down",4)==0		||
										  strnicmp(line,"hold",4)==0		||
										  strnicmp(line,"pvt",3)==0 )
							{	pointer=strchr(line,',');
								if (pointlist)
									{	point=atoi(pointer+1);
										if (!node)
											{	node=point;
												point=0;
											}
									}
								else
									{	node=atoi(pointer+1);
									}
								konvert(out,zone,net,node,point,pointer);
							}
					}

				counter++;
				if (!(counter & 0xff))
					printf("Converting... %lu\r",counter);

				fgets(line,MAX_STR-1,in);
			}
		printf("Converted.... %lu names\n\n",counter);

		fclose(out);
		fclose(in);
		return(0);
	}

static void konvert(FILE *out,int zone,int net,int node,int point,char *name)
	{	char *pointer,
				 temp[MAX_STR];
		name=strchr(name+1,',');
		name=strchr(name+1,',');
		name=strchr(name+1,',')+1;
		pointer=strchr(name+1,',');
		if (pointer!=NULL)
			*pointer=EOS;

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
		
		fprintf(out,"%s %s\r\n",name,mergeaddr(temp,zone,net,node,point,""));
	}

static void scanaddr(char *addr,int *zone,int *net,int *node,int *point,char *domain)
	{	char *p,*q=NULL,*q1,*q2;
		int chr;
		
		q1 = strchr(addr,'\n');
		q2 = strchr(addr,'\r');
		if (q1!=NULL && q2!=NULL)
			q=MIN(q1,q2);
		else if (q1!=NULL)
			q=q1;
		else if (q2!=NULL)
			q=q2;
		if (q!=NULL)
			{	chr=*q;
				*q=EOS;
			}

		p = strchr(addr, ':');
    if (p)	*zone = atoi(addr);
    else   	*zone = 0;
    p = strchr(addr, '/');
    if (p)	{	p--;
			        while (strchr("0123456789", *p) && (p >= addr)) p--;
      			  p++;
			        *net = atoi(p);
	    			}
    else	*net = 0;
    p = strchr(addr, '/');
    if (p) 	{ p++;
		        *node = atoi(p);
	  			  }
    else	*node = atoi(addr);
    p = strchr(addr, '.');
    if (p) 	{ p++;
			        *point = atoi(p);
	    			}
    else	*point = 0;
    p = strchr(addr, '@');
    if (p)	{ p++;
			        sscanf(p,"%s",domain);
	    			}
    else	*domain = EOS;
    
    if (q!=NULL)
    	*q=chr;
	}

static char *mergeaddr(char *addr,int zone,int net,int node,int point,char *domain)
	{ static char tmp[64];
    *addr = EOS;
    if (zone)	{	itoa(zone, tmp, 10);
    						strcat(addr, tmp);
								strcat(addr, ":");
						  }
    if (zone || net)	{	itoa(net, tmp, 10);
												strcat(addr, tmp);
												strcat(addr, "/");
											}
    itoa(node, tmp, 10);
    strcat(addr, tmp);
    if (point && node)	{ strcat(addr, ".");
										      itoa(point, tmp, 10);
													strcat(addr, tmp);
											  }
    if (*domain)	{ strcat(addr, "@");
						        strcat(addr, domain);
							    }
    return(addr);
}
