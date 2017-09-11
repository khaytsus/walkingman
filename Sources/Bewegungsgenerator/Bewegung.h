/* JPK13.04.92-24.04.92+05.05.92+22.05.92 */

#ifndef BEWEGUNG_H
#define BEWEGUNG_H

#ifndef LISTE_H
#include "Listenverwaltung/Liste.h"
#endif
#ifndef EXTERN_H
#include "Extern.h"
#endif
#ifndef DARSTELLUNG_H
#include "Darstellung/Darstellung.h"
#endif
#ifndef ANALYSE_H
#include "Bildschirmanalyse/Analyse.h"
#endif
#ifndef ZUFALLSGENERATOR_H
#include "Zufallsgenerator/Zufallsgenerator.h"
#endif


	/***** Strukturen *****/

#ifdef BEWEGUNG_PRIVAT
struct Mannliste
	{
	struct Liste maennchen;
	};
#endif
struct Manneintrag
	{
	struct Liste_Element kopf;

	enum {
		gehen, fallen, stehen, klettern, sitzen, unsichtbar
		} Bewegungsart;
	WORD
		speedX,
		speedY,
		posX,
		posY;
	UWORD bild;

	/* privat für Bewegungsgenerator */
		unsigned verschwindet: 1;
		UWORD Warten;
		/* Bewegungsgenerator Ende */

	/* privat für Bildschirmanalyse */
		struct Screeneintrag *grund;
		/* Bildschirmanalyse Ende */

	/* privat für Darstellung */
		unsigned nacht: 1;
		WORD helligkeit;
		struct SimpleSprite sprite;
		struct Disp_Grafik grafik;
		/* Darstellung Ende */
	};


	/***** Konstanten *****/

#define MANN_HOEHE	10
#define MANN_GROESSE	9
#define MANN_BREITE	5
#define MANN_HAND		4
#define KOMMA			4
#define GRAVITATION	1
#define MAXSPEED		(10<<KOMMA)
#define UEBERGANG		3

#define SEK60_0	1500
#define SEK30_0	750
#define SEK15_0	375
#define SEK10_0	250
#define SEK2_0		50
#define SEK1_0		25
#define SEK0_5		13


	/***** Funktionen *****/

extern UBYTE eigene_Sprites;

extern void
	Bewegung_Init(),
	Bewegung_Clear(),
	Bewegung_Alt(),
	Bewegung_Schritt(),
	Bewegung_Halbschritt();
extern char
	Bewegung_Neu();

#endif

