/* GEM Resource C Source */

#include <portab.h>
#include <aes.h>
#include "CASIO_SF.H"

#if !defined(WHITEBAK)
#define WHITEBAK    0x0040
#endif
#if !defined(DRAW3D)
#define DRAW3D      0x0080
#endif

#define FLAGS9  0x0200
#define FLAGS10 0x0400
#define FLAGS11 0x0800
#define FLAGS12 0x1000
#define FLAGS13 0x2000
#define FLAGS14 0x4000
#define FLAGS15 0x8000
#define STATE8  0x0100
#define STATE9  0x0200
#define STATE10 0x0400
#define STATE11 0x0800
#define STATE12 0x1000
#define STATE13 0x2000
#define STATE14 0x4000
#define STATE15 0x8000

TEDINFO rs_tedinfo[] =
{ "12312341234",
  "V_.__ (c) Stephan Slabihoud 1994   (____/____,8N1)",
  "XXXXXXXXXXX",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 12, 51,
  "Empfangen (einzeln TEL,BUS,MEM,SCD,CAL,REM,FRF,WEE,TOD) \007A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Empfangen (komplett CBU)                                \007B",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Empfangen (einzeln ASCII)                               \007C",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Senden (CBU,TEL,BUS,MEM,SCD,CAL,REM,FRF,WEE,TOD)        \007D",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Telephonliste senden (Export-Datei)                     \007E",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Telephon     (TEL)                                      \007F",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Termine      (SCHEDULE)                                 \007G",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Visitenkarte (BUSINESS CARD)                            \007H",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Notiz        (MEMO)                                     \007I",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Kalender     (CALENDAR)                                 \007J",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Erinnerung   (REMINDER)                                 \007K",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Erinnerung   (SCHEDULE)                                 \007L",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Programm beenden                                        \007Q",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 59, 1,
  "Registrieren              \007R",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 29, 1,
  "Konfigurieren             \007S",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 29, 1,
  "11111111111111111111111111111111111111111111111111",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 1,
  "11111111111111111111111111111111111111111111111111",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 51, 1,
  "123",
  "Datensatz: ___",
  "XXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 4, 15,
  "1234567890",
  "Suche nach: __________",
  "xxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 23,
  "11223333",
  "Datum: __/__/____",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 18,
  "12001400",
  "Zeit: __:__ ~ __:__",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 20,
  "1200",
  "Alarm: __:__",
  "9999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 13,
  "11111111111111111111111111111111111111111111111111",
  "Text: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 57,
  "11111111111111111111111111111111111111111111111111",
  "__________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 51,
  "11111111111111111111111111111111111111111111111111",
  "__________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 51,
  "Ctrl+M = CR",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11",
  "__",
  "99",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 3,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "11111111111111111111111111111111",
  "________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 33, 33,
  "Ctrl+M = CR",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "11111111111111111111111111111111111111111111111111",
  "Name: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 57,
  "11111111111111111111111111111111111111111111111111",
  "Firma: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 58,
  "11111111111111111111111111111111111111111111111111",
  "Telephon: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 61,
  "11111111111111111111111111111111111111111111111111",
  "Position: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 61,
  "11111111111111111111111111111111111111111111111111",
  "Abteilung: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 62,
  "11111111111111111111111111111111111111111111111111",
  "Postfach: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 61,
  "11111111111111111111111111111111111111111111111111",
  "Adresse: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 60,
  "11111111111111111111111111111111111111111111111111",
  "Telex: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 58,
  "11111111111111111111111111111111111111111111111111",
  "Fax: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 56,
  "11111111111111111111111111111111111111111111111111",
  "Frei 1: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 2: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 3: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 4: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 5: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 6: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Name: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 57,
  "11111111111111111111111111111111111111111111111111",
  "Telephon: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 61,
  "11111111111111111111111111111111111111111111111111",
  "Adresse: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 60,
  "11111111111111111111111111111111111111111111111111",
  "Frei 1: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 2: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 3: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 4: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 5: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "11111111111111111111111111111111111111111111111111",
  "Frei 6: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 59,
  "121234",
  "Datum: 01/__/____",
  "999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 18,
  "1",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "2",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "3",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "4",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "5",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "6",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "7",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "8",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "9",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "10",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "11",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "12",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "13",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "14",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "15",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "16",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "17",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "18",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "19",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "20",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "21",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "22",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "23",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "24",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "25",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "26",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "27",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "28",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "29",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "30",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "31",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "111111111111111111111111111111",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 31, 1,
  "1111111111111111111111111111111111111111",
  "Name: ________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 41, 47,
  "11111111",
  "Key: ________",
  "xxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 14,
  "1200",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "2400",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "4800",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "9600",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "19200",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "1200",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "2400",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "4800",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "9600",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "19200",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "akt.",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Modem1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Modem2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Ser.1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Ser.2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Ja",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 3, 1,
  "1122",
  "Datum: __/__",
  "9999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 13,
  "F\201r t\204glichen Alarm  : --/--",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 29, 1,
  "F\201r monatlichen Alarm: xx/--",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 29, 1,
  "F\201r j\204hrlichen Alarm : xx/xx",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 29, 1,
  "1200",
  "Alarm: __:__",
  "9999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 13,
  "11111111111111111111111111111111111111111111111111",
  "Text: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 57,
  "11111111111111111111111111111111111111111111111111",
  "__________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 51,
  "11111111111111111111111111111111111111111111111111",
  "__________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 51,
  "Ctrl+M = CR",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "11223333",
  "Von Datum: __/__/____",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 22,
  "11223333",
  "Bis Datum: __/__/____",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 22,
  "12001300",
  "Zeit: __:__ ~ __:__",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 20,
  "1200",
  "Alarm: __:__",
  "9999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 13,
  "11111111111111111111111111111111111111111111111111",
  "Text: __________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 57,
  "11111111111111111111111111111111111111111111111111",
  "__________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 51,
  "11111111111111111111111111111111111111111111111111",
  "__________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 51,
  "Ctrl+M = CR",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1
};

