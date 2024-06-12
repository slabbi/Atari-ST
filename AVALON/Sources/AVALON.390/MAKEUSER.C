#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ext.h>
#include <tos.h>
#include "define.h"
#include "diverse.h"
#include "windmini.h"

extern void xerror(int errorcode);

static int write_file(char *src,char *dst,int defzone,int pointlist);
static void konvert(FILE *out,int zone,int net,int node,int point,char *name);

int makeuserlist(char *txt)
	{	int defzone=0,pointlist=FALSE;
		char src[MAX_STR],dst[MAX_STR],fmt[MAX_STR],
				 *start;

		start=strtok(txt," ");
		if (start==NULL)
			{	xerror(0);
				return 2;
			}
		strcpy(fmt,start);

		start=strtok(NULL," ");
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

		WPRINT("")
		if (pointlist)
			WPRINT("Converting pointlist to userlist. Please wait...")
		else
			WPRINT("Converting nodelist to userlist. Please wait...")

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
			{	xerror(7);
				return(9);
			}
		setvbuf(in ,NULL,_IOFBF,32768U);
		setvbuf(out,NULL,_IOFBF,32768U);

		sprintf(temp,"Converting... %lu",counter);
		WWRITE(temp)

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
					{	sprintf(temp,"Converting... %lu",counter);
						WWRITE(temp)
					}

				fgets(line,MAX_STR-1,in);
			}
		sprintf(temp,"Converted.... %lu names",counter);
		WPRINT(temp)
		WPRINT("")

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
