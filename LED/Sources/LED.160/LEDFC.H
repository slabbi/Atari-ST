
#define  biene()         graf_mouse(HOURGLASS,(void *)0L)
#define  mouse_normal()  graf_mouse(ARROW,(void *)0L)

/* newuser.v */
word userbox(word mode,word waitdialog,word startobj,byte *rname,uword *rzone,uword *rnet,uword *rnode,uword *rpoint);
word xget_user_nobox(byte *name,word only_pvtuser,word waitdialog,uword *rzone,uword *rnet,uword *rnode,uword *rpoint,byte *domain);
word xsrch_user(word only_pvtuser,byte *name,byte *domain,uword *zone,uword *net,uword *node,uword *point);

/*   gem.c  */
word LangAlert(word entry);
word FormAlert(word defbut, word icon, byte *text, byte *buttext);
void hide_mouse(void);
void show_mouse(void);
word hole_file(byte *file, byte *nurname, byte *ext, byte *titel);
void draw_dialog(OBJECT *tree, DIALINFO *dial);
void del_dialog(DIALINFO *dial);
void set_dtext(OBJECT *tree, word index, char *s, word redraw, word len);
void set_dstring(OBJECT *tree, word index, char *s, word redraw, word len);
word is_text(OBJECT *tree, word index);
void get_dedit(OBJECT *tree, word index, char *s);
void set_state(OBJECT *tree, word index, word state,word redraw);
void del_state(OBJECT *tree, word index, word state, word redraw);
void set_mstate(OBJECT *tree, word state, word redraw, ...);
void del_mstate(OBJECT *tree, word state, word redraw, ...);
word req_state(OBJECT *tree, word index, word state);
word req_flag(OBJECT *tree, word index, word flag);
void set_flag(OBJECT *tree, word index, word flag, word redraw);
void del_flag(OBJECT *tree, word index, word flag, word redraw);
void set_mflag(OBJECT *tree, word state, word redraw, ...);
void del_mflag(OBJECT *tree, word state, word redraw, ...);
void redraw_obj(OBJECT *tree, word index);
word open_vwork(void);
void ende(void);
void set_desktop(void);
void myobjc_draw(OBJECT *tree,word index,word depth);
void show_doing(word s);
void show_doingtext(byte *s);
void del_doing(void);
word Form_do(DIALINFO *dial, int fo_dostartob );
word req_button(OBJECT *tree, word index);
void del_button(OBJECT *tree, word index, word redraw);
void set_button(OBJECT *tree, word index, word redraw);
void set_dbutton(OBJECT *tree, word index, char *s, word redraw);
void get_dbutton(OBJECT *tree, word index, char *s);

/* deklara.c */
void set_dialpos(void);
void save_dialpos(void);
word deklaration(void);

/* hndl.c */
void set_menu(word art);
void set_clipmenu(word set_fl);
word hndl_menu(word titel, word item);
void throw_evntaway(void);
word edit_window(long win);
word hndl_keybd(long win,word key);
word hndl_mouse(word mx, word my, word anzged, word button);
word hndl_wind(word msgbuff[8]);
void multi(void);
word num_key(word key);

/* hndlsub.c */
word quit(void);
void execute(byte *file, byte *cmd, word sh_fl);
void program(byte *prg, byte *cmd);
word get_optwind(word close);
void dia_about(void);
void msg_list(long win);
word area_change(long win, word redraw, word changeflag);
void msg_goto(long win, word preset);
void msg_kill(long win, word heal, word multiple);
void move_msg(long win);
void msg_forward(long win);
word forw_dial(word area, byte *to_neu, byte *snode);
void set_fwdstr(long win, byte *f_from, byte *f_fr, byte *f_to, byte *f_by);
word lade_wind(long win);
void print_msg(long win, word doformfeed);
word read_ascii(long win, word mode, byte *fname);
void write_ascii(long win, word mode, word block_only);
void write_listascii(long win, word mode);
void save_lread(word area);
void load_lread(void);
void save_echolist(void);
word get_clipname(byte *file);
void undelete(long win, word msgnr);
void setze_raster(long i);
word get_origin_address(long win,uword *Zone,uword *Net,uword *Node,uword *Point,word strip);
void write_report(void);
void split(long win);

