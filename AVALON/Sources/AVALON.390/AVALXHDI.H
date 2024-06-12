#ifndef __XHDI__
#define __XHDI__

#define XH_TARGET_STOPPABLE 0x00000001L
#define XH_TARGET_REMOVABLE 0x00000002L
#define XH_TARGET_LOCKABLE  0x00000004L
#define XH_TARGET_EJECTABLE 0x00000008L
#define XH_TARGET_RESERVED  0x80000000L

/* Inquire protocol version number */
int XHGetVersion (void);

/* Inquire informations about one target */
long XHInqTarget (int major,int minor,long *block_size,
             			long *device_flags,char *product_name);

/* Stop or start target */
long XHStop (int major,int minor,int do_stop,int key);

#endif
