
#ifndef ZUFALLSGENERATOR_H
#define ZUFALLSGENERATOR_H

extern void
	Zufall_Init(),
	Zufall_Clear();
extern char Zufall_Muenzwurf();
extern unsigned short Zufall_C_Wert();

#define Zufall_Wert Zufall_C_Wert

#endif