/* hndlopt.c */
void msginfo(long win);
void helping(OBJECT *tree, DIALINFO *dial);
void dia_optled(void);
void dia_optfont(void);
void set_dopt(word option, word index);
void set_treeopt(OBJECT *tree, word option, word index);
void save_optled(void);
void get_optled(void);
void dia_optswitches(void);
void dia_freq(long win);
void dia_header(long win);
void dia_header2(long win);
void dia_header3(long win);
void dia_fkeys(void);
void get_fkeys(void);
void next_subj(long win, byte *origname);
void prev_subj(long win, byte *origname);
void look_logbook(void);
void next_up(long win);
void prev_parent(long win);
void dia_optwindow(void);
void dia_optquote(void);
void dia_optuser(void);
void save_options(void);
byte *kap_re(byte *s);
void del_rek_subject(long win);


/* commend.c */
void comment_start(long win);
void comment(long win, word mode);
void dia_optcomment(void);


/* window.c */
void upd_status(long win, byte *s);
void new_status(long win, word stat);
word chk_status(long win, word redraw);
word align(word x, word n);
word alignx(word x);
void align_edit(long win, word *x, word *y, word *w, word *h);
word new_window(long win, word flags, char *name);
word rc_intersect(word p1[], word p2[]);
void set_wiclip(long win);
void get_wiwork(long win);
word top_window(void);
void get_flags(long win, byte *s1, byte *s2, byte *s3);
void set_wislider(long win);
void sys_wisetfiles(long windex, word leermachen);
void wisetfiles(long windex, word leermachen, word stat);
void get_maxwindcol(long win, word maxcol, word *w, word *h);
void redraw_all(void);
word open_defltwind(void);
void open_window(long windex, byte wname[], word msgwindfl);
word close_window(long windex);
void wiopen(void);
void wicycle(void);
void wifull(long win);
word srch_winarea(long win, word area);
long found_wind(void);
long area_msgwind(word area_index);
long get_freewind(void);
word list_window(long win);
long get_windarea(long win, word area);
long get_firstmsgwind(void);


/* config.c */
byte *skip_blanks(char *string);
void cut_endspc(byte *string);
void add_bslash(byte *path);
void kill_lblank(byte *s, word kill_bl);
void ctl_string(byte *buf, byte *string,word kill_bl);
void speicher_freigeben(void);
word config_init(byte *filename);

/* msg.c */
word is_netmail(word area);
word is_ptowner(byte *sysop,word area);
void berechne_msgp(long win);
void hole_msgp(long win);
void first_msg(byte *msgname);
word select_msg(long win, word msgnr, word redraw, word read_fl);
word srch_up(long win, word msgnr);
word srch_down(long win, word msgnr);
word open_area(long win, word areanr);
void close_area(long win);
void change_area(long win, word newarea);
void open_allarea(word area);
void close_allarea(word area);
static void search_midrep(long win, word aktmsgnr);
void set_comstatus(long win, word aktmsgnr);
void killnetmsg(long create);
void killnrmsg(long ed_win, word msgnr);
word get_lastmsg(word arindex);


/* liste.c */
word ascii2hex(word x);
void buf_to_liste(long win);

/* doform.c */
word male_seite(word seite1, word redraw);
word draw_side(word seite, word redraw);
void show_side(word side, word *oldside, word *area, word *max_sel);
void cut_arname(byte *name);
void select_side(word *sel_area, word *seite, word *max_sel);
word do_aform(word *sel_area, word drawdia);
void inhalt_listen(long win, word top_nr, word *sel_obj, word redraw);
word do_lform(long win, word sel_obj, word top_msg);
word find_keyarea(word key, word sel_area);


/* hdrcache.c */
void init_hdrcache(long win, byte *file);
size_t hdrseek(long win, FILE *hdrfp, word msg, int mode);
size_t hdrread(long win, void *hptr, size_t count, FILE *hdrfp);
size_t hdrwrite(long win, void *hptr, size_t count, FILE *hdrfp);
long hdrtell(long win, FILE *hdrfp);
size_t fasthdrseek(WIND *win, FILE *hdrfp, word msg, int mode);
size_t fasthdrread(WIND *win, void *hptr, size_t count, FILE *hdrfp);


