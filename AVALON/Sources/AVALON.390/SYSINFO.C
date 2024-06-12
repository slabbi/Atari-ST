/* MODUL: Systeminfo					 */
/*														 */
/* (c) St.Slabihoud 1994			 */

#include <aes.h>
#include <vdi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>
#include <portab.h>
#include <scancode.h>
#include <my_scan.h>
#if defined( _AVALSEMP_ )
	#include "avalsemp.h"
#else
	#include "avalon.h"
#endif
#include "avallang.h"
#include "define.h"
#include "diverse.h"
#include "version.h"
#include "form_run.h"
#include "windmini.h"
#include "vars.h"

void systeminfo(void);
void outboundinfo(void);

static OUTBOUND *out;
static long	out_counter;
static DTA dta1,dta2;
static int max_outbound;

static char ext[4],typ[80],file[MAX_STR];

/*******************************************************************/
/*******************************************************************/
/*******************************************************************/

void systeminfo(void)
	{	int i,d,wahl=0;
		char temp[MAX_STR],infotext[MAX_STR];
		void *buffer[100];
		long frei,gesamt=0;

		wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
		wind.handle=handle;
		sprintf(infotext," AVALON-SYSTEMINFO (%s) ",mailer==0 ? "Binkley" : "Semper");
		wind_set(wind.handle,WF_NAME,infotext);
		wind_set(wind.handle,WF_CURRXYWH,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);

		wind_update(BEG_UPDATE);
		wind_cls(&wind);
		wind_putch(&wind,27,VT52);
		wind_putch(&wind,'f',VT52);
		wind_mesag(&wind,MU_KEYBD|MU_MESAG|MU_TIMER);

		sprintf(temp,"Sysop %s",txtsysop);
		wind_print(&wind,temp);

		wind_print(&wind,"");
		wind_print(&wind,"Addresses:");
		wind_print(&wind,"---------------------------------------------------");
		for (i=0; i<=address_cnt; i++)
			{	if (fakenet[i]==0)
					sprintf(temp,"%-30s  no fakenet",address[i]);
				else
					sprintf(temp,"%-30s  %u",address[i],fakenet[i]);
				wind_print(&wind,temp);
			}
		wind_print(&wind,"");

		sprintf(temp,"%-20s %s","Domains:","Folders:");
		wind_print(&wind,temp);
		wind_print(&wind,"---------------------------------------------------");
		for (i=0; i<=domain_cnt; i++)
			{	/* Ist es unser Defaultdomain? */
				scanaddr(address[0],&d,&d,&d,&d,temp);
				if (stricmp(domain[i][0],temp)==0)
					sprintf(temp,"%-20s %s  (default domain)",domain[i][0],domain[i][1]);
				else
					sprintf(temp,"%-20s %s",domain[i][0],domain[i][1]);
				wind_print(&wind,temp);
			}
		wind_print(&wind,"");

		do
			{	frei = (long) Malloc(-1L);
				if (frei > 0)
					{	buffer[++wahl] = Malloc(frei);
						gesamt += frei;
					}
			}
		while (frei != 0);
		i = wahl;
		while (i > 0)
			if (Mfree(buffer[i--])!=0)
				form_alert(1,msg[33]);
		sprintf(temp,"Total free memory: %8lu bytes in %2u blocks",gesamt,wahl);
		wind_print(&wind,temp);

		if (MagX && MagX->aesvars && MagX->aesvars->version>=0x200)
			{	sprintf(temp,"MagX %u.%c%c installed.",
												MagX->aesvars->version >> 8,
												'0'+ ((MagX->aesvars->version >> 4) & 0xf),
												'0'+  (MagX->aesvars->version & 0xf) );
				wind_print(&wind,temp);
			}
		if (wind_get(0,'WX',&i,&i,&i,&i)=='WX')
			{	sprintf(temp,"Winx installed.");
				wind_print(&wind,temp);
			}
		sprintf(temp,"AES-Version %u.%c%c.",
										aes_version >> 8,
										'0'+ ((aes_version >> 4) & 0xf),
										'0'+  (aes_version & 0xf) );
		wind_print(&wind,temp);

		wind_update(END_UPDATE);

		wind_pause(&wind);

		wind_set(handle,WF_NAME,AVALONNAME);
		xform_maindialog(XFMD_DIALOG);
	}

