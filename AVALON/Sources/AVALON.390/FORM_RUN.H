#include <aes.h>

#define XFMD_INIT			0
#define XFMD_START		1
#define XFMD_GROW			2
#define	XFMD_DIALOG		3
#define	XFMD_SHRINK		4
#define XFMD_FINISH		5
#define XFMD_EXIT			6

extern int	formrun_msgbuf[8];

extern int	xform_do(int handle,OBJECT *tree,OBJECT *menutree,int start_field,long time,int (*trans_key)(int,int),int *nx,int *ny,int *nw,int *nh);
extern int	xform_dial(OBJECT *tree,int mode,int *handle,int *nx,int *ny,int *nw,int *nh);
extern int	NULLFUNC(int,int);
extern void xredraw_slider(OBJECT *tree,int handle,int sliderbox,int back,int front,long max,long count,int page,int redraw);
extern void	redraw_objc(OBJECT *tree, int index, int handle);
extern void	redraw_one_objc(OBJECT *tree, int index, int handle);
extern int	rc_intersect(int *x1,int *y1,int *w1,int *h1,int *x2,int *y2,int *w2,int *h2);
