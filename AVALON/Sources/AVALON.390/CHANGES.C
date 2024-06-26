/*******************************************************************/
/*																																 */
/*   A V A L O N ,  die ultimative Shell f�r Fido-Points!					 */
/*																																 */
/*   (c) Stephan Slabihoud 1991,1992,1993,1994										 */
/*																	         ( GFA-BASIC -> NUL )  */
/*																																 */
/*	V2.00 - Erste C-Version von AVALON.														 */
/*	V2.01 - Kleinere Optimierungen.																 */
/*	V2.02 - RSC Bestandteil von AVALON. Speicherbedarf nur 80 KB.	 */
/*	V2.03 - CFGs d�rfen jetzt l�nger als 10K sein.								 */
/*	V2.10 - Spezielle Festlegungen in "Spezial".									 */
/*	V2.11 - Dialogboxenaufbau korrigiert.													 */
/*	V2.12 - Wei�er Bildschirm bei "TOS" und "TTP", sonst graues		 */
/*					Desktop. Meldung, wenn Fileliste nicht vorhanden.			 */
/*					Utility-2 wird korrekt im Session-Setup angezeigt.		 */
/*					Variablenverwaltung ge�ndert. Redraw der Pointauswahl	 */
/*					korrigiert.																						 */
/*	V2.13 - 4KByte weniger Speicherverbrauch und kleinen Bug 			 */
/*					in XWAIT-Redraw entfernt. Fehler in Kommandozeilen-		 */
/*					�bergabe beseitigt.																		 */
/*  V2.20 - Batchinterpreter implementiert.												 */
/*	V2.21	-	EXISTn, CHDIR, X: implementiert.											 */
/*  V2.22 - Bug in der Kopierroutine behoben; ECHO, RENAME.				 */
/*	V2.23	-	Kleinerer Bug in "Edit Areas" beseitigt.							 */
/*	V2.24 -	Wieder ein wenig optimiert.														 */
/*	V2.25 - SCOPY, SMOVE implementiert, kleinere Optimierungen		 */
/*					put_f() Selektierung verbessert (strstr -> strnicmp)	 */
/*					NOTEXIST/NOTEXISTn implementiert.											 */
/*	V2.26 - kleinere Optimierungen; Handbuch �berarbeitet;				 */
/*					ge�nderte Sharewareabfrage.														 */
/*	V2.27 -	Fehler beim Point-Setup entfernt.											 */
/*	V2.30 - �nderungen beim Bildschirmaufbau, ERRORENDOFAVALON.		 */
/*	V2.31 - Bildschirmhandling nochmals ge�ndert (F'Request);			 */
/*					Fehlerauswertung des Batchinterpreter verbessert.			 */
/*	V2.32 -	"Edit AREAS.BBS" verbessert, drei Filebuttons im FReq, */
/*					CFG ge�ndert, CANCEL in "Find programs".							 */
/*	V2.40 - Wahlm�glichkeit "BT2.40"/"BT2.41"/"NoInfo".						 */
/*	V2.41 - kleinere Optimierungen; Wahl "MaxInfo".								 */
/*	V2.50 - Filemanager implementiert.														 */
/*	V2.51 - Cursor in "Edit Areas" zeigt auf freies Feld;					 */
/*					hide/show_mouse in autopoll(); LineA entfernt.				 */
/*	V2.52 - Filemanager-Info implementiert.												 */
/*	V2.53 - Neue SCANCODE Auswertung.															 */
/*	V2.54 - kleinere Optimierungen.																 */
/*	V2.60 - AREAS.BBS: Net/Node als Integer anstelle von Char.		 */
/*					Zone in AREAS.BBS erlaubt; "EXT" in "Edit areas";			 */
/*					Neuer Schutz per "Application AVALON xxxx".						 */
/*	V2.61 - SHELL,HIDEM,SHOWM implementiert.							         */
/*					"Find programs" erlaubt Batchdateien.									 */
/*					Neue Routine: "redraw_one_objc".											 */
/*	V2.70 - Notebook per UNDO, ZoneRequest m�glich.								 */
/*					HDR/MSG-Dateien werden automatisch angelegt.					 */
/*					SWITCH implementiert, Vier Filebuttons im F'Req,			 */
/*					Sicherheitsabfrage QUIT ge�ndert, 77 Zeichen in F'Req. */
/*					Sechs Sessions und Setups, Sechs frei definierbare		 */
/*					Utilities, LoopModus f�r jede Session, *.APP und *.*,	 */
/*					ErrorLevel-Exit (je Programm), EXIT n f�r CLI,				 */
/*					"Find 1" (Program) und "Find 2" (Utility)							 */
/*					"N/P" f�r Areas.bbs. Sessionnamen �nderbar.						 */
/*					"Wait for keypress" in "Spezial". "Select" in S.-Setup */
/*					Defaultbuttons in F'Req f�r Nodes und Filelisten.			 */
/*					Viewer in Filemanager.																 */
/*	V2.71 - Filelistenverwaltung korrigiert. Batchinterpreter			 */
/*					erweitert: "GOTO","IFERRx","CLRERR". BREAK �ber "ESC". */
/*	V2.80 - Filelistenverwaltung erweitert. Wenn "Alternate"			 */
/*					beim Start gedr�ckt, dann kein AutoStartPoll.					 */
/*					Abbruch einer Session �ber "Alternate". Direkter Start */
/*					einer AutoPoll-Session �ber SPACE.										 */
/*	V2.81 - kleiner Bug in "Edit Areas" korrigiert. Nun auch f�r	 */
/*					"SYSOP 2.11"-Filelisten geeignet. Scancode-Abfrage		 */
/*					erweitert. Areas-Editor unterst�tzt Domains.					 */
/*	V2.82 - Kleinere Optimierungen. "AUTOEXEC.BAT" (wird nicht)		 */
/*					abgearbeitet, wenn linke SHIFT-Taste beim Start				 */
/*					gedr�ckt wird.																				 */
/*	V2.83 -	Auswahl des Pfads in "Edit areas" jetzt �ber File-		 */
/*					selector m�glich (Doppelklick auf Pfad).							 */
/*					Fehler in Shareware-Datumsabfrage beseitigt.					 */
/*	V2.90	-	Unbelegte "Programmbuttons" jetzt hell dargestellt.		 */
/*					Infoboxanzeige ver�ndert: "Max" zeigt alle Zeilen an,	 */
/*					">=2.41" alle, ohne Remarks, "Ship" im Filemanager.		 */
/*					Der Pfad der "*.X"-Dateien kann frei definiert werden. */
/*					Searchroutine in der Requestbox verbessert.						 */
/*					Shareware-Datumsabfrage jetzt auch f�r Jahre 2000ff.	 */
/*					DemoKey bis zu einem bestimmten Datum m�glich.				 */
/*					Setup-Texte frei w�hlbar.															 */
/*					"WEEKDAY?","DAY?","OPEN","APPEND","WRITE","CLOSE"			 */
/*					"GETKEY" implementiert.																 */
/*	V3.00	-	MultiTOS Anpassung. Dialoge jetzt in Fenstern.				 */
/*					MainDialog abschaltbar. Kommandozeilen editierbar.		 */
/*					Acht Zeilen AREAS.BBS-Flags pro Area m�glich.					 */
/*					4D/5D-FileRequest, Terminal mit Logfile implementiert. */
/*	V3.10	- Files k�nnen nun gehatched werden.										 */
/*					200 Areas m�glich. KnownInbound/ProtInbound.					 */
/*					FormDo verbessert. SessionSetup erlaubt installierte	 */
/*					Dateien wieder zu entfernen. 3D-TICs einstellbar.			 */
/*					Zwei Zeitangaben f�r AutoStartPoll m�glich.						 */
/*	V3.11 - Keine Bomben mehr (File-Request).											 */
/*					Copy-Buffer-Reservierung ge�ndert f�r MultiTOS.				 */
/*	V3.20 -	Externer AREAS.BBS Editor kann definiert werden.			 */
/*					"N/P" erlaubt jetzt 180 Zeichen anstelle von 120.			 */
/*					BugFix in FORM_DO-Routine. Function TESTE_SETUP				 */
/*					sicherer. File-Request verbessert. Unterst�tzt				 */
/*					AREAS.BBS und DAREAS.BBS. Filename-Extrahierung aus    */
/*					Fileliste verbessert. Redraw verbessert. Schnellere		 */
/*					und sichere Suche in Filelisten. Hatchroutine erlaubt  */
/*					es Namen anstelle von Sysop anzugeben.								 */
/*					Clipping unter MultiTOS korrekt in USERDEF-Routine.		 */
/*	V3.30	-	Netmail kann in "Special" definiert werden.						 */
/*					"Areafix"-Manager implementiert. "Infobox <=2.41"			 */
/*					entfernt. Hatch/Areafix-Mails k�nnen mit "Kill/Sent"	 */
/*					Status versehen werden. Checkboxes implementiert.			 */
/*					Pfade und Netmail in "Special" kann per Doppelklick		 */
/*					definiert werden.	"xredraw_slider()" implementiert.		 */
/*					"AREAS.LST" mit "LINK|NODE", "PASSWORD", "AREAFIX" und */
/*					"REM". "-Fixname" und "-Fixpassword" f�r (D)AREAS.BBS. */
/*					"input()"-Routine verbessert. SHIP nun �ber XHDI.			 */
/*					BugFix in LoadRequest.																 */
/*					Ausgabe des Batchinterpreters/Terminals nun im Window	 */
/*					m�glich. Serieller Port kann ge�ndert werden.					 */
/*					TOS-Ausgaben k�nnen in ein Window umgelenkt werden.		 */
/*					Max.500 Areas, dynamisch verwaltet. Programme k�nnen	 */
/*					in eine PollSession eingef�gt werden.									 */
/*					HELP und ZModem-Transfer im Terminal m�glich.					 */
/*					Snapping von Windows m�glich. Drei weitere Default-		 */
/*					Buttons in "F'Hatch". MultiTOS-Support stark ver-			 */
/*					verbessert, u.a. jetzt hintergrundbedienbar.					 */
/*					VT100 Escapes teilweise implementiert.								 */
/*	V3.11 -	"filelist2filename" verbessert. Sonderzeichen, wie "*" */
/*					und "#", werden in der Datei AVALON.LST �bersprungen.	 */
/*					Max.2000 Areas im AreafixManager werden verwaltet.		 */
/*					"mtxt..."-Variablen entfernt.													 */
/*					BugFix in "poll" bzgl. "Redirection/Find-1/2".				 */
/*					"INS/DEL"-Redraw ge�ndert, Variable "firstcall".			 */
/*					Optimierungen bzgl. "OBJECT *". Es wird jetzt in der	 */
/*					DAREAS.BBS kein Default-Origin und Path erwartet.			 */
/*					"EXT" bis zu 65 Zeichen.															 */
/*					"!xxxx" Kommandos in Batchinterpreter und "PRINT"			 */
/*					implementiert. "Add area"-Funktion im AreafixManager	 */
/*					stark verbessert. Terminal mit Wrap on/off.						 */
/*					Weitere VT100-Codes implementiert.										 */
/*	V3.32	-	"Always center textwindows" eingebaut.								 */
/*					"Fsetdta()" jetzt jedesmal vor "Fsfirst()".						 */
/*	V3.40 - "LOG" in Batchinterpreter eingebaut.							 		 */
/*					"exec EXPORT|IMPORT|CRUNCH|SCAN|MSGEDIT|TXTEDIT|			 */
/*					MAILER|TERMINAL|UTIL1...6" m�glich.										 */
/*					Es kann nun ein Text in die Hatchroutine geladen			 */
/*					werden. Der Text wird in der Mail korrekt umgebrochen. */
/*					"Select node:" jetzt entfernt, daf�r werden jetzt			 */
/*					neun Filelisten direkt verwaltet.											 */
/*					Domain jetzt im AreafixManager erlaubt.								 */
/*					32KB Buffer in Terminal implementiert.								 */
/*					Mails tragen jetzt die korrekte Uhrzeit.							 */
/*					Netmail-Angabe wird mit NETMAIL-Statement aus der			 */
/*					Binkley.Cfg vorbesetzt. "!FILESELECT","FILESELECT",		 */
/*					"CUTEXTENSION","CUTPATH","%%n" implementiert.					 */
/*					"*filename*" entfernt, daf�r "%filename%","%file%" und */
/*					"%ext%" eingebaut. Kommandozeilenbefehle: "NoDelay"		 */
/*					"NoAutoBatch","NoAutoStart","Override","Batch:".			 */
/*					Viele Optimierungen.																	 */
/*	V3.50 - (Shift)Home in Listenanzeige m�glich. Leerzeichen im	 */
/*					Description-Text werden abgeschnitten.								 */
/*					Im Fileselector wird jetzt das aktuelle Verzeichnis		 */
/*					angezeigt und nicht mehr HOMEDIR (Batchinterpreter).	 */
/*					Kopierroutinen um 400% schneller. Ein paar						 */
/*					Optimierungen. Endlich implementiert: "Echte Slider"!	 */
/*					FileRequestBox und AreafixManager zeigen jetzt 20 bzw. */
/*					30 Dateien mehr an. "Big windows" in "Special"				 */
/*					implementiert. Request aus Msgs direkt m�glich!				 */
/*					Filelistenanzahl von 9 auf 27 erh�ht. HatchMails			 */
/*					k�nnen mit einem Header versehen werden.							 */
/*					"Edit areas" modifiziert: Die Adressen werden jetzt		 */
/*					String verwaltet! Alle Setup-Dateien werden jetzt im	 */
/*					Ordner AVALON abgelegt (CFG,WPS,HTC,TNB,LST,*.[0-5]).	 */
/*					"-DAYS 0" wird jetzt auch in AREAS.BBS geschrieben.		 */
/*					Environment TOSRUN wird ausgewertet. Pfadverwaltung		 */
/*					in Batchdateien ge�ndert.	SHIP zeigt korrekte					 */
/*					Kennung an.																						 */
/*	V3.51 - Kleinere Optimierungen.	Fontsize einstellbar.					 */
/*					HELP-Seite im Terminal verbessert. KEY-Verwaltung			 */
/*					verbessert. Neuer Info-Dialog. Redraw-Infobox 				 */
/*					ge�ndert.																							 */
/*	V3.52 - Slider bei gro�en Boxen korrigiert.										 */
/*					SEMPER-Support. TheBox-Support entfernt.							 */
/*					NetworkRequest/NetworkHatch verbessert.								 */
/*					"Use window for terminal/batchinterpreter" entfernt.	 */
/*					AVALON.WPS lautet nun AVALON.WPX.											 */
/*	V3.53 - "-DAYS 0" wird jetzt nur gespeichert, wenn auch "0"		 */
/*					angegeben wurde, wird keine Zahl unter "Days" ein-		 */
/*					getragen, so wird keine "-DAYS" Angabe gespeichert.		 */
/*					Outbound-Info in Filemanager implementiert.						 */
/*					"SAVE" im Hauptdialog wurde nicht neu gezeichnet, wenn */
/*					bt_version==0 war. "REM" in Areafix erlaubt jetzt 20	 */
/*					Zeichen.																							 */
/*	V3.60 - Internes Terminal kann �ber ALT-X aufgerufen werden		 */
/*					und ist bei der Zeichenausgabe �ber 150% schneller.		 */
/*					Bugfix in der Speicherverwaltung. Outbound-Info bombt	 */
/*					nicht mehr und wurde verbessert. AVALON fragt jetzt		 */
/*					nach, ob es ein zweites mal gestartet werden soll.		 */
/*					Defaultpfad in der AutoAdd-AreaFix-Box jetzt vor-			 */
/*					belegt. "SessionSetup" zeigt jetzt an, welches				 */
/*					Programm undefiniert ist. Wird eine "lokale" �nderung	 */
/*					vorgenommen, so wird dieses auch �bernommen, wenn die	 */
/*					Kommandozeile nicht ver�ndert wurde! Redraw aller			 */
/*					Buttons im Hauptdialog, wenn die Semper-Configdatei		 */
/*					geladen wurde. UNDO hat jetzt die gleiche Funktion wie */
/*					der CLOSER (au�er im MainDialog). HatchDialog mu� �ber */
/*					EXIT verlassen werden.																 */
/*	V3.61 - Wird im Hauptdialog der FULLER angew�hlt, so wird der	 */
/*					Hauptdialog von Avalon verkleinert (ICONIFY) oder			 */
/*					wieder ge�ffnet. Denselben Effekt hat CNTRL+"*" (auf	 */
/*					dem Zehnerblock). Es werden im Terminal jetzt alle 15	 */
/*					Baudraten unterst�tzt. Avalon legt jetzt eine Backup-	 */
/*					Datei f�rs Configfile an. Hatchareaname darf jetzt		 */
/*					35 Zeichen umfassen.																	 */
/*	V3.62	- "N/P" wieder anw�hlbar. 15 Programme im Programmanager */
/*					installierbar. CFG ab V3.40.													 */
/*	V3.63 - Bugfixes: Beim Programmende wurde immer "Change in		 */
/*					program-manager" angezeigt. Redirect funktionierte		 */
/*					nicht im Programm-Manager. Dialogpositionen werden		 */
/*					wieder gesichert. Slider im AreafixManager wird beim	 */
/*					Suchen nun mitgef�hrt. "-Originalecho" wird nicht mehr */
/*					als "-Origin lecho" interpretiert. DTE-Rate wird im		 */
/*					Terminal korrekt angezeigt. Erstinstallation					 */
/*					vereinfacht. Die Darstellung der Icons sollte jetzt in */
/*					jeder Aufl�sung korrekt sein. OutboundInfo zeigt			 */
/*					Inhalt von Flowfiles an.															 */
/*	V3.64 - RETURN beendet SystemInfo-Dialog.											 */
/*	V3.70 - APPLYDIFF,MAKEUSERLIST,SPLIT,INPUT,PRESET,%%e,WECHO,	 */
/*					DATE?,TIME?,MONTH?,YEAR?,HOUR?,MINUTE?,ECHO=FULL			 */
/*					implementiert.																				 */
/*					Bugfix: Im Programm-Manager war �fter "Redirect"			 */
/*					selektiert. DefaultSetup f�r Session wieder w�hlbar.	 */
/*					TOSRUN wieder entfernt.																 */
/*	V3.71 -	Environments wurden beim Direkt-Start wieder richtig	 */
/*					gesetzt. Konfigfile nun kompatibel zu Avalon und			 */
/*					Avalon4Semper. Batchfiles konnten in Sessions nicht		 */
/*					�ber Buttons eingebunden werden. CONVERT(4D)					 */
/*					implementiert. "Search" in EditAreas.									 */
/*	V3.72 - EXTRACT implementiert. "Special->Buffer screen output" */
/*					implementiert. Bugfix f�r Filelisten gr��er 2.4MB.		 */
/*					ICFS-Protokoll implementiert. 5000 anstatt 2000 Areas. */
/*					AreafixManager �berspringt einstellige Symbole.				 */
/*	V3.73 - "FileManager->MsgBaseInfo" implementiert.							 */
/*					AreafixManager wertet nun Adressen hinter Areanamen		 */
/*					besser aus. AreafixManager verwaltet nun bis zu 7500	 */
/*					Areanamen (wird jetzt auch angezeigt). Zwei kleine		 */
/*					Bugfixes.																							 */
/*	V3.74 - F'Hatch etwas �berarbeitet.														 */
/*																																 */
/*******************************************************************/
