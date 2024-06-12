#include "ccport.h"
#include "find.rh" 
#include "fsame.rh"
#include "ledgl.h"
#include "ledfc.h"
#include "ledscan.h"
#include "language.h"

#define MAXPAT  60 	                  /* maximale Musterlaenge */

typedef struct pattern_t
{ byte	p_c;              	                    /* das Zeichen */
  word 	p_index;     			        /* dessen Position im Muster */
} PAT;

typedef word shifttab_t[256];					     /* Shifttabellentyp */

static shifttab_t Td1;                    /* eine Shifttabelle */
static PAT pattern[MAXPAT];           /* ein geordnetes Muster */
static word Plen;                            	 /* Musterlaenge */

static word Cfreq[256] =               /* Zeichenhaeufigkeiten */
{ 0,0,0,0,0,0,0,0,0,0,1970,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,3,0,0,0,0,0,14964,75,0,0,1455,0,0,0,1,1,0,0,1577,
  100,680,727,126,761,499,296,233,191,165,144,136,131,
  235,221,0,0,0,90,0,181,175,54,271,204,142,433,280,
  79,217,108,144,212,85,32,103,2,68,373,151,204,143,
  289,0,0,76,89,47,89,0,0,27,3845,1363,2037,4353,12599,
  771,1892,3814,5730,92,547,1996,1669,7890,1499,314,
  2,5284,4350,4184,3187,453,1166,2,22,660,0,0,1,0,0,
  0,423,0,0,238,0,0,0,0,0,0,0,0,0,13,0,0,0,0,0,190,0,
  0,0,0,3,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  302,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0
};

/* Fuer besonders pingelige Compiler beim qsort()
*/
typedef word (*cmpfunc_t)(const void *, const void *);

/* Vergleichfunktion, basierend auf Zeichenhaeufigkeit
*/

word om_cmp(PAT *p1, PAT *p2)
{ word df = Cfreq[p1->p_c] - Cfreq[p2->p_c];
  return df ? df : (p2->p_index - p1->p_index);
}

/* Preprocessing-Funktion von OM
   Generiert die Shifttabelle und das geordnete Muster.
   In:  das Muster
   Out: Adresse der Shift-Tabelle oder 0 bei Fehler
*/

PAT *init_omsearch(const byte *pstr)
{ word c, nw;             	    /* akt. Zeichen, Wildcard-Zaehler */
  word m;    			                                     /* Zaehler */
  const byte *p;  	        	                      /* Laufzeiger */
  word *shift,       			         /* Zeiger auf die Shifttabelle */
        deltamax=0;            			     /* max. Shift, falls > 0 */
  PAT *pat;              		      /* Zeiger auf geordnetes Muster */

  for(p=pstr; *p; ++p);
  if((m=(word)(p-pstr)) > MAXPAT)
  	return NULL;
  Plen = m; 	                               /* Musterleange Plen */

  /* Initialisiere Shifttabelle mit Plen+1 */
  for(++m, shift=Td1, c=256; --c >=0;) *shift++ = m;

  /* Muster scannen, dabei Shifttabelle anpassen
     und geordnetes Muster aufbauen                            */
  for(nw=0, shift=Td1, p=pstr, pat=pattern; --m, c=*p; ++p)
  { if ( *p != '?' )             /* Zeichen ist kein Wildcard, */
    { pat->p_index = (word) (p-pstr-Plen);     /* wird also uebernommen */
      pat->p_c=c;
      ++pat;
      shift[c]=m;
    }
    else                               /* Zeichen ist Wildcard */
    { deltamax=m;                   /* und begrenzt die Shifts */
      ++nw;
    }
  }
  pat->p_c = 0;
  if ( deltamax )                          /* Shifts begrenzen */
    for(shift=Td1, c=256; --c >= 0; ++shift)
      if(*shift > deltamax) *shift=deltamax;
  /* geordnetes Muster erzeugen */
  qsort(pattern, Plen-nw, sizeof(PAT), (cmpfunc_t)om_cmp);
  return pattern;
}

/* Optimal-Mismatch-Suche nach einem Muster
   In:  Pointer auf den Text, das geordnete Muster sowie die
        Textlaenge, falls bekannt, sondt 0
   Out: Adresse der Fundstelle im Text oder 0
*/

