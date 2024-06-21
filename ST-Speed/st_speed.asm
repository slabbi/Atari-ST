*
* "ST-SPEED" - (c) MAXON Computer GmbH 1990
*
* Programmcode muß verschiebbar sein!
* (deshalb LEA xxx(pc),Ax TST (Ax) !)
*
* Programm unterstützt XBRA-Methode
* Kennung: "SPxx". xx gibt Versionsnummer an.
*
* Geschrieben mit Devpac-ST V2.0 von HiSoft.
*

          opt o+,p+           * Optimier, Abs.Code

zeilen:        equ  12*16*80-1
etv_critic:    equ  $404      * Critical-Error-Hdlr
hdv_bpb:       equ  $472      * GETBPB-Vektor
hdv_rw:        equ  $476      * RWABS-Vektor
hdv_mediach:   equ  $47e      * Mediach-Vektor
drvbits:       equ  $4c2      * Drvbits-Sysvar
_vblqueue:     equ  $456      * VBL-Liste
_dumpflg:      equ  $4ee      * Hardcopy-Flag
_sysbase:      equ  $4f2
_bootdev.hi:   equ  $446
_bootdev.lo:   equ  $447
vbl_slot:      equ  28        * VBL-Slot #7
kennung:       equ  'SP10'
version:       equ  '1'
revision:      equ  '0'

          output    \st_speed.bin

PRINT:    MACRO
          pea       \1
          move.w    #9,-(sp)  * PRINT
          trap      #1
          addq.l    #6,sp
          ENDM 
SUP_EXEC: MACRO
          pea       \1
          move.w    #$26,-(sp)     * SUP_EXEC
          trap      #14
          addq.l    #6,sp
          ENDM 
MALLOC:   MACRO
          move.l    \1,-(sp)
          move.w    #$48,-(sp)     * MALLOC
          trap      #1
          addq.l    #6,sp
          ENDM 
SETBLOCK: MACRO
          move.l    \1,-(sp)
          move.l    \2,-(sp)
          clr.w     -(sp)
          move.w    #$4a,-(sp)     * SETBLOCK
          trap      #1
          lea       12(sp),sp
          ENDM 
FCREATE:  MACRO
          clr.w     -(sp)          * Normale Datei
          pea       \1             * Dateiname
          move.w    #$3c,-(sp)     * FCREATE
          trap      #1
          addq.l    #8,sp
          ENDM 
FWRITE:   MACRO
          pea       \1             * Adresse
          move.l    \2,-(sp)       * Bytes
          move.w    \3,-(sp)
          move.w    #$40,-(sp)     * FWRITE
          trap      #1
          lea       $c(sp),sp
          ENDM 
FCLOSE:   MACRO
          move.w    \1,-(sp)
          move.w    #$3e,-(sp)     * FCLOSE
          trap      #1
          addq.l    #4,sp
          ENDM 
FOPEN:    MACRO
          clr.w     -(sp)          * Lesen
          pea       \1             * Dateiname
          move.w    #$3d,-(sp)     * FOPEN
          trap      #1
          addq.l    #8,sp
          ENDM 
FREAD:    MACRO
          pea       \1             * Save-Area
          move.l    \2,-(sp)       * Bytes
          move.w    \3,-(sp)
          move.w    #$3f,-(sp)     * FREAD
          trap      #1
          lea       $c(sp),sp
          ENDM 
REINS_VEC:MACRO
          lea       \1,a0          * Vector-Adr.
          move.l    \2,d0          * Alter Inhalt
          bsr       vector_reinstall
          ENDM


progstart:dc.l      $12123456      * TOS-MAGIC 1
          dc.l      0              * TOS-ADRESS
          bra.s     res_mem        * TOS-START
          dc.w      0,$1029,$3847  * ALIGN + MY-MAGIC

********* Einsprung vom Lader aus

          movem.l   d0-d7/a0-a6,-(sp)
          bsr       m_load2        * Parms laden
          SUP_EXEC  installvec(pc) * Prg in VBL
          SUP_EXEC  initoldvec1(pc) * RAMDISK
          SUP_EXEC  initoldvec2(pc) * FILE-Prot
          PRINT     install(pc)
          moveq.l   #-1,d0
          moveq.l   #1,d1
