/*
=================================================================================
 Name        : SSP_Test_lcd.c
 Author      : Tianran Chen
 Version     :
 Copyright   : $(copyright)
 Description : this program allow us using the 1.8" Color TFT LCD via ssp ports,
 	 	 	   able to display screensaver of rectangle rotation and tree on the
 	 	 	   screen
=================================================================================
*/
#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "longhorn_sunset.h"

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "ssp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "font.h"
#include "extint.h"


/* Be careful with the port number and location number, because
some of the location may not exist in that port. */
#define PORT_NUM            1
#define LOCATION_NUM        0
#define TREE_NUM           2

#define OFFSETX 64
#define OFFSETY 80
#define OFFSETZ 30
#define CUBESIZE 40
#define ARRAY_SIZE 15
#define NUM_LINES 19


#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

uint8_t src_addr[SSP_BUFSIZE];
uint8_t dest_addr[SSP_BUFSIZE];
int colstart = 0;
int rowstart = 0;

/*****************************************************************************
** Function name:       LCD_TEST
**
** Descriptions:        Draw line function
**
** parameters:            None
** Returned value:        None
**
*****************************************************************************/

//LCD
// for 2.2 inch lcd width : 239 height : 319
#define ST7735_TFTWIDTH  239
#define ST7735_TFTHEIGHT 319
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define swap(x, y) { x = x + y; y = x - y; x = x - y; }
#define PI 3.14

//Colors

#define BLACK  0x000000
#define WHITE  0xFFFFFF
#define GREEN  0x00FF00
#define RED    0xFF0000
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define CYAN   0x00FFFF
#define MAGENTA 0xFF00FF
#define PURPLE 0x8000FF
#define ORANGE 0xFF8000
#define OLIVE 0x808000
#define LTBLUE 0x9090FF
#define CHOCOLATE 0xD2691E
int squarecolor = 0;
uint32_t colorarray[100] = {BLACK, GREEN, RED, BLUE, YELLOW, CYAN, MAGENTA, PURPLE, ORANGE, OLIVE, LTBLUE, CHOCOLATE};

//Axes
int _height = ST7735_TFTHEIGHT;
int _width = ST7735_TFTWIDTH;
int cursor_x = 0, cursor_y = 0;


void spiwrite(uint8_t c)
{
    //int portnum = 0; for spi pins 16,17,18
	int portnum = 1;//for spi pins 5,6,7
    src_addr[0] = c;
    SSP_SSELToggle( portnum, 0 );
    SSPSend( portnum, (uint8_t *)src_addr, 1 );
    SSP_SSELToggle( portnum, 1 );
}

void writecommand(uint8_t c) {
    LPC_GPIO0->FIOCLR |= (0x1<<21);
    spiwrite(c);
}

void writedata(uint8_t c) {

    LPC_GPIO0->FIOSET |= (0x1<<21);
    spiwrite(c);
}

void writeword(uint16_t c) {

    uint8_t d;
    d = c >> 8;
    writedata(d);
    d = c & 0xFF;
    writedata(d);
}

void write888(uint32_t color, uint32_t repeat) {
    uint8_t red, green, blue;
    int i;
    red = (color >> 16);
    green = (color >> 8) & 0xFF;
    blue = color & 0xFF;
    for (i = 0; i< repeat; i++) {
        writedata(red);
        writedata(green);
        writedata(blue);
    }
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
                    uint16_t y1) {

      writecommand(ST7735_CASET);
      writeword(x0);
      writeword(x1);
      writecommand(ST7735_RASET);
      writeword(y0);
      writeword(y1);

}

void drawPixel(int16_t x, int16_t y, uint32_t color) {
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

    setAddrWindow(x,y,x+1,y+1);
    writecommand(ST7735_RAMWR);
    write888(color, 1);
}

void lcddelay(int ms)
{
	int count = 24000;
	int i;

	for ( i = count*ms; i--; i > 0);
}