/*******************************************************************/
/*******************************************************************/
/*******************************************************************/

static long bytes_in_flowfile(void)
	{ DTA temp_dta;
		FILE *fp;
		char temp[MAX_STR],*pointer;
		long ret=0;

		strcpy(temp,file);				/* aktueller Zugriffspfad mit "\*.*"	*/
		temp[strlen(file)-3]=EOS;	/* "\*.*" entfernen										*/
		strcat(temp,dta2.d_fname);

		Fsetdta(&temp_dta);
		fp=fopen(temp,"r");
		if (fp!=NULL)
			{	while (!feof(fp))
					{	input(temp,MAX_STR-1,fp);
						pointer=temp; trim(&pointer); strcpy(temp,pointer);
						if (*temp=='^' || *temp=='#')
							strcpy(temp,temp+1);
						if (*temp!=EOS && *temp!=';')
							{	if ((pointer=strchr(temp,' '))!=NULL)
									*pointer=EOS;
								if (Fsfirst(temp,0)==0)
									ret+=temp_dta.d_length;
							}
					}
				fclose(fp);
			}
		Fsetdta(&dta2);
		return(ret);
	}

static void out_dat(char *netz,int zone)
	{	int valid=0,filetyp;
		if (++out_counter>max_outbound-1)
			out_counter=max_outbound-1;

		out[out_counter].zone	 = zone;
		out[out_counter].net	 =
		out[out_counter].node	 =
		out[out_counter].point = 0;

		strncpy(ext,dta2.d_fname+9,3);
		switch(*ext)
			{	case 'F'	:
				case 'O'	: strcpy(typ,"Normal"	); break;
				case 'H'	: strcpy(typ,"Hold"		); break;
				case 'D'	:	strcpy(typ,"Direct"	); break;
				case 'C'	: strcpy(typ,"Crash"	); break;
				case '$'	:
				case 'R'	:
				case 'T'	:
				case 'Z'	:	*typ=EOS; break;
				default		: {	strcpy(typ,"Unknown" );
											valid++;
										}
			}

		if (stricmp(ext,"REQ")==0)
			{	strcat(typ,"Normal request");
				file2addr  (dta2.d_fname,&out[out_counter].net,&out[out_counter].node);
			}
		else if (stricmp(ext,"TIC")==0)
			{	strcpy(typ,"TIC file");
				out[out_counter].zone=-1;
			}
		else if (stricmp(ext,"TMP")==0)
			{	strcpy(typ,"Temporary file");
				out[out_counter].zone=-1;
			}
		else if (strnicmp(ext,"$$",2)==0)
			{	strcpy(typ,"Binkley flag");
				if (divflags & D_USE4D)
					file2addr36(dta2.d_fname,&out[out_counter].net,&out[out_counter].node,&out[out_counter].point);
				else
					file2addr  (dta2.d_fname,&out[out_counter].net,&out[out_counter].node);
			}
		else if (stricmp(ext,"Z"  )==0)
			{	strcpy(typ,"BadWazoo file");
				if (divflags & D_USE4D)
					file2addr36(dta2.d_fname,&out[out_counter].net,&out[out_counter].node,&out[out_counter].point);
				else
					file2addr  (dta2.d_fname,&out[out_counter].net,&out[out_counter].node);
			}
		else
			{	filetyp = (ext[1] << 8) | (ext[2]);
				switch(filetyp)
					{	case 'UT'	:	strcat(typ," unpacked mail");	file2addr	 (dta2.d_fname,&out[out_counter].net,&out[out_counter].node); break;
						case 'LO'	:	strcat(typ," flowfile");			file2addr	 (dta2.d_fname,&out[out_counter].net,&out[out_counter].node); break;

						case 'PT'	:	strcat(typ," unpacked mail");	file2addr36(dta2.d_fname,&out[out_counter].net,&out[out_counter].node,&out[out_counter].point); break;
						case 'AT'	:	strcat(typ," packed mail");		file2addr36(dta2.d_fname,&out[out_counter].net,&out[out_counter].node,&out[out_counter].point); break;
						case 'FT'	:	strcat(typ," flowfile");			file2addr36(dta2.d_fname,&out[out_counter].net,&out[out_counter].node,&out[out_counter].point); break;
						case 'RT'	:	strcat(typ," request");				file2addr36(dta2.d_fname,&out[out_counter].net,&out[out_counter].node,&out[out_counter].point); break;

						default		: {	strcpy(typ,"Unknown");
													valid++;
												}
					}
			}
		if ( valid!=0 )
			out[out_counter].zone=-1;
		strcpy(out[out_counter].filename,dta2.d_fname);
		strcpy(out[out_counter].netz,netz);
		strcpy(out[out_counter].typ,typ);
		out[out_counter].size = dta2.d_length;
		if (filetyp=='LO' || filetyp=='FT')
			{	out_counter++;
				strcpy(out[out_counter].filename,out[out_counter-1].filename);
				strcpy(out[out_counter].netz,out[out_counter-1].netz);
				strcpy(out[out_counter].typ,"Bytes in flowfile");
				out[out_counter].zone=out[out_counter-1].zone;
				out[out_counter].net=out[out_counter-1].net;
				out[out_counter].node=out[out_counter-1].node;
				out[out_counter].point=out[out_counter-1].point;
				out[out_counter].size = bytes_in_flowfile();
			}
	}

