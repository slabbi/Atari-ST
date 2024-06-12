/* GEM Resource C Source */

#include <portab.h>
#include <aes.h>
#include "AVALON.H"

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
{ "Poll-Sessions",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 1,
  "Program-Manager",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 16, 1,
  "Point-Setup",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "*** 1 ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "*** 2 ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "*** 3 ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "*** 4 ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "*** 5 ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "*** 6 ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "A",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 2, 1,
  "Import",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Utility 1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "*.APP",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Export",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Utility 2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "*.PRG",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Cruncher",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "Utility 3",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "*.TOS",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Scanner",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "Utility 4",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "*.TTP",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Terminal",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "Utility 5",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "*.BAT",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Mailer",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Utility 6",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "*.*  ",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "111111111111",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "222222222222",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "333333333333",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "444444444444",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "555555555555",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "666666666666",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "Msg-Editor",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 1,
  "Txt-Editor",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 1,
  "F\047Request",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "Session-Setup",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 1,
  "Special",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "Find 1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Find 2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Edit areas",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 1,
  "Save",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Filemanager",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "Quit",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "AreaFix",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "F\047Hatch",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "1234567890123456789012345678901234567890123456",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, 0, 47, 1,
  "*** AVALON III ***",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1480, 0, -1, 19, 1,
  "The greatest shell for Fido-Points",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 35, 1,
  "All rights reserved.",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 21, 1,
  "(c) St.Slabihoud",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 17, 1,
  "12:34:56",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, 0, 9, 1,
  "(c) Stephan Slabihoud",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 22, 1,
  "01.02.1993",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, 0, 11, 1,
  "Session-Setup",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1181, 0, -1, 14, 1,
  "click button twice to edit session-text",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 40, 1,
  "Error",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 6, 1,
  "Import",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 7, 1,
  "Utility 1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "Export",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 7, 1,
  "Utility 2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "Cruncher",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "Utility 3",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "Scanner",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 8, 1,
  "Utility 4",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "Msg-Ed",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 7, 1,
  "Utility 5",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "Mailer",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 7, 1,
  "Utility 6",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 1,
  "Terminal",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "Other",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 6, 1,
  "Batch",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 6, 1,
  "click to edit entry",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 20, 1,
  "Wait",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 5, 1,
  "Redirect",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "Level",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 6, 1,
  "Use Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 11, 1,
  "\00312345678.123",
  "_  1. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  2. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  3. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  4. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  5. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  6. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  7. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  8. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_  9. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_ 10. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_ 11. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "\00312345678.123",
  "_ 12. ____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 19,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "123",
  "___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 4,
  "11111111111",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "22222222222",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "33333333333",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "44444444444",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "55555555555",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "66666666666",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "No Setup",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "Options:",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "1234",
  "Loops:____",
  "9999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 11,
  "Del one",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 8, 1,
  "Del all",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 8, 1,
  "Only valid when \047AutoStart\047 is selected (set free for always):",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 63, 1,
  "00002400",
  "__:__-__:__",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 12,
  "00002400",
  "__:__-__:__",
  "99999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 12,
  "Install programs",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 17, 1,
  "Redirect",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "1234567890123456789012345678901234567890123456789012345",
  "Import: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 64,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Export: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 64,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Cruncher: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 66,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Scanner: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 65,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Msg-Ed: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 64,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Txt-Ed: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 64,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Mailer: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 64,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "1234567890123456789012345678901234567890123456789012345",
  "Terminal: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 66,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Viewer used in filemanager, AreaEditor replaces internal editor!",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1100, 0, -1, 65, 1,
  "1234567890123456789012345678901234567890123456789012345",
  "Viewer: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 64,
  "Exit",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -2, 5, 1,
  "1234567890123456789012345678901234567890123456789012345",
  "_______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 56,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 56, 61,
  "1200",
  "AutoPoll: __:__ h",
  "9999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 18,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 66, 66,
  "1234567890123456789012345678901234567890",
  "Boss: ________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 41, 47,
  "123",
  "Areas defined: ___",
  "XXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 19,
  "000",
  "max.areas: ___",
  "XXX",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 4, 15,
  "Please enter full path and filename without extention!",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 55, 1,
  "123456789012345678901234567890123456789012345678",
  "________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 49, 49,
  "123",
  "Days: ___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 10,
  "1234567890123456789012345678901234567890",
  "Boss: ________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 41, 47,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678",
  "________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 49, 49,
  "123",
  "Days: ___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 10,
  "1234567890123456789012345678901234567890",
  "Boss: ________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 41, 47,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678",
  "________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 49, 49,
  "123",
  "Days: ___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 10,
  "1234567890123456789012345678901234567890",
  "Boss: ________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 41, 47,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "N/P",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "EXT",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "INS",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "DEL",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "N/P",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "EXT",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "INS",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "DEL",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "N/P",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "EXT",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "INS",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "DEL",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 4, 1,
  "Insert",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -3, 7, 1,
  "File-Request",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1181, 0, -1, 13, 1,
  "12345123451234",
  "Node: _____/_____.____",
  "99999999999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 15, 23,
  "Available:",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 11, 1,
  "Filelist:",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345678.123",
  "____________",
  "XXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 13, 13,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 11, 11,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "12345/12345.1234",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 17, 1,
  "F\047Selector",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 11, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, 1, 12, 1,
  "click twice",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "click twice",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "Available keys: Press HELP!",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 28, 1,
  "to kill request",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 16, 1,
  "to change",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "12345678.1231234567890",
  "____________/__________",
  "XXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 23, 24,
  "12345678.1231234567890",
  "____________/__________",
  "XXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 23, 24,
  "12345678.1231234567890",
  "____________/__________",
  "XXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 23, 24,
  "12345678.1231234567890",
  "____________/__________",
  "XXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 23, 24,
  "Load old request",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 17, 1,
  "Select from filelist",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 21, 1,
  "FILES",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "CLEAR",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "<123>",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "\004\004",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "\003\003",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "Press \047F\047 to find a string and \047G\047 or \047N\047 to continue search.",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 62, 1,
  "1234567890",
  "Password: __________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 11, 21,
  "Keys:",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Keys with Ctrl:",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 16, 1,
  "Keys with Alternate:",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 21, 1,
  "Keys used in Filemanager",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 25, 1,
  "Normal, Alt, Ctrl or Shift",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 27, 1,
  "KEY",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -2, 4, 1,
  "ESC to abort",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 13, 1,
  "123456789012345678901234567890",
  "Find: ______________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 31, 37,
  "Special setup",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 1,
  "Enter path without filename (e.g. \042E:\134FIDO\134\042 or \042..\134BINKLEY\134\042)",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 63, 1,
  "12345678901234567890123456789012345678901234567890",
  "AREAS.BBS : __________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 63,
  "12345678901234567890123456789012345678901234567890",
  "BINKLEY.CFG : __________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 65,
  "<- BINKLEY",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "12345678901234567890123456789012345678901234567890",
  "CONFIG.DAT : __________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 64,
  "<- SEMPER",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "Enter path+file without extension (e.g.\042E:\134FIDO\134MSGS\134NETMAIL)",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 62, 1,
  "12345678901234567890123456789012345678901234567890",
  "Netmail : __________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 61,
  "Main-Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 12,
  "1st-Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "12345678901",
  "___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 12,
  "2nd-Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "12345678901",
  "___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 12,
  " 3rd-Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 12,
  "4th-Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "12345678901",
  "___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 12,
  "5th-Setup:",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "12345678901",
  "___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 12,
  "10",
  "Fontsize: __",
  "99",
  IBM  , 0, TE_LEFT , 0x1180, 0, 0, 3, 13,
  "Specials",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "Ship     -S-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Terminal -T-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Infos",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 6, 1,
  "System   -Y-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Outbound -O-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "MsgBase  -A-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Filemanager",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 12, 1,
  "Copy     -C-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Move     -M-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Delete   -D-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Rename   -R-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Info     -I-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "View     -V-",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 13, 1,
  "Notebook",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "ECU / LLEGADA",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 1,
  "LED",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 1,
  "JETmail",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "-PASSWD password",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "-QUOTE_MSG",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-UMLAUTE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "-MAXMSGS n",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-READONLY",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "-CRUNCH high low crunch",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 24, 1,
  "-QUOTE_NON",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-PASSTHROUGH",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "-MINMSGS n",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-PASSTHROUGH",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "-SIGNATURE filename",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "-QUOTE_NATIVE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-VISIBLE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 9, 1,
  "-REMOTE nodes",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-GROUP char",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "-FLAGS flags",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "-QUOTE_NACHRICHT",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "-INVISIBLE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-NOREMOTE nodes",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "-LOCAL",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "-QUOTE_ENGLISH",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "-MAUS",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "-ORIGINALECHO name",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "-STRIPSEENBY",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "AVALON",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "-FIXEDADDRESS",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-FORCEPVT",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "-USEALIAS addr",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "-NOCPP",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "-LATIN1SMALL",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "-NONEWECHO",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-PSEUDONYM name",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "-ALLOWPVT",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "-LATIN1",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "-RANDOMFOOTLINE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "-SIGNATURE file",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "-NOSAVESEENBY",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-FIXNAME name",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-FOOTLINEFILE <file>",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 21, 1,
  "-NODISCONNECT addr",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "-FORCEPVT",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "-FIXPASSWORD passwd",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 20, 1,
  "-NORANDOMFOOTLINE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  "-NOCONNECT addr",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "-NOHISTORY",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-IGNORENEWMSGFLAG",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  "-NODELETEUNREAD",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 16, 1,
  "-GATETO area",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 13, 1,
  "IOSmail",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "-IGNOREUNREADFLAG",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 18, 1,
  "-FORCECONNECT addr",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 19, 1,
  "-NOFORCE addr",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-PASSWORD pwd",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 14, 1,
  "-DESC text",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 11, 1,
  "-NOEXPORT",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 10, 1,
  "-NAME fakename",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "-LOCAL",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 7, 1,
  "-MESSAGES number",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 17, 1,
  "-REMOTE",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 8, 1,
  "-GROUP char",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 12, 1,
  "-KEEP",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 6, 1,
  "-FILE filename",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 15, 1,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "123",
  "Zone: ___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 10,
  "12345678901234567890",
  "Domain: ____________________",
  "XXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 21, 29,
  "Zone    Domain",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 15, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "123 - 12345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 27, 1,
  "Install utilities",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 18, 1,
  "Redirect",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 9, 1,
  "Utility 1",
  "1.Name: _________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 18,
  "1234567890123456789012345678901234567890123456789012345",
  "Path: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 62,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Utility 2",
  "2.Name: _________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 18,
  "1234567890123456789012345678901234567890123456789012345",
  "Path: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 62,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Utility 3",
  "3.Name: _________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 18,
  "1234567890123456789012345678901234567890123456789012345",
  "Path: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 62,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Utility 4",
  "4.Name: _________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 18,
  "1234567890123456789012345678901234567890123456789012345",
  "Path: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 62,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Utility 5",
  "5.Name: _________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 18,
  "1234567890123456789012345678901234567890123456789012345",
  "Path: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 62,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Utility 6",
  "6.Name: _________",
  "XXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 10, 18,
  "1234567890123456789012345678901234567890123456789012345",
  "Path: _______________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 62,
  "Exit",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -2, 5, 1,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 56, 61,
  "Other Nodes and Points",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 23, 1,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 61, 61,
  "123456789",
  "_________",
  "XXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 10, 10,
  "12345678901",
  "Name: ___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 18,
  "12345123451234",
  "Node: _____/_____.____",
  "NNNNNNNNNNNNNN",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 15, 23,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "Filelist: Enter path with full filename",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 40, 1,
  "(e.g. \042D:\134FIDO\134FILELIST.TXT\042)",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 30, 1,
  "MsgBase : Enter path and filename without extension",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 52, 1,
  "(e.g. \042D:\134FIDO\134REQUEST\042)",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 25, 1,
  "Autostart",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "Press any key to exit, ESC continues.",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1100, 0, -1, 38, 1,
  "01200",
  "AutoPoll-Session #_ at __:__",
  "99999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 29,
  "Press ESC to abort or SPACE to start session",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 45, 1,
  "123456",
  "__:__:__",
  "999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 9,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 56, 61,
  "del program and cmdline",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 24, 1,
  "Send file into network",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 23, 1,
  "click twice for fileselector",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 29, 1,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "Normal",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Crash",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "Hold",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "Direct",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 7, 1,
  "Touch",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 6, 1,
  "12345123451234",
  "Node: _____/_____.____",
  "99999999999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 15, 23,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345",
  "Area: ___________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 36, 42,
  "123456789012",
  "Password: ____________",
  "xxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 23,
  "123456789012",
  "Replaces: ____________",
  "xxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 23,
  "click twice to edit path+file",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 30, 1,
  "Netmail",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 8, 1,
  "Report 1",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "Report 2",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 1,
  "1234567890123456789012345",
  "Sysop: _________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 26, 33,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "12345678901",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 1,
  "click twice to edit buttons",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 28, 1,
  "None",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 5, 1,
  "TIC",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 1,
  "Mail&File",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 10, 1,
  "12345678901",
  "Name  : ___________",
  "XXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 12, 20,
  "12345123451234",
  "Node  : _____/_____.____",
  "99999999999999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 15, 25,
  "1234567890123456789012345",
  "Sysop : _________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 26, 34,
  "12345678901234567890123456789012345",
  "Area  : ___________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 36, 44,
  "123456789012",
  "Passwd: ____________",
  "xxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 13, 21,
  "1234567890123",
  "Enter path and filename without extension: _____________",
  "XXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 14, 57,
  "12345678901234567890123456789012345678901234567890",
  "Path: __________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 51, 57,
  "Enter path+file without extension (e.g.\042E:\134FIDO\134MSGS\134NETMAIL)",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 62, 1,
  "12345678",
  "Name: ________",
  "XXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 9, 15,
  "1234567890123456789012345678901234567890123456789012345",
  "Cmd: _______________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 56, 61,
  "del program and cmdline",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 24, 1,
  "Mail to AreaFix or FileFix",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1181, 0, -1, 27, 1,
  "1234",
  "max.areas: ____",
  "XXXX",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 5, 16,
  "\004\004",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "\003\003",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "Press \047F\047 to find a string and \047G\047 or \047N\047 to continue search.",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 62, 1,
  "12345:12345/12345",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 18, 1,
  "1234567890",
  "__________",
  "XXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 11, 11,
  "1234567890123456789012345",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 26, 1,
  "1234567890123456789012345",
  "Subject: _________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, 0, 26, 35,
  "enter password (and commands)",
  "\0",
  "\0",
  SMALL, 0, TE_LEFT , 0x1180, 0, -1, 30, 1,
  "123456789012345678901234567890123456789012345678",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 49, 1,
  "12345123451234567890123456789012345",
  "Boss: _____/_____@_________________________",
  "9999999999XXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 36, 44,
  "123",
  "Zone: ___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 10,
  "123",
  "Days: ___",
  "999",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 4, 10,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "123456789012345678901234567890123456789012345678901234567890",
  "____________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_LEFT , 0x1180, 0, -1, 61, 61,
  "\004\004",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "\003\003",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "Press \047F\047 to find a string and \047G\047 or \047N\047 to continue search.",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 62, 1,
  "Mail to AreaFix or FileFix",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1181, 0, -1, 27, 1,
  "1234",
  "max.areas: ____",
  "XXXX",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 5, 16,
  "\004\004",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "\003\003",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 3, 1,
  "Press \047F\047 to find a string and \047G\047 or \047N\047 to continue search.",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 62, 1,
  "123",
  "Goto message: ___",
  "999",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 4, 18,
  "Program-Manager",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 16, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "\0",
  "\0",
  IBM  , 0, TE_CNTR , 0x1100, 0, -1, 31, 1,
  "Click twice to change entry...",
  "\0",
  "\0",
  SMALL, 0, TE_CNTR , 0x1180, 0, -1, 31, 1,
  "123456789012345678901234567890",
  "Name: ______________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 31, 37,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66,
  "12345678901234567890123456789012345678901234567890123456789012345",
  "_________________________________________________________________",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  IBM  , 0, TE_CNTR , 0x1180, 0, -1, 66, 66
};

WORD RSBB0DATA[] =
{ 0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x7FFF, 0xFFFF, 
  0x7FFF, 0xFFFF, 0x0000, 0x0000
};

WORD RSBB1DATA[] =
{ 0x4000, 0x0000, 0x4000, 0x0000, 
  0x4000, 0x0000, 0x4000, 0x0000, 
  0x4227, 0x3EF0, 0x4328, 0x8880, 
  0x42A8, 0x88E0, 0x4268, 0x8880, 
  0x4227, 0x08F0, 0x4000, 0x0000, 
  0x4000, 0x0000, 0x4000, 0x0000, 
  0x4000, 0x0000, 0x7FFF, 0xFFFF, 
  0x0000, 0x0000
};

WORD RSBB2DATA[] =
{ 0xFFFF, 0xF00F, 0x8C31, 0x83C1, 
  0x83C1, 0x8C31, 0xF00F, 0xFFFF
};

WORD RSBB3DATA[] =
{ 0xFFFF, 0xF00F, 0x8C31, 0x83C1, 
  0x83C1, 0x8C31, 0xF00F, 0xFFFF
};

WORD RSBB4DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x00A8, 0x7C00, 0x0000, 
  0x0A85, 0x421F, 0xFFFF, 0xFFFF, 
  0xE150, 0x0000, 0x003E, 0x07FF, 
  0x3FF9, 0xFFC0, 0x0000, 0x0000, 
  0x0158, 0x7C00, 0x0000, 0x150A, 
  0xC10F, 0xFFFF, 0xFFFF, 0xE1A8, 
  0x0000, 0x003E, 0x07FF, 0x3FF9, 
  0xFFC0, 0x0000, 0x0000, 0x02B8, 
  0x7C00, 0x0000, 0x2A15, 0xC287, 
  0xFFFF, 0xFFFF, 0xE1D4, 0x0000, 
  0x003E, 0x07FF, 0x3FF9, 0xFFC0, 
  0x0000, 0x0000, 0x0578, 0x7C00, 
  0x0000, 0x542B, 0xC143, 0xFFFF, 
  0xFFFF, 0xE1EA, 0x0000, 0x003E, 
  0x07FF, 0x3FF9, 0xFFC0, 0x0000, 
  0x0000, 0x0AF8, 0x7C00, 0x0000, 
  0xA857, 0xC2A1, 0xFFFF, 0xFFFF, 
  0xE1F5, 0x0000, 0x003E, 0x07FF, 
  0x3FF9, 0xFFC0, 0x0000, 0x0000, 
  0x15F8, 0x7C00, 0x0001, 0x50AF, 
  0xC340, 0xA800, 0x0003, 0xE1FA, 
  0x8000, 0x003E, 0x00F8, 0x07C0, 
  0x3E00, 0x0000, 0x0000, 0x2AF8, 
  0x7C00, 0x0002, 0xA157, 0xC3A0, 
  0x5400, 0x0003, 0xE1F5, 0x4000, 
  0x003E, 0x00F8, 0x07C0, 0x3E00, 
  0x0000, 0x0000, 0x54F8, 0x7C00, 
  0x0005, 0x42A7, 0xC3C0, 0x2A00, 
  0x0003, 0xE1F2, 0xA000, 0x003E, 
  0x00F8, 0x07C0, 0x3E00, 0x0000, 
  0x0000, 0xA8F8, 0x7C00, 0x000A, 
  0x8547, 0xC3E0, 0x1500, 0x0003, 
  0xE1F1, 0x5000, 0x003E, 0x00F8, 
  0x07C0, 0x3E00, 0x0000, 0x0001, 
  0x50F8, 0x7C00, 0x0015, 0x0A87, 
  0xC3E0, 0x0A80, 0x0003, 0xE1F0, 
  0xA800, 0x003E, 0x00F8, 0x07C0, 
  0x3E00, 0x0000, 0x0002, 0xA0F8, 
  0x7C00, 0x002A, 0x1507, 0xC3E0, 
  0x0540, 0x0003, 0xE1F0, 0x5400, 
  0x003E, 0x00F8, 0x07C0, 0x3E00, 
  0x0000, 0x0005, 0x40F8, 0x7C00, 
  0x0054, 0x2A07, 0xC3E0, 0x02A0, 
  0x0003, 0xE1F0, 0x2A00, 0x003E, 
  0x00F8, 0x07C0, 0x3E00, 0x0000, 
  0x000A, 0x80F8, 0x7C00, 0x00A8, 
  0x5407, 0xC3E0, 0x0150, 0x0003, 
  0xE1F0, 0x1500, 0x003E, 0x00F8, 
  0x07C0, 0x3E00, 0x0000, 0x0015, 
  0x00F8, 0x7C00, 0x0150, 0xA807, 
  0xC3E0, 0x00A8, 0x0003, 0xE1F0, 
  0x0A80, 0x003E, 0x0078, 0x03C0, 
  0x1E00, 0x0000, 0x002A, 0x00F8, 
  0x7C00, 0x02A1, 0x5007, 0xC3E0, 
  0x0054, 0x0003, 0xE1F0, 0x0540, 
  0x003E, 0x00B8, 0x05C0, 0x2E00, 
  0x0000, 0x0054, 0x00F8, 0x7C00, 
  0x0542, 0xA007, 0xC3E0, 0x002A, 
  0x0003, 0xE1F0, 0x02A0, 0x003E, 
  0x0058, 0x02C0, 0x1600, 0x0000, 
  0x00A8, 0x00F8, 0x7C00, 0x0A85, 
  0x4007, 0xC3E0, 0x0015, 0x0003, 
  0xE1F0, 0x0150, 0x003E, 0x00A8, 
  0x0540, 0x2A00, 0x0000, 0x0150, 
  0x00F8, 0x7C00, 0x150A, 0x8007, 
  0xC3E0, 0x000A, 0x8003, 0xE1F0, 
  0x00A8, 0x003E, 0x0050, 0x0280, 
  0x1400, 0x0000, 0x02A0, 0x00F8, 
  0x7C00, 0x2A15, 0x0007, 0xC3E0, 
  0x0005, 0x4003, 0xE1F0, 0x0054, 
  0x003E, 0x00A8, 0x0540, 0x2A00, 
  0x0000, 0x057F, 0xFFF8, 0x7C00, 
  0x542B, 0xFFFF, 0xC3E0, 0x0002, 
  0xA003, 0xE1F0, 0x002A, 0x003E, 
  0x00D0, 0x0680, 0x3400, 0x0000, 
  0x0AFF, 0xFFF8, 0x7C00, 0xA857, 
  0xFFFF, 0xC3E0, 0x0001, 0x5003, 
  0xE1F0, 0x0015, 0x003E, 0x00E8, 
  0x0740, 0x3A00, 0x0000, 0x15FF, 
  0xFFF8, 0x7C01, 0x50AF, 0xFFFF, 
  0xC3E0, 0x0000, 0xA803, 0xE1F0, 
  0x000A, 0x803E, 0x00F0, 0x0780, 
  0x3C00, 0x0000, 0x2BFF, 0xFFF8, 
  0x7C02, 0xA15F, 0xFFFF, 0xC3E0, 
  0x0000, 0x5403, 0xE1F0, 0x0005, 
  0x403E, 0x00F8, 0x07C0, 0x3E00, 
  0x0000, 0x57FF, 0xFFF8, 0x7C05, 
  0x42BF, 0xFFFF, 0xC3E0, 0x0000, 
  0x2A03, 0xE1F0, 0x0002, 0xA03E, 
  0x00F8, 0x07C0, 0x3E00, 0x0000, 
  0xA800, 0x00F8, 0x7C0A, 0x8540, 
  0x0007, 0xC3E0, 0x0000, 0x1503, 
  0xE1F0, 0x0001, 0x503E, 0x00F8, 
  0x07C0, 0x3E00, 0x0001, 0x5000, 
  0x00F8, 0x7C15, 0x0A80, 0x0007, 
  0xC3E0, 0x0000, 0x0A83, 0xE1F0, 
  0x0000, 0xA83E, 0x00F8, 0x07C0, 
  0x3E00, 0x0002, 0xA000, 0x00F8, 
  0x7C2A, 0x1500, 0x0007, 0xC3E0, 
  0x0000, 0x0543, 0xE1F0, 0x0000, 
  0x543E, 0x00F8, 0x07C0, 0x3E00, 
  0x0005, 0x4000, 0x00F8, 0x7C54, 
  0x2A00, 0x0007, 0xC3E0, 0x0000, 
  0x02A3, 0xE1F0, 0x0000, 0x2A3E, 
  0x00F8, 0x07C0, 0x3E00, 0x000A, 
  0x8000, 0x00F8, 0x7CA8, 0x5400, 
  0x0007, 0xC3E0, 0x0000, 0x0153, 
  0xE1F0, 0x0000, 0x153E, 0x00F8, 
  0x07C0, 0x3E00, 0x0015, 0x0000, 
  0x00F8, 0x7D50, 0xA800, 0x0007, 
  0xC3E0, 0x0000, 0x00AB, 0xE1F0, 
  0x0000, 0x0ABE, 0x00F8, 0x07C0, 
  0x3E00, 0x002A, 0x0000, 0x00F8, 
  0x7EA1, 0x5000, 0x0007, 0xC3E0, 
  0x0000, 0x0057, 0xE1F0, 0x0000, 
  0x057E, 0x00F8, 0x07C0, 0x3E00, 
  0x0054, 0x0000, 0x00F8, 0x7D42, 
  0xA000, 0x0007, 0xC3FF, 0xFFFF, 
  0xD42B, 0xE1F0, 0x0000, 0x02BE, 
  0x07FE, 0x3FF9, 0xFFC0, 0x00A8, 
  0x0000, 0x00F8, 0x7A85, 0x4000, 
  0x0007, 0xC3FF, 0xFFFF, 0xEA15, 
  0xE1F0, 0x0000, 0x017E, 0x07FE, 
  0x3FF9, 0xFFC0, 0x0150, 0x0000, 
  0x00F8, 0x750A, 0x8000, 0x0007, 
  0xC3FF, 0xFFFF, 0xF50A, 0xE1F0, 
  0x0000, 0x00BE, 0x07FE, 0x3FF9, 
  0xFFC0, 0x02A0, 0x0000, 0x00F8, 
  0x6A15, 0x0000, 0x0007, 0xC3FF, 
  0xFFFF, 0xFA85, 0x61F0, 0x0000, 
  0x007E, 0x07FE, 0x3FF9, 0xFFC0, 
  0x0540, 0x0000, 0x00F8, 0x542A, 
  0x0000, 0x0007, 0xC3FF, 0xFFFF, 
  0xFD42, 0xA1F0, 0x0000, 0x003E, 
  0x07FE, 0x3FF9, 0xFFC0, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

