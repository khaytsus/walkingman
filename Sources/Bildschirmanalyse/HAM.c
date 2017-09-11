/*JPK30.04.92*/


	/* f�r die Ber�cksichtigung des HAM-Modus m�ssen s�mtliche Bitplanes
		gespeichert werden k�nnen.  Die Reduzierung der Bitplanes beim
		Extra-Halfbright-Modus sollte aber einzurichten sein.		*/

	/* anzuraten ist die Einf�hrung von folgenden Definitionen/Konstanten:
		ANALYSE_MAXPLANES f�r die Angabe der maximal g�ltigen Bitplanes,
		ANALYSE_COLPLANES f�r die Angabe, wieviele Bitplanes f�r die
		Betrachtung von Farbregistern maximal zu Rate gezogen werden
		(Extra-Halfbright-Modus), ANALYSE_HAMDIFF als minimale
		Farbkomponentenunterscheidung f�r das Erkennen einer g�ltigen Linie.
		Au�erdem MAXDENSX und MAXDENSY f�r die Angabe des maximalen Shifts.
		*/

	/* Angesichts des Umfangs der Routinen ist die �berlegung nicht abwegig,
		sie in Assembler installieren zu wollen. Dort w�ren auch einige
		Programmkonstruktionen weit weniger kompliziert formulierbar	*/


char VertHAM(scrn, xpos, ypos)
struct Screeneintrag *scrn;
WORD xpos, ypos;
	{
	UWORD
		maske;
	ULONG
		offset;
	WORD
		kontrast = 0,
		hoehe;

	/* Hier wird die Position relativ zum Screen berechnet; die Position
		wird in Relation zu der Aufl�sung des Screens gesetzt. S�mtliche
		�berpr�fungen bez�glich der Screenr�nder sind an dieser Stelle
		bereits durchgef�hrt und entsprechend bearbeitet.  */

	offset = ((xpos>>3)&~1) + ypos*scrn->breiteinbytes;
	maske = 0x8000>>(xpos&15);

	hoehe = MANN_GROESSE<<scrn->densY;
	while(hoehe>0)
		{
		if(!((*((UWORD *)(scrn->map[4]+offset)) |
				*((UWORD *)(scrn->map[5]+offset))) & maske))
			kontrast++;
		offset -= scrn->breiteinbytes;
		hoehe--;
		}

							/* lag an zu wenig Zeilen ein Kontrast an?	*/
	if(kontrast < ((MANN_GROESSE/2)<<scrn->densY))
		return(0);

	/* hier k�nnen Untersuchungen erfolgen, ob sich der zuvor eingestellte
		Wert in ausreichendem Ma�e von dem neuen Wert unterscheidet.  */

	return(1);
	}


char HorizHAM(scrn, xpos, ypos)
struct Screeneintrag *scrn;
WORD xpos, ypos;
	{
	UWORD						/* es spielt keine Rolle, ob 1 oder 2 nun oben	*/
		maske_1,				/*		oder unten liegt, ist irrelevant, es geht	*/
		maske_2,				/*		nur um den Unterschied							*/
		farbe;
	ULONG
		offset_1,
		offset_2;
	WORD
		xtest1,
		xtest2,
		kontrast = 0,
		hoehe,
		plane;
	char
		helle_1[3],
		helle_2[3],
		*tab_1 = helle_1,
		*tab_2 = helle_2,
		komponente,
		teildiff,
		differenz;

	/* Hier wird die Position relativ zum Screen berechnet; die Position
		wird in Relation zu der Aufl�sung des Screens gesetzt. S�mtliche
		�berpr�fungen bez�glich der Screenr�nder sind an dieser Stelle
		bereits durchgef�hrt und entsprechend bearbeitet.  */

	helle_1[0] = -1;				/* angeben, da� die Intensit�t dieser		*/
	helle_1[1] = -1;				/*		Farbkomponenten noch nicht bekannt	*/
	helle_1[2] = -1;				/*		ist											*/
	helle_2[0] = -1;
	helle_2[1] = -1;
	helle_2[2] = -1;

										/* aktuell untersuchte Position				*/
	xtest_1 = xpos+(MANN_BREITE<<scrn->densX);
	xtest_2 = xtest_1;

	offset_1 = ((xtest_1>>3)&~1) + ypos*scrn->breiteinbytes;
	maske_1 = 0x8000>>(xtest_1&15);

	offset_2 = offset_1+scrn->breiteinbytes;
	maske_2 = maske_1;


	while(xtest_1>=0)
		{
										/* nach einer �nderung suchen				*/
		while(((*((UWORD *)(scrn->map[4]+offset_1)) |
				  *((UWORD *)(scrn->map[5]+offset_1))) & maske_1)
			&& xtest_1>=0)
			{
			xtest_1--;
			maske_1 += maske_1;
			if(!maske_1)
				{
				maske_1 = 1;
				offset_1 -= 2;
				}
			}

		if(xtest<0)
			{
			/* hier mu� die Hintergrundfarbe eingetragen werden */
			}
		else
			{
			/* hier wird eine Komponente ver�ndert */
										/* Komponentennummer bestimmen			*/
			komponente = 0;
			if(((UWORD *)(scrn->map[4]+offset_1)) & maske_1)
				komponente++;
			if(((UWORD *)(scrn->map[5]+offset_2)) & maske_1)
				komponente += 2;

										/* falls die Komponente relevant ist,
											ihre Intensit�t bestimmen				*/
			if(tab_1[komponente]<0 || xtest_1>=xpos)
				{
				farbe = 0;
				plane = 4;
				while(plane>0)
					{
					farbe += farbe;
					plane--;
					if(*((UWORD *)(scrn->map[plane]+offset_1)) & mask1)
						farbe++;
					}
				tab_1[komponente] = farbe;
				}
			}

		differenz = 0;
		for(komponente=0; komponente<3; komponente++)
			{
			if(helle_2[komponente]>=0 && helle_1[komponente]>0)
				{
				teildiff = tab_1[komponente]-tab_2[komponente];
				if(teildiff<0)
				teildiff = -teildiff;
				differenz += teildiff;
				}
			}
		if(differenz >= ANALYSE_HAMDIFF)
			return(1);

		if(xtest_1<xtest_2)
			{
			ULONG	swapoff = offset_1;
			UWORD	swapmsk = mask_1;
			WORD	swaptst = xtest_1;
			char *swaptab = tab_1;

			offset_1 =	offset_2;
			mask_1 =		mask_2;
			xtest_1 =	xtest_2;
			tab_1 =		tab_2;

			offset_2 =	swapoff;
			mask_2 =		swapmsk;
			xtest_2 =	swaptst;
			tab_2 = 		swaptab;
			}
		if(xtest_1<xpos					/* hier kann keine �nderung mehr		*/
			&& helle_1[0]>=0				/*		geschehen							*/
			&& helle_1[1]>=0
			&& helle_1[1]>=0
			&& helle_2[0]>=0
			&& helle_2[1]>=0
			&& helle_2[1]>=0)
			return(0);
		}
	}