byte *omsearch(const byte *text, const PAT *pat, word tlen)
{ const byte *tx;                             /* Laufvariablen */
  byte c;
  word i;
  const PAT *p;
  const byte *txe;                                 /* Textende */
  word *td1 = Td1; 	                          /* Shift-Tabelle */

  if (text==NULL)
  	return NULL;
  if ((c = pat->p_c) == 0 )
  	return (byte *)text;

  i=pat->p_index; ++pat;
  if (tlen > 0)                 /* Textlaenge wurde uebergeben */
    txe = text + tlen;
  else                             /* ... oder wird berechnet. */
  { for(txe = text; *txe++;);
    --txe;
  }
  tx=text+Plen;
  while (tx <= txe)
  { while (c != *(tx+i))                    /* Suche 1. Zeichen */
    { tx += td1[*tx];
      if(tx >txe)
      	return NULL;
    }
    for (p = pat; p->p_c; ++p)         /* Suche weitere Zeichen */
      if (p->p_c != *(tx + p->p_index))
        goto mismatch;                 /* Mismatch aufgetreten */
    return (byte *)(tx-Plen);                       /* Match ! */
mismatch:
    tx += td1[*tx]; }      /* Shift zur naechsten Textposition */
  return NULL;
}



word match(byte *what, byte *how, byte **where)
{	byte *s, *p;
	word ls, lp;
	word is=1, ip=1;

	*where=what;
	ls=(word)strlen(what);
	lp=(word)strlen(how);
	if (ls==0 || lp==0)
		return(0);
	s=what;
	p=how;
/*	printf("Vergleiche: >%s< (%d) mit >%s< (%d):\n",s,ls,p,lp);*/
	while(1)
	{
		if (*p=='?')
		{
			if (is>ls)
				return(0);
			++is;
			++s;
			++ip;
			++p;
			if (ip>lp)
			{
				if (is>ls)
					return(1);
				else
					return(0);
			}
			continue;
		}
		if (*p=='*')
		{
			if (ip == lp)
				return(1);
			++ip;
			++p;
			if (is>ls)
				continue;
			do
			{
				if (match(s,p,where))
					return(1);
				++is;
				++s;
			} while (is<=ls);
			return(0);
		}
		if ((is>ls) || (*s++ != *p++))
			return(0);
		++ip;
		if ((++is>ls) && (ip>lp))
			return(1);
	}
}

PAT *pattabx;

void dia_find(long win)
{	word ext_btn;
	byte d_msg[5];

	if (wi[win].aktmsg>0)
	{
		sprintf(d_msg,"%-4.4d",wi[win].aktmsg);
		set_dtext(find_tree,FIMSGNR,d_msg,FALSE,4);
		if (wi[win].ramhdr)
			set_flag(find_tree,FIINFO,HIDETREE,FALSE);
		else
			del_flag(find_tree,FIINFO,HIDETREE,FALSE);

		if (wi[win].aktmsg==wi[win].lastmsg)
			set_button(find_tree,FIBACK,FALSE);
		else
			del_button(find_tree,FIBACK,FALSE);

		draw_dialog(find_tree, &find_dial);
		ext_btn=Form_do(&find_dial, FIFROM);
		del_state(find_tree,ext_btn,SELECTED,FALSE);
		del_dialog(&find_dial);
		if (ext_btn==FIOK)
			find_same(win,FALSE);
	}
}

