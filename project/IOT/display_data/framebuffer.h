#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__


#include "utf.h"

#define RGB888_FMT 32
#define RGB565_FMT 16


extern int init_fb(const char *dev);
extern void draw_point(int x, int y, unsigned int col);
extern void deinit_fb();
extern void draw_clear(unsigned int col);
extern void draw_h_line(int x, int y, int len, unsigned int col);
extern void draw_s_line(int x, int y, int len, unsigned int col);
extern void draw_rectangle(int x, int y, int w, int h, unsigned int col);
extern void draw_x_line(int x1, int y1, int x2, int y2, unsigned int col);
extern void draw_circle(int x0, int y0, int r, unsigned int col);
extern void draw_bmp(int x, int y, const char *bmp, int w, int h);
extern void draw_word(int x, int y, char *word, int w, int h, unsigned int col);
extern int draw_utf8(UTF8_INFO *info, int x, int y, char* zi, u32 col, u32 col1);
extern int draw_utf8_str(UTF8_INFO *info, int arg_x, int arg_y,  char* zi, u32 col, u32 col1);

#endif
