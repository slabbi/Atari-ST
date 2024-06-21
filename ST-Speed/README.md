Veröffentlicht in ST-Computer 03/1991 "ST-Speed: Flexibles Utility"

<h1>ST-Speed: Flexibles Utility, Teil 1</h1>

<p><strong>Utilities
 - kleine, aber oft sehr hilfreiche Hilfsprogramme, gibt es viele, aber 
nicht immer sind diese auch brauch- oder leicht erweiterbar (weil z.B. 
kein Source-Code vorhanden ist). Bei diesem Utility ist beides 
gewährleistet: leichte Erweiterbarkeit (Assembler-Kenntnisse 
vorausgesetzt) und nützliche Funktionen für Spieler, Anwender und 
Programmierer.</strong></p>

<p>Aufgerufen wird ST-Speed mit der Tastenkombination ALT-HELP. Das 
Utility selber besteht aus zwei Teilen: LADER und HAUPTPROGRAMM. Der 
Lader sorgt dafür, daß das Hauptprogramm resetresident im Speicher 
gehalten werden kann. Der Lader ist leider nicht auf meinem eigenen Mist
 gewachsen (warum sollte man das Rad auch ein zweites Mal erfinden?), 
sondern wurde [ 1 ] entnommen. Wer genau wissen möchte, wie dieser Lader
 funktioniert, sollte dort nachlesen. Für uns ist nur wichtig, daß das 
Hauptprogramm frei verschiebbar im Speicher sein muß.</p>

<p>Den Lader finden Sie im Listing 1. Die Konstante MYMAGIC = $10293847 
sorgt dafür, daß das Hauptprogramm ST-SPEED auch nur einmal installiert 
wird. Im Listing 2 finden Sie den Grundaufbau des Utilities. Sie werden 
feststellen, daß einige Zeilen noch fehlen, d.h. Sie können zwar schon 
damit beginnen, das Listing abzutippen (stöhn...), funktionieren wird es
 aber erst ab Ende des dritten Teils. Am besten warten Sie mit dem 
Abtippen bis zum dritten Teil, denn wenn Sie einige Funktionen nicht 
brauchen, z.B. wenn Sie schon Ihre Lieblings-RAM-Disk haben, dann 
brauchen Sie die entsprechenden Zeilen im Source-Text natürlich nicht 
mit abzutippen.</p>

<h1>Der Aufbau</h1>

<p>In den ersten 100 Zeilen finden Sie zunächst einige Makros und 
Konstantendefinitionen. Jetzt wird es interessant: Es existieren zwei 
Einsprungadressen, die eine für den Lader, die andere fürs TOS. Ab Zeile
 107 (Einsprung vom Lader aus) wird zunächst die Voreinstellung von 
Diskette geladen. Danach wird ST-SPEED in die VBL-Queue eingetragen, und
 alle Vektoren, die später verbogen werden, werden nach dem 
XBRA-Verfahren gesichert.</p>

<p>Die zweite Einsprungadresse (Zeilen 125 ff.) wird vom TOS bei jedem 
RESET benutzt. Hier sorgt man zunächst dafür, daß der Speicher, den 
ST-SPEED benutzt, nicht wieder ans GEMDOS zurückgegeben wird (ST-SPEED 
bleibt somit resident). Danach installiert man alle Vektoren, sofern 
benötigt (siehe Listing). Soll die RAM-Disk reset-resident sein, wird ab
 Zeile 164 dafür gesorgt, daß auch dieser Speicherbereich nicht wieder 
ans GEMDOS zurückgegeben wird. Achtung! Nach einem RESET läßt sich eine 
installierte Ramdisk nicht mehr aus dem System rausschmeißen. Das liegt 
daran, daß GEMDOS-Speicherbereiche nicht in beliebiger Reihenfolge 
wieder freigegeben werden können [2].</p>

<h1>Die Hauptroutine</h1>

<p>Die Hauptroutine (Zeilen 225 ff.) ist in der VBL-Queue des Rechners 
verankert und wird alle 1/50, 1/60 oder 1/71 Sekunde (je nach 
Videomodus) abgearbeitet. Falls der Computer verlangsamt werden soll, 
wird eine Warteschleife ausgeführt, weiterhin prüft man, ob die 
Tastenkombination ALT-HELP gedrückt wurde. Wenn dies der Fall war 
($4ee.w enthält eine 0), erfolgt die Verzweigung zum Hauptmenü.</p>

<p>Im Hauptmenü findet zunächst eine Überprüfung statt, ob ein Menü 
überhaupt auf dem Bildschirm angezeigt werden soll. Das ist bei einigen 
Spielprogrammen erforderlich, da diese Shapes einen Interrupt ausgeben 
und ein Menü nur den Bildschirmaufbau durcheinander bringen würde.</p>

<p>In der Schleife ab Zeile 256 wird die gedrückte Taste ausgewertet und
 in ein entsprechendes Unterprogramm verzweigt. Wollen Sie eigene 
Routinen einfügen, gehen Sie wie folgt vor:</p>

<p>Innerhalb der Schleife fügen Sie die Zeilen</p>

<pre><code>CMPI.B  #'x',D0
(x = Kleinbuchstabe oder Zahl)
BEQ     Unterprogramm
</code></pre>

<p>ein. Ein Unterprogramm hat nun folgenden Aufbau:</p>

<p>Falls der Bildschirm vor dem Ausführen der Routine restauriert werden
 muß und am Ende der Routine nicht zum Hauptmenü zurückgesprungen werden
 soll, z.B. bei einer Hardcopy-Routine:</p>

