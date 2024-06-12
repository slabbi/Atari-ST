		MC68000
		TEXT
		SUPER

TEST equ 0

_vblqueue	equ	$456
XBra			equ	'TTHC'

ZEILEN		equ		120					* 960 / 8
BPL				equ		160

MACRO	OUT par
		movem.l	d1-d3,-(sp)
		move.w	par,-(sp)
		move.w	#5,-(sp)
		trap		#1
		addq.w	#4,sp
		movem.l	(sp)+,d1-d3
ENDM

IF TEST
		BRA			haupt
ENDIF

		move.l	4(sp),a0
		move.l	#$100,d6
		add.l		12(a0),d6
		add.l		20(a0),d6
		add.l		28(a0),d6
		clr.w		-(sp)
		move.l	d6,-(sp)

		move.w	#4,-(sp)
		trap		#14
		addq.w	#2,sp
		cmp.w		#6,d0
		beq.s		rez_ok
		pea			text_4(pc)
		move.w	#9,-(sp)
		trap		#1
		addq.w	#6,sp
		bra.s		not_inst

rez_ok:
		pea			install(pc)
		move.w	#38,-(sp)
		trap		#14
		addq.w	#6,sp
		tst.l		d0
		beq.s		not_inst			* Nicht installiert!

		move.w	#$31,-(sp)		* KeepResident
		trap		#1

not_inst:	
		addq.w	#4,sp
		trap		#1

install:
		movea.l	(_vblqueue).w,a0		* Adresse VBL-QUEUE
		lea			28(a0),a0

loop:
		move.l	(a0),a2
		cmp.l		#XBra,2(a2)		* Schon da?
		beq.s		schon_da
		tst.l		(a0)				* Slot frei?
		beq.s		slot_frei
		subq.l	#4,a0
		move.l	a0,d0			* cmp.l #0,a0
;		tst.l		d0
		bne.s		loop

		pea			text_1(pc)
		move.w	#9,-(sp)
		trap		#1
		addq.w	#6,sp

		bra.s		errorende

schon_da:
		pea			text_2(pc)
		move.w	#9,-(sp)
		trap		#1
		addq.w	#6,sp
errorende:
		clr.l	d0
		rts

slot_frei:
		lea			start(pc),a1
		move.l	a1,(a0)
		pea			text_3(pc)
		move.w	#9,-(sp)
		trap		#1
		addq.w	#6,sp
		move.l	a1,d0			* d0 <> 0 -> Alles OK!
		rts

		dc.b		'XBRA'
		dc.l		XBra
		dc.l		0
start:
		bra.s		start1
		dc.l		XBra
start1:
		tst.w		$4ee.w    		* ALT+HELP gedrÅckt ? (_dumpflg)
		beq.s		haupt     		* Ja !
		rts

status:
		dc.w		0

haupt:
		lea			status(pc),a0
		move.w	sr,(a0)
		or.w		#$700,sr

		movem.l	d1-d7/a1-a6,-(sp)

IF TEST
		pea			text_3(pc)
		move.w	#9,-(sp)
		trap		#1
		addq.w	#6,sp
ENDIF

		move.w	#17,-(sp)
		trap		#1
		addq.w	#2,sp
		tst.w		d0
		beq			offline

		move.w	#3,-(sp)
		trap		#14
		addq.w	#2,sp
		move.l	d0,a6
		
		move.l	a6,a0
		move.w	#(960*BPL/4)-1,d0
inv:not.l		(a0)+
		dbra		d0,inv

		move.w	#11,-(sp)
		trap		#13
		addq.w	#2,sp
		and.w		#2,d0
		bne			hardcopy2			* zweite Hardcopy

		pea			init(pc)
		bsr 		COUT
		addq.w	#4,sp
		
zeile	SET d7

		moveq		#0,zeile
loop1:
var_j	SET d6
			moveq		#0,var_j
loop2:

var_t		SET			d5
				moveq		#0,var_t
loop3:	move.w	var_t,d4		;
				mulu		#24,d4		; t*24
				add.w		var_j,d4	; t*24+j -> Byte in Array

				move.l	a6,a0			;	base
				move.w	zeile,d0	; zeile
				mulu		#BPL,d0		; zeile*bpl
				asl.l		#3,d0			; zeile*bpl*8
				move.w	var_j,d1	; j
				mulu		#BPL,d1		;	j*bpl
				add.l		d0,a0
				add.l		d1,a0
				ext.l		var_t
				add.l		var_t,a0
					
				lea			puffer(pc),a5
				not.b		(a0)			; Kontrolle
				move.b	(a0),(a5,d4.w)
				
				addq.w	#1,var_t
				cmp.w		#BPL-1,var_t
				bls.s		loop3
			addq.w	#1,var_j
			cmp.w		#23,var_j
			bls.s		loop2

			pea			printer(pc)
			bsr			COUT
			addq.w	#4,sp
	
			OUT			#(BPL*8) % 256
			OUT			#(BPL*8) / 256
			