/* edit.c */
void clear_hdr(long win);
void get_xypos(long win, word *x, word *y);
void cursor(long win, word mode);
/* void myv_gtext(word x,word y,word lnr,byte *line,byte *complete_line,word effect_fl); */
void effect_v_gtext(long win,word x,word y,word lnr,byte *line);
void gputc1(ubyte c, word x, word y);
void phdrline(long win);
void phdrfromline(long win);
void psubjline(long win);
void pline(long win, word fcy, word lcy, word vslddraw, word hslddraw);
void del_line(long win, word lnr, word redraw);
word ins_line(long win, word lnr, byte *line, word useind, word redraw);
word ins_arline(long win, word lnr, word lanz);
word getindent(long win, byte *line, byte *indstr);
void set_cxpos(long win);
void set_firstc(long win, word cx);
void init_cutbuff(void);
void clear_cutbuff(void);
void ch_blkbounds(long win);
void del_blkbounds(long win, word redraw);
void append_line(long win);
void copy_blk(long win, word redraw);
void cut_blk(long win);
void ins_blk(long win);
void c_rt(long win);
void c_lf(long win);
void c_up(long win);
void c_dn(long win);
void c_return(long win);
void backsp(long win);
void delete(long win, word wrap, word multi, word draw);
void insert_tab(long win, word draw);
void extend_tab(long win);
void output(long win, ubyte c, word draw);
void outmode(long win, ubyte c, word draw);
void upded_stat(long win, word state);
void overwrite(long win, ubyte c);
void insert(long win, ubyte c, word draw);
word wrap_fwd(long win, word draw);
void wrap_bwd(long win, word multi);
void fkey_pressed(long win, word fknr);
void nomemalert(word mode);
word scan_effects(byte *text,byte *ret);

/* edhndl.c */
word title_select(void);
word edmenu_event(word title, word item);
void block_menu(word item);

word edkey_event(long win, word key);
void setdia_flags(long win);
void getdia_flags(long win);
void ed_elline(byte *titel, byte *edline);
void edbutton_event(long win,word x,word y,word clicks,word state);
word edwindow_event(word msgbuff[8]);
void multi_edit(void);
word graphic_output(long win, word key);
void msg_button(long win,word x,word y,word clicks,word state);


/* quote.c */
word add_uucp_to_line(long win,word lneu);
void del_allhflags(HDR *hdr_p);
byte *ifna_date(void);
word store_line(long win, word lineindex, byte *quote, byte *inhalt, word add_blank, word add_lf);
void no_blanks(byte *s);
word quote_wrap(long win, byte *q_name, word area, word forwardquote_fl);
void addendspc(byte *line);
void addnewl(byte *line);
byte *getquote(byte *line, byte *quote);
void get_origaddr(long win, byte *addr);
word copy_origaddr(byte *addr, byte *line);
void set_orignode(HDR *hdr,word area);
void del_mailerfields(long win);
void add_subjre(long win);
void store_msghdr(long win);
void get_qname(long win, byte *q_name);
void convert_percent(byte *free, word area, word fkey);
word mon2int(byte *m);


/* editmsg.c */
void free_allmsglines(long win);
void prep_topline(long win, word titel);
void edit_msg(long win, word newmsg);
void edit_end(long win, word e_save);
void leave_edit(long win);
void reply_msg(long win,word arindex);
void quote_msg(long win);
void change_msg(long win);
void copy_innetmail(long win, word neworig, word closenetm,
                    byte *n_to,byte *f_from,byte *f_fr,byte *f_to,byte *f_by);
void del_blanklines(long win, word memlines);
void store_msgid(long win);
void put_pid(FILE *fp);
word put_msgid(long win, HDR *hdr_p, FILE *msg_fp);
void put_replyid(long win, FILE *msg_fp);
void save_lines(long win, FILE *msgfp, word filter_chrs, word destarea);
void init_carbon(void);
void dia_carbon(void);
void save_carbon(long win);
void quotearea_msg(long win, word arindex);
word save_newmsg(long win, byte *cc_orig, word openarea);