<pre><code>Unterprogramm:
    LEA     menueflag(PC),A0 
    TST.W   (A0)
    BNE.S   Up1
    BSR     hole_screen
Up1:
    &lt;... Ihre Routine ...&gt;
    BRA ende
</code></pre>

<p>Die Routine soll ausgeführt werden, und es soll zum Hauptmenü zurückgesprungen werden.</p>

<pre><code>Unterprogramm:
    &lt;... Ihre Routine ...&gt;
    BRA menue
</code></pre>

<p>Das Unterprogramm benutzt den oberen Bildschirmbereich, der auch von 
ST-SPEED benutzt wird (12 Zeilen), für eigene Ausgaben. Das 
Unterprogramm darf nur ausgeführt werden, wenn auch eine Menüausgabe 
erfolgen darf.</p>

<pre><code>Unterprogramm:
    LEA     menueflag(PC),A0
    TST.W   (A0)
    BNE     m_quit
    BSR     loesche_screen
    &lt;... Ihre Routine ...&gt; 
    BRA     menue
oder BRA    ende
</code></pre>

<p>Zu den einzelnen Unterprogrammen in diesem Teil gibt es nicht mehr 
viel zu sagen, da sie ausreichend gut im Source-Text beschrieben sind.</p>

<p>Im zweiten Teil beschäftigen wir uns mit den RAM-Disk-Routinen, die 
es u.a. auch erlauben, die RAM-Disk schreibzuschützen. Weiterhin stelle 
ich Ihnen den XBRA-Lister vor.</p>

<p>[1] Zeitschrift c't, Ausgabe 6/89, „Residenter Wachhund“</p>

<p>[2] Atari-ST Profibuch, H.-D. Jankowski / J.F. Reschke / D. Rabich, Sybex Verlag</p>

<h1>Folgende Möglichkeiten bietet ST-Speed</h1>

<table>
<thead>
<tr>
  <th>Taste</th>
  <th>Beschreibung</th>
</tr>
</thead>
<tbody>
<tr>
  <td>0-9</td>
  <td>den Computer verlangsamen (klappt besonders gut in der mittleren 
und niedrigen Auflösung). Hiermit werden einige Spielprogramme einfacher
 zu spielen.</td>
</tr>
<tr>
  <td>A</td>
  <td>die Anfangsmeldung ein- und ausschalten (manche Spielprogramme 
setzen Shapes innerhalb einer Interrupt-Routine auf dem Bildschirm. 
Damit es zu keinen Kollisionen zwischen ST-SPEED und dem Spielprogramm 
kommt, kann hiermit die Menümeldung ausgeschaltet werden. Wichtig! Es 
können jetzt nur noch folgende Funktionen aufgerufen werden: 
0-9,A,C,H,P,R,Q,S,L).</td>
</tr>
<tr>
  <td>B</td>
  <td>das Boot-Laufwerk ändern. Für alle Festplattenbesitzer besonders 
interessant, die von beliebigen Partitionen booten möchten (auch von 
einer reset-residenten RAM-Disk).</td>
</tr>
<tr>
  <td>C</td>
  <td>schaltet zwischen 50 und 60 Hertz um.</td>
</tr>
<tr>
  <td>D</td>
  <td>Einrichten einer RAM-Disk (auch reset-resident)</td>
</tr>
<tr>
  <td>I</td>
  <td>gibt den freien Speicher, Informationen über den Schreibschutzstatus der Laufwerke und die Größe der installierten RAM-Disk aus.</td>
</tr>
<tr>
  <td>H</td>
  <td>druckt eine Hardcopy</td>
</tr>
<tr>
  <td>P</td>
  <td>Kalt-Start (RESET) (ST-Speed + RAM-Disk werden gelöscht)</td>
</tr>
<tr>
  <td>Q</td>
  <td>Utility beenden</td>
</tr>
<tr>
  <td>R</td>
  <td>Warmstart (RESET) (ST-Speed + RAM-Disk bleiben im Speicher)</td>
</tr>
<tr>
  <td>W</td>
  <td>Hiermit kann man beliebige Laufwerke mit einem Schreibschutz versehen (auch die RAM-Disk).</td>
</tr>
<tr>
  <td>S</td>
  <td>speichert aktuelle Informationen über:<br>- Einschaltmeldung (A)<br>- Schreibschutz (W)<br>- Fileprotect (F)</td>
</tr>
<tr>
  <td>L</td>
  <td>Lädt gespeicherte Informationen. Die Informationen werden auch 
beim ersten Programmstart geladen (z.B. bei dem Start aus dem 
AUTO-Ordner).</td>
</tr>
<tr>
  <td>X</td>
  <td>Listet alle im Speicher vorhandenen XBRA-Programme auf (wichtig: 
Es werden nur Programme angezeigt, die Vektoren im Adreßbereich $8 - 
$1000 verbiegen).</td>
</tr>
<tr>
  <td>F</td>
  <td>Man kann verhindern, daß Programme unerlaubt auf andere zugreifen 
(Virenschutz). Das ist mit FILEPROTECT möglich. Es gibt zwei Modi: Soft 
und Hard. Im Soft-Mode werden nur die Betriebssystemfunktionen Fcreate, 
Fopen (Schreiben und Lesen+Schreiben), Fdelete, im Hard-Mode zusätzlich 
Fopen (Lesen) und Pexec abgefangen.</td>
</tr>
</tbody>
</table>
