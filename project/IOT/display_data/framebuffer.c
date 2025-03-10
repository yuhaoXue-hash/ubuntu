#include <stdio.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <math.h>
#include "framebuffer.h"


struct fb_var_screeninfo vinf;
void *pmem;
int fd;
int init_fb(const char *dev)
{
	fd = open(dev, O_RDWR);	
	if (-1 == fd)
	{
		perror("fail open fb");
		return -1;
	}
	

	int ret = ioctl(fd, FBIOGET_VSCREENINFO, &vinf);
	if (0 != ret)
	{
		perror("fail ioctl");
		return -1;
	}
	
	printf("xres = %d, yres = %d\n", vinf.xres, vinf.yres);
	printf("xres_virtual = %d, yres_virtual = %d\n", vinf.xres_virtual, vinf.yres_virtual);
	printf("bits_per_pixel = %d\n", vinf.bits_per_pixel);
	
	size_t len = vinf.xres_virtual * vinf.yres_virtual*vinf.bits_per_pixel/8;
	pmem = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if ((void *)-1 == pmem)
	{
		perror("fail mmap");
		return -1;
	}

	return 0;
}

void draw_point(int x, int y, unsigned int col)
{
	if (x >= vinf.xres || y >= vinf.yres)
		return;

	if (vinf.bits_per_pixel == RGB888_FMT)
	{
		unsigned int *p = pmem;
		*(p + vinf.xres_virtual * y + x) = col;
	}
	else if (vinf.bits_per_pixel == RGB565_FMT)
	{
		unsigned short *p = pmem;
		*(p + vinf.xres_virtual * y + x) = col;
	}

}


void deinit_fb()
{
	munmap(pmem, vinf.xres_virtual*vinf.yres_virtual*vinf.bits_per_pixel/8);
	close(fd);

}


void draw_clear(unsigned int col)
{
	for (int j = 0; j < vinf.yres; j++)
	{
		for (int i = 0; i < vinf.xres; i++)
		{
			draw_point(i, j, col);
		}
	}
}

void draw_h_line(int x, int y, int len, unsigned int col)
{
	for (int i = x; i < x+len; i++)
	{
		draw_point(i, y, col);
	}
}

void draw_s_line(int x, int y, int len, unsigned int col)
{
	for (int i = y; i < y+len; i++)
	{
		draw_point(x, i, col);
	}
}

void draw_rectangle(int x, int y, int w, int h, unsigned int col)
{
	draw_h_line(x, y, w, col);
	draw_s_line(x, y, h, col);
	draw_h_line(x, y+h, w, col);
	draw_s_line(x+w, y, h, col);
}

void draw_x_line(int x1, int y1, int x2, int y2, unsigned int col)
{
	if (x1 == x2)
	{
		if (y2 > y1)
		{
			draw_s_line(x1, y1, y2-y1, col);
		}
		else if (y2 < y1)
		{
			draw_s_line(x2, y2, y1-y2, col);
		}
		return ;
	}


	int y = 0;
	double k = (double)(y2 - y1) / (double)(x2 - x1);
	double b = y1 - k * x1;

	if (x2 > x1)
	{
		for (int x = x1; x <= x2; x++)
		{
			y = k * x + b;
			draw_point(x, y, col);
		}
	}
	else
	{
		for (int x = x2; x <= x1; x++)
		{
			y = k * x + b;
			draw_point(x, y, col);
		}
	}
	
	return ;
}


void draw_circle(int x0, int y0, int r, unsigned int col)
{
	int x = 0;
	int y = 0;
	for (double si = 0; si <= 360; si += 0.01)
	{
		x = r * cos(2 * 3.1415926 / 360 * si) + x0;
		y = r * sin(2 * 3.1415926 / 360 * si) + y0;
		draw_point(x, y, col);
		draw_point(x-1, y, col);
		draw_point(x, y-1, col);
		draw_point(x+1, y, col);
		draw_point(x, y+1, col);
	}
}


void draw_bmp(int x, int y, const char *bmp, int w, int h)
{
	int fdpic = open(bmp, O_RDONLY);
	if (-1 == fdpic)
	{
		perror("fail open bmp");
		return ;
	}
	
	lseek(fdpic, 54, SEEK_SET);
	
	unsigned char *buff = malloc(w*h*24/8);
	if (NULL == buff)
	{
		perror("fail malloc");
		return ;
	}
	read(fdpic, buff, w*h*24/8);

	unsigned char*p = buff;
	unsigned char r,g,b;

	for (int j = h-1; j >= 0; j--)
	{
		for (int i = 0; i < w; i++)
		{
			b = *p;p++;
			g = *p;p++;
			r = *p;p++;
			if (vinf.bits_per_pixel == RGB888_FMT)
			{
				unsigned int col = (r << 16) | (g << 8) | (b << 0);
				draw_point(i+x, j+y, col);
			}
			else if (vinf.bits_per_pixel == RGB565_FMT)
			{
				unsigned short col = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
				draw_point(i+x, j+y, col);
			}
		}
	}

	free(buff);
	close(fdpic);
	return ;
}


void draw_word(int x, int y, char *word, int w, int h, unsigned int col)
{
	unsigned char tmp = 0;
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			tmp = word[w*j+i];
			for (int k = 0; k < 8; k++)
			{
				//printf("(%d, %d)\n", 8*i+k, j);
				if (tmp & 0x80)
				{
					draw_point(8*i+k+x, j+y, col);
				}
				else
				{
					//
				}
				tmp = tmp << 1;
			}

		}
	}
}

int draw_utf8(UTF8_INFO *info, int x, int y, char* zi, u32 col, u32 col1)
{
    unsigned long  out = 0 ;
    int ret = enc_utf8_to_unicode_one((unsigned char*)zi,&out);

    unsigned char* data = get_utf_data(info,out);
    unsigned char temp = 0 ;
    unsigned int i,j,k;
    unsigned int num = 0;
    for(i=0;i<info->height;i++)
    {
        for(j=0;j<info->width/8;j++)
        {
            temp = data[num++];
            for(k=0;k<8;k++)
            {
                if(0x80&temp)
                {
                    draw_point(x+k+j*8, y+i, col);
                }
                else
                {

                   // draw_point( x+k+j*8, y+i, col1);
                }
                temp= temp<<1;
            }
        }

    }
    return ret;
}


int draw_utf8_str(UTF8_INFO *info, int arg_x, int arg_y,  char* zi, u32 col, u32 col1)
{
    char* temp = zi;
    unsigned int x = arg_x ;
    unsigned int y =  arg_y;

    while(*temp != '\0')
    {
        int ret = draw_utf8(info, x, y, temp, col, col1);
        x += info->width;
        if(x > vinf.xres)
        {
            x = 0;
            y += info->height;
            if(y > vinf.yres)
            {
                y = 0;
            }
        }

        temp += ret;
    }
    return 0;
}