WORD RSBB0DATA[] =
{ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

WORD RSBB1DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x7FFF, 0xFFFC, 0x4000, 0x0006, 
  0x5555, 0x5556, 0x4000, 0x0006, 
  0x7FFF, 0xFFFE, 0x4000, 0x0006, 
  0x4000, 0x0006, 0x47FF, 0xFFC6, 
  0x4800, 0x0026, 0x53FF, 0xFF96, 
  0x5200, 0x0096, 0x5200, 0x0096, 
  0x53FF, 0xFF96, 0x5000, 0x0016, 
  0x4AAA, 0xAAA6, 0x47FF, 0xFFC6, 
  0x4800, 0x0026, 0x52AA, 0xAB96, 
  0x5000, 0x0016, 0x5755, 0x5556, 
  0x5000, 0x0016, 0x52FA, 0xBB96, 
  0x4800, 0x0026, 0x47FF, 0xFFC6, 
  0x4000, 0x0006, 0x7FFF, 0xFFFE, 
  0x3FFF, 0xFFFE, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

WORD RSBB2DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x07FF, 0xFFC0, 
  0x0800, 0x0020, 0x13FF, 0xFF90, 
  0x1200, 0x0090, 0x1200, 0x0090, 
  0x13FF, 0xFF90, 0x1000, 0x0010, 
  0x0AAA, 0xAAA0, 0x07FF, 0xFFC0, 
  0x0800, 0x0020, 0x12AA, 0xAB90, 
  0x1000, 0x0010, 0x1755, 0x5550, 
  0x1000, 0x0010, 0x12FA, 0xBB90, 
  0x0800, 0x0020, 0x07FF, 0xFFC0, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

BITBLK rs_bitblk[] =
{ RSBB0DATA,   2,   4,   0,   0, 0x0001,
  RSBB1DATA,   4,  32,   0,   0, 0x0001,
  RSBB2DATA,   4,  24,   0,   0, 0x0001
};

OBJECT rs_object[] =
{ 
  /******** Tree 0 DIALOG1 ****************************************************/
        -1, MAINVERS        , CONFIGUR        , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x003F, 0x0015,
  EMPFE           ,       -1,       -1, G_FTEXT           ,   /* Object 1 MAINVERS */
  NONE, NORMAL, (LONG)&rs_tedinfo[0],
  0x0000, 0x0000, 0x003F, 0x0001,
  EMPFK           ,       -1,       -1, G_BOXTEXT         ,   /* Object 2 EMPFE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[1],
  0x0002, 0x0002, 0x003B, 0x0001,
  EMPFASCI        ,       -1,       -1, G_BOXTEXT         ,   /* Object 3 EMPFK */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[2],
  0x0002, 0x0003, 0x003B, 0x0001,
  SENDALL         ,       -1,       -1, G_BOXTEXT         ,   /* Object 4 EMPFASCI */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[3],
  0x0002, 0x0004, 0x003B, 0x0001,
  SENDTEL         ,       -1,       -1, G_BOXTEXT         ,   /* Object 5 SENDALL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[4],
  0x0002, 0x0006, 0x003B, 0x0001,
  SENDADR         ,       -1,       -1, G_BOXTEXT         ,   /* Object 6 SENDTEL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[5],
  0x0002, 0x0007, 0x003B, 0x0001,
  SENDTERM        ,       -1,       -1, G_BOXTEXT         ,   /* Object 7 SENDADR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[6],
  0x0002, 0x0009, 0x003B, 0x0001,
  SENDBUS         ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 SENDTERM */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[7],
  0x0002, 0x000A, 0x003B, 0x0001,
  SENDMEMO        ,       -1,       -1, G_BOXTEXT         ,   /* Object 9 SENDBUS */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[8],
  0x0002, 0x000B, 0x003B, 0x0001,
  SENDCAL         ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 SENDMEMO */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[9],
  0x0002, 0x000C, 0x003B, 0x0001,
  SENDREM         ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 SENDCAL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[10],
  0x0002, 0x000D, 0x003B, 0x0001,
  SENDREM2        ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 SENDREM */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[11],
  0x0002, 0x000E, 0x003B, 0x0001,
  EXITPRG         ,       -1,       -1, G_BOXTEXT         ,   /* Object 13 SENDREM2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[12],
  0x0002, 0x000F, 0x003B, 0x0001,
  REGISTER        ,       -1,       -1, G_BOXTEXT         ,   /* Object 14 EXITPRG */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[13],
  0x0002, 0x0011, 0x003B, 0x0001,
  CONFIGUR        ,       -1,       -1, G_BOXTEXT         ,   /* Object 15 REGISTER */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[14],
  0x0002, 0x0013, 0x001D, 0x0001,
         0,       -1,       -1, G_BOXTEXT         ,   /* Object 16 CONFIGUR */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[15],
  0x0020, 0x0013, 0x001D, 0x0001,
  
  /******** Tree 1 DIALOG2 ****************************************************/
        -1,        1, LINESEND        , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004B, 0x0014,
  MARKON          ,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x002C, 0x0001, 0x0013, 0x0001,
         3,       -1,       -1, G_BUTTON          ,   /* Object 2 MARKON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0040, 0x0001, 0x0008, 0x0001,
  IGNOON          ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Ignoriere leere Eintr\204ge in Zeile 5 bis 9:",
  0x0015, 0x0002, 0x002A, 0x0001,
  INFOSTAT        ,       -1,       -1, G_BUTTON          ,   /* Object 4 IGNOON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0040, 0x0002, 0x0008, 0x0001,
  DATABOX         ,       -1,       -1, G_TEXT            ,   /* Object 5 INFOSTAT */
  NONE, NORMAL, (LONG)&rs_tedinfo[16],
  0x0002, 0x0004, 0x0046, 0x0001,
  LINEEXIT        ,        7,       17, G_BOX             ,   /* Object 6 DATABOX */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0002, 0x0006, 0x0046, 0x000B,
        17, LINE1           , LINE9           , G_IBOX            ,   /* Object 7  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0001, 0x0041, 0x000A,
  LINE2           ,       -1,       -1, G_STRING          ,   /* Object 8 LINE1 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0000, 0x0041, 0x0001,
  LINE3           ,       -1,       -1, G_STRING          ,   /* Object 9 LINE2 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0001, 0x0041, 0x0001,
  LINE4           ,       -1,       -1, G_STRING          ,   /* Object 10 LINE3 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0002, 0x0041, 0x0001,
  LINE5           ,       -1,       -1, G_STRING          ,   /* Object 11 LINE4 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0003, 0x0041, 0x0001,
  LINE6           ,       -1,       -1, G_STRING          ,   /* Object 12 LINE5 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0004, 0x0041, 0x0001,
  LINE7           ,       -1,       -1, G_STRING          ,   /* Object 13 LINE6 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0005, 0x0041, 0x0001,
  LINE8           ,       -1,       -1, G_STRING          ,   /* Object 14 LINE7 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0006, 0x0041, 0x0001,
  LINE9           ,       -1,       -1, G_STRING          ,   /* Object 15 LINE8 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0007, 0x0041, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 16 LINE9 */
  NONE, NORMAL, (LONG)"11111111111111111111111111111111111111111111111111111111111111111",
  0x0000, 0x0008, 0x0041, 0x0001,
  DATABOX         , IMP4            , IMP9            , G_IBOX            ,   /* Object 17  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0044, 0x0004, 0x0002, 0x0007,
  IMP5            ,       -1,       -1, G_BOXCHAR         ,   /* Object 18 IMP4 */
  SELECTABLE, SELECTED, (LONG) ((LONG)'<' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0002, 0x0001,
  IMP6            ,       -1,       -1, G_BOXCHAR         ,   /* Object 19 IMP5 */
  SELECTABLE, SELECTED, (LONG) ((LONG)'<' << 24)|0x00FF1100L,
  0x0000, 0x0001, 0x0002, 0x0001,
  IMP7            ,       -1,       -1, G_BOXCHAR         ,   /* Object 20 IMP6 */
  SELECTABLE, SELECTED, (LONG) ((LONG)'<' << 24)|0x00FF1100L,
  0x0000, 0x0002, 0x0002, 0x0001,
  IMP8            ,       -1,       -1, G_BOXCHAR         ,   /* Object 21 IMP7 */
  SELECTABLE, SELECTED, (LONG) ((LONG)'<' << 24)|0x00FF1100L,
  0x0000, 0x0003, 0x0002, 0x0001,
  IMP9            ,       -1,       -1, G_BOXCHAR         ,   /* Object 22 IMP8 */
  SELECTABLE, SELECTED, (LONG) ((LONG)'<' << 24)|0x00FF1100L,
  0x0000, 0x0004, 0x0002, 0x0001,
        17,       -1,       -1, G_BOXCHAR         ,   /* Object 23 IMP9 */
  SELECTABLE, SELECTED, (LONG) ((LONG)'<' << 24)|0x00FF1100L,
  0x0000, 0x0005, 0x0002, 0x0001,
  LINELOOK        ,       -1,       -1, G_BUTTON          ,   /* Object 24 LINEEXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x0002, 0x0012, 0x0008, 0x0001,
  LINERUN         ,       -1,       -1, G_BUTTON          ,   /* Object 25 LINELOOK */
  SELECTABLE|EXIT, NORMAL, (LONG)"Suche",
  0x0012, 0x0012, 0x0008, 0x0001,
  LINECONT        ,       -1,       -1, G_BUTTON          ,   /* Object 26 LINERUN */
  SELECTABLE|EXIT, NORMAL, (LONG)"Sende fortlaufend",
  0x0022, 0x0012, 0x0012, 0x0001,
  LINESEND        ,       -1,       -1, G_BUTTON          ,   /* Object 27 LINECONT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Weiter",
  0x0036, 0x0012, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 28 LINESEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x0040, 0x0012, 0x0008, 0x0001,
  
  /******** Tree 2 DIALOG3 ****************************************************/
        -1, INFOLINE        , DATAITEM        , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0036, 0x0006,
  DATAITEM        ,       -1,       -1, G_TEXT            ,   /* Object 1 INFOLINE */
  NONE, NORMAL, (LONG)&rs_tedinfo[17],
  0x0002, 0x0001, 0x0032, 0x0001,
         0,       -1,       -1, G_FTEXT           ,   /* Object 2 DATAITEM */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[18],
  0x0002, 0x0003, 0x0032, 0x0001,
  
  /******** Tree 3 DIALOG4 ****************************************************/
        -1, SEARCH          , SEARCHOK        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002A, 0x0003,
  SEARCHOK        ,       -1,       -1, G_FTEXT           ,   /* Object 1 SEARCH */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[19],
  0x0002, 0x0001, 0x0016, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 2 SEARCHOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0020, 0x0001, 0x0008, 0x0001,
  
  /******** Tree 4 DIALOG5 ****************************************************/
        -1, SDATUM          ,       12, G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0041, 0x000E,
         2,       -1,       -1, G_FTEXT           ,   /* Object 1 SDATUM */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[20],
  0x0003, 0x0001, 0x0011, 0x0001,
  SMARKON         ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x0023, 0x0001, 0x0013, 0x0001,
  SZEIT           ,       -1,       -1, G_BUTTON          ,   /* Object 3 SMARKON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0037, 0x0001, 0x0008, 0x0001,
  SALARM          ,       -1,       -1, G_FTEXT           ,   /* Object 4 SZEIT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[21],
  0x0004, 0x0003, 0x0013, 0x0001,
  STEXT1          ,       -1,       -1, G_FTEXT           ,   /* Object 5 SALARM */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[22],
  0x0003, 0x0004, 0x000C, 0x0001,
  STEXT2          ,       -1,       -1, G_FTEXT           ,   /* Object 6 STEXT1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[23],
  0x0004, 0x0006, 0x0038, 0x0001,
  STEXT3          ,       -1,       -1, G_FTEXT           ,   /* Object 7 STEXT2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[24],
  0x000A, 0x0007, 0x0032, 0x0001,
         9,       -1,       -1, G_FTEXT           ,   /* Object 8 STEXT3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[25],
  0x000A, 0x0008, 0x0032, 0x0001,
  SEXIT           ,       -1,       -1, G_TEXT            ,   /* Object 9  */
  NONE, NORMAL, (LONG)&rs_tedinfo[26],
  0x000A, 0x0009, 0x0208, 0x0001,
  SSEND           ,       -1,       -1, G_BUTTON          ,   /* Object 10 SEXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x002B, 0x000A, 0x0009, 0x0001,
        12,       -1,       -1, G_BUTTON          ,   /* Object 11 SSEND */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"Sende",
  0x0036, 0x000A, 0x0009, 0x0001,
         0,       13, TAG16           , G_IBOX            ,   /* Object 12  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x000C, 0x003E, 0x0001,
  TAG1            ,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"weitere Tage:",
  0x0000, 0x0000, 0x000D, 0x0001,
  TAG2            ,       -1,       -1, G_FTEXT           ,   /* Object 14 TAG1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[27],
  0x000E, 0x0000, 0x0002, 0x0001,
  TAG3            ,       -1,       -1, G_FTEXT           ,   /* Object 15 TAG2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[28],
  0x0011, 0x0000, 0x0002, 0x0001,
  TAG4            ,       -1,       -1, G_FTEXT           ,   /* Object 16 TAG3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[29],
  0x0014, 0x0000, 0x0002, 0x0001,
  TAG5            ,       -1,       -1, G_FTEXT           ,   /* Object 17 TAG4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[30],
  0x0017, 0x0000, 0x0002, 0x0001,
  TAG6            ,       -1,       -1, G_FTEXT           ,   /* Object 18 TAG5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[31],
  0x001A, 0x0000, 0x0002, 0x0001,
  TAG7            ,       -1,       -1, G_FTEXT           ,   /* Object 19 TAG6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[32],
  0x001D, 0x0000, 0x0002, 0x0001,
  TAG8            ,       -1,       -1, G_FTEXT           ,   /* Object 20 TAG7 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[33],
  0x0020, 0x0000, 0x0002, 0x0001,
  TAG9            ,       -1,       -1, G_FTEXT           ,   /* Object 21 TAG8 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[34],
  0x0023, 0x0000, 0x0002, 0x0001,
  TAG10           ,       -1,       -1, G_FTEXT           ,   /* Object 22 TAG9 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[35],
  0x0026, 0x0000, 0x0002, 0x0001,
  TAG11           ,       -1,       -1, G_FTEXT           ,   /* Object 23 TAG10 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[36],
  0x0029, 0x0000, 0x0002, 0x0001,
  TAG12           ,       -1,       -1, G_FTEXT           ,   /* Object 24 TAG11 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[37],
  0x002C, 0x0000, 0x0002, 0x0001,
  TAG13           ,       -1,       -1, G_FTEXT           ,   /* Object 25 TAG12 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[38],
  0x002F, 0x0000, 0x0002, 0x0001,
  TAG14           ,       -1,       -1, G_FTEXT           ,   /* Object 26 TAG13 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[39],
  0x0032, 0x0000, 0x0002, 0x0001,
  TAG15           ,       -1,       -1, G_FTEXT           ,   /* Object 27 TAG14 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[40],
  0x0035, 0x0000, 0x0002, 0x0001,
  TAG16           ,       -1,       -1, G_FTEXT           ,   /* Object 28 TAG15 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[41],
  0x0038, 0x0000, 0x0002, 0x0001,
        12,       -1,       -1, G_FTEXT           ,   /* Object 29 TAG16 */
  EDITABLE|LASTOB, NORMAL, (LONG)&rs_tedinfo[42],
  0x003B, 0x0000, 0x0002, 0x0001,
  
  /******** Tree 5 DIALOG6 ****************************************************/
        -1,        1, MSEND           , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0024, 0x0013,
  MMARKON         ,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x0006, 0x0001, 0x0013, 0x0001,
  MBOX            ,       -1,       -1, G_BUTTON          ,   /* Object 2 MMARKON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x001A, 0x0001, 0x0008, 0x0001,
        16, MTEXT1          , MTEXT12         , G_IBOX            ,   /* Object 3 MBOX */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0003, 0x0022, 0x000C,
  MTEXT2          ,       -1,       -1, G_FTEXT           ,   /* Object 4 MTEXT1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[43],
  0x0000, 0x0000, 0x0020, 0x0001,
  MTEXT3          ,       -1,       -1, G_FTEXT           ,   /* Object 5 MTEXT2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[44],
  0x0000, 0x0001, 0x0020, 0x0001,
  MTEXT4          ,       -1,       -1, G_FTEXT           ,   /* Object 6 MTEXT3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[45],
  0x0000, 0x0002, 0x0020, 0x0001,
  MTEXT5          ,       -1,       -1, G_FTEXT           ,   /* Object 7 MTEXT4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[46],
  0x0000, 0x0003, 0x0020, 0x0001,
  MTEXT6          ,       -1,       -1, G_FTEXT           ,   /* Object 8 MTEXT5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[47],
  0x0000, 0x0004, 0x0020, 0x0001,
  MTEXT7          ,       -1,       -1, G_FTEXT           ,   /* Object 9 MTEXT6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[48],
  0x0000, 0x0005, 0x0020, 0x0001,
  MTEXT8          ,       -1,       -1, G_FTEXT           ,   /* Object 10 MTEXT7 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[49],
  0x0000, 0x0006, 0x0020, 0x0001,
  MTEXT9          ,       -1,       -1, G_FTEXT           ,   /* Object 11 MTEXT8 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[50],
  0x0000, 0x0007, 0x0020, 0x0001,
  MTEXT10         ,       -1,       -1, G_FTEXT           ,   /* Object 12 MTEXT9 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[51],
  0x0000, 0x0008, 0x0020, 0x0001,
  MTEXT11         ,       -1,       -1, G_FTEXT           ,   /* Object 13 MTEXT10 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[52],
  0x0000, 0x0009, 0x0020, 0x0001,
  MTEXT12         ,       -1,       -1, G_FTEXT           ,   /* Object 14 MTEXT11 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[53],
  0x0000, 0x000A, 0x0020, 0x0001,
  MBOX            ,       -1,       -1, G_FTEXT           ,   /* Object 15 MTEXT12 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[54],
  0x0000, 0x000B, 0x0020, 0x0001,
  MEXIT           ,       -1,       -1, G_TEXT            ,   /* Object 16  */
  NONE, NORMAL, (LONG)&rs_tedinfo[55],
  0x0002, 0x000F, 0x0208, 0x0001,
  MLOAD           ,       -1,       -1, G_BUTTON          ,   /* Object 17 MEXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x0002, 0x0011, 0x0008, 0x0001,
  MSEND           ,       -1,       -1, G_BUTTON          ,   /* Object 18 MLOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)"Lade",
  0x000E, 0x0011, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 19 MSEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x001A, 0x0011, 0x0008, 0x0001,
  
  /******** Tree 6 DIALOG7 ****************************************************/
        -1, BNAME           , BSEND           , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0041, 0x0016,
  BFIRMA          ,       -1,       -1, G_FTEXT           ,   /* Object 1 BNAME */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[56],
  0x0007, 0x0001, 0x0038, 0x0001,
  BTEL            ,       -1,       -1, G_FTEXT           ,   /* Object 2 BFIRMA */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[57],
  0x0006, 0x0002, 0x0039, 0x0001,
  BPOS            ,       -1,       -1, G_FTEXT           ,   /* Object 3 BTEL */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[58],
  0x0003, 0x0003, 0x003C, 0x0001,
  BABT            ,       -1,       -1, G_FTEXT           ,   /* Object 4 BPOS */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[59],
  0x0003, 0x0005, 0x003C, 0x0001,
  BPOST           ,       -1,       -1, G_FTEXT           ,   /* Object 5 BABT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[60],
  0x0002, 0x0006, 0x003D, 0x0001,
  BADR            ,       -1,       -1, G_FTEXT           ,   /* Object 6 BPOST */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[61],
  0x0003, 0x0008, 0x003C, 0x0001,
  BTELEX          ,       -1,       -1, G_FTEXT           ,   /* Object 7 BADR */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[62],
  0x0004, 0x0009, 0x003B, 0x0001,
  BFAX            ,       -1,       -1, G_FTEXT           ,   /* Object 8 BTELEX */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[63],
  0x0006, 0x000A, 0x0039, 0x0001,
  BFREI1          ,       -1,       -1, G_FTEXT           ,   /* Object 9 BFAX */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[64],
  0x0008, 0x000B, 0x0037, 0x0001,
  BFREI2          ,       -1,       -1, G_FTEXT           ,   /* Object 10 BFREI1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[65],
  0x0005, 0x000D, 0x003A, 0x0001,
  BFREI3          ,       -1,       -1, G_FTEXT           ,   /* Object 11 BFREI2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[66],
  0x0005, 0x000E, 0x003A, 0x0001,
  BFREI4          ,       -1,       -1, G_FTEXT           ,   /* Object 12 BFREI3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[67],
  0x0005, 0x000F, 0x003A, 0x0001,
  BFREI5          ,       -1,       -1, G_FTEXT           ,   /* Object 13 BFREI4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[68],
  0x0005, 0x0010, 0x003A, 0x0001,
  BFREI6          ,       -1,       -1, G_FTEXT           ,   /* Object 14 BFREI5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[69],
  0x0005, 0x0011, 0x003A, 0x0001,
  BEXIT           ,       -1,       -1, G_FTEXT           ,   /* Object 15 BFREI6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[70],
  0x0005, 0x0012, 0x003A, 0x0001,
        17,       -1,       -1, G_BUTTON          ,   /* Object 16 BEXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x0002, 0x0014, 0x0009, 0x0001,
  BMARKON         ,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x0012, 0x0014, 0x0013, 0x0001,
  BSEND           ,       -1,       -1, G_BUTTON          ,   /* Object 18 BMARKON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0026, 0x0014, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 19 BSEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x0036, 0x0014, 0x0009, 0x0001,
  
  /******** Tree 7 DIALOG8 ****************************************************/
        -1, ANAME           , ASEND           , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0040, 0x000E,
  ATEL            ,       -1,       -1, G_FTEXT           ,   /* Object 1 ANAME */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[71],
  0x0006, 0x0001, 0x0038, 0x0001,
  AADR            ,       -1,       -1, G_FTEXT           ,   /* Object 2 ATEL */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[72],
  0x0002, 0x0002, 0x003C, 0x0001,
  AFREI1          ,       -1,       -1, G_FTEXT           ,   /* Object 3 AADR */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[73],
  0x0003, 0x0003, 0x003B, 0x0001,
  AFREI2          ,       -1,       -1, G_FTEXT           ,   /* Object 4 AFREI1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[74],
  0x0004, 0x0005, 0x003A, 0x0001,
  AFREI3          ,       -1,       -1, G_FTEXT           ,   /* Object 5 AFREI2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[75],
  0x0004, 0x0006, 0x003A, 0x0001,
  AFREI4          ,       -1,       -1, G_FTEXT           ,   /* Object 6 AFREI3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[76],
  0x0004, 0x0007, 0x003A, 0x0001,
  AFREI5          ,       -1,       -1, G_FTEXT           ,   /* Object 7 AFREI4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[77],
  0x0004, 0x0008, 0x003A, 0x0001,
  AFREI6          ,       -1,       -1, G_FTEXT           ,   /* Object 8 AFREI5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[78],
  0x0004, 0x0009, 0x003A, 0x0001,
  AEXIT           ,       -1,       -1, G_FTEXT           ,   /* Object 9 AFREI6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[79],
  0x0004, 0x000A, 0x003A, 0x0001,
        11,       -1,       -1, G_BUTTON          ,   /* Object 10 AEXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x0002, 0x000C, 0x0009, 0x0001,
  AMARKON         ,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x0012, 0x000C, 0x0013, 0x0001,
  ASEND           ,       -1,       -1, G_BUTTON          ,   /* Object 12 AMARKON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0026, 0x000C, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 13 ASEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x0035, 0x000C, 0x0009, 0x0001,
  
  /******** Tree 8 DIALOG9 ****************************************************/
        -1, CDATUM          , CSEND           , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0025, 0x0010,
  CBOX            ,       -1,       -1, G_FTEXT           ,   /* Object 1 CDATUM */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[80],
  0x0002, 0x0001, 0x0011, 0x0001,
  CEXIT           , C1              ,       33, G_IBOX            ,   /* Object 2 CBOX */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0003, 0x0021, 0x0009,
         4,       -1,       -1, G_BOXTEXT         ,   /* Object 3 C1 */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[81],
  0x0000, 0x0000, 0x0003, 0x0001,
         5,       -1,       -1, G_BOXTEXT         ,   /* Object 4  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[82],
  0x0005, 0x0000, 0x0003, 0x0001,
         6,       -1,       -1, G_BOXTEXT         ,   /* Object 5  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[83],
  0x000A, 0x0000, 0x0003, 0x0001,
         7,       -1,       -1, G_BOXTEXT         ,   /* Object 6  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[84],
  0x000F, 0x0000, 0x0003, 0x0001,
         8,       -1,       -1, G_BOXTEXT         ,   /* Object 7  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[85],
  0x0014, 0x0000, 0x0003, 0x0001,
         9,       -1,       -1, G_BOXTEXT         ,   /* Object 8  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[86],
  0x0019, 0x0000, 0x0003, 0x0001,
        10,       -1,       -1, G_BOXTEXT         ,   /* Object 9  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[87],
  0x001E, 0x0000, 0x0003, 0x0001,
        11,       -1,       -1, G_BOXTEXT         ,   /* Object 10  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[88],
  0x0000, 0x0002, 0x0003, 0x0001,
        12,       -1,       -1, G_BOXTEXT         ,   /* Object 11  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[89],
  0x0005, 0x0002, 0x0003, 0x0001,
        13,       -1,       -1, G_BOXTEXT         ,   /* Object 12  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[90],
  0x000A, 0x0002, 0x0003, 0x0001,
        14,       -1,       -1, G_BOXTEXT         ,   /* Object 13  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[91],
  0x000F, 0x0002, 0x0003, 0x0001,
        15,       -1,       -1, G_BOXTEXT         ,   /* Object 14  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[92],
  0x0014, 0x0002, 0x0003, 0x0001,
        16,       -1,       -1, G_BOXTEXT         ,   /* Object 15  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[93],
  0x0019, 0x0002, 0x0003, 0x0001,
        17,       -1,       -1, G_BOXTEXT         ,   /* Object 16  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[94],
  0x001E, 0x0002, 0x0003, 0x0001,
        18,       -1,       -1, G_BOXTEXT         ,   /* Object 17  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[95],
  0x0000, 0x0004, 0x0003, 0x0001,
        19,       -1,       -1, G_BOXTEXT         ,   /* Object 18  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[96],
  0x0005, 0x0004, 0x0003, 0x0001,
        20,       -1,       -1, G_BOXTEXT         ,   /* Object 19  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[97],
  0x000A, 0x0004, 0x0003, 0x0001,
        21,       -1,       -1, G_BOXTEXT         ,   /* Object 20  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[98],
  0x000F, 0x0004, 0x0003, 0x0001,
        22,       -1,       -1, G_BOXTEXT         ,   /* Object 21  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[99],
  0x0014, 0x0004, 0x0003, 0x0001,
        23,       -1,       -1, G_BOXTEXT         ,   /* Object 22  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[100],
  0x0019, 0x0004, 0x0003, 0x0001,
        24,       -1,       -1, G_BOXTEXT         ,   /* Object 23  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[101],
  0x001E, 0x0004, 0x0003, 0x0001,
        25,       -1,       -1, G_BOXTEXT         ,   /* Object 24  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[102],
  0x0000, 0x0006, 0x0003, 0x0001,
        26,       -1,       -1, G_BOXTEXT         ,   /* Object 25  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[103],
  0x0005, 0x0006, 0x0003, 0x0001,
        27,       -1,       -1, G_BOXTEXT         ,   /* Object 26  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[104],
  0x000A, 0x0006, 0x0003, 0x0001,
        28,       -1,       -1, G_BOXTEXT         ,   /* Object 27  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[105],
  0x000F, 0x0006, 0x0003, 0x0001,
        29,       -1,       -1, G_BOXTEXT         ,   /* Object 28  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[106],
  0x0014, 0x0006, 0x0003, 0x0001,
        30,       -1,       -1, G_BOXTEXT         ,   /* Object 29  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[107],
  0x0019, 0x0006, 0x0003, 0x0001,
        31,       -1,       -1, G_BOXTEXT         ,   /* Object 30  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[108],
  0x001E, 0x0006, 0x0003, 0x0001,
        32,       -1,       -1, G_BOXTEXT         ,   /* Object 31  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[109],
  0x0000, 0x0008, 0x0003, 0x0001,
        33,       -1,       -1, G_BOXTEXT         ,   /* Object 32  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[110],
  0x0005, 0x0008, 0x0003, 0x0001,
  CBOX            ,       -1,       -1, G_BOXTEXT         ,   /* Object 33  */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[111],
  0x000A, 0x0008, 0x0003, 0x0001,
  CSEND           ,       -1,       -1, G_BUTTON          ,   /* Object 34 CEXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x0002, 0x000E, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 35 CSEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x001B, 0x000E, 0x0008, 0x0001,
  
  /******** Tree 9 DIALOG10 ****************************************************/
        -1, REGINFO         , REGEXIT         , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0032, 0x0007,
  REGNAME         ,       -1,       -1, G_BOXTEXT         ,   /* Object 1 REGINFO */
  NONE, NORMAL, (LONG)&rs_tedinfo[112],
  0x0000, 0x0000, 0x0032, 0x0001,
  REGKEY          ,       -1,       -1, G_FTEXT           ,   /* Object 2 REGNAME */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[113],
  0x0002, 0x0002, 0x002E, 0x0001,
  REGSAVE         ,       -1,       -1, G_FTEXT           ,   /* Object 3 REGKEY */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[114],
  0x0003, 0x0003, 0x000D, 0x0001,
  REGEXIT         ,       -1,       -1, G_BUTTON          ,   /* Object 4 REGSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)"Sichern",
  0x0002, 0x0005, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 5 REGEXIT */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Ende",
  0x0028, 0x0005, 0x0008, 0x0001,
  
  /******** Tree 10 DIALOG11 ****************************************************/
        -1,        1, COK             , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0045, 0x000B,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Daten vom CASIO empfangen mit:",
  0x0002, 0x0001, 0x001E, 0x0001,
         8, E1200           , E19200          , G_IBOX            ,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0021, 0x0001, 0x0024, 0x0001,
  E2400           ,       -1,       -1, G_BOXTEXT         ,   /* Object 3 E1200 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[115],
  0x0000, 0x0000, 0x0006, 0x0001,
  E4800           ,       -1,       -1, G_BOXTEXT         ,   /* Object 4 E2400 */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[116],
  0x0007, 0x0000, 0x0006, 0x0001,
  E9600           ,       -1,       -1, G_BOXTEXT         ,   /* Object 5 E4800 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[117],
  0x000E, 0x0000, 0x0006, 0x0001,
  E19200          ,       -1,       -1, G_BOXTEXT         ,   /* Object 6 E9600 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[118],
  0x0015, 0x0000, 0x0006, 0x0001,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 7 E19200 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[119],
  0x001C, 0x0000, 0x0006, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"Daten zum CASIO senden mit:",
  0x0005, 0x0003, 0x001B, 0x0001,
        15, S1200           , S19200          , G_IBOX            ,   /* Object 9  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0021, 0x0003, 0x0024, 0x0001,
  S2400           ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 S1200 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[120],
  0x0000, 0x0000, 0x0006, 0x0001,
  S4800           ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 S2400 */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[121],
  0x0007, 0x0000, 0x0006, 0x0001,
  S9600           ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 S4800 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[122],
  0x000E, 0x0000, 0x0006, 0x0001,
  S19200          ,       -1,       -1, G_BOXTEXT         ,   /* Object 13 S9600 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[123],
  0x0015, 0x0000, 0x0006, 0x0001,
         9,       -1,       -1, G_BOXTEXT         ,   /* Object 14 S19200 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[124],
  0x001C, 0x0000, 0x0006, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"Schnittstelle:",
  0x0012, 0x0005, 0x000E, 0x0001,
        22, PORT0           , PORT4           , G_IBOX            ,   /* Object 16  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0021, 0x0005, 0x0024, 0x0001,
  PORT1           ,       -1,       -1, G_BOXTEXT         ,   /* Object 17 PORT0 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[125],
  0x0000, 0x0000, 0x0006, 0x0001,
  PORT2           ,       -1,       -1, G_BOXTEXT         ,   /* Object 18 PORT1 */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[126],
  0x0007, 0x0000, 0x0006, 0x0001,
  PORT3           ,       -1,       -1, G_BOXTEXT         ,   /* Object 19 PORT2 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[127],
  0x000E, 0x0000, 0x0006, 0x0001,
  PORT4           ,       -1,       -1, G_BOXTEXT         ,   /* Object 20 PORT3 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[128],
  0x0015, 0x0000, 0x0006, 0x0001,
        16,       -1,       -1, G_BOXTEXT         ,   /* Object 21 PORT4 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[129],
  0x001C, 0x0000, 0x0006, 0x0001,
  SUMLAUTE        ,       -1,       -1, G_STRING          ,   /* Object 22  */
  NONE, NORMAL, (LONG)"Umlaute konvertieren:",
  0x000B, 0x0007, 0x0015, 0x0001,
  CSAVE           ,       -1,       -1, G_BOXTEXT         ,   /* Object 23 SUMLAUTE */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[130],
  0x0021, 0x0007, 0x0006, 0x0001,
  COK             ,       -1,       -1, G_BUTTON          ,   /* Object 24 CSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)"Sichern",
  0x0031, 0x0009, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 25 COK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x003B, 0x0009, 0x0008, 0x0001,
  
  /******** Tree 11 DIALOG12 ****************************************************/
        -1, RDATUM          , RSEND           , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0041, 0x000C,
         2,       -1,       -1, G_FTEXT           ,   /* Object 1 RDATUM */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[131],
  0x0003, 0x0001, 0x000C, 0x0001,
  RMARKON         ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x0023, 0x0001, 0x0013, 0x0001,
         4,       -1,       -1, G_BUTTON          ,   /* Object 3 RMARKON */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0037, 0x0001, 0x0008, 0x0001,
         5,       -1,       -1, G_TEXT            ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_tedinfo[132],
  0x0003, 0x0002, 0x0015, 0x0001,
         6,       -1,       -1, G_TEXT            ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_tedinfo[133],
  0x0003, 0x0802, 0x0015, 0x0001,
  RALARM          ,       -1,       -1, G_TEXT            ,   /* Object 6  */
  NONE, NORMAL, (LONG)&rs_tedinfo[134],
  0x0003, 0x0003, 0x0015, 0x0001,
  RTEXT1          ,       -1,       -1, G_FTEXT           ,   /* Object 7 RALARM */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[135],
  0x0003, 0x0004, 0x000C, 0x0001,
  RTEXT2          ,       -1,       -1, G_FTEXT           ,   /* Object 8 RTEXT1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[136],
  0x0004, 0x0006, 0x0038, 0x0001,
  RTEXT3          ,       -1,       -1, G_FTEXT           ,   /* Object 9 RTEXT2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[137],
  0x000A, 0x0007, 0x0032, 0x0001,
        11,       -1,       -1, G_FTEXT           ,   /* Object 10 RTEXT3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[138],
  0x000A, 0x0008, 0x0032, 0x0001,
  REXIT           ,       -1,       -1, G_TEXT            ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[139],
  0x000A, 0x0009, 0x0208, 0x0001,
  RSEND           ,       -1,       -1, G_BUTTON          ,   /* Object 12 REXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x002B, 0x000A, 0x0009, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 13 RSEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x0036, 0x000A, 0x0009, 0x0001,
  
  /******** Tree 12 DIALOG13 ****************************************************/
        -1, REM2VON         , REM2SEND        , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0041, 0x000F,
         2,       -1,       -1, G_FTEXT           ,   /* Object 1 REM2VON */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[140],
  0x0003, 0x0001, 0x0015, 0x0001,
  REM2MARK        ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"Datensatz markiert:",
  0x0023, 0x0001, 0x0013, 0x0001,
  REM2BIS         ,       -1,       -1, G_BUTTON          ,   /* Object 3 REM2MARK */
  SELECTABLE, NORMAL, (LONG)"Ja",
  0x0037, 0x0001, 0x0008, 0x0001,
  REM2ZEIT        ,       -1,       -1, G_FTEXT           ,   /* Object 4 REM2BIS */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[141],
  0x0003, 0x0002, 0x0015, 0x0001,
  REM2ALAR        ,       -1,       -1, G_FTEXT           ,   /* Object 5 REM2ZEIT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[142],
  0x0008, 0x0003, 0x0013, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 REM2ALAR */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[143],
  0x0007, 0x0004, 0x000C, 0x0001,
  REM2FEST        , REM2YEAR        , REM2WORK        , G_IBOX            ,   /* Object 7  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0004, 0x0006, 0x0039, 0x0001,
  REM2MONT        ,       -1,       -1, G_BUTTON          ,   /* Object 8 REM2YEAR */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"j\204hrlich",
  0x0000, 0x0000, 0x000A, 0x0001,
  REM2WEEK        ,       -1,       -1, G_BUTTON          ,   /* Object 9 REM2MONT */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"monatlich",
  0x000B, 0x0000, 0x000A, 0x0001,
  REM2DAY         ,       -1,       -1, G_BUTTON          ,   /* Object 10 REM2WEEK */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"w\224chentlich",
  0x0016, 0x0000, 0x000B, 0x0001,
  REM2WORK        ,       -1,       -1, G_BUTTON          ,   /* Object 11 REM2DAY */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"t\204glich",
  0x0022, 0x0000, 0x000A, 0x0001,
         7,       -1,       -1, G_BUTTON          ,   /* Object 12 REM2WORK */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"werktags",
  0x002D, 0x0000, 0x000A, 0x0001,
  REM2TXT1        ,       -1,       -1, G_BUTTON          ,   /* Object 13 REM2FEST */
  SELECTABLE, NORMAL, (LONG)"Wochentag",
  0x000F, 0x0007, 0x000A, 0x0001,
  REM2TXT2        ,       -1,       -1, G_FTEXT           ,   /* Object 14 REM2TXT1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[144],
  0x0004, 0x0009, 0x0038, 0x0001,
  REM2TXT3        ,       -1,       -1, G_FTEXT           ,   /* Object 15 REM2TXT2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[145],
  0x000A, 0x000A, 0x0032, 0x0001,
        17,       -1,       -1, G_FTEXT           ,   /* Object 16 REM2TXT3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[146],
  0x000A, 0x000B, 0x0032, 0x0001,
  REM2EXIT        ,       -1,       -1, G_TEXT            ,   /* Object 17  */
  NONE, NORMAL, (LONG)&rs_tedinfo[147],
  0x000A, 0x000C, 0x0208, 0x0001,
  REM2SEND        ,       -1,       -1, G_BUTTON          ,   /* Object 18 REM2EXIT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Ende",
  0x002B, 0x000D, 0x0009, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 19 REM2SEND */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Sende",
  0x0036, 0x000D, 0x0009, 0x0001,
  
  /******** Tree 13 DIALOG99 ****************************************************/
        -1, ICFSPIC         , ICFSPIC         , G_BOX             ,   /* Object 0  */
  SELECTABLE|EXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0008, 0x0004,
         0,       -1,       -1, G_IMAGE           ,   /* Object 1 ICFSPIC */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)&rs_bitblk[0],
  0x0002, 0x0001, 0x0002, 0x0400,
  
  /******** Tree 14 PICTURE0 ****************************************************/
        -1, ICFS0           , ICFS1           , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0012, 0x0008,
  ICFS1           ,       -1,       -1, G_IMAGE           ,   /* Object 1 ICFS0 */
  NONE, NORMAL, (LONG)&rs_bitblk[1],
  0x0002, 0x0001, 0x0004, 0x0002,
         0,       -1,       -1, G_IMAGE           ,   /* Object 2 ICFS1 */
  LASTOB, NORMAL, (LONG)&rs_bitblk[2],
  0x0002, 0x0004, 0x0004, 0x0801
};

OBJECT *rs_trindex[] =
{ &rs_object[0],   /* Tree  0 DIALOG1          */
  &rs_object[17],   /* Tree  1 DIALOG2          */
  &rs_object[46],   /* Tree  2 DIALOG3          */
  &rs_object[49],   /* Tree  3 DIALOG4          */
  &rs_object[52],   /* Tree  4 DIALOG5          */
  &rs_object[82],   /* Tree  5 DIALOG6          */
  &rs_object[102],   /* Tree  6 DIALOG7          */
  &rs_object[122],   /* Tree  7 DIALOG8          */
  &rs_object[136],   /* Tree  8 DIALOG9          */
  &rs_object[172],   /* Tree  9 DIALOG10         */
  &rs_object[178],   /* Tree 10 DIALOG11         */
  &rs_object[204],   /* Tree 11 DIALOG12         */
  &rs_object[218],   /* Tree 12 DIALOG13         */
  &rs_object[238],   /* Tree 13 DIALOG99         */
  &rs_object[240]    /* Tree 14 PICTURE0         */
};
