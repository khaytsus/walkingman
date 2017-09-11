;JPK11.04.92-21.04.92

vhposr		equ	$dff006				;Registeradresse der Rasterstrahlposition
hi_of_word	equ	0
lo_of_word	equ	1




dseg

	global linear_kongruenz,2			;Hilfsregister für den Zufallswert
	global additiv_kongruenz,2			;Hilfsregister für den Münzwurf


cseg

	public _Zufall_Init
_Zufall_Init:
	move.w	vhposr,linear_kongruenz
	move.w	vhposr,additiv_kongruenz
	bne		init_exit
	addq.w	#6,additiv_kongruenz
init_exit:
	rts


	public _Zufall_Clear
_Zufall_Clear:
	rts


	public _Zufall_Muenzwurf
_Zufall_Muenzwurf:
	rol		additiv_kongruenz
	smi		d0
	btst		#5,additiv_kongruenz
	beq		muenzwurf_exit
	bchg		#0,additiv_kongruenz
muenzwurf_exit:
	neg.b		d0
	rts


	public _Zufall_C_Wert
_Zufall_C_Wert:
	move.l	4(sp),d0
	public _Zufall_Wert
_Zufall_Wert:
	movea.l	d0,a0							;a0.w:	Anzahl möglicher Werte (m)
	move.w	linear_kongruenz,d0		;d0.w:	Inhalt des Privatregisters (a)
	move.w	#27183,d1
	mulu		d1,d0							;d0.l = a*b
	addq.l	#1,d0							;				+ 1
	move.w	d0,linear_kongruenz		;Privatregister = d0.w
	move.l	a0,d1
schleife:
	divu		d1,d0							;d0.w = d0.l%m = (a*b+1)%m
	clr.w		d0								;bei nur einer Division kann es sein,
	swap		d0								;	daß der Rest (wegen zu kleinem
	cmp.w		d1,d0							;	Teiler) noch größer als der Teiler
	bge		schleife						;	ist!
	rts


	end

