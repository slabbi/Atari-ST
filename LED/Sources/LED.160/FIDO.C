#include "ccport.h"
#include <string.h>
#include "ledgl.h"
#include "ledfc.h"
#include <fido_msg.h>
#include <magx.h>

#define MAX(x,y)	((x)>(y)) ? (x) : (y)
#define MIN(x,y)	((x)<(y)) ? (x) : (y)

#define   TRUE      1
#define   FALSE     0

extern word gl_apid;

void send_msg(int id_to,long msg)
	{	int pipe[8];
		pipe[0] = FIDO_MSG;
		pipe[1] = gl_apid;
		pipe[2] = 0;
		*((long *) &pipe[3]) = FM_MSGREADER;
		*((long *) &pipe[5]) = msg;
		pipe[7] = 0;
		appl_write(id_to,16,pipe);
	}

word is_running(byte *prg)
{	word has_appl_search=FALSE,rc,aes_version,i,j;
	ulong value;
	int type,id;
	char program[9];
	MAGX_COOKIE *MagX;

	aes_version = _GemParBlk.global[0];
	MagX = getcookie( 'MagX', &value ) ? (MAGX_COOKIE *) value : NULL;
	if ( aes_version>=0x400 ||
			 wind_get(0,'WX',&i,&i,&i,&i)=='WX'	||
			 (MagX && MagX->aesvars && MagX->aesvars->version>=0x200) )
	{	if (appl_getinfo(4,&i,&i,&j,&i))
			has_appl_search = j ? TRUE : FALSE;
	}

	if (has_appl_search)
	{ rc = appl_search(0,program,&type,&id);
		while( rc )
			{	if ( !strncmpi(program,prg,8) && id!=gl_apid )
					return(id);
				rc = appl_search(1,program,&type,&id);
			}
	}
	else
	{ id = appl_find( prg );
		if ( id>=0 && id!=gl_apid )
			return(id);
	}
	return(-1);
}

void start_avalon(void)
{	word id;
	if ((id=is_running("AVALON  "))>=0)
		send_msg(id,AVAL_REQ);
}

void start_export(word receiver)
{	word id;
	if (receiver==0 && (id=is_running("AVALON  "))>=0)
		send_msg(id,AVAL_EXPORT);
	if (receiver==1 && (id=is_running("SEMPER  "))>=0)
		send_msg(id,SEMP_EXPORTER);
}



void scanaddr(char *addr,uword *zone,uword *net,uword *node,uword *point,char *domain)
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
			        while (isdigit(*p) && (p >= addr)) p--;
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
		
		if (domain!=NULL)
		{	p = strchr(addr, '@');
	    if (p)	{ p++;
				        sscanf(p,"%s",domain);
	  	  			}
	    else	*domain = EOS;
	  }

    if (q!=NULL)
    	*q=chr;
	}

char *mergeaddr(uword zone,uword net,uword node,uword point,char *domain)
	{ static char tmp[64];
		static char addr[64];
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
    if (point)	{ strcat(addr, ".");
						      itoa(point, tmp, 10);
									strcat(addr, tmp);
							  }
    if (domain && *domain)	{ strcat(addr, "@");
						      					  strcat(addr, domain);
												    }
		if (zone==0 && net==0 && node==0 && point==0)
			strcpy(addr,"?");
    return(addr);
}

byte *addr2file36(uword net,uword node,uword point)
	{	static byte filename[13];
		byte dum[10],*pointer;

		itoa(net	,dum,36);	sprintf(filename	,"%3s",dum);
		itoa(node ,dum,36);	sprintf(filename+3,"%3s",dum);
		itoa(point,dum,36);	sprintf(filename+6,"%2s",dum);
		pointer=filename;
		while (*pointer!=EOS)
			{	if (*pointer==' ')
					*pointer = '0';
				pointer++;
			}
		return(filename);
	}