njump:    mulu      #9999,d1
          mulu      #9999,d1
          dbra      d0,njump       * Zeitverzög.
          movem.l   (sp)+,d0-d7/a0-a6
          lea       res_end(pc),a0
          add.l     #zeilen+l,a0   * Ende melden
          rts

********* Programm in die VBL-QUEUE einbinden 
* Einsprung vom TOS nach einem RESET

res_mem:  movem.l   d0-d7/a0-a6,-(sp)
          MALLOC    #80000         * Speicher res.
          lea       res_end(pc),a1
          add.l     #zeilen+1,a1   * berechnen und
          move.l    a1,d1          * verkleinern
          sub.l     d0,d1
          SETBLOCK  d1,d0          * schützen
          bsr       file_reinstall * alter Vektor!
          lea       fileflag(pc),a0 * Protect?
          tst.w     (a0)
          beq.s     res_mem1       * Nein...
          bsr       file_install   * Vektoren

res_mem1: bsr       ramdisk_vec_reinstall * RAMDISK-Vektoren reinstallieren! 
          bsr       initoldvec1    * alte Vektoren

          lea       installflag(pc),a0
          tst.w     (a0)
          beq       no_ramdisk     * keine da

          lea       resident(pc),a0
          tst.w     (a0)
          bne.s     resdisk        * reset-resident!

          lea       installflag(pc),a0 * wenn nicht resident, dann 
          clr.w     (a0)           * Disk abmelden

          move.l    drvbits,d0     * Bit in Sysvar
          clr.l     d1             * löschen
          move.w    driveno(pc),d1
          tst.w     d1
          beq.s     no_ramdisk
          bclr      d1,d0
          move.l    d0,drvbits
          bra.s     no_ramdisk

resdiik:  MALLOC    #-1            * Ber. schützen
          sub.l     #30000,d0      * 30 kB
          MALLOC    d0
          lea       mfree(pc),a0
          clr.l     (a0)           * Je. Reinst.
          move.l    puffer(pc),d1  * Adr. RAMDISK
          add.l     groesse(pc),d1 * +Größe
          sub.l     d0,d1          * - Blockanfang
          SETBLOCK  d1,d0
          move.l    drvbits,d0     * anmelden
          clr.l     d1
          move.w    driveno(pc),d1
          bset      d1,d0
          move.l    d0,drvbits
no_ramdisk:
          movem.l   (sp)+,d0-d7/a0-a6
installvec:
          movem.l   d0/a0/a2,-(sp)
          lea       start(pc),a2   * Start
          move.l    _vblqueue,a0   * Start VBLANK
          move.l    a2,vbl_slot(a0) * in VBL

          move      sr,-(sp)       * Floppy aus
          or.w      #$700,sr
          move.b    #14,$ffff8800.w * Port A
          move.b    $ffff8800.w,d0 * aktuellen Wert holen
          and.b     #$f8,d0
          or.b      #5,d0          * LW A, Seite 0
          move.b    d0,$ffff8802.w * in Port A
          move      (sp)+,sr

          movem.l   (sp)+,d0/a0/a2
          rts
initoldvec1:
          lea       o_bpb(pc),a0   * alter BPB
          move.l    hdv_bpb,(a0)
          lea       o_rw (pc),a0   * alter RW 
          move.l    hdv_rw,(a0)
          lea       o_media(pc),a0 * alter MEDIA
          move.l    hdv_mediach,(a0)
          lea       mybpb(pc),a0   * Vektoren
          move.l    a0,hdv_bpb
          lea       myrwabs(pc),a0
          move.l    a0,hdv_rw
          lea       mymedia(pc),a0
          move.l    a0,hdv_mediach
          rts
initoldvec2:
          lea       trap1_old(pc),a0 * alter TRAP-1-Vektor 
          move.l    $84.w,(a0)
          lea       fileflag(pc),a0
          tst.w     (a0)
          beq.s     kein_fileprotect1 * kein Fileprotect 
          bsr       file_install   * Vektoren
          bra.s     initoldvecend

