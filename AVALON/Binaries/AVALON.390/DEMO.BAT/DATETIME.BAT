; This batchfiles demonstrates how to use parameters
; Dieses Batchfile zeigt, wie man die Parameter benutzen kann
;
ECHO=OFF

ECHO ESC to quit... / ESC bricht ab...

:loop

; Today's date (YYYYMMDD). Store it in parameter #1
; Heutiges Datum (JJJJMMTT) in Parameter #1 speichern

DATE?
PRESET 1 %%e

; Time (HHMMSS)
; Uhrzeit (HHMMSS)

TIME?

; Print values without linefeed
; Werte ohne Zeilenvorschub anzeigen

WECHO Datum: %%1  - Zeit: %%e

GOTO loop