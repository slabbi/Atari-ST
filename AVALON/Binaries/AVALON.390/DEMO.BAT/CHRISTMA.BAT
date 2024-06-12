; Writes a mail to some persons every christmas and calls the exporter.
; The messages will be sent crash.
;
; Sendet einigen Personen zu Weihnachten einen Text und ruft den
; Exporter auf. Die Nachrichten werden "crash" versendet.
; ------------------------------------------------------------------------

MONTH?
IFERR# 12 continue
DAY?
IFERR# 24 continue

!FILE CHRISTMA.TXT
!BASE D:\JETMAIL\MSGS\NETMAIL
!FLAGS PKC
!SUBJECT Merry Christmas
!FROM 2:2448/2020.6 Stephan Slabihoud

!SEND 2:2448/2020 GÅnther Paczia
!SEND 2:2456/95  Rolf Thieme
!SEND 2:2447/0   Klaus-Dieter Thelen

; Write an announce into logfile.
; Schreibe eine Mitteilung ins Logfile.

LOG Christmas mails created.
LOG Weihnachtsnachrichten geschrieben.

; Call export (must be installed in "Find-1").
; Exporter aufrufen (muû installiert sein in "Find-1").

EXEC EXPORT

:continue
