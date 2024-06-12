%;
%; Sample File Request Failure Template File for BinkleyTerm 2.40
%;
%; The commands available to you are as follows:
%;
%; %;                   In column 1, entire line is a comment, pass nothing
%;                      on; in any other column, ignore remainder of line.
%;                      To send a blank line, <space>%; will suffice.
%;
%; %abort               Don't send a response file.
%;
%; %abort <number>      If <number> matches the status value, don't send
%;                      a response file.
%;
%; %exit                Close the response file and send it as-is.
%;
%; %exit <number>       If <number> matches the status value, close the
%;                      response file and send it as-is.
%;
%; %text <number> text  When %status is seen, substitute remaining text
%;                      on line if status is <number>. '%' substitution
%;                      is allowed within the text if desired. Current
%;                      valid status values are:
%;
%;                      1 = file not found,
%;                      2 = no update necessary,
%;                      3 = password missing or wrong
%;                      4 = file request limit exceeded
%;                      5 = start of no-requests-honored event
%;                      6 = file would exceed request byte limit
%;                      7 = file would exceed time limit
%;                      9 = successful file request
%;
%; %line <number> text  If <number> matches the status value, the text
%;                      will be copied to the .RSP file.
%;
%; %date                Text for current local date.
%;
%; %time                Text for current local time.
%;
%; %bink                Text for "BinkleyTerm (current version)."
%;
%; %mynode              Text for "my" node address.
%;
%; %system              Text for system name.
%;
%; %sysop               Text for sysop name.
%;
%; %yrnode              Text for "your" node address.
%;
%; %request             Actual line in .REQ file which failed.
%;
%; %status              Text which you have defined for failure status.
%;
%; All other text, including % characters which are not followed by
%; recognized keywords, will be copied verbatim to the response file.
%;
%; NOTE: If you play games with the % character, you may get hurt in
%; future versions of BinkleyTerm. It is STRONGLY RECOMMENDED that you
%; not tempt fate by messing with that character.
%;
%;abort 9
%;
%text 1 file not found
%text 2 no update necessary
%text 3 password failure
%text 4 file request limit exceeded
%text 5 event doesn't honor requests
%text 6 file would exceed request byte limit
%text 7 file would exceed time limit
%text 9 Request successful
%;


Node %mynode           %bink            %date  %time
 

%system received a file request made by your system,
%yrnode, at this time. The status of the request is:

%;FILE                           STATUS
%;-------------------------------------------------------------------- 
%request                   

%;line 1 For a complete list of the files available on this board, you may
%;line 1 request FILES and you will be sent a list.
For a complete list of the files available on this board, you may
request FILES and you will be sent a list.

Thank you for your patience.
 
%sysop, System Operator
%system, %mynode
