/*
 * Vorgehensweise zum Versenden von Fido-Messages
 *
 * Version 1.0 by Jan Kriesten
 *
 */

/*
 * rudiment„res Beispiel eines Programms, das senden will
 * (eine Cookie-Suchroutine habe ich mir mal gespart!):
 */

/*
 * Mag!X Strukturen:
 */
typedef struct
{
  LONG  magic;            /* muž $87654321 sein         */
  VOID  *membot;          /* Ende der AES- Variablen    */
  VOID  *aes_start;          /* Startadresse               */
  LONG  magic2;            /* ist 'MAGX'                 */
  LONG  date;            /* Erstelldatum               */
  VOID  (*chgres)(int res, int txt);/* Aufl”sung „ndern           */
  LONG  (**shel_vector)(void);    /* residentes Desktop         */
  BYTE  *aes_bootdrv;        /* von hieraus wurde gebootet */
  WORD  *vdi_device;        /* vom AES benutzter Treiber  */
  VOID  *reservd1;
  VOID  *reservd2;
  VOID  *reservd3;
  WORD  version;
  WORD  release;
} AESVARS;

/* Cookie MagX --> */

typedef struct
{
  LONG  config_status;
  VOID  *dosvars;
  AESVARS  *aesvars;
} MAGX_COOKIE;

/*
 * Globale Variablen:
 */
GLOBAL MAGX_COOKIE  *MagX;
GLOBAL WORD      ap_id;
GLOBAL BOOLEAN    has_appl_search=FALSE;

GLOBAL WORD main( VOID )
{
  LONG    value;
  WORD    dummy;
 
  MagX  = getcookie( 'MagX', &value ) ? (MAGX_COOKIE *) value : NULL;

  if( ap_id=appl_init( ), ap_id<0 )
    return( -1 );
 
  if( _GemParBlk.globl[0]>=0x400                  ||
    wind_get( 0, 'WX', &dummy, &dummy, &dummy, &dummy)=='WX'  ||
    (MagX && MagX->aesvars && MagX->aesvars->version>=0x200) )
  {
    WORD  out3;
   
    if( appl_getinfo( 4, &dummy, &dummy, &out3, &dummy ) )
      has_appl_search = out3 ? TRUE : FALSE;
  }

  SearchAndSend( "AVALON  ", 1, 0 );
 
  appl_exit( );
 
  return( 0 );   
}

/**---------------------------------------------------------------------------
--
 ** Funktionsname:  GLOBAL WORD SearchAndSend( BYTE *app, LONG value, WORD 
extend
 );
 **    ->  Name der Zielapplikation und zu sendende Werte;
 **    <-  Anzahl der gesendeten Nachrichten (0, wenn kein gefunden
 **      wurde - logisch);
 **
 ** Die Funktion schickt jeder Applikation app eine Nachricht (aužer,
 ** wenn die ID gleich der eigenen ist)
 **/
GLOBAL WORD SearchAndSend( BYTE *app, LONG value, WORD extend )
{
  WORD  rc, found=0;
 
  if( has_appl_search )
  {
    BYTE  name[9];
    WORD  type, id;
   
    rc = appl_search( 0, name, &type, &id );

    while( rc )
    {
      if( !strncmpi( name, app, 8 ) && id!=ap_id )
      {
        ++found;
        SendMessage( id, value, extend );
      }
     
      rc = appl_search( 1, name, &type, &id );
    }
  }
  else
  {
    rc = appl_find( app );

    if( rc>=0 && rc!=ap_id )
    {
      ++found;
      SendMessage( rc, value, extend );
    }
  }
 
  return( found );
}

/**---------------------------------------------------------------------------
--
 ** Funktionsname:  GLOBAL VOID SendMessage( WORD id, LONG value, WORD extend 
);
 **    ->  ID der Zielapplikation und zu sendende Werte;
 **    <-  nichts;
 **
 ** Die Funktion schickt der Applikation id eine Nachricht
 **/
GLOBAL VOID SendMessage( WORD id, LONG value, WORD extend )
{
  WORD  buf[8];
 
  buf[0] = FIDO_MSG;
  buf[1] = ap_id;
  buf[2] = 0;
  *((LONG *) &buf[3]) = FM_SEMPER;
  *((LONG *) &buf[5]) = value;
  buf[7] = extend;
 
  appl_write( id, 16, buf );
}

GLOBAL WORD appl_getinfo( WORD ap_gtype, WORD *ap_gout1, WORD *ap_gout2, WORD 
*a p_gout3, WORD *ap_gout4 )
{
  EXTERN AESPB  AES_pb;
  WORD      *xptr;
 
  xptr = _GemParBlk.contrl;
 
  *xptr++ = 130;
  *xptr++ = 1;
  *xptr++ = 5;
  *xptr++ = 0;
  *xptr   = 0;
 
  _GemParBlk.intin[0] = ap_gtype;

  _crystal( &AES_pb );

  xptr = &_GemParBlk.intout[4];
   
  *ap_gout4 = *xptr--;
  *ap_gout3 = *xptr--;
  *ap_gout2 = *xptr--;
  *ap_gout1 = *xptr--;

  return( *xptr );
}
