; Dieses File sendet einen lÑngeren Text in eine Area. Der Text wird
; in maximal 8KB groûe Teile zerlegt.
; FÅr mehr Informationen siehe TXT2MSG.BAT.
; Diese Batchdatei sollte auf jedem System ohne Modifikationen laufen!
; (Nur die eigene Adresse unter "!FROM" eintragen!)
;
; This file sends a long text into an area. The text will be splitted
; in 8KB blocks.
; For more information see TXT2MSG.BAT.
; This batch should work on all systems without any modifications!
; (Only enter your own address after the statement "!FROM"!)
; ------------------------------------------------------------------

ECHO=OFF
!CONVERTTEARLINE=ON

:start
CLS
ECHO Choose area: / WÑhle Area:
ECHO
ECHO A. Science-Fiction    [ Subject: "Story: <name> Part: <n>" ]
ECHO B. Netmail            [ Subject: <name> (n)                ]
ECHO C. Other / Andere     [ Subject: <name> (n)                ]
ECHO
ECHO ESC - End / Ende
:loop
GETKEY
IFERR= 97 area_a
IFERR= 98 area_b
IFERR= 99 area_c
IFERR# 27 loop
EXIT

:area_a
PRESET 1 All
PRESET 2 0:0/0
PRESET 3 Story: %filename%   Part: %ext%
PRESET 4 SCIENCE_FICTION
PRESET 5
GOTO continue

:area_b
INPUT 1 To / EmpfÑnger:
INPUT 2 Address / Adresse:
PRESET 3 %filename% (%ext%)
PRESET 4 MAIL
PRESET 5 L
GOTO continue

:area_c
PRESET 1 All
PRESET 2 0:0/0
PRESET 3 %filename% (%ext%)
INPUT 4 Areaname:
PRESET 5
GOTO continue



:continue

; WÑhle File aus
; Choose file

FILESELECT 0

; Nun zerlege das File in 8KB Blîcke. Das gesplittete File bekommt die
; Endungen 001 - ...
; Now split the file into 8KB blocks. The splitted file is named with
; extension 001 - ...

SPLIT %%0 8000 0

; Wir wollen nur die aufgeteilten Teile versenden, so schneiden wir nun die
; Extension ab und versenden <filename>.0??
; We want to send only the splitted parts, so cut the extension and send
; <filename>.0??

CUTEXTENSION 0

; Versende File
; Send file

!AREA %%4
!FILE %%0.0??
!FROM 2:2448/2020.6 Stephan Slabihoud
!SUBJECT %%3
!FLAGS %%5
!SEND %%2 %%1

ECHO
ECHO Delete the original file (J/*)?
ECHO Soll das Originalfile gelîscht werden (J/*)?
ECHO
GETKEY
IFERR= 74 delall
IFERR= 106 delall

; Lîsche die einzelnen Teile
; Delete all spliited parts

DEL %%0.0??
GOTO start
:delall
DEL %%0.*
GOTO start
