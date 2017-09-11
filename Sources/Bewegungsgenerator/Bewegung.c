/* JPK13.04.92-26.04.92+22.05.92 */

#define BEWEGUNG_PRIVAT
#include "Bewegungsgenerator/Bewegung.h"
#undef BEWEGUNG_PRIVAT


	/***** private Daten *****/

struct Mannliste Mannliste;
long Altprioritaet;
struct Task *MyTask;
UBYTE eigene_Sprites;


	/***** Funktionen *****/

void Bewegung_Init(mannenzahl)
WORD mannenzahl;
	{
	Liste_Init();
	Zufall_Init();
	Liste_C_Neu(&Mannliste);
	Analyse_Init(&Mannliste);
	Disp_Init();
	while(mannenzahl>0)
		{
		mannenzahl--;
		if(!Bewegung_Neu())
			break;
		}
	if(MyTask = FindTask(0L))
		Altprioritaet = SetTaskPri(MyTask, 5L);
	}

void Mann_entfernen(mann)
struct Manneintrag *mann;
	{
	Analyse_Alt(mann);
	Disp_Alt(mann);
	FreeMem(mann, (long)sizeof(*mann));
	if(Liste_C_Leer(&Mannliste) && Flags.finale)
		Flags.quit = 1;
	}

void Bewegung_Clear()
	{
	if(MyTask)
		SetTaskPri(MyTask, Altprioritaet);
	Liste_C_Alt(&Mannliste, Mann_entfernen);
	Analyse_Clear();
	Disp_Clear();
	Liste_Clear();
	}

char Bewegung_Neu()
	{
	struct Manneintrag *mann = (struct Manneintrag *)
		AllocMem((long)sizeof(struct Manneintrag), MEMF_CLEAR|MEMF_CHIP);

	if(mann)
		{
		mann->Bewegungsart = stehen;
		mann->Warten = SEK2_0;
		if(Disp_Neu(mann))
			{
			Analyse_Neu(mann);
			Liste_C_Nachfuegen(&Mannliste, mann, &Mannliste);
			return(1);
			}
		FreeMem(mann, (long)sizeof(*mann));
		}
	Fehler();
	return(0);
	}


void Bewegung_Alt(anzahl)
UWORD anzahl;
	{
	struct Manneintrag *mann =
		(struct Manneintrag *)Liste_C_Vater(&Mannliste, &Mannliste);
	while(!Liste_C_Stop(&Mannliste, mann) && anzahl>0)
		{
		if(!mann->verschwindet)
			{
			mann->verschwindet = 1;
			mann->Warten = 1;
			anzahl--;
			}
		mann = (struct Manneintrag *)
			Liste_C_Vater(&Mannliste, mann);
		}
	}


void Bewegung_Halbschritt()
	{
	struct Manneintrag *mann;

	mann = (struct Manneintrag *)Liste_C_Sohn(&Mannliste, &Mannliste);
	while(!Liste_C_Stop(&Mannliste, mann))
		{
		if(mann->Bewegungsart!=klettern)
			mann->posX += mann->speedX;
		mann->posY += mann->speedY;
		Disp_Schritt(mann);
		mann = (struct Manneintrag *)Liste_C_Sohn(&Mannliste, mann);
		}
	}

