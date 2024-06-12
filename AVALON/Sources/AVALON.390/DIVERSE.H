#include <aes.h>
#include <tos.h>
#include <fido.h>

extern int	xFsfirst(const char *filename,int attr);
extern int	xFsnext(void);

extern int	fileselect(char *cin_path,char *csel_file,char *cext,char *titel);
extern void do_select(char *defaultprg, int cindex, OBJECT *ctree, int maxstr);
extern void	delete_extension(OBJECT *tree,int index);

extern long julian(int tag,int monat,int jahr);
extern void	getpfad(char *fname, char *pfadret);
extern void	trim(char **dest);
extern char *trimstr(char *dest);
extern void	trimright(char *dest);
extern char	*mergeaddr(char *addr,int zone,int net,int node,int point,char *domain);
extern void	scanaddr(char *addr,int *zone,int *net,int *node,int *point,char *domain);
extern void	input(char *string, int max, FILE *ftype);
extern void	output(char *string, FILE *ftype);
extern int	exist(char *filename);

extern void	put_text(OBJECT *ctree, int cindex, char *txt, int maxlen);
extern void	put_template(OBJECT *ctree, int cindex, char *text, char *tmplt, char *valid);
extern void	put_string(OBJECT *ctree, int cindex, char *txt, int maxlen);
extern char	*get_text(OBJECT *ctree, int cindex);
extern int	is_state(OBJECT *ctree, int cindex, int maske);
extern void	do_state(OBJECT *ctree, int cindex, int maske);
extern void	undo_state(OBJECT *ctree, int cindex, int maske);
extern void do_xstate(OBJECT *tree, int maske, ...);
extern void undo_xstate(OBJECT *tree, int maske, ...);
extern void do_xflags(OBJECT *tree, int maske, ...);
extern void undo_xflags(OBJECT *tree, int maske, ...);

extern int	log_it(int typ, char *text);

extern void file2addr(char *file,int *net,int *node);
extern void file2addr36(char *file,int *net,int *node,int *point);
extern char *addr2file(unsigned int net,unsigned int node);
extern char *addr2file36(unsigned int net,unsigned int node,unsigned int point);
extern void delete_unused_flags(MSG_HEADER *header);
extern void write_ids(MSG_HEADER *header,FILE *msg,char *msgaddr);

extern int	xform_maindialog(register int mode);
extern void	memory_error(void);
extern void fatal_error(char *ftxt);
extern void file_not_found(void);
extern void	vhide_mouse(void);
extern void	vshow_mouse(int reset);

extern void	filecopy(int xfin,int xfout,int mode,char *copy_buffer);
extern void setze_presets(void);
