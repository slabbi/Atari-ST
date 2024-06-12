/*
**			 20.08.94 - Anfang des Projekts
** V0.xx					- Diverse Testversionen in GFA-BASIC
** V1.00 20.08.94 - Erste lauffÑhige C-Version
** V1.10 28.08.94 - "Empfangen (einzeln ASCII)" implementiert,
**									Empfang sehr viel schneller,
**									XON/XOFF-Protokoll wird beachtet
** V1.20 21.09.94 - REMINDER Åber SCHEDULE implementiert.
** V1.25 09.11.94 - ICFS implementiert.
** V1.30 09.02.95 - 1N2 DatenÅbertragung
** V1.35 19.11.95 - FreeFile, Weekly, ToDo wird erkannt.
** V1.36 26.11.96 - "break;" in ASCII-"switch" vergessen :-(
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include <tos.h>
#include <aes.h>
#include <vdi.h>
#include <ctype.h>
#include <screen.h>
#include <portab.h>
#include <scancode.h>
#include "define.h"
#include "vars.h"
#include "form_run.h"
#include "diverse.h"
#include "casio_sf.h"
#include "casio_sf.rh"
#include "transkey.h"

#define VERSION "136"

int  wnx,wny,wnw,wnh,handle;
typedef struct DATAtag
	{	char	data[128];
	} DATA;
DATA data[15];
long SWAP(long value) 0x4840;

void set_rs232s(void);
void set_rs232e(void);
int	 out(int chr);
int  inp(int dev);						/* Zeichen */
int  askinp(int dev);					/* T/F		 */
void clearbuffer(void);
int  wait4ack(void);					/* 0=ESC,33,34,35,36 */
int  showack(int x);					/* TRUE=Weiter, FALSE=Abbruch durch Fehler */
int  init(void);							/* TRUE=OK, FALSE=ESC */
void send(char *txt);
char *convert(char *txt);			/* ASCII->HEX */
void addchecksum(char *txt);	/* Calc Checksum */
int  senddata(DATA *data,int marked);

int  getbyte(void);						/* -1=ESC */
int  wait4header(void);				/* -1=ESC */
char *decode(char *text,int len);
void umlaute(char *txt);

int  teste_ob_key_ok(char *name,long key);
void doregister(void);
void ascii_recv(void);
void cal_send(void);
void rem_send(void);
void rem2_send(void);
void tel_send(void);
void bus_adr_send(OBJECT *tree,int felder,int ffirst,int fmark,
                  int fexit,int fsend,char *init,int (*trans_key)(int,int));
											/* tree7,15 oder tree8,9 */
void term_send(void);
void memo_send(void);
void bereiche_recv(void);
void backup(void);
void restore(void);
void buttonhell(void);
int  ascii2hex(int x);
void setze_baud_ver(void);
void doconfig(void);

extern long dtr_on_modem1(void);
extern long dtr_on_modem2(void);
extern long dtr_on_serial2(void);
extern long dtr_off_modem1(void);
extern long dtr_off_modem2(void);
extern long dtr_off_serial2(void);
extern long get_hardware(void);
extern int HARDWARE;

