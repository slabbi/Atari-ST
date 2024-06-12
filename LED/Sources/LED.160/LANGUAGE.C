#include "ccport.h"
#include "ledgl.h"
#include "ledfc.h"
#include "language.h"

#define LANGUAGEVERSION		109

ALERT_ENTRY msgtxt[MAXLANGUAGE];

void language_speicher_freigeben(void)
{	word i;
	for(i=0;i<MAXLANGUAGE;i++)
	{	if (msgtxt[i].text)		free(msgtxt[i].text);
		if (msgtxt[i].button)	free(msgtxt[i].button);
	}
}

word read_language(byte *file)
{	byte tmp[512],*p1,*p2;
	FILE *fp;
	word cnt=0,stateno;
	
	memset(msgtxt,EOS,MAXLANGUAGE*sizeof(ALERT_ENTRY));

	fp=fopen(file,"rb");
	if (!fp)
	{	FormAlert(1,1,"Cannot open language file|"," [Abort ");
		return FALSE;
	}

	fgets(tmp,511,fp);
	if (atoi(tmp)<LANGUAGEVERSION)
	{	fclose(fp);
		FormAlert(1,3,"Old language file!|..."," [Abort ");
		return(FALSE);
	}
	if (atoi(tmp)>LANGUAGEVERSION)
	{	fclose(fp);
		FormAlert(1,3,"Wrong language file!|..."," [Abort ");
		return(FALSE);
	}
	while((fgets(tmp,511,fp))!=NULL && cnt<MAXLANGUAGE)
	{	p1=skip_blanks(tmp);
		if (*p1==';' || *p1==EOS) continue;

		sscanf(p1,"%u,%u,%u",&stateno,&msgtxt[cnt].defbut,&msgtxt[cnt].icon);
		p1=strchr(p1,'\"')+1;
		p2=strchr(p1,'\"');

		if (stateno!=cnt || !p1 || !p2)
		{	fclose(fp);
			sprintf(tmp,"Error in language file|in line %u|",cnt+1);
			FormAlert(1,3,tmp," [Abort ");
			return(FALSE);
		}

		*p2=EOS;
		if (*p1)
			msgtxt[cnt].text=strdup(p1);
		else
			msgtxt[cnt].text=NULL;

		p1=strchr(p2+1,'\"')+1;
		p2=strrchr(p1,'\"');

		if (!p1 || !p2)
		{	fclose(fp);
			sprintf(tmp,"Error in language file|in line %u|",cnt+1);
			FormAlert(1,3,tmp," [Abort ");
			return(FALSE);
		}
		*p2=EOS;

		if (*p1 && *p1!='!')
			msgtxt[cnt].button=strdup(p1);
		else
			msgtxt[cnt].button=NULL;
		cnt++;
	}
	if (cnt!=MAXLANGUAGE)
	{	fclose(fp);
		sprintf(tmp,"Expecting %u entries|in language file.|Actually one has %u!|",MAXLANGUAGE,cnt);
		FormAlert(1,3,tmp," [Abort ");
		return(FALSE);
	}
	fclose(fp);
	return TRUE;
}
