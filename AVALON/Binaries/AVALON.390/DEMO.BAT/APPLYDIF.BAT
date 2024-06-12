;
; Dieses Batchfile hÑngt eine Diff-Datei an eine Pointliste an und erzeugt
; danach aus der neuen Pointliste eine Userliste fÅr den LED.
; This batchfile applies a diff-file to a pointlist and creates a userlist
; from the new created pointlist for the LED.
;
ECHO=OFF
CLS
;
CHDIR \fido\list
;
EXIST4 pr24diff.*
  LOG Applying PointDiff
  APPLYDIFF d:\jetmail\list\points24.* d:\jetmail\list\pr24diff.* clean
  LOG Converting to PN24LIST.LST
  MAKEUSERLIST p d:\jetmail\list\points24.* d:\jetmail\list\pn24list.lst 2
