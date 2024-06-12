#define HEADEREXT	"XHD"

typedef
  struct MSG_HEADERtag             
    { char    sFrom[36];           /* User who created msg (0-terminated)  */
      char    sTo[36];             /* User who may read msg (0-terminated) */
      char    sSubject[72];        /* Subject (0-terminated)               */
      char    sDate[20];           /* Date/Time string of message          */
      long    lDate;               /* Date when msg was imported (UTC)     */

      ulong   lStart;              /* Start offset of message              */
      ulong   Size;                /* Length of msg                        */

      ulong   lMsgidcrc;           /* LED comment ^MSGID crc               */
      ulong   lReplycrc;           /* LED comment ^REPLY crc               */

      uword   wUp;                 /* QBBS reply links (1)                 */
      uword   wParent;             /* QBBS reply links (1)                 */
      uword   wReadcount;          /* QBBS                                 */
      uword   wCost;               /* QBBS                                 */

      uword   wFlags;              /* Message flags                        */
      ulong   XFlags; 	           /* Extended message flags               */
      ulong   MFlags;	             /* Maus message flags (XF_MAUSMSG set)  */

      ulong   lProcessed;

      char    sDomain[13];         /* Domain (0-terminated)                */
      byte    bReserved;           /* reserved by AFC (AtariFidoCommittee) */
      uword   wFromZone;           /* Zone of sender                       */
      uword   wFromNet;            /* Net...                               */
      uword   wFromNode;           /* Node...                              */
      uword   wFromPoint;          /* Point...                             */
      uword   wToZone;             /* Zone of receiver                     */
      uword   wToNet;              /* Net...                               */
      uword   wToNode;             /* Node...                              */
      uword   wToPoint;            /* Point...                             */
      uword   wReserved[10];       /* reserved by ATSC                     */
    } HDR;

/*
** (1) only for backward compatibility. Use "lMsgidcrc" and "lReplycrc"
** instead!
*/

/*
** Flags: lProcessed
** Write to AFSC for others...
*/

#define PROC_JETMAIL            (1UL << 0)
#define PROC_AU_MSGCHECK        (1UL << 1)
#define PROC_AU_FILEMGR         (1UL << 2)
#define PROC_CHARMODIFY         (1UL << 3)
#define PROC_CONNECTR           (1UL << 4)
#define PROC_DOORMAIL           (1UL << 5)
#define PROC_ARCED              (1UL << 6)
#define PROC_FIFO               (1UL << 7)

/*
** Flags: wFlags
*/

#define F_PRIVATE               (1U << 0)
#define F_CRASH                 (1U << 1)
#define F_RECEIVED              (1U << 2)
#define F_SENT                  (1U << 3)
#define F_FILEATTACH            (1U << 4)
#define F_INTRANSIT             (1U << 5)
#define F_ORPHAN                (1U << 6)
#define F_KILLIFSENT            (1U << 7)
#define F_KILLSENT              (1U << 7)
#define F_LOCAL                 (1U << 8)
#define F_HOLD                  (1U << 9)
#define F_RESERVED              (1U << 10)
#define F_FILEREQ               (1U << 11)
#define F_RETRECREQ             (1U << 12)
#define F_ISRETREC              (1U << 13)
#define F_AUDITREQ              (1U << 14)
#define F_DELETED               (1U << 15)

/*
** Flags: lXFlags
** Write to FTSC for others...
*/

#define XF_READ                 (1UL << 0)
#define XF_ARCHIVSENT           (1UL << 1)
#define XF_TRUNCFILESENT        (1UL << 2)
#define XF_KILLFILESENT         (1UL << 3)
#define XF_DIRECT               (1UL << 4)
#define XF_ZONEGATE             (1UL << 5)
#define XF_HOSTROUTE            (1UL << 6)
#define XF_LOCK                 (1UL << 7)
#define XF_MAUSMSG              (1UL << 8)
#define XF_GATED                (1UL << 9)
#define XF_CREATEFLOWFILE       (1UL << 10)
#define XF_IMMEDIATE            (1UL << 11)
#define XF_FIXEDADDRESS         (1UL << 12)
#define XF_SIGNATURE            (1UL << 13)
#define XF_MULTIPLECC						(1UL << 14)

/*
** Flags: lMFlags
** Write to ATSC for others...
*/

#define MF_NICHTGELESEN         (1UL << 0)
#define MF_NOTREAD              (1UL << 0)
#define MF_ZURUECK              (1UL << 1)
#define MF_RETURN               (1UL << 1)
#define MF_BEANTWORTET          (1UL << 2)
#define MF_ANSWERED             (1UL << 2)
#define MF_GELESEN              (1UL << 3)
#define MF_GELESEN              (1UL << 3)
#define MF_WEITER               (1UL << 4)
#define MF_CONTINUE             (1UL << 4)
#define MF_MAUSNET              (1UL << 5)
#define MF_ANGEKOMMEN           (1UL << 6)
#define MF_RECEIVED             (1UL << 6)
#define MF_GATEWAY              (1UL << 7)
#define MF_KOPIERT              (1UL << 8)
#define MF_COPIED               (1UL << 8)
#define MF_MAUSTAUSCH           (1UL << 9)
#define MF_UNBEKANNT            (1UL << 10)
#define MF_UNKNOWN              (1UL << 10)
#define MF_INTERESTING1         (1UL << 11)
#define MF_INTERESTING2         (1UL << 12)
#define MF_VERERBEN             (1UL << 13)
#define MF_HEREDITARY           (1UL << 13)
