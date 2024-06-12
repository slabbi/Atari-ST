#ifndef __FIDO__
#define __FIDO__

typedef
	struct MSG_HEADERtag
	{	char	szFrom[36]; 			/* Name des Absenders (0-terminiert)				*/
		char	szTo[36]; 				/* Name des Empfaengers (0-terminiert)			*/
		char	szSubject[72];		/* Stichwort (0-terminiert) 								*/
		char	szDate[20]; 			/* Datum, an dem die Mail geschrieben wurde */
		long	lDate;						/* Datum, an dem sie importiert wurde (UTC) */
		long	lStart; 					/* Anfang des Textes in der MSG Datei				*/

		int 	wUp;							/* QBBS reply links 												*/
		int 	wReply;

		int		wFlags; 					/* Message-Flags (s.u.)											*/

		long	lMsgidcrc;				/* Unused 0/1																*/
		long	lReplycrc;				/*        2/3																*/
		int		wMailer4;					/*				4																	*/
		int		wXFlags;					/*				5																	*/
		int		wTempprocessed;		/*				6																	*/
	struct
	{	int		JetMail			:	1,
					AU_MsgCheck	:	1,
					AU_FileMgr	:	1,
					CharModify	:	1,
          ConnectR    : 1,
          MsgCopy			: 1,
          Uucp				: 1,	/* was FiST */
					unused			: 5,
					Checkit			: 1,
					Doormail		: 1,
					Arced				:	1,
					FiFo				:	1;
	} wProcessed;							/*				7																	*/

		int		wSize;						/* Laenge der Msg in der MSG Datei					*/

		int		wReadcount;
		int		wCost;

		int		wFromZone;				/* Zone des Absenders												*/
		int		wFromNet; 				/* Netz...																	*/
		int		wFromNode;				/* Node...																	*/
		int		wFromPoint; 			/* Point...																	*/
		int		wToZone;					/* Zone des Empfaengers											*/
		int		wToNet; 					/* Netz...																	*/
		int		wToNode;					/* Node...																	*/
		int		wToPoint; 				/* Point...																	*/
	}	MSG_HEADER;

#define MF_PRIVATE		(1U << 0)
#define MF_CRASH 			(1U << 1)
#define MF_RECEIVED		(1U << 2)
#define MF_SENT				(1U << 3)
#define MF_WITHFILE		(1U << 4)
#define	MF_FILEATTACH	(1U << 4)
#define MF_FORWARDED 	(1U << 5)
#define	MF_INTRANSIT	(1U << 5)
#define MF_ORPHAN			(1U << 6)
#define MF_KILLIFSENT	(1U << 7)
#define	MF_KILLSENT		(1U << 7)
#define MF_LOCAL 			(1U << 8)
#define MF_HOLD				(1U << 9)
#define MF_SIGNATURE 	(1U << 10)		/* ECU intern */
#define	MF_UNUSED			(1U << 10)
#define MF_FILEREQ		(1U << 11)
#define MF_RETRECREQ	(1U << 12)
#define MF_ISRETREC		(1U << 13)
#define MF_AUDITREQ		(1U << 14)
#define MF_DELETED 		(1U << 15)

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
#define XF_MULTIPLECC 	    (1U << 11)
#define XF_RESERVED12 	    (1U << 12)
#define XF_HEREDITARY       (1U << 13)
#define XF_IMMEDIATE        (1U << 14)
#define XF_FIXEDADDRESS			(1U << 15)

#define PR_JETMAIL					(1U << 15)
#define PR_AU_MSGCHECK			(1U << 14)
#define PR_AU_FILEMGR				(1U << 13)
#define PR_CHARMODIFY				(1U << 12)
#define PR_CONNECTR					(1U << 11)
#define PR_MSGCOPY					(1U << 10)
#define PR_FIST							(1U <<  9)
#define PR_DOORMAIL					(1U <<  2)
#define PR_ARCHIVED					(1U <<  1)
#define PR_FIFO							(1U <<  0)

#endif