WORD RSBB5DATA[] =
{ 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0030, 0xF800, 0x0000, 0x330C, 
  0x301F, 0xFFFF, 0xFFFF, 0xFE18, 
  0x0000, 0x0003, 0xE07F, 0xE7FE, 
  0x7FE0, 0x0000, 0x0000, 0x00C0, 
  0xF800, 0x0000, 0xCC30, 0x0C07, 
  0xFFFF, 0xFFFF, 0xFE06, 0x0000, 
  0x0003, 0xE07F, 0xE7FE, 0x7FE0, 
  0x0000, 0x0000, 0x0330, 0xF800, 
  0x0003, 0x30CC, 0x3201, 0xFFFF, 
  0xFFFF, 0xFE19, 0x8000, 0x0003, 
  0xE00F, 0x00F0, 0x0F00, 0x0000, 
  0x0000, 0x0CF0, 0xF800, 0x000C, 
  0xC33C, 0x3C00, 0x6600, 0x0000, 
  0x3E1E, 0x6000, 0x0003, 0xE00F, 
  0x00F0, 0x0F00, 0x0000, 0x0000, 
  0x33F0, 0xF800, 0x0033, 0x0CFC, 
  0x3E00, 0x1980, 0x0000, 0x3E1F, 
  0x9800, 0x0003, 0xE00F, 0x00F0, 
  0x0F00, 0x0000, 0x0000, 0xCDF0, 
  0xF800, 0x00CC, 0x337C, 0x3E00, 
  0x0660, 0x0000, 0x3E1F, 0x6600, 
  0x0003, 0xE00F, 0x00F0, 0x0F00, 
  0x0000, 0x0003, 0x31F0, 0xF800, 
  0x0330, 0xCC7C, 0x3E00, 0x0198, 
  0x0000, 0x3E1F, 0x1980, 0x0003, 
  0xE00F, 0x00F0, 0x0F00, 0x0000, 
  0x000C, 0xC1F0, 0xF800, 0x0CC3, 
  0x307C, 0x3E00, 0x0066, 0x0000, 
  0x3E1F, 0x0660, 0x0003, 0xE007, 
  0x0070, 0x0700, 0x0000, 0x0033, 
  0x01F0, 0xF800, 0x330C, 0xC07C, 
  0x3E00, 0x0019, 0x8000, 0x3E1F, 
  0x0198, 0x0003, 0xE009, 0x0090, 
  0x0900, 0x0000, 0x00CC, 0x01F0, 
  0xF800, 0xCC33, 0x007C, 0x3E00, 
  0x0006, 0x6000, 0x3E1F, 0x0066, 
  0x0003, 0xE006, 0x0060, 0x0600, 
  0x0000, 0x0330, 0x01F0, 0xF803, 
  0x30CC, 0x007C, 0x3E00, 0x0001, 
  0x9800, 0x3E1F, 0x0019, 0x8003, 
  0xE009, 0x0090, 0x0900, 0x0000, 
  0x0CFF, 0xFFF0, 0xF80C, 0xC33F, 
  0xFFFC, 0x3E00, 0x0000, 0x6600, 
  0x3E1F, 0x0006, 0x6003, 0xE00E, 
  0x00E0, 0x0E00, 0x0000, 0x33FF, 
  0xFFF0, 0xF833, 0x0CFF, 0xFFFC, 
  0x3E00, 0x0000, 0x1980, 0x3E1F, 
  0x0001, 0x9803, 0xE00F, 0x00F0, 
  0x0F00, 0x0000, 0xCFFF, 0xFFF0, 
  0xF8CC, 0x33FF, 0xFFFC, 0x3E00, 
  0x0000, 0x0660, 0x3E1F, 0x0000, 
  0x6603, 0xE00F, 0x00F0, 0x0F00, 
  0x0003, 0x3000, 0x01F0, 0xFB30, 
  0xCC00, 0x007C, 0x3E00, 0x0000, 
  0x0198, 0x3E1F, 0x0000, 0x1983, 
  0xE00F, 0x00F0, 0x0F00, 0x000C, 
  0xC000, 0x01F0, 0xFCC3, 0x3000, 
  0x007C, 0x3E00, 0x0000, 0x0066, 
  0x3E1F, 0x0000, 0x0663, 0xE00F, 
  0x00F0, 0x0F00, 0x0033, 0x0000, 
  0x01F0, 0xF30C, 0xC000, 0x007C, 
  0x3E00, 0x0000, 0x0019, 0xBE1F, 
  0x0000, 0x019B, 0xE00F, 0x00F0, 
  0x0F00, 0x00CC, 0x0000, 0x01F0, 
  0xCC33, 0x0000, 0x007C, 0x3FFF, 
  0xFFFF, 0xF986, 0x7E1F, 0x0000, 
  0x0067, 0xE00F, 0x00F0, 0x0F00, 
  0x0330, 0x0000, 0x01F0, 0x30CC, 
  0x0000, 0x007C, 0x3FFF, 0xFFFF, 
  0xFE61, 0x9E1F, 0x0000, 0x001B, 
  0xE07F, 0xE7FE, 0x7FE0, 0x0CC0, 
  0x0000, 0x01F0, 0xC330, 0x0000, 
  0x007C, 0x3FFF, 0xFFFF, 0xFF98, 
  0x661F, 0x0000, 0x0007, 0xE07F, 
  0xE7FE, 0x7FE0, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000
};

WORD RSBB6DATA[] =
{ 0x1F7B, 0xF1E0, 0x1F7B, 0xFBF0, 
  0x1831, 0x9B30, 0x1831, 0x9B30, 
  0x1E31, 0x9B30, 0x1E31, 0x9B30, 
  0x1831, 0x9B30, 0x187B, 0xFBF0, 
  0x187B, 0xF1E0, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x050A, 0xBF42, 
  0x050A, 0xBF42, 0x0D16, 0x9162, 
  0x0D16, 0x9162, 0x152A, 0x8952, 
  0x152A, 0x8952, 0x3D5E, 0x854A, 
  0x3D5E, 0x854A, 0x45A2, 0x8346, 
  0x45A2, 0x8346, 0x8542, 0xFD42, 
  0x8542, 0xFD42, 0x0000, 0x0000
};

WORD RSBB7DATA[] =
{ 0x03DD, 0xE300, 0x0208, 0x9480, 
  0x0388, 0x9480, 0x0208, 0x9480, 
  0x021D, 0xE300, 0x0000, 0x0000, 
  0x050A, 0xBF42, 0x0D16, 0x9162, 
  0x152A, 0x8952, 0x3D5E, 0x854A, 
  0x45A2, 0x8346, 0x8542, 0xFD42
};

