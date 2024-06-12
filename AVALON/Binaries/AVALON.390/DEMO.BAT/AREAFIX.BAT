; Writes a mail to Areafix every 1st of month and calls the exporter.
;
; Schreibt eine Mail zum Areafix an jeden ersten im Monat und ruft den
; Exporter auf.
; ------------------------------------------------------------------------

DAY?
IFERR# 1 continue

!FILE AREAFIX.TXT
!BASE D:\JETMAIL\MSGS\NETMAIL
!FLAGS PK
!SUBJECT password
!FROM 2:2448/2020.6 Stephan Slabihoud
!SEND 2:2448/2020 Areafix

; Write an announce into logfile.
; Schreibe eine Mitteilung ins Logfile.

LOG Mail to Areafix created.
LOG Mail an den Areafix geschrieben.

; Call export (must be installed in "Find-1").
; Exporter aufrufen (muž installiert sein in "Find-1").

EXEC EXPORT

:continue
