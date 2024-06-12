
#ifndef __FIDO__
#define __FIDO__

typedef
  struct HEADERtag             
  { byte    sFrom[36];           /* User who created msg (0-terminated)  */
    byte    sTo[36];             /* User who may read msg (0-terminated) */
    byte    sSubject[72];        /* Subject (0-terminated)               */
    byte    sDate[20];           /* Date/Time string of message          */
    ulong   lDate;               /* Date when msg was imported (UTC)     */
    ulong   lStart;              /* Start offset of message              */
    ulong   _reserved_1;         /*  # reserved (do not use!)            */
    uword   wFlags;              /* Message flags                        */

    ulong   lMsgidcrc;           /* LED comment ^MSGID crc               */
    ulong   lReplycrc;           /* LED comment ^REPLY crc               */
    uword   wMFlags;             /* Maus message flags (XF_MAUSMSG set)  */
    uword   wXFlags;             /* Extended message flags               */
    uword   wTempProcessed;
    uword   wProcessed;

    uword   wSize;               /* Length of msg                        */

    uword   wReadcount;          /* BBS: Read counter                    */
    uword   wCost;               /* BBS: Cost field                      */

    ulong   _reserved_2;         /*  # reserved (do not use!)            */
    uword   wFromZone;           /* Zone of sender                       */
    uword   wFromNet;            /* Net...                               */
    uword   wFromNode;           /* Node...                              */
    uword   wFromPoint;          /* Point...                             */
    uword   wToZone;             /* Zone of receiver                     */
    uword   wToNet;              /* Net...                               */
    uword   wToNode;             /* Node...                              */
    uword   wToPoint;            /* Point...                             */
  } HDR_HEADER;

/*
** Flags: wProcessed
*/

#define PR_JETMAIL              (1U << 15)
#define PR_AU_MSGCHECK          (1U << 14)
#define PR_AU_FILEMGR           (1U << 13)
#define PR_CHARMODIFY           (1U << 12)
#define PR_CONNECTR             (1U << 11)
#define PR_MSGCOPY              (1U << 10)
#define PR_UUCP                 (1U <<  9)
#define PR_CHECKIT				(1U <<	3)
#define PR_DOORMAIL             (1U <<  2)
#define PR_ARCHIVED             (1U <<  1)
#define PR_FIFO                 (1U <<  0)

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
** Flags: wXFlags
*/

#define XF_READ                 (1U << 0)
#define XF_ARCHIVSENT           (1U << 1)
#define XF_TRUNCFILESENT        (1U << 2)
#define XF_KILLFILESENT         (1U << 3)
#define XF_DIRECT               (1U << 4)
#define XF_ZONEGATE             (1U << 5)
#define XF_HOSTROUTE            (1U << 6)
#define XF_LOCK                 (1U << 7)
#define XF_MAUSMSG              (1U << 8)
#define XF_GATED                (1U << 9)
#define XF_CREATEFLOWFILE       (1U << 10)
#define XF_MULTIPLECC           (1U << 11)
#define XF_RESERVED12           (1U << 12)
#define XF_SIGNATURE            (1U << 13)
#define XF_IMMEDIATE            (1U << 14)
#define XF_FIXEDADDRESS         (1U << 15)

/*
** Flags: wMFlags
*/

#define MF_NICHTGELESEN         (1U << 0)
#define MF_NOTREAD              (1U << 0)
#define MF_ZURUECK              (1U << 1)
#define MF_RETURN               (1U << 1)
#define MF_BEANTWORTET          (1U << 2)
#define MF_ANSWERED             (1U << 2)
#define MF_GELESEN              (1U << 3)
#define MF_READ                 (1U << 3)
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

#endif


Format of date/time string of message in sDate[20] (ascii format):

  DAY [ ] MONTH [ ] JEAR [ ][ ] HOUR [:] MINUTE [:] SECOND [0]
        
    DAY: [00] ... [31]
  MONTH: [Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec]
   YEAR: [00] ... [99]
   HOUR: [00] ... [23]
 MINUTE: [00] ... [59]
 SECOND: [00] ... [59]


Calculation of Msgidcrc (and Replycrc):

  ===== SOURCE =======================================================
  ulong get_msgid(...)
  { static word count;
    byte crc_msgid[256],msgid[256];
    byte *cp;
    ulong msgidcrc;

    sprintf(msgid,"\x1MSGID: %s %8.8lx\n",
                        mergeaddr(zone,net,node,point,domain),
                        time(NULL)+count);
    [...]
    strcpy(crc_msgid,msgid+8);
    cp=strchr(crc_msgid,'\n');
    cp=EOS;
    msgidcrc = crc32str(crc_msgid);

    count++;
    return(msgidcrc);
  }
  ====================================================================


Calculation of 32-BIT ANSI X3.66 CRC checksum:

  ===== CRC-32 Tab removed (see FTSC for more information) ===========
  The polynomial is
  X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0

  static ulong crc_32_tab[] = { /* CRC polynomial 0xedb88320 */
     0x00000000L, 0x77073096L, [...], 0x5a05df1bL, 0x2d02ef8dL };

  ===== SOURCE =======================================================
  #define UPDC32(octet, crc) \
               (crc_32_tab[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

  unsigned long crc32str(char *s)
  { register ulong crc32;
    crc32 = 0xFFFFFFFFL;
    while (*s)
      crc32 = UPDC32((int) *s++, crc32);
    crc32 = ~crc32;
    return (crc32);
  }
  ====================================================================
