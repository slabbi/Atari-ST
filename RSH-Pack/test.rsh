/* GEM Resource C Source */

#include <portab.h>
#include <aes.h>
#include "TEST.H"

#if !defined(WHITEBAK)
#define WHITEBAK    0x0080
#endif
#if !defined(DRAW3D)
#define DRAW3D      0x0040
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
{ "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  "Test:____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 66,
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  "Test:____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 66,
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  "Test:____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 66,
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  "Test:____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 66,
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  "Test:____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 66,
  "9999999999",
  "Zahl:__________",
  "9999999999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 16,
  "Dies ist ein Textobjekt!",
  "",
  "",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 25, 1,
  "9999999999",
  "Zahl:__________",
  "9999999999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 16,
  "Dies ist ein Textobjekt!",
  "",
  "",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 25, 1,
  "9999999999",
  "Zahl:__________",
  "9999999999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 16,
  "Dies ist ein Textobjekt!",
  "",
  "",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 25, 1,
  "9999999999",
  "Zahl:__________",
  "9999999999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 16,
  "Dies ist ein Textobjekt!",
  "",
  "",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 25, 1
};

OBJECT rs_object[] =
{ 
  /******** Tree 0 TEST ****************************************************/
        -1,        1,       14, G_BOX     ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0003, 0x0005, 0x0045, 0x000B,
         2,       -1,       -1, G_FTEXT   ,   /* Object 1  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[0],
  0x0002, 0x0001, 0x0041, 0x0001,
         3,       -1,       -1, G_FTEXT   ,   /* Object 2  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[1],
  0x0002, 0x0002, 0x0041, 0x0001,
         4,       -1,       -1, G_FTEXT   ,   /* Object 3  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[2],
  0x0002, 0x0003, 0x0041, 0x0001,
         5,       -1,       -1, G_FTEXT   ,   /* Object 4  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[3],
  0x0002, 0x0004, 0x0041, 0x0001,
         6,       -1,       -1, G_FTEXT   ,   /* Object 5  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[4],
  0x0002, 0x0005, 0x0041, 0x0001,
         7,       -1,       -1, G_FTEXT   ,   /* Object 6  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[5],
  0x0002, 0x0006, 0x000F, 0x0001,
         8,       -1,       -1, G_TEXT    ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_tedinfo[6],
  0x0015, 0x0006, 0x0018, 0x0001,
         9,       -1,       -1, G_FTEXT   ,   /* Object 8  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[7],
  0x0002, 0x0007, 0x000F, 0x0001,
        10,       -1,       -1, G_TEXT    ,   /* Object 9  */
  NONE, NORMAL, (LONG)&rs_tedinfo[8],
  0x0015, 0x0007, 0x0018, 0x0001,
        11,       -1,       -1, G_FTEXT   ,   /* Object 10  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[9],
  0x0002, 0x0008, 0x000F, 0x0001,
        12,       -1,       -1, G_TEXT    ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[10],
  0x0015, 0x0008, 0x0018, 0x0001,
        13,       -1,       -1, G_FTEXT   ,   /* Object 12  */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[11],
  0x0002, 0x0009, 0x000F, 0x0001,
        14,       -1,       -1, G_TEXT    ,   /* Object 13  */
  NONE, NORMAL, (LONG)&rs_tedinfo[12],
  0x0015, 0x0009, 0x0018, 0x0001,
         0,       -1,       -1, G_BUTTON  ,   /* Object 14  */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Exit",
  0x0039, 0x0009, 0x0009, 0x0001,
  
};

OBJECT *rs_trindex[] =
{ &rs_object[0],   /* Tree 0 TEST */
  
};
