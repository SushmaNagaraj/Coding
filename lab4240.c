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
#include<stdlib.h>
#include <time.h>

struct worldPoints{
	float x;
	float y;
}p0;

struct cameraPoints{
	int xe,ye,ze;
}pCam;

struct world_3DPoints{
	int x,y,z;
}D3;


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
int _cp437=0;
int gfxFont=0;

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
#define ST7735_TFTWIDTH  240
#define ST7735_TFTHEIGHT 320
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
#define DKBLUE 0x404040
int squarecolor = 0;
uint32_t colorarray[100] = {BLACK, GREEN, RED, BLUE, YELLOW, CYAN, MAGENTA, PURPLE, ORANGE, OLIVE, LTBLUE, CHOCOLATE};

//Axes
int _height = ST7735_TFTHEIGHT;
int _width = ST7735_TFTWIDTH;
int cursor_x = 0, cursor_y = 0;
void line_rotate(float *x1,float *y1,float x0,float y0,int angle);
void drawCircle(int16_t xPos, int16_t yPos, int16_t radius, uint16_t color);
void drawTree(float x0,float y0,float x1,float y1,int squarecolor,int levels,int angle);
void point_of80(float x0,float y0,float *x1,float *y1,float x0_temp,float y0_temp);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);

int option =0;
int subOption =0;
char* suboptionname1 = "1. Draw Squares\n";
char* suboptionname2 = "2. Draw Triangles\n";
char* suboptionname3 = "3. Draw Cubes\n ";
char* suboptionname4 = "Back\n ";
char* suboptionname5 = "1. Draw Trees\n";
char* suboptionname6 = "2. Draw Cube\n ";
char* suboptionname7 = "3. Draw cubes\n ";
char* suboptionname8 = "Back\n ";
char* suboptionname9 = "1. Draw cube\n";
char* suboptionname10 ="2. Draw cubes\n";
char* suboptionname11 = "3. Draw cubes with shadow\n";
char* suboptionname12= "Back\n ";
uint32_t textcolor;
uint32_t textbgcolor;
float textsize;
int wrap=1;
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
	// Pin Sel first
	LPC_PINCON->PINSEL4 &= ~(3 << 12);
	LPC_PINCON->PINSEL4 &= ~(3 << 14);

	// Selecting pull down resistor
	LPC_PINCON->PINMODE4 |= 3 << 12;
	LPC_PINCON->PINMODE4 |= 3 << 14;
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
	 // Setting the direction of 2.6,2,7,2.8,2,10 as input
	GPIOinitIn(2,6);
	GPIOinitIn(2,7);
	GPIOinitIn(2,8);
	GPIOinitIn(2,10);
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
void drawLineforchar(int16_t y0, int16_t x0, int16_t y1, int16_t x1,uint32_t color)
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
	 for(int i=0;i<=2;i++){
	 int x0 = rand()%51;
	 printf("%d square's [x0,y0] position's value--> [%d,%d]\n",i+1,x0,x0);
	 int y0=x0;
	 //calling function to draw and display square on LCD
	 square(x0, y0, x0+160, y0, x0+160, y0+160, x0, y0+160, 10);
 	 lcddelay(200);
 	 //fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
 	 //lcddelay(200);
	 }
 }

 void randomtriangle() {
 	 for(int i=0;i<=2;i++){
 	 int x0 = rand()%51;
 	// printf("%d triangle's [x0,y0] position's value--> [%d,%d]\n",i+1,x0,x0);
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
  	 //fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
  	 //lcddelay(200);
 	 }
  }

 void line_rotate(float *x1,float *y1,float x0,float y0,int angle){
	 float dx = *x1-x0;
	 float dy = *y1-y0;
	 float pi = 3.142;
	 float rad = (pi/180)*angle;
	 float right_dx = (cos(rad) * dx)+(-sin(rad) * dy);
	 float right_dy = (sin(rad) * dx)+(cos(rad) * dy);
	 *x1 = right_dx + x0;
	 *y1 = right_dy + y0;
 }

 void drawTree(float x0,float y0,float x1,float y1,int squarecolor,int levels,int angle){
	 float x0_temp,y0_temp;
	 if (levels <= 0)
		return;
 	 drawLine(x0, y0, x1, y1, colorarray[squarecolor]);
 	 x0_temp=x0;
 	 y0_temp=y0;
 	 x0=x1;
 	 y0=y1;
 	 point_of80(x0,y0,&x1,&y1,x0_temp,y0_temp);
 	 line_rotate(&x1,&y1,x0,y0,angle);
 	 drawLine(x0, y0, x1, y1, colorarray[squarecolor]);
 	 drawTree(x0,y0,x1,y1,squarecolor,levels-1,angle);

 	 line_rotate(&x1,&y1,x0,y0,360-angle);
 	 drawLine(x0, y0, x1, y1, colorarray[squarecolor]);
 	 drawTree(x0,y0,x1,y1,squarecolor,levels-1,angle);

 	 line_rotate(&x1,&y1,x0,y0,360-angle);
 	 drawLine(x0, y0, x1, y1, colorarray[squarecolor]);
 	 drawTree(x0,y0,x1,y1,squarecolor,levels-1,angle);

  }

 void point_of80(float x0,float y0,float *x1,float *y1,float x0_temp,float y0_temp){
	 *x1 = *x1+0.8*(x0-x0_temp);
	 *y1 = *y1+0.8*(y0-y0_temp);
 }

 void drawCircle(int16_t xPos, int16_t yPos, int16_t radius, uint16_t color) {
 	/*draws circle at x,y with given radius & color*/
 	int x, xEnd, y;
 	xEnd = (0.7071 * radius) + 1;
 	for (x = 0; x <= xEnd; x++) {
 		y = (sqrt(radius * radius - x * x));
 		drawPixel(xPos + x, yPos + y, color);
 		drawPixel(xPos + x, yPos - y, color);
 		drawPixel(xPos - x, yPos + y, color);
 		drawPixel(xPos - x, yPos - y, color);
 		drawPixel(xPos + y, yPos + x, color);
 		drawPixel(xPos + y, yPos - x, color);
 		drawPixel(xPos - y, yPos + x, color);
 		drawPixel(xPos - y, yPos - x, color);
 	}
 }

 struct worldPoints world_3dCordinateSystem(float x, float y, float z)
 {
 	int xe=pCam.xe;
 	int ye=pCam.ye;
	int ze=pCam.ze;
 	int d=100;
 	int x0=120,y0=160;
 	struct worldPoints p;

 	double theta,phi,rho;
 	double xprime,yprime,zprime;

 	rho = sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2));
 	theta = acos(xe/(sqrt(pow(xe,2)+pow(ye,2))));
 	phi = acos(ze/(sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2))));

 	//Transformation matrix
 	double matrix3d[4][4]={
 							{-sin(theta),				cos(theta),					0,						0	},
 							{-cos(phi)*sin(theta),		-cos(phi)*sin(theta),		sin(phi),				0	},
 							{-sin(phi)*cos(theta),		-sin(phi)*cos(theta),		-cos(phi),				rho	},
 							{0,							0,							0,						1	}
 						};

 	xprime	=	((x*matrix3d[0][0])	+	(y*matrix3d[0][1]));
 	yprime	=	((x*matrix3d[1][0])	+	(y*matrix3d[1][1])	+	(z*matrix3d[1][2]));
 	zprime	=	((x*matrix3d[2][0])	+	(y*matrix3d[2][1])	+	(z*matrix3d[2][2])	+	(1*matrix3d[2][3]));

 	p.x=x0+((xprime/zprime)*d);
 	p.y=y0-((yprime/zprime)*d);

 	return p;
 }

 struct worldPoints world_3dCordinateSystemForSecondCube(float x, float y, float z)
 {
 	int xe=pCam.xe;
 	int ye=pCam.ye;
	int ze=pCam.ze;
 	int d=100;
 	int x0=60,y0=180;
 	struct worldPoints p;

 	double theta,phi,rho;
 	double xprime,yprime,zprime;

 	rho = sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2));
 	theta = acos(xe/(sqrt(pow(xe,2)+pow(ye,2))));
 	phi = acos(ze/(sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2))));

 	//Transformation matrix
 	double matrix3d[4][4]={
 							{-sin(theta),				cos(theta),					0,						0	},
 							{-cos(phi)*sin(theta),		-cos(phi)*sin(theta),		sin(phi),				0	},
 							{-sin(phi)*cos(theta),		-sin(phi)*cos(theta),		-cos(phi),				rho	},
 							{0,							0,							0,						1	}
 						};

 	xprime	=	((x*matrix3d[0][0])	+	(y*matrix3d[0][1]));
 	yprime	=	((x*matrix3d[1][0])	+	(y*matrix3d[1][1])	+	(z*matrix3d[1][2]));
 	zprime	=	((x*matrix3d[2][0])	+	(y*matrix3d[2][1])	+	(z*matrix3d[2][2])	+	(1*matrix3d[2][3]));

 	p.x=x0+((xprime/zprime)*d);
 	p.y=y0-((yprime/zprime)*d);

 	return p;
 }

 struct worldPoints world_3dCordinateSystemForThirdCube(float x, float y, float z)
   {
   	int xe=pCam.xe;
   	int ye=pCam.ye;
  	int ze=pCam.ze;
   	int d=100;
   	int x0=170,y0=180;
   	struct worldPoints p;

   	double theta,phi,rho;
   	double xprime,yprime,zprime;

   	rho = sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2));
   	theta = acos(xe/(sqrt(pow(xe,2)+pow(ye,2))));
   	phi = acos(ze/(sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2))));

   	//Transformation matrix
   	double matrix3d[4][4]={
   							{-sin(theta),				cos(theta),					0,						0	},
   							{-cos(phi)*sin(theta),		-cos(phi)*sin(theta),		sin(phi),				0	},
   							{-sin(phi)*cos(theta),		-sin(phi)*cos(theta),		-cos(phi),				rho	},
   							{0,							0,							0,						1	}
   						};

   	xprime	=	((x*matrix3d[0][0])	+	(y*matrix3d[0][1]));
   	yprime	=	((x*matrix3d[1][0])	+	(y*matrix3d[1][1])	+	(z*matrix3d[1][2]));
   	zprime	=	((x*matrix3d[2][0])	+	(y*matrix3d[2][1])	+	(z*matrix3d[2][2])	+	(1*matrix3d[2][3]));

   	p.x=x0+((xprime/zprime)*d);
   	p.y=y0-((yprime/zprime)*d);

   	return p;
   }
 struct worldPoints world_3dCordinateSystemForFourthCube(float x, float y, float z)
    {
    	int xe=pCam.xe;
    	int ye=pCam.ye;
    	int ze=pCam.ze;
    	int d=100;
    	int x0=100,y0=140;
    	struct worldPoints p;

    	double theta,phi,rho;
    	double xprime,yprime,zprime;

    	rho = sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2));
    	theta = acos(xe/(sqrt(pow(xe,2)+pow(ye,2))));
    	phi = acos(ze/(sqrt(pow(xe,2)+pow(ye,2)+pow(ze,2))));

    	//Transformation matrix
    	double matrix3d[4][4]={
    							{-sin(theta),				cos(theta),					0,						0	},
    							{-cos(phi)*sin(theta),		-cos(phi)*sin(theta),		sin(phi),				0	},
    							{-sin(phi)*cos(theta),		-sin(phi)*cos(theta),		-cos(phi),				rho	},
    							{0,							0,							0,						1	}
    						};

    	xprime	=	((x*matrix3d[0][0])	+	(y*matrix3d[0][1]));
    	yprime	=	((x*matrix3d[1][0])	+	(y*matrix3d[1][1])	+	(z*matrix3d[1][2]));
    	zprime	=	((x*matrix3d[2][0])	+	(y*matrix3d[2][1])	+	(z*matrix3d[2][2])	+	(1*matrix3d[2][3]));

    	p.x=x0+((xprime/zprime)*d);
    	p.y=y0-((yprime/zprime)*d);

    	return p;
    }
 void draw_3dCordinateSystem()
 {
 	struct worldPoints p1,p2,p3;

 	p1=world_3dCordinateSystem(180,0,0);
 	drawLine(p0.x,p0.y,p1.x,p1.y,RED);
 	p1=world_3dCordinateSystem(180,1,0);
 	drawLine(p0.x,p0.y,p1.x,p1.y,RED);
 	p1=world_3dCordinateSystem(180,0,1);
 	drawLine(p0.x,p0.y,p1.x,p1.y,RED);

 	p2=world_3dCordinateSystem(0,180,0);
 	drawLine(p0.x,p0.y,p2.x,p2.y,GREEN);
 	p2=world_3dCordinateSystem(1,180,0);
 	drawLine(p0.x,p0.y,p2.x,p2.y,GREEN);
 	p2=world_3dCordinateSystem(0,180,1);
 	drawLine(p0.x,p0.y,p2.x,p2.y,GREEN);


 	p3=world_3dCordinateSystem(0,0,180);
 	drawLine(p0.x,p0.y,p3.x,p3.y,BLUE);
 	p3=world_3dCordinateSystem(1,0,180);
 	drawLine(p0.x,p0.y,p3.x,p3.y,BLUE);
 	p3=world_3dCordinateSystem(0,1,180);
 	drawLine(p0.x,p0.y,p3.x,p3.y,BLUE);
 }
