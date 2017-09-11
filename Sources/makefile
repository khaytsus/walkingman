#JPK-21.04.92-30.04.92

Mann: Mann.o B:Analyse.o M:Bewegung.o D:Darstellung.o D:Grafik.o
	ln Mann.o L:Liste.o B:Analyse.o M:Bewegung.o D:Grafik.o D:Darstellung.o Z:Zufallsgenerator.o -lc +Cd

D:Darstellung.o: D:Darstellung.c D:Darstellung.h M:Bewegung.h
	cc D:Darstellung.c +I/Includes/Inc.pre
D:Grafik.o: D:Grafik.asm
	as D:Grafik.asm

B:Analyse.o: B:Analyse.c B:Analyse.h D:Darstellung.h M:Bewegung.h Extern.h
	cc B:Analyse.c +I/Includes/Inc.pre

M:Bewegung.o: M:Bewegung.c M:Bewegung.h D:Darstellung.h Extern.h B:Analyse.h
	cc M:Bewegung.c +I/Includes/Inc.pre
Mann.o: Mann.c Extern.h B:Analyse.h M:Bewegung.h D:Darstellung.h
	cc Mann.c +I/Includes/Inc.pre

