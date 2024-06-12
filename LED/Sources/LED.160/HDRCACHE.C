#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"

#define DEBUG 		FALSE
#define MOREHDR		40L

void init_hdrcache(long win, byte *file)
{	struct ffblk dta;
	long i,flen,elcnt;
	HDR *hdr;
	
	if (wi[win].hdrbuf)
		free(wi[win].hdrbuf);
	wi[win].hdrbuf =
	wi[win].hdrptr = NULL;
	wi[win].ramhdr = FALSE;
	wi[win].hdrlen = 0;
	if (!findfirst(file,&dta,0))
		wi[win].hdrlen=dta.ff_fsize;
	if (opt.hdrcache && file != NULL && wi[win].hdrlen>0L)
	{
		flen=wi[win].hdrlen+(long)(sizeof(HDR)*MOREHDR);
		/* bleibt noch etwas fr den Msgtext ber? */
		if (wi[win].hdrlen > 0L && flen < (long)Malloc(-1L) - 65536L)
		{
			if ((wi[win].hdrbuf=(char *)calloc(1,flen))!=NULL)
			{
				wi[win].ramhdr=TRUE;
				elcnt=wi[win].hdrlen/sizeof(HDR);
				
				flen=fread(wi[win].hdrbuf,sizeof(HDR),elcnt,wi[win].hdrfp);
				
				if (flen != elcnt)
				{	wi[win].ramhdr=FALSE;
					free(wi[win].hdrbuf);
					wi[win].hdrbuf=NULL;
				}
				else
				{	for (i=0L; i<MOREHDR; i++)
					{	hdr = (HDR *)(wi[win].hdrbuf+wi[win].hdrlen+i*sizeof(HDR));
						hdr->wFlags |= F_DELETED;
					}
					wi[win].hdrlen+=(long)sizeof(HDR)*MOREHDR;
					wi[win].hdrptr=wi[win].hdrbuf;
				}
			}
			else
				nomemalert(3);
		}
	}
}

size_t hdrseek(long win, FILE *hdrfp, word msg, int mode)
{	long err=0L;
	size_t offset;
	HDR *hdr;
	
	offset=msg*sizeof(HDR);

	if (wi[win].ramhdr && offset <= wi[win].hdrlen && wi[win].hdrbuf != NULL)
	{
		if (mode == SEEK_SET)
			wi[win].hdrptr = (char *)(wi[win].hdrbuf+offset);
		else if (mode == SEEK_CUR)
		{
			if (wi[win].hdrptr >= wi[win].hdrbuf || wi[win].hdrptr <= wi[win].hdrbuf+wi[win].hdrlen-offset)
				wi[win].hdrptr+=offset;
			else
				return(fseek(hdrfp, offset, SEEK_CUR));
		}
		else if (mode == SEEK_END)
			wi[win].hdrptr = (char *)((wi[win].hdrbuf+(long)wi[win].lastmsg*sizeof(HDR))-offset);
		else
			err=-1L;
		hdr = (HDR *)wi[win].hdrptr;
		if (hdr->Size == 0)
			err=-1L;
		
		return(err);
	}
	else
	{
		return(fseek(hdrfp, offset, mode));
	}
}

size_t hdrread(long win, void *hptr, size_t count, FILE *hdrfp)
{	long i=0L;
	size_t el_size=sizeof(HDR);
	
	if (wi[win].ramhdr && wi[win].hdrbuf != NULL)
	{
		while (count-- > 0 && (long)wi[win].hdrptr < (long)(wi[win].hdrbuf+wi[win].hdrlen))
		{
			memcpy(hptr,wi[win].hdrptr,el_size);
			wi[win].hdrptr+=el_size;
			hptr=(void *)((long)hptr+el_size);
			++i;
		}
		return(i);
	}
	else
	{	
		return(fread(hptr, el_size, count, hdrfp));
	}
}


size_t hdrwrite(long win, void *hptr, size_t count, FILE *hdrfp)
{	long offset, tmpcnt;
	size_t el_size=sizeof(HDR);
	word oldset;
	long i=0L;
	char tmp[MAX_STR], *tmpptr;
	
	tmpptr=hptr;
	tmpcnt=count;
	if (wi[win].ramhdr)
	{

		if (wi[win].hdrptr >= wi[win].hdrbuf+wi[win].hdrlen)
		{
			oldset=(word)(hdrtell(win,hdrfp)/el_size);
			sprintf(tmp, "%s.%s", gl_area[wi[win].aktarea].path, HEADEREXT);
			init_hdrcache(win,tmp);
			hdrseek(win,hdrfp,oldset,SEEK_SET);
		}
		
		if (wi[win].ramhdr)  /* nochmal testen falls nun doch kein speicher frei */
		{
			offset=(long)(wi[win].hdrptr-wi[win].hdrbuf);
			
			while (tmpcnt-- > 0 && (long)wi[win].hdrptr < (long)(wi[win].hdrbuf+wi[win].hdrlen))
			{
				memcpy(wi[win].hdrptr,tmpptr,el_size);
				wi[win].hdrptr+=el_size;
				tmpptr=(char *)((long)tmpptr+el_size);
				i++;
			}
	
			fseek(wi[win].hdrfp,offset,SEEK_SET);
		}
	}
/*   wi[win].hdrlen erh”hen */
/*	wi[win].hdrlen+=(el_size*count);	*/
	
	return(fwrite(hptr,el_size,count,hdrfp));
}

long hdrtell(long win, FILE *hdrfp)
{
	if (wi[win].ramhdr)
		return((long) (wi[win].hdrptr-wi[win].hdrbuf));
	else
		return (ftell(hdrfp));
}



size_t fasthdrseek(WIND *win, FILE *hdrfp, word msg, int mode)
{	long err=0L;
	size_t offset;

	offset=msg*sizeof(HDR);

	if (win->ramhdr && offset<=win->hdrlen && win->hdrbuf!=NULL)
	{
		if (mode==SEEK_SET)
			win->hdrptr = (char *)(win->hdrbuf+offset);
		else
			err=-1L;
		if ( ((HDR *)win->hdrptr)->Size==0 )
			err=-1L;

		return(err);
	}
	else
	{
		return(fseek(hdrfp, offset, mode));
	}
}

size_t fasthdrread(WIND *win, void *hptr, size_t count, FILE *hdrfp)
{	long i=0L;
	size_t el_size=sizeof(HDR);
	
	if (win->ramhdr && win->hdrbuf!=NULL)
	{
		while (count-->0 && (long)win->hdrptr < (long)(win->hdrbuf+win->hdrlen))
		{
			memcpy(hptr,win->hdrptr,el_size);
			win->hdrptr+=el_size;
			hptr=(void *)((long)hptr+el_size);
			++i;
		}
		return(i);
	}
	else
	{	
		return(fread(hptr, el_size, count, hdrfp));
	}
}
