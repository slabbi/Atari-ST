; This file shows, how to do a update filerequest.
; The archive AVAL_????.??? will only be requested if a new version exists.
; The request will be created every 1st of month.
;
; Dieser Batch demonstriert, wie ein sogenannter UpdateRequest erstellt
; werden kann. Das Archiv AVAL_????.??? wird nur dann requestet, wenn eine
; neuere Version existiert. An jedem 1. im Monat wird der Request erstellt.
; --------------------------------------------------------------------------

DAY?
IFERR# 1 jump
APPEND 09610103.REQ

; Now write UpdateRequest (with Drive\Path\File)
; For more information read the manual of Binkley-Term.
; Nun schreibe den UpdateRequest (mit Laufwerk\Pfad\Datei)
; FÅr mehr Informationen bitte das Handbuch zu Binkley-Term lesen.

WRITE D:\ARCHIVES\AVAL_????.???

CLOSE
:jump

; You can also use APPEND 1UP02V00.ORT in 4D-mode.
; Im 4D-Mode kann auch APPEND 1UP02V00.ORT verwendet werden.
