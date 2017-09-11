/* JPK13.04.92-30.04.92+22.05.92 */

#define ANALYSE_PRIVAT
#include "Bildschirmanalyse/Analyse.h"
#undef ANALYSE_PRIVAT

#ifndef EXTERN_H
#include "Extern.h"
#endif
#ifndef ANALYSE_H
#include "Bildschirmanalyse/Analyse.h"
#endif
#ifndef BEWEGUNG_H
#include "Bewegungsgenerator/Bewegung.h"
#endif
#ifndef ZUFALLSGENERATOR_H
#include "Zufallsgenerator/Zufallsgenerator.h"
#endif


/***** private Daten *****/

struct Screenliste Screenliste;
UWORD Bildzeilen;
struct Screeneintrag noscreen;


/***** Funktionen *****/

void Analyse_Init(mannliste)
struct Liste *mannliste;
	{
	Liste_C_Neu(&Screenliste);
	Bildzeilen = GfxBase->NormalDisplayRows;
	noscreen.origB = GfxBase->NormalDisplayColumns;
	noscreen.normB = GfxBase->NormalDisplayColumns>>1;
	noscreen.origY = Bildzeilen;
	noscreen.normY = Bildzeilen;
	noscreen.densX = 1;
	noscreen.pseudoscreen = 1;
	noscreen.begehbar = 1;
	noscreen.benutzbar = 1;
	noscreen.tiefe = 1;								/* Pseudoscreen ruft keine	*/
	noscreen.helle[0] = ANALYSE_MAXLUMEN/5;	/*		Taschenlampe hervor	*/
	noscreen.helle[1] = ANALYSE_MAXLUMEN/5;
	Analyse_Screens(mannliste);
	}

void Analyse_Clear()
	{
	Liste_C_Alt(&Screenliste, FreeMem, (long)sizeof(struct Screeneintrag));
	}

void Analyse_Neu(mann)
struct Manneintrag *mann;
	{
	if(!Liste_C_Leer(&Screenliste))
		{
		struct Screeneintrag *scrn;
		scrn = (struct Screeneintrag *)
			Liste_C_Sohn(&Screenliste, &Screenliste);

		mann->posX = Zufall_C_Wert((long)((scrn->normB-MANN_BREITE)<<KOMMA));
		mann->posY = (scrn->normY+scrn->sichtbar+MANN_HOEHE)<<KOMMA;
		mann->grund = scrn;
		}
	}

void Analyse_Alt(mann)
struct Manneintrag *mann;
	{
	}


char Analyse_Vert(scrn, xpos, ypos, prio)	/* prüft über die gesamte Höhe,	*/
struct Screeneintrag *scrn;				/* mindestens die Hälfte muß		*/
WORD xpos, ypos;								/* Kontrast aufweisen				*/
LONG prio;
	{
	ULONG
		wert,
		maske,
		muster,
		offset;
	WORD
		m,
		kontrast = 0,
		line = MANN_GROESSE;

	ypos -= scrn->normY;
	xpos -= scrn->normX;

	if(ypos<MANN_GROESSE && prio==2)			/* nicht oben aus dem	*/
		return(0);									/*		Screen raus!		*/
														/*		(bei klettern)		*/

	if(xpos<0 || xpos>scrn->normB-MANN_BREITE+2)
		return(1);									/* Screenränder beachten */

	if(!prio || !scrn->begehbar)				/* nur Screenränder		*/
		return(0);							/*		(bei quit oder nicht		*/
												/*		benutzbaren Screens)		*/

	if(prio==1)										/* 50%/50%					*/
		{												/*		(bei gehen)			*/
		if(Zufall_Muenzwurf()					/* Meist kommen zwei		*/
			|| Zufall_Muenzwurf())				/*		Kontrastgrenzen!	*/
			return(0);
		}

	xpos <<= scrn->densX;
	ypos <<= scrn->densY;
	line <<= scrn->densY;

	offset = (scrn->breiteinbytes*((long)ypos) + (xpos>>3)) & ~1;
	maske = 0x80000000 >> (xpos&15);

	while(line>0)
		{
		line--;
		muster = 0;
		if(scrn->densX)
			{
			for(m=0; m<scrn->tiefe; m++)
				{
				wert = *((ULONG *)(scrn->map[m]+offset));
				muster |= (wert^(wert<<1)) | (wert^(wert<<2));
				}
			}
		else
			{
			for(m=0; m<scrn->tiefe; m++)
				{
				wert = *((ULONG *)(scrn->map[m]+offset));
				muster |= (wert^(wert<<1));
				}
			}
		offset -= scrn->breiteinbytes;
		if(muster & maske)
			kontrast++;
		}

	if(kontrast >= ((MANN_GROESSE/2)<<scrn->densY))
		return(1);
	return(0);
	}


