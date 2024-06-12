/****	wind_init(WIND *wind,vdi_handle,kind,fontpnt,snap,mode,
									clear,x,y,w,h)

		wind				- Zeiger auf WIND-Struktur
		vdi_handle	-	Vdi-Handle (wird in WIND-Struktur gesichert)
		kind				-	Windowelemente
		fontpnt			- Punktgr”že des Fonts.
		snap				- TRUE = Window wird auf 16er-Koordinate gesnapped.
		mode				-	FALSE = Komplette Angabe in Punkten
									TRUE	= x,y in Punkten; w,h in Zeichen
		clear				- L”scht Bildschirmbuffer

		Achtung: Window darf nicht mehr als 80x25 Zeichen umfassen!
						 Ansonsten WIND_MAX_X und WIND_MAX_Y anpassen!

***** wind_init_full(WIND *wind,vdi_handle,kind,fontpnt,center,snap,
											clear,x,y,w,h)

		Wie "wind_init", aber es wird versucht das Window in der
		Gr”že 80x25 Zeichen zu erzeugen (zentriert). Falls dieses in
		der vertikalen und/oder horizontalen nicht m”glich ist,
		dann werden die Punkt-Koordinaten x/w und/oder y/h benutzt.

		center			- Immer zentriert darstellen.

*****	int wind_mesag(WIND *wind,int events)
		
		events			- Events auf die gewartet werden soll
									Normalerweise: MU_BUTTON|MU_KEYBD|MU_MESAG|MU_TIMER

		wind.evnt		- Event
		wind.msg[8] - MsgBuffer
		wind.kstate	-	Sondertastenstatus
		wind.kscan	-	Scancode
		wind.mx/my	- Mausposition
		wind.mclicks- Mausklicks
		wind.mstate	-	Maustasten
		->FALSE 		- Abbruch durch Closer

		Diese Funktion sorgt fr den Redraw und sollte in einer
		Schleife abgearbeitet werden.

*****	int wind_top(WIND *wind)
		
		-> TRUE			- Window ist oben (Funktion muž mit "wind_update"
									geklammert sein)
		Ausgaben drfen nur dann erfolgen, wenn unser Window oben
		liegt.
									
*****	wind_write(WIND *wind,char *txt)
		
		Ausgabe einer Zeichenkette OHNE Zeilenvorschub.
		Die Cursorposition wird nicht ver„ndert.
		Es werden keine VT52-Codes emuliert.

*****	wind_print(WIND *wind,char *txt)

		Ausgabe einer Zeichenkette MIT Zeilenvorschub.
		Es werden NUR die VT52-Codes "ESC-p" und "ESC-q" (Invers on/off)
		emuliert.

***** wind_input(WIND *wind,char *text,char *ret,int length)

		Einlesen einer Zeichenkette.
		
		text				- Eingabeaufforderung
		ret					- Rckgabetext
		length			- maximale L„nge

*****	wind_cls(WIND *wind)
		
		Windowinhalt l”schen.
		
***** wind_clip(WIND *wind,mode)

		mode				-	TRUE = Clipping ber Window an, MausCursor aus
									FALSE = Clipping ber Window aus, MausCursor an

*****	wind_putch(WIND *wind,c,vtmode)

		Ausgabe eines Zeichens.

		vtmode			- 0 = TTY
									1 = VT52
									2 = VT100
									|128 : Cursorhandling und Clipping extern
												 (Sonst: Clipping an, Cursor l”schen, falls
												 auf dem Screen, Zeichen ausgeben, Cursor
												 anzeigen, Clipping aus)

***** void wind_cursor(WIND *wind)

		Cursor zeichen bzw. l”schen (XOR-Modus). Routine nur zusammen
		mit WIND_PUTCH(..., vtmode|128) benutzen.

***** int wind_pause(WIND *wind)

			-> Scancode:	UNDO (auch bei Closer)
										ALT_Q
										CNTRL_Q
										KEY_SPACE

***** set_redirect(int linea)
***** reset_redirect(void)

		Hiermit wird die BIOS-Ausgabe ins Window umgeleitet. Es muž
		hierzu die Struktur "wind" (definiert in WINDMINI.H) benutzt
		werden. Fr einfache Ausgaben kann auch eine beliebe andere
		(oder auch mehrere) vom Typ "WIND" definiert werden.
		
		linea				- TRUE = LineA benutzen

*****	putch_bios(int dev,int chr)

		Ausgabe eines Zeichens bers BIOS, wenn die Ausgabe in ein
		Window mit "set_redirect()" umgelenkt wurde.

*****/

#if  !defined( __WINDMINI__ )
#define __WINDMINI__

#ifndef MAX
	#define MAX(x,y)	(((x)>(y)) ? (x) : (y))
#endif
#ifndef MIN
	#define MIN(x,y)	(((x)<(y)) ? (x) : (y))
#endif
#ifndef FALSE
	#define FALSE	0
#endif
#ifndef TRUE
	#define TRUE	1
#endif
#ifndef EOS
	#define EOS	'\0'
#endif

#define WIND_MAX_X 80
#define WIND_MAX_Y 25

#define WPRINT(x) wind_print(&wind,x);
#define WWRITE(x) wind_write(&wind,x);

enum { TTY=0, VT52, VT100 };

typedef long cdecl (*function)(int opcode,...);

typedef struct WINDtag
	{	int b[4],						/* Wind.Border						*/
				w[4],						/* Wind.Work	 						*/
				pxy[8],					/* Wind.Work als pxy			*/
				txy[8],					/* Wind.Text als pxy			*/
				wchar,hchar,
				wbox,hbox,
				max_x,max_y,		/* Maximale x,y-Position	*/
												/* von 0...Zeilen-1				*/
				x,y,						/* Akt. Cursor-Position		*/
				vdi_handle,			/* VDI-Handle							*/
				handle,					/* Window-Handle					*/
				effects,				/* vst_effects						*/
				mode,						/* vswr_mode							*/
				escape,					/* ESC-Status							*/
				wrap,						/* Wrap_on/off						*/
				savex,savey,		/* Save_pos								*/
				cursor,					/* Cursor									*/
				fontpnt,				/* Aktuelle Fontgr”že			*/
				kind;						/* Windowelemente					*/
		int	evnt,						/* Welches Ereignis				*/
				mx,my,					/* Mausposition						*/
				mclicks,				/* Mausklicks							*/
				mstate,					/* Maustaste							*/
				kstate,					/* KeyState								*/
				kscan,					/* KeyScan								*/
				msg[8];					/* EventMulti MsgBuffer		*/
		char screen[WIND_MAX_Y][WIND_MAX_X+1];
	} WIND;

extern WIND wind;

int		wind_mesag(WIND *wind,int events);
int		wind_top(WIND *wind);
int		wind_pause(WIND *wind);
void	wind_init(WIND *wind,int vdi_handle,int kind,int fontpnt,int snap,int mode,int clear,int x,int y,int w,int h);
void	wind_init_full(WIND *wind,int vdi_handle,int kind,int fontpnt,int center,int snap,int clear,int x,int y,int w,int h);
void	wind_print(WIND *wind,char *txt);
void	wind_write(WIND *wind,char *txt);
void	wind_input(WIND *wind,char *text,char *ret,int length);
void	wind_cls(WIND *wind);
void	wind_putch(WIND *wind,int c,int vt52);
void	wind_clip(WIND *wind,int mode);
void	wind_cursor(WIND *wind);

void	set_redirect(int linea);
void	reset_redirect(void);
void	putch_bios(int dev,int chr);

#endif
