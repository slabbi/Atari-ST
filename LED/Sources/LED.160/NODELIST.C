#include	"ccport.h"
#include	"ledgl.h"
#include	"nodelist.h"

static NLINDEX	*index=NULL;
static NLDOMAIN	*domain=NULL;

static long	systems, domains;

#define		NLCONTINOUSMAIL		(1U << 1)
#define		NLNOARCMAIL				(1U << 2)
#define		NLPRIVATE					(1U << 8)
#define		NLHOLD						(1U << 9)
#define		NLZONECOORD				6
#define		NLREGCOORD				5
#define		NLNETCOORD				4
#define		NLHUB							3
#define		NLNODE						2
#define		NLPOINT						1
#define		NLMULTIZONE				0


static void UpdateAddress(NLADDRESS *addr, byte trigger, uword value)
{	switch (trigger)
	{	case NLZONECOORD:	addr->zone = addr->net = value;
											addr->node = addr->point = 0; break;
		case NLREGCOORD:
		case NLNETCOORD:	addr->net = value;
											addr->node = addr->point = 0; break;
		case NLHUB:
		case NLNODE:			addr->node = value;
											addr->point = 0; break;
		case NLPOINT:			addr->point = value; break;
	}
}

static long GetListIndex(NLADDRESS *addr)
{	long	i, sys;
	NLADDRESS	curaddr;
	
#if 0
	for (i=0; i<domains; i++)
		if (stricmp(addr->domain, domain[i].name))
		{	found=TRUE;
			break;
		}
#endif

	for (i=0; i<domains; i++)
	{	strcpy(addr->domain,domain[i].name);

#if 0
		if (!found) return(EOF);
			found=FALSE;
#endif

		if (domain[i].zone!=NLMULTIZONE && domain[i].zone!=addr->zone)
			return(EOF);

		for (sys=domain[i].startoffset/sizeof(NLINDEX);
								sys<(domain[i].startoffset/sizeof(NLINDEX))+domain[i].length; sys++)
		{	UpdateAddress(&curaddr, index[sys].type, index[sys].value);
			if (curaddr.node==addr->node && curaddr.net  ==addr->net &&
					curaddr.zone==addr->zone && curaddr.point==addr->point)
				return(sys);
		}
	}
	return(EOF);
}

static word ReadNLindex(void)
{	byte	idxname[128],	domainname[128];
	FILE	*idxfile, *domainfile;
	long	idxlength, domainlength;
	struct stat	statbuf;
	
	sprintf(idxname,		"%sINDEX.BNL",		gl_nodelistpath);
	sprintf(domainname,	"%sDOMAINS.BNL",	gl_nodelistpath);
	
	stat(idxname, 	&statbuf);	idxlength 		= statbuf.st_size;
	stat(domainname,&statbuf);	domainlength	= statbuf.st_size;
	
	index		= malloc(idxlength);
	domain	= malloc(domainlength);

	if (!index || !domain)
		return(FALSE);
	
	idxfile = fopen(idxname, "rb");
	if (!idxfile)
		return(FALSE);
	fread(index, idxlength, 1, idxfile);
	fclose(idxfile);
	
	domainfile = fopen(domainname, "rb");
	if (!domainfile)
		return(FALSE);
	fread(domain, domainlength, 1, domainfile);
	fclose(domainfile);
	
	systems = idxlength / sizeof(NLINDEX);
	domains = domainlength / sizeof(NLDOMAIN);
	return(TRUE);
}

static word GetSystemInfo(NLADDRESS *addr, NLSYSTEM *buf)
{	long	entry = GetListIndex(addr);
	byte	filename[128];
	FILE	*datfile;
	if (entry==EOF)
		return(FALSE);
	sprintf(filename, "%sNODEINFO.BNL", gl_nodelistpath);
	datfile = fopen(filename, "rb");
	if (!datfile)
		return(FALSE);
	fseek(datfile, entry*sizeof(NLSYSTEM), SEEK_SET);
	fread(buf, sizeof(NLSYSTEM), 1, datfile);
	fclose(datfile);
	return(TRUE);
}

word search_bnl_usernode(uword zone, uword net, uword node, uword point,
												 NLSYSTEM *info,byte *retdomain)
{	word ret;
	NLADDRESS addr;
	NLSYSTEM buf;

	memset(&buf,0,sizeof(NLSYSTEM));
	addr.zone=zone;
	addr.net=net;
	addr.node=node;
	addr.point=point;
	*addr.domain=EOS;

	ret=ReadNLindex();

	if (ret)
		ret=GetSystemInfo(&addr, &buf);
	
	if (domain)	free(domain);
	if (index)	free(index);

	memcpy(info,&buf,sizeof(NLSYSTEM));
	if (retdomain)
		strcpy(retdomain,addr.domain);

	return(ret);
}

#if 0
word Listed(ADDRESS *addr)
{	return (GetListIndex(addr) == EOF ? FALSE : TRUE);
}
#endif
