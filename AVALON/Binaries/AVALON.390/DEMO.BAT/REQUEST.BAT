; Request new filelist of 2401/103 every Sunday. Append request to
; existing one.
;
; Requeste die neue Fileliste von 2401/103 jeden Sonntag. H„nge den neuen
; Request an einen bestehenden an.
; ------------------------------------------------------------------------

WEEKDAY?
IFERR# 6 jump
APPEND 09610067.REQ
WRITE NEWFILES
CLOSE
:jump

; You can also use APPEND 1UP02V00.ORT in 4D-mode.
; Im 4D-Mode kann auch APPEND 1UP02V00.ORT verwendet werden.


; It is more convenient to use CONVERT or CONVERT4D:
; Es ist komfortabler CONVERT oder CONVERT4D zu benutzen:

;  WEEKDAY?
;  IFERR# 6 jump
;  CONVERT 0 2401/103
;  APPEND %%0.REQ
;  WRITE NEWFILES
;  CLOSE
;  :jump

; Or when using 4D-addresses:
; Oder bei Benutzung von 4D-Adressen:

;  CONVERT4D 0 2401/103
;  APPEND %%0.ORT
