.EXPORT	RingBell
.EXPORT RingGong


/* void RingBell (void); */

RingBell:
		pea			BellPattern(pc)
		move.w	#32,-(sp)
		trap		#14
		addq.w	#6,sp
		rts
BellPattern:
		dc.b	 0,$34, 1,$00, 2,$00, 3,$00
		dc.b	 4,$00, 5,$00, 6,$00, 7,$fe
		dc.b   8,$10, 9,$00,10,$00,11,$00
		dc.b	12,$10,13,$09,$FF,$00

/* void RingGong (void); */

RingGong:
		pea			GongPattern(pc)
		move.w	#32,-(sp)
		trap		#14
		addq.w	#6,sp
		rts
GongPattern:
		dc.b	 0,$C8, 1,$00, 7,$fe,11,$FF
		dc.b	12,$6E,13,$09, 8,$10,$FF,60
		dc.b	 0,$F0, 1,$00, 7,$fe,11,$FF
		dc.b	12,$6E,13,$09, 8,$10,$FF,$0

		end