int main(void)
	{	int wahl;

		if ((ap_id=appl_init()) < 0)
			{	form_alert(1,"[3][ AES: appl_init failed |][ Exit ]");
				exit(1);
			}
		init_vars();
		buttonhell();
		setze_baud_ver();

		if (xform_dial(NULL,XFMD_INIT,&handle,&wnx,&wny,&wnw,&wnh)==FALSE)
			{	form_alert(1,"[3][ AES: wind_create/delete failed |][ Exit ]");
				v_clsvwk(vdi_handle);
				appl_exit();
				exit(1);
			}
		xform_dial(tree1,XFMD_START,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	wahl=xform_do(handle,tree1,NULL,0,0,trans_key1,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=EXITPRG;
				wahl &= 0x0fff;
				undo_state(tree1,wahl,SELECTED);
				redraw_objc(tree1,wahl,handle);

				switch(wahl)
					{	case	EMPFE		:	bereiche_recv();
														break;
						case	EMPFASCI:	ascii_recv();
														break;
						case	EMPFK		:	backup();
														break;
						case	SENDALL	:	restore();
														break;
						case	SENDTEL	:	if (vollversion)
															tel_send();
														else
															form_alert(1,"[3][Nur fÅr registrierte Anwender |][  OK  ]");
														break;
						case	SENDCAL	:	if (vollversion)
															cal_send();
														else
															form_alert(1,"[3][Nur fÅr registrierte Anwender |][  OK  ]");
														break;
						case	SENDREM	:	rem_send();
														break;
						case	SENDREM2:	if (vollversion)
															rem2_send();
														else
															form_alert(1,"[3][Nur fÅr registrierte Anwender |][  OK  ]");
														break;
						case	SENDTERM:	term_send();
														break;
						case	SENDMEMO:	memo_send();
														break;
						case	SENDBUS	:	if (vollversion)
															bus_adr_send(tree7,15,BNAME,BMARKON,BEXIT,BSEND,":02000002C0003C",trans_key5);
														else
															form_alert(1,"[3][Nur fÅr registrierte Anwender |][  OK  ]");
														break;
						case	SENDADR	:	bus_adr_send(tree8, 9,ANAME,AMARKON,AEXIT,ASEND,":0200000290006C",trans_key6);
														break;
						case	REGISTER:	doregister();
														break;
						case	CONFIGUR:	doconfig();
														break;
					}
			}
		while (wahl!=EXITPRG);

		xform_dial(NULL,XFMD_FINISH,&handle,&wnx,&wny,&wnw,&wnh);
		xform_dial(NULL,XFMD_EXIT,&handle,&wnx,&wny,&wnw,&wnh);
		v_clsvwk(vdi_handle);
		appl_exit();
		return 0;
	}

void buttonhell(void)
	{	if (vollversion)
			undo_xstate(tree1,DISABLED,SENDCAL,SENDTERM,SENDBUS,SENDTEL,-1);
		else
			do_xstate(tree1,DISABLED,SENDCAL,SENDTERM,SENDBUS,SENDTEL,-1);
	}

void setze_baud_ver(void)
	{	char temp[16];
		strcpy(temp,VERSION);
		switch(sbaudcode)
			{	case	0	:	strcat(temp,"19k2"); break;
				case	1	:	strcat(temp,"9600"); break;
				case	2	: strcat(temp,"4800"); break;
				case	4 : strcat(temp,"2400"); break;
				case	7	: strcat(temp,"1200");
			}
		switch(ebaudcode)
			{	case	0	:	strcat(temp,"19k2"); break;
				case	1	:	strcat(temp,"9600"); break;
				case	2	: strcat(temp,"4800"); break;
				case	4 : strcat(temp,"2400"); break;
				case	7	: strcat(temp,"1200");
			}
		put_text(tree1,MAINVERS,temp,11);
	}

int ascii2hex(int x)
	{	if (x>='A') return(x-55);
		else				return(x-48);
	}
/*******************************************************************/

int teste_ob_key_ok(char *name,long key)
	{	
			return TRUE;
	}

void doregister(void)
	{	char temp[MAX_STR];
		int wahl;
		FILE *fout;

		if (!(*sysopname) || sysopkey==-1 )
			{	put_text(tree10,REGINFO,"Namen und Key eingeben",30);
				put_text(tree10,REGNAME,"",40);
				put_text(tree10,REGKEY,"",8);
			}
		else
			{	put_text(tree10,REGNAME,sysopname,40);
				sprintf(temp,"%08lX",sysopkey);
				put_text(tree10,REGKEY,temp,8);
				if (teste_ob_key_ok(sysopname,sysopkey))
					{	put_text(tree10,REGINFO,"Key OK - Version registriert",30);
						vollversion=TRUE;
					}
				else
					{	put_text(tree10,REGINFO,"UngÅltiger Key!",30);
						vollversion=FALSE;
					}
			}
		xform_dial(tree10,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	do
					{	wahl=xform_do(handle,tree10,NULL,0,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
						if (wahl & 0x4000) wahl=REGEXIT;
						wahl &= 0x0fff;
						undo_state(tree10,wahl,SELECTED);
						redraw_objc(tree10,wahl,handle);
					}
				while (wahl==REGSAVE && (!(*get_text(tree10,REGNAME)) || strlen(get_text(tree10,REGKEY))!=8) );
				if (wahl==REGSAVE)
					{	strcpy(sysopname,get_text(tree10,REGNAME));
						strcpy(temp,get_text(tree10,REGKEY));
						sysopkey=strtoul(temp,NULL,16);
						if (teste_ob_key_ok(sysopname,sysopkey))
							{	put_text(tree10,REGINFO,"Key OK - Version registriert",30);
								vollversion=TRUE;
							}
						else
							{	put_text(tree10,REGINFO,"UngÅltiger Key!",30);
								vollversion=FALSE;
							}
						redraw_objc(tree10,REGINFO,handle);
						sprintf(temp,"%s\\CASIO_SF.KEY",def_path);
						Dsetdrv(def_drive);
						fout=fopen(temp,"w");
						output(sysopname,fout);
						sprintf(temp,"%08lX",sysopkey);
						output(temp,fout);
						fclose(fout);
					}
			}
		while (wahl!=REGEXIT);
		buttonhell();
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

void doconfig(void)
	{	char temp[MAX_STR];
		int wahl;
		FILE *fout;

		Supexec(get_hardware);
		switch(HARDWARE)
			{	case	0	:	/* ST		*/
									do_xstate(tree11,DISABLED,PORT2,PORT3,PORT4,-1);
									break;
				case	1	:	/* STE	*/
									do_xstate(tree11,DISABLED,PORT3,-1);
									break;
				case	2	:	/* TT		*/
									break;
				case	3	:	/* F030	*/
									do_xstate(tree11,DISABLED,PORT1,PORT3,PORT4,-1);
									break;
			}
		undo_xstate(tree11,SELECTED,E1200,E2400,E4800,E9600,E19200,
																S1200,S2400,S4800,S9600,S19200,
																PORT0,PORT1,PORT2,PORT3,PORT4,
																SUMLAUTE,-1);
		switch(ebaudcode)
			{	case	0	:	do_state(tree11,E19200,SELECTED); break;
				case	1	:	do_state(tree11, E9600,SELECTED); break;
				case	2	:	do_state(tree11, E4800,SELECTED); break;
				case	4	:	do_state(tree11, E2400,SELECTED); break;
				case	7	:	do_state(tree11, E1200,SELECTED);
			}
		switch(sbaudcode)
			{	case	0	:	do_state(tree11,S19200,SELECTED); break;
				case	1	:	do_state(tree11, S9600,SELECTED); break;
				case	2	:	do_state(tree11, S4800,SELECTED); break;
				case	4	:	do_state(tree11, S2400,SELECTED); break;
				case	7	:	do_state(tree11, S1200,SELECTED);
			}
		if (!(Bconmap(0)==0))
			{	port=0;
				undo_xstate(tree11,DISABLED,PORT2,PORT3,PORT4,-1);
			}
		do_state(tree11,PORT0+port,SELECTED);
		if (konvert)
			do_state(tree11,SUMLAUTE,SELECTED);

		xform_dial(tree11,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		do
			{	wahl=xform_do(handle,tree11,NULL,0,0,NULLFUNC,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=COK;
				wahl &= 0x0fff;
				undo_state(tree11,wahl,SELECTED);
				redraw_objc(tree11,wahl,handle);

						 if (is_state(tree11, E1200,SELECTED)) ebaudcode=7;
				else if (is_state(tree11, E2400,SELECTED)) ebaudcode=4;
				else if (is_state(tree11, E4800,SELECTED)) ebaudcode=2;
				else if (is_state(tree11, E9600,SELECTED)) ebaudcode=1;
				else if (is_state(tree11,E19200,SELECTED)) ebaudcode=0;

						 if (is_state(tree11, S1200,SELECTED)) sbaudcode=7;
				else if (is_state(tree11, S2400,SELECTED)) sbaudcode=4;
				else if (is_state(tree11, S4800,SELECTED)) sbaudcode=2;
				else if (is_state(tree11, S9600,SELECTED)) sbaudcode=1;
				else if (is_state(tree11,S19200,SELECTED)) sbaudcode=0;
				
						 if (is_state(tree11,PORT0,SELECTED)) port=0;
				else if (is_state(tree11,PORT1,SELECTED)) port=1;
				else if (is_state(tree11,PORT2,SELECTED)) port=2;
				else if (is_state(tree11,PORT3,SELECTED)) port=3;
				else if (is_state(tree11,PORT4,SELECTED)) port=4;

				konvert=FALSE;
				if (is_state(tree11,SUMLAUTE,SELECTED)) konvert=TRUE;

				if (wahl==CSAVE)
					{	sprintf(temp,"%s\\CASIO_SF.INF",def_path);
						Dsetdrv(def_drive);
						fout=fopen(temp,"w");
						sprintf(temp,"%u",ebaudcode);	output(temp,fout);
						sprintf(temp,"%u",sbaudcode);	output(temp,fout);
						sprintf(temp,"%u",     port);	output(temp,fout);
						sprintf(temp,"%u",	konvert);	output(temp,fout);
						fclose(fout);
					}
			}
		while (wahl!=COK);
		setze_baud_ver();
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void set_rs232e(void)
	{	if (port!=0)
			if (Bconmap(0)==0)
				{	switch(port)
						{	case	1	: Bconmap(6); Supexec(dtr_on_modem1);		break;
							case	2	: Bconmap(7); Supexec(dtr_on_modem2);		break;
							case	3	: Bconmap(8); break;
							case	4	: Bconmap(9); Supexec(dtr_on_serial2);	break;
						}
				}
		Rsconf(ebaudcode,1,8|16|128,-1,-1,-1);		/* XON/XOFF benutzen */
		clearbuffer();
	}
void set_rs232s(void)
	{	if (port!=0)
			if (Bconmap(0)==0)
				{	switch(port)
						{	case	1	: Bconmap(6); Supexec(dtr_on_modem1);		break;
							case	2	: Bconmap(7); Supexec(dtr_on_modem2);		break;
							case	3	: Bconmap(8); break;
							case	4	: Bconmap(9); Supexec(dtr_on_serial2);	break;
						}
				}
		Rsconf(sbaudcode,1,8|16|128,-1,-1,-1);		/* XON/XOFF benutzen */
		clearbuffer();
	}
int out(int chr)			/* Ein Zeichen senden */
	{	for(;;)
			{	if (Bcostat(1))
					{	Bconout(1,chr);
						return(TRUE);
					}
				if (askinp(2))
					if (inp(2)==27)
						return(FALSE);
			}
	}

/* int inp(int dev)
	{	return (int) Bconin(dev) & 0xff;
	} */

int inp(int dev)
	{ int which,d,kr,tophandle,formrun_msgbuf[8];
		if (dev==2)
			{	which = evnt_multi(MU_KEYBD|MU_TIMER,2,1,1,0,0,0,0,0,0,0,0,0,0,formrun_msgbuf,
    		    									 (int) (1 & 0xffffL),(int) ((1 & 0xffff0000L) >> 16),
        											 &d,&d,&d,&d,&kr,&d);
				wind_get(handle,WF_TOP,&tophandle,0,0,0);
	  		if ((which & MU_KEYBD) && (handle==tophandle) )
			  	return(kr & 0xff);
				return(0);
			}
/*	if (which & MU_TIMER) */
		return (int) Bconin(dev) & 0xff;
	}

int askinp(int dev)		/* Ist ein Zeichen vorhanden? */
	{	return (int) Bconstat(dev);
	}
void clearbuffer(void)
	{	while(askinp(1))
			inp(1);
	}
int wait4ack(void)
	{	int c=-1;
		do
			{	if (askinp(1))
					c=inp(1);
				if (c==63)
					{	if (form_alert(1,"[3][CRC Fehler im Header |][Nochmal|Abbruch]")==2)
							c=0;		/* Abbruch */
						else
							c=-1;		/* Was tun? Erst 'mal in der Schleife bleiben */
					}
				if (askinp(2))
					if (inp(2)==27)
						c=0;
			}
		while (c!=33 && c!=34 && c!=35 && c!=36 && c!=0);
		return c;
	}
int showack(int x)
	{ switch(x)
			{	case	0	:	form_alert(1,"[1][Abbruch! ][  OK  ]");
									return FALSE;
				case 33	:	form_alert(1,"[3][Allgemeiner Fehler |(Abbruch oder Speicher voll?) |][  OK  ]");
									return FALSE;
				case 34	:	form_alert(1,"[3][CRC Fehler |][  OK  ]");
									return FALSE;
				case 35	:	return TRUE;
				case 36	:	form_alert(1,"[3][Funktion vom CASIO nicht |unterstÅtzt! |][  OK  ]");
									return FALSE;
				default	:	form_alert(1,"[3][Unbekannter Fehler #1# |][  OK  ]");
									return FALSE;
			}
	}
int init(void)	/* Casio zum Empfang auffordern */
	{	out(13);
		out(10);
		delay(10);
		out(13);
		out(10);
		delay(40);	/* Kurze Verzîgerung */
		return TRUE;
	}
void send(char *txt)
	{	while (*txt)
			out(*txt++);
	}
char *convert(char *txt)	/* ASCII-Text -> Intel-HEX */
	{	static char buffer[1024],*pointer;
		pointer=buffer;
		while (*txt)
			{	if (*txt==158) *txt=225;
				sprintf(pointer,"%02X",*txt++);
				pointer+=2;
			}
		*pointer++=EOS;
		*pointer=EOS;
		return buffer;
	}
void addchecksum(char *txt)	/* Aus Intel-HEX (":xxxx") Checksum bilden */
	{	int sum=0,digit;
		txt++;
		while (*txt)
			{	digit=*txt++;
				digit=ascii2hex(digit);
			  sum -= digit << 4;
				digit=*txt++;
				digit=ascii2hex(digit);
			  sum -= digit;
			}
		sprintf(txt,"%02X",sum & 0xff);
	}
int getbyte(void)		/* Ein Byte lesen */
	{ for (;;)
			{	if (askinp(1))
					return inp(1);
				if (askinp(2))
					if (inp(2)==27)
						return -1;
			}
	}
int wait4header(void)		/* Empfang: Auf 13d,10d warten */
	{	int chr;
		for (;;)
			{	if (askinp(1))
					{	chr=inp(1);
						if (chr==0x0a)
							return TRUE;
						if (chr==0x21)	/* ESC am CASIO */
							return FALSE;
					}
				if (askinp(2))
					{	if (inp(2)==27)
							return FALSE;
					}
			}
	}
char *decode(char *text,int len)	/* Intel-Hex -> ASCII-String, CR und LF unterdrÅcken */
	{	static char temp[512],*pointer;
		int digit,chr;
		pointer=temp;
		while (*text && len>0)
			{	digit=*text++;
				digit=ascii2hex(digit);
			  chr = digit << 4;
				digit=*text++;
				digit=ascii2hex(digit);
				chr += digit;
				if (chr!=13 && chr!=10)
					*pointer++=chr;
				len-=2;
			}
		*pointer=EOS;
		return(temp);
	}
char *decodedays(char *text,int len)	/* Tage aus Intel-HEX bestimmen */
	{	static char buffer[128];
		long days=0;
		int i,pos=0,digit;
		for (i=1; i<=len; i++)
			{	days=days << 4;
				digit=*text++;
				digit=ascii2hex(digit);
			  days |= digit & 0xf;
			}
		for (i=0; i<=31; i++)
			{	if (days & (1UL << i))
					sprintf(buffer+(pos++)*3,"%02u ",i+1);
			}
		return(buffer);
	}
int getheaderblock(char *buffer,int *length) /* Einen kompletten Block lesen */
	{	int chr,len,i,checksum,digit;
		char *pointer;
		do
			{	chr=getbyte();
				if (chr==0x21)	/* ESC am Casio */
					return(0x21);
				if (chr==13)		/* 13, dann folgt noch eine 10 -> Resync */
					{	if (wait4header()==FALSE)
							return(-1);
						out(0x11);	/* XON */
					}
			}
		while (chr!=':');
		buffer[0]=':';
		chr=getbyte(); buffer[1]=chr; if (chr==-1 || chr==0x21) return(chr);
		chr=getbyte(); buffer[2]=chr; if (chr==-1 || chr==0x21) return(chr);
		buffer[3]=EOS;
		len=(int) strtoul(buffer+1,NULL,16) << 1;
		*length=len;
		for (i=3; i<=8; i++)
			{	chr=getbyte(); buffer[i]=chr; if (chr==-1 || chr==0x21) return(chr);
			}
		if (len>0)
			{	for (i=9; i<=len+9-1; i++)
					{	chr=getbyte(); buffer[i]=chr; if (chr==-1 || chr==0x21) return(chr);
					}
			}
		else
			{	i=9;
			}
		chr=getbyte(); buffer[i++]=chr; if (chr==-1 || chr==0x21) return(chr);
		chr=getbyte(); buffer[i++]=chr; if (chr==-1 || chr==0x21) return(chr);
		buffer[i]=EOS;
		checksum=(int) strtoul(buffer+i-2,NULL,16);

		chr=0;
		pointer=buffer+1;
		for (i=1; i<=((len+8) >> 1); i++)
			{	digit=*pointer++;
				digit=ascii2hex(digit);
				chr-=digit << 4;
				digit=*pointer++;
				digit=ascii2hex(digit);
				chr-=digit;
			}
		if (checksum!=(char)chr)
			return(-2);

		out(0x23);	/* OK */
		return TRUE;
	}
void umlaute(char *txt)
	{	char *pointer,temp[1024];
		pointer=temp;
		while (*txt)
			{	switch(*txt)
					{	case	'é'	:	*pointer++='A'; *pointer++='e'; break;
						case	'ô'	:	*pointer++='O'; *pointer++='e'; break;
						case	'ö'	:	*pointer++='U'; *pointer++='e'; break;
						case	'Ñ'	:	*pointer++='a'; *pointer++='e'; break;
						case	'î'	:	*pointer++='o'; *pointer++='e'; break;
						case	'Å'	:	*pointer++='u'; *pointer++='e'; break;
						case	158	:
						case	225	:	*pointer++='s'; *pointer++='s';	break;
						default		: *pointer++=*txt;
					}
				txt++;
			}
		*pointer=EOS;
		strcpy(txt,temp);
	}

/*******************************************************************/

void tel_send(void)
	{	FILE	*fin;
		char	temp[MAX_STR],in_path[PATH_MAX],sel_file[13]="";
		int		wahl,len,ok,nonstop=FALSE,i,ignoreempty=TRUE;
		for (i=0; i<=8; i++)
			*data[i].data=EOS;

		set_rs232s();

		do_state(tree2,IGNOON,SELECTED);
		undo_state(tree2,MARKON,SELECTED);
		put_text(tree2,INFOSTAT,"",50);
		for (i=0; i<=8; i++)
			put_string(tree2,LINE1+i,"",65);
		for (i=IMP4; i<=IMP9; i++)
			do_state(tree2,i,SELECTED);

		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");
		
		if (fileselect(in_path,sel_file,"*.*","Welche Datei senden?")==0)
			return;
		strcat(in_path, sel_file);
		if (exist(in_path)!=0)
			{	form_alert(1,"[3][Datei nicht gefunden! |][  OK  ]");
				return;
			}

		xform_dial(tree2,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		xform_do(handle,tree2,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

		put_text(tree2,INFOSTAT,"Synchronisiere",50); redraw_objc(tree2,INFOSTAT,handle);
		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				return;
			}
		put_text(tree2,INFOSTAT,"",50); redraw_objc(tree2,INFOSTAT,handle);

		fin=fopen(in_path,"r");
		send(":0200000290006C");

		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				fclose(fin);
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				return;
			}

		while (!feof(fin))
			{
				ok=TRUE;
				do
					{
						input(temp,MAX_STR,fin);	/* Name			*/
sprung:			if (feof(fin)) break;
						strcpy(data[0].data,temp);
					if (!(*data[0].data)) ok=FALSE;
						input(temp,MAX_STR,fin);	/* Vorname	*/
						if (feof(fin)) break;
						if (*temp)
							{	strcat(data[0].data,", ");
								strcat(data[0].data,temp);
							}

						input(temp,MAX_STR,fin);	/* Telephon	*/
						if (feof(fin)) break;
						strcpy(data[1].data,temp);

						input(temp,MAX_STR,fin);	/* Straûe		*/
						if (feof(fin)) break;
						strcpy(data[2].data,temp);
						input(temp,MAX_STR,fin);	/* Wohnort	*/
						if (feof(fin)) break;
						if (*temp)
							{	strcat(data[2].data,", ");
								strcat(data[2].data,temp);
							}

						input(temp,MAX_STR,fin);	/* GebDatum	*/
						if (feof(fin)) break;
						if (strnicmp(temp,"00/00/00",8)==0)
							*temp=EOS;
						strcpy(data[3].data,temp);

						for (i=4; i<=8; i++)
							{	input(temp,MAX_STR,fin);	/* Frei 2-6	*/
								if (feof(fin)) break;
								strcpy(data[i].data,temp);
							}
						if (feof(fin)) break;

						input(temp,MAX_STR,fin);	/* EOD			*/
						if (*data[1].data==EOS && *data[2].data==EOS) ok=FALSE;
					}
				while (!ok);

				if (feof(fin)) break;

				for (i=0; i<=8; i++)
					{	sprintf(temp,"%u. %s",i+1,data[i].data);
						put_string(tree2,LINE1+i,temp,65);
					}
				redraw_objc(tree2,DATABOX,handle);
				if (!nonstop)
					{	wahl=xform_do(handle,tree2,NULL,0,0,trans_key2,&wnx,&wny,&wnw,&wnh);
						if (wahl & 0x4000) wahl=LINEEXIT;
						wahl &= 0xfff;
						undo_state(tree2,wahl,SELECTED);
						if (wahl!=LINERUN)
							redraw_objc(tree2,wahl,handle);
					}
				else
					{	wahl=LINESEND;
						xform_do(handle,tree2,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
					}

				for (i=IMP4; i<=IMP9; i++)
					{	if (!is_state(tree2,i,SELECTED))
							*data[i-IMP4+3].data=EOS;
					}

				if (is_state(tree2,IGNOON,SELECTED)) ignoreempty=TRUE;
				else																 ignoreempty=FALSE;

				if (ignoreempty)
					{	for (i=4; i<=7; i++)
							{	if (*data[i].data==EOS)
									{	for (len=i+1; len<=8; len++)
											strcpy(data[len-1].data,data[len].data);
										*data[8].data=EOS;
									}
							}
					}

				for (i=8; i>=1; i--)
					if (*data[i].data!=EOS)	strcat(data[i-1].data,"\n");

				if (wahl==LINERUN)
					{	wahl=LINESEND;
						nonstop=TRUE;
					}

				if (wahl==LINELOOK)
					{ int x1,y1,w1,h1;
						form_center(tree4,&x1,&y1,&w1,&h1);
						put_text(tree4,SEARCH,"",10);
						form_dial(FMD_START,mid_x,mid_y,1,1,x1,y1,w1,h1);
						form_dial(FMD_GROW,mid_x,mid_y,1,1,x1,y1,w1,h1);
						objc_draw(tree4,ROOT,MAX_DEPTH,x1,y1,w1,h1);
						i=form_do(tree4,0) & 0x7FFF;
						undo_state(tree4,i,SELECTED);
						form_dial(FMD_SHRINK,mid_x,mid_y,1,1,x1,y1,w1,h1);
						form_dial(FMD_FINISH,mid_x,mid_y,1,1,x1,y1,w1,h1);
						for (;;)
							{	input(temp,MAX_STR,fin);	/* Name			*/
								if (feof(fin))
									{	wahl=LINEEXIT;
										break;
									}
								len=(int) strlen(get_text(tree4,SEARCH));
								if (strnicmp(temp,get_text(tree4,SEARCH),len)==0) goto sprung;
								for (i=2; i<=12; i++)
									input(temp,MAX_STR,fin);	/* Vorname,Tel,Str,Ort,GebDat,EOD	*/
							}
					}

				if (wahl==LINEEXIT)
					break;

				if (wahl==LINESEND)
					if (senddata(data,is_state(tree2,MARKON,SELECTED) ? TRUE : FALSE)==FALSE)
						{	put_text(tree2,INFOSTAT,"öbertragungsfehler",50); redraw_objc(tree2,INFOSTAT,handle);
							break;
						}
					else
						{	put_text(tree2,INFOSTAT,"OK",50); redraw_objc(tree2,INFOSTAT,handle);
						}
			}
		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		fclose(fin);
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

int senddata(DATA *data, int marked)
	{ int bytecnt=0,len,i;
		char temp[1024];

		for (i=0; i<=8; i++)
			if (*data[i].data)
				{	if (konvert) umlaute(data[i].data);
					len=(int) strlen(data[i].data);
					if (bytecnt>=0x180)
						{	form_alert(1,"[3][Datensatz zu lang... |][ Weiter ]");
							break;
						}
					else if (bytecnt+len>0x180)
						{	len=0x180-bytecnt;
							data[i].data[len]=EOS;
							form_alert(1,"[3][Feldeintrag gekÅrzt... |][ Weiter ]");
						}
					if (bytecnt<0x180)
						{	sprintf(temp,":%02X8%03X00%s",len,bytecnt & 0xfff,convert(data[i].data));
							addchecksum(temp);
							bytecnt += len;
							send(temp);
						}
				}

			if (marked)	send(":008000017F");	/* EOD marked */
			else				send(":00000001FF");	/* EOD				*/

			if (showack(wait4ack())==FALSE)
				return FALSE;

			while (askinp(1))
				{	sprintf(temp,"Synchronizing: <$%X>",inp(1));
					put_text(tree2,INFOSTAT,temp,50); redraw_objc(tree2,INFOSTAT,handle);
					if (init()==FALSE)
						return FALSE;
				}
			return TRUE;
	}

/*******************************************************************/

void backup(void)
	{	FILE	*fout;
		char	buffer[1024],dataitem[16],in_path[PATH_MAX],sel_file[13]="";
		int		error=0,block=0,len;

		set_rs232e();
		if (fileselect(in_path,sel_file,"*.CBU","Empfangen (komplett)")==0)
			return;
		cutext(sel_file); strcat(in_path,sel_file); strcat(in_path,".CBU");

		put_text(tree3,INFOLINE,"Starte DatenÅbertragung!",50);
		put_text(tree3,DATAITEM,"   ",50);
		xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

		fout=fopen(in_path,"wb");

		if (wait4header()==FALSE)
			{	fclose(fout);
				xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				return;
			}

		put_text(tree3,INFOLINE,"Transfer gestartet.",50); redraw_objc(tree3,INFOLINE,handle);
		out(0x11);
		
		for (;;)
			{	
				switch(getheaderblock(buffer,&len))
					{	case	TRUE	: break;
						case	0x21	: error=1;		/* Datei lîschen, ESC am CASIO */
													break;
						case	-1		: error=27;		/* Datei lîschen, ESC am ST */
													break;
						case	-2		: error=2;		/* Datei lîschen, CHECKSUM */
													break;
						default			: error=3;		/* Datei lîschen */
													break;
					}
				if (error) break;
				fputs(buffer,fout);
				xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

				if (strcmp(buffer,":0200000280007C")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Calendar",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":0200000290006C")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Telephone",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":0200000291006B")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Reminder",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":02000002A0005C")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Memo",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
/*???*/	else if (strcmp(buffer,":02000002A1005B")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Weekly",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":02000002B0004C")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Schedule",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":02000002B1004B")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Free File",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":02000002C0003C")==0)
					{	put_text(tree3,INFOLINE,"Empfange: Business Card",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
/*???*/	else if (strcmp(buffer,":02000002C1003B")==0)
					{	put_text(tree3,INFOLINE,"Empfange: ToDo",50); redraw_objc(tree3,INFOLINE,handle);
						block=0;
					}
				else if (strcmp(buffer,":00000001FF")==0 ||
						strcmp(buffer,":008000017F")==0 )
					{	sprintf(dataitem,"%3u",++block);
						put_text(tree3,DATAITEM,dataitem,3); redraw_objc(tree3,DATAITEM,handle);
					}
				else if (strcmp(buffer,":000000FF01")==0)
					break;
			}
		fclose(fout);
		if (error==27) out(0x21);
		if (error) remove(in_path);

		clearbuffer();
		if (error==0)
			form_alert(1,"[1][Transfer OK... |][  OK  ]");
		else if (error==2)
			form_alert(1,"[1][CRC Fehler... |][  OK  ]");
		else if (error==3)
			form_alert(1,"[1][Unbekannter Fehler... |][  OK  ]");
		else
			form_alert(1,"[1][Transfer abgebrochen... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

void restore(void)
	{	FILE	*fin;
		char	buffer[16],dataitem[16],in_path[PATH_MAX],sel_file[13]="";
		int		chr,block=0;
		long	lof;

		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (fileselect(in_path,sel_file,"*.*","Welche Datei senden?")==0)
			return;
		strcat(in_path, sel_file);
		if (exist(in_path)!=0)
			{	form_alert(1,"[3][Datei nicht gefunden! |][  OK  ]");
				return;
			}

		put_text(tree3,DATAITEM,"   ",50);
		xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
		put_text(tree3,INFOLINE,"Synchronisiere...",50); redraw_objc(tree3,INFOLINE,handle);

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				return;
			}
		fin=fopen(in_path,"rb");
		lof=filelength(fin->Handle);

		memset(buffer,' ',15); buffer[15]=EOS;
		while (lof)
			{ chr=getc(fin);
				lof--;
				strcpy(buffer,buffer+1);
				buffer[14]=chr;
				out(chr);

				if (*buffer==':')
					{	xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

						if (strncmp(buffer,":02000002",9)==0)
							{	if (strcmp(buffer+9,"80007C")==0)
									{	put_text(tree3,INFOLINE,"Sende: Calendar",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
								else if (strcmp(buffer+9,"90006C")==0)
									{	put_text(tree3,INFOLINE,"Sende: Telephone",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
								else if (strcmp(buffer+9,"91006B")==0)
									{	put_text(tree3,INFOLINE,"Sende: Reminder",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
								else if (strcmp(buffer+9,"A0005C")==0)
									{	put_text(tree3,INFOLINE,"Sende: Memo",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
/*???*/					else if (strcmp(buffer+9,"A1005B")==0)
									{	put_text(tree3,INFOLINE,"Sende: Weekly",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
								else if (strcmp(buffer+9,"B0004C")==0)
									{	put_text(tree3,INFOLINE,"Sende: Schedule",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
								else if (strcmp(buffer+9,"B1004B")==0)
									{	put_text(tree3,INFOLINE,"Sende: Free File",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
								else if (strcmp(buffer+9,"C0003C")==0)
									{	put_text(tree3,INFOLINE,"Sende: Business Card",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
/*???*/					else if (strcmp(buffer+9,"C1003B")==0)
									{	put_text(tree3,INFOLINE,"Sende: ToDo",50); redraw_objc(tree3,INFOLINE,handle);
										put_text(tree3,DATAITEM,"HDR",3); redraw_objc(tree3,DATAITEM,handle);
										if (showack(wait4ack())==FALSE)
											break;
									}
							}
					}

				if (buffer[4]==':')
					{	if (strcmp(buffer+4,":00000001FF")==0 ||
								strcmp(buffer+4,":008000017F")==0 )
							{	sprintf(dataitem,"%3u",++block);
								put_text(tree3,DATAITEM,dataitem,3); redraw_objc(tree3,DATAITEM,handle);
								if (showack(wait4ack())==FALSE)
									break;
							}
						if (strcmp(buffer+4,":000000FF01")==0)
							break;
					}
			}

		fclose(fin);
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void bereiche_recv(void)
	{	FILE	*fout=NULL;
		char	buffer[1024],temp[MAX_STR],in_path[PATH_MAX],sel_file[13]="",
					dataitem[16],mode[3];
		int		error=0,block=0,open=FALSE,len,singleitem;

		set_rs232e();
		if (fileselect(in_path,sel_file,"*.*","Empfangen (einzeln)")==0)
			return;
		cutext(sel_file); strcat(in_path, sel_file);

		if (form_alert(2,"[2][Sollen einzelne Daten empfangen |werden? |(ONE DATA ITEM, EIN EINTRAG) |][  Ja  | Nein ]")==1)
			{	singleitem=TRUE;
				strcpy(mode,"ab");
				strcpy(temp,in_path); strcat(temp,".TEL"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".BUS"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".MEM"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".SCD"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".CAL"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".REM"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".FRF"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".WEE"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,".TOD"); if (exist(temp)==0) remove(temp);
			}
		else
			{	strcpy(mode,"wb");
				singleitem=FALSE;
			}

		put_text(tree3,INFOLINE,"Starte DatenÅbertragung!",50);
		put_text(tree3,DATAITEM,"   ",50);
		xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

		if (wait4header()==FALSE)
			{	xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				return;
			}

		put_text(tree3,INFOLINE,"Transfer gestartet.",50); redraw_objc(tree3,INFOLINE,handle);
		out(0x11);
		
		for (;;)
			{ if (open)
					fputs(buffer,fout);

				switch(getheaderblock(buffer,&len))
					{	case	TRUE	: break;
						case	0x21	: error=1;		/* Datei lîschen, ESC am CASIO */
													break;
						case	-1		: error=27;		/* Datei lîschen, ESC am ST */
													break;
						case	-2		: error=2;		/* Datei lîschen, CHECKSUM */
													break;
						default			: error=3;		/* Datei lîschen */
													break;
					}
				if (error) break;
				xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

				if (strncmp(buffer,":02000002",9)==0)
					{	
						if (open)
							{	if (!singleitem) fputs(":000000FF01",fout);
								fclose(fout);
							}

						if (strcmp(buffer+9,"80007C")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Calendar",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".CAL");
								fout=fopen(temp,mode);
							}
						else if (strcmp(buffer+9,"90006C")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Telephone",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".TEL");
								fout=fopen(temp,mode);
							}
						else if (strcmp(buffer+9,"91006B")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Reminder",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".REM");
								fout=fopen(temp,mode);
							}
						else if (strcmp(buffer+9,"A0005C")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Memo",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".MEM");
								fout=fopen(temp,mode);
							}
/*???*/			else if (strcmp(buffer+9,"A1005B")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Weekly",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".MEM");
								fout=fopen(temp,mode);
							}
						else if (strcmp(buffer+9,"B0004C")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Schedule",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".SCD");
								fout=fopen(temp,mode);
							}
						else if (strcmp(buffer+9,"B1004B")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Free File",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".FRF");
								fout=fopen(temp,mode);
							}
						else if (strcmp(buffer+9,"C0003C")==0)
							{	put_text(tree3,INFOLINE,"Empfange: Business Card",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".BUS");
								fout=fopen(temp,mode);
							}
/*???*/			else if (strcmp(buffer+9,"C1003B")==0)
							{	put_text(tree3,INFOLINE,"Empfange: ToDo",50); redraw_objc(tree3,INFOLINE,handle);
								strcpy(temp,in_path); strcat(temp,".TOD");
								fout=fopen(temp,mode);
							}
						else
							{	sprintf(temp,"Empfange: Unbekannt ($%c)",buffer[9]);
								put_text(tree3,INFOLINE,temp,50); redraw_objc(tree3,INFOLINE,handle);
								sprintf(temp,"%s.ER%c",in_path,buffer[9]);
								fout=fopen(temp,"wb");	/* VORHANDENE DATEI LôSCHEN!!! */
							}

						open=TRUE;
						if (!singleitem) block=0;
						
					}


				if (strcmp(buffer,":00000001FF")==0 ||
						strcmp(buffer,":008000017F")==0 )
					{	sprintf(dataitem,"%3u",++block);
						put_text(tree3,DATAITEM,dataitem,3); redraw_objc(tree3,DATAITEM,handle);
					}
				if (strcmp(buffer,":000000FF01")==0)
					break;
			}
		if (open)
			{	fputs(":000000FF01",fout);
				fclose(fout);
			}
		if (error==27) out(0x21);
		if (error)
			if (singleitem==FALSE || error==2 || error==3)
				remove(temp);
		clearbuffer();
		if (error==0)
			form_alert(1,"[1][Transfer OK... |][  OK  ]");
		else if (error==2)
			form_alert(1,"[1][CRC Fehler... |][  OK  ]");
		else if (error==3)
			form_alert(1,"[1][Unbekannter Fehler... |][  OK  ]");
		else
			form_alert(1,"[1][Transfer abgebrochen... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void ascii_recv(void)
	{	FILE	*fout=NULL;
		char	temp[MAX_STR],buffer[1024],in_path[PATH_MAX],sel_file[13]="",
					dataitem[16],mode[3],*pointer;
		int		i,error=0,len,block=0,open=FALSE,singleitem,linecnt=-1;

		set_rs232e();
		if (fileselect(in_path,sel_file,"*.*","Empfangen (einzeln ASCII)")==0)
			return;
		cutext(sel_file); strcat(in_path, sel_file);

		if (form_alert(2,"[2][Sollen einzelne Daten empfangen |werden? |(ONE DATA ITEM, EIN EINTRAG) |][  Ja  | Nein ]")==1)
			{	singleitem=TRUE;
				strcpy(mode,"a");
				strcpy(temp,in_path); strcat(temp,"._TE"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._BU"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._ME"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._SC"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._CA"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._RE"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._FR"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._WE"); if (exist(temp)==0) remove(temp);
				strcpy(temp,in_path); strcat(temp,"._TO"); if (exist(temp)==0) remove(temp);
			}
		else
			{	strcpy(mode,"w");
				singleitem=FALSE;
			}

		put_text(tree3,INFOLINE,"Starte DatenÅbertragung!",50);
		put_text(tree3,DATAITEM,"   ",50);
		xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
		xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

		if (wait4header()==FALSE)
			{	xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
				return;
			}

		put_text(tree3,INFOLINE,"Transfer gestartet.",50); redraw_objc(tree3,INFOLINE,handle);
		out(0x11);

		for (;;)
			{ 
				switch(getheaderblock(buffer,&len))
					{	case	TRUE	: break;
						case	0x21	: error=1;		/* Datei lîschen, ESC am CASIO */
													break;
						case	-1		: error=27;		/* Datei lîschen, wenn !singleitem */
													break;
						case	-2		: error=2;		/* Datei lîschen, CHECKSUM */
													break;
						default			: error=3;		/* Datei lîschen */
													break;
					}
				if (error) break;
				xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
				
				if (strcmp(buffer,":000000FF01")==0)
					break;
				else if (strcmp(buffer,":00000001FF")==0)
					{	sprintf(dataitem,"%3u",++block);
						put_text(tree3,DATAITEM,dataitem,3); redraw_objc(tree3,DATAITEM,handle);
						if (linecnt!=-1)
							{	for (i=linecnt+1; i<=11; i++)
									fputc('\n',fout);
								fputc('\n',fout);
								linecnt=0;
							}
						else
							{	fprintf(fout,"---EOD---\n");
							}
					}
				else if (strcmp(buffer,":800000017F")==0)
					{	sprintf(dataitem,"%3u",++block);
						put_text(tree3,DATAITEM,dataitem,3); redraw_objc(tree3,DATAITEM,handle);
						if (linecnt!=-1)
							{	for (i=linecnt+1; i<=11; i++)
									fputc('\n',fout);
								fputc('\n',fout);
								linecnt=0;
							}
						else
							{	fprintf(fout,"---EOD/MARKED---\n");
							}
					}
				else if (strncmp(buffer,":02000002",9)==0)
					{	if (open)
							fclose(fout);
						open=TRUE;
						linecnt=-1;
						if (!singleitem) block=0;
						strcpy(temp,in_path);
						switch((char) buffer[9])
							{	case	'8'	:	put_text(tree3,INFOLINE,"Empfange: Calendar",50); redraw_objc(tree3,INFOLINE,handle);
														strcat(temp,"._CA");
														fout=fopen(temp,mode);
														break;
								case	'9'	:	switch(buffer[10])
														{	case '0'	:	put_text(tree3,INFOLINE,"Empfange: Telephone",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._TE");
																					linecnt=0;
																					break;
															case '1'	:	put_text(tree3,INFOLINE,"Empfange: Reminder",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._RE");
																					break;
															default		:	goto unknown;
														}
														fout=fopen(temp,mode);
														break;
								case	'A'	:	switch(buffer[10])
														{	case '0'	:	put_text(tree3,INFOLINE,"Empfange: Memo",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._ME");
																					break;
															case '1'	:	put_text(tree3,INFOLINE,"Empfange: Weekly",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._WE");
																					break;
															default		:	goto unknown;
														}
														fout=fopen(temp,mode);
														break;
								case	'B'	:	switch(buffer[10])
														{	case '0'	:	put_text(tree3,INFOLINE,"Empfange: Schedule",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._SC");
																					break;
															case '1'	:	put_text(tree3,INFOLINE,"Empfange: Free File",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._FR");
																					break;
															default		:	goto unknown;
														}
														fout=fopen(temp,mode);
														break;
								case	'C'	:	switch(buffer[10])
														{	case '0'	:	put_text(tree3,INFOLINE,"Empfange: Business Card",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._BU");
																					break;
															case '1'	:	put_text(tree3,INFOLINE,"Empfange: ToDo",50); redraw_objc(tree3,INFOLINE,handle);
																					strcat(temp,"._TO");
																					break;
															default		: goto unknown;
														}
														fout=fopen(temp,mode);
														break;
								default		:
unknown:										sprintf(temp,"Empfange: Unbekannt ($%c)",buffer[9]);
														put_text(tree3,INFOLINE,temp,50); redraw_objc(tree3,INFOLINE,handle);
														sprintf(temp,"%s.E_%c",in_path,buffer[9]);
														fout=fopen(temp,"a");	/* war "w": VORHANDENE DATEI LôSCHEN */
														fprintf(fout,"Unbekannter Bereich\n");
														fprintf(fout,"Header: %s\n",buffer);
														break;
							}
					}
				else
					{	if (linecnt!=-1) linecnt++;
						switch((char) buffer[3])	/* :LLTppp00dd...ddCC */
							{	case	'7'	: fprintf(fout,"FreeFile (Datenfeld): %s\n",buffer);
														break;
								case	'8'	:
								case 	'9'	: if (linecnt==1 || linecnt==4)
															{	strcpy(buffer,decode(buffer+9,len));
																pointer=strstr(buffer,", ");
																if (pointer==NULL)
																	{	fprintf(fout,"%s\n",buffer);
																	}
																else
																	{	*pointer=EOS;
																		fprintf(fout,"%s\n",buffer);
																		fprintf(fout,"%s\n",pointer+2);
																		linecnt++;
																	}
															}
														else
															{	fprintf(fout,"%s\n",decode(buffer+9,len));
															}
														break;
								case	'C'	: fprintf(fout,"Alarm: %s\n",decode(buffer+9,len));
														break;
								case	'D'	: fprintf(fout," Frei: %s\n",decodedays(buffer+9,len));
														break;
								case	'E'	: fprintf(fout," Zeit: %s\n",decode(buffer+9,len));
														break;
								case	'F'	: fprintf(fout,"Datum: %s\n",decode(buffer+9,len));
														break;
								default		:	fprintf(fout,"Unbekanntes Feld: %s\n",buffer);
							}
					}
			} /* for */
		if (open)
			fclose(fout);
		if (error==27) out(0x21);
		if (error)
			if (singleitem==FALSE || error==2 || error==3)
				remove(temp);
			
		clearbuffer();
		if (error==0)
			form_alert(1,"[1][Transfer OK... |][  OK  ]");
		else if (error==2)
			form_alert(1,"[1][CRC Fehler... |][  OK  ]");
		else if (error==3)
			form_alert(1,"[1][Unbekannter Fehler... |][  OK  ]");
		else
			form_alert(1,"[1][Transfer abgebrochen... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void term_send(void)
	{	char	temp[1024],dataitem[32];
		int		wahl,i,len,bytecnt,tag;
		struct date HEUTE;

		getdate( &HEUTE );
		for (i=0; i<=2; i++)
			*data[i].data=EOS;
		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				return;
			}

		send(":02000002B0004C");

		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				return;
			}

		undo_state(tree5,SMARKON,SELECTED);
		put_text(tree5,SDATUM,"",8);
		put_text(tree5,SZEIT,"",8);
		put_text(tree5,SALARM,"",4);
		put_text(tree5,STEXT1,"",50);
		put_text(tree5,STEXT2,"",50);
		put_text(tree5,STEXT3,"",50);
		for (i=0; i<=15; i++)
			put_text(tree5,TAG1+i,"",2);
		xform_dial(tree5,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	
loop1:	wahl=xform_do(handle,tree5,NULL,0,0,trans_key3,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=SEXIT;
				wahl &= 0xfff;
				undo_state(tree5,wahl,SELECTED);
				redraw_objc(tree5,wahl,handle);

				if (wahl==SSEND)
					{	if (strlen(get_text(tree5,SDATUM))!=4 &&
								strlen(get_text(tree5,SDATUM))!=6 &&
								strlen(get_text(tree5,SDATUM))!=8 )
							{	form_alert(1,"[1][ Datum falsch! |][  OK  ]");
								goto loop1;
							}
					}

				if (wahl==SSEND)
					{	put_text(tree3,INFOLINE,"DatenÅbertragung",50); redraw_objc(tree3,INFOLINE,handle);
						put_text(tree3,DATAITEM,"   ",3);
						xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

						for (tag=-1; tag<=15; tag++)
							{ if (tag==-1 ||
										*get_text(tree5,TAG1+tag))
									{
										if (strlen(get_text(tree5,SDATUM))==4)
											{	sprintf(temp,"%04u",HEUTE.da_year);
												strcpy(dataitem,temp);
											}
										else if (strlen(get_text(tree5,SDATUM))==6)
											{	strcpy(dataitem,"19");
											}
										else
											{	*dataitem = EOS;
											}
										strcat(dataitem,get_text(tree5,SDATUM)+4);
		
										dataitem[4] = '-';
										dataitem[5] = *(get_text(tree5,SDATUM)+2);
										dataitem[6] = *(get_text(tree5,SDATUM)+3);
										dataitem[7] = '-';
										dataitem[8] = *get_text(tree5,SDATUM);
										dataitem[9] = *(get_text(tree5,SDATUM)+1);
										dataitem[10]= EOS;
										if (tag>=0)
											{	if (strlen(get_text(tree5,TAG1+tag))==1)
													{	dataitem[8]='0';
														dataitem[9] = *get_text(tree5,TAG1+tag);
													}
												else
													{	dataitem[8] = *get_text(tree5,TAG1+tag);
														dataitem[9] = *(get_text(tree5,TAG1+tag)+1);
													}
											}
										sprintf(temp,":%02XF%03X00%s",10,0,convert(dataitem));
										addchecksum(temp);
										send(temp);
						
										if (*get_text(tree5,SZEIT))
											{	while(strlen(get_text(tree5,SZEIT))<4)
													strcat(get_text(tree5,SZEIT),"0");
												if (strlen(get_text(tree5,SZEIT))>4)
													while(strlen(get_text(tree5,SZEIT))<8)
														strcat(get_text(tree5,SZEIT),"0");

												strcpy(dataitem,get_text(tree5,SZEIT));
												dataitem[2] = ':';
												strcpy(dataitem+3,get_text(tree5,SZEIT)+2);
												if (strlen(dataitem)>5)
													{	dataitem[5] = '~';
														strcpy(dataitem+6,get_text(tree5,SZEIT)+4);
														dataitem[8] = ':';
														strcpy(dataitem+9,get_text(tree5,SZEIT)+6);
													}
												sprintf(temp,":%02XE%03X00%s",(int)strlen(dataitem),0,convert(dataitem));
												addchecksum(temp);
												send(temp);
											}
						
										if (*get_text(tree5,SALARM))
											{	while(strlen(get_text(tree5,SALARM))<4)
													strcat(get_text(tree5,SALARM),"0");
												strcpy(dataitem,get_text(tree5,SALARM));
												dataitem[2] = ':';
												strcpy(dataitem+3,get_text(tree5,SALARM)+2);
												sprintf(temp,":%02XC%03X00%s",5,0,convert(dataitem));
												addchecksum(temp);
												send(temp);
											}
						
										for (i=0; i<=2; i++)
											strcpy(data[i].data,get_text(tree5,STEXT1+i));
						
										bytecnt=0;
										for (i=0; i<=2; i++)
											if (*data[i].data)
												{	if (konvert) umlaute(data[i].data);
													len=(int) strlen(data[i].data);
													sprintf(temp,":%02X8%03X00%s",len,bytecnt & 0xfff,convert(data[i].data));
													addchecksum(temp);
													bytecnt += len;
													send(temp);
												}
						
										sprintf(temp,"%3u",tag+2);
										put_text(tree3,DATAITEM,temp,3); redraw_objc(tree3,DATAITEM,handle);
										xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

										if (is_state(tree5,SMARKON,SELECTED) ? TRUE : FALSE)
											send(":008000017F");	/* EOD marked */
										else
											send(":00000001FF");	/* EOD				*/

										if (showack(wait4ack())==FALSE)
											{	clearbuffer();
												form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
												xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
												return;
											}
									}
							}
						xform_dial(tree5,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
					}
			}
		while (wahl!=SEXIT);

		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void rem_send(void)
	{	char	temp[1024],dataitem[32];
		int		wahl,i,len,bytecnt;

		for (i=0; i<=2; i++)
			*data[i].data=EOS;
		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				return;
			}

		send(":0200000291006B");

		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				return;
			}

		undo_state(tree12,RMARKON,SELECTED);
		put_text(tree12,RDATUM,"",4);
		put_text(tree12,RALARM,"",4);
		put_text(tree12,RTEXT1,"",50);
		put_text(tree12,RTEXT2,"",50);
		put_text(tree12,RTEXT3,"",50);
		xform_dial(tree12,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	
loop11:	wahl=xform_do(handle,tree12,NULL,0,0,trans_key8,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=REXIT;
				wahl &= 0xfff;
				undo_state(tree12,wahl,SELECTED);
				redraw_objc(tree12,wahl,handle);

				if (wahl==RSEND)
					{	if (strlen(get_text(tree12,RDATUM))!=0 &&
								strlen(get_text(tree12,RDATUM))!=2 &&
								strlen(get_text(tree12,RDATUM))!=4 )
							{	form_alert(1,"[1][ Datum falsch! |][  OK  ]");
								goto loop11;
							}
					}

				if (wahl==RSEND)
					{	put_text(tree3,INFOLINE,"DatenÅbertragung",50); redraw_objc(tree3,INFOLINE,handle);
						put_text(tree3,DATAITEM,"   ",3);
						xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

						if (strlen(get_text(tree12,RDATUM))==0)
							strcpy(dataitem,"----------");		/* tÑglich */
						else if (strlen(get_text(tree12,RDATUM))==2)
							sprintf(dataitem,"--------%c%c",
													*get_text(tree12,RDATUM),
													*(get_text(tree12,RDATUM)+1));	/* monatlich */
						else
							sprintf(dataitem,"-----%c%c-%c%c",
													*(get_text(tree12,RDATUM)+2),
													*(get_text(tree12,RDATUM)+3),
													*get_text(tree12,RDATUM),
													*(get_text(tree12,RDATUM)+1));	/* jÑhrlich */

						sprintf(temp,":%02XF%03X00%s",10,0,convert(dataitem));
						addchecksum(temp);
						send(temp);
		
						if (*get_text(tree12,RALARM))
							{	while(strlen(get_text(tree12,RALARM))<4)
									strcat(get_text(tree5,RALARM),"0");
								strcpy(dataitem,get_text(tree12,RALARM));
								dataitem[2] = ':';
								strcpy(dataitem+3,get_text(tree12,RALARM)+2);
								sprintf(temp,":%02XC%03X00%s",5,0,convert(dataitem));
								addchecksum(temp);
								send(temp);
							}
		
						for (i=0; i<=2; i++)
							strcpy(data[i].data,get_text(tree12,RTEXT1+i));
		
						bytecnt=0;
						for (i=0; i<=2; i++)
							if (*data[i].data)
								{	if (konvert) umlaute(data[i].data);
									len=(int) strlen(data[i].data);
									sprintf(temp,":%02X8%03X00%s",len,bytecnt & 0xfff,convert(data[i].data));
									addchecksum(temp);
									bytecnt += len;
									send(temp);
								}
		
						put_text(tree3,DATAITEM," OK",3); redraw_objc(tree3,DATAITEM,handle);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

						if (is_state(tree12,RMARKON,SELECTED) ? TRUE : FALSE)
							send(":008000017F");	/* EOD marked */
						else
							send(":00000001FF");	/* EOD				*/

						if (showack(wait4ack())==FALSE)
							{	clearbuffer();
								form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
								xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
								return;
							}
						xform_dial(tree12,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
					}
			}
		while (wahl!=REXIT);

		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void memo_send(void)
	{	char	temp[1024],*pointer,in_path[PATH_MAX],sel_file[13]="";
		FILE	*fin;
		int		wahl,i,len,bytecnt;

		for (i=0; i<=11; i++)
			*data[i].data=EOS;
		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				return;
			}

		send(":02000002A0005C");
		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				return;
			}

		for (i=0; i<=11; i++)
			put_text(tree6,MTEXT1+i,"",32);
		undo_state(tree6,MMARKON,SELECTED);
		xform_dial(tree6,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	
loop2:	wahl=xform_do(handle,tree6,NULL,0,0,trans_key4,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=MEXIT;
				wahl &= 0xfff;
				undo_state(tree6,wahl,SELECTED);
				redraw_objc(tree6,wahl,handle);
		
				if (wahl==MLOAD)
					{	if (fileselect(in_path,sel_file,"*.TXT","Welche ASCII-Datei senden?")==0)
							goto loop2;
						strcat(in_path, sel_file);
						if (exist(in_path)!=0)
							{	form_alert(1,"[3][Datei nicht gefunden! |][  OK  ]");
								goto loop2;
							}
						pointer=temp; len=0;
						fin = fopen(in_path,"rb");
						while (!feof(fin) && len<32*12)
							{	i = getc(fin);
								if (i==EOF)
									break;
								if (i=='\n')
									i=' ';
								*pointer++ = i;
								len++;
							}
						*pointer = EOS;
						fclose(fin);
						for (i=0; i<=11; i++)
							put_text(tree6,MTEXT1+i,"",32);
						for (i=11; i>=1; i--)
							{	if (strlen(temp)>32*i)
									{	put_text(tree6,MTEXT1+i,temp+32*i,32);
										temp[32*i]=EOS;
									}
							}
						put_text(tree6,MTEXT1,temp,32);
						redraw_objc(tree6,MBOX,handle);
					}
		
				if (wahl==MSEND)
					{	put_text(tree3,INFOLINE,"DatenÅbertragung",50);
						put_text(tree3,DATAITEM,"   ",3);
						xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
		
						for (i=0; i<=11; i++)
							strcpy(data[i].data,get_text(tree6,MTEXT1+i));
		
						bytecnt=0;
						for (i=0; i<=11; i++)
							if (*data[i].data)
								{	if (konvert) umlaute(data[i].data);
									len=(int) strlen(data[i].data);
									sprintf(temp,":%02X8%03X00%s",len,bytecnt & 0xfff,convert(data[i].data));
									addchecksum(temp);
									bytecnt += len;
									send(temp);
								}
						
						if (is_state(tree6,MMARKON,SELECTED) ? TRUE : FALSE)
							send(":008000017F");	/* EOD marked */
						else
							send(":00000001FF");	/* EOD				*/
						if (showack(wait4ack())==FALSE)
							{	clearbuffer();
								form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
								xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
								return;
							}

						put_text(tree3,DATAITEM," OK",3); redraw_objc(tree3,DATAITEM,handle);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
						xform_dial(tree6,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
					}
			}
		while (wahl!=MEXIT);

		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void bus_adr_send(OBJECT *tree,int felder,int ffirst,int fmark,int fexit,int fsend,char *finit,int (*ftrans_key)(int,int))
	{	char	temp[1024];
		int		wahl,i,len,bytecnt;
		felder--;

		for (i=0; i<=felder; i++)
			*data[i].data=EOS;
		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				return;
			}

		send(finit);
		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				return;
			}

		for (i=0; i<=felder; i++)
			put_text(tree,ffirst+i,"",50);
		undo_state(tree,fmark,SELECTED);
		xform_dial(tree,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	
				wahl=xform_do(handle,tree,NULL,0,0,ftrans_key,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=fexit;
				wahl &= 0xfff;
				undo_state(tree,wahl,SELECTED);
				redraw_objc(tree,wahl,handle);
		
				if (wahl==fsend)
					{	put_text(tree3,INFOLINE,"DatenÅbertragung",50);
						put_text(tree3,DATAITEM,"   ",3);
						xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
		
						for (i=0; i<=felder; i++)
							strcpy(data[i].data,get_text(tree,ffirst+i));
						for (i=felder; i>=1; i--)
							if (*data[i].data!=EOS)	strcat(data[i-1].data,"\n");
		
						bytecnt=0;
						for (i=0; i<=felder; i++)
							if (*data[i].data)
								{	if (konvert) umlaute(data[i].data);
									len=(int) strlen(data[i].data);
									if (bytecnt>=0x180)
										{	form_alert(1,"[3][Datensatz zu lang... |][ Weiter ]");
											break;
										}
									else if (bytecnt+len>0x180)
										{	len=0x180-bytecnt;
											data[i].data[len]=EOS;
											form_alert(1,"[3][Feldeintrag gekÅrzt... |][ Weiter ]");
										}
									if (bytecnt<0x180)
										{	sprintf(temp,":%02X8%03X00%s",len,bytecnt & 0xfff,convert(data[i].data));
											addchecksum(temp);
											bytecnt += len;
											send(temp);
										}
								}
						
						if (is_state(tree,fmark,SELECTED) ? TRUE : FALSE)
							send(":008000017F");	/* EOD marked */
						else
							send(":00000001FF");	/* EOD				*/
						if (showack(wait4ack())==FALSE)
							{	clearbuffer();
								form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
								xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
								return;
							}

						put_text(tree3,DATAITEM," OK",3); redraw_objc(tree3,DATAITEM,handle);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);
						xform_dial(tree,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
					}
			}
		while (wahl!=fexit);

		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

void cal_send(void)
	{	char	temp[1024],dataitem[32];
		int		wahl,i;
		struct date HEUTE;
		long	freietage;

		getdate( &HEUTE );
		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				return;
			}

		send(":0200000280007C");

		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				return;
			}

		put_text(tree9,CDATUM,"",6);
		for (i=0; i<=30; i++)
			undo_state(tree9,C1+i,SELECTED);
		xform_dial(tree9,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	
loop3:	wahl=xform_do(handle,tree9,NULL,0,0,trans_key7,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=CEXIT;
				wahl &= 0xfff;
				undo_state(tree9,wahl,SELECTED);
				redraw_objc(tree9,wahl,handle);

				if (wahl==CSEND)
					{	if (strlen(get_text(tree9,CDATUM))!=2 &&
								strlen(get_text(tree9,CDATUM))!=4 &&
								strlen(get_text(tree9,CDATUM))!=6 )
							{	form_alert(1,"[1][ Datum falsch! |][  OK  ]");
								goto loop3;
							}
					}

				if (wahl==CSEND)
					{	put_text(tree3,INFOLINE,"DatenÅbertragung",50); redraw_objc(tree3,INFOLINE,handle);
						put_text(tree3,DATAITEM,"   ",3);
						xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

						freietage=0;
						for (i=0; i<=30; i++)
							if (is_state(tree9,C1+i,SELECTED))
								freietage |= (1UL << i);

						if (strlen(get_text(tree9,CDATUM))==2)
							{	sprintf(temp,"%04u",HEUTE.da_year);
								strcpy(dataitem,temp);
							}
						else if (strlen(get_text(tree9,CDATUM))==4)
							{	strcpy(dataitem,"19");
							}
						else
							{	*dataitem = EOS;
							}
						strcat(dataitem,get_text(tree9,CDATUM)+2);
						dataitem[4] = '-';
						dataitem[5] = *get_text(tree9,CDATUM);
						dataitem[6] = *(get_text(tree9,CDATUM)+1);
						dataitem[7] = '-';
						dataitem[8] = '0';
						dataitem[9] = '1';
						dataitem[10]= EOS;

						sprintf(temp,":%02XF%03X00%s",10,0,convert(dataitem));
						addchecksum(temp);
						send(temp);
						
						sprintf(temp,":%02XD%03X00%08lX",4,0,freietage);
						addchecksum(temp);
						send(temp);
						
						send(":00000001FF");	/* EOD				*/

						if (showack(wait4ack())==FALSE)
							{	clearbuffer();
								form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
								xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
								return;
							}
						xform_dial(tree9,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
					}
			}
		while (wahl!=CEXIT);

		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

/*******************************************************************/

static void sende_eintrag(char *datum, int datcnt);
static int get_step(int *tag, int *monat, int *jahr);

void rem2_send(void)
	{	char	dataitem[32];
		int		wahl,i,tag,monat,jahr,step,datcnt,werktags;
		unsigned long datumvon,datumbis,jcounter;

		for (i=0; i<=2; i++)
			*data[i].data=EOS;
		set_rs232s();
		form_alert(1,"[1][CASIO auf Empfang schalten... |][  OK  ]");

		if (init()==FALSE)
			{	form_alert(1,"[3][Abbruch durch User... |][  OK  ]");
				return;
			}

		send(":02000002B0004C");

		if (showack(wait4ack())==FALSE)
			{	clearbuffer();
				form_alert(1,"[3][DatenÅbertragung abgebrochen... |][  OK  ]");
				return;
			}

		undo_xstate(tree13,SELECTED,REM2MARK,REM2YEAR,REM2MONT,REM2DAY,
																REM2WORK,REM2FEST,-1);
		do_state(tree13,REM2WEEK,SELECTED);

		put_text(tree13,REM2VON,"",8);
		put_text(tree13,REM2BIS,"",8);

		put_text(tree13,REM2ALAR,"",4);
		put_text(tree13,REM2ZEIT,"",8);
		put_text(tree13,REM2TXT1,"",50);
		put_text(tree13,REM2TXT2,"",50);
		put_text(tree13,REM2TXT3,"",50);
		xform_dial(tree13,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);

		do
			{	
loop4:	wahl=xform_do(handle,tree13,NULL,0,0,trans_key9,&wnx,&wny,&wnw,&wnh);
				if (wahl & 0x4000) wahl=SEXIT;
				wahl &= 0xfff;
				undo_state(tree13,wahl,SELECTED);
				redraw_objc(tree13,wahl,handle);

				if (wahl==REM2SEND)
					{	if (strlen(get_text(tree13,REM2VON))!=8 &&
								strlen(get_text(tree13,REM2VON))!=6 )
							{	form_alert(1,"[1][ Datum falsch! |][  OK  ]");
								goto loop4;
							}
						if (strlen(get_text(tree13,REM2BIS))!=8 &&
								strlen(get_text(tree13,REM2BIS))!=6 )
							{	form_alert(1,"[1][ Datum falsch! |][  OK  ]");
								goto loop4;
							}
					}

				if (wahl==REM2SEND)
					{	put_text(tree3,INFOLINE,"DatenÅbertragung",50); redraw_objc(tree3,INFOLINE,handle);
						put_text(tree3,DATAITEM,"   ",3);
						xform_dial(tree3,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
						xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

						tag		= (*get_text(tree13,REM2BIS)-48)*10 +
						      	 *(get_text(tree13,REM2BIS)+1)-48;
						monat	= (*(get_text(tree13,REM2BIS)+2)-48)*10 +
						       	 *(get_text(tree13,REM2BIS)+3)-48;
						jahr	= atoi(get_text(tree13,REM2BIS)+4);
						if (strlen(get_text(tree13,REM2BIS))==6)	jahr += 1900;
						datumbis = julian(tag,monat,jahr);

						tag		= (*get_text(tree13,REM2VON)-48)*10 +
						      	 *(get_text(tree13,REM2VON)+1)-48;
						monat	= (*(get_text(tree13,REM2VON)+2)-48)*10 +
						       	 *(get_text(tree13,REM2VON)+3)-48;
						jahr	= atoi(get_text(tree13,REM2VON)+4);
						if (strlen(get_text(tree13,REM2VON))==6)	jahr += 1900;
						datumvon = julian(tag,monat,jahr);

						if (datumvon>datumbis)
							{	form_alert(1,"[1][ Datum falsch! |][  OK  ]");
								goto loop4;
							}

						werktags = FALSE;

						if (is_state(tree13,REM2YEAR,SELECTED))
							step=365;
						else if (is_state(tree13,REM2MONT,SELECTED))
							{	step=40;	/* bedeutet monatlich */
								if (is_state(tree13,REM2FEST,SELECTED))
									step=30;
							}
						else if (is_state(tree13,REM2WEEK,SELECTED))
							step=7;
						else if (is_state(tree13,REM2WORK,SELECTED))
							{	step=1;
								werktags=TRUE;
							}
						else
							step=1;

						datcnt=0;
						if (step>=40)
							{	/* Jahr bzw. Monat hochzÑhlen */
								jcounter=datumvon;
								while (jcounter<=datumbis)
									{	strcpy(dataitem,datum(jcounter));
										sende_eintrag(dataitem,++datcnt);
										if (showack(wait4ack())==FALSE)
											{	clearbuffer();
												form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
												xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
												return;
											}
										if (step==40)
											{	monat++;
												if (monat>12)
													{	monat=1;
														jahr++;
													}
											}
										else
											{	jahr++;
											}
										jcounter = julian(tag,monat,jahr);
									}
							}
						else
							{	/* Julian Datum benutzen */
								for (jcounter=datumvon; jcounter<=datumbis; jcounter+=step)
									{	if (werktags)
											if (jcounter % 7 == 5 ||
													jcounter % 7 == 6)
												goto wochenende;

										strcpy(dataitem,datum(jcounter));
										sende_eintrag(dataitem,++datcnt);
										if (showack(wait4ack())==FALSE)
											{	clearbuffer();
												form_alert(1,"[3][öbertragungsfehler... |][  OK  ]");
												xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
												return;
											}
wochenende:					if (step>7)	step=get_step(&tag,&monat,&jahr);
									}
							}
						xform_dial(tree13,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
					}
			}
		while (wahl!=REM2EXIT);

		send(":000000FF01");	/* EOD complete	*/
		clearbuffer();
		form_alert(1,"[1][Transfer OK... |][  OK  ]");
		xform_dial(tree1,XFMD_DIALOG,&handle,&wnx,&wny,&wnw,&wnh);
	}

static int get_step(int *tag, int *monat, int *jahr)
	{	int ret=0;
		switch(*monat)
			{	case	1	:
				case	3	:
				case	5	:
				case	7	:
				case	8	:
				case	10:
				case	12:	if (*tag<=3)
										{	*tag+=4;
											ret=35;
										}
									else
										{	*tag-=3;
											ret=28;
										}
									break;
				case	2	:	if ( ((*jahr % 4 ==0) && (*jahr % 100 !=0)) ||
				               (*jahr % 400 == 0) )
										{	if (*tag<=1)
												{	*tag+=6;
													ret=35;
												}
											else
												{	*tag+=0;
													ret=28;
												}
										}
									else
										{	ret=28;
										}
									break;
				default	:	if (*tag<=2)
										{	*tag+=5;
											ret=35;
										}
									else
										{	*tag-=2;
											ret=28;
										}
									break;
			}
		(*monat)++;
		if (*monat>12)
			{	*monat=1;
				(*jahr)++;
			}
		return(ret);
	}

static void sende_eintrag(char *datum, int datcnt)
	{ char temp[1024],dataitem[32];
		int i,len,bytecnt;

		sprintf(temp,":%02XF%03X00%s",10,0,convert(datum));
		addchecksum(temp);
		send(temp);
						
		if (*get_text(tree13,REM2ZEIT))
			{	while(strlen(get_text(tree13,REM2ZEIT))<4)
					strcat(get_text(tree13,REM2ZEIT),"0");
				if (strlen(get_text(tree13,REM2ZEIT))>4)
					while(strlen(get_text(tree13,REM2ZEIT))<8)
						strcat(get_text(tree13,REM2ZEIT),"0");

				strcpy(dataitem,get_text(tree13,REM2ZEIT));
				dataitem[2] = ':';
				strcpy(dataitem+3,get_text(tree13,REM2ZEIT)+2);
				if (strlen(dataitem)>5)
					{	dataitem[5] = '~';
						strcpy(dataitem+6,get_text(tree13,REM2ZEIT)+4);
						dataitem[8] = ':';
						strcpy(dataitem+9,get_text(tree13,REM2ZEIT)+6);
					}
				sprintf(temp,":%02XE%03X00%s",(int)strlen(dataitem),0,convert(dataitem));
				addchecksum(temp);
				send(temp);
			}
						
		if (*get_text(tree13,REM2ALAR))
			{	while(strlen(get_text(tree13,REM2ALAR))<4)
					strcat(get_text(tree13,REM2ALAR),"0");
				strcpy(dataitem,get_text(tree13,REM2ALAR));
				dataitem[2] = ':';
				strcpy(dataitem+3,get_text(tree13,REM2ALAR)+2);
				sprintf(temp,":%02XC%03X00%s",5,0,convert(dataitem));
				addchecksum(temp);
				send(temp);
			}

		for (i=0; i<=2; i++)
			strcpy(data[i].data,get_text(tree13,REM2TXT1+i));

		bytecnt=0;
		for (i=0; i<=2; i++)
			if (*data[i].data)
				{	if (konvert) umlaute(data[i].data);
					len=(int) strlen(data[i].data);
					sprintf(temp,":%02X8%03X00%s",len,bytecnt & 0xfff,convert(data[i].data));
					addchecksum(temp);
					bytecnt += len;
					send(temp);
				}

		sprintf(temp,"%3u",datcnt);
		put_text(tree3,DATAITEM,temp,3); redraw_objc(tree3,DATAITEM,handle);
		xform_do(handle,tree3,NULL,0,1L,NULLFUNC,&wnx,&wny,&wnw,&wnh);

		if (is_state(tree13,REM2MARK,SELECTED) ? TRUE : FALSE)
			send(":008000017F");	/* EOD marked */
		else
			send(":00000001FF");	/* EOD				*/
	}
