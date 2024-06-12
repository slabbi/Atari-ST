/******************************************************************/
/* DEFINE.H fr AVALON																						*/
/******************************************************************/

#ifndef __CA_DEFINE__
#define __CA_DEFINE__

#define OB_X(tree,index)			tree[index].ob_x
#define OB_Y(tree,index)			tree[index].ob_y
#define OB_W(tree,index)			tree[index].ob_width
#define OB_H(tree,index)			tree[index].ob_height
#define OB_SPEC(tree,index)		tree[index].ob_spec
#define OB_TYPE(tree,index)		tree[index].ob_type
#define OB_FLAGS(tree,index)	tree[index].ob_flags
#define OB_STATE(tree,index)	tree[index].ob_state
#define MAX(x,y)	((x)>(y)) ? (x) : (y)
#define MIN(x,y)	((x)<(y)) ? (x) : (y)
#define BOOLEAN(x)	(x) ? TRUE : FALSE

#define FALSE 					0
#define TRUE						1
#define	EOS							'\0'
#define MAX_STR					256

#define AVALONNAME			"CASIO-SF <-> Atari-ST"
#define D_USESMART			1

/*	AES-Abh„ngigkeiten		*/
#define XFSELECT			0x140				/* Erweiterter Fileselector			*/

/*	GEMDOS-Abh„ngigkeiten	*/
#define XRSCONF				0x1500			/* Rsconf() - Baudratenabfrage	*/

#endif