WORD RSBB8DATA[] =
{ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

BITBLK rs_bitblk[] =
{ RSBB0DATA,   4,  16,   0,   0, 0x0000,
  RSBB1DATA,   4,  15,   0,   0, 0x0001,
  RSBB2DATA,   2,   8,   0,   0, 0x0001,
  RSBB3DATA,   2,   8,   0,   0, 0x0001,
  RSBB4DATA,  30,  48,   0,   0, 0x0001,
  RSBB5DATA,  30,  24,   0,   0, 0x0001,
  RSBB6DATA,   4,  24,   0,   0, 0x0001,
  RSBB7DATA,   4,  12,   0,   0, 0x0001,
  RSBB8DATA,   2,   4,   0,   0, 0x0001
};

OBJECT rs_object[] =
{ 
  /******** Tree 0 DIALOG1 ****************************************************/
        -1, NOTEBOOK        , DESKINFO        , G_BOX             ,   /* Object 0 DESKFIRS */
  NONE, NORMAL, (LONG)0x00FF1123L,
  0x0000, 0x0000, 0x004F, 0x0816,
  STARTSEL        , DONOTE          , DONOTE          , G_IMAGE           ,   /* Object 1 NOTEBOOK */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_bitblk[0],
  0x004B, 0x0000, 0x0004, 0x0001,
  NOTEBOOK        ,       -1,       -1, G_IMAGE           ,   /* Object 2 DONOTE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_bitblk[1],
  0x0000, 0x0000, 0x0004, 0x0F00,
  DOPROGS         ,       -1,       -1, G_BOXTEXT         ,   /* Object 3 STARTSEL */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[0],
  0x0003, 0x0001, 0x0011, 0x0001,
  TBOX2           ,       -1,       -1, G_BOXTEXT         ,   /* Object 4 DOPROGS */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[1],
  0x0018, 0x0001, 0x0022, 0x0001,
  DESKSESS        ,       -1,       -1, G_BOXTEXT         ,   /* Object 5 TBOX2 */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[2],
  0x003E, 0x0001, 0x000E, 0x0001,
  DESKPRGS        , START1          , AUTO6           , G_IBOX            ,   /* Object 6 DESKSESS */
  NONE, NORMAL, (LONG)0x00001121L,
  0x0003, 0x0003, 0x0011, 0x0006,
  AUTO1           ,       -1,       -1, G_BOXTEXT         ,   /* Object 7 START1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[3],
  0x0000, 0x0000, 0x000B, 0x0001,
  START2          ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 AUTO1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[4],
  0x000D, 0x0000, 0x0004, 0x0001,
  AUTO2           ,       -1,       -1, G_BOXTEXT         ,   /* Object 9 START2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[5],
  0x0000, 0x0001, 0x000B, 0x0001,
  START3          ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 AUTO2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[6],
  0x000D, 0x0001, 0x0004, 0x0001,
  AUTO3           ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 START3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[7],
  0x0000, 0x0002, 0x000B, 0x0001,
  START4          ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 AUTO3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[8],
  0x000D, 0x0002, 0x0004, 0x0001,
  AUTO4           ,       -1,       -1, G_BOXTEXT         ,   /* Object 13 START4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[9],
  0x0000, 0x0003, 0x000B, 0x0001,
  START5          ,       -1,       -1, G_BOXTEXT         ,   /* Object 14 AUTO4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[10],
  0x000D, 0x0003, 0x0004, 0x0001,
  AUTO5           ,       -1,       -1, G_BOXTEXT         ,   /* Object 15 START5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[11],
  0x0000, 0x0004, 0x000B, 0x0001,
  START6          ,       -1,       -1, G_BOXTEXT         ,   /* Object 16 AUTO5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[12],
  0x000D, 0x0004, 0x0004, 0x0001,
  AUTO6           ,       -1,       -1, G_BOXTEXT         ,   /* Object 17 START6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[13],
  0x0000, 0x0005, 0x000B, 0x0001,
  DESKSESS        ,       -1,       -1, G_BOXTEXT         ,   /* Object 18 AUTO6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[14],
  0x000D, 0x0005, 0x0004, 0x0001,
  DESKPNT         , DOIMPORT        , DOSTAR          , G_IBOX            ,   /* Object 19 DESKPRGS */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0018, 0x0003, 0x0022, 0x0006,
  DOUTIL1         ,       -1,       -1, G_BOXTEXT         ,   /* Object 20 DOIMPORT */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[15],
  0x0000, 0x0000, 0x000A, 0x0001,
  DOAPP           ,       -1,       -1, G_BOXTEXT         ,   /* Object 21 DOUTIL1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[16],
  0x000C, 0x0000, 0x000A, 0x0001,
  DOEXPORT        ,       -1,       -1, G_BOXTEXT         ,   /* Object 22 DOAPP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[17],
  0x0018, 0x0000, 0x000A, 0x0001,
  DOUTIL2         ,       -1,       -1, G_BOXTEXT         ,   /* Object 23 DOEXPORT */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[18],
  0x0000, 0x0001, 0x000A, 0x0001,
  DOPRG           ,       -1,       -1, G_BOXTEXT         ,   /* Object 24 DOUTIL2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[19],
  0x000C, 0x0001, 0x000A, 0x0001,
  DOCRUNCH        ,       -1,       -1, G_BOXTEXT         ,   /* Object 25 DOPRG */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[20],
  0x0018, 0x0001, 0x000A, 0x0001,
  DOUTIL3         ,       -1,       -1, G_BOXTEXT         ,   /* Object 26 DOCRUNCH */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[21],
  0x0000, 0x0002, 0x000A, 0x0001,
  DOTOS           ,       -1,       -1, G_BOXTEXT         ,   /* Object 27 DOUTIL3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[22],
  0x000C, 0x0002, 0x000A, 0x0001,
  DOSCAN          ,       -1,       -1, G_BOXTEXT         ,   /* Object 28 DOTOS */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[23],
  0x0018, 0x0002, 0x000A, 0x0001,
  DOUTIL4         ,       -1,       -1, G_BOXTEXT         ,   /* Object 29 DOSCAN */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[24],
  0x0000, 0x0003, 0x000A, 0x0001,
  DOTTP           ,       -1,       -1, G_BOXTEXT         ,   /* Object 30 DOUTIL4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[25],
  0x000C, 0x0003, 0x000A, 0x0001,
  DOTERM          ,       -1,       -1, G_BOXTEXT         ,   /* Object 31 DOTTP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[26],
  0x0018, 0x0003, 0x000A, 0x0001,
  DOUTIL5         ,       -1,       -1, G_BOXTEXT         ,   /* Object 32 DOTERM */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[27],
  0x0000, 0x0004, 0x000A, 0x0001,
  DOALL           ,       -1,       -1, G_BOXTEXT         ,   /* Object 33 DOUTIL5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[28],
  0x000C, 0x0004, 0x000A, 0x0001,
  DOMAILER        ,       -1,       -1, G_BOXTEXT         ,   /* Object 34 DOALL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[29],
  0x0018, 0x0004, 0x000A, 0x0001,
  DOUTIL6         ,       -1,       -1, G_BOXTEXT         ,   /* Object 35 DOMAILER */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[30],
  0x0000, 0x0005, 0x000A, 0x0001,
  DOSTAR          ,       -1,       -1, G_BOXTEXT         ,   /* Object 36 DOUTIL6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[31],
  0x000C, 0x0005, 0x000A, 0x0001,
  DESKPRGS        ,       -1,       -1, G_BOXTEXT         ,   /* Object 37 DOSTAR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[32],
  0x0018, 0x0005, 0x000A, 0x0001,
  DOMSGED         , POINT0          , POINT5          , G_IBOX            ,   /* Object 38 DESKPNT */
  NONE, NORMAL, (LONG)0x00001100L,
  0x003E, 0x0003, 0x000F, 0x0006,
  POINT1          ,       -1,       -1, G_BOXTEXT         ,   /* Object 39 POINT0 */
  SELECTABLE|EXIT, CHECKED, (LONG)&rs_tedinfo[33],
  0x0000, 0x0000, 0x000E, 0x0001,
  POINT2          ,       -1,       -1, G_BOXTEXT         ,   /* Object 40 POINT1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[34],
  0x0000, 0x0001, 0x000E, 0x0001,
  POINT3          ,       -1,       -1, G_BOXTEXT         ,   /* Object 41 POINT2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[35],
  0x0000, 0x0002, 0x000E, 0x0001,
  POINT4          ,       -1,       -1, G_BOXTEXT         ,   /* Object 42 POINT3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[36],
  0x0000, 0x0003, 0x000E, 0x0001,
  POINT5          ,       -1,       -1, G_BOXTEXT         ,   /* Object 43 POINT4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[37],
  0x0000, 0x0004, 0x000E, 0x0001,
  DESKPNT         ,       -1,       -1, G_BOXTEXT         ,   /* Object 44 POINT5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[38],
  0x0000, 0x0005, 0x000E, 0x0001,
  DOTXTED         ,       -1,       -1, G_BOXTEXT         ,   /* Object 45 DOMSGED */
  SELECTABLE|EXIT, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[39],
  0x0003, 0x000A, 0x000B, 0x0003,
  DOFREQ          ,       -1,       -1, G_BOXTEXT         ,   /* Object 46 DOTXTED */
  SELECTABLE|EXIT, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[40],
  0x000F, 0x000A, 0x000B, 0x0801,
        48,       -1,       -1, G_BOXTEXT         ,   /* Object 47 DOFREQ */
  SELECTABLE|EXIT, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[41],
  0x001B, 0x000A, 0x000B, 0x0801,
  DOQUIT          , DOSETUP1        , DMANAGER        , G_BOX             ,   /* Object 48  */
  NONE, OUTLINED|SHADOWED, (LONG)0x00FF1101L,
  0x0029, 0x000A, 0x001C, 0x0003,
  DOSPEZIA        ,       -1,       -1, G_BOXTEXT         ,   /* Object 49 DOSETUP1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[42],
  0x0000, 0x0000, 0x000E, 0x0001,
  DOSETUP2        ,       -1,       -1, G_BOXTEXT         ,   /* Object 50 DOSPEZIA */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[43],
  0x000E, 0x0000, 0x000E, 0x0001,
  DOSETUP3        ,       -1,       -1, G_BOXTEXT         ,   /* Object 51 DOSETUP2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[44],
  0x0000, 0x0001, 0x0007, 0x0001,
  DOAREAS         ,       -1,       -1, G_BOXTEXT         ,   /* Object 52 DOSETUP3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[45],
  0x0007, 0x0001, 0x0007, 0x0001,
  DSAVE           ,       -1,       -1, G_BOXTEXT         ,   /* Object 53 DOAREAS */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[46],
  0x000E, 0x0001, 0x000E, 0x0001,
  DMANAGER        ,       -1,       -1, G_BOXTEXT         ,   /* Object 54 DSAVE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[47],
  0x0000, 0x0002, 0x000E, 0x0001,
        48,       -1,       -1, G_BOXTEXT         ,   /* Object 55 DMANAGER */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[48],
  0x000E, 0x0002, 0x000E, 0x0001,
  DOFIX           ,       -1,       -1, G_BOXTEXT         ,   /* Object 56 DOQUIT */
  SELECTABLE|EXIT, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[49],
  0x0046, 0x000A, 0x0006, 0x0003,
  DOHATCH         ,       -1,       -1, G_BOXTEXT         ,   /* Object 57 DOFIX */
  SELECTABLE|EXIT, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[50],
  0x000F, 0x000C, 0x000B, 0x0001,
  TBOX            ,       -1,       -1, G_BOXTEXT         ,   /* Object 58 DOHATCH */
  SELECTABLE|EXIT, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[51],
  0x001B, 0x000C, 0x000B, 0x0001,
  TLOGO           , BT3001          , BT300C          , G_BOX             ,   /* Object 59 TBOX */
  SELECTABLE|EXIT, SHADOWED, (LONG)0x00FF1101L,
  0x0003, 0x000E, 0x0049, 0x0006,
  BT3002          ,       -1,       -1, G_TEXT            ,   /* Object 60 BT3001 */
  NONE, NORMAL, (LONG)&rs_tedinfo[52],
  0x0001, 0x0000, 0x0422, 0x0001,
  BT3003          ,       -1,       -1, G_TEXT            ,   /* Object 61 BT3002 */
  NONE, NORMAL, (LONG)&rs_tedinfo[53],
  0x0001, 0x0001, 0x0422, 0x0001,
  BT3004          ,       -1,       -1, G_TEXT            ,   /* Object 62 BT3003 */
  NONE, NORMAL, (LONG)&rs_tedinfo[54],
  0x0001, 0x0002, 0x0422, 0x0001,
  BT3005          ,       -1,       -1, G_TEXT            ,   /* Object 63 BT3004 */
  NONE, NORMAL, (LONG)&rs_tedinfo[55],
  0x0001, 0x0003, 0x0422, 0x0001,
  BT3006          ,       -1,       -1, G_TEXT            ,   /* Object 64 BT3005 */
  NONE, NORMAL, (LONG)&rs_tedinfo[56],
  0x0001, 0x0004, 0x0422, 0x0001,
  BT3007          ,       -1,       -1, G_TEXT            ,   /* Object 65 BT3006 */
  NONE, NORMAL, (LONG)&rs_tedinfo[57],
  0x0001, 0x0005, 0x0422, 0x0001,
  BT3008          ,       -1,       -1, G_TEXT            ,   /* Object 66 BT3007 */
  NONE, NORMAL, (LONG)&rs_tedinfo[58],
  0x0025, 0x0000, 0x0422, 0x0001,
  BT3009          ,       -1,       -1, G_TEXT            ,   /* Object 67 BT3008 */
  NONE, NORMAL, (LONG)&rs_tedinfo[59],
  0x0025, 0x0001, 0x0422, 0x0001,
  BT300A          ,       -1,       -1, G_TEXT            ,   /* Object 68 BT3009 */
  NONE, NORMAL, (LONG)&rs_tedinfo[60],
  0x0025, 0x0002, 0x0023, 0x0001,
  BT300B          ,       -1,       -1, G_TEXT            ,   /* Object 69 BT300A */
  NONE, NORMAL, (LONG)&rs_tedinfo[61],
  0x0025, 0x0003, 0x0023, 0x0001,
  BT300C          ,       -1,       -1, G_TEXT            ,   /* Object 70 BT300B */
  NONE, NORMAL, (LONG)&rs_tedinfo[62],
  0x0025, 0x0004, 0x0023, 0x0001,
  TBOX            ,       -1,       -1, G_TEXT            ,   /* Object 71 BT300C */
  NONE, NORMAL, (LONG)&rs_tedinfo[63],
  0x0025, 0x0005, 0x0422, 0x0001,
  DESKINFO        ,       73,       75, G_BOX             ,   /* Object 72 TLOGO */
  NONE, SHADOWED, (LONG)0x00FF11C2L,
  0x0003, 0x000E, 0x0049, 0x0006,
        75, AVALLOGO        , AVALLOGO        , G_BOX             ,   /* Object 73  */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x0002, 0x0001, 0x0020, 0x0004,
        73,       -1,       -1, G_IMAGE           ,   /* Object 74 AVALLOGO */
  NONE, NORMAL, (LONG)&rs_bitblk[2],
  0x0001, 0x0800, 0x0002, 0x0800,
  TLOGO           ,       76,       79, G_BOX             ,   /* Object 75  */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x0024, 0x0001, 0x0023, 0x0004,
        77,       -1,       -1, G_TEXT            ,   /* Object 76  */
  NONE, NORMAL, (LONG)&rs_tedinfo[64],
  0x0000, 0x0000, 0x0023, 0x0001,
        78,       -1,       -1, G_TEXT            ,   /* Object 77  */
  NONE, NORMAL, (LONG)&rs_tedinfo[65],
  0x0000, 0x0001, 0x0023, 0x0001,
        79,       -1,       -1, G_TEXT            ,   /* Object 78  */
  NONE, NORMAL, (LONG)&rs_tedinfo[66],
  0x0000, 0x0003, 0x0023, 0x0001,
        75,       -1,       -1, G_TEXT            ,   /* Object 79  */
  NONE, NORMAL, (LONG)&rs_tedinfo[67],
  0x0000, 0x0002, 0x0023, 0x0001,
  DESKFIRS        , DOTIME          , DODATE          , G_BOX             ,   /* Object 80 DESKINFO */
  NONE, SHADOWED, (LONG)0x00FF1181L,
  0x0003, 0x0814, 0x0049, 0x0001,
  DOINFO          ,       -1,       -1, G_BOXTEXT         ,   /* Object 81 DOTIME */
  NONE, NORMAL, (LONG)&rs_tedinfo[68],
  0x0001, 0x0000, 0x0008, 0x0001,
  DODATE          ,       -1,       -1, G_BOXTEXT         ,   /* Object 82 DOINFO */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[69],
  0x000A, 0x0000, 0x0035, 0x0001,
  DESKINFO        ,       -1,       -1, G_BOXTEXT         ,   /* Object 83 DODATE */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[70],
  0x0040, 0x0000, 0x0008, 0x0001,
  
  /******** Tree 1 DIALOG2 ****************************************************/
        -1,        1, XOK             , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0816,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[71],
  0x0000, 0x0000, 0x004F, 0x0001,
         9, SESSION1        , SESSION6        , G_IBOX            ,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00001101L,
  0x0004, 0x0002, 0x0049, 0x0001,
  SESSION2        ,       -1,       -1, G_BUTTON          ,   /* Object 3 SESSION1 */
  SELECTABLE|RBUTTON|TOUCHEXIT, SELECTED, (LONG)"Session 1",
  0x0000, 0x0000, 0x000B, 0x0001,
  SESSION3        ,       -1,       -1, G_BUTTON          ,   /* Object 4 SESSION2 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)"Session 2",
  0x000C, 0x0000, 0x000B, 0x0001,
  SESSION4        ,       -1,       -1, G_BUTTON          ,   /* Object 5 SESSION3 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)"Session 3",
  0x0018, 0x0000, 0x000B, 0x0001,
  SESSION5        ,       -1,       -1, G_BUTTON          ,   /* Object 6 SESSION4 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)"Session 4",
  0x0024, 0x0000, 0x000B, 0x0001,
  SESSION6        ,       -1,       -1, G_BUTTON          ,   /* Object 7 SESSION5 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)"Session 5",
  0x0030, 0x0000, 0x000B, 0x0001,
         2,       -1,       -1, G_BUTTON          ,   /* Object 8 SESSION6 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)"Session 6",
  0x003C, 0x0000, 0x000B, 0x0001,
        10,       -1,       -1, G_TEXT            ,   /* Object 9  */
  NONE, NORMAL, (LONG)&rs_tedinfo[72],
  0x0004, 0x0003, 0x021D, 0x0001,
        11,       -1,       -1, G_TEXT            ,   /* Object 10  */
  NONE, NORMAL, (LONG)&rs_tedinfo[73],
  0x0039, 0x0803, 0x0005, 0x0001,
        27, XIMPORT         , XBATCH          , G_IBOX            ,   /* Object 11  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0001, 0x0004, 0x0016, 0x000A,
  XUTIL1          ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 XIMPORT */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[74],
  0x0001, 0x0001, 0x000A, 0x0001,
  XEXPORT         ,       -1,       -1, G_BOXTEXT         ,   /* Object 13 XUTIL1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[75],
  0x000C, 0x0001, 0x000A, 0x0001,
  XUTIL2          ,       -1,       -1, G_BOXTEXT         ,   /* Object 14 XEXPORT */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[76],
  0x0001, 0x0002, 0x000A, 0x0001,
  XCRUNCH         ,       -1,       -1, G_BOXTEXT         ,   /* Object 15 XUTIL2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[77],
  0x000C, 0x0002, 0x000A, 0x0001,
  XUTIL3          ,       -1,       -1, G_BOXTEXT         ,   /* Object 16 XCRUNCH */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[78],
  0x0001, 0x0003, 0x000A, 0x0001,
  XSCANNER        ,       -1,       -1, G_BOXTEXT         ,   /* Object 17 XUTIL3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[79],
  0x000C, 0x0003, 0x000A, 0x0001,
  XUTIL4          ,       -1,       -1, G_BOXTEXT         ,   /* Object 18 XSCANNER */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[80],
  0x0001, 0x0004, 0x000A, 0x0001,
  XMSGED          ,       -1,       -1, G_BOXTEXT         ,   /* Object 19 XUTIL4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[81],
  0x000C, 0x0004, 0x000A, 0x0001,
  XUTIL5          ,       -1,       -1, G_BOXTEXT         ,   /* Object 20 XMSGED */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[82],
  0x0001, 0x0005, 0x000A, 0x0001,
  XMAILER         ,       -1,       -1, G_BOXTEXT         ,   /* Object 21 XUTIL5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[83],
  0x000C, 0x0005, 0x000A, 0x0001,
  XUTIL6          ,       -1,       -1, G_BOXTEXT         ,   /* Object 22 XMAILER */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[84],
  0x0001, 0x0006, 0x000A, 0x0001,
  XTERM           ,       -1,       -1, G_BOXTEXT         ,   /* Object 23 XUTIL6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[85],
  0x000C, 0x0006, 0x000A, 0x0001,
  XOTHER          ,       -1,       -1, G_BOXTEXT         ,   /* Object 24 XTERM */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[86],
  0x0001, 0x0007, 0x000A, 0x0001,
  XBATCH          ,       -1,       -1, G_BOXTEXT         ,   /* Object 25 XOTHER */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[87],
  0x0001, 0x0008, 0x000A, 0x0001,
        11,       -1,       -1, G_BOXTEXT         ,   /* Object 26 XBATCH */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[88],
  0x000C, 0x0008, 0x000A, 0x0001,
        28,       -1,       -1, G_TEXT            ,   /* Object 27  */
  NONE, NORMAL, (LONG)&rs_tedinfo[89],
  0x001B, 0x0004, 0x0013, 0x0001,
        29,       -1,       -1, G_TEXT            ,   /* Object 28  */
  NONE, NORMAL, (LONG)&rs_tedinfo[90],
  0x002F, 0x0004, 0x0004, 0x0001,
        30,       -1,       -1, G_TEXT            ,   /* Object 29  */
  NONE, NORMAL, (LONG)&rs_tedinfo[91],
  0x0033, 0x0004, 0x0006, 0x0001,
        31,       -1,       -1, G_TEXT            ,   /* Object 30  */
  NONE, NORMAL, (LONG)&rs_tedinfo[92],
  0x0039, 0x0004, 0x0005, 0x0001,
  XBOX            ,       -1,       -1, G_TEXT            ,   /* Object 31  */
  NONE, NORMAL, (LONG)&rs_tedinfo[93],
  0x0041, 0x0004, 0x000C, 0x0001,
  XBOX3           , XPROG1          , XPROG12         , G_BOX             ,   /* Object 32 XBOX */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x001B, 0x0005, 0x0013, 0x000C,
  XPROG2          ,       -1,       -1, G_FTEXT           ,   /* Object 33 XPROG1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[94],
  0x0000, 0x0000, 0x0012, 0x0001,
  XPROG3          ,       -1,       -1, G_FTEXT           ,   /* Object 34 XPROG2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[95],
  0x0000, 0x0001, 0x0012, 0x0001,
  XPROG4          ,       -1,       -1, G_FTEXT           ,   /* Object 35 XPROG3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[96],
  0x0000, 0x0002, 0x0012, 0x0001,
  XPROG5          ,       -1,       -1, G_FTEXT           ,   /* Object 36 XPROG4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[97],
  0x0000, 0x0003, 0x0012, 0x0001,
  XPROG6          ,       -1,       -1, G_FTEXT           ,   /* Object 37 XPROG5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[98],
  0x0000, 0x0004, 0x0012, 0x0001,
  XPROG7          ,       -1,       -1, G_FTEXT           ,   /* Object 38 XPROG6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[99],
  0x0000, 0x0005, 0x0012, 0x0001,
  XPROG8          ,       -1,       -1, G_FTEXT           ,   /* Object 39 XPROG7 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[100],
  0x0000, 0x0006, 0x0012, 0x0001,
  XPROG9          ,       -1,       -1, G_FTEXT           ,   /* Object 40 XPROG8 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[101],
  0x0000, 0x0007, 0x0012, 0x0001,
  XPROG10         ,       -1,       -1, G_FTEXT           ,   /* Object 41 XPROG9 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[102],
  0x0000, 0x0008, 0x0012, 0x0001,
  XPROG11         ,       -1,       -1, G_FTEXT           ,   /* Object 42 XPROG10 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[103],
  0x0000, 0x0009, 0x0012, 0x0001,
  XPROG12         ,       -1,       -1, G_FTEXT           ,   /* Object 43 XPROG11 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[104],
  0x0000, 0x000A, 0x0012, 0x0001,
  XBOX            ,       -1,       -1, G_FTEXT           ,   /* Object 44 XPROG12 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[105],
  0x0000, 0x000B, 0x0012, 0x0001,
  XBOX1           , XWAIT1          , XERR12          , G_IBOX            ,   /* Object 45 XBOX3 */
  NONE, NORMAL, (LONG)0x00001101L,
  0x0030, 0x0005, 0x000E, 0x000C,
  XWAIT2          ,       -1,       -1, G_BOX             |0x0300,   /* Object 46 XWAIT1 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0000, 0x0002, 0x0001,
  XWAIT3          ,       -1,       -1, G_BOX             |0x0300,   /* Object 47 XWAIT2 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0001, 0x0002, 0x0001,
  XWAIT4          ,       -1,       -1, G_BOX             |0x0300,   /* Object 48 XWAIT3 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0002, 0x0002, 0x0001,
  XWAIT5          ,       -1,       -1, G_BOX             |0x0300,   /* Object 49 XWAIT4 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0003, 0x0002, 0x0001,
  XWAIT6          ,       -1,       -1, G_BOX             |0x0300,   /* Object 50 XWAIT5 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0004, 0x0002, 0x0001,
  XWAIT7          ,       -1,       -1, G_BOX             |0x0300,   /* Object 51 XWAIT6 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0005, 0x0002, 0x0001,
  XWAIT8          ,       -1,       -1, G_BOX             |0x0300,   /* Object 52 XWAIT7 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0006, 0x0002, 0x0001,
  XWAIT9          ,       -1,       -1, G_BOX             |0x0300,   /* Object 53 XWAIT8 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0007, 0x0002, 0x0001,
  XWAIT10         ,       -1,       -1, G_BOX             |0x0300,   /* Object 54 XWAIT9 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0008, 0x0002, 0x0001,
  XWAIT11         ,       -1,       -1, G_BOX             |0x0300,   /* Object 55 XWAIT10 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0009, 0x0002, 0x0001,
  XWAIT12         ,       -1,       -1, G_BOX             |0x0300,   /* Object 56 XWAIT11 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x000A, 0x0002, 0x0001,
  XRED1           ,       -1,       -1, G_BOX             |0x0300,   /* Object 57 XWAIT12 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x000B, 0x0002, 0x0001,
  XRED2           ,       -1,       -1, G_BOX             |0x0300,   /* Object 58 XRED1 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0000, 0x0002, 0x0001,
  XRED3           ,       -1,       -1, G_BOX             |0x0300,   /* Object 59 XRED2 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0001, 0x0002, 0x0001,
  XRED4           ,       -1,       -1, G_BOX             |0x0300,   /* Object 60 XRED3 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0002, 0x0002, 0x0001,
  XRED5           ,       -1,       -1, G_BOX             |0x0300,   /* Object 61 XRED4 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0003, 0x0002, 0x0001,
  XRED6           ,       -1,       -1, G_BOX             |0x0300,   /* Object 62 XRED5 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0004, 0x0002, 0x0001,
  XRED7           ,       -1,       -1, G_BOX             |0x0300,   /* Object 63 XRED6 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0005, 0x0002, 0x0001,
  XRED8           ,       -1,       -1, G_BOX             |0x0300,   /* Object 64 XRED7 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0006, 0x0002, 0x0001,
  XRED9           ,       -1,       -1, G_BOX             |0x0300,   /* Object 65 XRED8 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0007, 0x0002, 0x0001,
  XRED10          ,       -1,       -1, G_BOX             |0x0300,   /* Object 66 XRED9 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0008, 0x0002, 0x0001,
  XRED11          ,       -1,       -1, G_BOX             |0x0300,   /* Object 67 XRED10 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x0009, 0x0002, 0x0001,
  XRED12          ,       -1,       -1, G_BOX             |0x0300,   /* Object 68 XRED11 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x000A, 0x0002, 0x0001,
  XERR1           ,       -1,       -1, G_BOX             |0x0300,   /* Object 69 XRED12 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0005, 0x000B, 0x0002, 0x0001,
  XERR2           ,       -1,       -1, G_FTEXT           ,   /* Object 70 XERR1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[106],
  0x000A, 0x0000, 0x0003, 0x0001,
  XERR3           ,       -1,       -1, G_FTEXT           ,   /* Object 71 XERR2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[107],
  0x000A, 0x0001, 0x0003, 0x0001,
  XERR4           ,       -1,       -1, G_FTEXT           ,   /* Object 72 XERR3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[108],
  0x000A, 0x0002, 0x0003, 0x0001,
  XERR5           ,       -1,       -1, G_FTEXT           ,   /* Object 73 XERR4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[109],
  0x000A, 0x0003, 0x0003, 0x0001,
  XERR6           ,       -1,       -1, G_FTEXT           ,   /* Object 74 XERR5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[110],
  0x000A, 0x0004, 0x0003, 0x0001,
  XERR7           ,       -1,       -1, G_FTEXT           ,   /* Object 75 XERR6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[111],
  0x000A, 0x0005, 0x0003, 0x0001,
  XERR8           ,       -1,       -1, G_FTEXT           ,   /* Object 76 XERR7 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[112],
  0x000A, 0x0006, 0x0003, 0x0001,
  XERR9           ,       -1,       -1, G_FTEXT           ,   /* Object 77 XERR8 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[113],
  0x000A, 0x0007, 0x0003, 0x0001,
  XERR10          ,       -1,       -1, G_FTEXT           ,   /* Object 78 XERR9 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[114],
  0x000A, 0x0008, 0x0003, 0x0001,
  XERR11          ,       -1,       -1, G_FTEXT           ,   /* Object 79 XERR10 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[115],
  0x000A, 0x0009, 0x0003, 0x0001,
  XERR12          ,       -1,       -1, G_FTEXT           ,   /* Object 80 XERR11 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[116],
  0x000A, 0x000A, 0x0003, 0x0001,
  XBOX3           ,       -1,       -1, G_FTEXT           ,   /* Object 81 XERR12 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[117],
  0x000A, 0x000B, 0x0003, 0x0001,
  XSETNO          , XMAIN           , XSET5           , G_IBOX            ,   /* Object 82 XBOX1 */
  NONE, NORMAL, (LONG)0x00001101L,
  0x0041, 0x0005, 0x000D, 0x0006,
  XSET1           ,       -1,       -1, G_BOXTEXT         ,   /* Object 83 XMAIN */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[118],
  0x0000, 0x0000, 0x000C, 0x0001,
  XSET2           ,       -1,       -1, G_BOXTEXT         ,   /* Object 84 XSET1 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[119],
  0x0000, 0x0001, 0x000C, 0x0001,
  XSET3           ,       -1,       -1, G_BOXTEXT         ,   /* Object 85 XSET2 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[120],
  0x0000, 0x0002, 0x000C, 0x0001,
  XSET4           ,       -1,       -1, G_BOXTEXT         ,   /* Object 86 XSET3 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[121],
  0x0000, 0x0003, 0x000C, 0x0001,
  XSET5           ,       -1,       -1, G_BOXTEXT         ,   /* Object 87 XSET4 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[122],
  0x0000, 0x0004, 0x000C, 0x0001,
  XBOX1           ,       -1,       -1, G_BOXTEXT         ,   /* Object 88 XSET5 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[123],
  0x0000, 0x0005, 0x000C, 0x0001,
        90,       -1,       -1, G_BOXTEXT         ,   /* Object 89 XSETNO */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[124],
  0x0041, 0x000C, 0x000C, 0x0001,
  XBOX2           ,       -1,       -1, G_TEXT            ,   /* Object 90  */
  NONE, NORMAL, (LONG)&rs_tedinfo[125],
  0x0002, 0x000E, 0x0015, 0x0001,
  XDELONE         , XWAIT           , XSELECT         , G_IBOX            ,   /* Object 91 XBOX2 */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x000F, 0x0015, 0x0004,
  XNOMAIL         ,       -1,       -1, G_BUTTON          ,   /* Object 92 XWAIT */
  SELECTABLE, NORMAL, (LONG)"Wait",
  0x0000, 0x0000, 0x000A, 0x0001,
  XAUTOPOL        ,       -1,       -1, G_BUTTON          ,   /* Object 93 XNOMAIL */
  SELECTABLE, NORMAL, (LONG)"\047No Mail\047",
  0x0000, 0x0001, 0x000A, 0x0001,
  XLOOP           ,       -1,       -1, G_BUTTON          ,   /* Object 94 XAUTOPOL */
  SELECTABLE, NORMAL, (LONG)"AutoStart",
  0x000B, 0x0000, 0x000A, 0x0001,
  XLOOPCNT        ,       -1,       -1, G_BUTTON          ,   /* Object 95 XLOOP */
  SELECTABLE, NORMAL, (LONG)"Loop",
  0x0000, 0x0003, 0x000A, 0x0001,
  XSELECT         ,       -1,       -1, G_FTEXT           ,   /* Object 96 XLOOPCNT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[126],
  0x000B, 0x0003, 0x000A, 0x0001,
  XBOX2           ,       -1,       -1, G_BUTTON          ,   /* Object 97 XSELECT */
  SELECTABLE, NORMAL, (LONG)"Select",
  0x000B, 0x0001, 0x000A, 0x0001,
  XDELALL         ,       -1,       -1, G_BOXTEXT         ,   /* Object 98 XDELONE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[127],
  0x001B, 0x0012, 0x0009, 0x0001,
       100,       -1,       -1, G_BOXTEXT         ,   /* Object 99 XDELALL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[128],
  0x0025, 0x0012, 0x0009, 0x0001,
       101,       -1,       -1, G_TEXT            ,   /* Object 100  */
  NONE, NORMAL, (LONG)&rs_tedinfo[129],
  0x0002, 0x0014, 0x042E, 0x0001,
  TIMEONE         ,       -1,       -1, G_STRING          ,   /* Object 101  */
  NONE, NORMAL, (LONG)"Only allow AutoStart between:",
  0x0002, 0x0015, 0x001D, 0x0001,
       103,       -1,       -1, G_FTEXT           ,   /* Object 102 TIMEONE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[130],
  0x0020, 0x0015, 0x000B, 0x0001,
  TIMETWO         ,       -1,       -1, G_STRING          ,   /* Object 103  */
  NONE, NORMAL, (LONG)"and",
  0x002C, 0x0015, 0x0003, 0x0001,
  XOK             ,       -1,       -1, G_FTEXT           ,   /* Object 104 TIMETWO */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[131],
  0x0030, 0x0015, 0x000B, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 105 XOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0041, 0x0015, 0x000C, 0x0001,
  
  /******** Tree 2 DIALOG3 ****************************************************/
        -1,        1, ABBSEDIT        , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0016,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[132],
  0x0000, 0x0000, 0x004F, 0x0001,
  AIMPORT         ,       -1,       -1, G_TEXT            ,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[133],
  0x0043, 0x0001, 0x0006, 0x0001,
  CIMPORT         ,       -1,       -1, G_FTEXT           ,   /* Object 3 AIMPORT */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[134],
  0x0005, 0x0002, 0x003F, 0x0001,
  AOK             ,       -1,       -1, G_BOX             |0x0300,   /* Object 4 CIMPORT */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0002, 0x0002, 0x0001,
  BIMPORT         ,       -1,       -1, G_BUTTON          ,   /* Object 5 AOK */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"OK",
  0x0049, 0x0002, 0x0004, 0x0010,
  AEXPORT         ,       -1,       -1, G_FTEXT           ,   /* Object 6 BIMPORT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[135],
  0x0008, 0x0003, 0x003C, 0x0001,
  CEXPORT         ,       -1,       -1, G_FTEXT           ,   /* Object 7 AEXPORT */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[136],
  0x0005, 0x0004, 0x003F, 0x0001,
  BEXPORT         ,       -1,       -1, G_BOX             |0x0300,   /* Object 8 CEXPORT */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0004, 0x0002, 0x0001,
  ACRUNCH         ,       -1,       -1, G_FTEXT           ,   /* Object 9 BEXPORT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[137],
  0x0008, 0x0005, 0x003C, 0x0001,
  CCRUNCH         ,       -1,       -1, G_FTEXT           ,   /* Object 10 ACRUNCH */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[138],
  0x0003, 0x0006, 0x0041, 0x0001,
  BCRUNCH         ,       -1,       -1, G_BOX             |0x0300,   /* Object 11 CCRUNCH */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0006, 0x0002, 0x0001,
  ASCANNER        ,       -1,       -1, G_FTEXT           ,   /* Object 12 BCRUNCH */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[139],
  0x0008, 0x0007, 0x003C, 0x0001,
  CSCANNER        ,       -1,       -1, G_FTEXT           ,   /* Object 13 ASCANNER */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[140],
  0x0004, 0x0008, 0x0040, 0x0001,
  BSCANNER        ,       -1,       -1, G_BOX             |0x0300,   /* Object 14 CSCANNER */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0008, 0x0002, 0x0001,
  AMSGED          ,       -1,       -1, G_FTEXT           ,   /* Object 15 BSCANNER */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[141],
  0x0008, 0x0009, 0x003C, 0x0001,
  CMSGED          ,       -1,       -1, G_FTEXT           ,   /* Object 16 AMSGED */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[142],
  0x0005, 0x000A, 0x003F, 0x0001,
  BMSGED          ,       -1,       -1, G_BOX             |0x0300,   /* Object 17 CMSGED */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x000A, 0x0002, 0x0001,
  ATXTED          ,       -1,       -1, G_FTEXT           ,   /* Object 18 BMSGED */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[143],
  0x0008, 0x000B, 0x003C, 0x0001,
  CTXTED          ,       -1,       -1, G_FTEXT           ,   /* Object 19 ATXTED */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[144],
  0x0005, 0x000C, 0x003F, 0x0001,
  BTXTED          ,       -1,       -1, G_BOX             |0x0300,   /* Object 20 CTXTED */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x000C, 0x0002, 0x0001,
  AMAILER         ,       -1,       -1, G_FTEXT           ,   /* Object 21 BTXTED */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[145],
  0x0008, 0x000D, 0x003C, 0x0001,
  CMAILER         ,       -1,       -1, G_FTEXT           ,   /* Object 22 AMAILER */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[146],
  0x0005, 0x000E, 0x003F, 0x0001,
  BMAILER         ,       -1,       -1, G_BOX             |0x0300,   /* Object 23 CMAILER */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x000E, 0x0002, 0x0001,
  ATERM           ,       -1,       -1, G_FTEXT           ,   /* Object 24 BMAILER */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[147],
  0x0008, 0x000F, 0x003C, 0x0001,
  CTERM           ,       -1,       -1, G_FTEXT           ,   /* Object 25 ATERM */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[148],
  0x0003, 0x0010, 0x0041, 0x0001,
  BTERM           ,       -1,       -1, G_BOX             |0x0300,   /* Object 26 CTERM */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0010, 0x0002, 0x0001,
  ANOTICE         ,       -1,       -1, G_FTEXT           ,   /* Object 27 BTERM */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[149],
  0x0008, 0x0011, 0x003C, 0x0001,
  AVIEW           ,       -1,       -1, G_TEXT            ,   /* Object 28 ANOTICE */
  NONE, NORMAL, (LONG)&rs_tedinfo[150],
  0x000D, 0x0012, 0x0030, 0x0001,
  ACANCEL         ,       -1,       -1, G_FTEXT           ,   /* Object 29 AVIEW */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[151],
  0x0005, 0x0013, 0x003F, 0x0001,
  ABBSTEXT        ,       -1,       -1, G_BOXTEXT         ,   /* Object 30 ACANCEL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[152],
  0x0049, 0x0013, 0x0004, 0x0002,
  ABBSEDIT        ,       -1,       -1, G_STRING          ,   /* Object 31 ABBSTEXT */
  NONE, NORMAL, (LONG)"AreaEditor:",
  0x0001, 0x0014, 0x000B, 0x0001,
         0,       -1,       -1, G_FTEXT           ,   /* Object 32 ABBSEDIT */
  EDITABLE|LASTOB|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[153],
  0x000D, 0x0014, 0x0037, 0x0001,
  
  /******** Tree 3 DIALOG5 ****************************************************/
        -1, XCMD            , XCMDOK          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004B, 0x0003,
  XCMDOK          ,       -1,       -1, G_FTEXT           ,   /* Object 1 XCMD */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[154],
  0x0002, 0x0001, 0x003C, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 2 XCMDOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0041, 0x0001, 0x0008, 0x0001,
  
  /******** Tree 4 DIALOG6 ****************************************************/
        -1, XPOLLTIM        , XPOLLOK         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0022, 0x0006,
  XPOLLCAN        ,       -1,       -1, G_FBOXTEXT        ,   /* Object 1 XPOLLTIM */
  EDITABLE, SHADOWED, (LONG)&rs_tedinfo[155],
  0x0002, 0x0001, 0x001E, 0x0002,
  XPOLLOK         ,       -1,       -1, G_BUTTON          ,   /* Object 2 XPOLLCAN */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0002, 0x0004, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 3 XPOLLOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0016, 0x0004, 0x000A, 0x0001,
  
  /******** Tree 5 DIALOG7 ****************************************************/
        -1,        1, AROK            , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0816,
  ORIGIN0         ,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Origin:",
  0x0003, 0x0001, 0x0007, 0x0001,
  BOSS0           ,       -1,       -1, G_FTEXT           ,   /* Object 2 ORIGIN0 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[156],
  0x000B, 0x0001, 0x0041, 0x0001,
  CNTAREAS        ,       -1,       -1, G_FTEXT           ,   /* Object 3 BOSS0 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[157],
  0x0005, 0x0002, 0x002E, 0x0001,
  CNTMAX          ,       -1,       -1, G_FTEXT           ,   /* Object 4 CNTAREAS */
  NONE, NORMAL, (LONG)&rs_tedinfo[158],
  0x003A, 0x0002, 0x0012, 0x0001,
         6,       -1,       -1, G_FTEXT           ,   /* Object 5 CNTMAX */
  NONE, NORMAL, (LONG)&rs_tedinfo[159],
  0x0041, 0x0003, 0x000B, 0x0001,
  ARROWFUL        ,        7, DELETE3         , G_BOX             ,   /* Object 6  */
  NONE, SHADOWED, (LONG)0x00FF1101L,
  0x0002, 0x0004, 0x004B, 0x0010,
  AREASBOX        ,       -1,       -1, G_TEXT            ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_tedinfo[160],
  0x0000, 0x0000, 0x004B, 0x0001,
  NODE1           ,        9, ORIGIN3         , G_IBOX            ,   /* Object 8 AREASBOX */
  NONE, NORMAL, (LONG)0x00001101L,
  0x0001, 0x0001, 0x0044, 0x000F,
  DNAME1          ,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"Name:",
  0x0002, 0x0000, 0x0005, 0x0001,
  DAYS1           ,       -1,       -1, G_FTEXT           ,   /* Object 10 DNAME1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[161],
  0x0008, 0x0000, 0x0030, 0x0001,
  BOSS1           ,       -1,       -1, G_FTEXT           ,   /* Object 11 DAYS1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[162],
  0x003B, 0x0000, 0x0009, 0x0001,
  NPATH1          ,       -1,       -1, G_FTEXT           ,   /* Object 12 BOSS1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[163],
  0x0002, 0x0001, 0x002E, 0x0001,
  PATH1           ,       -1,       -1, G_STRING          ,   /* Object 13 NPATH1 */
  NONE, NORMAL, (LONG)"Path:",
  0x0002, 0x0002, 0x0005, 0x0001,
  NORG1           ,       -1,       -1, G_FTEXT           ,   /* Object 14 PATH1 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[164],
  0x0008, 0x0002, 0x003C, 0x0001,
  ORIGIN1         ,       -1,       -1, G_STRING          ,   /* Object 15 NORG1 */
  NONE, NORMAL, (LONG)"Origin:",
  0x0000, 0x0003, 0x0007, 0x0001,
        17,       -1,       -1, G_FTEXT           ,   /* Object 16 ORIGIN1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[165],
  0x0008, 0x0003, 0x003C, 0x0001,
  DNAME2          ,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"Name:",
  0x0002, 0x0005, 0x0005, 0x0001,
  DAYS2           ,       -1,       -1, G_FTEXT           ,   /* Object 18 DNAME2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[166],
  0x0008, 0x0005, 0x0030, 0x0001,
  BOSS2           ,       -1,       -1, G_FTEXT           ,   /* Object 19 DAYS2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[167],
  0x003B, 0x0005, 0x0009, 0x0001,
  NPATH2          ,       -1,       -1, G_FTEXT           ,   /* Object 20 BOSS2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[168],
  0x0002, 0x0006, 0x002E, 0x0001,
  PATH2           ,       -1,       -1, G_STRING          ,   /* Object 21 NPATH2 */
  NONE, NORMAL, (LONG)"Path:",
  0x0002, 0x0007, 0x0005, 0x0001,
  NORG2           ,       -1,       -1, G_FTEXT           ,   /* Object 22 PATH2 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[169],
  0x0008, 0x0007, 0x003C, 0x0001,
  ORIGIN2         ,       -1,       -1, G_STRING          ,   /* Object 23 NORG2 */
  NONE, NORMAL, (LONG)"Origin:",
  0x0000, 0x0008, 0x0007, 0x0001,
        25,       -1,       -1, G_FTEXT           ,   /* Object 24 ORIGIN2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[170],
  0x0008, 0x0008, 0x003C, 0x0001,
  DNAME3          ,       -1,       -1, G_STRING          ,   /* Object 25  */
  NONE, NORMAL, (LONG)"Name:",
  0x0002, 0x000A, 0x0005, 0x0001,
  DAYS3           ,       -1,       -1, G_FTEXT           ,   /* Object 26 DNAME3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[171],
  0x0008, 0x000A, 0x0030, 0x0001,
  BOSS3           ,       -1,       -1, G_FTEXT           ,   /* Object 27 DAYS3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[172],
  0x003B, 0x000A, 0x0009, 0x0001,
  NPATH3          ,       -1,       -1, G_FTEXT           ,   /* Object 28 BOSS3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[173],
  0x0002, 0x000B, 0x002E, 0x0001,
  PATH3           ,       -1,       -1, G_STRING          ,   /* Object 29 NPATH3 */
  NONE, NORMAL, (LONG)"Path:",
  0x0002, 0x000C, 0x0005, 0x0001,
  NORG3           ,       -1,       -1, G_FTEXT           ,   /* Object 30 PATH3 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[174],
  0x0008, 0x000C, 0x003C, 0x0001,
  ORIGIN3         ,       -1,       -1, G_STRING          ,   /* Object 31 NORG3 */
  NONE, NORMAL, (LONG)"Origin:",
  0x0000, 0x000D, 0x0007, 0x0001,
  AREASBOX        ,       -1,       -1, G_FTEXT           ,   /* Object 32 ORIGIN3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[175],
  0x0008, 0x000D, 0x003C, 0x0001,
  EXTEND1         ,       -1,       -1, G_BOXTEXT         ,   /* Object 33 NODE1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[176],
  0x0046, 0x0001, 0x0004, 0x0001,
  INSERT1         ,       -1,       -1, G_BOXTEXT         ,   /* Object 34 EXTEND1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[177],
  0x0046, 0x0002, 0x0004, 0x0001,
  DELETE1         ,       -1,       -1, G_BOXTEXT         ,   /* Object 35 INSERT1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[178],
  0x0046, 0x0003, 0x0004, 0x0001,
        37,       -1,       -1, G_BOXTEXT         ,   /* Object 36 DELETE1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[179],
  0x0046, 0x0004, 0x0004, 0x0001,
  NODE2           ,       -1,       -1, G_IBOX            |0x0200,   /* Object 37  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0005, 0x004B, 0x0001,
  EXTEND2         ,       -1,       -1, G_BOXTEXT         ,   /* Object 38 NODE2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[180],
  0x0046, 0x0006, 0x0004, 0x0001,
  INSERT2         ,       -1,       -1, G_BOXTEXT         ,   /* Object 39 EXTEND2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[181],
  0x0046, 0x0007, 0x0004, 0x0001,
  DELETE2         ,       -1,       -1, G_BOXTEXT         ,   /* Object 40 INSERT2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[182],
  0x0046, 0x0008, 0x0004, 0x0001,
        42,       -1,       -1, G_BOXTEXT         ,   /* Object 41 DELETE2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[183],
  0x0046, 0x0009, 0x0004, 0x0001,
  NODE3           ,       -1,       -1, G_IBOX            |0x0200,   /* Object 42  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x000A, 0x004B, 0x0001,
  EXTEND3         ,       -1,       -1, G_BOXTEXT         ,   /* Object 43 NODE3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[184],
  0x0046, 0x000B, 0x0004, 0x0001,
  INSERT3         ,       -1,       -1, G_BOXTEXT         ,   /* Object 44 EXTEND3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[185],
  0x0046, 0x000C, 0x0004, 0x0001,
  DELETE3         ,       -1,       -1, G_BOXTEXT         ,   /* Object 45 INSERT3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[186],
  0x0046, 0x000D, 0x0004, 0x0001,
         6,       -1,       -1, G_BOXTEXT         ,   /* Object 46 DELETE3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[187],
  0x0046, 0x000E, 0x0004, 0x0001,
  ARSEARCH        , ARROWDN         , ARROWUP         , G_IBOX            ,   /* Object 47 ARROWFUL */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0002, 0x0015, 0x002A, 0x0001,
  ARROWBK         ,       -1,       -1, G_BOXCHAR         ,   /* Object 48 ARROWDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0000, 0x0000, 0x0005, 0x0001,
  ARROWUP         , ARROWFD         , ARROWFD         , G_BOX             ,   /* Object 49 ARROWBK */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1143L,
  0x0005, 0x0000, 0x0020, 0x0001,
  ARROWBK         ,       -1,       -1, G_BOX             ,   /* Object 50 ARROWFD */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1112L,
  0x0003, 0x0000, 0x0003, 0x0001,
  ARROWFUL        ,       -1,       -1, G_BOXCHAR         ,   /* Object 51 ARROWUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0025, 0x0000, 0x0005, 0x0001,
  ARINSERT        ,       -1,       -1, G_BUTTON          ,   /* Object 52 ARSEARCH */
  SELECTABLE|EXIT, NORMAL, (LONG)"Search",
  0x002D, 0x0015, 0x0007, 0x0001,
  ARCANCEL        ,       -1,       -1, G_BOXTEXT         ,   /* Object 53 ARINSERT */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)&rs_tedinfo[188],
  0x0036, 0x0015, 0x0007, 0x0001,
  AROK            ,       -1,       -1, G_BUTTON          ,   /* Object 54 ARCANCEL */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x003E, 0x0015, 0x0007, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 55 AROK */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)"SAVE",
  0x0046, 0x0015, 0x0007, 0x0001,
  
  /******** Tree 6 DIALOG8 ****************************************************/
        -1,        1,       74, G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x0000, 0x0000, 0x004F, 0x0816,
  REQNODE         ,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[189],
  0x0000, 0x0000, 0x004F, 0x0001,
         3,       -1,       -1, G_FBOXTEXT        ,   /* Object 2 REQNODE */
  EDITABLE, SHADOWED, (LONG)&rs_tedinfo[190],
  0x0002, 0x0002, 0x004B, 0x0002,
         4,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[191],
  0x0036, 0x0004, 0x000D, 0x0001,
  REQLEFT         ,       -1,       -1, G_TEXT            ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_tedinfo[192],
  0x0044, 0x0004, 0x0009, 0x0001,
  REQRIGHT        , PROG1           , PASS10          , G_IBOX            ,   /* Object 5 REQLEFT */
  NONE, NORMAL, (LONG)0x00001101L,
  0x0001, 0x0005, 0x001A, 0x000A,
  PASS1           ,       -1,       -1, G_FTEXT           ,   /* Object 6 PROG1 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[193],
  0x0001, 0x0000, 0x000C, 0x0001,
  PROG2           ,       -1,       -1, G_FTEXT           ,   /* Object 7 PASS1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[194],
  0x000F, 0x0000, 0x000A, 0x0001,
  PASS2           ,       -1,       -1, G_FTEXT           ,   /* Object 8 PROG2 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[195],
  0x0001, 0x0001, 0x000C, 0x0001,
  PROG3           ,       -1,       -1, G_FTEXT           ,   /* Object 9 PASS2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[196],
  0x000F, 0x0001, 0x000A, 0x0001,
  PASS3           ,       -1,       -1, G_FTEXT           ,   /* Object 10 PROG3 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[197],
  0x0001, 0x0002, 0x000C, 0x0001,
  PROG4           ,       -1,       -1, G_FTEXT           ,   /* Object 11 PASS3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[198],
  0x000F, 0x0002, 0x000A, 0x0001,
  PASS4           ,       -1,       -1, G_FTEXT           ,   /* Object 12 PROG4 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[199],
  0x0001, 0x0003, 0x000C, 0x0001,
  PROG5           ,       -1,       -1, G_FTEXT           ,   /* Object 13 PASS4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[200],
  0x000F, 0x0003, 0x000A, 0x0001,
  PASS5           ,       -1,       -1, G_FTEXT           ,   /* Object 14 PROG5 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[201],
  0x0001, 0x0004, 0x000C, 0x0001,
  PROG6           ,       -1,       -1, G_FTEXT           ,   /* Object 15 PASS5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[202],
  0x000F, 0x0004, 0x000A, 0x0001,
  PASS6           ,       -1,       -1, G_FTEXT           ,   /* Object 16 PROG6 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[203],
  0x0001, 0x0005, 0x000C, 0x0001,
  PROG7           ,       -1,       -1, G_FTEXT           ,   /* Object 17 PASS6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[204],
  0x000F, 0x0005, 0x000A, 0x0001,
  PASS7           ,       -1,       -1, G_FTEXT           ,   /* Object 18 PROG7 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[205],
  0x0001, 0x0006, 0x000C, 0x0001,
  PROG8           ,       -1,       -1, G_FTEXT           ,   /* Object 19 PASS7 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[206],
  0x000F, 0x0006, 0x000A, 0x0001,
  PASS8           ,       -1,       -1, G_FTEXT           ,   /* Object 20 PROG8 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[207],
  0x0001, 0x0007, 0x000C, 0x0001,
  PROG9           ,       -1,       -1, G_FTEXT           ,   /* Object 21 PASS8 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[208],
  0x000F, 0x0007, 0x000A, 0x0001,
  PASS9           ,       -1,       -1, G_FTEXT           ,   /* Object 22 PROG9 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[209],
  0x0001, 0x0008, 0x000C, 0x0001,
  PROG10          ,       -1,       -1, G_FTEXT           ,   /* Object 23 PASS9 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[210],
  0x000F, 0x0008, 0x000A, 0x0001,
  PASS10          ,       -1,       -1, G_FTEXT           ,   /* Object 24 PROG10 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[211],
  0x0001, 0x0009, 0x000C, 0x0001,
  REQLEFT         ,       -1,       -1, G_FTEXT           ,   /* Object 25 PASS10 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[212],
  0x000F, 0x0009, 0x000A, 0x0001,
  REQBOXES        , PROG11          , PASS20          , G_IBOX            ,   /* Object 26 REQRIGHT */
  NONE, NORMAL, (LONG)0x00001101L,
  0x001B, 0x0005, 0x0019, 0x000A,
  PASS11          ,       -1,       -1, G_FTEXT           ,   /* Object 27 PROG11 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[213],
  0x0001, 0x0000, 0x000C, 0x0001,
  PROG12          ,       -1,       -1, G_FTEXT           ,   /* Object 28 PASS11 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[214],
  0x000F, 0x0000, 0x000A, 0x0001,
  PASS12          ,       -1,       -1, G_FTEXT           ,   /* Object 29 PROG12 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[215],
  0x0001, 0x0001, 0x000C, 0x0001,
  PROG13          ,       -1,       -1, G_FTEXT           ,   /* Object 30 PASS12 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[216],
  0x000F, 0x0001, 0x000A, 0x0001,
  PASS13          ,       -1,       -1, G_FTEXT           ,   /* Object 31 PROG13 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[217],
  0x0001, 0x0002, 0x000C, 0x0001,
  PROG14          ,       -1,       -1, G_FTEXT           ,   /* Object 32 PASS13 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[218],
  0x000F, 0x0002, 0x000A, 0x0001,
  PASS14          ,       -1,       -1, G_FTEXT           ,   /* Object 33 PROG14 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[219],
  0x0001, 0x0003, 0x000C, 0x0001,
  PROG15          ,       -1,       -1, G_FTEXT           ,   /* Object 34 PASS14 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[220],
  0x000F, 0x0003, 0x000A, 0x0001,
  PASS15          ,       -1,       -1, G_FTEXT           ,   /* Object 35 PROG15 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[221],
  0x0001, 0x0004, 0x000C, 0x0001,
  PROG16          ,       -1,       -1, G_FTEXT           ,   /* Object 36 PASS15 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[222],
  0x000F, 0x0004, 0x000A, 0x0001,
  PASS16          ,       -1,       -1, G_FTEXT           ,   /* Object 37 PROG16 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[223],
  0x0001, 0x0005, 0x000C, 0x0001,
  PROG17          ,       -1,       -1, G_FTEXT           ,   /* Object 38 PASS16 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[224],
  0x000F, 0x0005, 0x000A, 0x0001,
  PASS17          ,       -1,       -1, G_FTEXT           ,   /* Object 39 PROG17 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[225],
  0x0001, 0x0006, 0x000C, 0x0001,
  PROG18          ,       -1,       -1, G_FTEXT           ,   /* Object 40 PASS17 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[226],
  0x000F, 0x0006, 0x000A, 0x0001,
  PASS18          ,       -1,       -1, G_FTEXT           ,   /* Object 41 PROG18 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[227],
  0x0001, 0x0007, 0x000C, 0x0001,
  PROG19          ,       -1,       -1, G_FTEXT           ,   /* Object 42 PASS18 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[228],
  0x000F, 0x0007, 0x000A, 0x0001,
  PASS19          ,       -1,       -1, G_FTEXT           ,   /* Object 43 PROG19 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[229],
  0x0001, 0x0008, 0x000C, 0x0001,
  PROG20          ,       -1,       -1, G_FTEXT           ,   /* Object 44 PASS19 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[230],
  0x000F, 0x0008, 0x000A, 0x0001,
  PASS20          ,       -1,       -1, G_FTEXT           ,   /* Object 45 PROG20 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[231],
  0x0001, 0x0009, 0x000C, 0x0001,
  REQRIGHT        ,       -1,       -1, G_FTEXT           ,   /* Object 46 PASS20 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[232],
  0x000F, 0x0009, 0x000A, 0x0001,
  REQBOX2         , GETNODE1        , GETNODE0        , G_BOX             ,   /* Object 47 REQBOXES */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0036, 0x0005, 0x000E, 0x000A,
  GETNODE2        ,       -1,       -1, G_BOXTEXT         ,   /* Object 48 GETNODE1 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[233],
  0x0000, 0x0000, 0x000D, 0x0001,
  GETNODE3        ,       -1,       -1, G_BOXTEXT         ,   /* Object 49 GETNODE2 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[234],
  0x0000, 0x0001, 0x000D, 0x0001,
  GETNODE4        ,       -1,       -1, G_BOXTEXT         ,   /* Object 50 GETNODE3 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[235],
  0x0000, 0x0002, 0x000D, 0x0001,
  GETNODE5        ,       -1,       -1, G_BOXTEXT         ,   /* Object 51 GETNODE4 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[236],
  0x0000, 0x0003, 0x000D, 0x0001,
  GETNODE6        ,       -1,       -1, G_BOXTEXT         ,   /* Object 52 GETNODE5 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[237],
  0x0000, 0x0004, 0x000D, 0x0001,
  GETNODE7        ,       -1,       -1, G_BOXTEXT         ,   /* Object 53 GETNODE6 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[238],
  0x0000, 0x0005, 0x000D, 0x0001,
  GETNODE8        ,       -1,       -1, G_BOXTEXT         ,   /* Object 54 GETNODE7 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[239],
  0x0000, 0x0006, 0x000D, 0x0001,
  GETNODE9        ,       -1,       -1, G_BOXTEXT         ,   /* Object 55 GETNODE8 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[240],
  0x0000, 0x0007, 0x000D, 0x0001,
  GETNODE0        ,       -1,       -1, G_BOXTEXT         ,   /* Object 56 GETNODE9 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[241],
  0x0000, 0x0008, 0x000D, 0x0001,
  REQBOXES        ,       -1,       -1, G_BOXTEXT         ,   /* Object 57 GETNODE0 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[242],
  0x0000, 0x0009, 0x000D, 0x0001,
        69, FILELST0        , FILELST9        , G_BOX             ,   /* Object 58 REQBOX2 */
  NONE, NORMAL, (LONG)0x00001101L,
  0x0044, 0x0005, 0x000A, 0x000A,
  FILELST1        ,       -1,       -1, G_BOXTEXT         ,   /* Object 59 FILELST0 */
  SELECTABLE|RBUTTON|TOUCHEXIT, SELECTED, (LONG)&rs_tedinfo[243],
  0x0000, 0x0000, 0x0009, 0x0001,
  FILELST2        ,       -1,       -1, G_BOXTEXT         ,   /* Object 60 FILELST1 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[244],
  0x0000, 0x0001, 0x0009, 0x0001,
  FILELST3        ,       -1,       -1, G_BOXTEXT         ,   /* Object 61 FILELST2 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[245],
  0x0000, 0x0002, 0x0009, 0x0001,
  FILELST4        ,       -1,       -1, G_BOXTEXT         ,   /* Object 62 FILELST3 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[246],
  0x0000, 0x0003, 0x0009, 0x0001,
  FILELST5        ,       -1,       -1, G_BOXTEXT         ,   /* Object 63 FILELST4 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[247],
  0x0000, 0x0004, 0x0009, 0x0001,
  FILELST6        ,       -1,       -1, G_BOXTEXT         ,   /* Object 64 FILELST5 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[248],
  0x0000, 0x0005, 0x0009, 0x0001,
  FILELST7        ,       -1,       -1, G_BOXTEXT         ,   /* Object 65 FILELST6 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[249],
  0x0000, 0x0006, 0x0009, 0x0001,
  FILELST8        ,       -1,       -1, G_BOXTEXT         ,   /* Object 66 FILELST7 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[250],
  0x0000, 0x0007, 0x0009, 0x0001,
  FILELST9        ,       -1,       -1, G_BOXTEXT         ,   /* Object 67 FILELST8 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[251],
  0x0000, 0x0008, 0x0009, 0x0001,
  REQBOX2         ,       -1,       -1, G_BOXTEXT         ,   /* Object 68 FILELST9 */
  SELECTABLE|RBUTTON|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[252],
  0x0000, 0x0009, 0x0009, 0x0001,
        70,       -1,       -1, G_TEXT            ,   /* Object 69  */
  NONE, NORMAL, (LONG)&rs_tedinfo[253],
  0x0036, 0x000F, 0x000D, 0x0001,
        71,       -1,       -1, G_TEXT            ,   /* Object 70  */
  NONE, NORMAL, (LONG)&rs_tedinfo[254],
  0x0044, 0x000F, 0x0009, 0x0001,
        72,       -1,       -1, G_TEXT            ,   /* Object 71  */
  NONE, NORMAL, (LONG)&rs_tedinfo[255],
  0x0002, 0x0010, 0x0015, 0x0001,
        73,       -1,       -1, G_TEXT            ,   /* Object 72  */
  NONE, NORMAL, (LONG)&rs_tedinfo[256],
  0x0036, 0x080F, 0x000D, 0x0001,
        74,       -1,       -1, G_TEXT            ,   /* Object 73  */
  NONE, NORMAL, (LONG)&rs_tedinfo[257],
  0x0044, 0x080F, 0x0009, 0x0001,
         0, REQDEF          , REQCANCE        , G_BOX             ,   /* Object 74  */
  NONE, NORMAL, (LONG)0x00FF1123L,
  0x0000, 0x0011, 0x004F, 0x0805,
        80, GETFILE1        , GETFILE4        , G_BOX             ,   /* Object 75 REQDEF */
  NONE, OUTLINED|SHADOWED, (LONG)0x00FF1101L,
  0x0002, 0x0001, 0x0019, 0x0004,
  GETFILE2        ,       -1,       -1, G_FTEXT           ,   /* Object 76 GETFILE1 */
  SELECTABLE|EXIT|EDITABLE, NORMAL, (LONG)&rs_tedinfo[258],
  0x0000, 0x0000, 0x0019, 0x0001,
  GETFILE3        ,       -1,       -1, G_FTEXT           ,   /* Object 77 GETFILE2 */
  SELECTABLE|EXIT|EDITABLE, NORMAL, (LONG)&rs_tedinfo[259],
  0x0000, 0x0001, 0x0019, 0x0001,
  GETFILE4        ,       -1,       -1, G_FTEXT           ,   /* Object 78 GETFILE3 */
  SELECTABLE|EXIT|EDITABLE, NORMAL, (LONG)&rs_tedinfo[260],
  0x0000, 0x0002, 0x0019, 0x0001,
  REQDEF          ,       -1,       -1, G_FTEXT           ,   /* Object 79 GETFILE4 */
  SELECTABLE|EXIT|EDITABLE, NORMAL, (LONG)&rs_tedinfo[261],
  0x0000, 0x0003, 0x0019, 0x0001,
  REQSAVEZ        , REQLOAD         , REQBLOCK        , G_BOX             ,   /* Object 80  */
  NONE, OUTLINED|SHADOWED, (LONG)0x00FF1101L,
  0x001F, 0x0001, 0x0016, 0x0004,
  REQSELEC        ,       -1,       -1, G_BOXTEXT         ,   /* Object 81 REQLOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[262],
  0x0000, 0x0000, 0x0016, 0x0001,
  REQFILES        ,       -1,       -1, G_BOXTEXT         ,   /* Object 82 REQSELEC */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[263],
  0x0000, 0x0001, 0x0016, 0x0002,
  REQCLEAR        ,       -1,       -1, G_BOXTEXT         ,   /* Object 83 REQFILES */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[264],
  0x0000, 0x0003, 0x0008, 0x0001,
  REQBLOCK        ,       -1,       -1, G_BOXTEXT         ,   /* Object 84 REQCLEAR */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[265],
  0x0008, 0x0003, 0x0007, 0x0001,
        80,       -1,       -1, G_BOXTEXT         ,   /* Object 85 REQBLOCK */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[266],
  0x000F, 0x0003, 0x0007, 0x0001,
  REQSAVE         ,       -1,       -1, G_BUTTON          ,   /* Object 86 REQSAVEZ */
  SELECTABLE|EXIT, NORMAL, (LONG)"ZoneReq",
  0x0039, 0x0001, 0x0009, 0x0002,
  REQCANCE        ,       -1,       -1, G_BUTTON          ,   /* Object 87 REQSAVE */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"Request",
  0x0044, 0x0001, 0x0009, 0x0002,
        74,       -1,       -1, G_BUTTON          ,   /* Object 88 REQCANCE */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)"EXIT",
  0x0039, 0x0004, 0x0014, 0x0001,
  
  /******** Tree 7 DIALOG9 ****************************************************/
        -1, FBOX            , REQINFO         , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1123L,
  0x0000, 0x0000, 0x004F, 0x0816,
  FILEFULL        , FNAME1          , FNAME18         , G_BOX             ,   /* Object 1 FBOX */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0014,
  FNAME2          ,       -1,       -1, G_STRING          ,   /* Object 2 FNAME1 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0001, 0x004D, 0x0001,
  FNAME3          ,       -1,       -1, G_STRING          ,   /* Object 3 FNAME2 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0002, 0x004D, 0x0001,
  FNAME4          ,       -1,       -1, G_STRING          ,   /* Object 4 FNAME3 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0003, 0x004D, 0x0001,
  FNAME5          ,       -1,       -1, G_STRING          ,   /* Object 5 FNAME4 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0004, 0x004D, 0x0001,
  FNAME6          ,       -1,       -1, G_STRING          ,   /* Object 6 FNAME5 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0005, 0x004D, 0x0001,
  FNAME7          ,       -1,       -1, G_STRING          ,   /* Object 7 FNAME6 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0006, 0x004D, 0x0001,
  FNAME8          ,       -1,       -1, G_STRING          ,   /* Object 8 FNAME7 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0007, 0x004D, 0x0001,
  FNAME9          ,       -1,       -1, G_STRING          ,   /* Object 9 FNAME8 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0008, 0x004D, 0x0001,
  FNAME10         ,       -1,       -1, G_STRING          ,   /* Object 10 FNAME9 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0009, 0x004D, 0x0001,
  FNAME11         ,       -1,       -1, G_STRING          ,   /* Object 11 FNAME10 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000A, 0x004D, 0x0001,
  FNAME12         ,       -1,       -1, G_STRING          ,   /* Object 12 FNAME11 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000B, 0x004D, 0x0001,
  FNAME13         ,       -1,       -1, G_STRING          ,   /* Object 13 FNAME12 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000C, 0x004D, 0x0001,
  FNAME14         ,       -1,       -1, G_STRING          ,   /* Object 14 FNAME13 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000D, 0x004D, 0x0001,
  FNAME15         ,       -1,       -1, G_STRING          ,   /* Object 15 FNAME14 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000E, 0x004D, 0x0001,
  FNAME16         ,       -1,       -1, G_STRING          ,   /* Object 16 FNAME15 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000F, 0x004D, 0x0001,
  FNAME17         ,       -1,       -1, G_STRING          ,   /* Object 17 FNAME16 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0010, 0x004D, 0x0001,
  FNAME18         ,       -1,       -1, G_STRING          ,   /* Object 18 FNAME17 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0011, 0x004D, 0x0001,
  FBOX            ,       -1,       -1, G_STRING          ,   /* Object 19 FNAME18 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0012, 0x004D, 0x0001,
  FOK             , FILEDNDN        , FILEUPUP        , G_IBOX            ,   /* Object 20 FILEFULL */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0814, 0x0041, 0x0001,
  FILEDN          ,       -1,       -1, G_BOXTEXT         ,   /* Object 21 FILEDNDN */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[267],
  0x0000, 0x0000, 0x0004, 0x0001,
  FILEBK          ,       -1,       -1, G_BOXCHAR         ,   /* Object 22 FILEDN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0004, 0x0000, 0x0004, 0x0001,
  FILEUP          , FILEFD          , FILEFD          , G_BOX             ,   /* Object 23 FILEBK */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1143L,
  0x0008, 0x0000, 0x002F, 0x0001,
  FILEBK          ,       -1,       -1, G_BOX             ,   /* Object 24 FILEFD */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1112L,
  0x0006, 0x0000, 0x0003, 0x0001,
  FILEUPUP        ,       -1,       -1, G_BOXCHAR         ,   /* Object 25 FILEUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0037, 0x0000, 0x0004, 0x0001,
  FILEFULL        ,       -1,       -1, G_BOXTEXT         ,   /* Object 26 FILEUPUP */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[268],
  0x003B, 0x0000, 0x0004, 0x0001,
  REQINFO         ,       -1,       -1, G_BUTTON          ,   /* Object 27 FOK */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"OK",
  0x0044, 0x0814, 0x0009, 0x0001,
         0,       -1,       -1, G_TEXT            ,   /* Object 28 REQINFO */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[269],
  0x000A, 0x0815, 0x002F, 0x0001,
  
  /******** Tree 8 DIALOG10 ****************************************************/
        -1, PASSEDIT        , PASSOK          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002A, 0x0004,
  PASSOK          ,       -1,       -1, G_FBOXTEXT        ,   /* Object 1 PASSEDIT */
  EDITABLE, SHADOWED, (LONG)&rs_tedinfo[270],
  0x0003, 0x0001, 0x0019, 0x0002,
         0,       -1,       -1, G_BUTTON          ,   /* Object 2 PASSOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x001F, 0x0002, 0x0008, 0x0001,
  
  /******** Tree 9 DIALOG11 ****************************************************/
        -1,        1,       56, G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004B, 0x0816,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[271],
  0x0003, 0x0800, 0x0016, 0x0001,
         3,       -1,       -1, G_BOXTEXT         ,   /* Object 2  */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[272],
  0x001B, 0x0800, 0x0016, 0x0001,
         4,       -1,       -1, G_BOXTEXT         ,   /* Object 3  */
  SELECTABLE|EXIT, SHADOWED, (LONG)&rs_tedinfo[273],
  0x0033, 0x0800, 0x0016, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"HELP - This screen",
  0x0004, 0x0002, 0x0012, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"n - Utility n",
  0x001C, 0x0002, 0x000D, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"n - AutoPoll n",
  0x0034, 0x0002, 0x000E, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"UNDO - Notebook",
  0x0004, 0x0003, 0x000F, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"B - Mailer",
  0x001C, 0x0003, 0x000A, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"A - Edit areas",
  0x0034, 0x0003, 0x000E, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"0 - Poll selected",
  0x0007, 0x0004, 0x0011, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"C - Cruncher",
  0x001C, 0x0004, 0x000C, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"B - Execute *.BAT",
  0x0034, 0x0004, 0x0011, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"n - Start poll n",
  0x0007, 0x0005, 0x0010, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"E - Export",
  0x001C, 0x0005, 0x000A, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"E - Hatch file",
  0x0034, 0x0005, 0x000E, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"F1 - MainSetup",
  0x0006, 0x0006, 0x000E, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"F - Filemanager",
  0x001C, 0x0006, 0x000F, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"F - Find 1 (Program)",
  0x0034, 0x0006, 0x0014, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 19  */
  NONE, NORMAL, (LONG)"F2 - 1st   \042",
  0x0006, 0x0007, 0x000C, 0x0001,
        21,       -1,       -1, G_STRING          ,   /* Object 20  */
  NONE, NORMAL, (LONG)"I - Import",
  0x001C, 0x0007, 0x000A, 0x0001,
        22,       -1,       -1, G_STRING          ,   /* Object 21  */
  NONE, NORMAL, (LONG)"G - Execute *.PRG",
  0x0034, 0x0007, 0x0011, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  NONE, NORMAL, (LONG)"F3 - 2nd   \042",
  0x0006, 0x0008, 0x000C, 0x0001,
        24,       -1,       -1, G_STRING          ,   /* Object 23  */
  NONE, NORMAL, (LONG)"M - Message editor",
  0x001C, 0x0008, 0x0012, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  NONE, NORMAL, (LONG)"H - Find 2 (Utility)",
  0x0034, 0x0008, 0x0014, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  NONE, NORMAL, (LONG)"F4 - 3rd   \042",
  0x0006, 0x0009, 0x000C, 0x0001,
        27,       -1,       -1, G_STRING          ,   /* Object 26  */
  NONE, NORMAL, (LONG)"P - Programmanager",
  0x001C, 0x0009, 0x0012, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  NONE, NORMAL, (LONG)"I - Info",
  0x0034, 0x0009, 0x0008, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  NONE, NORMAL, (LONG)"F5 - 4th   \042",
  0x0006, 0x000A, 0x000C, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  NONE, NORMAL, (LONG)"Q - Quit AVALON",
  0x001C, 0x000A, 0x000F, 0x0001,
        31,       -1,       -1, G_STRING          ,   /* Object 30  */
  NONE, NORMAL, (LONG)"M - Areafix manager",
  0x0034, 0x000A, 0x0013, 0x0001,
        32,       -1,       -1, G_STRING          ,   /* Object 31  */
  NONE, NORMAL, (LONG)"F6 - 5th   \042",
  0x0006, 0x000B, 0x000C, 0x0001,
        33,       -1,       -1, G_STRING          ,   /* Object 32  */
  NONE, NORMAL, (LONG)"S - Scanner",
  0x001C, 0x000B, 0x000B, 0x0001,
        34,       -1,       -1, G_STRING          ,   /* Object 33  */
  NONE, NORMAL, (LONG)"O - Session setup",
  0x0034, 0x000B, 0x0011, 0x0001,
        35,       -1,       -1, G_STRING          ,   /* Object 34  */
  NONE, NORMAL, (LONG)"F10 - Redraw infobox",
  0x0005, 0x000C, 0x0014, 0x0001,
        36,       -1,       -1, G_STRING          ,   /* Object 35  */
  NONE, NORMAL, (LONG)"T - Text editor",
  0x001C, 0x000C, 0x000F, 0x0001,
        37,       -1,       -1, G_STRING          ,   /* Object 36  */
  NONE, NORMAL, (LONG)"P - Special",
  0x0034, 0x000C, 0x000B, 0x0001,
        38,       -1,       -1, G_STRING          ,   /* Object 37  */
  NONE, NORMAL, (LONG)"X - Terminal",
  0x001C, 0x000D, 0x000C, 0x0001,
        39,       -1,       -1, G_STRING          ,   /* Object 38  */
  NONE, NORMAL, (LONG)"Q - Quit AVALON",
  0x0034, 0x000D, 0x000F, 0x0001,
  INFOEXIT        ,       -1,       -1, G_STRING          ,   /* Object 39  */
  NONE, NORMAL, (LONG)"R - Request file",
  0x0034, 0x000E, 0x0010, 0x0001,
        41,       -1,       -1, G_BOXTEXT         ,   /* Object 40 INFOEXIT */
  SELECTABLE|DEFAULT|EXIT, SHADOWED, (LONG)&rs_tedinfo[274],
  0x0003, 0x000F, 0x002E, 0x0001,
        42,       -1,       -1, G_STRING          ,   /* Object 41  */
  NONE, NORMAL, (LONG)"S - Save config",
  0x0034, 0x000F, 0x000F, 0x0001,
        43,       -1,       -1, G_TEXT            ,   /* Object 42  */
  NONE, NORMAL, (LONG)&rs_tedinfo[275],
  0x0003, 0x0010, 0x002F, 0x0001,
        44,       -1,       -1, G_STRING          ,   /* Object 43  */
  NONE, NORMAL, (LONG)"T - Execute *.TOS",
  0x0034, 0x0010, 0x0011, 0x0001,
        45,       -1,       -1, G_STRING          ,   /* Object 44  */
  NONE, NORMAL, (LONG)"C - Copy",
  0x0007, 0x0011, 0x0008, 0x0001,
        46,       -1,       -1, G_STRING          ,   /* Object 45  */
  NONE, NORMAL, (LONG)"S - Ship",
  0x001C, 0x0011, 0x0008, 0x0001,
        47,       -1,       -1, G_STRING          ,   /* Object 46  */
  NONE, NORMAL, (LONG)"U - Execute *.APP",
  0x0034, 0x0011, 0x0011, 0x0001,
        48,       -1,       -1, G_STRING          ,   /* Object 47  */
  NONE, NORMAL, (LONG)"D - Delete",
  0x0007, 0x0012, 0x000A, 0x0001,
        49,       -1,       -1, G_STRING          ,   /* Object 48  */
  NONE, NORMAL, (LONG)"T - Terminal",
  0x001C, 0x0012, 0x000C, 0x0001,
        50,       -1,       -1, G_STRING          ,   /* Object 49  */
  NONE, NORMAL, (LONG)"V - Execute *.*",
  0x0034, 0x0012, 0x000F, 0x0001,
        51,       -1,       -1, G_STRING          ,   /* Object 50  */
  NONE, NORMAL, (LONG)"I - Info",
  0x0007, 0x0013, 0x0008, 0x0001,
        52,       -1,       -1, G_STRING          ,   /* Object 51  */
  NONE, NORMAL, (LONG)"V - View",
  0x001C, 0x0013, 0x0008, 0x0001,
        53,       -1,       -1, G_STRING          ,   /* Object 52  */
  NONE, NORMAL, (LONG)"X - Terminal (int.)",
  0x0034, 0x0013, 0x0013, 0x0001,
        54,       -1,       -1, G_STRING          ,   /* Object 53  */
  NONE, NORMAL, (LONG)"M - Move",
  0x0007, 0x0014, 0x0008, 0x0001,
        55,       -1,       -1, G_STRING          ,   /* Object 54  */
  NONE, NORMAL, (LONG)"Y - SystemInfo",
  0x001C, 0x0014, 0x000E, 0x0001,
        56,       -1,       -1, G_STRING          ,   /* Object 55  */
  NONE, NORMAL, (LONG)"Z - Execute *.TTP",
  0x0034, 0x0014, 0x0011, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 56  */
  LASTOB, NORMAL, (LONG)"R - Rename",
  0x0007, 0x0015, 0x000A, 0x0001,
  
  /******** Tree 10 DIALOG12 ****************************************************/
        -1,        1,        9, G_IBOX            ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0000, 0x0050, 0x0019,
         9,        2,        2, G_BOX             ,   /* Object 1  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0000, 0x0000, 0x0050, 0x0201,
         1,        3,        8, G_IBOX            ,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0002, 0x0000, 0x0030, 0x0301,
         4,       -1,       -1, G_TITLE           ,   /* Object 3  */
  NONE, NORMAL, (LONG)" AVALON",
  0x0000, 0x0000, 0x0008, 0x0301,
         5,       -1,       -1, G_TITLE           ,   /* Object 4  */
  NONE, NORMAL, (LONG)" Poll",
  0x0008, 0x0000, 0x0006, 0x0301,
         6,       -1,       -1, G_TITLE           ,   /* Object 5  */
  NONE, NORMAL, (LONG)" Program",
  0x000E, 0x0000, 0x0009, 0x0301,
         7,       -1,       -1, G_TITLE           ,   /* Object 6  */
  NONE, NORMAL, (LONG)" Utility",
  0x0017, 0x0000, 0x0009, 0x0301,
         8,       -1,       -1, G_TITLE           ,   /* Object 7  */
  NONE, NORMAL, (LONG)" Extras",
  0x0020, 0x0000, 0x0008, 0x0301,
         2,       -1,       -1, G_TITLE           ,   /* Object 8  */
  NONE, NORMAL, (LONG)" Config",
  0x0028, 0x0000, 0x0008, 0x0301,
         0,       10,       74, G_IBOX            ,   /* Object 9  */
  NONE, NORMAL, (LONG)0x00000000L,
  0x0000, 0x0301, 0x0050, 0x0017,
        19, MINFO           ,       18, G_BOX             ,   /* Object 10  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0002, 0x0000, 0x0016, 0x0008,
        12,       -1,       -1, G_STRING          ,   /* Object 11 MINFO */
  NONE, NORMAL, (LONG)"  About AVALON     \007I",
  0x0000, 0x0000, 0x0016, 0x0001,
        13,       -1,       -1, G_STRING          |0x0100,   /* Object 12  */
  NONE, DISABLED, (LONG)"----------------------",
  0x0000, 0x0001, 0x0016, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"  Desk Accessory 1  ",
  0x0000, 0x0002, 0x0016, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"  Desk Accessory 2  ",
  0x0000, 0x0003, 0x0016, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"  Desk Accessory 3  ",
  0x0000, 0x0004, 0x0016, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  NONE, NORMAL, (LONG)"  Desk Accessory 4  ",
  0x0000, 0x0005, 0x0016, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"  Desk Accessory 5  ",
  0x0000, 0x0006, 0x0016, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"  Desk Accessory 6  ",
  0x0000, 0x0007, 0x0016, 0x0001,
        37, MPOLL1          , MQUIT           , G_BOX             ,   /* Object 19  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x000A, 0x0000, 0x0016, 0x0011,
  MPOLL2          ,       -1,       -1, G_STRING          ,   /* Object 20 MPOLL1 */
  NONE, NORMAL, (LONG)"  PollSession 1     1",
  0x0000, 0x0000, 0x0016, 0x0001,
  MPOLL3          ,       -1,       -1, G_STRING          ,   /* Object 21 MPOLL2 */
  NONE, NORMAL, (LONG)"  PollSession 2     2",
  0x0000, 0x0001, 0x0016, 0x0001,
  MPOLL4          ,       -1,       -1, G_STRING          ,   /* Object 22 MPOLL3 */
  NONE, NORMAL, (LONG)"  PollSession 3     3",
  0x0000, 0x0002, 0x0016, 0x0001,
  MPOLL5          ,       -1,       -1, G_STRING          ,   /* Object 23 MPOLL4 */
  NONE, NORMAL, (LONG)"  PollSession 4     4",
  0x0000, 0x0003, 0x0016, 0x0001,
  MPOLL6          ,       -1,       -1, G_STRING          ,   /* Object 24 MPOLL5 */
  NONE, NORMAL, (LONG)"  PollSession 5     5",
  0x0000, 0x0004, 0x0016, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25 MPOLL6 */
  NONE, NORMAL, (LONG)"  PollSession 6     6",
  0x0000, 0x0005, 0x0016, 0x0001,
  MAUTO1          ,       -1,       -1, G_STRING          |0x0100,   /* Object 26  */
  NONE, DISABLED, (LONG)"----------------------",
  0x0000, 0x0006, 0x0016, 0x0001,
  MAUTO2          ,       -1,       -1, G_STRING          ,   /* Object 27 MAUTO1 */
  NONE, NORMAL, (LONG)"  AutoPoll 1       \0071",
  0x0000, 0x0007, 0x0016, 0x0001,
  MAUTO3          ,       -1,       -1, G_STRING          ,   /* Object 28 MAUTO2 */
  NONE, NORMAL, (LONG)"  AutoPoll 2       \0072",
  0x0000, 0x0008, 0x0016, 0x0001,
  MAUTO4          ,       -1,       -1, G_STRING          ,   /* Object 29 MAUTO3 */
  NONE, NORMAL, (LONG)"  AutoPoll 3       \0073",
  0x0000, 0x0009, 0x0016, 0x0001,
  MAUTO5          ,       -1,       -1, G_STRING          ,   /* Object 30 MAUTO4 */
  NONE, NORMAL, (LONG)"  AutoPoll 4       \0074",
  0x0000, 0x000A, 0x0016, 0x0001,
  MAUTO6          ,       -1,       -1, G_STRING          ,   /* Object 31 MAUTO5 */
  NONE, NORMAL, (LONG)"  AutoPoll 5       \0075",
  0x0000, 0x000B, 0x0016, 0x0001,
        33,       -1,       -1, G_STRING          ,   /* Object 32 MAUTO6 */
  NONE, NORMAL, (LONG)"  AutoPoll 6       \0076",
  0x0000, 0x000C, 0x0016, 0x0001,
  MRESTART        ,       -1,       -1, G_STRING          |0x0100,   /* Object 33  */
  NONE, DISABLED, (LONG)"----------------------",
  0x0000, 0x000D, 0x0016, 0x0001,
        35,       -1,       -1, G_STRING          ,   /* Object 34 MRESTART */
  NONE, NORMAL, (LONG)"  Poll selected     0",
  0x0000, 0x000E, 0x0016, 0x0001,
  MQUIT           ,       -1,       -1, G_STRING          |0x0100,   /* Object 35  */
  NONE, DISABLED, (LONG)"----------------------",
  0x0000, 0x000F, 0x0016, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 36 MQUIT */
  NONE, NORMAL, (LONG)"  Quit AVALON      ^Q",
  0x0000, 0x0010, 0x0016, 0x0001,
        55, MIMPORT         , MSTAR           , G_BOX             ,   /* Object 37  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0010, 0x0000, 0x0012, 0x0011,
  MEXPORT         ,       -1,       -1, G_STRING          ,   /* Object 38 MIMPORT */
  NONE, NORMAL, (LONG)"  Import       ^I",
  0x0000, 0x0000, 0x0012, 0x0001,
  MCRUNCH         ,       -1,       -1, G_STRING          ,   /* Object 39 MEXPORT */
  NONE, NORMAL, (LONG)"  Export       ^E",
  0x0000, 0x0001, 0x0012, 0x0001,
  MSCANNER        ,       -1,       -1, G_STRING          ,   /* Object 40 MCRUNCH */
  NONE, NORMAL, (LONG)"  Cruncher     ^C",
  0x0000, 0x0002, 0x0012, 0x0001,
        42,       -1,       -1, G_STRING          ,   /* Object 41 MSCANNER */
  NONE, NORMAL, (LONG)"  Scanner      ^S",
  0x0000, 0x0003, 0x0012, 0x0001,
  MMAILER         ,       -1,       -1, G_STRING          |0x0100,   /* Object 42  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0004, 0x0012, 0x0001,
  MTERM           ,       -1,       -1, G_STRING          ,   /* Object 43 MMAILER */
  NONE, NORMAL, (LONG)"  Mailer       ^B",
  0x0000, 0x0005, 0x0012, 0x0001,
        45,       -1,       -1, G_STRING          ,   /* Object 44 MTERM */
  NONE, NORMAL, (LONG)"  Terminal     ^X",
  0x0000, 0x0006, 0x0012, 0x0001,
  MMSGEDIT        ,       -1,       -1, G_STRING          |0x0100,   /* Object 45  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x0007, 0x0012, 0x0001,
  MTXTEDIT        ,       -1,       -1, G_STRING          ,   /* Object 46 MMSGEDIT */
  NONE, NORMAL, (LONG)"  MsgEditor    ^M",
  0x0000, 0x0008, 0x0012, 0x0001,
        48,       -1,       -1, G_STRING          ,   /* Object 47 MTXTEDIT */
  NONE, NORMAL, (LONG)"  TxtEditor    ^T",
  0x0000, 0x0009, 0x0012, 0x0001,
  MAPP            ,       -1,       -1, G_STRING          |0x0100,   /* Object 48  */
  NONE, DISABLED, (LONG)"------------------",
  0x0000, 0x000A, 0x0012, 0x0001,
  MPRG            ,       -1,       -1, G_STRING          ,   /* Object 49 MAPP */
  NONE, NORMAL, (LONG)"  *.APP        \007U",
  0x0000, 0x000B, 0x0012, 0x0001,
  MTOS            ,       -1,       -1, G_STRING          ,   /* Object 50 MPRG */
  NONE, NORMAL, (LONG)"  *.PRG        \007G",
  0x0000, 0x000C, 0x0012, 0x0001,
  MTTP            ,       -1,       -1, G_STRING          ,   /* Object 51 MTOS */
  NONE, NORMAL, (LONG)"  *.TOS        \007T",
  0x0000, 0x000D, 0x0012, 0x0001,
  MALL            ,       -1,       -1, G_STRING          ,   /* Object 52 MTTP */
  NONE, NORMAL, (LONG)"  *.TTP        \007Z",
  0x0000, 0x000E, 0x0012, 0x0001,
  MSTAR           ,       -1,       -1, G_STRING          ,   /* Object 53 MALL */
  NONE, NORMAL, (LONG)"  *.BAT        \007B",
  0x0000, 0x000F, 0x0012, 0x0001,
        37,       -1,       -1, G_STRING          ,   /* Object 54 MSTAR */
  NONE, NORMAL, (LONG)"  *.*          \007V",
  0x0000, 0x0010, 0x0012, 0x0001,
        62, MUTIL1          , MUTIL6          , G_BOX             ,   /* Object 55  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0019, 0x0000, 0x0012, 0x0006,
  MUTIL2          ,       -1,       -1, G_STRING          ,   /* Object 56 MUTIL1 */
  NONE, NORMAL, (LONG)"  Utility 1    ^1",
  0x0000, 0x0000, 0x0012, 0x0001,
  MUTIL3          ,       -1,       -1, G_STRING          ,   /* Object 57 MUTIL2 */
  NONE, NORMAL, (LONG)"  Utility 2    ^2",
  0x0000, 0x0001, 0x0012, 0x0001,
  MUTIL4          ,       -1,       -1, G_STRING          ,   /* Object 58 MUTIL3 */
  NONE, NORMAL, (LONG)"  Utility 3    ^3",
  0x0000, 0x0002, 0x0012, 0x0001,
  MUTIL5          ,       -1,       -1, G_STRING          ,   /* Object 59 MUTIL4 */
  NONE, NORMAL, (LONG)"  Utility 4    ^4",
  0x0000, 0x0003, 0x0012, 0x0001,
  MUTIL6          ,       -1,       -1, G_STRING          ,   /* Object 60 MUTIL5 */
  NONE, NORMAL, (LONG)"  Utility 5    ^5",
  0x0000, 0x0004, 0x0012, 0x0001,
        55,       -1,       -1, G_STRING          ,   /* Object 61 MUTIL6 */
  NONE, NORMAL, (LONG)"  Utility 6    ^6",
  0x0000, 0x0005, 0x0012, 0x0001,
        74, MFILEREQ        , MNOTE           , G_BOX             ,   /* Object 62  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x0022, 0x0000, 0x0015, 0x000B,
        64,       -1,       -1, G_STRING          ,   /* Object 63 MFILEREQ */
  NONE, NORMAL, (LONG)"  Request file    \007R ",
  0x0000, 0x0000, 0x0015, 0x0001,
  MHATCH          ,       -1,       -1, G_STRING          |0x0100,   /* Object 64  */
  NONE, DISABLED, (LONG)"---------------------",
  0x0000, 0x0001, 0x0015, 0x0001,
        66,       -1,       -1, G_STRING          ,   /* Object 65 MHATCH */
  NONE, NORMAL, (LONG)"  Hatch file      \007E ",
  0x0000, 0x0002, 0x0015, 0x0001,
  MFIX            ,       -1,       -1, G_STRING          |0x0100,   /* Object 66  */
  NONE, DISABLED, (LONG)"---------------------",
  0x0000, 0x0003, 0x0015, 0x0001,
        68,       -1,       -1, G_STRING          ,   /* Object 67 MFIX */
  NONE, NORMAL, (LONG)"  AreaFix         \007M ",
  0x0000, 0x0004, 0x0015, 0x0001,
  MMANAGER        ,       -1,       -1, G_STRING          |0x0100,   /* Object 68  */
  NONE, DISABLED, (LONG)"---------------------",
  0x0000, 0x0005, 0x0015, 0x0001,
  MFTERM          ,       -1,       -1, G_STRING          ,   /* Object 69 MMANAGER */
  NONE, NORMAL, (LONG)"  Filemanager     ^F ",
  0x0000, 0x0006, 0x0015, 0x0001,
  MPMANAGE        ,       -1,       -1, G_STRING          ,   /* Object 70 MFTERM */
  NONE, NORMAL, (LONG)"  ...Terminal     \007X ",
  0x0000, 0x0007, 0x0015, 0x0001,
        72,       -1,       -1, G_STRING          ,   /* Object 71 MPMANAGE */
  NONE, NORMAL, (LONG)"  Programmanager  ^P ",
  0x0000, 0x0008, 0x0015, 0x0001,
  MNOTE           ,       -1,       -1, G_STRING          |0x0100,   /* Object 72  */
  NONE, DISABLED, (LONG)"---------------------",
  0x0000, 0x0009, 0x0015, 0x0001,
        62,       -1,       -1, G_STRING          ,   /* Object 73 MNOTE */
  NONE, NORMAL, (LONG)"  Notebook      UNDO ",
  0x0000, 0x000A, 0x0015, 0x0001,
         9, MSSETUP         , MSAVE           , G_BOX             ,   /* Object 74  */
  NONE, NORMAL, (LONG)0x00FF1000L,
  0x002A, 0x0000, 0x0017, 0x0010,
  MFINDPRG        ,       -1,       -1, G_STRING          ,   /* Object 75 MSSETUP */
  NONE, NORMAL, (LONG)"  SessionSetup      \007O ",
  0x0000, 0x0000, 0x0017, 0x0001,
  MFINDUTL        ,       -1,       -1, G_STRING          ,   /* Object 76 MFINDPRG */
  NONE, NORMAL, (LONG)"  Find 1 (Program)  \007F ",
  0x0000, 0x0001, 0x0017, 0x0001,
  MAREAS          ,       -1,       -1, G_STRING          ,   /* Object 77 MFINDUTL */
  NONE, NORMAL, (LONG)"  Find 2 (Utility)  \007H ",
  0x0000, 0x0002, 0x0017, 0x0001,
  MSPEZIAL        ,       -1,       -1, G_STRING          ,   /* Object 78 MAREAS */
  NONE, NORMAL, (LONG)"  Edit areas        \007A ",
  0x0000, 0x0003, 0x0017, 0x0001,
        80,       -1,       -1, G_STRING          ,   /* Object 79 MSPEZIAL */
  NONE, NORMAL, (LONG)"  Special           \007P ",
  0x0000, 0x0004, 0x0017, 0x0001,
  MPOINT1         ,       -1,       -1, G_STRING          |0x0100,   /* Object 80  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x0005, 0x0017, 0x0001,
  MPOINT2         ,       -1,       -1, G_STRING          ,   /* Object 81 MPOINT1 */
  NONE, NORMAL, (LONG)"  Main Setup        F1 ",
  0x0000, 0x0006, 0x0017, 0x0001,
  MPOINT3         ,       -1,       -1, G_STRING          ,   /* Object 82 MPOINT2 */
  NONE, NORMAL, (LONG)"   1st Setup        F2 ",
  0x0000, 0x0007, 0x0017, 0x0001,
  MPOINT4         ,       -1,       -1, G_STRING          ,   /* Object 83 MPOINT3 */
  NONE, NORMAL, (LONG)"   2nd Setup        F3 ",
  0x0000, 0x0008, 0x0017, 0x0001,
  MPOINT5         ,       -1,       -1, G_STRING          ,   /* Object 84 MPOINT4 */
  NONE, NORMAL, (LONG)"   3rd Setup        F4 ",
  0x0000, 0x0009, 0x0017, 0x0001,
  MPOINT6         ,       -1,       -1, G_STRING          ,   /* Object 85 MPOINT5 */
  NONE, NORMAL, (LONG)"   4th Setup        F5 ",
  0x0000, 0x000A, 0x0017, 0x0001,
        87,       -1,       -1, G_STRING          ,   /* Object 86 MPOINT6 */
  NONE, NORMAL, (LONG)"   5th Setup        F6 ",
  0x0000, 0x000B, 0x0017, 0x0001,
  MREREAD         ,       -1,       -1, G_STRING          |0x0100,   /* Object 87  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x000C, 0x0017, 0x0001,
        89,       -1,       -1, G_STRING          ,   /* Object 88 MREREAD */
  NONE, NORMAL, (LONG)"  Redraw infobox   F10 ",
  0x0000, 0x000D, 0x0017, 0x0001,
  MSAVE           ,       -1,       -1, G_STRING          |0x0100,   /* Object 89  */
  NONE, DISABLED, (LONG)"-----------------------",
  0x0000, 0x000E, 0x0017, 0x0001,
        74,       -1,       -1, G_STRING          ,   /* Object 90 MSAVE */
  LASTOB, NORMAL, (LONG)"  Save config       \007S ",
  0x0000, 0x000F, 0x0017, 0x0001,
  
  /******** Tree 11 DIALOG13 ****************************************************/
        -1, PRESSKEY        ,        2, G_BOX             ,   /* Object 0  */
  NONE, OUTLINED|SHADOWED, (LONG)0x00FD1152L,
  0x0000, 0x0000, 0x000B, 0x0004,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1 PRESSKEY */
  NONE, OUTLINED|SHADOWED, (LONG)&rs_tedinfo[276],
  0x0002, 0x0001, 0x0007, 0x0002,
         0,       -1,       -1, G_TEXT            ,   /* Object 2  */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[277],
  0x0000, 0x0003, 0x000B, 0x0001,
  
  /******** Tree 12 DIALOG14 ****************************************************/
        -1, FINDTEXT        , FINDOK          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x003D, 0x0003,
  FINDABB         ,       -1,       -1, G_FTEXT           ,   /* Object 1 FINDTEXT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[278],
  0x0002, 0x0001, 0x0024, 0x0001,
  FINDOK          ,       -1,       -1, G_BUTTON          ,   /* Object 2 FINDABB */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0029, 0x0001, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 3 FINDOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Find",
  0x0033, 0x0001, 0x0008, 0x0001,
  
  /******** Tree 13 DIALOG15 ****************************************************/
        -1,        1, POK             , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0016,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[279],
  0x0000, 0x0000, 0x004F, 0x0001,
  PAREAS          ,       -1,       -1, G_TEXT            ,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[280],
  0x0011, 0x0001, 0x002F, 0x0001,
  PBINKLEY        ,       -1,       -1, G_FTEXT           ,   /* Object 3 PAREAS */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[281],
  0x0005, 0x0002, 0x003E, 0x0001,
         5,       -1,       -1, G_FTEXT           ,   /* Object 4 PBINKLEY */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[282],
  0x0003, 0x0003, 0x0040, 0x0001,
  PSEMPER         ,       -1,       -1, G_TEXT            ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_tedinfo[283],
  0x0044, 0x0003, 0x0407, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 PSEMPER */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[284],
  0x0004, 0x0004, 0x003F, 0x0001,
         8,       -1,       -1, G_TEXT            ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_tedinfo[285],
  0x0044, 0x0004, 0x0606, 0x0001,
  PNETMAIL        ,       -1,       -1, G_TEXT            ,   /* Object 8  */
  NONE, NORMAL, (LONG)&rs_tedinfo[286],
  0x0011, 0x0005, 0x062D, 0x0001,
        10,       -1,       -1, G_FTEXT           ,   /* Object 9 PNETMAIL */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[287],
  0x0007, 0x0006, 0x003C, 0x0001,
        11,       -1,       -1, G_IBOX            |0x0200,   /* Object 10  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0007, 0x004F, 0x0001,
  NAMSET1         ,       -1,       -1, G_TEXT            ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[288],
  0x0004, 0x0008, 0x0009, 0x0001,
        13,       -1,       -1, G_FTEXT           ,   /* Object 12 NAMSET1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[289],
  0x000D, 0x0008, 0x000B, 0x0001,
  NAMSET2         ,       -1,       -1, G_TEXT            ,   /* Object 13  */
  NONE, NORMAL, (LONG)&rs_tedinfo[290],
  0x001B, 0x0008, 0x0008, 0x0001,
        15,       -1,       -1, G_FTEXT           ,   /* Object 14 NAMSET2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[291],
  0x0023, 0x0008, 0x000B, 0x0001,
  NAMSET3         ,       -1,       -1, G_TEXT            ,   /* Object 15  */
  NONE, NORMAL, (LONG)&rs_tedinfo[292],
  0x0031, 0x0008, 0x0008, 0x0001,
        17,       -1,       -1, G_FTEXT           ,   /* Object 16 NAMSET3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[293],
  0x0039, 0x0008, 0x000B, 0x0001,
  NAMSET4         ,       -1,       -1, G_TEXT            ,   /* Object 17  */
  NONE, NORMAL, (LONG)&rs_tedinfo[294],
  0x0004, 0x0009, 0x0009, 0x0001,
        19,       -1,       -1, G_FTEXT           ,   /* Object 18 NAMSET4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[295],
  0x000D, 0x0009, 0x000B, 0x0001,
  NAMSET5         ,       -1,       -1, G_TEXT            ,   /* Object 19  */
  NONE, NORMAL, (LONG)&rs_tedinfo[296],
  0x001B, 0x0009, 0x0008, 0x0001,
        21,       -1,       -1, G_FTEXT           ,   /* Object 20 NAMSET5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[297],
  0x0023, 0x0009, 0x000B, 0x0001,
  NAMSET6         ,       -1,       -1, G_TEXT            ,   /* Object 21  */
  NONE, NORMAL, (LONG)&rs_tedinfo[298],
  0x0031, 0x0009, 0x0008, 0x0001,
        23,       -1,       -1, G_FTEXT           ,   /* Object 22 NAMSET6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[299],
  0x0039, 0x0009, 0x000B, 0x0001,
        24,       -1,       -1, G_IBOX            |0x0200,   /* Object 23  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x000A, 0x004F, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  NONE, NORMAL, (LONG)"Infobox:",
  0x0003, 0x000B, 0x0008, 0x0001,
        29, USE000          , USE999          , G_IBOX            ,   /* Object 25  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x000C, 0x000B, 0x0015, 0x0001,
  USE241          ,       -1,       -1, G_BUTTON          ,   /* Object 26 USE000 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"None",
  0x0000, 0x0000, 0x0006, 0x0001,
  USE999          ,       -1,       -1, G_BUTTON          ,   /* Object 27 USE241 */
  SELECTABLE|RBUTTON, NORMAL, (LONG)"Normal",
  0x0007, 0x0000, 0x0006, 0x0001,
        25,       -1,       -1, G_BUTTON          ,   /* Object 28 USE999 */
  SELECTABLE|RBUTTON, SELECTED, (LONG)"Max",
  0x000E, 0x0000, 0x0006, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  NONE, NORMAL, (LONG)"K/Sent:",
  0x0023, 0x000B, 0x0007, 0x0001,
  USEDSIZE        , KSHATCH         , KSFIX           , G_IBOX            ,   /* Object 30  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x002B, 0x000B, 0x0012, 0x0001,
  KSFIX           ,       -1,       -1, G_BUTTON          ,   /* Object 31 KSHATCH */
  SELECTABLE, NORMAL, (LONG)"F\047Hatch",
  0x0000, 0x0000, 0x0008, 0x0001,
        30,       -1,       -1, G_BUTTON          ,   /* Object 32 KSFIX */
  SELECTABLE, NORMAL, (LONG)"Areafix",
  0x0009, 0x0000, 0x0008, 0x0001,
  CHECKON         ,       -1,       -1, G_FTEXT           ,   /* Object 33 USEDSIZE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[300],
  0x003F, 0x000B, 0x000C, 0x0001,
        35,       -1,       -1, G_BOX             |0x0300,   /* Object 34 CHECKON */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x000D, 0x0002, 0x0001,
  USEMAXI         ,       -1,       -1, G_STRING          ,   /* Object 35  */
  NONE, NORMAL, (LONG)"MsgBaseChecking (\042Edit areas\042)",
  0x0006, 0x000D, 0x001E, 0x0001,
        37,       -1,       -1, G_BOX             |0x0300,   /* Object 36 USEMAXI */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x000D, 0x0002, 0x0001,
  KEYPON          ,       -1,       -1, G_STRING          ,   /* Object 37  */
  NONE, NORMAL, (LONG)"Big windows (vert.>=672 points)",
  0x002B, 0x000D, 0x001F, 0x0001,
        39,       -1,       -1, G_BOX             |0x0300,   /* Object 38 KEYPON */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x000E, 0x0002, 0x0001,
  USECENTR        ,       -1,       -1, G_STRING          ,   /* Object 39  */
  NONE, NORMAL, (LONG)"Wait for keypress (TOS & TTP)",
  0x0006, 0x000E, 0x001D, 0x0001,
        41,       -1,       -1, G_BOX             |0x0300,   /* Object 40 USECENTR */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x000E, 0x0002, 0x0001,
  REDIRECT        ,       -1,       -1, G_STRING          ,   /* Object 41  */
  NONE, NORMAL, (LONG)"Always center textwindows",
  0x002B, 0x000E, 0x0019, 0x0001,
        43,       -1,       -1, G_BOX             |0x0300,   /* Object 42 REDIRECT */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x000F, 0x0002, 0x0001,
  USESNAP         ,       -1,       -1, G_STRING          ,   /* Object 43  */
  NONE, NORMAL, (LONG)"Redirect output (direct start)",
  0x0006, 0x000F, 0x001E, 0x0001,
        45,       -1,       -1, G_BOX             |0x0300,   /* Object 44 USESNAP */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x000F, 0x0002, 0x0001,
  USE4DON         ,       -1,       -1, G_STRING          ,   /* Object 45  */
  NONE, NORMAL, (LONG)"Snap textwindows",
  0x002B, 0x000F, 0x0010, 0x0001,
        47,       -1,       -1, G_BOX             |0x0300,   /* Object 46 USE4DON */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x0010, 0x0002, 0x0001,
  USENOLNA        ,       -1,       -1, G_STRING          ,   /* Object 47  */
  NONE, NORMAL, (LONG)"4D-Mode (Binkley>=V3.0)",
  0x0006, 0x0010, 0x0017, 0x0001,
        49,       -1,       -1, G_BOX             |0x0300,   /* Object 48 USENOLNA */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x0010, 0x0002, 0x0001,
  USEDELAY        ,       -1,       -1, G_STRING          ,   /* Object 49  */
  NONE, NORMAL, (LONG)"No Line-A (TOS redirect)",
  0x002B, 0x0010, 0x0018, 0x0001,
        51,       -1,       -1, G_BOX             |0x0300,   /* Object 50 USEDELAY */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x0011, 0x0002, 0x0001,
  USESMART        ,       -1,       -1, G_STRING          ,   /* Object 51  */
  NONE, NORMAL, (LONG)"Delay AutoStartPoll",
  0x0006, 0x0011, 0x0013, 0x0001,
        53,       -1,       -1, G_BOX             |0x0300,   /* Object 52 USESMART */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x0011, 0x0002, 0x0001,
  USEDBBS         ,       -1,       -1, G_STRING          ,   /* Object 53  */
  NONE, NORMAL, (LONG)"Smart redraw",
  0x002B, 0x0011, 0x000C, 0x0001,
        55,       -1,       -1, G_BOX             |0x0300,   /* Object 54 USEDBBS */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x0012, 0x0002, 0x0001,
  BOXON           ,       -1,       -1, G_STRING          ,   /* Object 55  */
  NONE, NORMAL, (LONG)"Ask for AREAS.BBS/DAREAS.BBS",
  0x0006, 0x0012, 0x001C, 0x0001,
        57,       -1,       -1, G_BOX             |0x0300,   /* Object 56 BOXON */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x0012, 0x0002, 0x0001,
  BOXSMALL        ,       -1,       -1, G_STRING          ,   /* Object 57  */
  NONE, NORMAL, (LONG)"Main-Dialogbox (",
  0x002B, 0x0012, 0x0010, 0x0001,
        59,       -1,       -1, G_BOX             |0x0300,   /* Object 58 BOXSMALL */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x003B, 0x0012, 0x0002, 0x0001,
  SAVEON          ,       -1,       -1, G_STRING          ,   /* Object 59  */
  NONE, NORMAL, (LONG)"no icon)",
  0x003E, 0x0012, 0x0008, 0x0001,
        61,       -1,       -1, G_BOX             |0x0300,   /* Object 60 SAVEON */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x0013, 0x0002, 0x0001,
  USEBUFF         ,       -1,       -1, G_STRING          ,   /* Object 61  */
  NONE, NORMAL, (LONG)"Save dialogpositions",
  0x0006, 0x0013, 0x0014, 0x0001,
        63,       -1,       -1, G_BOX             |0x0300,   /* Object 62 USEBUFF */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x0013, 0x0002, 0x0001,
  USE3DON         ,       -1,       -1, G_STRING          ,   /* Object 63  */
  NONE, NORMAL, (LONG)"Buffer screen output",
  0x002B, 0x0013, 0x0014, 0x0001,
        65,       -1,       -1, G_BOX             |0x0300,   /* Object 64 USE3DON */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0003, 0x0014, 0x0002, 0x0001,
  USEOUT36        ,       -1,       -1, G_STRING          ,   /* Object 65  */
  NONE, NORMAL, (LONG)"Send 3D-TICs",
  0x0006, 0x0014, 0x000C, 0x0001,
        67,       -1,       -1, G_BOX             |0x0300,   /* Object 66 USEOUT36 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0028, 0x0014, 0x0002, 0x0001,
  POK             ,       -1,       -1, G_STRING          ,   /* Object 67  */
  NONE, NORMAL, (LONG)"Base36 outbound style",
  0x002B, 0x0014, 0x0015, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 68 POK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0044, 0x0014, 0x0009, 0x0001,
  
  /******** Tree 14 DIALOG16 ****************************************************/
        -1,        1, MANVIEW         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED|SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x000E, 0x000E,
  MANSHIP         ,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, SELECTED, (LONG)&rs_tedinfo[301],
  0x0000, 0x0000, 0x000E, 0x0001,
  MANTERM         ,       -1,       -1, G_BOXTEXT         ,   /* Object 2 MANSHIP */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[302],
  0x0000, 0x0001, 0x000E, 0x0001,
         4,       -1,       -1, G_BOXTEXT         ,   /* Object 3 MANTERM */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[303],
  0x0000, 0x0002, 0x000E, 0x0001,
  MANSYS          ,       -1,       -1, G_BOXTEXT         ,   /* Object 4  */
  NONE, SELECTED, (LONG)&rs_tedinfo[304],
  0x0000, 0x0003, 0x000E, 0x0001,
  MANOUT          ,       -1,       -1, G_BOXTEXT         ,   /* Object 5 MANSYS */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[305],
  0x0000, 0x0004, 0x000E, 0x0001,
  MANMSGS         ,       -1,       -1, G_BOXTEXT         ,   /* Object 6 MANOUT */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[306],
  0x0000, 0x0005, 0x000E, 0x0001,
  MANREF          ,       -1,       -1, G_BOXTEXT         ,   /* Object 7 MANMSGS */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[307],
  0x0000, 0x0006, 0x000E, 0x0001,
  MANCOPY         ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 MANREF */
  NONE, SELECTED, (LONG)&rs_tedinfo[308],
  0x0000, 0x0007, 0x000E, 0x0001,
  MANMOVE         ,       -1,       -1, G_BOXTEXT         ,   /* Object 9 MANCOPY */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[309],
  0x0000, 0x0008, 0x000E, 0x0001,
  MANDEL          ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 MANMOVE */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[310],
  0x0000, 0x0009, 0x000E, 0x0001,
  MANREN          ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 MANDEL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[311],
  0x0000, 0x000A, 0x000E, 0x0001,
  MANINFO         ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 MANREN */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[312],
  0x0000, 0x000B, 0x000E, 0x0001,
  MANVIEW         ,       -1,       -1, G_BOXTEXT         ,   /* Object 13 MANINFO */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[313],
  0x0000, 0x000C, 0x000E, 0x0001,
         0,       -1,       -1, G_BOXTEXT         ,   /* Object 14 MANVIEW */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)&rs_tedinfo[314],
  0x0000, 0x000D, 0x000E, 0x0001,
  
  /******** Tree 15 DIALOG17 ****************************************************/
        -1, IMANAG1         , IMANAG2         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004A, 0x0004,
  IMANAG2         ,       -1,       -1, G_STRING          ,   /* Object 1 IMANAG1 */
  NONE, NORMAL, (LONG)"1234567890123456789012345678901234567890123456789012345678901234567890",
  0x0002, 0x0001, 0x0046, 0x0001,
         0,       -1,       -1, G_STRING          ,   /* Object 2 IMANAG2 */
  LASTOB, NORMAL, (LONG)"1234567890123456789012345678901234567890123456789012345678901234567890",
  0x0002, 0x0002, 0x0046, 0x0001,
  
  /******** Tree 16 DIALOG18 ****************************************************/
        -1,        1, NOTEEXIT        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021123L,
  0x0000, 0x0000, 0x0044, 0x0014,
  NOTEBOX         ,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[315],
  0x0002, 0x0001, 0x0040, 0x0001,
  NOTECLR         , NOTE1           , NOTEC           , G_BOX             ,   /* Object 2 NOTEBOX */
  NONE, SHADOWED, (LONG)0x00FF1101L,
  0x0002, 0x0003, 0x0040, 0x000E,
  NOTE2           ,       -1,       -1, G_FTEXT           ,   /* Object 3 NOTE1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[316],
  0x0002, 0x0001, 0x003C, 0x0001,
  NOTE3           ,       -1,       -1, G_FTEXT           ,   /* Object 4 NOTE2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[317],
  0x0002, 0x0002, 0x003C, 0x0001,
  NOTE4           ,       -1,       -1, G_FTEXT           ,   /* Object 5 NOTE3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[318],
  0x0002, 0x0003, 0x003C, 0x0001,
  NOTE5           ,       -1,       -1, G_FTEXT           ,   /* Object 6 NOTE4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[319],
  0x0002, 0x0004, 0x003C, 0x0001,
  NOTE6           ,       -1,       -1, G_FTEXT           ,   /* Object 7 NOTE5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[320],
  0x0002, 0x0005, 0x003C, 0x0001,
  NOTE7           ,       -1,       -1, G_FTEXT           ,   /* Object 8 NOTE6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[321],
  0x0002, 0x0006, 0x003C, 0x0001,
  NOTE8           ,       -1,       -1, G_FTEXT           ,   /* Object 9 NOTE7 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[322],
  0x0002, 0x0007, 0x003C, 0x0001,
  NOTE9           ,       -1,       -1, G_FTEXT           ,   /* Object 10 NOTE8 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[323],
  0x0002, 0x0008, 0x003C, 0x0001,
  NOTEA           ,       -1,       -1, G_FTEXT           ,   /* Object 11 NOTE9 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[324],
  0x0002, 0x0009, 0x003C, 0x0001,
  NOTEB           ,       -1,       -1, G_FTEXT           ,   /* Object 12 NOTEA */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[325],
  0x0002, 0x000A, 0x003C, 0x0001,
  NOTEC           ,       -1,       -1, G_FTEXT           ,   /* Object 13 NOTEB */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[326],
  0x0002, 0x000B, 0x003C, 0x0001,
  NOTEBOX         ,       -1,       -1, G_FTEXT           ,   /* Object 14 NOTEC */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[327],
  0x0002, 0x000C, 0x003C, 0x0001,
  NOTELOAD        ,       -1,       -1, G_BUTTON          ,   /* Object 15 NOTECLR */
  SELECTABLE|EXIT, NORMAL, (LONG)"Clear",
  0x0002, 0x0012, 0x0009, 0x0001,
  NOTEFF          ,       -1,       -1, G_BUTTON          ,   /* Object 16 NOTELOAD */
  SELECTABLE|EXIT, NORMAL, (LONG)"Reload",
  0x000C, 0x0012, 0x0009, 0x0001,
  NOTEPRN         ,       -1,       -1, G_BUTTON          ,   /* Object 17 NOTEFF */
  SELECTABLE|EXIT, NORMAL, (LONG)"FormFeed",
  0x0018, 0x0012, 0x0009, 0x0001,
  NOTECANC        ,       -1,       -1, G_BUTTON          ,   /* Object 18 NOTEPRN */
  SELECTABLE|EXIT, NORMAL, (LONG)"Print",
  0x0022, 0x0012, 0x0009, 0x0001,
  NOTEEXIT        ,       -1,       -1, G_BUTTON          ,   /* Object 19 NOTECANC */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x002E, 0x0012, 0x0009, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 20 NOTEEXIT */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Save",
  0x0038, 0x0012, 0x000A, 0x0001,
  
  /******** Tree 17 DIALOG71 ****************************************************/
        -1,        1, EXTENDOK        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004E, 0x0816,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[328],
  0x0002, 0x0001, 0x0018, 0x0001,
         3,       -1,       -1, G_BOXTEXT         ,   /* Object 2  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[329],
  0x001B, 0x0001, 0x0018, 0x0001,
         4,       -1,       -1, G_BOXTEXT         ,   /* Object 3  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[330],
  0x0034, 0x0001, 0x0018, 0x0001,
         5,       -1,       -1, G_TEXT            ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_tedinfo[331],
  0x0003, 0x0802, 0x000C, 0x0001,
         6,       -1,       -1, G_TEXT            ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_tedinfo[332],
  0x001C, 0x0802, 0x0407, 0x0001,
         7,       -1,       -1, G_TEXT            ,   /* Object 6  */
  NONE, NORMAL, (LONG)&rs_tedinfo[333],
  0x0029, 0x0802, 0x0006, 0x0001,
         8,       -1,       -1, G_TEXT            ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_tedinfo[334],
  0x0035, 0x0802, 0x0008, 0x0001,
         9,       -1,       -1, G_TEXT            ,   /* Object 8  */
  NONE, NORMAL, (LONG)&rs_tedinfo[335],
  0x0043, 0x0802, 0x0007, 0x0001,
        10,       -1,       -1, G_TEXT            ,   /* Object 9  */
  NONE, NORMAL, (LONG)&rs_tedinfo[336],
  0x0003, 0x0003, 0x0012, 0x0001,
        11,       -1,       -1, G_TEXT            ,   /* Object 10  */
  NONE, NORMAL, (LONG)&rs_tedinfo[337],
  0x001C, 0x0003, 0x0407, 0x0001,
        12,       -1,       -1, G_TEXT            ,   /* Object 11  */
  NONE, NORMAL, (LONG)&rs_tedinfo[338],
  0x0029, 0x0003, 0x0009, 0x0001,
        13,       -1,       -1, G_TEXT            ,   /* Object 12  */
  NONE, NORMAL, (LONG)&rs_tedinfo[339],
  0x0035, 0x0003, 0x0008, 0x0001,
        14,       -1,       -1, G_TEXT            ,   /* Object 13  */
  NONE, NORMAL, (LONG)&rs_tedinfo[340],
  0x0043, 0x0003, 0x0009, 0x0001,
        15,       -1,       -1, G_TEXT            ,   /* Object 14  */
  NONE, NORMAL, (LONG)&rs_tedinfo[341],
  0x0003, 0x0803, 0x000F, 0x0001,
        16,       -1,       -1, G_TEXT            ,   /* Object 15  */
  NONE, NORMAL, (LONG)&rs_tedinfo[342],
  0x001C, 0x0803, 0x0609, 0x0001,
        17,       -1,       -1, G_TEXT            ,   /* Object 16  */
  NONE, NORMAL, (LONG)&rs_tedinfo[343],
  0x0029, 0x0803, 0x0006, 0x0001,
        18,       -1,       -1, G_TEXT            ,   /* Object 17  */
  NONE, NORMAL, (LONG)&rs_tedinfo[344],
  0x0035, 0x0803, 0x000A, 0x0001,
        19,       -1,       -1, G_TEXT            ,   /* Object 18  */
  NONE, NORMAL, (LONG)&rs_tedinfo[345],
  0x0043, 0x0803, 0x0009, 0x0001,
        20,       -1,       -1, G_TEXT            ,   /* Object 19  */
  NONE, NORMAL, (LONG)&rs_tedinfo[346],
  0x0003, 0x0004, 0x0009, 0x0001,
        21,       -1,       -1, G_TEXT            ,   /* Object 20  */
  NONE, NORMAL, (LONG)&rs_tedinfo[347],
  0x001C, 0x0004, 0x000C, 0x0001,
        22,       -1,       -1, G_TEXT            ,   /* Object 21  */
  NONE, NORMAL, (LONG)&rs_tedinfo[348],
  0x0029, 0x0004, 0x0407, 0x0001,
        23,       -1,       -1, G_TEXT            ,   /* Object 22  */
  NONE, NORMAL, (LONG)&rs_tedinfo[349],
  0x0035, 0x0004, 0x020B, 0x0001,
        24,       -1,       -1, G_TEXT            ,   /* Object 23  */
  NONE, NORMAL, (LONG)&rs_tedinfo[350],
  0x0043, 0x0004, 0x0005, 0x0001,
        25,       -1,       -1, G_TEXT            ,   /* Object 24  */
  NONE, NORMAL, (LONG)&rs_tedinfo[351],
  0x001C, 0x0804, 0x040A, 0x0001,
        26,       -1,       -1, G_TEXT            ,   /* Object 25  */
  NONE, NORMAL, (LONG)&rs_tedinfo[352],
  0x0029, 0x0804, 0x0603, 0x0001,
        27,       -1,       -1, G_TEXT            ,   /* Object 26  */
  NONE, NORMAL, (LONG)&rs_tedinfo[353],
  0x0035, 0x0804, 0x040D, 0x0001,
        28,       -1,       -1, G_TEXT            ,   /* Object 27  */
  NONE, NORMAL, (LONG)&rs_tedinfo[354],
  0x0043, 0x0804, 0x0009, 0x0001,
        29,       -1,       -1, G_BOXTEXT         ,   /* Object 28  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[355],
  0x0002, 0x0005, 0x0018, 0x0001,
        30,       -1,       -1, G_TEXT            ,   /* Object 29  */
  NONE, NORMAL, (LONG)&rs_tedinfo[356],
  0x001C, 0x0005, 0x0609, 0x0001,
        31,       -1,       -1, G_TEXT            ,   /* Object 30  */
  NONE, NORMAL, (LONG)&rs_tedinfo[357],
  0x0029, 0x0005, 0x0606, 0x0001,
        32,       -1,       -1, G_TEXT            ,   /* Object 31  */
  NONE, NORMAL, (LONG)&rs_tedinfo[358],
  0x0035, 0x0005, 0x000B, 0x0001,
        33,       -1,       -1, G_TEXT            ,   /* Object 32  */
  NONE, NORMAL, (LONG)&rs_tedinfo[359],
  0x0043, 0x0005, 0x0005, 0x0001,
        34,       -1,       -1, G_TEXT            ,   /* Object 33  */
  NONE, NORMAL, (LONG)&rs_tedinfo[360],
  0x001C, 0x0805, 0x0009, 0x0001,
        35,       -1,       -1, G_TEXT            ,   /* Object 34  */
  NONE, NORMAL, (LONG)&rs_tedinfo[361],
  0x0029, 0x0805, 0x0407, 0x0001,
        36,       -1,       -1, G_TEXT            ,   /* Object 35  */
  NONE, NORMAL, (LONG)&rs_tedinfo[362],
  0x0035, 0x0805, 0x000C, 0x0001,
        37,       -1,       -1, G_TEXT            ,   /* Object 36  */
  NONE, NORMAL, (LONG)&rs_tedinfo[363],
  0x0043, 0x0805, 0x0007, 0x0001,
        38,       -1,       -1, G_TEXT            ,   /* Object 37  */
  NONE, NORMAL, (LONG)&rs_tedinfo[364],
  0x001C, 0x0006, 0x0205, 0x0001,
        39,       -1,       -1, G_TEXT            ,   /* Object 38  */
  NONE, NORMAL, (LONG)&rs_tedinfo[365],
  0x0029, 0x0006, 0x020B, 0x0001,
        40,       -1,       -1, G_TEXT            ,   /* Object 39  */
  NONE, NORMAL, (LONG)&rs_tedinfo[366],
  0x0035, 0x0006, 0x000C, 0x0001,
        41,       -1,       -1, G_TEXT            ,   /* Object 40  */
  NONE, NORMAL, (LONG)&rs_tedinfo[367],
  0x0043, 0x0006, 0x000A, 0x0001,
        42,       -1,       -1, G_TEXT            ,   /* Object 41  */
  NONE, NORMAL, (LONG)&rs_tedinfo[368],
  0x0003, 0x0806, 0x0609, 0x0001,
        43,       -1,       -1, G_TEXT            ,   /* Object 42  */
  NONE, NORMAL, (LONG)&rs_tedinfo[369],
  0x001C, 0x0806, 0x000F, 0x0001,
        44,       -1,       -1, G_TEXT            ,   /* Object 43  */
  NONE, NORMAL, (LONG)&rs_tedinfo[370],
  0x0035, 0x0806, 0x040D, 0x0001,
        45,       -1,       -1, G_TEXT            ,   /* Object 44  */
  NONE, NORMAL, (LONG)&rs_tedinfo[371],
  0x0043, 0x0806, 0x0606, 0x0001,
        46,       -1,       -1, G_TEXT            ,   /* Object 45  */
  NONE, NORMAL, (LONG)&rs_tedinfo[372],
  0x0003, 0x0007, 0x020E, 0x0001,
        47,       -1,       -1, G_TEXT            ,   /* Object 46  */
  NONE, NORMAL, (LONG)&rs_tedinfo[373],
  0x001C, 0x0007, 0x060C, 0x0001,
        48,       -1,       -1, G_TEXT            ,   /* Object 47  */
  NONE, NORMAL, (LONG)&rs_tedinfo[374],
  0x0035, 0x0007, 0x020B, 0x0001,
        49,       -1,       -1, G_TEXT            ,   /* Object 48  */
  NONE, NORMAL, (LONG)&rs_tedinfo[375],
  0x0043, 0x0007, 0x0407, 0x0001,
        50,       -1,       -1, G_TEXT            ,   /* Object 49  */
  NONE, NORMAL, (LONG)&rs_tedinfo[376],
  0x001C, 0x0807, 0x060C, 0x0001,
        51,       -1,       -1, G_TEXT            ,   /* Object 50  */
  NONE, NORMAL, (LONG)&rs_tedinfo[377],
  0x0035, 0x0807, 0x020B, 0x0001,
        52,       -1,       -1, G_TEXT            ,   /* Object 51  */
  NONE, NORMAL, (LONG)&rs_tedinfo[378],
  0x0043, 0x0807, 0x0009, 0x0001,
        53,       -1,       -1, G_BOXTEXT         ,   /* Object 52  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[379],
  0x0002, 0x0008, 0x0018, 0x0001,
        54,       -1,       -1, G_TEXT            ,   /* Object 53  */
  NONE, NORMAL, (LONG)&rs_tedinfo[380],
  0x001C, 0x0008, 0x060C, 0x0001,
        55,       -1,       -1, G_TEXT            ,   /* Object 54  */
  NONE, NORMAL, (LONG)&rs_tedinfo[381],
  0x0035, 0x0008, 0x040D, 0x0001,
        56,       -1,       -1, G_TEXT            ,   /* Object 55  */
  NONE, NORMAL, (LONG)&rs_tedinfo[382],
  0x0043, 0x0008, 0x0609, 0x0001,
        57,       -1,       -1, G_TEXT            ,   /* Object 56  */
  NONE, NORMAL, (LONG)&rs_tedinfo[383],
  0x0035, 0x0808, 0x0609, 0x0001,
        58,       -1,       -1, G_TEXT            ,   /* Object 57  */
  NONE, NORMAL, (LONG)&rs_tedinfo[384],
  0x0043, 0x0808, 0x0008, 0x0001,
        59,       -1,       -1, G_TEXT            ,   /* Object 58  */
  NONE, NORMAL, (LONG)&rs_tedinfo[385],
  0x0035, 0x0009, 0x0606, 0x0001,
        60,       -1,       -1, G_TEXT            ,   /* Object 59  */
  NONE, NORMAL, (LONG)&rs_tedinfo[386],
  0x0003, 0x0809, 0x000B, 0x0001,
        61,       -1,       -1, G_TEXT            ,   /* Object 60  */
  NONE, NORMAL, (LONG)&rs_tedinfo[387],
  0x0013, 0x0809, 0x0005, 0x0001,
        62,       -1,       -1, G_TEXT            ,   /* Object 61  */
  NONE, NORMAL, (LONG)&rs_tedinfo[388],
  0x0003, 0x000A, 0x000C, 0x0001,
        63,       -1,       -1, G_TEXT            ,   /* Object 62  */
  NONE, NORMAL, (LONG)&rs_tedinfo[389],
  0x0013, 0x000A, 0x0006, 0x0001,
        64,       -1,       -1, G_TEXT            ,   /* Object 63  */
  NONE, NORMAL, (LONG)&rs_tedinfo[390],
  0x0003, 0x080A, 0x0009, 0x0001,
        65,       -1,       -1, G_TEXT            ,   /* Object 64  */
  NONE, NORMAL, (LONG)&rs_tedinfo[391],
  0x0013, 0x080A, 0x0004, 0x0001,
        66,       -1,       -1, G_TEXT            ,   /* Object 65  */
  NONE, NORMAL, (LONG)&rs_tedinfo[392],
  0x0003, 0x000B, 0x000B, 0x0001,
  EXTENDOK        , AREAS1          , AREAS8          , G_IBOX            ,   /* Object 66  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x000C, 0x0041, 0x0009,
  AREAS2          ,       -1,       -1, G_FTEXT           ,   /* Object 67 AREAS1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[393],
  0x0000, 0x0001, 0x0041, 0x0001,
  AREAS3          ,       -1,       -1, G_FTEXT           ,   /* Object 68 AREAS2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[394],
  0x0000, 0x0002, 0x0041, 0x0001,
  AREAS4          ,       -1,       -1, G_FTEXT           ,   /* Object 69 AREAS3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[395],
  0x0000, 0x0003, 0x0041, 0x0001,
  AREAS5          ,       -1,       -1, G_FTEXT           ,   /* Object 70 AREAS4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[396],
  0x0000, 0x0004, 0x0041, 0x0001,
  AREAS6          ,       -1,       -1, G_FTEXT           ,   /* Object 71 AREAS5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[397],
  0x0000, 0x0005, 0x0041, 0x0001,
  AREAS7          ,       -1,       -1, G_FTEXT           ,   /* Object 72 AREAS6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[398],
  0x0000, 0x0006, 0x0041, 0x0001,
  AREAS8          ,       -1,       -1, G_FTEXT           ,   /* Object 73 AREAS7 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[399],
  0x0000, 0x0007, 0x0041, 0x0001,
        66,       -1,       -1, G_FTEXT           ,   /* Object 74 AREAS8 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[400],
  0x0000, 0x0008, 0x0041, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 75 EXTENDOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0047, 0x0813, 0x0005, 0x0002,
  
  /******** Tree 18 DIALOG81 ****************************************************/
        -1,        1, REQZSAVE        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0022, 0x0011,
         4, REQZONE         , REQDOMAI        , G_BOX             ,   /* Object 1  */
  NONE, SHADOWED, (LONG)0x00FF1101L,
  0x0002, 0x0001, 0x001E, 0x0004,
  REQDOMAI        ,       -1,       -1, G_FTEXT           ,   /* Object 2 REQZONE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[401],
  0x0001, 0x0000, 0x001C, 0x0002,
         1,       -1,       -1, G_FTEXT           ,   /* Object 3 REQDOMAI */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[402],
  0x0001, 0x0002, 0x001C, 0x0002,
         5,       -1,       -1, G_TEXT            ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_tedinfo[403],
  0x0004, 0x0005, 0x000B, 0x0001,
  REQZCAN         , PRESET0         , PRESET7         , G_BOX             ,   /* Object 5  */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x0002, 0x0006, 0x001E, 0x0008,
  PRESET1         ,       -1,       -1, G_BOXTEXT         ,   /* Object 6 PRESET0 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[404],
  0x0000, 0x0000, 0x001E, 0x0001,
  PRESET2         ,       -1,       -1, G_BOXTEXT         ,   /* Object 7 PRESET1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[405],
  0x0000, 0x0001, 0x001E, 0x0001,
  PRESET3         ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 PRESET2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[406],
  0x0000, 0x0002, 0x001E, 0x0001,
  PRESET4         ,       -1,       -1, G_BOXTEXT         ,   /* Object 9 PRESET3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[407],
  0x0000, 0x0003, 0x001E, 0x0001,
  PRESET5         ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 PRESET4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[408],
  0x0000, 0x0004, 0x001E, 0x0001,
  PRESET6         ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 PRESET5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[409],
  0x0000, 0x0005, 0x001E, 0x0001,
  PRESET7         ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 PRESET6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[410],
  0x0000, 0x0006, 0x001E, 0x0001,
         5,       -1,       -1, G_BOXTEXT         ,   /* Object 13 PRESET7 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[411],
  0x0000, 0x0007, 0x001E, 0x0001,
  REQZSAVE        ,       -1,       -1, G_BUTTON          ,   /* Object 14 REQZCAN */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0002, 0x000F, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 15 REQZSAVE */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0016, 0x000F, 0x000A, 0x0001,
  
  /******** Tree 19 DIALOG31 ****************************************************/
        -1,        1, BUTIL6          , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0015,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[412],
  0x0000, 0x0000, 0x004F, 0x0001,
  DUTIL1          ,       -1,       -1, G_TEXT            ,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[413],
  0x0043, 0x0001, 0x0006, 0x0001,
  BOK             ,       -1,       -1, G_FTEXT           ,   /* Object 3 DUTIL1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[414],
  0x0005, 0x0002, 0x0011, 0x0001,
  AUTIL1          ,       -1,       -1, G_BUTTON          ,   /* Object 4 BOK */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"OK",
  0x0049, 0x0002, 0x0004, 0x000F,
  CUTIL1          ,       -1,       -1, G_FTEXT           ,   /* Object 5 AUTIL1 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[415],
  0x0007, 0x0003, 0x003D, 0x0001,
  BUTIL1          ,       -1,       -1, G_BOX             |0x0300,   /* Object 6 CUTIL1 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0003, 0x0002, 0x0001,
  DUTIL2          ,       -1,       -1, G_FTEXT           ,   /* Object 7 BUTIL1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[416],
  0x0008, 0x0004, 0x003C, 0x0001,
  AUTIL2          ,       -1,       -1, G_FTEXT           ,   /* Object 8 DUTIL2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[417],
  0x0005, 0x0005, 0x0011, 0x0001,
  CUTIL2          ,       -1,       -1, G_FTEXT           ,   /* Object 9 AUTIL2 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[418],
  0x0007, 0x0006, 0x003D, 0x0001,
  BUTIL2          ,       -1,       -1, G_BOX             |0x0300,   /* Object 10 CUTIL2 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0006, 0x0002, 0x0001,
  DUTIL3          ,       -1,       -1, G_FTEXT           ,   /* Object 11 BUTIL2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[419],
  0x0008, 0x0007, 0x003C, 0x0001,
  AUTIL3          ,       -1,       -1, G_FTEXT           ,   /* Object 12 DUTIL3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[420],
  0x0005, 0x0008, 0x0011, 0x0001,
  CUTIL3          ,       -1,       -1, G_FTEXT           ,   /* Object 13 AUTIL3 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[421],
  0x0007, 0x0009, 0x003D, 0x0001,
  BUTIL3          ,       -1,       -1, G_BOX             |0x0300,   /* Object 14 CUTIL3 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0009, 0x0002, 0x0001,
  DUTIL4          ,       -1,       -1, G_FTEXT           ,   /* Object 15 BUTIL3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[422],
  0x0008, 0x000A, 0x003C, 0x0001,
  AUTIL4          ,       -1,       -1, G_FTEXT           ,   /* Object 16 DUTIL4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[423],
  0x0005, 0x000B, 0x0011, 0x0001,
  CUTIL4          ,       -1,       -1, G_FTEXT           ,   /* Object 17 AUTIL4 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[424],
  0x0007, 0x000C, 0x003D, 0x0001,
  BUTIL4          ,       -1,       -1, G_BOX             |0x0300,   /* Object 18 CUTIL4 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x000C, 0x0002, 0x0001,
  DUTIL5          ,       -1,       -1, G_FTEXT           ,   /* Object 19 BUTIL4 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[425],
  0x0008, 0x000D, 0x003C, 0x0001,
  AUTIL5          ,       -1,       -1, G_FTEXT           ,   /* Object 20 DUTIL5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[426],
  0x0005, 0x000E, 0x0011, 0x0001,
  CUTIL5          ,       -1,       -1, G_FTEXT           ,   /* Object 21 AUTIL5 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[427],
  0x0007, 0x000F, 0x003D, 0x0001,
  BUTIL5          ,       -1,       -1, G_BOX             |0x0300,   /* Object 22 CUTIL5 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x000F, 0x0002, 0x0001,
  DUTIL6          ,       -1,       -1, G_FTEXT           ,   /* Object 23 BUTIL5 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[428],
  0x0008, 0x0010, 0x003C, 0x0001,
  AUTIL6          ,       -1,       -1, G_FTEXT           ,   /* Object 24 DUTIL6 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[429],
  0x0005, 0x0011, 0x0011, 0x0001,
  CUTIL6          ,       -1,       -1, G_FTEXT           ,   /* Object 25 AUTIL6 */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[430],
  0x0007, 0x0012, 0x003D, 0x0001,
  BCANCEL         ,       -1,       -1, G_BOX             |0x0300,   /* Object 26 CUTIL6 */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0045, 0x0012, 0x0002, 0x0001,
  BUTIL6          ,       -1,       -1, G_BOXTEXT         ,   /* Object 27 BCANCEL */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[431],
  0x0049, 0x0012, 0x0004, 0x0002,
         0,       -1,       -1, G_FTEXT           ,   /* Object 28 BUTIL6 */
  EDITABLE|LASTOB, NORMAL, (LONG)&rs_tedinfo[432],
  0x0008, 0x0013, 0x003C, 0x0001,
  
  /******** Tree 20 DIALOG72 ****************************************************/
        -1,        1,       10, G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0042, 0x000E,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, SHADOWED, (LONG)&rs_tedinfo[433],
  0x0002, 0x0001, 0x003E, 0x0001,
  XAREA           ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"Area:",
  0x0003, 0x0003, 0x0005, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3 XAREA */
  NONE, NORMAL, (LONG)"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
  0x0009, 0x0003, 0x0036, 0x0001,
  XNODE           ,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"Node:",
  0x0003, 0x0004, 0x0005, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5 XNODE */
  NONE, NORMAL, (LONG)"xxx:xxxxx/xxxxx@xxxxxxxxxxxxxxxxxxxxxxxxx",
  0x0009, 0x0004, 0x0029, 0x0001,
  NLINE1          ,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"following lines will be added:",
  0x0003, 0x0006, 0x001E, 0x0001,
  NLINE2          ,       -1,       -1, G_FTEXT           ,   /* Object 7 NLINE1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[434],
  0x0003, 0x0008, 0x003C, 0x0001,
  NLINE3          ,       -1,       -1, G_FTEXT           ,   /* Object 8 NLINE2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[435],
  0x0003, 0x0009, 0x003C, 0x0001,
        10,       -1,       -1, G_FTEXT           ,   /* Object 9 NLINE3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[436],
  0x0003, 0x000A, 0x003C, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 10  */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0036, 0x000C, 0x000A, 0x0001,
  
  /******** Tree 21 DIALOG21 ****************************************************/
        -1, SESSTEXT        , SESSTEXT        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x000B, 0x0001,
         0,       -1,       -1, G_FBOXTEXT        ,   /* Object 1 SESSTEXT */
  SELECTABLE|DEFAULT|EXIT|EDITABLE|LASTOB, NORMAL, (LONG)&rs_tedinfo[437],
  0x0000, 0x0000, 0x000B, 0x0001,
  
  /******** Tree 22 DIALOG82 ****************************************************/
        -1, FILETEXT        , FILEOK          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004B, 0x0009,
  FILENODE        ,       -1,       -1, G_FTEXT           ,   /* Object 1 FILETEXT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[438],
  0x0002, 0x0001, 0x0011, 0x0001,
         3,       -1,       -1, G_FTEXT           ,   /* Object 2 FILENODE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[439],
  0x0002, 0x0002, 0x0016, 0x0001,
  FILEPATH        ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Path:",
  0x0002, 0x0003, 0x0005, 0x0001,
         5,       -1,       -1, G_FTEXT           ,   /* Object 4 FILEPATH */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[440],
  0x0008, 0x0003, 0x0041, 0x0001,
         6,       -1,       -1, G_TEXT            ,   /* Object 5  */
  NONE, NORMAL, (LONG)&rs_tedinfo[441],
  0x0008, 0x0004, 0x021D, 0x0001,
         7,       -1,       -1, G_TEXT            ,   /* Object 6  */
  NONE, NORMAL, (LONG)&rs_tedinfo[442],
  0x0026, 0x0004, 0x0615, 0x0001,
         8,       -1,       -1, G_TEXT            ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_tedinfo[443],
  0x0008, 0x0804, 0x0226, 0x0001,
  FILEOK          ,       -1,       -1, G_TEXT            ,   /* Object 8  */
  NONE, NORMAL, (LONG)&rs_tedinfo[444],
  0x002F, 0x0804, 0x0012, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 9 FILEOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0040, 0x0007, 0x0009, 0x0001,
  
  /******** Tree 23 DIALOG19 ****************************************************/
        -1, TIMEBOXT        , TIMEBOXS        , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0028, 0x0007,
  MAXTIME         ,       -1,       -1, G_TEXT            ,   /* Object 1 TIMEBOXT */
  NONE, NORMAL, (LONG)&rs_tedinfo[445],
  0x0004, 0x0000, 0x0020, 0x0002,
  TIMEBOXS        , LEFTTIME        , LEFTTIME        , G_BOX             ,   /* Object 2 MAXTIME */
  NONE, NORMAL, (LONG)0x00FE1100L,
  0x0004, 0x0002, 0x0020, 0x0003,
  MAXTIME         ,       -1,       -1, G_BOX             ,   /* Object 3 LEFTTIME */
  NONE, NORMAL, (LONG)0x00FF1132L,
  0x0000, 0x0000, 0x0008, 0x0003,
         0,       -1,       -1, G_TEXT            ,   /* Object 4 TIMEBOXS */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[446],
  0x0003, 0x0005, 0x0021, 0x0001,
  
  /******** Tree 24 DIALOG61 ****************************************************/
        -1, APOLLTIM        , ATIME           , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0028, 0x0009,
         2,       -1,       -1, G_FBOXTEXT        ,   /* Object 1 APOLLTIM */
  NONE, SHADOWED, (LONG)&rs_tedinfo[447],
  0x0002, 0x0001, 0x0024, 0x0002,
  ATIME           ,       -1,       -1, G_TEXT            ,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[448],
  0x0002, 0x0004, 0x0024, 0x0001,
         0,       -1,       -1, G_FBOXTEXT        ,   /* Object 3 ATIME */
  LASTOB, SHADOWED, (LONG)&rs_tedinfo[449],
  0x0002, 0x0006, 0x0024, 0x0002,
  
  /******** Tree 25 DIALOG20 ****************************************************/
        -1, DUMMYPIC        , DUMMYPIC        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x001E, 0x0003,
         0,       -1,       -1, G_IMAGE           ,   /* Object 1 DUMMYPIC */
  LASTOB, NORMAL, (LONG)&rs_bitblk[3],
  0x0000, 0x0000, 0x0002, 0x0800,
  
  /******** Tree 26 DIALOG51 ****************************************************/
        -1, XCMD1           , XCMDCA1         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0040, 0x0009,
         2,       -1,       -1, G_FTEXT           ,   /* Object 1 XCMD1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[450],
  0x0002, 0x0001, 0x003C, 0x0001,
  XCMDDEL         ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"To kill entry:",
  0x0012, 0x0003, 0x000E, 0x0001,
         4,       -1,       -1, G_BUTTON          ,   /* Object 3 XCMDDEL */
  SELECTABLE|EXIT, NORMAL, (LONG)"Kill",
  0x0022, 0x0003, 0x0008, 0x0001,
         5,       -1,       -1, G_TEXT            ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_tedinfo[451],
  0x002B, 0x0003, 0x0211, 0x0001,
  XCMDINS         ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"To insert an entry:",
  0x000D, 0x0005, 0x0013, 0x0001,
         7,       -1,       -1, G_BUTTON          ,   /* Object 6 XCMDINS */
  SELECTABLE|EXIT, NORMAL, (LONG)"Insert",
  0x0022, 0x0005, 0x0008, 0x0001,
  XCMDOK1         ,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"Change commandline:",
  0x000D, 0x0007, 0x0013, 0x0001,
  XCMDGL1         ,       -1,       -1, G_BUTTON          ,   /* Object 8 XCMDOK1 */
  SELECTABLE|EXIT, NORMAL, (LONG)"Local",
  0x0022, 0x0007, 0x0008, 0x0001,
  XCMDCA1         ,       -1,       -1, G_BUTTON          ,   /* Object 9 XCMDGL1 */
  SELECTABLE|EXIT, NORMAL, (LONG)"Global",
  0x002C, 0x0007, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 10 XCMDCA1 */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Cancel",
  0x0036, 0x0007, 0x0008, 0x0001,
  
  /******** Tree 27 DIALOG22 ****************************************************/
        -1,        1,       42, G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0816,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[452],
  0x0000, 0x0000, 0x004F, 0x0001,
         3,       -1,       -1, G_TEXT            ,   /* Object 2  */
  NONE, NORMAL, (LONG)&rs_tedinfo[453],
  0x000A, 0x0001, 0x0015, 0x0001,
  SENDFILE        ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"File:",
  0x0004, 0x0002, 0x0005, 0x0001,
         5,       -1,       -1, G_FTEXT           ,   /* Object 4 SENDFILE */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[454],
  0x000A, 0x0002, 0x0041, 0x0001,
  SENDFLAG        ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Send:",
  0x0004, 0x0004, 0x0005, 0x0001,
  SENDTOUC        , SENDNORM        , SENDDIRE        , G_IBOX            ,   /* Object 6 SENDFLAG */
  NONE, NORMAL, (LONG)0x00001100L,
  0x000A, 0x0004, 0x001D, 0x0001,
  SENDCRAS        ,       -1,       -1, G_BOXTEXT         ,   /* Object 7 SENDNORM */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[455],
  0x0000, 0x0000, 0x0006, 0x0001,
  SENDHOLD        ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 SENDCRAS */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[456],
  0x0007, 0x0000, 0x0006, 0x0001,
  SENDDIRE        ,       -1,       -1, G_BOXTEXT         ,   /* Object 9 SENDHOLD */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[457],
  0x000E, 0x0000, 0x0006, 0x0001,
  SENDFLAG        ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 SENDDIRE */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[458],
  0x0015, 0x0000, 0x0006, 0x0001,
  SENDNODE        ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 SENDTOUC */
  SELECTABLE, NORMAL, (LONG)&rs_tedinfo[459],
  0x0029, 0x0004, 0x0007, 0x0001,
        13,       -1,       -1, G_FTEXT           ,   /* Object 12 SENDNODE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[460],
  0x0035, 0x0004, 0x0016, 0x0001,
        21, SENDTXT1        ,       20, G_BOX             ,   /* Object 13  */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x0002, 0x0007, 0x004B, 0x0807,
  SENDTXT2        ,       -1,       -1, G_FTEXT           ,   /* Object 14 SENDTXT1 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[461],
  0x0008, 0x0001, 0x0041, 0x0001,
  SENDTXT3        ,       -1,       -1, G_FTEXT           ,   /* Object 15 SENDTXT2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[462],
  0x0008, 0x0002, 0x0041, 0x0001,
  SENDAREA        ,       -1,       -1, G_FTEXT           ,   /* Object 16 SENDTXT3 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[463],
  0x0008, 0x0003, 0x0041, 0x0001,
  SENDPASS        ,       -1,       -1, G_FTEXT           ,   /* Object 17 SENDAREA */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[464],
  0x0002, 0x0005, 0x0029, 0x0001,
  SENDREPL        ,       -1,       -1, G_FTEXT           ,   /* Object 18 SENDPASS */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[465],
  0x0033, 0x0005, 0x0016, 0x0001,
        20,       -1,       -1, G_FTEXT           ,   /* Object 19 SENDREPL */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[466],
  0x0033, 0x0006, 0x0016, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 20  */
  NONE, NORMAL, (LONG)"Desc:",
  0x0002, 0x0001, 0x0005, 0x0001,
        22,       -1,       -1, G_TEXT            ,   /* Object 21  */
  NONE, NORMAL, (LONG)&rs_tedinfo[467],
  0x0014, 0x000F, 0x0615, 0x0001,
  SENDMAIL        ,       -1,       -1, G_STRING          ,   /* Object 22  */
  NONE, NORMAL, (LONG)"Report in",
  0x0003, 0x0010, 0x0009, 0x0001,
  SENDREP1        ,       -1,       -1, G_BOXTEXT         ,   /* Object 23 SENDMAIL */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[468],
  0x000D, 0x0010, 0x0009, 0x0001,
  SENDREP2        ,       -1,       -1, G_BOXTEXT         ,   /* Object 24 SENDREP1 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[469],
  0x0017, 0x0010, 0x0009, 0x0001,
  SENDNAME        ,       -1,       -1, G_BOXTEXT         ,   /* Object 25 SENDREP2 */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[470],
  0x0021, 0x0010, 0x0009, 0x0001,
        27,       -1,       -1, G_FTEXT           ,   /* Object 26 SENDNAME */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[471],
  0x002C, 0x0010, 0x0020, 0x0001,
  SENDSAVZ        ,       28, SENDDEF9        , G_IBOX            ,   /* Object 27  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0012, 0x0035, 0x0003,
  SENDDEF1        ,       -1,       -1, G_STRING          ,   /* Object 28  */
  NONE, NORMAL, (LONG)"Defaults:",
  0x0001, 0x0000, 0x0009, 0x0001,
  SENDDEF2        ,       -1,       -1, G_BOXTEXT         ,   /* Object 29 SENDDEF1 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[472],
  0x000B, 0x0000, 0x000C, 0x0001,
  SENDDEF3        ,       -1,       -1, G_BOXTEXT         ,   /* Object 30 SENDDEF2 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[473],
  0x0019, 0x0000, 0x000C, 0x0001,
  SENDDEF4        ,       -1,       -1, G_BOXTEXT         ,   /* Object 31 SENDDEF3 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[474],
  0x0027, 0x0000, 0x000C, 0x0001,
  SENDDEF5        ,       -1,       -1, G_BOXTEXT         ,   /* Object 32 SENDDEF4 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[475],
  0x000B, 0x0001, 0x000C, 0x0001,
  SENDDEF6        ,       -1,       -1, G_BOXTEXT         ,   /* Object 33 SENDDEF5 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[476],
  0x0019, 0x0001, 0x000C, 0x0001,
  SENDDEF7        ,       -1,       -1, G_BOXTEXT         ,   /* Object 34 SENDDEF6 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[477],
  0x0027, 0x0001, 0x000C, 0x0001,
  SENDDEF8        ,       -1,       -1, G_BOXTEXT         ,   /* Object 35 SENDDEF7 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[478],
  0x000B, 0x0002, 0x000C, 0x0001,
  SENDDEF9        ,       -1,       -1, G_BOXTEXT         ,   /* Object 36 SENDDEF8 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[479],
  0x0019, 0x0002, 0x000C, 0x0001,
        27,       -1,       -1, G_BOXTEXT         ,   /* Object 37 SENDDEF9 */
  SELECTABLE|EXIT|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[480],
  0x0027, 0x0002, 0x000C, 0x0001,
  SENDSAVE        ,       -1,       -1, G_BUTTON          ,   /* Object 38 SENDSAVZ */
  SELECTABLE|EXIT, NORMAL, (LONG)"ZoneSend",
  0x0038, 0x0012, 0x0009, 0x0001,
  SENDCANC        ,       -1,       -1, G_BUTTON          ,   /* Object 39 SENDSAVE */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"Send",
  0x0043, 0x0012, 0x0009, 0x0001,
        41,       -1,       -1, G_BUTTON          ,   /* Object 40 SENDCANC */
  SELECTABLE|EXIT, NORMAL, (LONG)"EXIT",
  0x0038, 0x0014, 0x0014, 0x0001,
        42,       -1,       -1, G_TEXT            ,   /* Object 41  */
  NONE, NORMAL, (LONG)&rs_tedinfo[481],
  0x000D, 0x0015, 0x0214, 0x0001,
         0, SENDNONE        , SENDLOAD        , G_IBOX            ,   /* Object 42  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0004, 0x0806, 0x0048, 0x0001,
  SENDTIC         ,       -1,       -1, G_BOXTEXT         ,   /* Object 43 SENDNONE */
  SELECTABLE|RBUTTON, SELECTED, (LONG)&rs_tedinfo[482],
  0x0000, 0x0000, 0x0008, 0x0001,
  SENDATTA        ,       -1,       -1, G_BOXTEXT         ,   /* Object 44 SENDTIC */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[483],
  0x0009, 0x0000, 0x0008, 0x0001,
  SENDLOAD        ,       -1,       -1, G_BOXTEXT         ,   /* Object 45 SENDATTA */
  SELECTABLE|RBUTTON, NORMAL, (LONG)&rs_tedinfo[484],
  0x0012, 0x0000, 0x000A, 0x0001,
        42,       -1,       -1, G_BUTTON          ,   /* Object 46 SENDLOAD */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)"LoadText",
  0x003E, 0x0000, 0x0009, 0x0001,
  
  /******** Tree 28 DIALOG23 ****************************************************/
        -1, DEFNAME         , DEFOK           , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002F, 0x0807,
  DEFNODE         ,       -1,       -1, G_FTEXT           ,   /* Object 1 DEFNAME */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[485],
  0x0002, 0x0001, 0x0013, 0x0001,
  DEFSYSOP        ,       -1,       -1, G_FTEXT           ,   /* Object 2 DEFNODE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[486],
  0x0002, 0x0002, 0x0018, 0x0001,
  DEFAREA         ,       -1,       -1, G_FTEXT           ,   /* Object 3 DEFSYSOP */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[487],
  0x0002, 0x0003, 0x0021, 0x0001,
  DEFPASS         ,       -1,       -1, G_FTEXT           ,   /* Object 4 DEFAREA */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[488],
  0x0002, 0x0004, 0x002B, 0x0001,
  DEFOK           ,       -1,       -1, G_FTEXT           ,   /* Object 5 DEFPASS */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[489],
  0x0002, 0x0005, 0x0014, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 6 DEFOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0024, 0x0805, 0x0009, 0x0001,
  
  /******** Tree 29 DIALOG24 ****************************************************/
        -1, EDITTEXT        , EDITOK          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0046, 0x0007,
  EDITMAIL        ,       -1,       -1, G_FBOXTEXT        ,   /* Object 1 EDITTEXT */
  NONE, SHADOWED, (LONG)&rs_tedinfo[490],
  0x0002, 0x0001, 0x0042, 0x0001,
         3,       -1,       -1, G_FTEXT           ,   /* Object 2 EDITMAIL */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[491],
  0x0002, 0x0003, 0x0038, 0x0001,
  EDITNAME        ,       -1,       -1, G_TEXT            ,   /* Object 3  */
  NONE, NORMAL, (LONG)&rs_tedinfo[492],
  0x0008, 0x0004, 0x062D, 0x0001,
  EDITOK          ,       -1,       -1, G_FTEXT           ,   /* Object 4 EDITNAME */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[493],
  0x0002, 0x0005, 0x000E, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 5 EDITOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x003B, 0x0005, 0x0009, 0x0001,
  
  /******** Tree 30 DIALOG52 ****************************************************/
        -1, XCMD2           , XCMDOK2         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0040, 0x0009,
         2,       -1,       -1, G_FTEXT           ,   /* Object 1 XCMD2 */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[494],
  0x0002, 0x0001, 0x003C, 0x0001,
  XCMDDEL2        ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"To kill entry:",
  0x0012, 0x0003, 0x000E, 0x0001,
         4,       -1,       -1, G_BUTTON          ,   /* Object 3 XCMDDEL2 */
  SELECTABLE|EXIT, NORMAL, (LONG)"Kill",
  0x0022, 0x0003, 0x0008, 0x0001,
         5,       -1,       -1, G_TEXT            ,   /* Object 4  */
  NONE, NORMAL, (LONG)&rs_tedinfo[495],
  0x002B, 0x0003, 0x0211, 0x0001,
  XCMDINS2        ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"To insert an entry:",
  0x000D, 0x0005, 0x0013, 0x0001,
  XCMDOK2         ,       -1,       -1, G_BUTTON          ,   /* Object 6 XCMDINS2 */
  SELECTABLE|EXIT, NORMAL, (LONG)"Insert",
  0x0022, 0x0005, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 7 XCMDOK2 */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0036, 0x0007, 0x0008, 0x0001,
  
  /******** Tree 31 DIALOG25 ****************************************************/
        -1,        1, FIXAREA         , G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1123L,
  0x0000, 0x0000, 0x004B, 0x0013,
  FIXBOX          ,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[496],
  0x0000, 0x0000, 0x004B, 0x0001,
  FIXMAX          , FIX1            , FIX10           , G_BOX             ,   /* Object 2 FIXBOX */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0002, 0x0002, 0x0047, 0x000C,
  FIX2            ,       -1,       -1, G_STRING          ,   /* Object 3 FIX1 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0001, 0x0045, 0x0001,
  FIX3            ,       -1,       -1, G_STRING          ,   /* Object 4 FIX2 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0002, 0x0045, 0x0001,
  FIX4            ,       -1,       -1, G_STRING          ,   /* Object 5 FIX3 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0003, 0x0045, 0x0001,
  FIX5            ,       -1,       -1, G_STRING          ,   /* Object 6 FIX4 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0004, 0x0045, 0x0001,
  FIX6            ,       -1,       -1, G_STRING          ,   /* Object 7 FIX5 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0005, 0x0045, 0x0001,
  FIX7            ,       -1,       -1, G_STRING          ,   /* Object 8 FIX6 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0006, 0x0045, 0x0001,
  FIX8            ,       -1,       -1, G_STRING          ,   /* Object 9 FIX7 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0007, 0x0045, 0x0001,
  FIX9            ,       -1,       -1, G_STRING          ,   /* Object 10 FIX8 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0008, 0x0045, 0x0001,
  FIX10           ,       -1,       -1, G_STRING          ,   /* Object 11 FIX9 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0009, 0x0045, 0x0001,
  FIXBOX          ,       -1,       -1, G_STRING          ,   /* Object 12 FIX10 */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000A, 0x0045, 0x0001,
  FIXFULL         ,       -1,       -1, G_FTEXT           ,   /* Object 13 FIXMAX */
  NONE, NORMAL, (LONG)&rs_tedinfo[497],
  0x003E, 0x000E, 0x020B, 0x0001,
        21, FIXUPUP         , FIXDNDN         , G_IBOX            ,   /* Object 14 FIXFULL */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x000F, 0x0048, 0x0001,
  FIXUP           ,       -1,       -1, G_BOXTEXT         ,   /* Object 15 FIXUPUP */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[498],
  0x0000, 0x0000, 0x0004, 0x0001,
  FIXBK           ,       -1,       -1, G_BOXCHAR         ,   /* Object 16 FIXUP */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0004, 0x0000, 0x0004, 0x0001,
  FIXDOWN         , FIXFD           , FIXFD           , G_BOX             ,   /* Object 17 FIXBK */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1143L,
  0x0008, 0x0000, 0x0037, 0x0001,
  FIXBK           ,       -1,       -1, G_BOX             ,   /* Object 18 FIXFD */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1192L,
  0x0006, 0x0000, 0x0003, 0x0001,
  FIXDNDN         ,       -1,       -1, G_BOXCHAR         ,   /* Object 19 FIXDOWN */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x003F, 0x0000, 0x0004, 0x0001,
  FIXFULL         ,       -1,       -1, G_BOXTEXT         ,   /* Object 20 FIXDNDN */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[499],
  0x0043, 0x0000, 0x0004, 0x0001,
  FIXCANC         ,       -1,       -1, G_TEXT            ,   /* Object 21  */
  NONE, NORMAL, (LONG)&rs_tedinfo[500],
  0x000A, 0x0010, 0x0037, 0x0001,
  FIXAREA         ,       -1,       -1, G_BUTTON          ,   /* Object 22 FIXCANC */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0033, 0x0011, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 23 FIXAREA */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"AreaFix",
  0x003F, 0x0011, 0x000A, 0x0001,
  
  /******** Tree 32 DIALOG26 ****************************************************/
        -1,        1, FIXOK           , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0028, 0x000F,
  PASSSUBJ        , PASSNODE        , PASSDOMA        , G_BOX             ,   /* Object 1  */
  NONE, SHADOWED, (LONG)0x00FF1101L,
  0x0002, 0x0001, 0x0024, 0x0008,
         3,       -1,       -1, G_TEXT            ,   /* Object 2 PASSNODE */
  NONE, SHADOWED, (LONG)&rs_tedinfo[501],
  0x0000, 0x0005, 0x0024, 0x0001,
  FIXNAME         ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, SHADOWED, (LONG)"Message to",
  0x000D, 0x0001, 0x000A, 0x0001,
  PASSDOMA        ,       -1,       -1, G_FTEXT           ,   /* Object 4 FIXNAME */
  EDITABLE, SHADOWED, (LONG)&rs_tedinfo[502],
  0x0000, 0x0003, 0x0024, 0x0001,
         1,       -1,       -1, G_TEXT            ,   /* Object 5 PASSDOMA */
  NONE, SHADOWED, (LONG)&rs_tedinfo[503],
  0x0000, 0x0006, 0x0024, 0x0001,
         7,       -1,       -1, G_FTEXT           ,   /* Object 6 PASSSUBJ */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[504],
  0x0003, 0x000A, 0x0022, 0x0001,
  FIXNEXT         ,       -1,       -1, G_TEXT            ,   /* Object 7  */
  NONE, NORMAL, (LONG)&rs_tedinfo[505],
  0x000C, 0x000B, 0x0615, 0x0001,
  FIXOK           ,       -1,       -1, G_BUTTON          ,   /* Object 8 FIXNEXT */
  SELECTABLE|EXIT, NORMAL, (LONG)"Next",
  0x0002, 0x000D, 0x0008, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 9 FIXOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Create",
  0x001E, 0x000D, 0x0008, 0x0001,
  
  /******** Tree 33 DIALOG27 ****************************************************/
        -1, AUTOAREA        , AUTOOK          , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021113L,
  0x0000, 0x0000, 0x004B, 0x000C,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1 AUTOAREA */
  NONE, SHADOWED, (LONG)&rs_tedinfo[506],
  0x0002, 0x0001, 0x0047, 0x0002,
  AUTOCANC        , AUTOBOSS        , AUTOORG         , G_BOX             ,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00FF1101L,
  0x0002, 0x0004, 0x0047, 0x0005,
  AUTOZONE        ,       -1,       -1, G_FTEXT           ,   /* Object 3 AUTOBOSS */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[507],
  0x0003, 0x0001, 0x002B, 0x0001,
  AUTODAYS        ,       -1,       -1, G_FTEXT           ,   /* Object 4 AUTOZONE */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[508],
  0x0030, 0x0001, 0x0009, 0x0001,
         6,       -1,       -1, G_FTEXT           ,   /* Object 5 AUTODAYS */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[509],
  0x003C, 0x0001, 0x0009, 0x0001,
  AUTOPATH        ,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"Path:",
  0x0003, 0x0002, 0x0005, 0x0001,
         8,       -1,       -1, G_FTEXT           ,   /* Object 7 AUTOPATH */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[510],
  0x0009, 0x0002, 0x003C, 0x0001,
  AUTOORG         ,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"Origin:",
  0x0001, 0x0003, 0x0007, 0x0001,
         2,       -1,       -1, G_FTEXT           ,   /* Object 9 AUTOORG */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[511],
  0x0009, 0x0003, 0x003C, 0x0001,
  AUTOOK          ,       -1,       -1, G_BUTTON          ,   /* Object 10 AUTOCANC */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0002, 0x000A, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 11 AUTOOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Add area",
  0x003F, 0x000A, 0x000A, 0x0001,
  
  /******** Tree 34 DIALOG28 ****************************************************/
        -1, HELPLST         ,       30, G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0043, 0x0016,
  HELPFIX         ,       -1,       -1, G_STRING          ,   /* Object 1 HELPLST */
  NONE, NORMAL, (LONG)"Select from filelist:",
  0x0002, 0x0001, 0x0015, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2 HELPFIX */
  NONE, NORMAL, (LONG)"Areafix manager:",
  0x0026, 0x0001, 0x0010, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"\001,\002 ... Previous/Next page",
  0x0004, 0x0003, 0x001A, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"\001,\002 ... Line up/down",
  0x0027, 0x0003, 0x0014, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"\004,\003 ... 10KB backwards/forwards",
  0x0004, 0x0004, 0x001F, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"\004,\003 ... Page up/down",
  0x0027, 0x0004, 0x0014, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"F ... Find string",
  0x0006, 0x0005, 0x0011, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"F ... Find string (area)",
  0x0029, 0x0005, 0x0018, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"G,N ... Continue search",
  0x0004, 0x0006, 0x0017, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"G,N ... Continue search",
  0x0027, 0x0006, 0x0017, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"Home ... Start of list",
  0x0003, 0x0007, 0x0016, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"Home ... Start of list",
  0x0026, 0x0007, 0x0016, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"Clr ... End of list",
  0x0004, 0x0008, 0x0013, 0x0001,
  HELPREQ         ,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"Clr ... End of list",
  0x0027, 0x0008, 0x0013, 0x0001,
  HELPSND         ,       -1,       -1, G_STRING          ,   /* Object 15 HELPREQ */
  NONE, NORMAL, (LONG)"Filerequest:",
  0x0002, 0x000A, 0x000C, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16 HELPSND */
  NONE, NORMAL, (LONG)"File hatch:",
  0x0026, 0x000A, 0x000B, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  NONE, NORMAL, (LONG)"ALT C ... Clear entries",
  0x0002, 0x000C, 0x0017, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  NONE, NORMAL, (LONG)"ALT S ... Select file",
  0x0026, 0x000C, 0x0015, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 19  */
  NONE, NORMAL, (LONG)"ALT F ... Request FILES",
  0x0002, 0x000D, 0x0017, 0x0001,
        21,       -1,       -1, G_STRING          ,   /* Object 20  */
  NONE, NORMAL, (LONG)"ALT L ... Load description",
  0x0026, 0x000D, 0x001A, 0x0001,
        22,       -1,       -1, G_STRING          ,   /* Object 21  */
  NONE, NORMAL, (LONG)"ALT S ... Select from filelist",
  0x0002, 0x000E, 0x001E, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  NONE, NORMAL, (LONG)"ALT Z ... 4D/5D hatch",
  0x0026, 0x000E, 0x0015, 0x0001,
        24,       -1,       -1, G_STRING          ,   /* Object 23  */
  NONE, NORMAL, (LONG)"ALT W ... Select 1-9,10-18,19-27",
  0x0002, 0x000F, 0x0020, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  NONE, NORMAL, (LONG)"ALT Q,E . Quit",
  0x0026, 0x000F, 0x000E, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  NONE, NORMAL, (LONG)"ALT L ... Load old request",
  0x0002, 0x0010, 0x001A, 0x0001,
        27,       -1,       -1, G_STRING          ,   /* Object 26  */
  NONE, NORMAL, (LONG)"ALT Z ... 4D/5D request",
  0x0002, 0x0011, 0x0017, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  NONE, NORMAL, (LONG)"ALT 0 ... Fileselector",
  0x0002, 0x0012, 0x0016, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  NONE, NORMAL, (LONG)"ALT 1-9 . Select filelist",
  0x0002, 0x0013, 0x0019, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  NONE, NORMAL, (LONG)"ALT Q,E . Quit",
  0x0002, 0x0014, 0x000E, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 30  */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Exit",
  0x0037, 0x0014, 0x000A, 0x0001,
  
  /******** Tree 35 DIALOG9B ****************************************************/
        -1,        1,       48, G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1123L,
  0x0000, 0x0000, 0x004F, 0x002B,
        40,        2,       39, G_BOX             ,   /* Object 1  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x004F, 0x0028,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0001, 0x004D, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0002, 0x004D, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0003, 0x004D, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0004, 0x004D, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0005, 0x004D, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0006, 0x004D, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0007, 0x004D, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0008, 0x004D, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0009, 0x004D, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000A, 0x004D, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000B, 0x004D, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000C, 0x004D, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000D, 0x004D, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000E, 0x004D, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x000F, 0x004D, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0010, 0x004D, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0011, 0x004D, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 19  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0012, 0x004D, 0x0001,
        21,       -1,       -1, G_STRING          ,   /* Object 20  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0013, 0x004D, 0x0001,
        22,       -1,       -1, G_STRING          ,   /* Object 21  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0014, 0x004D, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0015, 0x004D, 0x0001,
        24,       -1,       -1, G_STRING          ,   /* Object 23  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0016, 0x004D, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0017, 0x004D, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0018, 0x004D, 0x0001,
        27,       -1,       -1, G_STRING          ,   /* Object 26  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0019, 0x004D, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x001A, 0x004D, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x001B, 0x004D, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x001C, 0x004D, 0x0001,
        31,       -1,       -1, G_STRING          ,   /* Object 30  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x001D, 0x004D, 0x0001,
        32,       -1,       -1, G_STRING          ,   /* Object 31  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x001E, 0x004D, 0x0001,
        33,       -1,       -1, G_STRING          ,   /* Object 32  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x001F, 0x004D, 0x0001,
        34,       -1,       -1, G_STRING          ,   /* Object 33  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0020, 0x004D, 0x0001,
        35,       -1,       -1, G_STRING          ,   /* Object 34  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0021, 0x004D, 0x0001,
        36,       -1,       -1, G_STRING          ,   /* Object 35  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0022, 0x004D, 0x0001,
        37,       -1,       -1, G_STRING          ,   /* Object 36  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0023, 0x004D, 0x0001,
        38,       -1,       -1, G_STRING          ,   /* Object 37  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0024, 0x004D, 0x0001,
        39,       -1,       -1, G_STRING          ,   /* Object 38  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0025, 0x004D, 0x0001,
         1,       -1,       -1, G_STRING          ,   /* Object 39  */
  SELECTABLE|TOUCHEXIT, NORMAL, (LONG)"12345678901234567890123456789012345678901234567890123456789012345123456789012",
  0x0001, 0x0026, 0x004D, 0x0001,
        47,       41,       46, G_IBOX            ,   /* Object 40  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0029, 0x0041, 0x0001,
        42,       -1,       -1, G_BOXTEXT         ,   /* Object 41  */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[512],
  0x0000, 0x0000, 0x0004, 0x0001,
        43,       -1,       -1, G_BOXCHAR         ,   /* Object 42  */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0004, 0x0000, 0x0004, 0x0001,
        45,       44,       44, G_BOX             ,   /* Object 43  */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1143L,
  0x0008, 0x0000, 0x002F, 0x0001,
        43,       -1,       -1, G_BOX             ,   /* Object 44  */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1112L,
  0x0006, 0x0000, 0x0003, 0x0001,
        46,       -1,       -1, G_BOXCHAR         ,   /* Object 45  */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x0037, 0x0000, 0x0004, 0x0001,
        40,       -1,       -1, G_BOXTEXT         ,   /* Object 46  */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[513],
  0x003B, 0x0000, 0x0004, 0x0001,
        48,       -1,       -1, G_BUTTON          ,   /* Object 47  */
  SELECTABLE|DEFAULT|EXIT, NORMAL, (LONG)"OK",
  0x0044, 0x0029, 0x0009, 0x0001,
         0,       -1,       -1, G_TEXT            ,   /* Object 48  */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[514],
  0x000A, 0x002A, 0x002F, 0x0001,
  
  /******** Tree 36 DIALO25B ****************************************************/
        -1,        1,       43, G_BOX             ,   /* Object 0  */
  NONE, NORMAL, (LONG)0x00FF1123L,
  0x0000, 0x0000, 0x004B, 0x0027,
         2,       -1,       -1, G_BOXTEXT         ,   /* Object 1  */
  NONE, NORMAL, (LONG)&rs_tedinfo[515],
  0x0000, 0x0000, 0x004B, 0x0001,
        33,        3,       32, G_BOX             ,   /* Object 2  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0002, 0x0002, 0x0047, 0x0020,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0001, 0x0045, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0002, 0x0045, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0003, 0x0045, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0004, 0x0045, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0005, 0x0045, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0006, 0x0045, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0007, 0x0045, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0008, 0x0045, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0009, 0x0045, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000A, 0x0045, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000B, 0x0045, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000C, 0x0045, 0x0001,
        16,       -1,       -1, G_STRING          ,   /* Object 15  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000D, 0x0045, 0x0001,
        17,       -1,       -1, G_STRING          ,   /* Object 16  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000E, 0x0045, 0x0001,
        18,       -1,       -1, G_STRING          ,   /* Object 17  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x000F, 0x0045, 0x0001,
        19,       -1,       -1, G_STRING          ,   /* Object 18  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0010, 0x0045, 0x0001,
        20,       -1,       -1, G_STRING          ,   /* Object 19  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0011, 0x0045, 0x0001,
        21,       -1,       -1, G_STRING          ,   /* Object 20  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0012, 0x0045, 0x0001,
        22,       -1,       -1, G_STRING          ,   /* Object 21  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0013, 0x0045, 0x0001,
        23,       -1,       -1, G_STRING          ,   /* Object 22  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0014, 0x0045, 0x0001,
        24,       -1,       -1, G_STRING          ,   /* Object 23  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0015, 0x0045, 0x0001,
        25,       -1,       -1, G_STRING          ,   /* Object 24  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0016, 0x0045, 0x0001,
        26,       -1,       -1, G_STRING          ,   /* Object 25  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0017, 0x0045, 0x0001,
        27,       -1,       -1, G_STRING          ,   /* Object 26  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0018, 0x0045, 0x0001,
        28,       -1,       -1, G_STRING          ,   /* Object 27  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x0019, 0x0045, 0x0001,
        29,       -1,       -1, G_STRING          ,   /* Object 28  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x001A, 0x0045, 0x0001,
        30,       -1,       -1, G_STRING          ,   /* Object 29  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x001B, 0x0045, 0x0001,
        31,       -1,       -1, G_STRING          ,   /* Object 30  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x001C, 0x0045, 0x0001,
        32,       -1,       -1, G_STRING          ,   /* Object 31  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x001D, 0x0045, 0x0001,
         2,       -1,       -1, G_STRING          ,   /* Object 32  */
  SELECTABLE|EXIT, NORMAL, (LONG)"+ 123456789012345678901234567890123456789012345678 12345:12345/12345 ",
  0x0001, 0x001E, 0x0045, 0x0001,
        34,       -1,       -1, G_FTEXT           ,   /* Object 33  */
  NONE, NORMAL, (LONG)&rs_tedinfo[516],
  0x003E, 0x0022, 0x020B, 0x0001,
        41,       35,       40, G_IBOX            ,   /* Object 34  */
  NONE, NORMAL, (LONG)0x00001100L,
  0x0002, 0x0023, 0x0048, 0x0001,
        36,       -1,       -1, G_BOXTEXT         ,   /* Object 35  */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[517],
  0x0000, 0x0000, 0x0004, 0x0001,
        37,       -1,       -1, G_BOXCHAR         ,   /* Object 36  */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\004' << 24)|0x00FF1100L,
  0x0004, 0x0000, 0x0004, 0x0001,
        39,       38,       38, G_BOX             ,   /* Object 37  */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1143L,
  0x0008, 0x0000, 0x0037, 0x0001,
        37,       -1,       -1, G_BOX             ,   /* Object 38  */
  TOUCHEXIT, NORMAL, (LONG)0x00FF1192L,
  0x0006, 0x0000, 0x0003, 0x0001,
        40,       -1,       -1, G_BOXCHAR         ,   /* Object 39  */
  TOUCHEXIT, NORMAL, (LONG) ((LONG)'\003' << 24)|0x00FF1100L,
  0x003F, 0x0000, 0x0004, 0x0001,
        34,       -1,       -1, G_BOXTEXT         ,   /* Object 40  */
  TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[518],
  0x0043, 0x0000, 0x0004, 0x0001,
        42,       -1,       -1, G_TEXT            ,   /* Object 41  */
  NONE, NORMAL, (LONG)&rs_tedinfo[519],
  0x000A, 0x0024, 0x0037, 0x0001,
        43,       -1,       -1, G_BUTTON          ,   /* Object 42  */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0033, 0x0025, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 43  */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"AreaFix",
  0x003F, 0x0025, 0x000A, 0x0001,
  
  /******** Tree 37 DIALOG29 ****************************************************/
        -1, MSGEDIT         , MSGOK           , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0023, 0x0003,
  MSGOK           ,       -1,       -1, G_FTEXT           ,   /* Object 1 MSGEDIT */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[520],
  0x0003, 0x0001, 0x0011, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 2 MSGOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0018, 0x0001, 0x0009, 0x0001,
  
  /******** Tree 38 DIALOG30 ****************************************************/
        -1,        1, AVALEXIT        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x002D, 0x000B,
  CSPECIAL        ,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"You have made following changes:",
  0x0007, 0x0001, 0x0020, 0x0001,
  CFILELIS        ,       -1,       -1, G_STRING          ,   /* Object 2 CSPECIAL */
  NONE, NORMAL, (LONG)"Special setup",
  0x0003, 0x0003, 0x000D, 0x0001,
  CSESSION        ,       -1,       -1, G_STRING          ,   /* Object 3 CFILELIS */
  NONE, NORMAL, (LONG)"FileRequest",
  0x0017, 0x0003, 0x000B, 0x0001,
  CHATCHMA        ,       -1,       -1, G_STRING          ,   /* Object 4 CSESSION */
  NONE, NORMAL, (LONG)"Session setup",
  0x0003, 0x0004, 0x000D, 0x0001,
  CPROGRAM        ,       -1,       -1, G_STRING          ,   /* Object 5 CHATCHMA */
  NONE, NORMAL, (LONG)"FileHatch (MailArea)",
  0x0017, 0x0004, 0x0014, 0x0001,
  CHATCHDE        ,       -1,       -1, G_STRING          ,   /* Object 6 CPROGRAM */
  NONE, NORMAL, (LONG)"Find 1 (programs)",
  0x0003, 0x0005, 0x0011, 0x0001,
  CUTILITY        ,       -1,       -1, G_STRING          ,   /* Object 7 CHATCHDE */
  NONE, NORMAL, (LONG)"FileHatch (defaults)",
  0x0017, 0x0005, 0x0014, 0x0001,
  CTERMINA        ,       -1,       -1, G_STRING          ,   /* Object 8 CUTILITY */
  NONE, NORMAL, (LONG)"Find 2 (utilities)",
  0x0003, 0x0006, 0x0012, 0x0001,
  CEPROGRA        ,       -1,       -1, G_STRING          ,   /* Object 9 CTERMINA */
  NONE, NORMAL, (LONG)"Terminal (serport)",
  0x0017, 0x0006, 0x0012, 0x0001,
  DOCANCEL        ,       -1,       -1, G_STRING          ,   /* Object 10 CEPROGRA */
  NONE, NORMAL, (LONG)"Program-Manager",
  0x0003, 0x0007, 0x000F, 0x0001,
  AVALEXIT        ,       -1,       -1, G_BUTTON          ,   /* Object 11 DOCANCEL */
  SELECTABLE|EXIT, NORMAL, (LONG)"Cancel",
  0x0002, 0x0009, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 12 AVALEXIT */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"EXIT",
  0x0021, 0x0009, 0x000A, 0x0001,
  
  /******** Tree 39 DIALOG40 ****************************************************/
        -1,        1, FIRSTSET        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0047, 0x0016,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Avalon was not able to find the system folder [.\134AVALON]...",
  0x0002, 0x0001, 0x003B, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"1.) You use Avalon the first time:",
  0x0002, 0x0003, 0x0022, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Avalon creates the missing system folder.",
  0x0006, 0x0004, 0x0029, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"2.) An older version of Avalon has been installed so far:",
  0x0002, 0x0005, 0x0039, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Avalon creates the missing system folder and moves the existing",
  0x0006, 0x0006, 0x003F, 0x0001,
         7,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"system files into it.",
  0x0006, 0x0007, 0x0015, 0x0001,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"ATTENTION: You must copy BINKLEY.[0-5] manually!",
  0x0006, 0x0008, 0x0030, 0x0001,
         9,       -1,       -1, G_IBOX            |0x0200,   /* Object 8  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0009, 0x0047, 0x0002,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"Avalon konnte nicht den Systemordner [.\134AVALON] finden...",
  0x0002, 0x000B, 0x0039, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"1.) Avalon wurde das erstemal gestartet:",
  0x0002, 0x000D, 0x0028, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"Avalon erzeugt den fehlenden Systemordner.",
  0x0006, 0x000E, 0x002A, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"2.) Eine \204ltere Version von Avalon wurde bereits installiert:",
  0x0002, 0x000F, 0x003D, 0x0001,
        14,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"Avalon erzeugt den fehlenden Systemordner und verschiebt die",
  0x0006, 0x0010, 0x003C, 0x0001,
        15,       -1,       -1, G_STRING          ,   /* Object 14  */
  NONE, NORMAL, (LONG)"bereits vorhandenen Systemdateien dorthin.",
  0x0006, 0x0011, 0x002A, 0x0001,
  FIRSTSET        ,       -1,       -1, G_STRING          ,   /* Object 15  */
  NONE, NORMAL, (LONG)"ACHTUNG: BINKLEY.[0-5] mu\236 manuell kopiert werden!",
  0x0006, 0x0012, 0x0032, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 16 FIRSTSET */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x003B, 0x0014, 0x000A, 0x0001,
  
  /******** Tree 40 PICTURE0 ****************************************************/
        -1, AVALBLD0        , ICFS1           , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0037, 0x0410,
  AVALBLD1        ,       -1,       -1, G_IMAGE           ,   /* Object 1 AVALBLD0 */
  NONE, NORMAL, (LONG)&rs_bitblk[4],
  0x0002, 0x0001, 0x001E, 0x0003,
  ICFS0           ,       -1,       -1, G_IMAGE           ,   /* Object 2 AVALBLD1 */
  NONE, NORMAL, (LONG)&rs_bitblk[5],
  0x0002, 0x0407, 0x001E, 0x0801,
  ICFS1           ,       -1,       -1, G_IMAGE           ,   /* Object 3 ICFS0 */
  NONE, NORMAL, (LONG)&rs_bitblk[6],
  0x0004, 0x000B, 0x0004, 0x0801,
         0,       -1,       -1, G_IMAGE           ,   /* Object 4 ICFS1 */
  LASTOB, NORMAL, (LONG)&rs_bitblk[7],
  0x0004, 0x000D, 0x0004, 0x0C00,
  
  /******** Tree 41 DIALOG41 ****************************************************/
        -1,        1, USEBINK         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0036, 0x000D,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Which mailer do you use...?",
  0x0002, 0x0001, 0x001B, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"Your choice can be changed everytime you want!",
  0x0002, 0x0003, 0x002E, 0x0001,
         4,       -1,       -1, G_IBOX            |0x0200,   /* Object 3  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0004, 0x0036, 0x0002,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"Welcher Mailer wird benutzt...?",
  0x0002, 0x0006, 0x001F, 0x0001,
  USESEMP         ,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"Die Auswahl kann sp\204ter jederzeit ge\204ndert werden!",
  0x0002, 0x0008, 0x0032, 0x0001,
  USEBINK         ,       -1,       -1, G_BUTTON          ,   /* Object 6 USESEMP */
  SELECTABLE|EXIT, NORMAL, (LONG)"Semper",
  0x001E, 0x000B, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 7 USEBINK */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)"Binkley",
  0x002A, 0x000B, 0x000A, 0x0001,
  
  /******** Tree 42 DIALOG33 ****************************************************/
        -1, EPROGREF        ,       17, G_BOX             ,   /* Object 0  */
  NONE, SHADOWED, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0022, 0x0011,
  EPROG1          ,       -1,       -1, G_BOXTEXT         ,   /* Object 1 EPROGREF */
  NONE, SELECTED, (LONG)&rs_tedinfo[521],
  0x0000, 0x0000, 0x0022, 0x0001,
  EPROG2          ,       -1,       -1, G_BOXTEXT         ,   /* Object 2 EPROG1 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[522],
  0x0000, 0x0001, 0x0022, 0x0001,
  EPROG3          ,       -1,       -1, G_BOXTEXT         ,   /* Object 3 EPROG2 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[523],
  0x0000, 0x0002, 0x0022, 0x0001,
  EPROG4          ,       -1,       -1, G_BOXTEXT         ,   /* Object 4 EPROG3 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[524],
  0x0000, 0x0003, 0x0022, 0x0001,
  EPROG5          ,       -1,       -1, G_BOXTEXT         ,   /* Object 5 EPROG4 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[525],
  0x0000, 0x0004, 0x0022, 0x0001,
  EPROG6          ,       -1,       -1, G_BOXTEXT         ,   /* Object 6 EPROG5 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[526],
  0x0000, 0x0005, 0x0022, 0x0001,
  EPROG7          ,       -1,       -1, G_BOXTEXT         ,   /* Object 7 EPROG6 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[527],
  0x0000, 0x0006, 0x0022, 0x0001,
  EPROG8          ,       -1,       -1, G_BOXTEXT         ,   /* Object 8 EPROG7 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[528],
  0x0000, 0x0007, 0x0022, 0x0001,
  EPROG9          ,       -1,       -1, G_BOXTEXT         ,   /* Object 9 EPROG8 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[529],
  0x0000, 0x0008, 0x0022, 0x0001,
  EPROG10         ,       -1,       -1, G_BOXTEXT         ,   /* Object 10 EPROG9 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[530],
  0x0000, 0x0009, 0x0022, 0x0001,
  EPROG11         ,       -1,       -1, G_BOXTEXT         ,   /* Object 11 EPROG10 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[531],
  0x0000, 0x000A, 0x0022, 0x0001,
  EPROG12         ,       -1,       -1, G_BOXTEXT         ,   /* Object 12 EPROG11 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[532],
  0x0000, 0x000B, 0x0022, 0x0001,
  EPROG13         ,       -1,       -1, G_BOXTEXT         ,   /* Object 13 EPROG12 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[533],
  0x0000, 0x000C, 0x0022, 0x0001,
  EPROG14         ,       -1,       -1, G_BOXTEXT         ,   /* Object 14 EPROG13 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[534],
  0x0000, 0x000D, 0x0022, 0x0001,
  EPROG15         ,       -1,       -1, G_BOXTEXT         ,   /* Object 15 EPROG14 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[535],
  0x0000, 0x000E, 0x0022, 0x0001,
        17,       -1,       -1, G_BOXTEXT         ,   /* Object 16 EPROG15 */
  SELECTABLE|EXIT, NORMAL, (LONG)&rs_tedinfo[536],
  0x0000, 0x000F, 0x0022, 0x0001,
         0,       -1,       -1, G_TEXT            ,   /* Object 17  */
  LASTOB, NORMAL, (LONG)&rs_tedinfo[537],
  0x0000, 0x0010, 0x0022, 0x0001,
  
  /******** Tree 43 DIALOG34 ****************************************************/
        -1, ENAME           , EPROGOK         , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004C, 0x0007,
         2,       -1,       -1, G_FTEXT           ,   /* Object 1 ENAME */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[538],
  0x0003, 0x0001, 0x0024, 0x0001,
  EREDIR          ,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"Redirect:",
  0x003E, 0x0001, 0x0009, 0x0001,
         4,       -1,       -1, G_BOX             |0x0300,   /* Object 3 EREDIR */
  SELECTABLE, NORMAL, (LONG)0x00FF1101L,
  0x0048, 0x0001, 0x0002, 0x0001,
  EPFAD           ,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"Pfad:",
  0x0003, 0x0002, 0x0005, 0x0001,
         6,       -1,       -1, G_FTEXT           ,   /* Object 5 EPFAD */
  EDITABLE|TOUCHEXIT, NORMAL, (LONG)&rs_tedinfo[539],
  0x0009, 0x0002, 0x0041, 0x0001,
  ECMD            ,       -1,       -1, G_STRING          ,   /* Object 6  */
  NONE, NORMAL, (LONG)"Cmd:",
  0x0004, 0x0003, 0x0004, 0x0001,
  EPROGOK         ,       -1,       -1, G_FTEXT           ,   /* Object 7 ECMD */
  EDITABLE, NORMAL, (LONG)&rs_tedinfo[540],
  0x0009, 0x0003, 0x0041, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 8 EPROGOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"OK",
  0x0040, 0x0005, 0x000A, 0x0001,
  
  /******** Tree 44 DIALOG90 ****************************************************/
        -1,        1, INEEDHLP        , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x0043, 0x0009,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"Avalon can help you to install itself... Do you want to do so?",
  0x0002, 0x0001, 0x003E, 0x0001,
         3,       -1,       -1, G_IBOX            |0x0200,   /* Object 2  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0002, 0x0043, 0x0002,
  INOHELP         ,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"Avalon kann Dir bei der Installation helfen... Willst Du das?",
  0x0002, 0x0004, 0x003D, 0x0001,
  INEEDHLP        ,       -1,       -1, G_BUTTON          ,   /* Object 4 INOHELP */
  SELECTABLE|EXIT, NORMAL, (LONG)"No",
  0x0027, 0x0007, 0x000A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 5 INEEDHLP */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"Yes, help me",
  0x0033, 0x0007, 0x000E, 0x0001,
  
  /******** Tree 45 DIALOG91 ****************************************************/
        -1,        1, IOK             , G_BOX             ,   /* Object 0  */
  NONE, OUTLINED, (LONG)0x00021100L,
  0x0000, 0x0000, 0x004F, 0x0012,
         2,       -1,       -1, G_STRING          ,   /* Object 1  */
  NONE, NORMAL, (LONG)"And now you are on your own:",
  0x0002, 0x0001, 0x001C, 0x0001,
         3,       -1,       -1, G_STRING          ,   /* Object 2  */
  NONE, NORMAL, (LONG)"1. Select your netmail in \042Special->Netmail\042. Only neccessary when it is",
  0x0003, 0x0002, 0x0048, 0x0001,
         4,       -1,       -1, G_STRING          ,   /* Object 3  */
  NONE, NORMAL, (LONG)"not defined in BINKLEY.CFG or when Semper is used.",
  0x0006, 0x0003, 0x0032, 0x0001,
         5,       -1,       -1, G_STRING          ,   /* Object 4  */
  NONE, NORMAL, (LONG)"2. Check your session setup. Perhaps you want to change something.",
  0x0003, 0x0004, 0x0042, 0x0001,
         6,       -1,       -1, G_STRING          ,   /* Object 5  */
  NONE, NORMAL, (LONG)"3. SAVE YOUR SETUP (Click on \042Save\042)!!!",
  0x0003, 0x0005, 0x0027, 0x0001,
         7,       -1,       -1, G_IBOX            |0x0200,   /* Object 6  */
  NONE, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0006, 0x004F, 0x0802,
         8,       -1,       -1, G_STRING          ,   /* Object 7  */
  NONE, NORMAL, (LONG)"Und nun bist Du an der Reihe:",
  0x0002, 0x0808, 0x001D, 0x0001,
         9,       -1,       -1, G_STRING          ,   /* Object 8  */
  NONE, NORMAL, (LONG)"1. Bitte gebe Deine Netmail in \042Special->Netmail\042 an. Dieses ist nur dann",
  0x0003, 0x0809, 0x0049, 0x0001,
        10,       -1,       -1, G_STRING          ,   /* Object 9  */
  NONE, NORMAL, (LONG)"notwendig, wenn sie nicht im BINKLEY.CFG definiert wurde, oder Semper",
  0x0006, 0x080A, 0x0045, 0x0001,
        11,       -1,       -1, G_STRING          ,   /* Object 10  */
  NONE, NORMAL, (LONG)"als Mailer benutzt wird.",
  0x0006, 0x080B, 0x0018, 0x0001,
        12,       -1,       -1, G_STRING          ,   /* Object 11  */
  NONE, NORMAL, (LONG)"2. \232berpr\201fe das Session-Setup. Eventuell m\224chtest Du hier noch etwas",
  0x0003, 0x080C, 0x0045, 0x0001,
        13,       -1,       -1, G_STRING          ,   /* Object 12  */
  NONE, NORMAL, (LONG)"ver\204ndern.",
  0x0006, 0x080D, 0x000A, 0x0001,
  IOK             ,       -1,       -1, G_STRING          ,   /* Object 13  */
  NONE, NORMAL, (LONG)"3. SPEICHERE DIE KONFIGURATION (\042Save\042)!!!",
  0x0003, 0x080E, 0x002A, 0x0001,
         0,       -1,       -1, G_BUTTON          ,   /* Object 14 IOK */
  SELECTABLE|DEFAULT|EXIT|LASTOB, NORMAL, (LONG)"  OK  ",
  0x0043, 0x0010, 0x000A, 0x0001,
  
  /******** Tree 46 DIALOG99 ****************************************************/
        -1, ICFSPIC         , ICFSPIC         , G_BOX             ,   /* Object 0  */
  SELECTABLE|EXIT, NORMAL, (LONG)0x00FF1100L,
  0x0000, 0x0000, 0x0008, 0x0004,
         0,       -1,       -1, G_IMAGE           ,   /* Object 1 ICFSPIC */
  SELECTABLE|EXIT|LASTOB, NORMAL, (LONG)&rs_bitblk[8],
  0x0002, 0x0001, 0x0002, 0x0400
};

