/* JPK13.04.92-14.04.92 */

#ifndef EXTERN_H
#define EXTERN_H

struct Flags {
	unsigned quit:		1;
	unsigned finale:	1;
	unsigned error:	1;
	};

extern struct IntuitionBase *IntuitionBase;
extern struct GfxBase *GfxBase;

extern struct Window *MyWindow;
extern struct NewWindow MyNewWindow;
extern struct Menu MyMenu;
extern struct MenuItem
	NeuItem, AltItem, HaltItem, QuitItem;

extern struct Flags Flags;


char
	StartUp(),
	Fehler();
void
	CleanUp(),
	frequenz();

#endif

