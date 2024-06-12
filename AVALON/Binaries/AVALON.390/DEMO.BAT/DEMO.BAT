ECHO
ECHO  AVALON Demo-Batch (c) Stephan Slabihoud 1991,1992,1993,1994
ECHO
;
LOG Running AutoPoint-Batch
;
CHDIR \fido\outbound
EXIST6 *.z
 TYPE *.z
 ECHO
 ECHO BadWazoo-Session! Exit Batchfile with errorcode 999
 ECHO
 PAUSE
 EXIT 999
;
; Kill all status files
;
CHDIR \iosmail\transfer\outbound
EXIST *.$$?
 DEL *.$$?
;
; Change directory to E:\FIDO\INBOUND
;
CHDIR \fido\inbound
;
; Unpacks filelist (FILES.LZH) to (E:\FIDO\LIST\) and deletes archive
;
EXIST2 files.lzh
 DEXEC files.lzh \fido\lharc.ttp x -m files.lzh e:\fido\list\ *.*
;
; Another filelist packed with arj
;
EXIST2 22401053.arj
 DEXEC 22401053.arj \iosmail\unarj.ttp e 22401053.arj
 MOVE 22401053.* \iosmail\list\filelist\*.*
;
; Attention! DEXEC only deletes a file, when the errorcode is ZERO!
;
; Unpack Point-List/-Diff and Node-List/-Diff, move files to E:\FIDO\LIST\
;
EXIST3 pr24diff.a*
 LOG Unpacking and moving PR24DIFF
 DEXEC pr24diff.a* \fido\arc.ttp x pr24diff.a* *.*
 RENAME pr24diff.* \fido\list\
;
EXIST3 points24.*
 DEXEC points24.* \fido\unarj.ttp x points24.*
 DEL \fido\list\points24.*
 MOVE points24.* \fido\list\
;
EXIST2 nodelist.*
 DEXEC nodelist.* \fido\unarj.ttp x nodelist.*
 MOVE nodelist.* \fido\list\
EXIST2 node_brd.*
 DEXEC node_brd.* \fido\unarj.ttp x node_brd.*
 MOVE node_brd.* \fido\list\
EXIST2 nodediff.*
 DEXEC nodediff.* \fido\arc.ttp x nodediff.* *.*
 MOVE nodediff.* \fido\list\
;
; Change directory to E:\FIDO\LIST
;
CHDIR \fido\list
;
; Update PointList, when PointDiff is found (add "2:" to Userlist)
;
; Old version using external compiler and converter:
;
; EXIST4 pr24diff.*
;  LOG Applying PointDiff
;  EXEC compdiff.ttp points24.* pr24diff.* clean
;  LOG Converting to PN24LIST.LST
;  EXEC pn24conv.ttp s points24.* pn24list.lst 2
;
; New version using internal batch-functions:
;
EXIST4 pr24diff.*
  LOG Applying PointDiff
  APPLYDIFF d:\jetmail\list\points24.* d:\jetmail\list\pr24diff.* clean
  LOG Converting to PN24LIST.LST
  MAKEUSERLIST p d:\jetmail\list\points24.* d:\jetmail\list\pn24list.lst 2
;
; Convert RSP-Files to Msg using FILE2MSG
;
; EXIST \fido\inbound\*.rs?
;  EXEC \fido\spezial\file2msg.ttp INBOUND\*.RS? e:\fido\msgs\system 2:2447/124.6
;
; The same procedure using internal converter
;
EXIST7 \fido\inbound\*.rs?
 !FILE \fido\inbound\*.rs?
 !AREA SYSTEM
 !FROM 2:2447/124.6 RSP->MAIL Converter
 !SUBJECT Respondfile received
 !FLAGS
 !SEND 2:2447/124.6 Stephan Slabihoud
 DEL \fido\inbound\*.rs?
;
; Archive Netmail Msgs in ".ABK" (Avalon Backup)
;
EXEC \fido\spezial\arc_it.ttp n "Stephan Slabihoud" E:\FIDO\MSGS\NETMAIL
EXEC \fido\spezial\arc_it.ttp e -x "Stephan Slabihoud" E:\FIDO\MSGS\WICHTIG
;
; Create statistic about AREAS.BBS
;
EXEC \iosmail\spezial\bbs_info.ttp "Stephan Slabihoud" e:\iosmail\areas.bbs >>e:\iosmail\binkley.log
;
PAUSE
