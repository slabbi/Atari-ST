#include <casio_sf.h>
#include <scancode.h>

int trans_key1(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_A : cwahl = EMPFE;		break;
											case ALT_B : cwahl = EMPFK;		break;
											case ALT_C : cwahl = EMPFASCI;break;
											case ALT_D : cwahl = SENDALL;	break;
											case ALT_E : cwahl = SENDTEL;	break;
											case ALT_F : cwahl = SENDADR;	break;
											case ALT_G : cwahl = SENDTERM;break;
											case ALT_H : cwahl = SENDBUS;	break;
											case ALT_I : cwahl = SENDMEMO;break;
											case ALT_J : cwahl = SENDCAL;	break;
											case ALT_K : cwahl = SENDREM;	break;
											case ALT_L : cwahl = SENDREM2;break;
											case ALT_Q : cwahl = EXITPRG;	break;
											case ALT_R : cwahl = REGISTER;break;
											case ALT_S : cwahl = CONFIGUR;break;
										}; break;
			}
		return(cwahl);
	}
int trans_key2(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = LINEEXIT;	break;
											case ALT_R : cwahl = LINERUN;		break;
											case ALT_S : cwahl = LINESEND;	break;
											case ALT_W : cwahl = LINECONT;	break;
											case ALT_F : cwahl = LINELOOK;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key3(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = SEXIT;	break;
											case ALT_S : cwahl = SSEND;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key4(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = MEXIT;	break;
											case ALT_S : cwahl = MSEND;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key5(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = BEXIT;	break;
											case ALT_S : cwahl = BSEND;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key6(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = AEXIT;	break;
											case ALT_S : cwahl = ASEND;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key7(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = CEXIT;	break;
											case ALT_S : cwahl = CSEND;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key8(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = REXIT;	break;
											case ALT_S : cwahl = RSEND;	break;
										}; break;
			}
		return(cwahl);
	}
int trans_key9(int s,int key)
	{	int cwahl=-1;
		switch(s)
			{	case 8	:	switch(key)
										{	case ALT_Q : cwahl = REM2EXIT;	break;
											case ALT_S : cwahl = REM2SEND;	break;
										}; break;
			}
		return(cwahl);
	}
