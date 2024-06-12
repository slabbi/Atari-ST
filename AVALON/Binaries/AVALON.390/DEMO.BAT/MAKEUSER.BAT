; This batchfile shows, how to convert a node- or pointlist into a
; userlist for the LED.
; Dieses Batchfile demonstriert, wie eine Node- oder Pointliste in
; eine Userliste fÅr den LED konvertiert werden kann.

; Using of parameters:
; Die Parameter werden wie folgt benutzt:
;  %%0 = "n" or "p" for node- or pointlist
;  %%1 = filename of node- or pointlist
;  %%2 = zone (only for 3d-pointlist)
;  %%3 = filename of userlist (output)

:start
CLS
ECHO=OFF
ECHO
ECHO Was soll konvertiert werden?
ECHO
ECHO 1. Nodelist
ECHO 2. Pointlist (3D- or 4D-format)
ECHO
ECHO ESC - End / Ende
:loop
GETKEY
IFERR= 49 menu1
IFERR= 50 menu2
IFERR# 27 loop
EXIT

:menu1
CLS
PRESET 0 n
ECHO Coverting nodelist in userlist / Konvertiere Nodeliste in Userliste
ECHO
ECHO Please select nodelist / Bitte Nodeliste auswÑhlen
FILESELECT 1
IFERR# 0 error1
PRESET 2
GOTO continue

:menu2
CLS
PRESET 0 p
ECHO Coverting pointlist in userlist / Konvertiere Pointliste in Userliste
ECHO
ECHO Please select pointlist / Bitte Pointliste auswÑhlen
FILESELECT 1
IFERR# 0 error1
ECHO Please enter zone / Bitte die Zone eingeben
INPUT 2

:continue
ECHO Please select userlist / Bitte Userliste auswÑhlen
ECHO
FILESELECT 3
IFERR= 2 start
MAKEUSERLIST %%0 %%1 %%3 %%2
GOTO start

:error1
ECHO
ECHO ERROR: File not found / FEHLER: Datei nicht gefunden
GETKEY
GOTO start
