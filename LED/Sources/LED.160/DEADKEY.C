#include "ccport.h"
#include "ledgl.h"

word is_deadkey(word deadkey)
{	word i;
	i=opt.deadkeyflag;
	if (i & 0x3ff)
		if ((i & DKEY_A 			 && deadkey=='\''	) ||
				(i & DKEY_BACKA		 && deadkey=='`'	) ||
				(i & DKEY_SCHLANGE && deadkey=='~'	)	||
				(i & DKEY_DACH		 &&	deadkey=='^'	)	||
				(i & DKEY_SLASH		 && deadkey=='/'	)	||
				(i & DKEY_KREIS		 &&	deadkey==0xf8	)	||
				(i & DKEY_DPNT		 && deadkey==0xb9 )	||
				(i & DKEY_LITERAL	 && deadkey=='\"' )	||
				(i & DKEY_KOMMA		 && deadkey==','	) ||
				(i & DKEY_MASTER	 && deadkey=='|'  ))
			return TRUE;
	return FALSE;
}

word convert_deadkey(word deadkey, word key)
{	if (deadkey=='\'')
	{	switch(key)
		{	case 'e'	:	return 0x82;
			case 'E'	: return 0x90;
			case 'a'	: return 0xa0;
			case 'i'	: return 0xa1;
			case 'o'	:	return 0xa2;
			case 'u'	: return 0xa3;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey=='`')
	{	switch(key)
		{	case 'a'	:	return 0x85;
			case 'e'	: return 0x8a;
			case 'i'	: return 0x8d;
			case 'o'	: return 0x95;
			case 'u'	:	return 0x97;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey=='~')
	{	switch(key)
		{	case 'n'	:	return 0xa4;
			case 'N'	: return 0xa5;
			case 'a'	: return 0xb0;
			case 'o'	: return 0xb1;
			case 'A'	:	return 0xb7;
			case 'O'	:	return 0xb8;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey=='^')
	{	switch(key)
		{	case 'a'	:	return 0x83;
			case 'e'	: return 0x88;
			case 'i'	: return 0x8c;
			case 'o'	: return 0x93;
			case 'u'	:	return 0x96;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey=='/')
	{	switch(key)
		{	case 'O'	:	return 0xb2;
			case 'o'	: return 0xb3;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey==0xf8)
	{	switch(key)
		{	case 'a'	:	return 0x86;
			case 'A'	: return 0x8f;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey==0xb9 || deadkey=='\"')
	{	switch(key)
		{	case 'u'	:	return 0x81;
			case 'a'	:	return 0x84;
			case 'e'	: return 0x89;
			case 'i'	: return 0x8b;
			case 'A'	: return 0x8e;
			case 'o'	: return 0x94;
			case 'y'	: return 0x98;
			case 'O'	: return 0x99;
			case 'U'	: return 0x9a;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey==',')
	{	switch(key)
		{	case 'c'	:	return 0x87;
			case 'C'	: return 0x80;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	else if (deadkey=='|')
	{	switch(key)
		{ case 'a'	: return 0x86;
			case 'b'	: return 0xa6;
			case 'c'	: return 0x87;
			case 'd'	: return 0x9b;
			case 'e'	: return 0x91;
			case 'f'	: return 0x9f;
			case 'g'	: return 0xf8;
			case 'h'	: return 0xf9;
			case 'i'	: return 0xc0;
			case 'o'	: return 0xb4;
			case 'p'	: return 0xa7;
			case 'r'	: return 0xbe;
			case 's'	: return 0xbd;
			case 't'	: return 0xbf;
			case 'y'	: return 0x9d;
			case 'A'	: return 0x8f;
			case 'C'	: return 0x80;
			case 'E'	: return 0x92;
			case 'I'	: return 0xc1;
			case 'O'	: return 0xb5;
			case 'P'	: return 0xbc;
			case 'T'	: return 0xbb;
			case 'V'	: return 0xdd;
			case 'W'	: return 0xa9;
			case 'X'	: return 0xaa;
			case 'Y'	: return 0xf4;
			case 'Z'	: return 0xf5;
			case '?'	: return 0xa8;
			case '!'	: return 0xad;
			case '<'	: return 0xae;
			case '>'	: return 0xaf;
			case '~'	: return 0xf7;
			case '/'	: return 0xf6;
			case '$'	: return 0x9c;
			case '2'	: return 0xab;
			case '4'	: return 0xac;
			case '8'	: return 0xdf;
			case ':'	: return 0xb9;
			case ' '	: return deadkey;
			default		: return key;
		}
	}
	return key;
}
