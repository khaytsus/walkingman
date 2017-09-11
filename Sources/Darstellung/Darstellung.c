/* JPK14.04.92-24.04.92+05.05.92 */

#define DISP_PRIVAT
#include "Darstellung/Darstellung.h"


	/***** private Daten *****/

UWORD Alternative_warten;

struct Disp_Bild Bildfolge[] =
	{
	  0,768,	 7,7,		6,9,		/* (0) Stehen		*/
	 48,768,	 8,8,		6,7,		/* (1) Sitzen		*/
	 96,768,	 9,9,		5,9,		/* (2) Fallen		*/
	144,720,	10,10,	2,8,	/* ( 3) Rechtsgehend	*/
	336,672,	17,17,	9,8,	/* ( 4) Linksgehend	*/
	648,768,	27,27,	6,8,		/* (5) Linkskletternd	*/
	576,768,	24,24,	5,8,		/* (6) Rechtskletternd	*/

	 24,792,	0,DISP_JONGLEUR,	6,9,		/* (7) Stehen 2	*/
	 72,792,	1,1,					6,7,		/* (8) Sitzen 2	*/
	120,792,	2,2,					5,9,		/* (9) Fallen 2	*/

	168,744,	11,11,	2,8,	/* (10) rGehend 2	*/
	192,720,	12,12,	2,8,	/* (11) rGehend 3	*/
	216,744,	13,13,	2,8,	/* (12) rGehend 4	*/
	240,720,	14,14,	2,9,	/* (13) rGehend 5	*/
	264,744,	15,15,	2,9,	/* (14) rGehend 6	*/
	288,720,	16,16,	2,9,	/* (15) rGehend 7	*/
	312,744,	 3,3,		2,9,	/* (16) rGehend 8	*/

	360,696,	18,18,	9,8,	/* (17) lGehend 2	*/
	384,672,	19,19,	9,8,	/* (18) lGehend 3	*/
	408,696,	20,20,	9,8,	/* (19) lGehend 4	*/
	432,672,	21,21,	9,9,	/* (20) lGehend 5	*/
	456,696,	22,22,	9,9,	/* (21) lGehend 6	*/
	480,672,	23,23,	9,9,	/* (22) lGehend 7	*/
	504,696,	 4,4,		9,9,	/* (23) lGehend 8	*/

	528,792,	25,25,	5,9,		/* (24) rKletternd 2	*/
	552,768,	26,26,	5,8,		/* (25) rKletternd 3	*/
	576,792,	 6,6,		5,8,		/* (26) rKletternd 4	*/

	600,792,	28,28,	6,9,		/* (27) lKletternd 2	*/
	624,768,	29,29,	6,8,		/* (28) lKletternd 3	*/
	648,792,	 5,5,		6,8,		/* (29) lKletternd 4	*/

	~0,~0,	30,30,	0,0,	/* (30) unsichtbar	*/

	 ~24,~24,	32,32,	3,8,		/* (31) Jongleur 1	*/
	 ~48,~48,	33,33,	3,8,		/* (32) Jongleur 2	*/
	 ~72,~72,	34,34,	3,8,		/* (33) Jongleur 3	*/
	 ~96,~96,	35,35,	3,8,		/* (34) Jongleur 4	*/
	~120,~120,	36,36,	3,8,		/* (35) Jongleur 5	*/
	~144,~144,	37,37,	3,8,		/* (36) Jongleur 6	*/
	~168,~168,	38,38,	3,8,		/* (37) Jongleur 7	*/
	~192,~192,	39,39,	3,8,		/* (38) Jongleur 8	*/
	~216,~216,	40,40,	3,8,		/* (39) Jongleur 9	*/
	~240,~240,	31,31,	3,8,		/* (40) Jongleur 10	*/
	};


	/***** Funktionen *****/

void Disp_Init()
	{
	Alternative_warten = SEK60_0+Zufall_C_Wert((long)SEK60_0);
	}

void Disp_Clear()
	{
	struct Preferences PrefBuffer;
											/* Mauszeigerfarben wiederherstellen	*/
	SetPrefs(
		GetPrefs(&PrefBuffer, (long)sizeof(PrefBuffer)),
		(long)sizeof(PrefBuffer), 0L);
	}

void Disp_Schritt(mann)
struct Manneintrag *mann;
	{
	UWORD	*matrix;
	WORD	offset;

	Alternative_warten--;
	if(!Alternative_warten)
		{
		if(Zufall_Muenzwurf())
			{
			mann->bild = Bildfolge[mann->bild].alternativ;
			mann->Warten = SEK30_0;
			}
		Alternative_warten = 1+Zufall_C_Wert((long)SEK60_0);
		}

	offset = Bildfolge[mann->bild].MapBeginn[mann->nacht];
	if(offset>=0)
		matrix = &mann->grafik.map[offset];
	else
		{
		matrix = &Mann_Statisch[~offset];
		Alternative_warten = SEK60_0;				/* kein anderes Männchen	*/
		}													/*		gleichzeitig!			*/
	if(matrix != mann->sprite.posctldata)
		ChangeSprite(0L, &mann->sprite, matrix);

	MoveSprite(0L, &mann->sprite,
		(long)((mann->posX>>KOMMA) - Bildfolge[mann->bild].spotX),
		(long)((mann->posY>>KOMMA) - Bildfolge[mann->bild].spotY));

	mann->bild = Bildfolge[mann->bild].weiter;
	Analyse_Nachttest(mann);
	}

char Disp_Neu(mann)
struct Manneintrag *mann;
	{
	WORD spritenr;

	mann->grafik = Mann_Grafik;
	mann->sprite.height = MANN_HOEHE;
	mann->sprite.posctldata = &Mann_Statisch[0];
	switch(mann->Bewegungsart)
		{
		case gehen:
			mann->bild = DISP_STEHEND;
			break;
		case fallen:
			mann->bild = DISP_FALLEND;
			break;
		}

	spritenr = GetSprite(&mann->sprite, -1L);
	if(spritenr<0)
		return(Fehler());
	eigene_Sprites += 1<<spritenr;

	return(1);
	}

void Disp_Alt(mann)
struct Manneintrag *mann;
	{
	eigene_Sprites -= 1<<mann->sprite.num;
	FreeSprite((long)mann->sprite.num);
	}