void find_same(long win,word nextmsg_fl)
{	HDR thdr;
	FILE *fp;
	word lnr;
	word aktmsg;
	word found, foundnr, msgnr;
	word backward, inside_fl, usequantor;
	word fromfl, tofl, subjfl, textfl;
	word first_msg, last_msg, stat;
	byte *cp;
	byte d_msg[5];
	byte from[40], to[40], subj[80], text[MAX_STR], search[MAX_STR];
	byte patto[40], patfrom[40], patsubj[80], pattext[MAX_STR];

	foundnr = 0;
	found =
	backward =
	inside_fl =
	usequantor = FALSE;


	if (wi[win].aktmsg>0 || wi[win].listwind==WI_LIST_MSGS)
	{	while (Cconis())
			Cnecin();
			
		if (req_button(find_tree,FIBACK))		backward=TRUE;
		if (req_button(find_tree,FIINSIDE))	inside_fl=TRUE;	
		if (req_button(find_tree,FIALL))		usequantor=TRUE;	

		msgnr=
		aktmsg=wi[win].aktmsg;

		get_dedit(find_tree,FIFROM,from);
		get_dedit(find_tree,FITO,to);
		get_dedit(find_tree,FISUBJ,subj);
		get_dedit(find_tree,FITEXT,text);
			
		if (strlen(from)+strlen(to)+strlen(subj)+strlen(text)>0L)
		{	
			sprintf(patfrom,"*%s*",from);
			sprintf(patto,"*%s*",to);
			sprintf(patsubj,"*%s*",subj);

			if (!usequantor)
				strcpy(pattext,text);
			else
				sprintf(pattext,"*%s*",text);

			strupr(patfrom);
			strupr(patto);
			strupr(patsubj);
			strupr(pattext);

			fp=wi[win].hdrfp;
			if (fp==NULL && wi[win].listwind==WI_LIST_MSGS)
			{	fp=wi[win].hdrfp2;
				if (*text!=EOS)
				{	LangAlert(MSG_CANNOTSEARCH);
					*text=EOS;
				}
			}

			if (nextmsg_fl && !inside_fl)
			{	if (backward)
					msgnr--;
				else
					msgnr++;
			}	
			biene();
			sprintf(d_msg,"%-4.4d",msgnr);
			set_dtext(fsame_tree,FSMSGNR,d_msg,FALSE,4);
			draw_dialog(fsame_tree, &fsame_dial);
			if (*from==EOS)	fromfl=TRUE;
			if (*to==EOS)		tofl=TRUE;
			if (*subj==EOS)	subjfl=TRUE;
			if (*text==EOS)	textfl=TRUE;
			if (!inside_fl)	
			{	last_msg=wi[win].lastmsg;
				first_msg=wi[win].firstmsg;
			}
			else
			{	last_msg=first_msg=msgnr;
			}

			if (!usequantor)
				if (*text)
					pattabx=init_omsearch(pattext);

			while (msgnr<=last_msg && msgnr>=first_msg)
			{	sprintf(d_msg,"%-4.4d",msgnr);
				set_dtext(fsame_tree,FSMSGNR,d_msg,TRUE,4);

				hdrseek(win,fp,(msgnr-1),SEEK_SET);
				hdrread(win,&thdr,1,fp);

				if (!(thdr.wFlags & F_DELETED))
				{	
					if (*from)
					{	strcpy(search,thdr.sFrom);
						strupr(search);
						cp=search;

						fromfl=FALSE;
						if (match(cp,patfrom,&cp))
							fromfl=TRUE;
					}

					if (*to)
					{
						strcpy(search,thdr.sTo);
						strupr(search);
						cp=search;

						tofl=FALSE;
						if (match(cp,patto,&cp))
							tofl=TRUE;
					}

					if (*subj)
					{
						strcpy(search,thdr.sSubject);
						strupr(search);
						cp=search;

						subjfl=FALSE;
						if (match(cp,patsubj,&cp))
							subjfl=TRUE;
					}

					if (*text && fromfl && tofl && subjfl)
					{
						textfl=FALSE;
						select_msg(win,msgnr,FALSE,FALSE);
						lnr=0;
						while (wi[win].lines[lnr]!=NULL)
						{	cp=wi[win].lines[lnr];
							strupr(cp);

							if (!usequantor)
							{	if (omsearch(cp,pattabx,(word)strlen(cp)))
								{	textfl=TRUE;
									break;
								}
							}
							else
							{	if (match(cp,pattext,&cp))
								{	textfl=TRUE;
									break;
								}
							}
							lnr++;
						}
					}
					
					if (fromfl && tofl && subjfl && textfl)
					{	found=TRUE;
						foundnr=msgnr;
						break;
					}	
				}
				if (Cconis())
				{	while (Cconis())
						Cnecin();
					throw_evntaway();
					foundnr=aktmsg;
					break;
				}
				
				if (backward)
					msgnr--;
				else
					msgnr++;
			}
			mouse_normal();
			del_dialog(&fsame_dial);
			if (!found && found!=aktmsg)
			{
				if (backward)
					del_button(find_tree,FIBACK,FALSE);
				else
					set_button(find_tree,FIBACK,FALSE);
			}
			if (found || found==aktmsg || *text)
			{
				wi[win].oldmsg=0;
				if (foundnr==0)
					foundnr=aktmsg;

				if (!wi[win].listwind)
				{	stat=select_msg(win,foundnr,FALSE,TRUE);
					set_comstatus(win,foundnr);
					if (*text && textfl && wi[win].lines[lnr]!=NULL)
						wi[win].topline=lnr;
					if (wi[win].topline==0)
						wi[win].topline=1;
					new_status(win,stat);
					wisetfiles(win,TRUE /*found*/,NO_CHANGE);
/*					chk_status(win,found); */
					need_update_msglistwindow(win,RETURN);
				}
				else
				{	wi[win].aktmsg=foundnr;
					need_update_msglistwindow(win,RETURN);
				}
			}
		}
	}
}
