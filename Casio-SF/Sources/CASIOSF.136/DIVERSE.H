#include <aes.h>
#include <tos.h>
#include <fido.h>

extern void	init_vars(void);
extern int	xFsfirst(const char *filename,int attr);

extern void cutext(char *env);
extern int	fileselect(char *cin_path,char *csel_file,char *cext,char *titel);

extern void	getpfad(char *fname, char *pfadret);
extern void	trim(char **dest);
extern char *trimstr(char *dest);
extern void	trimright(char *dest);
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

extern void	vhide_mouse(void);
extern void	vshow_mouse(int reset);

extern unsigned long julian(int tag,int monat,int jahr);
extern char *datum(unsigned long n);