kein_fileprotect1:
          bsr       file_reinstall * Vektoren
initoldvecend:
          rts

********* Anfang Hauptroutine

          dc.b      'XBRA'
          dc.l      kennung
          dc.l      0
start:    move.l    a0,-(sp)
          lea       speedflag(pc),a0
          tst.w     (a0)           * verlangsamen?
          bne.s     speedwait      * Ja...
timeout:  move.l    (sp)+,a0
          tst.w     $4ee.w         * ALT+HELP?
          beq.s     haupt          * Ja !
          rts

speedwait:move.l    d0,-(sp)
          move.l    speedtime(pc),d0
time1:    nop
          dbra      d0,time1
          move.l    (sp)+,d0
          bra.s     timeout

********* ALT+HELP wurde gedrückt

haupt:    lea       menueflag(pc),a0
          tst.w     (a0)           * Menü anzeigen
          bne.s     schleife       * Nein
          bsr       maus_aus       * Maus aus
          bsr       sichere_screen * Screen si.
menue:    bsr       loesche_screen * Screen lö.
          PRINT     text(pc)       * Text ausgeben

schleife: bsr       inkey          * Tast.-Abfrage
          bset      #5,d0          * Nur Kleinb.

          cmpi.b    #'h',d0        * Hardcopy
          beq       m_hardcopy     * drucken!
          cmpi.b    #'q',d0        * Zurück zum
          beq.s     m_quit         * Programm
          cmpi.b    #' ',d0        * wie Q 
          beq.s     m_quit 
          cmpi.b    #'c',d0        * Synchronisatonl
          beq       m_sync         * 50 <-> 60 Hz
          cmpi.b    #'a',d0        * Menümldg I/O
          beq       m_menueaa
          cmpi.b    #'p',d0        * RESET kalt
          beq.s     m_reset 
          cmpi.b    #'r',d0        * RESET warm
          beq       m_reset1
          cmpi.b    #'d',d0        * RAMDISK
          beq       m_startram
          cmpi.b    #'b',d0        * BOOT-Device
          beq       m_bootdevice
          cmpi.b    #'w',d0        * WRITE-Prot
          beq       m_write 
          cmpi.b    #'s',d0        * SAVE-Default
          beq       m_save
          cmpi.b    #'l',d0        * LOAD-Default
          beq       m_load
          cmpi.b    #'x',d0        * XBRA-List
          beq       m_xbra
          cmpi.b    #'i',d0        * Systeminfo
          beq       m_stspeed
          cmpi.b    #'f',d0        * File-Protect
          beq       m_file
          cmpi.b    #'0’,d0        * Tasten-Code>=0
          bge.s     speed          * Ja !
notspeed: bra       schleife
speed:    cmpi.b    #'9',d0        * Tasten-Code<=9
          ble       speed1         * Ja !
          bra.s     notspeed

********* Zurück zun Programm oder GEM—DESKTOP

m_quit:   lea       menueflag(pc),a0
          tst.w     (a0)
          bne.s     ende
          bsr       hole_screen    * Screen zurück
ende:     move.w    #-1,_dumpflg   * Hardcopy-Flag
          lea       menueflag(pc),a0
          tst.w     (a0)
          bne.s     ende1
          bsr       maus_an        * Maus an
ende1:    rts                      * Zurück

********* reset

m_reset:  lea       progstart(pc),a0 *Kill Magic
          clr.l     (a0)

          REINS_VEC $84,trap1_old(pc) * TRAP-1

          lea       installflag(pc),a0 * RAMDISK
          tst.w     (a0)
          beq.s     m_reset1       * Nein...

          bsr       ramdisk_vec_reinstall

          move.l    drvbits,d0
          clr.l     d1
          move.w    driveno(pc),d1
          tst.w     d1
          beq.s     m_reset1
          bclr      d1,d0
          move.l    d0,drvbits
m_reset1: move.l    _sysbase,a0    * in RESET-Vek.
          jmp       (a0)

********* Ausgabe einer Hardcopy

