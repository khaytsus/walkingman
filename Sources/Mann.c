/* JPK13.04.92-30.04.92+05.05.92+22.05.92 */

#ifndef EXTERN_H
#include "Extern.h"
#endif
#ifndef BILDSCHIRMANALYSE/ANALYSE_H
#include "Bildschirmanalyse/Analyse.h"
#endif
#ifndef LISTENVERWALTUNG/LISTE_H
#include "Listenverwaltung/Liste.h"
#endif
#ifndef BEWEGUNGSGENERATOR/BEWEGUNG_H
#include "Bewegungsgenerator/Bewegung.h"
#endif
#ifndef DARSTELLUNG/DARSTELLUNG_H
#include "Darstellung/Darstellung.h"
#endif


	/***** globale Variablen *****/

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

struct Window *MyWindow;

struct Flags Flags;


	/***** statische Daten *****/

UBYTE Titel[] = (UBYTE *)
	"Wandersmann - ©Jan P.Katz/JPK MCMXCII. Version 22.05.92";

struct NewWindow MyNewWindow =
	{
	78,0,147,10, 0,1, 0L,WINDOWDRAG|WINDOWDEPTH|SMART_REFRESH,
	0L,0L, Titel, 0L,0L, 100,10, 100,10, WBENCHSCREEN
	};

struct IntuiText
	QuitText =	{	0,1, JAM2, 20,1, 0L, (UBYTE *)"Quit", 0L	},
	BreakText =	{	0,1, JAM2, 20,1, 0L, (UBYTE *)"Abbruch", 0L	},
	NeuText =	{	0,1, JAM2, 20,1, 0L, (UBYTE *)"Neues Männchen",0L	},
	AltText =	{	0,1, JAM2, 20,1, 0L, (UBYTE *)"Männchen entfernen",0L	};

struct MenuItem
	NeuItem = {
		0L, 0,0,176,10, ITEMTEXT|HIGHCOMP|CHECKIT|ITEMENABLED,
		0L, (APTR)&NeuText,0L, '\0', 0L,0	},
	AltItem = {
		&NeuItem, 0,10,176,10, ITEMTEXT|HIGHCOMP|CHECKIT|ITEMENABLED,
		0L, (APTR)&AltText,0L, '\0', 0L,0	},
	BreakItem = {
		&AltItem, 0,25,176,10, ITEMTEXT|HIGHCOMP|COMMSEQ|CHECKIT|ITEMENABLED,
		0L, (APTR)&BreakText,0L, 'B', 0L,0	},
	QuitItem = {
		&BreakItem, 0,35,176,10, ITEMTEXT|HIGHCOMP|COMMSEQ|CHECKIT|ITEMENABLED,
		0L, (APTR)&QuitText,0L, 'Q', 0L,0	};

struct Menu MyMenu = {
	0L, 8,0,96,10, MENUENABLED, (BYTE *)Titel, &QuitItem,
	0,0,0,0	};



	/***** Funktionen *****/

char Fehler()
	{
	DisplayBeep(0L);
	DisplayBeep(0L);
	return(0);
	}

char StartUp(mannenzahl)
WORD mannenzahl;
	{
	if(!(IntuitionBase = (struct IntuitionBase *)
		OpenLibrary("intuition.library", 33L)))
		{
		printf("Konnte Intuition nicht öffnen.\n");
		return(1);
		}
	if(!(GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 33L)))
		{
		printf("Konnte Graphics nicht öffnen.\n");
		return(1);
		}
	if(!(MyWindow = OpenWindow(&MyNewWindow)))
		{
		printf("Kann das Fenster nicht öffnen.\n");
		return(1);
		}
	SetWindowTitles(MyWindow, -1L, Titel);
	SetMenuStrip(MyWindow, &MyMenu);
	Bewegung_Init(mannenzahl);
	return(0);
	}


void CleanUp()
	{
	Bewegung_Clear();
	if(MyWindow)
		{
		ClearMenuStrip(MyWindow);
		CloseWindow(MyWindow);
		}
	if(GfxBase) CloseLibrary(GfxBase);
	if(IntuitionBase) CloseLibrary(IntuitionBase);
	}


void main(argc, argv)
int argc;
char *argv[];
	{
	WORD mannenzahl = 1;
	if(argc>1)
		{
		char *zeichen = argv[1];
		mannenzahl = 0;
		while(*zeichen>='0' && *zeichen<='9')
			{
			mannenzahl = mannenzahl*10 + *zeichen - '0';
			zeichen++;
			}
		if(*zeichen || argc>2)
			{
			printf(
"Aufruf:\n"
"   '%s [Anzahl der Männchen]'\n"
"Bei fehlender oder fehlerhafter Anzahl der Männchen wird 1 gesetzt\n",
				argv[0]);
			mannenzahl = 1;
			}
		}


	if(!StartUp(mannenzahl))
		{
		Flags.quit = 0;
		Flags.finale = 0;
		while(!Flags.quit)
			{
			WaitTOF();
			Bewegung_Schritt();
			WaitTOF();
			Bewegung_Halbschritt();
			if(NeuItem.Flags&CHECKED)
				{
				NeuItem.Flags &= ~CHECKED;
				Bewegung_Neu();
				}
			if(AltItem.Flags&CHECKED)
				{
				AltItem.Flags &= ~CHECKED;
				Bewegung_Alt((WORD)1);
				}
			if(QuitItem.Flags&CHECKED)
				{
				if(!Flags.finale)
					{
					Flags.finale = 1;
					Bewegung_Alt((WORD)-1);
					}
				}
			if(BreakItem.Flags&CHECKED)
				Flags.quit = 1;
			}
		}
	CleanUp();
	}