static void out_teste(char *transfer,char *ordner, char *netz, int zone)
	{	Fsetdta(&dta2);
		sprintf(file,"%s%s\\*.*",transfer,ordner);
		if ( Fsfirst(file,0)==0 )
			{	out_dat(netz,zone);
				while ( Fsnext()==0 )
					out_dat(netz,zone);
			}
		Fsetdta(&dta1);
	}

static void collect_outbound(void)
	{	char	transfer[PATH_MAX],*pointer,
					pfad[PATH_MAX],ordner[20],altdomain[20];
		int		zone,i,dc,defzone;

		scanaddr(address[0],&defzone,&i,&i,&i,pfad);
		out_counter = 0;

		Fsetdta(&dta1);
		strcpy(transfer,hold);
		if (*(transfer+strlen(transfer)-1)=='\\')
			*(transfer+strlen(transfer)-1) = EOS;

		pointer = strrchr(transfer,'\\');
		if (pointer!=NULL)
			{	if (domain_cnt==-1)								/* Es gibt KEIN Domain	*/
					{	*domain[0][0] = EOS;
						strcpy(domain[0][1],pointer+1);
					}
				else					/* Default-Domain-Name wird OUTBOUND-Ordner */
					{	strcpy(altdomain,domain[0][1]);
						strcpy(domain[0][1],pointer+1);
						*(pointer+1) = EOS;
					}
			}

		dc = domain_cnt;
		if (dc==-1)
			dc=0;

		for (i=0; i<=dc; i++)
			{	sprintf(pfad,"%s%s.*",transfer,domain[i][1]);

				if ( Fsfirst(pfad,FA_SUBDIR)==0 )
					{	strcpy(ordner,dta1.d_fname);
						pointer = strrchr(ordner,'.');
						if (pointer!=NULL)
						{	if (divflags & D_USEOUT36)
								zone=(int)strtoul(pointer+1,NULL,36);
							else
								zone=(int)strtoul(pointer+1,NULL,16);
						}
						else
						{	zone=defzone;
						}
						out_teste(transfer,ordner,domain[i][0],zone);
						
						while ( Fsnext()==0 )
							{	strcpy(ordner,dta1.d_fname);
								pointer = strrchr(ordner,'.');
								if (pointer!=NULL)
								{	if (divflags & D_USEOUT36)
										zone=(int)strtoul(pointer+1,NULL,36);
									else
										zone=(int)strtoul(pointer+1,NULL,16);
								}
								else
								{	zone=defzone;
								}
								out_teste(transfer,ordner,domain[i][0],zone);
							}
					}
			}
		if (domain_cnt>=0)
			strcpy(domain[0][1],altdomain);			/* Domain restaurieren */
	}