var_t	SET d6
			moveq		#0,var_t
for_t:

var_z		SET d5
				moveq		#7,var_z
for_z:
				moveq		#0,d1
				moveq		#0,d2
				moveq		#0,d3
				
					move.l	zeile,-(sp)
	
var_x			SET d4
					moveq		#0,var_x
for_x:		move.w	var_t,d0	;
					mulu		#24,d0		; t*24
					add.w		var_x,d0	; t*24+x -> Byte in Array
	
					lea			puffer(pc),a5
	
					btst.b	var_z,(a5,d0.w)
					beq.s		false1
						moveq		#7,d7
						sub.w		var_x,d7
						bset.b	d7,d1
false1:
					btst.b	var_z,8(a5,d0.w)
					beq.s		false2
						moveq		#7,d7 
						sub.w		var_x,d7 
						bset.b	d7,d2
false2:
					btst.b	var_z,16(a5,d0.w)
					beq.s		false3
						moveq		#7,d7 
						sub.w		var_x,d7 
						bset.b	d7,d3
false3:
					addq.w	#1,var_x
					cmp.w		#7,var_x
					bls.s		for_x
	
					move.l	(sp)+,zeile
	
					OUT			d1
					OUT			d2
					OUT			d3

				dbra		var_z,for_z
			addq.w	#1,var_t
			cmp.w		#BPL-1,var_t
			bls			for_t

		OUT			#13
		OUT			#10

		addq.w	#3,zeile
		cmp.w		#ZEILEN-1,zeile
		bls 		loop1

		OUT			#13
		OUT			#10
		OUT			#27
		OUT			#64

offline:
exit:
		movem.l	(sp)+,d1-d7/a1-a6
IF TEST
		clr.w		-(sp)
		trap		#1
ENDIF
		move.w		#-1,$4ee.w
		move.w		status(pc),sr
		rts

COUT:
		pea			(a6)
		move.l	8(sp),a6
COUTloop:
		tst.b		(a6)
		beq.s		COUTexit
		moveq		#0,d0
		move.b	(a6)+,d0
		OUT			d0
		bra.s		COUTloop
COUTexit:
		move.l	(sp)+,a6
		rts

hardcopy2:
		move.l	a6,a5
		move.l	a6,a4
		move.l	a6,a3
		add.l		#BPL*(960-1)+0,a5
		add.l		#BPL*(960-1)+1,a4
		add.l		#BPL*(960-1)+2,a3
		move.l	#(BPL*960)+3,d6

		pea			init2(pc)
		bsr 		COUT
		addq.w	#4,sp

var_y	SET d5
		move.w	#53,d5					; 54 DurchlÑufe
h2loop:
		pea			printer2(pc)
		bsr			COUT
		addq.w	#4,sp
		OUT			#960 % 256
		OUT			#960 / 256

var_x	SET d7
		move.w	#960-1,var_x
for2_x:
		moveq		#0,d1
		moveq		#0,d2
		moveq		#0,d3

		not.b		(a5)
		move.b	(a5),d1
		OUT			d1

		tst.w		var_y
		beq.s		lastline
			not.b		(a4)
			move.b	(a4),d2
			not.b		(a3)
			move.b	(a3),d3
lastline:
		OUT			d2
		OUT			d3
		sub.w		#BPL,a5
		sub.w		#BPL,a4
		sub.w		#BPL,a3
		dbra		var_x,for2_x
		OUT			#13
		OUT			#10
		add.l		d6,a5
		add.l		d6,a4
		add.l		d6,a3

		dbra		var_y,h2loop

		OUT			#13
		OUT			#10
		OUT			#27
		OUT			#64

		bra			exit

		DATA

; fÅr 1280x960 Hochformat (180dpi)
init:
		dc.b		27,64,27,'3',25,0
printer:
		dc.b		27,'*',39,0

; fÅr 960x1280 Querformat (180dpi)
init2:
		dc.b		27,64,27,'3',25,0
printer2:
		dc.b		27,'*',39,0


text_4:
		dc.b		13,10
		dc.b		' TT-High Hardcopy only works in TT-High! ',13,10
		dc.b		13,10,0
text_3:
IF TEST
		dc.b		27,'E'
ENDIF
		dc.b		13,10
		dc.b		'+-----------------------------+',13,10
		dc.b		'| TT-High Hardcopy installed! |',13,10
		dc.b		'+-----------------------------+',13,10
		dc.b		'| (c) Stephan Slabihoud 1993  |',13,10
		dc.b		'+-----------------------------+',13,10
		dc.b		'| Version 1.2      03.06.1994 |',13,10
		dc.b		'+-----------------------------+',13,10
		dc.b		13,10,0
text_2:
		dc.b		13,10
		dc.b		' TT-High Hardcopy already installed! ',13,10
		dc.b		13,10,0
text_1:
		dc.b		13,10
		dc.b		' TT-High Hardcopy needs VBL slot #7! ',13,10
		dc.b		13,10,0

		BSS
puffer:
		ds.b		BPL*24
