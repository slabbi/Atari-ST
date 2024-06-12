#define HEADEREXT	"HDR"

typedef union
{	struct
	{	uword up;
		uword parent;
	} upparent;
	ulong	midcrc;		
} MID;

typedef union
{	struct
	{	uword reads;
		uword cost;
	} readscost;
	ulong 	repcrc;
} REP;

typedef struct H
{
	char	sFrom[36],		/* User who created msg					*/
				sTo[36],			/* User who may read msg				*/
				sSubject[72],	/* Topic of msg									*/
				sDate[20];		/* Date/Time string of message	*/
	long	lDate,				/* Date when MSGS was received	*/
				lStart;				/* Start offset of message			*/

	MID		mid;		

	uword	wFlags;			/* Attribute bits					*/
	ulong	lMsgidcrc;	/* Mailer 0/1							*/
	ulong lReplycrc;	/* Mailer 2/3							*/
	uword	MFlags;			/* Mailer 4								*/
	uword	XFlags;			/* Mailer 5								*/
	uword	free6;			/* Mailer 6								*/
	uword wProcessed;	/* Mailer 7								*/
	uword	Size;				/* # bytes in message			*/

	REP		rep;		

	uword	wFromZone,			/* Zone on which msg was createt	*/
				wFromNet,				/* Net on which msg was created		*/
				wFromNode,			/* Node on which msg was created	*/
				wFromPoint,			/* Point on which msg was created	*/
				wToZone,				/* Destination zone for msg				*/
				wToNet,					/* Destination net for msg				*/
				wToNode,				/* Destination node for msg				*/
				wToPoint;				/* Destination point for msg			*/
} HDR;

#define F_PRIVATE					(1U << 0)
#define F_CRASH 					(1U << 1)
#define F_RECEIVED				(1U << 2)
#define F_SENT						(1U << 3)
#define	F_FILEATTACH			(1U << 4)
#define	F_INTRANSIT				(1U << 5)
#define F_ORPHAN					(1U << 6)
#define	F_KILLSENT				(1U << 7)
#define F_LOCAL 					(1U << 8)
#define F_HOLD						(1U << 9)
#define F_RESERVED	 			(1U << 10)
#define F_FILEREQ					(1U << 11)
#define F_RETRECREQ				(1U << 12)
#define F_ISRETREC				(1U << 13)
#define F_AUDITREQ				(1U << 14)
#define F_DELETED 				(1U << 15)

/* extended flags in mailer[5] */

#define XF_READ             (1U << 0)
#define XF_ARCHIVSENT	      (1U << 1)
#define XF_TRUNCFILESENT    (1U << 2)
#define XF_KILLFILESENT     (1U << 3)
#define XF_DIRECT           (1U << 4)
#define XF_ZONEGATE         (1U << 5)
#define XF_HOSTROUTE        (1U << 6)
#define XF_LOCK             (1U << 7)
#define XF_MAUSMSG					(1U << 8)
#define XF_GATED            (1U << 9)
#define XF_CREATEFLOWFILE   (1U << 10)
#define XF_MULTIPLECC				(1U << 11)
#define XF_RESERVED12				(1U << 12)
#define XF_SIGNATURE        (1U << 13)
#define XF_IMMEDIATE        (1U << 14)
#define XF_FIXEDADDRESS			(1U << 15)
         
/* maus flags in mailer[4] */ 

#define MF_NICHTGELESEN         (1U << 0)
#define MF_NOTREAD              (1U << 0)
#define MF_ZURUECK              (1U << 1)
#define MF_RETURN               (1U << 1)
#define MF_BEANTWORTET          (1U << 2)
#define MF_ANSWERED             (1U << 2)
#define MF_GELESEN              (1U << 3)
#define MF_GELESEN              (1U << 3)
#define MF_WEITER               (1U << 4)
#define MF_CONTINUE             (1U << 4)
#define MF_MAUSNET              (1U << 5)
#define MF_ANGEKOMMEN           (1U << 6)
#define MF_RECEIVED             (1U << 6)
#define MF_GATEWAY              (1U << 7)
#define MF_KOPIERT              (1U << 8)
#define MF_COPIED               (1U << 8)
#define MF_MAUSTAUSCH           (1U << 9)
#define MF_UNBEKANNT            (1U << 10)
#define MF_UNKNOWN              (1U << 10)
#define MF_INTERESTING1         (1U << 11)
#define MF_INTERESTING2         (1U << 12)
#define MF_VERERBEN             (1U << 13)
#define MF_HEREDITARY           (1U << 13)
#define MF_RESERVED14           (1U << 14)
#define MF_RESERVED15           (1U << 15)
