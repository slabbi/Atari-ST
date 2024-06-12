#include "winddefs.h"

#define KEYS_INCLUDED	0

extern word gem_statusline;
extern word	iconify_possible;
extern word FROM_LINE;
extern word TO_LINE;
extern word TOPIC_LINE;
extern word TOPMSG_LINE;

extern byte fm_sound_dta[];

extern byte autor1[];
extern byte autor2[];
extern byte autor3[];

extern byte chrs_line[];
extern byte unreg[];
extern byte pidserial[];

extern word s_cutbuff, e_cutbuff;

extern byte prn_table[256][PRN_TAB];
extern word	prn_options[64];
extern byte prn_divs[32][PRN_TAB];
#define OPT_LEFTMARGIN	0
#define DIV_INIT				0

#define LATIN1S				0
#define LATIN1				1
#define ATARIST				2
extern byte convert_table[3][256];

#define ISO_SWEDISH		0
#define ISO_SPANISH		1
#define ISO_FRENSH		2
#define ISO_GERMAN		3
#define ISO_UK				4
#define ISO_PORTU			5
#define ISO_NORWEG		6
#define ISO_ITALIAN		7
extern byte iso_convert_table[8][12];