/* rcopy.c */
word *setxy(word pxy[], word x1, word y1, word x2, word y2);
void rcopy(word xs, word ys, word ws, word hs, word xd, word yd, word mode);
void line_redraw(long win, word x, word y, word w, word h);
void scr_up(long win);
void scr_down(long win);
void lines_up(long win, word lnr);
void lines_down(long win, word lnr);


/* user.c */
void save_user(long win);
void init_userdia(byte *s, uword zone, uword net, uword node, uword point);
void nnstr2node(byte *s, uword *zone, uword *net, uword *node, uword *point);

/* font.c */
void set_msgfnt(void);
void set_editfnt(void);
void set_mlistfnt(void);
void set_alistfnt(void);
void set_pointsize(long win, word point);
void set_wifont(long win);
void load_fonts(void);
void self_anzeigen(word topindex, word selindex, word redraw);
word male_points(word *point_obj, word ft_index, word anfpt, word redraw);
word dia_font(word *msg_index,word *msg_pt,word *ed_index,word *ed_pt,
							word *mlist_index,word *mlist_pt,word *alist_index,word *alist_pt);
word get_selpoint(word *point_obj);
word get_fontindex(word fid);

/* rsc.c */
word Rsrc_load(byte *dummy);
void Rsrc_free(void);

/* key.c */
word get_code(int key, int state);

/* crc.c */
unsigned long crc32str(char *s);

/* windlist.c */
void my_gtext(word x, word y, byte *line, word effect_fl);
word printline(long win, word lnr, word sel_fl);
void list_edbutton(long win, word x, word y, word clicks);
word list_keybd(long win,word key);
void top_it(long win);
void no_areawind_successiv(void);
void areawind_successiv(void);
byte *get_successiv_areaname(void);

/* windarea.c */
word make_arealist(long newwin);
long openarea_listwindow(long win, word arindex, word changeflag);
void update_arealistwindow(long win, word aktarea_or_all);
void update_areawindow(word newarea);

/* windmsgs.c */
void make_msglist(long win,long newwin);
long msg_listwindow(long win);
void update_msglistwindow(long win, word aktmsg);
void open_msglistwindow(long win);
void need_update_msglistwindow(long win,word mode);
void sort_header(long win);

/* fido.c */
word is_running(byte *prg);
void start_avalon(void);
void start_export(word receiver);
void scanaddr(char *addr,uword *zone,uword *net,uword *node,uword *point,char *domain);
char *mergeaddr(uword zone,uword net,uword node,uword point,char *domain);
byte *addr2file36(uword net,uword node,uword point);

/* selorg.c */
int  passender_absender(byte *touser,
												uword toZone,uword toNet,uword toNode,uword toPoint,byte *toDomain,
												uword *fZone,uword *fNet,uword *fNode,uword *fPoint,byte *fDomain);
void set_if_fixed_address(long win,word area);
word select_sysop(char *sysop);
word select_domain(char *domain);
word show_shortcut(long win,byte *filter);
word select_function(word mode);

/* cookie.c */
word getcookie(ulong cookie,ulong *p_value);

/* register.c */
word test_registered(void);
word available(byte *txt);
word read_keytab(byte *fname);
void register_speicher_freigeben(void);

/* thanks.c */
void draw_led(word x,word y,word fast);
void thanks(void);
word cripple(long win);

/* search.c */
void dia_find(long win);
void find_same(long win,word nextmsg_fl);

/* HYPERTXT.C */
void aufrufbar_machen(byte *prgname);
void hypertext(word mode, byte *txt);

/* UUENCODE.C */
word read_binary(long win);
word write_binary(long win,word fastflag);

/* LANGUAGE.C */
void language_speicher_freigeben(void);
word read_language(byte *file);

/* ICONIFY.C */
word handle_kbd_iconify(long win,word convkey,word key,word shf);
void redraw_iconify(long win);