//Initialize LCD
void lcd_init()
{
/*
 * portnum     = 0 ;
 * cs         = p0.16 / p0.6 ?
 * rs        = p0.21
 * rst        = p0.22
 */
    //uint32_t portnum = 0;for spi0
	   uint32_t portnum = 1;//for spi1
    int i;
    printf("LCD initialized\n");
    /* Notice the hack, for portnum 0 p0.16 is used */
    if ( portnum == 0 )
      {
        LPC_GPIO0->FIODIR |= (0x1<<16);        /* SSP1, P0.16 defined as Outputs */
      }
      else
      {
        LPC_GPIO0->FIODIR |= (0x1<<6);        /* SSP0 P0.6 defined as Outputs */
      }
    /* Set rs(dc) and rst as outputs */
    LPC_GPIO0->FIODIR |= (0x1<<21);        /* rs/dc P0.21 defined as Outputs */
    LPC_GPIO0->FIODIR |= (0x1<<22);        /* rst P0.22 defined as Outputs */


    /* Reset sequence */
    LPC_GPIO0->FIOSET |= (0x1<<22);

 lcddelay(500);                        /*delay 500 ms */
    LPC_GPIO0->FIOCLR |= (0x1<<22);
 lcddelay(500);                        /* delay 500 ms */
    LPC_GPIO0->FIOSET |= (0x1<<22);
 lcddelay(500);                        /* delay 500 ms */

 for ( i = 0; i < SSP_BUFSIZE; i++ )    /* Init RD and WR buffer */
        {
            src_addr[i] = 0;
            dest_addr[i] = 0;
        }

     /* do we need Sw reset (cmd 0x01) ? */

     /* Sleep out */
     SSP_SSELToggle( portnum, 0 );
     src_addr[0] = 0x11;    /* Sleep out */
     SSPSend( portnum, (uint8_t *)src_addr, 1 );
     SSP_SSELToggle( portnum, 1 );

lcddelay(200);
    /* delay 200 ms */
    /* Disp on */
     SSP_SSELToggle( portnum, 0 );
     src_addr[0] = 0x29;    /* Disp On */
     SSPSend( portnum, (uint8_t *)src_addr, 1 );
     SSP_SSELToggle( portnum, 1 );
    /* delay 200 ms */
lcddelay(200);
}

void fillrect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
{
	int16_t i;
	int16_t width, height;

	width = x1-x0+1;
	height = y1-y0+1;
	setAddrWindow(x0,y0,x1,y1);
	writecommand(ST7735_RAMWR);
	write888(color,width*height);
}

//Draw line function
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint32_t color)
{
	int16_t slope = abs(y1 - y0) > abs(x1 - x0);
	if (slope)
	{
	swap(x0, y0);
	swap(x1, y1);
	}

	if (x0 > x1)
	{
	swap(x0, x1);
	swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1)
	{
	ystep = 1;
	} else
	{
	ystep = -1;
	}

	for (; x0<=x1; x0++)
	{
	if (slope)
	{
	drawPixel(y0, x0, color);
	} else
	{
	drawPixel(x0, y0, color);
	}
	err -= dy;
	if (err < 0)
	{
	y0 += ystep;
	err += dx;
	}
	}
}


void VLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	drawLine(x, y, x, y+h-1, color);
 }

 void HLine(int16_t x, int16_t y,  int16_t w, uint16_t color)
 {
	 drawLine(x, y, x+w-1, y, color);
 }

 void square(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
 		int16_t y2, int16_t x3, int16_t y3, int16_t squarecount) {
	if(squarecount==0)
		return;
 	if(squarecolor == 12){
 		 squarecolor = 0;
 	}
 	squarecolor += 1;
 	drawLine(x0, y0, x1, y1, colorarray[squarecolor]);
 	drawLine(x1, y1, x2, y2, colorarray[squarecolor]);
 	drawLine(x2, y2, x3, y3, colorarray[squarecolor]);
 	drawLine(x3, y3, x0, y0, colorarray[squarecolor]);

 	int x4 = x0 + 0.8 * (x1 - x0);
 	int y4 = y0 + 0.8 * (y1 - y0);

 	int x5 = x1 + 0.8 * (x2 - x1);
 	int y5 = y1 + 0.8 * (y2 - y1);

 	int x6 = x2 + 0.8 * (x3 - x2);
 	int y6 = y2 + 0.8 * (y3 - y2);

 	int x7 = x3 + 0.8 * (x0 - x3);
 	int y7 = y3 + 0.8 * (y0 - y3);

 	drawLine(x4, y4, x5, y5, colorarray[squarecolor]);
 	drawLine(x5, y5, x6, y6, colorarray[squarecolor]);
 	drawLine(x6, y6, x7, y7, colorarray[squarecolor]);
 	drawLine(x7, y7, x4, y4, colorarray[squarecolor]);

 	lcddelay(300);

 	square(x4, y4, x5, y5, x6, y6, x7, y7, squarecount - 1);
 }
 void triangle(int x0, int y0, int x1, int y1, int x2, int y2, int squarecount) {
	 if(squarecount==0)
		 	 return;
	 if(squarecolor == 12){
		 squarecolor = 0;
	 }
	squarecolor += 1;
 	drawLine(x0, y0, x1, y1, colorarray[squarecolor]);
 	drawLine(x1, y1, x2, y2, colorarray[squarecolor]);
 	drawLine(x2, y2, x0, y0, colorarray[squarecolor]);
 	//drawLine(x3, y3, x0, y0, WHITE);

 	int x3 = x0 + 0.8 * (x1 - x0);
 	int y3 = y0 + 0.8 * (y1 - y0);

 	int x4 = x1 + 0.8 * (x2 - x1);
 	int y4 = y1 + 0.8 * (y2 - y1);

 	int x5 = x2 + 0.8 * (x0 - x2);
 	int y5 = y2 + 0.8 * (y0 - y2);


 	drawLine(x3, y3, x4, y4, colorarray[squarecolor]);
 	drawLine(x4, y4, x5, y5, colorarray[squarecolor]);
 	drawLine(x5, y5, x3, y3, colorarray[squarecolor]);

 	lcddelay(300);

 	triangle(x3, y3, x4, y4, x5, y5, squarecount - 1);

 }
 void randomsquare() {
	 for(int i=0;i<=5;i++){
	 int x0 = rand()%51;
	 printf("%d square's [x0,y0] position's value--> [%d,%d]\n",i+1,x0,x0);
	 int y0=x0;
	 //calling function to draw and display square on LCD
	 square(x0, y0, x0+160, y0, x0+160, y0+160, x0, y0+160, 10);
 	 lcddelay(200);
 	 fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, WHITE);
 	 lcddelay(200);
	 }
 }
 void randomtriangle() {
 	 for(int i=0;i<=5;i++){
 	 int x0 = rand()%51;
 	 printf("%d triangle's [x0,y0] position's value--> [%d,%d]\n",i+1,x0,x0);
 	 int y0=x0;
 	 int x1=x0+160;
 	 int y1=y0;
 	//find offset from point 1 to 2
 	 int dX = x1 - x0;
 	 int dY = y1 - y0;
 	 int pi= 3.142;
 	//rotate and add to point 1 to find point 3
 	int x2 = (cos(pi/3) * dX + sin(pi/3) * dY) + x0;
 	int y2 = (sin(pi/3) * dX - cos(pi/3) * dY) + y0;

 	 //calling function to draw and display square on LCD
 	 triangle(x0, y0, x1, y1, x2, y2, 10);
  	 lcddelay(200);
  	 fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, WHITE);
  	 lcddelay(200);
 	 }
  }
/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  //EINTInit();
  uint32_t i, portnum = PORT_NUM;
  //portnum = 0 ; /* For LCD use 1 */
  /* SystemClockUpdate() updates the SystemFrequency variable */
//  SystemClockUpdate();
	portnum = 1;
   if ( portnum == 0 )
    SSP0Init();            /* initialize SSP port */
  else if ( portnum == 1 )
    SSP1Init();
  for ( i = 0; i < SSP_BUFSIZE; i++ )
  {
    src_addr[i] = (uint8_t)i;
    dest_addr[i] = 0;
  }
  //initialize LCD
  	  lcd_init();
  	  fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, WHITE);
  	  lcddelay(300);
  	  squarecolor = 0;
  	  randomsquare();
  	  randomtriangle();
  	  //calling function to draw and display triangle on LCD
      //fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, WHITE);
  return 0;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
