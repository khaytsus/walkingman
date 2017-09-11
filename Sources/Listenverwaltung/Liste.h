/*JPK12.04.92-13.04.92*/

#ifndef LISTE_H
#define LISTE_H

struct Liste {
	char privat[8];
	};
struct Liste_Element {
	char liste_privat[8];
#ifdef LISTE_INHALT
	LISTE_INHALT
#endif
	};

extern void
	Liste_Init(),
	Liste_Clear(),
	Liste_C_Neu(),
	Liste_C_Alt();
extern struct Liste_Element
	*Liste_C_Ausfuegen(),
	*Liste_C_Nachfuegen(),
	*Liste_C_Vorfuegen(),
	*Liste_C_Sohn(),
	*Liste_C_Vater();
extern char
	Liste_C_Erstes(),
	Liste_C_Letztes(),
	Liste_C_Leer(),
	Liste_C_Stop();

#define Liste_Neu Liste_C_Neu
#define Liste_Alt Liste_C_Alt
#define Liste_Ausfuegen Liste_C_Ausfuegen
#define Liste_Nachfuegen Liste_C_Nachfuegen
#define Liste_Vorfuegen Liste_C_Vorfuegen
#define Liste_Erstes Liste_C_Erstes
#define Liste_Letztes Liste_C_Letztes
#define Liste_Leer Liste_C_Leer
#define Liste_Sohn Liste_C_Sohn
#define Liste_Vater Liste_C_Vater
#define Liste_Stop Liste_C_Stop

#define LISTE_DUMMY 0,0,0,0,0,0,0,0

#endif

