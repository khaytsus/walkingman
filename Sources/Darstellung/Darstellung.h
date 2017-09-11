/* JPK13.04.92-23.04.92+05.05.92 */

#ifndef DARSTELLUNG_H
#define DARSTELLUNG_H


	/***** private Strukturen *****/

struct Disp_Grafik
	{
	UWORD map[816];
	};

#ifdef DISP_PRIVAT
struct Disp_Bild
	{
	WORD	MapBeginn[2];	/* Beginn einer Grafik im Ablauf, auch		*/
								/*		falls der Hintergrund zu dunkel ist	*/
	UBYTE	weiter;			/* Nr. des nächsten Bildes */
	UBYTE	alternativ;		/* ggf. anders zu wählendes Bild */
	BYTE
		spotX,			/* Verschiebung des Sprites zum Auflagepunkt */
		spotY;
	};
#endif


#ifndef BEWEGUNG_H
#include "Bewegungsgenerator/Bewegung.h"
#endif

#define DISP_UNSICHTBAR	30
#define DISP_STEHEND		0
#define DISP_SITZEND		1
#define DISP_FALLEND		2
#define DISP_RGEHEND		3
#define DISP_LGEHEND		4
#define DISP_LKLETTERND	5
#define DISP_RKLETTERND	6
#define DISP_JONGLEUR	31


extern UWORD Mann_Statisch[];
extern struct Disp_Grafik Mann_Grafik;

extern void
	Disp_Init(),
	Disp_Clear(),
	Disp_Alt(),
	Disp_Schritt();
extern char
	Disp_Neu();

#endif

