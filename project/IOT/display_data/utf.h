#ifndef UTF
#define UTF
#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include<sys/ioctl.h>
#include<sys/mman.h>
#include<linux/fb.h>
#define u8 unsigned char 
#define u16 unsigned short 
#define u32 unsigned int

 #define RGB888_FMT 32
#define RGB565_FMT 16

#define ZIKUK_FILE_SMALL "./ziku"
#define ZIKUK_FILE_BIG "./ziku2_w32_h32"
/***字模文件缓存**/
typedef struct
{
    char path[256];//字模库文件路径
    unsigned width; //字模宽度
    unsigned height;//字模高度
    unsigned zimo_size;//每个字字模字节数
    unsigned char* g_ziku_data;//字模库文件缓存区
}UTF8_INFO;

/*     用之前先初始化这两步       */
extern int init_fb(const char *dev);
extern int init_UTF8_INFO(UTF8_INFO *utf8_info, int width, int height);
/*     用完后记得释放初始化      */

extern void draw_clear(unsigned int col);

extern void uninit_utf8(UTF8_INFO *info);

extern void init_utf8(UTF8_INFO *info);
extern unsigned char *get_utf_data(UTF8_INFO *info,int out);

extern int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic);
extern int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize);
extern int enc_get_utf8_size(const unsigned char pInput);

extern int draw_point(int x, int y, unsigned int color);
extern int display_show_utf8(UTF8_INFO *info, int x, int y, char* zi, u32 col, u32 col1);
extern int display_show_utf8_str(UTF8_INFO *info, int arg_x, int arg_y,  char* zi, u32 col, u32 col1);
extern void draw_bmp(int x, int y, const char *bmp, int w, int h);

#ifdef __cplusplus
}
#endif // UTF
#endif