char Analyse_Horiz(scrn, xpos, ypos)
struct Screeneintrag *scrn;
WORD xpos, ypos;
	{
	ULONG
		offset,
		muster,
		maske;
	WORD
		m;				/* Planenummer */

	ypos -= scrn->normY;
	xpos -= scrn->normX;
	if((ypos<MANN_GROESSE&&!scrn->pseudoscreen) || !scrn->begehbar)
		return(0);						/* nicht oben aus dem Screen gehen		*/
	if(ypos>=(scrn->sichtbar-1)	/* von unten aus in den Screen			*/
		|| xpos<-1 || xpos>scrn->normB-MANN_BREITE+3)
		return(1);

	xpos <<= scrn->densX;			/* Auflösung anpassen						*/
	ypos <<= scrn->densY;

	offset = (scrn->breiteinbytes*((long)ypos) + (xpos>>3)) & ~1;
	maske = ((0xFFFFffffL >> (MANN_BREITE<<scrn->densX)) ^ -1L)
											/* Bit-Maske, hier am linken Rand, ...	*/
		>> (xpos&15);					/* ... an die richtige Stelle				*/

	muster = 0;
	if(scrn->densY)
		{
		for(m=0; m<scrn->tiefe; m++)
			muster |=
				(	*((ULONG *)(scrn->map[m] + offset))
				^	*((ULONG *)(scrn->map[m] + offset-scrn->breiteinbytes)))
				|
				(	*((ULONG *)(scrn->map[m] + offset))
				^	*((ULONG *)(scrn->map[m] + offset+scrn->breiteinbytes)))
				;
		}
	else
		{
		for(m=0; m<scrn->tiefe; m++)
			muster |= *((ULONG *)(scrn->map[m] + offset))
					^	 *((ULONG *)(scrn->map[m] + offset+scrn->breiteinbytes));
		}

	if(muster&maske)
		return(1);
	return(0);
	}


