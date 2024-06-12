	EXPORT dtr_on_modem1,dtr_on_modem2,dtr_on_serial2
	EXPORT dtr_off_modem1,dtr_off_modem2,dtr_off_serial2
	EXPORT get_hardware,HARDWARE

MFPPORT1	EQU		0x00fffa01
MFPPORT2	EQU		0x00fffa81
SCCA_CNT	EQU		0xFFFF8C81
SCCB_CNT	EQU		0xFFFF8C85

dtr_on_modem1:
		pea			(a2)
		move.w	#0xe7,-(sp)
		move.w	#29,-(sp)
		trap		#14
		addq.w	#4,sp
		move.l	(sp)+,a2
		rts
dtr_on_modem2:
		move.b	#5,SCCB_CNT
		nop
		nop
		nop
		move.b	#$ea,SCCB_CNT
		rts
dtr_on_serial2:
		move.b	#5,SCCA_CNT
		nop
		nop
		nop
		move.b	#$ea,SCCA_CNT
		rts

dtr_off_modem1:
		pea			(a2)
		move.w	#0x18,-(sp)
		move.w	#30,-(sp)
		trap		#14
		addq.w	#4,sp
		move.l	(sp)+,a2
		rts
dtr_off_modem2:
		move.b	#5,SCCB_CNT
		nop
		nop
		nop
		move.b	#$6a,SCCB_CNT
		rts
dtr_off_serial2:
		move.b	#5,SCCA_CNT
		nop
		nop
		nop
		move.b	#$6a,SCCA_CNT
		rts

get_hardware:	move.l	$5a0.w,a0
							move.l	a0,d0				* No Cookie-Jar found -> ST
							beq.s		ende
loop:					cmp.l		#'_MCH',(a0)
							bne.s		not_found
							move.l	4(a0),d0
							bra.s		ende
not_found:		addq.w	#8,a0
							move.l	-8(a0),d0
							bne.s		loop
							moveq		#0,d0
ende:					swap		d0
							lea			HARDWARE(pc),a0
							move.w	d0,(a0)
							rts

HARDWARE:			dc.w		0
