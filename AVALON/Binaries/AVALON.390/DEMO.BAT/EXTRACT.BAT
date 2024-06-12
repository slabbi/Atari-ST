; This batchfile creates a region 24 and 25 nodelist
;
; Dieses Batchfile erzeugt eine Region 24 und 25 Nodeliste

CLS
ECHO=OFF
ECHO
ECHO Please select nodelist / Bitte nodeliste ausw„hlen
ECHO
FILESELECT 1
IFERR# 0 error
ECHO Please select outputlist / Bitte Ausgabefile w„hlen
ECHO
FILESELECT 2
IFERR= 2 error

EXTRACT %%1 %%2 24 25

:error
