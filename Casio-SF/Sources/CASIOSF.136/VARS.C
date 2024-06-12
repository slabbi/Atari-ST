#include <tos.h>
#include <aes.h>
#include <stdio.h>
#include "define.h"

int	ap_id,
		divflags=0,
		mid_x,mid_y,
		aes_version,gemdos_version,vdi_handle,
		def_drive,
		vollversion=FALSE,
		sbaudcode=4,ebaudcode=4,port=0,
		konvert=FALSE;

DTA	dta_buffer;

OBJECT *tree1,*tree2,*tree3,
			 *tree4,*tree5,*tree6,
			 *tree7,*tree8,*tree9,
			 *tree10,*tree11,*tree12,
			 *tree13,*tree99,*treepic0;

char def_path[PATH_MAX],
		 sysopname[MAX_STR];

long sysopkey=-1;
