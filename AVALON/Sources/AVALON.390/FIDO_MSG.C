#include <aes.h>
#include <portab.h>
#include <string.h>
#include "fido_msg.h"
#include "define.h"
#include "vars.h"

void send_pipe(void);
void send_message(int id,int fm_id,long fm,long command,int extend);
static int send_pipe_now(char *program,int fm_id,long fm_from,long command,int extend);

void send_pipe(void)
	{	if ( mailer==SEMPER )
			{	if ( pipeflags & P_SEMP_REREAD )
					send_pipe_now("SEMPER  ",'FM',FM_AVALON,SEMP_RESCAN,0);
			}
		pipeflags=0;
	}

/*
		char	*programmname,
		int		'FM',
		long	fm_from,
		long	command,
		int		extend
*/

static int send_pipe_now(char *program,int fm_id,long fm_from,long command,int extend)
	{	int id,rc,found=0,type;
		char name[9];

		if (has_appl_search)
			{ rc = appl_search( 0,name,&type,&id );
				while( rc )
					{	if ( !strncmpi(name,program,8) && id!=ap_id )
							{	++found;
								send_message(id,fm_id,fm_from,command,extend);
							}
						rc = appl_search(1,name,&type,&id);
					}
			}
		else
			{ id = appl_find( program );
				if ( id>=0 && id!=ap_id )
					{	++found;
						send_message(id,fm_id,fm_from,command,extend);
					}
			}
		return( found );
	}

void send_message(int id,int fm_id,long fm_from,long command,int extend)
	{	int pipe[8];

		pipe[0] = fm_id;
		pipe[1] = ap_id;
		pipe[2] = 0;
		*((long *) &pipe[3]) = fm_from;
		*((long *) &pipe[5]) = command;
		pipe[7] = extend;
		appl_write(id,16,pipe);
	}
