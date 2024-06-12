/*
**	FIDOPLUS.H  Headerfile for fido software.
**
**  Version 1 Release 1  25.12.1994  (c) St.Slabihoud
*/

#ifndef __FIDOPLUS__
#define __FIDOPLUS__

/*
**  <*.HDX> definition used by LED 1.30 and higher!
*/

typedef
	struct MSG_HEADERtag
	{	char	sFrom[36]; 			/* User who created msg (0-terminated)	*/
		char	sTo[36]; 				/* User who may read msg (0-terminated)	*/
		char	sSubject[72];		/* Subject (0-terminated)								*/
		char	sDate[20]; 			/* Date/Time string of message					*/
		ulong	lDate;					/* Date when Msg was imported (UTC) 		*/

		ulong	lStart; 				/* Start offset of message							*/
		ulong	lSize;					/* Length of msg												*/

		ulong	lMsgidcrc;			/* LED comment ^MSGID crc								*/
		ulong lReplycrc;			/* LED comment ^REPLY crc								*/

		uword wUp;						/* QBBS	reply links											*/
		uword wParent;				/* QBBS reply links											*/
		uword	wReadcount;			/* QBBS																	*/
		uword	wCost;					/* QBBS																	*/

		ulong	lFlags; 				/* Message flags												*/
		ulong	lXFlags;				/* Extended message flags								*/
		ulong	lMFlags;				/* Maus message flags	(XF_MAUSMSG set)	*/

		ulong	lTempprocessed;
    ulong lProcessed;

		char	sDomain[16];		/* Domain	(0-terminated)								*/
		uword	wFromZone;			/* Zone of sender												*/
		uword	wFromNet; 			/* Net...																*/
		uword	wFromNode;			/* Node...															*/
		uword	wFromPoint; 		/* Point...															*/
		uword	wToZone;				/* Zone of receiver											*/
		uword	wToNet; 				/* Net...																*/
		uword	wToNode;				/* Node...															*/
		uword	wToPoint; 			/* Point...															*/
	}	MSG_HEADER;


/* lTempprocessed 	*/
/* lProcessed				*/

#define PROC_JETMAIL			(1UL << 0)
#define PROC_AU_MSGCHECK	(1UL << 1)
#define PROC_AU_FILEMGR		(1UL << 2)
#define PROC_CHARMODIFY		(1UL << 3)
#define PROC_CONNECTR			(1UL << 4)
#define PROC_DOORMAIL			(1UL << 5)
#define PROC_ARCED				(1UL << 6)
#define PROC_FIFO					(1UL << 7)

/* lFlags;					*/

#define F_PRIVATE					(1UL << 0)
#define F_CRASH 					(1UL << 1)
#define F_RECEIVED				(1UL << 2)
#define F_SENT						(1UL << 3)
#define F_WITHFILE				(1UL << 4)
#define	F_FILEATTACH			(1UL << 4)
#define F_FORWARDED 			(1UL << 5)
#define	F_INTRANSIT				(1UL << 5)
#define F_ORPHAN					(1UL << 6)
#define F_KILLIFSENT			(1UL << 7)
#define	F_KILLSENT				(1UL << 7)
#define F_LOCAL 					(1UL << 8)
#define F_HOLD						(1UL << 9)
#define F_SIGNATURE 			(1UL << 10)		/* ECU			*/
#define F_FILEREQ					(1UL << 11)
#define F_RETRECREQ				(1UL << 12)
#define F_ISRETREC				(1UL << 13)
#define F_AUDITREQ				(1UL << 14)
#define F_DELETED 				(1UL << 15)

/* lXFlags					*/

#define XF_READ           (1UL << 0)
#define XF_ARCHIVSENT	    (1UL << 1)
#define XF_TRUNCFILESENT  (1UL << 2)
#define XF_KILLFILESENT   (1UL << 3)
#define XF_DIRECT         (1UL << 4)
#define XF_ZONEGATE       (1UL << 5)
#define XF_HOSTROUTE      (1UL << 6)
#define XF_LOCK           (1UL << 7)
#define XF_MAUSMSG				(1UL << 8)
#define XF_GATED          (1UL << 9)
#define XF_CREATEFLOWFILE (1UL << 10)
#define XF_INTERESTING1   (1UL << 11)
#define XF_INTERESTING2   (1UL << 12)
#define XF_HEREDITARY     (1UL << 13)
#define XF_IMMEDIATE      (1UL << 14)
#define XF_FIXEDADDRESS		(1UL << 15)

/* lMFlags					*/

#define MF_NICHTGELESEN		(1UL << 0)
#define MF_NOTREAD				(1UL << 0)
#define MF_ZURUECK				(1UL << 1)
#define MF_RETURN					(1UL << 1)
#define MF_BEANTWORTET		(1UL << 2)
#define MF_ANSWERED				(1UL << 2)
#define MF_GELESEN				(1UL << 3)
#define MF_GELESEN				(1UL << 3)
#define MF_WEITER					(1UL << 4)
#define MF_CONTINUE				(1UL << 4)
#define MF_MAUSNET				(1UL << 5)
#define MF_ANGEKOMMEN			(1UL << 6)
#define MF_RECEIVED				(1UL << 6)
#define MF_GATEWAY				(1UL << 7)
#define MF_KOPIERT				(1UL << 8)
#define MF_COPIED					(1UL << 8)
#define MF_MAUSTAUSCH			(1UL << 9)
#define MF_UNBEKANNT			(1UL << 10)
#define MF_UNKNOWN				(1UL << 10)

#endif