OBJECT *rs_trindex[] =
{ &rs_object[0],   /* Tree  0 DIALOG1          */
  &rs_object[84],   /* Tree  1 DIALOG2          */
  &rs_object[190],   /* Tree  2 DIALOG3          */
  &rs_object[223],   /* Tree  3 DIALOG5          */
  &rs_object[226],   /* Tree  4 DIALOG6          */
  &rs_object[230],   /* Tree  5 DIALOG7          */
  &rs_object[286],   /* Tree  6 DIALOG8          */
  &rs_object[375],   /* Tree  7 DIALOG9          */
  &rs_object[404],   /* Tree  8 DIALOG10         */
  &rs_object[407],   /* Tree  9 DIALOG11         */
  &rs_object[464],   /* Tree 10 DIALOG12         */
  &rs_object[555],   /* Tree 11 DIALOG13         */
  &rs_object[558],   /* Tree 12 DIALOG14         */
  &rs_object[562],   /* Tree 13 DIALOG15         */
  &rs_object[631],   /* Tree 14 DIALOG16         */
  &rs_object[646],   /* Tree 15 DIALOG17         */
  &rs_object[649],   /* Tree 16 DIALOG18         */
  &rs_object[670],   /* Tree 17 DIALOG71         */
  &rs_object[746],   /* Tree 18 DIALOG81         */
  &rs_object[762],   /* Tree 19 DIALOG31         */
  &rs_object[791],   /* Tree 20 DIALOG72         */
  &rs_object[802],   /* Tree 21 DIALOG21         */
  &rs_object[804],   /* Tree 22 DIALOG82         */
  &rs_object[814],   /* Tree 23 DIALOG19         */
  &rs_object[819],   /* Tree 24 DIALOG61         */
  &rs_object[823],   /* Tree 25 DIALOG20         */
  &rs_object[825],   /* Tree 26 DIALOG51         */
  &rs_object[836],   /* Tree 27 DIALOG22         */
  &rs_object[883],   /* Tree 28 DIALOG23         */
  &rs_object[890],   /* Tree 29 DIALOG24         */
  &rs_object[896],   /* Tree 30 DIALOG52         */
  &rs_object[904],   /* Tree 31 DIALOG25         */
  &rs_object[928],   /* Tree 32 DIALOG26         */
  &rs_object[938],   /* Tree 33 DIALOG27         */
  &rs_object[950],   /* Tree 34 DIALOG28         */
  &rs_object[981],   /* Tree 35 DIALOG9B         */
  &rs_object[1030],   /* Tree 36 DIALO25B         */
  &rs_object[1074],   /* Tree 37 DIALOG29         */
  &rs_object[1077],   /* Tree 38 DIALOG30         */
  &rs_object[1090],   /* Tree 39 DIALOG40         */
  &rs_object[1107],   /* Tree 40 PICTURE0         */
  &rs_object[1112],   /* Tree 41 DIALOG41         */
  &rs_object[1120],   /* Tree 42 DIALOG33         */
  &rs_object[1138],   /* Tree 43 DIALOG34         */
  &rs_object[1147],   /* Tree 44 DIALOG90         */
  &rs_object[1153],   /* Tree 45 DIALOG91         */
  &rs_object[1168]    /* Tree 46 DIALOG99         */
};