m_hardcopy:
          lea       menueflag(pc),a0
          tst.w     (a0)
          bne.s     hard1
          bsr       hole_screen
hard1:    move.w    #20,-(sp)      * XBIOS 20
          trap      #14
          addq.l    #2,sp
          bra.s     ende

********* Synchronisation 50 <—> 60 Hz

m_sync:   bchg.b    #1,$ffff820a.w
          bra       menue

********* Verändern der Geschwindigkeit

speed1:   cmpi.b    #'0',d0
          beq.s     speednormal
          clr.l     d1
          move.b    d0,d1
          sub.b     #48,d1         * Taste 1 bis 9
          mulu      #1070,d1
          lea       speedtime(pc),a0
          move.l    d1,(a0)
          lea       speedflag(pc),a0
          move.w    #-1,(a0)
          bra       m_quit
speednormal:
          lea       speedflag(pc),a0
          clr.w     (a0)
          lea       speedtime(pc),a0
          clr.l     (a0)
          bra       m_quit

********* Menüflag invertieren

m_menueaa:lea       menueflag(pc),a0
          not.w     (a0)
          move.w    #-1,_dumpflg
          tst.w     (a0)
          beq       ende1
          bsr       hole_screen
          bsr       maus_an
          bra       ende1

********* BOOT-Device

m_bootdevice:
          lea       menueflag(pc),a0
          tst.w     (a0)
          bne       m_quit
          bsr       loesche_screen
          PRINT     boottext(pc)

          move.l    drvbits,d0     * Drvbits holen
          move.w    #-1,d1         * Startwert, LW
ffdlp:    addq.w    #1,d1          * nächstes LW
          cmp.w     #16,d1         * schon 16?
          beq.s     fende          * ja, Fehler
          btst      d1,d0          * LW inst.?
          beq.s     ffdlp
          movem.l   d0/d1,-(sp)
          add.w     #65,d1         * ASCII
          move.w    d1,-(sp)       * Laufwerk
          move.w    #2,-(sp)
          trap      #1
          addq.l    #4,sp
          PRINT     spaces(pc)
          movem.l   (sp)+,d0/d1
          bra.s     ffdlp
fende:    bsr       inkey
          bclr      #5,d0
          cmp.b     #'Q',d0
          beq       menue
          ext.w     d0
          sub.w     #65,d0
          move.l    drvbits,d1
          btst      d0,d1
          bne.s     fok
          bra.s     fende
fok:      move.b    d0,_bootdev.hi * Laufwerk in LOW- und HIGH-Byte 
          move.b    d0,_bootdev.lo * eintragen. Falls ein gepatchtes TOS 
          bra       menue          * verwendet wird, durch
                                   * MOVE.W D0,_BOOTDEV.HI ersetzen

********* Systeminfo

m_stspeed:lea       menueflag(pc),a0
          tst.w     (a0)
          bne       m_quit
          bsr       loesche_screen

          PRINT     st_speedl(pc)  * Freier Spei.
          MALLOC    #-1
          move.l    d0, d1
          lea       freezahl(pc),a2
          bsr       binasc
          PRINT     freezahl(pc)   * Memory
          PRINT     st_speed2(pc)  * Drives
          bsr       show_protected
          PRINT     st_speed3(pc)  * Massage
          lea       installflag(pc),a0 * Ramdisk vorhanden?
          tst.w     (a0)
          bne       memdisk        * Ja...
          PRINT     line(pc)
          bra.s     st_jump
memdisk:  move.l    groesse(pc),d1
          lea       freezahl(pc),a2
          bsr       binasc
          PRINT     freezahl(pc)
st_jump:  lea       filepmode(pc),a0 * File-Prot.
          move.b    #'O',(a0)+     * = An
          move.b    #'N',(a0)+
          move.b    #' ',(a0)+
          lea       fileflag(pc),a1
          tst.w     (a1)
          bna.s     st_jump1
          move.b    #'F',-(a0)     * Prot, aus
          move.b    #'F',-(a0)
st_jump1: PRINT     fileptext(pc)
          PRINT     filetext1(pc)
          PRINT     fileptext1(pc)
          bsr       wait
          bra       menue