void Analyse_Screens(mannliste)
struct Mannliste *mannliste;
	{
	char aenderung = 0;
	WORD grenze;
	struct Screen *screen;
	typedef struct Screeneintrag *ELM;
	ELM intern, suche, alt;
	struct Manneintrag *mann;
	ULONG lock;

	lock = LockIBase(0L);
	screen = IntuitionBase->FirstScreen;
	intern = (ELM)Liste_C_Sohn(&Screenliste, &Screenliste);

	while(screen)
		{
		if(screen!=intern->original || Liste_C_Stop(&Screenliste, intern))
			{
			aenderung = 1;
			suche = intern;
			while(!Liste_C_Stop(&Screenliste, suche))
				{
				if(suche->original == screen)
					{
					Liste_C_Ausfuegen(&Screenliste, suche);
					goto weiter;
					}
				suche = (ELM)Liste_C_Sohn(&Screenliste, suche);
				}
			suche = AllocMem((long)sizeof(*suche), MEMF_CLEAR);
			if(suche)
				{
				int i;
				neuer_Screen:
				suche->original = screen;
				if(screen->ViewPort.Modes&HIRES)
					suche->densX = 1;
				if(screen->ViewPort.Modes&LACE)
					suche->densY = 1;
				suche->tiefe = screen->BitMap.Depth;
				if(suche->tiefe<5)		/* Grafikfarben nicht überschreiben	*/
					{
					int i;
					suche->benutzbar = 1;
					for(i=16; i<32; i+=4)
						{
						SetRGB4(&screen->ViewPort, i+1L, 0L,0L,0L);
						SetRGB4(&screen->ViewPort, i+2L, 15L,15L,15L);
						SetRGB4(&screen->ViewPort, i+3L, 15L,15L,0L);
						}
					}
				if(suche->tiefe>ANALYSE_MAXDEPTH)
					suche->tiefe = ANALYSE_MAXDEPTH;
					/* HAM- und VP_HIDE-Screens sowie unbekannte Grafikformate
						können nicht untersucht werden	*/
/*				if(screen->ViewPort.Modes &
					~(HIRES|LACE|EXTRA_HALFBRITE|DUALPF|SPRITES|GENLOCK_AUDIO
					|PFBA|GENLOCK_VIDEO))
					suche->benutzbar = 0;
*/				suche->origMode = screen->ViewPort.Modes;
				suche->origX = screen->LeftEdge;
				suche->origY = screen->TopEdge;
				suche->normX = suche->origX>>suche->densX;
				suche->normY = suche->origY>>suche->densY;
				suche->breiteinbytes = screen->BitMap.BytesPerRow;
				suche->origB = screen->Width;
				suche->origH = screen->BitMap.Rows;
				suche->normB = suche->origB>>suche->densX;
				suche->normH = suche->origH>>suche->densY;
				for(i=0; i<suche->tiefe; i++)
					suche->map[i] = screen->BitMap.Planes[i];
				}
			else
				Fehler();
			weiter:
			if(suche)
				intern = (ELM)Liste_C_Vorfuegen(&Screenliste, suche, intern);
			}

		if(intern->nachschauen == 0)	/* gravierende Änderungen der			*/
			{									/*		grundlegenden Arbeitsmittel	*/
			ULONG i, farbe;				/*		sofort bereinigen					*/

			intern->nachschauen = SEK2_0;
			if(intern->origB != screen->Width
				|| intern->origH != screen->BitMap.Rows
				|| intern->tiefe != screen->BitMap.Depth
				|| intern->origMode != screen->ViewPort.Modes)
				{
				suche = (struct Screeneintrag *)
					Liste_C_Ausfuegen(&Screenliste, intern);
				suche->densX = 0;
				suche->densY = 0;
				goto neuer_Screen;
				}

			for(i=0; i<intern->tiefe; i++)	/* Planes neu laden				*/
				intern->map[i] = screen->BitMap.Planes[i];
			if(intern->tiefe<5)
				{
				for(i=16; i<32; i+=4)
					{
					if(GetRGB4(&screen->ViewPort, i+1L) != 0x000)
						SetRGB4(&screen->ViewPort, i+1L, 0L,0L,0L);
					if(GetRGB4(&screen->ViewPort, i+2L) != 0xFFF)
						SetRGB4(&screen->ViewPort, i+2L, 15L,15L,15L);
					if(GetRGB4(&screen->ViewPort, i+3L) != 0xFF0)
						SetRGB4(&screen->ViewPort, i+3L, 15L,15L,0L);
					}
				}

														/* Helligkeiten neu laden		*/
			for(i=0; i<(1<<screen->BitMap.Depth); i++)
				{
				farbe = GetRGB4(screen->ViewPort.ColorMap, (long)i);
				intern->helle[i] = (farbe&0xf)+((farbe>>4)&0xf)
					+((farbe>>8)&0xf);
				}
			}
		intern->nachschauen --;


		if(intern->origX!=screen->LeftEdge)
			{
			intern->origX = screen->LeftEdge;
			intern->normX = intern->origX>>intern->densX;
			}
		if(intern->origY!=screen->TopEdge)
			{
			WORD weg, alt;
			aenderung = 1;
			alt = intern->normY;
			weg = (screen->TopEdge>>intern->densY) - intern->normY;
			intern->origY = screen->TopEdge;
			intern->normY += weg;
			if(weg<0)
				{
				mann = (struct Manneintrag *)Liste_C_Sohn(mannliste, mannliste);
				while(!Liste_C_Stop(mannliste, mann))
					{
					if(mann->grund==intern ||
						((mann->posY>>KOMMA)>=intern->normY-UEBERGANG
							&& (mann->posY>>KOMMA)<=alt))
						{
						if(mann->Bewegungsart==stehen
							|| mann->Bewegungsart==gehen
							|| mann->Bewegungsart==sitzen)
							{
							mann->speedY = weg;
							if(-mann->speedY > MAXSPEED)
								mann->speedY = -MAXSPEED;
							mann->posY += weg<<KOMMA;
							if(mann->posY < (MANN_GROESSE<<KOMMA))
								{
								mann->posY = MANN_GROESSE<<KOMMA;
								mann->speedY = 0;
								}
							mann->Bewegungsart = fallen;
							mann->bild = DISP_FALLEND;
							}
						}
					mann = (struct Manneintrag *)Liste_C_Sohn(mannliste, mann);
					}
				}
			}
		intern = (ELM)Liste_C_Sohn(&Screenliste, intern);
		screen = screen->NextScreen;
		}

	while(!Liste_C_Stop(&Screenliste, intern))
		{
		aenderung = 1;
		alt = intern;
		intern = (ELM)Liste_C_Sohn(&Screenliste, intern);
		FreeMem(Liste_C_Ausfuegen(&Screenliste, alt), (long)sizeof(*alt));
		}

	UnlockIBase(lock);

	if(aenderung)
		{
		grenze = Bildzeilen;

		intern = (ELM)Liste_C_Sohn(&Screenliste, &Screenliste);
		while(!Liste_C_Stop(&Screenliste, intern))
			{
			intern->sichtbar = grenze-intern->normY;
			if(intern->sichtbar > intern->normH)
				intern->sichtbar = intern->normH;
			if(intern->sichtbar>0)
				{
				if(intern->normY-UEBERGANG < grenze)
					grenze = intern->normY-UEBERGANG;
				intern->begehbar = 0;
				if(intern->sichtbar>=MANN_GROESSE)
					intern->begehbar = intern->benutzbar;
				}
			intern = (struct Screeneintrag *)
				Liste_C_Sohn(&Screenliste, intern);
			}

		mann = (struct Manneintrag *)Liste_C_Sohn(mannliste, mannliste);
		while(!Liste_C_Stop(mannliste, mann))
			{
			Analyse_Grundlage(mann);
			mann = (struct Manneintrag *)Liste_C_Sohn(mannliste, mann);
			}
		}
	}


