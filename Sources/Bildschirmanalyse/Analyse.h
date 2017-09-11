/* JPK13.04.92-26.04.92+05.05.92+22.05.92 */

#ifndef ANALYSE_H
#define ANALYSE_H

#ifndef LISTE_H
#include "Listenverwaltung/Liste.h"
#endif
#ifndef BEWEGUNG_H
#include "Bewegungsgenerator/Bewegung.h"
#endif
#ifndef EXTERN_H
#include "Extern.h"
#endif


	/***** Konstanten *****/

#define ANALYSE_MAXDEPTH 	6
#define ANALYSE_MAXLUMEN	45


	/***** Strukturen *****/

#ifdef ANALYSE_PRIVAT
struct Screenliste
	{
	struct Liste screens;
	};
#endif
struct Screeneintrag
	{
	struct Liste_Element kopf;
	struct Screen *original;
	unsigned begehbar:	1;
	unsigned benutzbar:	1;
	unsigned pseudoscreen:	1;
	UBYTE
		tiefe,
		nachschauen;
	PLANEPTR
		map[ANALYSE_MAXDEPTH];
	BYTE
		helle[1<<ANALYSE_MAXDEPTH];
	UWORD origMode;
	WORD
		breiteinbytes,
		origX, origY,			/* Originalposition	*/
		origB, origH,			/* Originalausmaße	*/
		normX, normY,				/* normalisierte Position			*/
		normB, normH,				/* normalisierte Ausmaße			*/
		sichtbar;					/* normalisierte sichtbare Höhe	*/
	UBYTE
		densX,
		densY;
	};


	/***** Funktionen *****/

extern void
	Analyse_Clear(),
	Analyse_Init(),
	Analyse_Neu(),
	Analyse_Alt(),
	Analyse_Screens(),
	Analyse_Grundlage(),
	Analyse_Nachttest();
extern char
	Analyse_Horiz(),
	Analyse_Vert();

#endif