static int out_cmp(OUTBOUND *e1, OUTBOUND *e2)
	{	if (e1->zone < e2->zone)
			return(-1);
		else if (e1->zone > e2->zone)
			return(1);
		else if (e1->net < e2->net)
			return(-1);
		else if (e1->net > e2->net)
			return(1);
		else if (e1->node < e2->node)
			return(-1);
		else if (e1->node > e2->node)
			return(1);
		else if (e1->point < e2->point)
			return(-1);
		else if (e1->point > e2->point)
			return(1);
		else if (e1->size < e2->size)
			return(-1);
		else if (e1->size > e2->size)
			return(1);
		return(0);
	}

void outboundinfo(void)
	{	int i,line=0;
		long t,sum_bytes;
		char infotext[MAX_STR],temp[MAX_STR],temp2[MAX_STR];

		t = (((long)Malloc(-1L))-65536L)/sizeof(*out);
		if (t>MAXOUTBOUND) t=MAXOUTBOUND;
		if (t<MINOUTBOUND) t=MINOUTBOUND;
		max_outbound = (int) t;
		out = (OUTBOUND *) calloc(sizeof(*out), max_outbound+2);

		if (out==NULL)
			{	memory_error();
				return;
			}

		wind_init_full(&wind,vdi_handle,NAME|CLOSER|MOVER|FULLER,fontpnt,BOOLEAN(divflags & D_CENTER),BOOLEAN(divflags & D_SNAP),TRUE,wnx,wny,wnw,wnh);
		wind.handle=handle;
		sprintf(infotext," AVALON-OUTBOUNDINFO ");
		wind_set(wind.handle,WF_NAME,infotext);
		wind_set(wind.handle,WF_CURRXYWH,wind.b[0],wind.b[1],wind.b[2],wind.b[3]);

		wind_update(BEG_UPDATE);
		wind_cls(&wind);
		wind_putch(&wind,27,VT52);
		wind_putch(&wind,'f',VT52);
		wind_mesag(&wind,MU_KEYBD|MU_MESAG|MU_TIMER);

		collect_outbound();

		qsort(&out[1],out_counter,sizeof(*out),out_cmp);

		sum_bytes=
		i=0;
		for (t=1; t<=out_counter; t++)
			{	mergeaddr(temp2,out[t].zone,out[t].net,out[t].node,out[t].point,out[t].netz);
				sprintf(temp,"%-12s   %7lu   %-30s  %s",
														out[t].filename,
														out[t].size,
														out[t].zone!=-1 ? temp2 : "",
														out[t].typ);
				sum_bytes+=out[t].size;
				wind_print(&wind,temp);
				if (++line>=wind.max_y-2)
					{	wind_print(&wind,"");
						wind_print(&wind,"--- press SPACE to continue (UNDO to exit) ---");
						do
							{	i = wind_pause(&wind);
							}
						while (i != KEY_SPACE && i!=UNDO );
						if ( i==UNDO )
							{	t=(int) (out_counter+1);
							}
						else
							{	wind_print(&wind,"");
								wind_print(&wind,"");
							}
						line=0;
					}
			}
		memset(temp,'-',80); temp[80]=EOS;
		wind_print(&wind,temp);
		sprintf(temp," %5lu files   %7lu bytes",out_counter,sum_bytes);
		wind_print(&wind,temp);

		wind_update(END_UPDATE);

		if ( i!=UNDO )
			wind_pause(&wind);

		if (out!=NULL)
			free((OUTBOUND *) &out[0]);

		wind_set(handle,WF_NAME,AVALONNAME);
		xform_maindialog(XFMD_DIALOG);
	}
