#ifndef __WINDEFS__

#define __WINDEFS__

#define MIN_LINES_EDIT_WIND	(7+1)

#define MAXCOL_LISTWIND	127

#define HDRCOL			6
#define MSGCOL			0

#define EFF_FETT		1
#define EFF_HELL		2
#define EFF_KURSIV	4
#define EFF_UNTER		8
#define EFF_HOHL		16
#define EFF_SCHATT	32
#define EFF_INVERS	64

#define REV(x, y, w, h)     rcopy(x, y, w, h, x, y, 12)
#define RWHITE(x, y, w, h)  rcopy(x, y, w, h, x, y, 0)
#define RBLACK(x, y, w, h)  rcopy(x, y, w, h, x, y, 15)

#define CH_INSMD		1

#define PRN_TAB			12

#define HYPER_LED				0
#define	HYPER_SEARCH		1
#define	HYPER_LED_KEYS	2

#endif