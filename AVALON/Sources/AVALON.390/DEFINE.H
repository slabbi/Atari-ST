/******************************************************************/
/* DEFINE.H fr AVALON																						*/
/******************************************************************/

#ifndef __AV_DEFINE__
#define __AV_DEFINE__

#define	UNKNOWN					0
#define	TEMPORARY				1
#define REGISTERED			2

#define BINKLEY					0
#define SEMPER					1

#define OB_X(tree,index)			tree[index].ob_x
#define OB_Y(tree,index)			tree[index].ob_y
#define OB_W(tree,index)			tree[index].ob_width
#define OB_H(tree,index)			tree[index].ob_height
#define OB_SPEC(tree,index)		tree[index].ob_spec
#define OB_TYPE(tree,index)		tree[index].ob_type
#define OB_FLAGS(tree,index)	tree[index].ob_flags
#define OB_STATE(tree,index)	tree[index].ob_state
#define MAX(x,y)	((x)>(y)) ? (x) : (y)
#define MIN(x,y)	((x)<(y)) ? (x) : (y)
#define BOOLEAN(x)	(x) ? TRUE : FALSE

#define FALSE 					0
#define TRUE						1
#define	EOS							'\0'
#define MAX_STR					200				/* Nicht UNTER 200!!! */
#define FREQLEN					77
#define FREQLINES				18
#define FREQLINESADD		20
#define FREQSETUPS			3					/* kann beliebig variiert werden */
#define MAXSESSION			6
#define MAXPRGSESS			12
#define MAXLINESMSG			2048

#define FIXLINES				10
#define FIXLINESADD			20

#define MINAREAS				25
#define MAXAREAS				500
#define MINMANAGER			50
#define MAXMANAGER			7500
#define MINOUTBOUND			20
#define MAXOUTBOUND			500

/* MAX_ADDRESS >= MAX_PRESET */

#define MAX_DOMAIN			10
#define MAX_ADDRESS			10
#define MAX_PRESET			8

#define MAX_INFOBT300		6					/* pro Spalte */
#define	MAX_LABEL				100
#define MAX_LABEL_LEN		20
#define	MAX_UTILS				6
#define	MAX_SETUP				6
#define MAX_EPROG				15
#define	MAX_GETFILE			4
#define MAX_HATCH				9					/* kann beliebig variiert werden */
#define AREAS_OTHER			8
#define AREAS_OTHERLEN	65

#define BUFFER_SIZE			32768UL		/* Buffer used by fread		*/
#define PHYS_BUFFER			32768UL		/* Buffer set by setvbuf	*/

/*	AES-Abh„ngigkeiten		*/
#define XFSELECT			0x140				/* Erweiterter Fileselector			*/

/*	GEMDOS-Abh„ngigkeiten	*/
#define XRSCONF				0x1500			/* Rsconf() - Baudratenabfrage	*/

/* ================================== w_session[session][prg][0] = */
/* ======================================== ExitErrorCode in [1] = */
#define F_PRGWAIT			(1U << 0)
#define F_BATCH				(1U << 1)
#define F_REDIR				(1U << 2)
/* ===================================== sflags[session][0] ====== */
/* =========================================== Loops in [1] ====== */
#define F_WAITALL			(1U << 0)
#define F_NOMAIL			(1U << 1)
#define F_AUTOPOLL		(1U << 2)
#define	F_MAIN				(1U << 3)
#define	F_SET1				(1U << 4)
#define	F_SET2				(1U << 5)
#define	F_SET3				(1U << 6)
#define F_SET4				(1U << 7)
#define F_SET5				(1U << 8)
#define F_LOOP				(1U << 9)
#define F_SELECTED		(1U << 10)
/* ==================================================== divflags = */
#define D_DIRECTWAIT	(1LU << 0)
#define D_CHECKING		(1LU << 1)
#define D_NOAVALONBOX	(1LU << 2)
#define D_SAVE				(1LU << 3)
#define D_USE4D				(1LU << 4)
#define D_USE3DTIC		(1LU << 5)
#define D_USEDELAY		(1LU << 6)
#define D_USESMART		(1LU << 7)
#define D_USEDBBS			(1LU << 8)
#define D_KSHATCH			(1LU << 9)
#define D_KSFIX				(1LU << 10)
#define D_REDIRECT		(1LU << 13)
#define D_NOLINEA			(1LU << 14)
#define D_SNAP				(1LU << 15)
#define D_CENTER			(1LU << 16)
#define D_MAXIWIND		(1LU << 17)
/* #define	D_USETOSRUN (1LU << 18) */
#define	D_BOXSMALL		(1LU << 19)
#define	D_USEBUFFER		(1LU << 20) /* Auch in WINDMINI.C definiert */
#define D_USEOUT36		(1LU << 21)
/* ================================================== hatchflags = */
#define D_TOUCH				(1U << 0)
#define D_TIC					(1U << 1)
#define	D_MAILFILE		(1U << 2)
#define	D_NONE				(1U << 3)

#define D_NETMAIL			(1U << 6)
#define D_REPORT1			(1U << 7)
#define D_REPORT2			(1U << 8)
/* ============================================ commandlineflags = */
#define D_NOAUTOBATCH	(1U << 0)
#define D_NOAUTOSTART	(1U << 1)
#define D_NODELAY			(1U << 2)
#define D_OVERRIDE		(1U << 3)
#define D_STARTBATCH	(1U << 4)
/* ===================================================== changes = */
#define	C_SPECIAL			(1U << 0)
#define	C_SESSION			(1U << 1)
#define	C_PROGRAMS		(1U << 2)
#define	C_UTILITIES		(1U << 3)
#define C_FILELIST		(1U << 4)
#define	C_HATCHMAIL		(1U << 5)
#define C_HATCHDEF		(1U << 6)
#define C_TERMINAL		(1U << 7)
#define C_EPROGRAMS		(1U << 8)
/* =================================================== pipeflags = */
#define P_SEMP_REREAD (1U << 0)
/* =============================================================== */

typedef struct AREASBBStag
	{	char	name[49],
					path[61],
					origin[61],
					np[181],
					address[41],
					areasbbs[AREAS_OTHER][AREAS_OTHERLEN];
		int		days;
	} AREAS_BBS;

typedef struct LABLEtag
	{	char	lable[MAX_LABEL_LEN+1];
		long	position;
	} LABLE;

typedef struct MANAGERtag
	{	char	name[49],
					fixpassword[26],
					fixname[11],
					domain[26];
		int		flag,
					boss[3];										/* 0=Net  1=Node  2=Zone */
	} MANAGER;

typedef struct OUTBOUNDtag
	{	char	filename[14],
					netz[14],
					typ[40];
		long	size;
		int		zone,
					net,
					node,
					point;
	} OUTBOUND;
		
#endif
