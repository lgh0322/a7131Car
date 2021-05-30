//oled.h
#ifndef __OLED_H
#define __OLED_H			  	 
#include "stdlib.h"	   
#include "i2c.h"
#include "string.h"
#include "stdio.h"
#include "font.h"
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    		
#define oledAddr 0x78
//-----------------OLED IIC端口定义----------------  					   
extern I2C_HandleTypeDef hi2c1;


#define _screen_absDiff(x, y) (((x) > (y)) ?  ((x)-(y)) : ((y)-(x)))
#define _screen_swap(a, b) {int t; t = (a); a = (b);  b = t;}

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

static unsigned char gra[1024];
typedef unsigned char u8;
typedef unsigned int u32;
//OLED控制用函数

void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Init(void);

static void display_gra(void){
	
		u8 i;		    
	int h=0;
	for(i=0;i<8;i++)  
	{  
		Write_IIC_Command(0xb0+i);   
		Write_IIC_Command(0x00);     
		Write_IIC_Command(0x10);     
		HAL_I2C_Mem_Write(&hi2c1,oledAddr,0x40,I2C_MEMADD_SIZE_8BIT,gra+h,128,2000);
		h+=128;
		
	} 
	
}

static void oled_clear(void)
{
	memset(gra,0,1024);
}

static void oled_clear2(void)
{
	memset(gra,255,1024);
}

static void display_char(int a, int b,char x){
	int h=b*128+a;
	memcpy(gra+h,font16+x*32,8);
	memcpy(gra+h+128,font16+x*32+16,8);
}

static void display_string(int a,int b,char* x){
	while(*x!=0){
		display_char(a,b,*x);
		x++;
		a+=8;
	}
}

static void draw_pix(int x1,int y1){
	int y=y1/8;
	int h=y*128+x1;
	int y2=y1%8;
	gra[h]=gra[h]|(1<<y2);
}

static void screen_draw_line(int x1, int y1, int x2, int y2) {
    uint8_t deltax, deltay, x, y, steep;
    int8_t error, ystep;

    steep = _screen_absDiff(y1, y2) > _screen_absDiff(x1, x2);

    if (steep) {
        _screen_swap(x1, y1);
        _screen_swap(x2, y2);
    }

    if (x1 > x2) {
        _screen_swap(x1, x2);
        _screen_swap(y1, y2);
    }

    deltax = x2 - x1;
    deltay = _screen_absDiff(y2, y1);
    error = deltax / 2;
    y = y1;

    if (y1 < y2) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (x = x1; x <= x2; x++) {
        if (steep) {
            draw_pix(y, x);
        } else {
            draw_pix(x, y);
        }
        error = error - deltay;

        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}
#endif  
