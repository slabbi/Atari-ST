
typedef struct
{
	uword	zone,
				net,
				node,
				point;
	
	byte	domain[13];
} NLADDRESS;

typedef struct
{
	byte	name[13];
	byte	listfile[13];
	uword	zone;				/* Zone or 0 for multizone */
	ulong	startoffset;
	ulong	length;
} NLDOMAIN;

typedef struct
{
	byte	type;
	uword	value;
} NLINDEX;

typedef struct
{
	byte	sysname[34];
	byte	location[30];
	byte	operator[30];
	byte	phone[40];
	
	uword	hubnode;
	
	byte	maxbaud;		/* Baud/300	*/
	word	modemtype;
	
	uword	flags;
} NLSYSTEM;

word search_bnl_usernode(uword zone, uword net, uword node, uword point,
												 NLSYSTEM *info,byte *domain);
