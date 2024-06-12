int init(void)	/* Casio zum Empfang auffordern */
	{	out(13);
		out(10);
		return TRUE;
/*X*for (;;)
			{	if (askinp(1))
					{	if (inp(1)==0x11)
							return TRUE;
					}
				if (askinp(2))
					{	key=inp(2);
						if (key==27)
							return FALSE;
						if (key==' ')
							{	out(13);
								out(10);
							}
					}
			} */
	}
