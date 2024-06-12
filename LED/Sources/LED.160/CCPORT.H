/*
 *		Turbo C 2.0
 *		verkÅrztes altes ccport.h
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
/* #include <time.h> */
#include <ctype.h>
#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <flydial.h>
#include <portab.h>

#define   MAX_STR   128						 /* fÅr den LED, >=80! */
#define   REG       register       /* Verwendet man Makros fÅr   */
#define   EXTERN    extern         /* die Typ-Bezeichnung, muû man    */
#define   LOCAL     auto           /* auch die Speicherklassen   */
#define   MLOCAL    static         /* groûschreiben kînnen, alles     */
                                   /* andere sieht nicht aus     */
/*
 * Konstanten
 */

#define   EOF       (-1)
#define   TRUE      1
#define   FALSE     0
#define   ON        1
#define   OFF       0
#define   YES       1
#define   NO        0
#define   FOREVER   for(;;)

#define max(a,b)	((a)>(b)?(a):(b))
#define min(a,b)	((a)<(b)?(a):(b))