void Bewegung_Schritt()
	{
	struct Manneintrag
		*mann = (struct Manneintrag *)Liste_C_Sohn(&Mannliste, &Mannliste),
		*vater;

	Analyse_Screens(&Mannliste);

	while(!Liste_C_Stop(&Mannliste, mann))
		{
		switch(mann->Bewegungsart)
			{
			case unsichtbar:
				if(mann->verschwindet)
					{
					vater = (struct Manneintrag *)
						Liste_C_Vater(&Mannliste, mann);
					Mann_entfernen(Liste_C_Ausfuegen(&Mannliste, mann));
					mann = vater;
					}
				goto nextmann;
			case klettern:
				mann->Warten--;
				if(!mann->Warten)
					{
					if(Zufall_Muenzwurf() && !mann->verschwindet)
						mann->Warten = 1+Zufall_C_Wert((long)SEK10_0);
					else
						{
						mann->Bewegungsart = fallen;
						mann->bild = DISP_FALLEND;
						}
					}

				mann->posY += mann->speedY;

				if((mann->speedX>0)?
					!Analyse_Vert(mann->grund,
						mann->posX>>KOMMA,mann->posY>>KOMMA, 2L):
					!Analyse_Vert(mann->grund,
						(mann->posX>>KOMMA)+MANN_BREITE,
						mann->posY>>KOMMA, 2L))
					{
					if(Analyse_Horiz(mann->grund,
						mann->posX>>KOMMA,(mann->posY>>KOMMA)-MANN_HAND-1)
						&& !mann->verschwindet)
						{
						mann->posY -= MANN_HAND<<KOMMA;
						mann->speedX = 0;
						mann->speedY = 0;
						mann->Bewegungsart = stehen;
						mann->bild = DISP_STEHEND;
						mann->Warten = SEK2_0+Zufall_C_Wert((long)SEK2_0);
						}
					else
						{
						mann->Bewegungsart = fallen;
						mann->bild = DISP_FALLEND;
						}
					}
				Disp_Schritt(mann);
				break;

			case gehen:
				mann->posX += mann->speedX;
				mann->Warten--;
				if(!mann->Warten)
					{
					if(Zufall_Muenzwurf() || mann->verschwindet)
						mann->Warten = Zufall_C_Wert((long)SEK15_0)+1;
					else
						{
						mann->bild = DISP_STEHEND;
						mann->Bewegungsart = stehen;
						mann->Warten = Zufall_C_Wert((long)SEK1_0)+SEK2_0;
						mann->speedX = 0;
						}
					}

					/* Männchen nicht seitlich aus dem Bild gehen lassen */
				if(!mann->verschwindet)
					{
					if((mann->speedX>0)?
						Analyse_Vert(mann->grund,
							(mann->posX>>KOMMA)+MANN_BREITE,
							mann->posY>>KOMMA, 1L):
						Analyse_Vert(mann->grund,
							mann->posX>>KOMMA,mann->posY>>KOMMA, 1L))
						{
						mann->Bewegungsart = klettern;
						mann->speedX = -mann->speedX;
						mann->speedY = -1<<(KOMMA-2);
						mann->posY &= 0xffff<<KOMMA;
						mann->bild = (mann->speedX<0)?
							DISP_LKLETTERND:  DISP_RKLETTERND;
						mann->Warten = 1+Zufall_C_Wert((long)SEK10_0);
						Disp_Schritt(mann);
						break;
						}
					}
				else
					{
					if((mann->posX>>KOMMA)<=-MANN_BREITE
						|| (mann->posX>>KOMMA)>=mann->grund->normB)
						{
						mann->Bewegungsart = unsichtbar;
						mann->bild = DISP_UNSICHTBAR;
						break;
						}
					}

				if(Analyse_Horiz(mann->grund,
					mann->posX>>KOMMA,(mann->posY>>KOMMA)-1)
					&& !mann->verschwindet)
					{
					mann->posY -= 1<<KOMMA;
					}
				else if(!Analyse_Horiz
					(mann->grund, mann->posX>>KOMMA,mann->posY>>KOMMA))
					{
					mann->Bewegungsart = fallen;
					mann->bild = DISP_FALLEND;
					mann->Warten = 0;
					}
				Disp_Schritt(mann);
				break;

			case stehen:
				mann->Warten--;
				if(!mann->Warten)
					{
					if(Zufall_Muenzwurf() && !mann->verschwindet)
						mann->Warten = 1+Zufall_C_Wert((long)SEK1_0);
					else
						{
						UWORD zufall = Zufall_C_Wert(32L);

						if(zufall<1)			/* hinsetzen */
							{
							mann->Bewegungsart = sitzen;
							mann->bild = DISP_SITZEND;
							mann->Warten = SEK2_0+Zufall_C_Wert((long)SEK2_0);
							}
						else if(zufall<3)		/* abtauchen */
							{
							mann->Warten = 2;
							mann->posY += 3<<KOMMA;
							}
						else if(zufall<4)		/* hochspringen */
							{
							mann->Bewegungsart = fallen;
							mann->bild = DISP_FALLEND;
							mann->speedY = -15;
							}
						else						/* gehen */
							{
							mann->Bewegungsart = gehen;
							mann->Warten = Zufall_C_Wert((long)SEK15_0)+1;
							if(zufall<18)
								{
								mann->bild = DISP_RGEHEND;
								mann->speedX = 1<<(KOMMA-1);
								}
							else
								{
								mann->bild = DISP_LGEHEND;
								mann->speedX = -1<<(KOMMA-1);
								}
							}
						Disp_Schritt(mann);
						break;
						}
					}

				if(Analyse_Horiz(mann->grund,
					mann->posX>>KOMMA,(mann->posY>>KOMMA)-1)
					&& !mann->verschwindet)
					{
					mann->posY -= 1<<KOMMA;
					Disp_Schritt(mann);
					break;
					}
				if(!Analyse_Horiz
					(mann->grund, mann->posX>>KOMMA,mann->posY>>KOMMA))
					{
					mann->Bewegungsart = fallen;
					mann->bild = DISP_FALLEND;
					mann->speedX = Zufall_C_Wert(5L)-2;
					mann->Warten = 0;
					}
				Disp_Schritt(mann);
				break;

			case sitzen:
				mann->Warten--;
				if(!mann->Warten)
					{
					if(Zufall_Muenzwurf() && !mann->verschwindet)
						mann->Warten = 1+Zufall_C_Wert((long)SEK0_5);
					else
						{
						mann->Warten = SEK0_5+Zufall_C_Wert((long)SEK2_0);
						mann->Bewegungsart = stehen;
						mann->bild = DISP_STEHEND;
						Disp_Schritt(mann);
						break;
						}
					}

				if(!(Analyse_Horiz(mann->grund,
					mann->posX>>KOMMA,mann->posY>>KOMMA)) || mann->verschwindet)
					{
					mann->Bewegungsart = fallen;
					mann->bild = DISP_FALLEND;
					mann->Warten = 0;
					}
				Disp_Schritt(mann);
				break;

			case fallen:
				mann->speedY += GRAVITATION;
				mann->posY += mann->speedY;
				mann->posX += mann->speedX;
				mann->Warten++;
				if(mann->posY < ((MANN_GROESSE-1)<<KOMMA))
					{
					mann->posY = (MANN_GROESSE-1)<<KOMMA;
					mann->speedY = 0;
					}

					/* Männchen auf dem Screenrand stehenbleiben lassen */
				if((mann->posY>>KOMMA) >=
					mann->grund->normY+mann->grund->sichtbar
					&& mann->grund->begehbar)
					{
					mann->posY =
						((mann->grund->normY+mann->grund->sichtbar-1)<<KOMMA);
					mann->speedY = 0;
					mann->speedX = 0;
					mann->bild = DISP_STEHEND;
					mann->Bewegungsart = stehen;
					if(mann->Warten > SEK2_0)
						mann->Warten = SEK2_0;
					mann->Warten += Zufall_C_Wert((long)SEK1_0);
					}

					/* Männchen nicht seitlich aus dem Bildschirm bewegen */
				if((mann->speedX<0)?
					Analyse_Vert(mann->grund,
						mann->posX>>KOMMA,mann->posY>>KOMMA, 1L):
					Analyse_Vert(mann->grund,
						(mann->posX>>KOMMA)+MANN_BREITE,mann->posY>>KOMMA, 1L))
					{
					mann->speedX = -mann->speedX;
					}

					/* Männchen trifft evtl. auf eine waagerechte Linie */
				if(mann->speedY>=0)
				if(Analyse_Horiz
					(mann->grund, mann->posX>>KOMMA,mann->posY>>KOMMA))
					{
					mann->speedY = 0;
					if((mann->Warten<=SEK1_0 || mann->verschwindet) &&
						(mann->speedX==(1<<KOMMA-1)
							|| mann->speedX==-(1<<KOMMA-1)))
						{
						mann->Bewegungsart = gehen;
						if(mann->speedX<0)
							mann->bild = DISP_LGEHEND;
						else
							mann->bild = DISP_RGEHEND;
						mann->Warten = 1+Zufall_C_Wert((long)SEK15_0);
						}
					else
						{
						mann->Bewegungsart = stehen;
						mann->bild = DISP_STEHEND;
						mann->speedX = 0;
						if(mann->Warten > SEK2_0)
							mann->Warten = SEK2_0;
						mann->Warten += Zufall_C_Wert((long)SEK1_0);
						if(mann->verschwindet)
							mann->Warten = 1;
						}
					}

				Disp_Schritt(mann);
				break;
			}

			/* Männchen betrat vielleicht einen anderen Screen */
		if((mann->posY>>KOMMA) < mann->grund->normY ||
			(mann->posY>>KOMMA) >= mann->grund->normY+mann->grund->sichtbar)
			{
			Analyse_Grundlage(mann);
			}
		nextmann:
		mann = (struct Manneintrag *)Liste_C_Sohn(&Mannliste, mann);
		}
	}

