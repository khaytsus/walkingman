;JPK12.04.92

erstes	equ	0			;Eintrag des ersten Elements einer Liste
letztes	equ	4			;Eintrag des letzten Elements einer Liste
sohn		equ	0			;Eintrag des Nachfolgers eines Elements
vater		equ	4			;Eintrag des Vorgängers eines Elements
size		equ	8			;Größe des Listenkopfs sowie Elementkopfes

param1	equ	4			;Offset des ersten,
param2	equ	8			;	zweiten,
param3	equ	12			;	dritten Parameters


cseg

	public _Liste_Init
_Liste_Init:
	moveq		#1,d0
	rts


	public _Liste_Clear
_Liste_Clear:
	rts


	public _Liste_C_Neu
_Liste_C_Neu:
	movea.l	param1(sp),a0		;a0.l:	die neue Liste
	public _Liste_Neu
_Liste_Neu:
	move.l	a0,erstes(a0)
	move.l	a0,letztes(a0)
	rts


	public _Liste_C_Alt
_Liste_C_Alt:
	movea.l	param1(sp),a0		;a0.l:	die zu löschende Liste
	movea.l	param2(sp),a1		;a1.l:	aufzurufende Funktion
	move.l	param3(sp),d0		;d0.l:	Zusatzparameter
	public _Liste_Alt
_Liste_Alt:
	move.l	a1,d1					;Test, ob eine Funktion aufzurufen ist
	beq		_Liste_Neu			;wenn ja, nur initialisieren

	move.l	d0,-(sp)				;Stackoffset 8: Funktionsparameter
	move.l	a1,-(sp)				;Stackoffset 4: Funktion
	move.l	a0,-(sp)				;Stackoffset 0: Liste
alt_leertest:						;a0.l:	zu löschende Liste
	cmpa.l	erstes(a0),a0		;sind noch Einträge vorhanden?
	beq		alt_ende

	movea.l	erstes(a0),a1		;a1.l: auszufügendes Element
	bsr		_Liste_Ausfuegen
										;Funktionsparameter laden:
	movea.l	d0,a1					;a1.l:	zu bearbeitendes Element
	move.l	8(sp),d0				;d0.l:	zusätzlicher Parameter
	move.l	4(sp),a0				;a0.l:	aufzurufende Funktion

	move.l	d0,-(sp)				;Parameter auf den Stack
	move.l	a1,-(sp)
	jsr		(a0)					;Funktion aufrufen
	addq.l	#8,sp					;Stack wiederherstellen

	movea.l	(sp),a0				;a0.l:	Liste wieder laden
	bra		alt_leertest		;falls nicht leer, weiterlöschen
alt_ende:
	lea		12(sp),sp			;die lokal angelegten Daten entfernen
	rts


	public _Liste_C_Ausfuegen
_Liste_C_Ausfuegen:
	movea.l	param2(sp),a1		;a1.l:	das alte Element
	public _Liste_Ausfuegen
_Liste_Ausfuegen:
	move.l	a1,d0					;d0.l:	Element (Ergebniswert)
	movea.l	vater(a1),a0		;a0.l:	der Vorgänger
	movea.l	sohn(a1),a1			;a1.l:	der Nachfolger
	move.l	a0,vater(a1)
	move.l	a1,sohn(a0)
	rts


	public _Liste_C_Nachfuegen
_Liste_C_Nachfuegen:
	movea.l	param2(sp),a0		;a0.l:	das neue Element
	movea.l	param3(sp),a1		;a1.l:	das Vorgängerelement
	public _Liste_Nachfuegen
_Liste_Nachfuegen:
	move.l	a0,d0					;d0.l:	das neue Element (Ergebniswert)
	move.l	sohn(a1),sohn(a0)
	move.l	a0,sohn(a1)
	move.l	a1,vater(a0)
	movea.l	sohn(a0),a1			;a1.l:	das Nachfolgeelement des neuen
	move.l	a0,vater(a1)
	rts


	public _Liste_C_Vorfuegen
_Liste_C_Vorfuegen:
	movea.l	param2(sp),a0		;a0.l:	das neue Element
	movea.l	param3(sp),a1		;a1.l:	das Nachfolgeelement
	public _Liste_Vorfuegen
_Liste_Vorfuegen:
	movea.l	vater(a1),a1		;a1.l:	das Vorgängerelement
	bra		_Liste_Nachfuegen


	public _Liste_C_Erstes
_Liste_C_Erstes:
	movea.l	param1(sp),a0		;a0.l:	die Liste
	movea.l	param2(sp),a1		;a1.l:	das Element
	public _Liste_Erstes
_Liste_Erstes:
	cmpa.l	erstes(a0),a1
	seq		d0						;d0.b:	bei Gleichheit -1
	neg.b		d0
	rts


	public _Liste_C_Letztes
_Liste_C_Letztes:
	movea.l	param1(sp),a0		;a0.l:	die Liste
	movea.l	param2(sp),a1		;a1.l:	das Element
	public _Liste_Letztes
_Liste_Letztes:
	cmpa.l	letztes(a0),a1
	seq		d0						;d0.b:	bei Gleichheit -1
	neg.b		d0
	rts


	public _Liste_C_Leer
_Liste_C_Leer:
	movea.l	param1(sp),a0		;a0.l:	die Liste
	public _Liste_Leer
_Liste_Leer:
	cmpa.l	erstes(a0),a0
	seq		d0						;d0.b:	bei Gleichheit -1
	neg.b		d0
	rts


	public _Liste_C_Stop
_Liste_C_Stop:
	movea.l	param1(sp),a0		;a0.l:	die Liste
	movea.l	param2(sp),a1		;a1.l:	das Element
	public _Liste_Stop
_Liste_Stop:
	cmpa.l	a0,a1
	seq		d0						;d0.b:	bei Gleichheit -1
	neg.b		d0
	rts


	public _Liste_C_Sohn
_Liste_C_Sohn:
	movea.l	param2(sp),a1		;a1.l:	das Element
	public _Liste_Sohn
_Liste_Sohn:
	move.l	sohn(a1),d0
	rts


	public _Liste_C_Vater
_Liste_C_Vater:
	movea.l	param2(sp),a1		;a1.l:	das Element
	public _Liste_Vater
_Liste_Vater:
	move.l	vater(a1),d0
	rts


end

