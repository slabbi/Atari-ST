; This batch shows how to send textfiles...
;
; Dieser Batch demonstriert, wie Textfiles versand werden kînnen...
; ------------------------------------------------------------------

; Define file to send (wildcards are allowed)
; Definiere zu versendenes File (Wildcards sind erlaubt)

!FILE C:\FILE_???.TXT

; Instead of using "!FILE" you can also use the fileselector:
;   FILESELECT 0
;   !FILE %%0
; or better
;   !FILESELECT
; Anstelle "!FILE" zu benutzen, kann auch der Fileselektor verwendet werden:
;   FILESELECT 0
;   !FILE %%0
; oder besser
;   !FILESELECT

; Define area (this area must be defined in AREAS.BBS)
;   or define the msgbase with e.g. "!BASE E:\JETMAIL\MSGS\NETMAIL"
; Angabe der Area (diese MUû in der AREAS.BBS abgegeben sein)
;   oder Angabe des Pfads mit MsgBase (z.B. "!BASE E:\JETMAIL\MSGS\NETMAIL)

!AREA MAIL

; Your address and name
; Die eigene Adresse und Name

!FROM 2:2448/2020.6 Stephan Slabihoud

; The subject. You can use "%filename%","%file%" and "%ext%".
; ("%filename%" will be replaced with the filename without extension,
; "%file%" with the filename with extension and "%ext%" with the extension
; that is interpreted as number (or zero when the extension is no number)).
; Der Betreff. In diesem kann "%filename%","%file%" und "%ext%" angegeben
; werden. ("%filename%" wird ersetzt durch den Filenamen ohne Extension,
; "%file%" mit dem Filenamen mit Extension und "%ext%" durch die Extension,
; welche als Zahl interpretiert wird (0 wenn die Extension keine Zahl ist).

!SUBJECT Filename: %file%

; Flags ("Local" is always set)
;    P - Private (must be set in NETMAIL)
;    C - Crash
;    S - Sent
;    W - Send-With-File (subject contains filename)
;    K - Kill-If-Sent
;    H - Hold
; Flags ("Local" wird immer gesetzt)
;    P - Private (muû in der Netmail gesetzt werden)
;    W - Send-With-File (der Betreff muû den Filenamen beinhalten)

!FLAGS PCK

; Now send the text to following persons
; Nun sende den Text an folgende Personen

!SEND 2:2448/2020 Guenther Paczia
!SEND 2:243/6007 Joerg Spilker
!SEND 2:2456/95 Rolf Thieme

; Now change to another area
; Nun Ñndere die Area

!AREA REQUEST.ST

; It is a echomail area, so no flag - except "Local" - must be set.
; Es ist eine EchoArea, so kein anderes Flag - auûer "Local" - wird gesetzt.

!FLAGS

; Now use the subject
; Nun Ñndere den Betreff

!SUBJECT New file received

; It is not necessary to define a valid address in echomail areas.
;   File is the same as above.
; Es ist nicht nîtig eine gÅltige Zieladresse in EchoAreas anzugeben.
;   Das File ist das selbe wie oben.

!SEND 0:0/0 All

PAUSE