void drawOutlineCube(struct worldPoints t1,struct worldPoints t2,struct worldPoints t3,struct worldPoints t4,struct worldPoints t5,struct worldPoints t6,struct worldPoints t7,struct worldPoints t8)
{
	drawLine(t1.x, t1.y, t2.x, t2.y, GREEN);
	drawLine(t2.x, t2.y, t3.x, t3.y, GREEN);
	drawLine(t3.x, t3.y, t4.x, t4.y, GREEN);
	drawLine(t4.x, t4.y, t1.x, t1.y, GREEN);
	drawLine(t5.x, t5.y, t6.x, t6.y, GREEN);
	drawLine(t6.x, t6.y, t7.x, t7.y, GREEN);
	drawLine(t7.x, t7.y, t8.x, t8.y, GREEN);
	drawLine(t8.x, t8.y, t5.x, t5.y, GREEN);
	drawLine(t1.x, t1.y, t5.x, t5.y, GREEN);
	drawLine(t2.x, t2.y, t6.x, t6.y, GREEN);
	drawLine(t3.x, t3.y, t7.x, t7.y, GREEN);
	drawLine(t4.x, t4.y, t8.x, t8.y, GREEN);
}
 void drawOutlineOfCubeForSecondCube(int size)
  {
  	struct worldPoints p1,p2,p3,p4;
  	struct worldPoints p5,p6,p7,p8;

  	p1=world_3dCordinateSystemForSecondCube(55,55,size);
  	p2=world_3dCordinateSystemForSecondCube(size,55,size);
  	p3=world_3dCordinateSystemForSecondCube(size,size,size);
  	p4=world_3dCordinateSystemForSecondCube(55,size,size);

  	drawLine(p1.x,p1.y,p2.x,p2.y,YELLOW);
  	drawLine(p2.x,p2.y,p3.x,p3.y,YELLOW);
  	drawLine(p3.x,p3.y,p4.x,p4.y,YELLOW);
  	drawLine(p4.x,p4.y,p1.x,p1.y,YELLOW);


  	p5=world_3dCordinateSystemForSecondCube(55,55,55);
  	p6=world_3dCordinateSystemForSecondCube(size,55,55);
  	p7=world_3dCordinateSystemForSecondCube(size,size,55);
  	p8=world_3dCordinateSystemForSecondCube(55,size,55);

  	drawLine(p2.x,p2.y,p6.x,p6.y,YELLOW);
  	drawLine(p3.x,p3.y,p7.x,p7.y,YELLOW);
  	drawLine(p4.x,p4.y,p8.x,p8.y,YELLOW);
  	drawLine(p6.x,p6.y,p7.x,p7.y,YELLOW);
  	drawLine(p7.x,p7.y,p8.x,p8.y,YELLOW);
  }
  void drawOutlineOfCubeForThirdCube(int size)
  {
    	struct worldPoints p1,p2,p3,p4;
    	struct worldPoints p5,p6,p7,p8;

    	p1=world_3dCordinateSystemForThirdCube(45,45,size);
    	p2=world_3dCordinateSystemForThirdCube(size,45,size);
    	p3=world_3dCordinateSystemForThirdCube(size,size,size);
    	p4=world_3dCordinateSystemForThirdCube(45,size,size);

    	drawLine(p1.x,p1.y,p2.x,p2.y,YELLOW);
    	drawLine(p2.x,p2.y,p3.x,p3.y,YELLOW);
    	drawLine(p3.x,p3.y,p4.x,p4.y,YELLOW);
    	drawLine(p4.x,p4.y,p1.x,p1.y,YELLOW);


    	p5=world_3dCordinateSystemForThirdCube(45,45,45);
    	p6=world_3dCordinateSystemForThirdCube(size,45,45);
    	p7=world_3dCordinateSystemForThirdCube(size,size,45);
    	p8=world_3dCordinateSystemForThirdCube(45,size,45);

    	drawLine(p2.x,p2.y,p6.x,p6.y,YELLOW);
    	drawLine(p3.x,p3.y,p7.x,p7.y,YELLOW);
    	drawLine(p4.x,p4.y,p8.x,p8.y,YELLOW);
    	drawLine(p6.x,p6.y,p7.x,p7.y,YELLOW);
    	drawLine(p7.x,p7.y,p8.x,p8.y,YELLOW);
    }
 struct worldPoints rotate(float a, float b, float c,float sinax,float cosax,float sinay,float cosay,float sinaz,float cosaz)                     		// routine to draw and calc 3d cube
  {
	struct worldPoints p;
	float xt, yt, zt, x, y, z;

	x = a;                	// get x for vertex i
	y = b;                	// get y for vertex i
	z = c;                	// get z for vertex i
	yt = y * cosax - z * sinax;	// rotate around the x axis
	zt = y * sinax + z * cosax;	// using the Y and Z for the rotation
	y=yt;
	z=zt;

	xt = x * cosay - z * sinay;	// rotate around the Y axis
	zt = x * sinay + z * cosay;	// using X and Z
	x = xt;
	z = zt;

	xt = x * cosaz - y * sinaz;	// finally rotate around the Z axis
	yt = x * sinaz + y * cosaz;	// using X and Y
	x = xt;
	y = yt;
	z = zt;
	p=world_3dCordinateSystem(x,y,z);
	return p;
}
 struct worldPoints rotate4(float a, float b, float c,float sinax,float cosax,float sinay,float cosay,float sinaz,float cosaz)                     		// routine to draw and calc 3d cube
 {
 	struct worldPoints p;
 	float xt, yt, zt, x, y, z;

 			x = a;                	// get x for vertex i
 			y = b;                	// get y for vertex i
 			z = c;                	// get z for vertex i
 			yt = y * cosax - z * sinax;	// rotate around the x axis
 			zt = y * sinax + z * cosax;	// using the Y and Z for the rotation
 			y=yt;
 			z=zt;

 			xt = x * cosay - z * sinay;	// rotate around the Y axis
 			zt = x * sinay + z * cosay;	// using X and Z
 			x = xt;
 			z = zt;

 			xt = x * cosaz - y * sinaz;	// finally rotate around the Z axis
 			yt = x * sinaz + y * cosaz;	// using X and Y
 			x = xt;
 			y = yt;
 			z = zt;
 			p=world_3dCordinateSystemForFourthCube(x,y,z);
 			return p;
 }
 void fill3DCube1(int size)
 {
	struct worldPoints p;
	float roti = 0;                  	// starting amount of x rotation
	float rotj = 0;                 	// starting amount of y rotation
	float rotcube_side = 0;
	roti = roti + 3;          	// rotate the cube on X axis
	rotj = rotj + 3;          	// and on its y axis
	rotcube_side = rotcube_side + 0;
	float sinax = sin(roti);				// precalculate the sin and cos values
	float cosax = cos(roti);				// for the rotation as this saves a
	float sinay = sin(rotj);				// little time when running as we
	float cosay = cos(rotj);				// call sin and cos less often
	float sinaz = sin(rotcube_side);				// they are slow routines
	float cosaz = cos(rotcube_side);				// and we dont want slow!
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			p = rotate(j, i, size,sinax,cosax,sinay,cosay,sinaz,cosaz);
				drawPixel(p.x, p.y, RED);

			p = rotate(-size/16,i,j,sinax,cosax,sinay,cosay,sinaz,cosaz);
				drawPixel(p.x, p.y,YELLOW );

			p = rotate(i,-size/16,j,sinax,cosax,sinay,cosay,sinaz,cosaz);
				drawPixel(p.x, p.y, DKBLUE);
		}
	}
 }

 void fill3DCubeForSecondCube(int size)
  {
  	struct worldPoints p1;

  	int i,j;

  	for(i=55;i<size;i++)
  	{
  		for(j=55;j<size;j++)
  		{
  				p1=world_3dCordinateSystemForSecondCube(j,i,size);	//top fill
  				drawPixel(p1.x,p1.y,RED);

  				p1=world_3dCordinateSystemForSecondCube(i,size,j);	// right fill
  				drawPixel(p1.x,p1.y,DKBLUE);

  				p1=world_3dCordinateSystemForSecondCube(size,j,i);	// left fill
  				drawPixel(p1.x,p1.y,YELLOW);
  		}
  	}
  }
 void fill3DCubeForThirdCube(int size)
  {
	 struct worldPoints p1;

	   	int i,j;

	   	for(i=45;i<size;i++)
	   	{
	   		for(j=45;j<size;j++)
	   		{
	   				p1=world_3dCordinateSystemForThirdCube(j,i,size);	//top fill
	   				drawPixel(p1.x,p1.y,RED);

	   				p1=world_3dCordinateSystemForThirdCube(i,size,j);	// right fill
	   				drawPixel(p1.x,p1.y,DKBLUE);

	   				p1=world_3dCordinateSystemForThirdCube(size,j,i);	// left fill
	   				drawPixel(p1.x,p1.y,YELLOW);
	   		}
	   	}
  }
 void fill3DCubeForFourthCube(int size)
   {
	 struct worldPoints p;
	float roti = 0;                  	// starting amount of x rotation
	float rotj = 0;                 	// starting amount of y rotation
	float rotcube_side = 0;
	roti = roti - 3;          	// rotate the cube on X axis
	rotj = rotj - 3;          	// and on its y axis
	rotcube_side = rotcube_side + 0;
	float sinax = sin(roti);				// precalculate the sin and cos values
	float cosax = cos(roti);				// for the rotation as this saves a
	float sinay = sin(rotj);				// little time when running as we
	float cosay = cos(rotj);				// call sin and cos less often
	float sinaz = sin(rotcube_side);				// they are slow routines
	float cosaz = cos(rotcube_side);				// and we dont want slow!
	for (int i = 40; i < size; i++) {
		for (int j = 40; j < size; j++) {
			p = rotate4(j, i, size,sinax,cosax,sinay,cosay,sinaz,cosaz);
				drawPixel(p.x, p.y, RED);
			p = rotate4(40,i,j,sinax,cosax,sinay,cosay,sinaz,cosaz);
				drawPixel(p.x, p.y,YELLOW );
		}
	}
	for (int i = 52; i < size; i++) {
			for (int j = 52; j < size; j++) {
				p = rotate4(i-59,3,j+5,sinax,cosax,sinay,cosay,sinaz,cosaz);
					drawPixel(p.x, p.y, DKBLUE);
		}
	}
  }

 void squareSaver_insidecube(int size){

	   	int dx=0,dy=0,dz=0;
	   	float sinax, cosax, sinay, cosay, sinaz, cosaz;						// lots of work variables
		float rotx = 0;                  	// starting amount of x rotation
		float roty = 0;                 	// starting amount of y rotation
		float rotz = 0;                		// starting amount of z rotation
		rotx = rotx + 3;          	// rotate the cube on X axis
		roty = roty + 3;          	// and on its y axis
		rotz = rotz + 0;          // dont bother with z or it gets confusing
		sinax = sin(rotx);				// precalculate the sin and cos values
		cosax = cos(rotx);				// for the rotation as this saves a
		sinay = sin(roty);				// little time when running as we
		cosay = cos(roty);				// call sin and cos less often
		sinaz = sin(rotz);				// they are slow routines
		cosaz = cos(rotz);				// and we dont want slow!
	   	int g=0;
	   	while(g!=3){
	   		if(size==50){
	   			dx=rand()%(size-50);
	   			dy=rand()%(size-50);
	   			dz=40+rand()%42;

	   			switch(g){
	   				case 0:dx=-10;
				   	   	   dy=-10;
				   	   	   dz=-50;
				   	   	   break;
	   				case 1:dx=-10;
				   	   	   dy=-10;
				   	   	   dz=-50;
				   	   	   break;
	   				case 2:dx=-10;
				   	   	   dy=-10;
				   	   	   dz=-50;
				   	   	   break;
	   			}
	   		}
	   	  	float x0=dx,y0=dy;
	   	  	float x1=x0,y1=y0+dz;
	   	  	float x2=x0+dz,y2=y0+dz;
	   	  	float x3=x0+dz,y3=y0;
	   	 	struct worldPoints p00=rotate(0,size,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	   	 	struct worldPoints p1=rotate(0,0,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	   	    struct worldPoints p2=rotate(0,0,0,sinax,cosax,sinay,cosay,sinaz,cosaz);
	   	    struct worldPoints p3=rotate(0,size,0,sinax,cosax,sinay,cosay,sinaz,cosaz);

	   		x0=p00.x;
	   		y0=p00.y;
	   		x1=p1.x;
	   		y1=p1.y;
	   		x2=p2.x;
	   		y2=p2.y;
	   		x3=p3.x;
	   		y3=p3.y;
	   		square(x0, y0, x1, y1, x2, y2, x3, y3, 5);
	 		g++;
	   	}
 }

  void squareSaver_insidecubeForSecondCube(int size){

   	int dx=0,dy=0,dz=0;

   	int g=0;
   	while(g!=3){
   		if(size==80){
   			dx=rand()%(size-80);
   			dy=rand()%(size-80);
   			dz=40+rand()%80;

   			switch(g){
   				case 0:dx=1;
			   	   	   dy=1;
			   	   	   dz=40;
			   	   	   break;
   				case 1:dx=1;
			   	   	   dy=5;
			   	   	   dz=40;
			   	   	   break;
   				case 2:dx=1;
			   	   	   dy=1;
			   	   	   dz=40;
			   	   	   break;
   			}
   		}
   	  	float x0=dx,y0=dy;
   	  	float x1=x0,y1=y0+dz;
   	  	float x2=x0+dz,y2=y0+dz;
   	  	float x3=x0+dz,y3=y0;

   	  	struct worldPoints p00,p1,p2,p3;
   	  	/*front surface*/
   		p00=world_3dCordinateSystemForSecondCube(size,55,55);
   		p1=world_3dCordinateSystemForSecondCube(size,55,size);
   		p2=world_3dCordinateSystemForSecondCube(size,size,size);
   		p3=world_3dCordinateSystemForSecondCube(size,size,55);

   		x0=p00.x;
   		y0=p00.y;
   		x1=p1.x;
   		y1=p1.y;
   		x2=p2.x;
   		y2=p2.y;
   		x3=p3.x;
   		y3=p3.y;
   		square(x0, y0, x1, y1, x2, y2, x3, y3, 5);
 		g++;
   	}
 }

  void squareSaver_insidecubeForThirdCube(int size){

	  int dx=0,dy=0,dz=0;

	     	int g=0;
	     	while(g!=3){
	     		if(size==70){
	     			dx=rand()%(size-70);
	     			dy=rand()%(size-70);
	     			dz=40+rand()%80;

	     			switch(g){
	     				case 0:dx=1;
	  			   	   	   dy=1;
	  			   	   	   dz=40;
	  			   	   	   break;
	     				case 1:dx=1;
	  			   	   	   dy=5;
	  			   	   	   dz=40;
	  			   	   	   break;
	     				case 2:dx=1;
	  			   	   	   dy=1;
	  			   	   	   dz=40;
	  			   	   	   break;
	     			}
	     		}
	     	  	float x0=dx,y0=dy;
	     	  	float x1=x0,y1=y0+dz;
	     	  	float x2=x0+dz,y2=y0+dz;
	     	  	float x3=x0+dz,y3=y0;

	     	  	struct worldPoints p00,p1,p2,p3;
	     	  	/*front surface*/
	     		p00=world_3dCordinateSystemForThirdCube(size,45,45);
	     		p1=world_3dCordinateSystemForThirdCube(size,45,size);
	     		p2=world_3dCordinateSystemForThirdCube(size,size,size);
	     		p3=world_3dCordinateSystemForThirdCube(size,size,45);

	     		x0=p00.x;
	     		y0=p00.y;
	     		x1=p1.x;
	     		y1=p1.y;
	     		x2=p2.x;
	     		y2=p2.y;
	     		x3=p3.x;
	     		y3=p3.y;
	     		square(x0, y0, x1, y1, x2, y2, x3, y3, 5);
	   		g++;
	     	}
 }
 void squareSaver_insidecubeForFourthCube(int size){

	   	int dx=0,dy=0,dz=0;
	   	float sinax, cosax, sinay, cosay, sinaz, cosaz;						// lots of work variables
		float rotx = 0;                  	// starting amount of x rotation
		float roty = 0;                 	// starting amount of y rotation
		float rotz = 0;                		// starting amount of z rotation
		rotx = rotx - 3;          	// rotate the cube on X axis
		roty = roty - 3;          	// and on its y axis
		rotz = rotz + 0;          // dont bother with z or it gets confusing
		sinax = sin(rotx);				// precalculate the sin and cos values
		cosax = cos(rotx);				// for the rotation as this saves a
		sinay = sin(roty);				// little time when running as we
		cosay = cos(roty);				// call sin and cos less often
		sinaz = sin(rotz);				// they are slow routines
		cosaz = cos(rotz);				// and we dont want slow!
	   	int g=0;
	   	while(g!=3){
	   		if(size==90){
	   			dx=rand()%(size-90);
	   			dy=rand()%(size-90);
	   			dz=40+rand()%42;

	   			switch(g){
					case 0:dx=-10;
						   dy=-10;
						   dz=-50;
						   break;
					case 1:dx=-10;
						   dy=-10;
						   dz=-50;
						   break;
					case 2:dx=-10;
						   dy=-10;
						   dz=-50;
						   break;
				}
	   		}
	   	  	float x0=dx,y0=dy;
	   	  	float x1=x0,y1=y0+dz;
	   	  	float x2=x0+dz,y2=y0+dz;
	   	  	float x3=x0+dz,y3=y0;
	   	 	struct worldPoints p00=rotate4(40,size,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	   	 	struct worldPoints p1=rotate4(40,40,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	   	    struct worldPoints p2=rotate4(40,40,40,sinax,cosax,sinay,cosay,sinaz,cosaz);
	   	    struct worldPoints p3=rotate4(40,size,40,sinax,cosax,sinay,cosay,sinaz,cosaz);

	   		x0=p00.x;
	   		y0=p00.y;
	   		x1=p1.x;
	   		y1=p1.y;
	   		x2=p2.x;
	   		y2=p2.y;
	   		x3=p3.x;
	   		y3=p3.y;
	   		square(x0, y0, x1, y1, x2, y2, x3, y3, 5);
	 		g++;
	   	}
 }

 int cubetreeStraight(double x0, double x1)
 {
 	return (0.6*(x1-x0))+x1;
 }

 int cubetreeLeftX(double x1, double y1, double x2, double y2)
 {
 	return ((0.134*x1)+(0.866*x2)+(0.5*y2)-(0.5*y1));
 }

 int cubetreeLeftY(double x1, double y1, double x2, double y2)
 {
 	return ((0.5*x1)-(0.5*x2)+(1.866*y2)-(0.866*y1));
 }

 int cubetreeRightX(double x1, double y1, double x2, double y2)
 {
 	return ((0.134*x1)+(0.866*x2)-(0.5*y2)+(0.5*y1));
 }

 int cubetreeRightY(double x1, double y1, double x2, double y2)
 {
 	return ((0.5*x2)-(0.5*x1)+(1.866*y2)-(0.866*y1));
 }

 void cubedrawFastVLine(int16_t x, int16_t y,int16_t h, uint16_t color)
 {
   // Update in subclasses if desired!
	 drawLine(x, y, x, y+h-1, color);
 }

 void cubedrawFastHLine(int16_t x, int16_t y,int16_t w, uint16_t color)
 {
   // Update in subclasses if desired!
	 drawLine(x, y, x+w-1, y, color);
 }

 void drawRect(int16_t x, int16_t y,int16_t w, int16_t h,uint16_t color)
 {
   cubedrawFastHLine(x, y, w, color);
   cubedrawFastHLine(x, y+h-1, w, color);
   cubedrawFastVLine(x, y, h, color);
   cubedrawFastVLine(x+w-1, y, h, color);
 }
 void drawRectanglewith4points(struct worldPoints point1, struct worldPoints point2,
 		struct worldPoints point3, struct worldPoints point4, uint32_t color) {
 	drawLine(point1.x, point1.y, point2.x, point2.y, color);
 	drawLine(point2.x, point2.y, point3.x, point3.y, color);
 	drawLine(point3.x, point3.y, point4.x, point4.y, color);
 	drawLine(point4.x, point4.y, point1.x, point1.y, color);
 }
 void treeSaver_insidecube(double x0, double y0, double x1, double y1, int size)
 {
   	struct worldPoints p1,p2;
 	p1=world_3dCordinateSystem(x0,size-50,y0);
 	p2=world_3dCordinateSystem(x1,size-50,y1);
 	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

 	p1=world_3dCordinateSystem(x0+1,size-50,y0+1);
 	p2=world_3dCordinateSystem(x1+1,size-50,y1+1);
 	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

 	p1=world_3dCordinateSystem(x0,size-50,y0-1);
 	p2=world_3dCordinateSystem(x1,size-50,y1-1);
 	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

 	double xn[81],yn[81];
 	xn[0]=x1;
 	yn[0]=y1;

 	int i=0,temp1=0,temp2=0;

 	while(i!=21)
 	{
 		if(i%3==0)
 		{
 			xn[i+1] = cubetreeStraight(x0,x1);
 			yn[i+1] = cubetreeStraight(y0,y1);
 		}
 		if(i%3==1)
 		{
 			if(i==10||i==19){
 				xn[i]=xn[i]+5;
 			}
 			xn[i+1] = cubetreeLeftX(x1,y1,xn[i],yn[i]);
 			yn[i+1] = cubetreeLeftY(x1,y1,xn[i],yn[i]);
 		}
 		if(i%3==2)
 		{
 			xn[i+1] = cubetreeRightX(x1,y1,xn[i],yn[i-1]);
 			yn[i+1] = cubetreeRightY(x1,y1,xn[i],yn[i-1]);
 		}
 		p1=world_3dCordinateSystem(x1,size-50,y1);
 		p2=world_3dCordinateSystem(xn[i+1],size-50,yn[i+1]);
 		drawLine(p1.x,p1.y,p2.x,p2.y,GREEN);

 		i++;

 		if(i%3==0)
 		{
 			x1 = xn[temp1+1];
 			y1 = yn[temp1+1];

 			temp1++;
 		}

 		if(i==((9*temp2)+3))
 		{
 			x0 = xn[temp2];
 			y0 = yn[temp2];

 			temp2++;
 		}

 	}
  }
 void treeSaver_insideSecondCube(double x0, double y0, double x1, double y1, int size)
  {
    struct worldPoints p1,p2;
  	p1=world_3dCordinateSystemForSecondCube(x0,size-28,y0-10);
  	p2=world_3dCordinateSystemForSecondCube(x1,size-28,y1-10);
  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

  	p1=world_3dCordinateSystemForSecondCube(x0+1,size-28,y0+1);
  	p2=world_3dCordinateSystemForSecondCube(x1+1,size-28,y1+1);
  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

  	p1=world_3dCordinateSystemForSecondCube(x0-1,size-28,y0-1);
  	p2=world_3dCordinateSystemForSecondCube(x1-1,size-28,y1-1);
  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

  	double xn[81],yn[81];
  	xn[0]=x1;
  	yn[0]=y1;

  	int i=0,temp1=0,temp2=0;

  	while(i!=21)
  	{
  		if(i%3==0)
  		{
  			xn[i+1] = cubetreeStraight(x0,x1);
  			yn[i+1] = cubetreeStraight(y0,y1);
  		}
  		if(i%3==1)
  		{
  			if(i==10||i==19){
  				xn[i]=xn[i]+5;
  			}
  			xn[i+1] = cubetreeLeftX(x1,y1,xn[i],yn[i]);
  			yn[i+1] = cubetreeLeftY(x1,y1,xn[i],yn[i]);
  		}
  		if(i%3==2)
  		{
  			xn[i+1] = cubetreeRightX(x1,y1,xn[i],yn[i-1]);
  			yn[i+1] = cubetreeRightY(x1,y1,xn[i],yn[i-1]);
  		}
  		p1=world_3dCordinateSystemForSecondCube(x1,size-28,y1-10);
  		p2=world_3dCordinateSystemForSecondCube(xn[i+1],size-28,yn[i+1]-10);
  		drawLine(p1.x,p1.y,p2.x,p2.y,GREEN);

  		i++;

  		if(i%3==0)
  		{
  			x1 = xn[temp1+1];
  			y1 = yn[temp1+1];

  			temp1++;
  		}

  		if(i==((9*temp2)+3))
  		{
  			x0 = xn[temp2];
  			y0 = yn[temp2];

  			temp2++;
  		}

  	}
   }
 void treeSaver_insideThridcube(double x0, double y0, double x1, double y1, int size)
 {
	 struct worldPoints p1,p2;
	  	p1=world_3dCordinateSystemForThirdCube(x0,size-26,y0);
	  	p2=world_3dCordinateSystemForThirdCube(x1,size-26,y1);
	  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

	  	p1=world_3dCordinateSystemForThirdCube(x0+1,size-26,y0+1);
	  	p2=world_3dCordinateSystemForThirdCube(x1+1,size-26,y1+1);
	  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

	  	p1=world_3dCordinateSystemForThirdCube(x0-1,size-26,y0-1);
	  	p2=world_3dCordinateSystemForThirdCube(x1-1,size-26,y1-1);
	  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

	  	double xn[81],yn[81];
	  	xn[0]=x1;
	  	yn[0]=y1;

	  	int i=0,temp1=0,temp2=0;

	  	while(i!=21)
	  	{
	  		if(i%3==0)
	  		{
	  			xn[i+1] = cubetreeStraight(x0,x1);
	  			yn[i+1] = cubetreeStraight(y0,y1);
	  		}
	  		if(i%3==1)
	  		{
	  			if(i==10||i==19){
	  				xn[i]=xn[i]+5;
	  			}
	  			xn[i+1] = cubetreeLeftX(x1,y1,xn[i],yn[i]);
	  			yn[i+1] = cubetreeLeftY(x1,y1,xn[i],yn[i]);
	  		}
	  		if(i%3==2)
	  		{
	  			xn[i+1] = cubetreeRightX(x1,y1,xn[i],yn[i-1]);
	  			yn[i+1] = cubetreeRightY(x1,y1,xn[i],yn[i-1]);
	  		}
	  		p1=world_3dCordinateSystemForThirdCube(x1,size-26,y1);
	  		p2=world_3dCordinateSystemForThirdCube(xn[i+1],size-26,yn[i+1]);
	  		drawLine(p1.x,p1.y,p2.x,p2.y,GREEN);

	  		i++;

	  		if(i%3==0)
	  		{
	  			x1 = xn[temp1+1];
	  			y1 = yn[temp1+1];

	  			temp1++;
	  		}

	  		if(i==((9*temp2)+3))
	  		{
	  			x0 = xn[temp2];
	  			y0 = yn[temp2];

	  			temp2++;
	  		}

	  	}
  }
 void treeSaver_insideFourthcube(double x0, double y0, double x1, double y1, int size)
  {
    	struct worldPoints p1,p2;
  	p1=world_3dCordinateSystemForFourthCube(x0,size-200,y0);
  	p2=world_3dCordinateSystemForFourthCube(x1,size-200,y1);
  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

  	p1=world_3dCordinateSystemForFourthCube(x0+1,size-200,y0+1);
  	p2=world_3dCordinateSystemForFourthCube(x1+1,size-200,y1+1);
  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

  	p1=world_3dCordinateSystemForFourthCube(x0,size-200,y0-1);
  	p2=world_3dCordinateSystemForFourthCube(x1,size-200,y1-1);
  	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

  	double xn[81],yn[81];
  	xn[0]=x1;
  	yn[0]=y1;

  	int i=0,temp1=0,temp2=0;

  	while(i!=21)
  	{
  		if(i%3==0)
  		{
  			xn[i+1] = cubetreeStraight(x0,x1);
  			yn[i+1] = cubetreeStraight(y0,y1);
  		}
  		if(i%3==1)
  		{
  			if(i==10||i==19){
  				xn[i]=xn[i]+5;
  			}
  			xn[i+1] = cubetreeLeftX(x1,y1,xn[i],yn[i]);
  			yn[i+1] = cubetreeLeftY(x1,y1,xn[i],yn[i]);
  		}
  		if(i%3==2)
  		{
  			xn[i+1] = cubetreeRightX(x1,y1,xn[i],yn[i-1]);
  			yn[i+1] = cubetreeRightY(x1,y1,xn[i],yn[i-1]);
  		}
  		p1=world_3dCordinateSystemForFourthCube(x1,size-200,y1);
  		p2=world_3dCordinateSystemForFourthCube(xn[i+1],size-200,yn[i+1]);
  		drawLine(p1.x,p1.y,p2.x,p2.y,GREEN);

  		i++;

  		if(i%3==0)
  		{
  			x1 = xn[temp1+1];
  			y1 = yn[temp1+1];

  			temp1++;
  		}

  		if(i==((9*temp2)+3))
  		{
  			x0 = xn[temp2];
  			y0 = yn[temp2];

  			temp2++;
  		}

  	}
   }
 void drawInitialinsideFourthcube(int size)
 {
 	struct worldPoints p1;
 	float sinax, cosax, sinay, cosay, sinaz, cosaz;						// lots of work variables
	float rotx = 0;                  	// starting amount of x rotation
	float roty = 0;                 	// starting amount of y rotation
	float rotz = 0;                		// starting amount of z rotation
	rotx = rotx - 3;          	// rotate the cube on X axis
	roty = roty - 3;          	// and on its y axis
	rotz = rotz + 0;          // dont bother with z or it gets confusing
	sinax = sin(rotx);				// precalculate the sin and cos values
	cosax = cos(rotx);				// for the rotation as this saves a
	sinay = sin(roty);				// little time when running as we
	cosay = cos(roty);				// call sin and cos less often
	sinaz = sin(rotz);				// they are slow routines
	cosaz = cos(rotz);				// and we dont want slow!

 	int i,j;
 	int colormap[size][size];

 	for(i=40;i<size;i++)
 	{
 		for(j=40;j<size;j++)
 		{
 			if(i>=48&& i<=50 && j>=48 && j<=size-10)
 				colormap[i][j]=1;
 			else if(i>=50  && i<=52 && j>=48 && j<=50)
 				colormap[i][j]=1;
 			else if(i>=52 && i<=54 && j>=48 && j<=size-10)
 				colormap[i][j]=1;
 			else if(i>=54 && i<=56 && j>=size-20 && j<=size-10)
 				colormap[i][j]=1;
 			else if(i>=56 && i<=58 && j>=48 && j<=size-10)
 				colormap[i][j]=1;
 			else
 				colormap[i][j]=0;
 		}
 	}

 	for(i=40;i<size;i++)
 	{
 		for(j=40;j<size;j++)
 		{
 			if(colormap[i][j]==1)
 			{
 				p1=rotate4(j,i,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
 				drawPixel(p1.x,p1.y,BLACK);
 			}
 		}
 	}
 }
 void drawInitialinsideSecondcube(int size)
 {
 	struct worldPoints p1;

 	int i,j;
 	int colormap[size][size];

 	for(i=55;i<size;i++)
 	{
 		for(j=55;j<size;j++)
 		{
 			if(i>=57 && i<=59 && j>=57 && j<=size-2)
 			 	colormap[i][j]=1;
 			else if(i>=59  && i<=63 && j>=57 && j<=63)
 			 	colormap[i][j]=1;
 			else if(i>=63 && i<=67 && j>=57 && j<=size-2)
 			 	colormap[i][j]=1;
 			else if(i>=67 && i<=73 && j>=size-7 && j<=size-2)
 			 	colormap[i][j]=1;
 			else if(i>=73 && i<=76 && j>=57 && j<=size-2)
 			 	colormap[i][j]=1;
 			else
 			    colormap[i][j]=0;
 		}
 	}

 	for(i=55;i<size;i++)
 	{
 		for(j=55;j<size;j++)
 		{
 			if(colormap[i][j]==1)
 			{
 				p1=world_3dCordinateSystemForSecondCube(j,i,size);
 				drawPixel(p1.x,p1.y,BLACK);
 			}
 		}
 	}
 }
 void drawInitialinsideThirdcube(int size)
 {
	 struct worldPoints p1;

	  	int i,j;
	  	int colormap[size][size];

	  	for(i=45;i<size;i++)
	  	{
	  		for(j=45;j<size;j++)
	  		{
	  			if(i>=47 && i<=52 && j>=47 && j<=size-2)
	  			 	colormap[i][j]=1;
	  			else if(i>=52  && i<=56 && j>=47 && j<=52)
	  			 	colormap[i][j]=1;
	  			else if(i>=56 && i<=60 && j>=47 && j<=size-2)
	  			 	colormap[i][j]=1;
	  			else if(i>=60 && i<=64 && j>=size-7 && j<=size-2)
	  			 	colormap[i][j]=1;
	  			else if(i>=64 && i<=67 && j>=47 && j<=size-2)
	  			 	colormap[i][j]=1;
	  			else
	  			 	colormap[i][j]=0;
	  		}
	  	}

	  	for(i=45;i<size;i++)
	  	{
	  		for(j=45;j<size;j++)
	  		{
	  			if(colormap[i][j]==1)
	  			{
	  				p1=world_3dCordinateSystemForThirdCube(j,i,size);
	  				drawPixel(p1.x,p1.y,BLACK);
	  			}
	  		}
	  	}
 }
 void drawInitialinsidecube(int size)
  {
  	struct worldPoints p1;
  	float sinax, cosax, sinay, cosay, sinaz, cosaz;						// lots of work variables
 	float rotx = 0;                  	// starting amount of x rotation
 	float roty = 0;                 	// starting amount of y rotation
 	float rotz = 0;                		// starting amount of z rotation
 	rotx = rotx + 3;          	// rotate the cube on X axis
 	roty = roty + 3;          	// and on its y axis
 	rotz = rotz + 0;          // dont bother with z or it gets confusing
 	sinax = sin(rotx);				// precalculate the sin and cos values
 	cosax = cos(rotx);				// for the rotation as this saves a
 	sinay = sin(roty);				// little time when running as we
 	cosay = cos(roty);				// call sin and cos less often
 	sinaz = sin(rotz);				// they are slow routines
 	cosaz = cos(rotz);				// and we dont want slow!

  	int i,j;
  	int colormap[size][size];

  	for(i=0;i<size;i++)
  	{
  		for(j=0;j<size;j++)
  		{
  			if(i>=4&& i<=10 && j>=5 && j<=size-4)
  				colormap[i][j]=1;
  			else if(i>=10  && i<=18 && j>=5 && j<=10)
  				colormap[i][j]=1;
  			else if(i>=18 && i<=25 && j>=5 && j<=size-4)
  				colormap[i][j]=1;
  			else if(i>=25 && i<=34 && j>=size-7 && j<=size-2)
  				colormap[i][j]=1;
  			else if(i>=34 && i<=40 && j>=5 && j<=size-4)
  				colormap[i][j]=1;
  			else
  				colormap[i][j]=0;
  		}
  	}

  	for(i=0;i<size;i++)
  	{
  		for(j=0;j<size;j++)
  		{
  			if(colormap[i][j]==1)
  			{
  				p1=rotate(j,i,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
  				drawPixel(p1.x,p1.y,BLACK);
  			}
  		}
  	}
  }
 void forest(){
	 for(int i=0;i<30;i++){
	     	  	 srand(time(NULL));
	     	  	 int levels = (rand()%4)+3;
	     	 	 squarecolor = (rand()%11)+1;
	     	 	 float x0 = (float)(rand()%100)+60;
	     	 	 float y0 =  (float)(rand()%80)+20;
	     	 	 int angle = (rand()%35)+20;
	     		 int len = (rand()%60)+10;
	     	 	 float x1 = x0;
	     	 	 float y1 = y0+len;
	     		 drawTree(x0,y0,x1,y1,squarecolor,levels,angle);
	     		 lcddelay(10);
	  }
	  for(int i=10;i>0;i--){
	     		  drawCircle(220,305,i,LTBLUE);
	  }
 }

 void drawRotatedOutlineOfCube1(int cube_size){  // routine to draw and calc 3d cube

 	float sinax, cosax, sinay, cosay, sinaz, cosaz;						// lots of work variables
 	float rotx = 0;                  	// starting amount of x rotation
 	float roty = 0;                 	// starting amount of y rotation
 	float rotz = 0;                		// starting amount of z rotation
	rotx = rotx + 3;          	// rotate the cube on X axis
 	roty = roty + 3;          	// and on its y axis
 	rotz = rotz + 0;          // dont bother with z or it gets confusing
	sinax = sin(rotx);				// precalculate the sin and cos values
 	cosax = cos(rotx);				// for the rotation as this saves a
 	sinay = sin(roty);				// little time when running as we
 	cosay = cos(roty);				// call sin and cos less often
 	sinaz = sin(rotz);				// they are slow routines
 	cosaz = cos(rotz);				// and we dont want slow!
	struct worldPoints s1=rotate(0,0,cube_size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s2=rotate(cube_size,0,cube_size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s3=rotate(cube_size,cube_size,cube_size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s4=rotate(0,cube_size,cube_size,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s5=rotate(0,0,0,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s6=rotate(cube_size,0,0,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s7=rotate(cube_size,cube_size,0,sinax,cosax,sinay,cosay,sinaz,cosaz);
	struct worldPoints s8=rotate(0,cube_size,0,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	drawOutlineCube(s1,s2,s3,s4,s5,s6,s7,s8);
	fill3DCube1(cube_size);
 }
 void drawRotatedOutlineOfCubeForFourthCube(int size)
    {
    float sinax, cosax, sinay, cosay, sinaz, cosaz;						// lots of work variables
  	float rotx = 0;                  	// starting amount of x rotation
  	float roty = 0;                 	// starting amount of y rotation
  	float rotz = 0;                		// starting amount of z rotation
 	rotx = rotx -3;          	// rotate the cube on X axis
  	roty = roty -3;          	// and on its y axis
  	rotz = rotz + 0;          // dont bother with z or it gets confusing
 	sinax = sin(rotx);				// precalculate the sin and cos values
  	cosax = cos(rotx);				// for the rotation as this saves a
  	sinay = sin(roty);				// little time when running as we
  	cosay = cos(roty);				// call sin and cos less often
  	sinaz = sin(rotz);				// they are slow routines
  	cosaz = cos(rotz);				// and we dont want slow!
 	struct worldPoints s1=rotate4(40,40,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s2=rotate4(size,40,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s3=rotate4(size,size,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s4=rotate4(40,size,size,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s5=rotate4(40,40,40,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s6=rotate4(size,40,40,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s7=rotate4(size,size,40,sinax,cosax,sinay,cosay,sinaz,cosaz);
 	struct worldPoints s8=rotate4(40,size,40,sinax,cosax,sinay,cosay,sinaz,cosaz);
  	drawOutlineCube(s1,s2,s3,s4,s5,s6,s7,s8);
 	fill3DCubeForFourthCube(size);
    }

 struct worldPoints Shadow_eqncube1(struct world_3DPoints n, struct world_3DPoints pi)
{
	float lamda;
	struct world_3DPoints ps;
	struct world_3DPoints R;
	struct worldPoints intersection_2d;
	ps.x=500;
	ps.y=-1000;
	ps.z=3100;
	float numerator = -((n.x*pi.x)+(n.y*pi.y)+(n.z*pi.z));
	float denominator= (n.x*(ps.x-pi.x))+(n.y*(ps.y-pi.y))+(n.z*(ps.z-pi.z));
	lamda = numerator/denominator;
	R.x=pi.x+lamda*(ps.x-pi.x);
	R.y=pi.y+lamda*(ps.y-pi.y);
	R.z=pi.z+lamda*(ps.z-pi.z);
	intersection_2d=world_3dCordinateSystem(R.x,R.y,R.z);
	return intersection_2d;
}
struct worldPoints Shadow_eqncube2(struct world_3DPoints n, struct world_3DPoints pi)
{
	float lamda;
	struct world_3DPoints ps;
	struct world_3DPoints R;
	struct worldPoints intersection_2d;
	ps.x=500;
	ps.y=-1000;
	ps.z=3100;
	float numerator = -((n.x*pi.x)+(n.y*pi.y)+(n.z*pi.z));
	float denominator= (n.x*(ps.x-pi.x))+(n.y*(ps.y-pi.y))+(n.z*(ps.z-pi.z));
	lamda = numerator/denominator;
	R.x=pi.x+lamda*(ps.x-pi.x);
	R.y=pi.y+lamda*(ps.y-pi.y);
	R.z=pi.z+lamda*(ps.z-pi.z);
	intersection_2d=world_3dCordinateSystemForSecondCube(R.x,R.y,R.z);
	return intersection_2d;
}
struct worldPoints Shadow_eqncube3(struct world_3DPoints n, struct world_3DPoints pi)
{
	float lamda;
	struct world_3DPoints ps;
	struct world_3DPoints R;
	struct worldPoints intersection_2d;
	ps.x= 500;
	ps.y=-1000;
	ps.z=3100;
	float numerator = -((n.x*pi.x)+(n.y*pi.y)+(n.z*pi.z));
	float denominator= (n.x*(ps.x-pi.x))+(n.y*(ps.y-pi.y))+(n.z*(ps.z-pi.z));
	lamda = numerator/denominator;
	R.x=pi.x+lamda*(ps.x-pi.x);
	R.y=pi.y+lamda*(ps.y-pi.y);
	R.z=pi.z+lamda*(ps.z-pi.z);
	intersection_2d=world_3dCordinateSystemForThirdCube(R.x,R.y,R.z);
	return intersection_2d;
}
struct worldPoints Shadow_eqncube4(struct world_3DPoints n, struct world_3DPoints pi)
{
	float lamda;
	struct world_3DPoints ps;
	struct world_3DPoints R;
	struct worldPoints intersection_2d;
	ps.x= 500;
	ps.y=-1000;
	ps.z=3100;
	float numerator = -((n.x*pi.x)+(n.y*pi.y)+(n.z*pi.z));
	float denominator= (n.x*(ps.x-pi.x))+(n.y*(ps.y-pi.y))+(n.z*(ps.z-pi.z));
	lamda = numerator/denominator;
	R.x=pi.x+lamda*(ps.x-pi.x);
	R.y=pi.y+lamda*(ps.y-pi.y);
	R.z=pi.z+lamda*(ps.z-pi.z);
	intersection_2d=world_3dCordinateSystemForFourthCube(R.x,R.y,R.z);
	return intersection_2d;
}
 void shadowforfirstcube()
 {
	struct worldPoints p;
	struct world_3DPoints n,pi1,pi2,pi3,pi4,pi,n1;
	int cube_side=50;
	n.x=0;n.y=0;n.z=1;
	pi1.x=50;pi1.y=50,pi1.z=50;
	pi2.x=0;pi2.y=50,pi2.z=50;
	pi3.x=0;pi3.y=0,pi3.z=50;
	pi4.x=50;pi4.y=0,pi4.z=50;
	struct worldPoints p1,p2,p3,p4;
	p1=Shadow_eqncube1(n,pi1);
	p2=Shadow_eqncube1(n,pi2);
	p3=Shadow_eqncube1(n,pi3);
	p4=Shadow_eqncube1(n,pi4);
	drawLine(p1.x, p1.y, p2.x, p2.y, 0x003300);
 	drawLine(p2.x, p2.y, p3.x, p3.y, 0x003300);
 	drawLine(p3.x, p3.y, p4.x, p4.y, 0x003300);
 	drawLine(p4.x, p4.y, p1.x, p1.y, 0x003300);
	for (int i = 0; i < cube_side; i++) {
		for (int j = 0; j < cube_side; j++) {
			pi.x=j;
			pi.y=i;
			pi.z=cube_side;
			n1.x=0;n1.y=0;n1.z=1;
			p = Shadow_eqncube1(n1,pi);
			drawPixel(p.x, p.y, 0x003300);
		}
	}
}
 void shadowforSecondCube()
 {
	struct worldPoints p;
 	struct world_3DPoints n,pi1,pi2,pi3,pi4,pi,n1;
	int cube_side=80;
	n.x=0;n.y=0;n.z=1;
	pi1.x=80;pi1.y=80,pi1.z=80;
	pi2.x=55;pi2.y=80,pi2.z=80;
	pi3.x=55;pi3.y=55,pi3.z=80;
	pi4.x=80;pi4.y=55,pi4.z=80;
	struct worldPoints p1,p2,p3,p4;
	p1=Shadow_eqncube2(n,pi1);
	p2=Shadow_eqncube2(n,pi2);
	p3=Shadow_eqncube2(n,pi3);
	p4=Shadow_eqncube2(n,pi4);
	drawLine(p1.x, p1.y, p2.x, p2.y, 0x003300);
 	drawLine(p2.x, p2.y, p3.x, p3.y, 0x003300);
 	drawLine(p3.x, p3.y, p4.x, p4.y, 0x003300);
 	drawLine(p4.x, p4.y, p1.x, p1.y, 0x003300);
	for (int i = 55; i < cube_side; i++) {
		for (int j = 55; j < cube_side; j++) {
			pi.x=j;
			pi.y=i;
			pi.z=cube_side;
			n1.x=0;n1.y=0;n1.z=1;
			p = Shadow_eqncube2(n1,pi);
			drawPixel(p.x, p.y, 0x003300);
		}
	}
 }

 void shadowforThirdCube()
  {
	struct worldPoints p;
  	struct world_3DPoints n,pi1,pi2,pi3,pi4,pi,n1;
  	int cube_side =70;
	n.x=0;n.y=0;n.z=1;
	pi1.x=70;pi1.y=70,pi1.z=70;
	pi2.x=45;pi2.y=70,pi2.z=70;
	pi3.x=45;pi3.y=45,pi3.z=70;
	pi4.x=70;pi4.y=45,pi4.z=70;
	struct worldPoints p1,p2,p3,p4;
	p1=Shadow_eqncube3(n,pi1);
	p2=Shadow_eqncube3(n,pi2);
	p3=Shadow_eqncube3(n,pi3);
	p4=Shadow_eqncube3(n,pi4);
	drawLine(p1.x, p1.y, p2.x, p2.y, 0x003300);
 	drawLine(p2.x, p2.y, p3.x, p3.y, 0x003300);
 	drawLine(p3.x, p3.y, p4.x, p4.y, 0x003300);
 	drawLine(p4.x, p4.y, p1.x, p1.y, 0x003300);
	for (int i = 45; i < cube_side; i++) {
		for (int j = 45; j < cube_side; j++) {
			pi.x=j;
			pi.y=i;
			pi.z=cube_side;
			n1.x=0;n1.y=0;n1.z=1;
			p = Shadow_eqncube3(n1,pi);
			drawPixel(p.x, p.y, 0x003300);
		}
	}
  }
 void shadowforFourthCube()
   {
 	struct worldPoints p;
   	struct world_3DPoints n,pi1,pi2,pi3,pi4,pi,n1;
   	int cube_side =90;
 	n.x=0;n.y=0;n.z=1;
 	pi1.x=90;pi1.y=90,pi1.z=90;
 	pi2.x=40;pi2.y=90,pi2.z=90;
 	pi3.x=40;pi3.y=40,pi3.z=90;
 	pi4.x=90;pi4.y=40,pi4.z=90;
 	struct worldPoints p1,p2,p3,p4;
 	p1=Shadow_eqncube4(n,pi1);
 	p2=Shadow_eqncube4(n,pi2);
 	p3=Shadow_eqncube4(n,pi3);
 	p4=Shadow_eqncube4(n,pi4);
 	drawLine(p1.x, p1.y, p2.x, p2.y, 0x003300);
  	drawLine(p2.x, p2.y, p3.x, p3.y, 0x003300);
  	drawLine(p3.x, p3.y, p4.x, p4.y, 0x003300);
  	drawLine(p4.x, p4.y, p1.x, p1.y, 0x003300);
 	for (int i = 40; i < cube_side; i++) {
 		for (int j = 40; j < cube_side; j++) {
 			pi.x=j;
 			pi.y=i;
 			pi.z=cube_side;
 			n1.x=0;n1.y=0;n1.z=1;
 			p = Shadow_eqncube4(n1,pi);
 			drawPixel(p.x, p.y, 0x003300);
 		}
 	}
   }
 void RandomCubes(){
 	    pCam.xe=110;
 	  	pCam.ye=110;
 	  	pCam.ze=110;
 		drawRect(0,0,240,320,YELLOW);
 		p0=world_3dCordinateSystem(0,0,0);
 		//draw_3dCordinateSystem();
 		/*-------first cube starts--------*/
 		int cube_size=50;
 		drawRotatedOutlineOfCube1(cube_size);
 		/*-------first cube ends--------*/

 		/*-------second cube starts--------*/
 		cube_size = 80;
 		drawOutlineOfCubeForSecondCube(cube_size);
 		fill3DCubeForSecondCube(cube_size);
 		/*-------second cube ends--------*/

 		/*-------third cube starts--------*/
 		cube_size = 70;
 		drawOutlineOfCubeForThirdCube(cube_size);
 		fill3DCubeForThirdCube(cube_size);
 		/*-------third cube ends--------*/

 		/*-------fourth cube starts--------*/
 		cube_size = 90;
 		drawRotatedOutlineOfCubeForFourthCube(cube_size);
 		/*-------fourth cube ends--------*/
  }
 void cubewithcolorfill(){
 	    pCam.xe=110;
 	  	pCam.ye=110;
 	  	pCam.ze=110;
 		drawRect(0,0,240,320,YELLOW);
 		p0=world_3dCordinateSystem(0,0,0);
 		draw_3dCordinateSystem();
 		/*-------third cube starts--------*/
 		int cube_size = 70;
 		drawOutlineOfCubeForThirdCube(cube_size);
 		fill3DCubeForThirdCube(cube_size);
 		/*-------third cube ends--------*/
  }
 void cubeswithcolorfill(){
 	    pCam.xe=110;
 	  	pCam.ye=110;
 	  	pCam.ze=110;
 		drawRect(0,0,240,320,YELLOW);
 		p0=world_3dCordinateSystem(0,0,0);
 		draw_3dCordinateSystem();
 		/*-------first cube starts--------*/
 		int cube_size=50;
 		drawRotatedOutlineOfCube1(cube_size);

 		/*-------first cube ends--------*/

 		/*-------second cube starts--------*/
 		cube_size = 80;
 		drawOutlineOfCubeForSecondCube(cube_size);
 		fill3DCubeForSecondCube(cube_size);
 		/*-------second cube ends--------*/

 		/*-------third cube starts--------*/
 		cube_size = 70;
 		drawOutlineOfCubeForThirdCube(cube_size);
 		fill3DCubeForThirdCube(cube_size);
 		/*-------third cube ends--------*/

 		/*-------fourth cube starts--------*/
 		cube_size = 90;
 		drawRotatedOutlineOfCubeForFourthCube(cube_size);
 		/*-------fourth cube ends--------*/
  }
 void Drawcubewithdec(){
 	    pCam.xe=110;
 	  	pCam.ye=110;
 	  	pCam.ze=110;
 		drawRect(0,0,240,320,YELLOW);
 		p0=world_3dCordinateSystem(0,0,0);
 		//draw_3dCordinateSystem();
 		//for x-y plane coloring

 		/*-------second cube starts--------*/
 		int cube_size = 80;
 		drawOutlineOfCubeForSecondCube(cube_size);
 		fill3DCubeForSecondCube(cube_size);
 		squareSaver_insidecubeForSecondCube(cube_size);
 		treeSaver_insideSecondCube(15,3,15,17,cube_size);
 		treeSaver_insideSecondCube(30,3,30,17,cube_size);
 		treeSaver_insideSecondCube(45,3,45,17,cube_size);
 		drawInitialinsideSecondcube(cube_size);
 		/*-------second cube ends--------*/
  }
 void Drawcubeswithdec(){
 	    pCam.xe=110;
 	  	pCam.ye=110;
 	  	pCam.ze=110;
 		drawRect(0,0,240,320,YELLOW);
 		p0=world_3dCordinateSystem(0,0,0);
 		draw_3dCordinateSystem();
 		//for x-y plane coloring
 		for(int i=0;i<500;++i){
 			struct worldPoints p1,p2;
 		 	p1=world_3dCordinateSystem(i,0,0);
 		 	p2=world_3dCordinateSystem(0,i,0);
 		 	drawLine(p1.x, p1.y,p2.x,p2.y,ORANGE);
 		 }
 		fillrect(2, 240, ST7735_TFTWIDTH-2, ST7735_TFTHEIGHT,ORANGE);
 		/*-------first cube starts--------*/
 		int cube_size=50;
 		drawRotatedOutlineOfCube1(cube_size);
 		squareSaver_insidecube(cube_size);
 		treeSaver_insidecube(-32,-4,-32,10,cube_size);
 		treeSaver_insidecube(-19,-4,-19,10,cube_size);
 		treeSaver_insidecube(-7,-4,-7,10,cube_size);
 	  	drawInitialinsidecube(cube_size);
 		/*-------first cube ends--------*/

 		/*-------second cube starts--------*/
 		cube_size = 80;
 		drawOutlineOfCubeForSecondCube(cube_size);
 		fill3DCubeForSecondCube(cube_size);
 		squareSaver_insidecubeForSecondCube(cube_size);
 		treeSaver_insideSecondCube(15,3,15,17,cube_size);
 		treeSaver_insideSecondCube(30,3,30,17,cube_size);
 		treeSaver_insideSecondCube(45,3,45,17,cube_size);
 		drawInitialinsideSecondcube(cube_size);
 		/*-------second cube ends--------*/

 		/*-------third cube starts--------*/
 		cube_size = 70;
 		drawOutlineOfCubeForThirdCube(cube_size);
 		fill3DCubeForThirdCube(cube_size);
 		squareSaver_insidecubeForThirdCube(cube_size);
 		treeSaver_insideThridcube(10,3,10,13,cube_size);
 		treeSaver_insideThridcube(25,3,25,13,cube_size);
 		treeSaver_insideThridcube(37,3,37,13,cube_size);
 		drawInitialinsideThirdcube(cube_size);
 		/*-------third cube ends--------*/

 		/*-------fourth cube starts--------*/
 		cube_size = 90;
 		drawRotatedOutlineOfCubeForFourthCube(cube_size);
 		squareSaver_insidecubeForFourthCube(cube_size);
 		treeSaver_insideFourthcube(-198,-4,-198,10,cube_size);
 		treeSaver_insideFourthcube(-175,-4,-175,10,cube_size);
 		treeSaver_insideFourthcube(-155,-4,-155,10,cube_size);
 		//drawInitialinsideFourthcube(cube_size);
 		/*-------fourth cube ends--------*/
  }
 void cubeWithGraphics(){
	    pCam.xe=110;
	  	pCam.ye=110;
	  	pCam.ze=110;
		drawRect(0,0,240,320,YELLOW);
		p0=world_3dCordinateSystem(0,0,0);
		draw_3dCordinateSystem();
		//for x-y plane coloring
		for(int i=0;i<500;++i){
			struct worldPoints p1,p2;
		 	p1=world_3dCordinateSystem(i,0,0);
		 	p2=world_3dCordinateSystem(0,i,0);
		 	drawLine(p1.x, p1.y,p2.x,p2.y,ORANGE);
		 }
		fillrect(2, 240, ST7735_TFTWIDTH-2, ST7735_TFTHEIGHT,ORANGE);
		/*-------first cube starts--------*/
		int cube_size=50;
	 	shadowforfirstcube();
		drawRotatedOutlineOfCube1(cube_size);
		squareSaver_insidecube(cube_size);
		treeSaver_insidecube(-32,-4,-32,10,cube_size);
		treeSaver_insidecube(-19,-4,-19,10,cube_size);
		treeSaver_insidecube(-7,-4,-7,10,cube_size);
	  	drawInitialinsidecube(cube_size);
		/*-------first cube ends--------*/

		/*-------second cube starts--------*/
		cube_size = 80;
		shadowforSecondCube();
		drawOutlineOfCubeForSecondCube(cube_size);
		fill3DCubeForSecondCube(cube_size);
		squareSaver_insidecubeForSecondCube(cube_size);
		treeSaver_insideSecondCube(15,3,15,17,cube_size);
		treeSaver_insideSecondCube(30,3,30,17,cube_size);
		treeSaver_insideSecondCube(45,3,45,17,cube_size);
		drawInitialinsideSecondcube(cube_size);
		/*-------second cube ends--------*/

		/*-------third cube starts--------*/
		cube_size = 70;
		shadowforThirdCube();
		drawOutlineOfCubeForThirdCube(cube_size);
		fill3DCubeForThirdCube(cube_size);
		squareSaver_insidecubeForThirdCube(cube_size);
		treeSaver_insideThridcube(10,3,10,13,cube_size);
		treeSaver_insideThridcube(25,3,25,13,cube_size);
		treeSaver_insideThridcube(37,3,37,13,cube_size);
		drawInitialinsideThirdcube(cube_size);
		/*-------third cube ends--------*/

		/*-------fourth cube starts--------*/
		cube_size = 90;
		shadowforFourthCube();
		drawRotatedOutlineOfCubeForFourthCube(cube_size);
		squareSaver_insidecubeForFourthCube(cube_size);
		treeSaver_insideFourthcube(-198,-4,-198,10,cube_size);
		treeSaver_insideFourthcube(-175,-4,-175,10,cube_size);
		treeSaver_insideFourthcube(-155,-4,-155,10,cube_size);
		//drawInitialinsideFourthcube(cube_size);
		/*-------fourth cube ends--------*/
 }
 //Initialize the port and pin as outputs.
 void GPIOinitOut(uint8_t portNum, uint32_t pinNum)
 {
 	if (portNum == 0)
 	{
 		LPC_GPIO0->FIODIR |= (1 << pinNum);
 		LPC_PINCON->PINMODE0 |= (0 << (pinNum*2));
 	}
 	else if (portNum == 1)
 	{
 		LPC_GPIO1->FIODIR |= (1 << pinNum);
 	}
 	else if (portNum == 2)
 	{
 		LPC_GPIO2->FIODIR |= (1 << pinNum);
 	}
 	else
 	{
 		puts("Not a valid port!\n");
 	}
 }
 void GPIOinitIn(uint8_t portNum, uint32_t pinNum)
 {
 	if (portNum == 0)
 	{
 		LPC_GPIO0->FIODIR=(0<<pinNum);
 	}
 	else if (portNum == 1)
 	{
 		LPC_GPIO1->FIODIR=(0<<pinNum);
 	}
 	else if (portNum == 2)
 	{
 		LPC_GPIO2->FIODIR=(0<<pinNum);
 	}
 	else
 	{
 		puts("Not a valid port!\n");
 	}
 }
 void setGPIO(uint8_t portNum, uint32_t pinNum)
 {
 	if (portNum == 0)
 	{
 		LPC_GPIO0->FIOSET = (1 << pinNum);
 		printf("Pin 0.%d has been set.\n",pinNum);
 	}
 	//Can be used to set pins on other ports for future modification
 	else
 	{
 		puts("Only port 0 is used, try again!\n");
 	}
 }

 //Deactivate the pin
 void clearGPIO(uint8_t portNum, uint32_t pinNum)
 {
 	if (portNum == 0)
 	{
 		LPC_GPIO0->FIOCLR = (1 << pinNum);
 		printf("Pin 0.%d has been cleared.\n", pinNum);
 	}
 	//Can be used to clear pins on other ports for future modification
 	else
 	{
 		puts("Only port 0 is used, try again!\n");
 	}
 }
 void ReadGPIO1(uint32_t portNum, uint32_t pinNum)
 {
 	uint32_t switchStatus;
 	if(portNum==2)
 	{
 		switchStatus = (LPC_GPIO2->FIOPIN>>pinNum) & 0x01 ;
 		if(switchStatus==0)
 		{
 			printf("port2.%d has been pressed\n",pinNum);
 			if(pinNum==6){
 				option = 1;
			printf("option is set to %d\n",option);
 			}else if(pinNum==7){
 				option = 2;
			printf("option is set to %d\n",option);
 			}else if(pinNum==8){
 				option = 3;
			printf("option is set to %d\n",option);
 			}else if(pinNum==10){
 				option = 4;
			printf("option is set to %d\n",option);
 			}
 		}
 	}
 }
 void ReadGPIO2(uint32_t portNum, uint32_t pinNum)
 {
 	uint32_t switchStatus;
 	if(portNum==2)
 	{
 		switchStatus = (LPC_GPIO2->FIOPIN>>pinNum) & 0x01 ;
 		if(switchStatus==0)
 		{
 			printf("port2.%d has been pressed\n",pinNum);
 			if(pinNum==6){
 				subOption = 1;
			printf("subOption is set to %d\n",option);
 			}else if(pinNum==7){
 				subOption = 2;
			printf("subOption is set to %d\n",option);
 			}else if(pinNum==8){
 				subOption = 3;
			printf("subOption is set to %d\n",option);
 			}else if(pinNum==10){
 				subOption = 4;
			printf("subOption is set to %d\n",option);
 			}
 		}
 	}
 	if(portNum==0)
 	{
 		switchStatus = (LPC_GPIO0->FIOPIN>>pinNum) & 0x01 ;
 		if(switchStatus==0)
 				{

 					printf("port0.%d has been pressed\n",pinNum);
 				}
 	}
 }
 void drawFastVLine(int16_t x, int16_t y,
  int16_t h, uint32_t color) {
   // Update in subclasses if desired!
	 drawLineforchar(x, y, x, y+h-1, color);
 }
 void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
  drawFastVLine(i, y, h, color);
  }
  }
 void drawChar(int16_t x, int16_t y, unsigned char c,
  uint32_t color, uint32_t bg, uint8_t size) {

   if(!gfxFont) { // 'Classic' built-in font

   /*  if((x >= _width)            || // Clip right
        (y >= _height)           || // Clip bottom
        ((x + 6 * size - 1) < 0) || // Clip left
        ((y + 8 * size - 1) < 0))   // Clip top
       return;*/

     if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

     for(int8_t i=0; i<6; i++ ) {
       uint8_t line;
       if(i < 5) line = pgm_read_byte(font+(c*5)+i);
       else      line = 0x0;
       for(int8_t j=0; j<8; j++, line >>= 1) {
         if(line & 0x1) {
           if(size == 1) drawPixel(x+i, y+j, color);
           else          fillRect(x+(i*size), y+(j*size), size, size, color);//fillRect(x+(i*size), y-(j*size), size, size, color);
         } else if(bg != color) {
           if(size == 1) drawPixel(x+i, y+j, bg);
           else          fillRect(x+i*size, y+j*size, size, size, bg);//fillRect(x+(i*size, y-j*size, size, size, bg)
         }
       }
     }
   }
 }

  void write(uint8_t c, uint32_t tcolor, uint32_t bgcolor)
  {
  	if (c == '\n') {
  		cursor_y += textsize*8;
  		cursor_x = 0;
  	} else if (c == '\r') {
  		// skip them
  	} else {
  		drawChar(cursor_x, cursor_y, c, tcolor, bgcolor, textsize);    //these colors change text color
  		cursor_x += textsize*6;
  		if (wrap && (cursor_x > (_width - textsize*6))) {
  			cursor_y += textsize*8;
  			cursor_x = 0;
  		}
  	}

  }

  void writestring(char *str, int x, int y, uint32_t tcolor, uint32_t bgcolor, float tsize)
  {
  	char c;
  	cursor_x = x;
  	cursor_y = y;
  	textcolor = tcolor;
  	textbgcolor = bgcolor;
  	textsize = tsize;
  	while(*str != NULL)
  	{
  		c = *str++;
  		write(c,tcolor,bgcolor);
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
    lcddelay(10);
    fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
	//display the options for the user
    char* optionname21 = "WElCOME TO SJSU CMPE\n";
    char* optionname22 = "   240 LAB4 DEMO    \n";
    char* optionname23 = "         BY         \n";
    char* optionname24 = "   SUSHMA NAGARAJ   \n";
    writestring(optionname21,0,10,WHITE,BLACK,2);
    writestring(optionname22,30,40,WHITE,BLACK,2);
    writestring(optionname23,30,80,WHITE,BLACK,2);
    writestring(optionname24,30,120,WHITE,BLACK,2);
    lcddelay(2000);
    fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
    char* optionname1 = "1. Draw Polygons\n";
    char* optionname2 = "2. 2D Graphics\n ";
    char* optionname3 = "3. 3D Graphics\n ";
    char* optionname4 = "Exit\n ";
    writestring(optionname1,30,10,WHITE,BLACK,2);
    writestring(optionname2,30,60,WHITE,BLACK,2);
    writestring(optionname3,30,110,WHITE,BLACK,2);
    writestring(optionname4,30,160,WHITE,BLACK,2);
    lcddelay(1000);
	while(1)
	{
		if(option == 0){
		ReadGPIO1(2,6);
		ReadGPIO1(2,7);
		ReadGPIO1(2,8);
		ReadGPIO1(2,10);
		}
		if(option!=0){
			switch(option)
			{
				case 1:	lcddelay(100);
						fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
						writestring(suboptionname1,30,10,WHITE,BLACK,2);
						writestring(suboptionname2,30,60,WHITE,BLACK,2);
						writestring(suboptionname3,30,110,WHITE,BLACK,2);
						writestring(suboptionname4,30,160,WHITE,BLACK,2);
						while(1){
							if(subOption==0){
								ReadGPIO2(2,6);
								ReadGPIO2(2,7);
								ReadGPIO2(2,8);
								ReadGPIO2(2,10);
						      }
							if(subOption!=0){
								switch(subOption)
								{
									case 1:printf("11");
										   lcddelay(10);
										   fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
									       randomsquare();
									       subOption=0;
									       break;
									case 2:printf("12");
										   lcddelay(10);
									       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
										   randomtriangle();
										   subOption=0;
										   break;
									case 3:printf("13");
										   lcddelay(10);
									       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
									       RandomCubes();
										   subOption=0;
										   break;
									case 4:printf("14");
										   lcddelay(10);
										   fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
										   writestring(optionname1,30,10,WHITE,BLACK,2);
										   writestring(optionname2,30,60,WHITE,BLACK,2);
										   writestring(optionname3,30,110,WHITE,BLACK,2);
										   writestring(optionname4,30,160,WHITE,BLACK,2);
										   subOption=0;
										   option=0;
										   break;
							     }
								if(subOption==0)break;
							}
							}
							option=0;
							break;
				case 2: lcddelay(10);
						fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
						writestring(suboptionname5,30,10,WHITE,BLACK,2);
						writestring(suboptionname6,30,60,WHITE,BLACK,2);
						writestring(suboptionname7,30,110,WHITE,BLACK,2);
						writestring(suboptionname8,30,160,WHITE,BLACK,2);
						while(1){
							if(subOption==0){
							ReadGPIO2(2,6);
							ReadGPIO2(2,7);
							ReadGPIO2(2,8);
							ReadGPIO2(2,10);
							}
							if(subOption!=0){
								switch(subOption)
								{
									case 1:printf("21");
										   lcddelay(10);
									       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
									       forest();
										   subOption=0;
										   break;
									case 2:printf("22");
										   lcddelay(10);
									       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
									       cubewithcolorfill();
										   subOption=0;
										   break;
									case 3:printf("23");
										   lcddelay(10);
									       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
										   cubeswithcolorfill();
										   subOption=0;
										   break;
									case 4:printf("24");
									       lcddelay(10);
										   fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
										   writestring(optionname1,30,10,WHITE,BLACK,2);
										   writestring(optionname2,30,60,WHITE,BLACK,2);
										   writestring(optionname3,30,110,WHITE,BLACK,2);
										   writestring(optionname4,30,160,WHITE,BLACK,2);
										   subOption=0;
										   option=0;
										   break;
							     }
								if(subOption==0)break;
							}
							}
							option=0;
							break;
				case 3:	lcddelay(10);
						fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
						writestring(suboptionname9,30,10,WHITE,BLACK,2);
						writestring(suboptionname10,30,60,WHITE,BLACK,2);
						writestring(suboptionname11,30,110,WHITE,BLACK,2);
						writestring(suboptionname12,30,160,WHITE,BLACK,2);
						while(1){
							if(subOption==0){
								ReadGPIO2(2,6);
								ReadGPIO2(2,7);
								ReadGPIO2(2,8);
								ReadGPIO2(2,10);
							}
							if(subOption!=0){
							switch(subOption)
							{
								case 1:printf("31");
									   lcddelay(10);
								       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
								       Drawcubewithdec();
									   subOption=0;
									   break;
								case 2:printf("32");
									   lcddelay(10);
								       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
								       Drawcubeswithdec();
								       subOption=0;
									   break;
								case 3:printf("33");
									   lcddelay(10);
								       fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
								       cubeWithGraphics();
									   subOption=0;
									   break;
								case 4:printf("34");
									   lcddelay(10);
									   fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
									   writestring(optionname1,30,10,WHITE,BLACK,2);
									   writestring(optionname2,30,60,WHITE,BLACK,2);
									   writestring(optionname3,30,110,WHITE,BLACK,2);
									   writestring(optionname4,30,160,WHITE,BLACK,2);
									   subOption=0;
									   option=0;
									   break;
							}
							if(subOption==0)break;
						}
						}
						option=0;
						break;
				case 4:	   lcddelay(10);
						   fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
						   writestring(optionname1,30,10,WHITE,BLACK,2);
						   writestring(optionname2,30,60,WHITE,BLACK,2);
						   writestring(optionname3,30,110,WHITE,BLACK,2);
						   writestring(optionname4,30,160,WHITE,BLACK,2);
						   subOption=0;
						   option=0;
						   break;
			}
		}
	}
  return 0;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