void Analyse_Grundlage(mann)
struct Manneintrag *mann;
	{
	struct Screeneintrag *screen =
		(struct Screeneintrag *)Liste_C_Sohn(&Screenliste, &Screenliste);
	WORD grenze = Bildzeilen;

	while(!Liste_C_Stop(&Screenliste, screen))
		{
		if((mann->posY>>KOMMA) >= screen->normY
			&& (mann->posY>>KOMMA) < grenze)
			{
			mann->grund = screen;
			return();
			}
		grenze = screen->normY;
		screen = (struct Screeneintrag *)Liste_C_Sohn(&Screenliste, screen);
		}
	mann->grund = &noscreen;
	}


void Analyse_Nachttest(mann)
struct Manneintrag *mann;
	{
	struct Screeneintrag *scrn;
	int planes;
	ULONG offset;
	UBYTE mask;
	WORD farbnr;

	scrn = mann->grund;
	planes = scrn->tiefe;
	offset = (((mann->posX>>KOMMA)+MANN_BREITE/2) >> (3-scrn->densX)) +
		(((mann->posY>>KOMMA)-scrn->normY-MANN_GROESSE/2) 
			<< scrn->densY)*scrn->breiteinbytes;
	mask = 0x80 >> ((mann->posX>>(KOMMA-scrn->densX)) & 7);
	farbnr = 0;

	while(planes>0)
		{
		planes--;
		farbnr += farbnr;
		if(scrn->map[planes][offset]&mask)
			farbnr++;
		}

	mann->helligkeit += -(ANALYSE_MAXLUMEN/5)
		+ scrn->helle[farbnr];

	if(mann->helligkeit > 5)
		{
		mann->nacht = 0;
		if(mann->helligkeit > SEK1_0*ANALYSE_MAXLUMEN)
			mann->helligkeit = SEK1_0*ANALYSE_MAXLUMEN;
		}
	else if(mann->helligkeit < -5)
		{
		mann->nacht = 1;
		if(mann->helligkeit < -SEK0_5*ANALYSE_MAXLUMEN)
			mann->helligkeit = -SEK0_5*ANALYSE_MAXLUMEN;
		}
	}

