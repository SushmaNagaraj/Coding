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
	int x;
	int y;
}p0;

struct cameraPoints{
	int xe,ye,ze;
}pCam;

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
  	 fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, WHITE);
  	 lcddelay(200);
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
 	//printf("%d\n",xEnd);
 	for (x = 0; x <= xEnd; x++) {
 		y = (sqrt(radius * radius - x * x));
 		//printf("%d \n",y);
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

 struct worldPoints world_3dCordinateSystem(int x, int y, int z)
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

 void drawOutlineOfCube(int size)
 {
 	struct worldPoints p1,p2,p3,p4;
 	struct worldPoints p5,p6,p7,p8;

 	p1=world_3dCordinateSystem(0,0,size);
 	p2=world_3dCordinateSystem(size,0,size);
 	p3=world_3dCordinateSystem(size,size,size);
 	p4=world_3dCordinateSystem(0,size,size);

 	drawLine(p1.x,p1.y,p2.x,p2.y,YELLOW);
 	drawLine(p2.x,p2.y,p3.x,p3.y,YELLOW);
 	drawLine(p3.x,p3.y,p4.x,p4.y,YELLOW);
 	drawLine(p4.x,p4.y,p1.x,p1.y,YELLOW);


 	p5=world_3dCordinateSystem(0,0,0);
 	p6=world_3dCordinateSystem(size,0,0);
 	p7=world_3dCordinateSystem(size,size,0);
 	p8=world_3dCordinateSystem(0,size,0);

 	drawLine(p2.x,p2.y,p6.x,p6.y,YELLOW);
 	drawLine(p3.x,p3.y,p7.x,p7.y,YELLOW);
 	drawLine(p4.x,p4.y,p8.x,p8.y,YELLOW);
 	drawLine(p6.x,p6.y,p7.x,p7.y,YELLOW);
 	drawLine(p7.x,p7.y,p8.x,p8.y,YELLOW);
 }

 void fill3DCube(int size)
 {
 	struct worldPoints p1;

 	int i,j;

 	for(i=0;i<size;i++)
 	{
 		for(j=0;j<size;j++)
 		{
 				p1=world_3dCordinateSystem(j,i,size);	//top fill
 				drawPixel(p1.x,p1.y,RED);

 				p1=world_3dCordinateSystem(i,size,j);	// right fill
 				drawPixel(p1.x,p1.y,DKBLUE);

 				p1=world_3dCordinateSystem(size,j,i);	// left fill
 				drawPixel(p1.x,p1.y,YELLOW);
 		}
 	}
 }

 void squareSaver_insidecube(int size){

   	int dx=0,dy=0,dz=0;

   	int g=0;
   	while(g!=3){

   		dx=rand()%(size-80);
   		dy=rand()%(size-80);
   		dz=40+rand()%80;

   		switch(g){
   			case 0:dx=5;
			   	   dy=12;
			   	   dz=50;
			   	   break;
   			case 1:dx=25;
			   	   dy=8;
			   	   dz=40;
			   	   break;
   			case 2:dx=40;
			   	   dy=40;
			   	   dz=25;
			   	   break;
   		}

   	  	float x0=dx,y0=dy;
   	  	float x1=x0,y1=y0+dz;
   	  	float x2=x0+dz,y2=y0+dz;
   	  	float x3=x0+dz,y3=y0;

   	  	struct worldPoints p00,p1,p2,p3;

   		p00=world_3dCordinateSystem(size,x0,y0);
   		p1=world_3dCordinateSystem(size,x1,y1);
   		p2=world_3dCordinateSystem(size,x2,y2);
   		p3=world_3dCordinateSystem(size,x3,y3);

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

 void treeSaver_insidecube(double x0, double y0, double x1, double y1, int size)
 {
   	struct worldPoints p1,p2;
 	p1=world_3dCordinateSystem(x0,size,y0);
 	p2=world_3dCordinateSystem(x1,size,y1);
 	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

 	p1=world_3dCordinateSystem(x0+1,size,y0+1);
 	p2=world_3dCordinateSystem(x1+1,size,y1+1);
 	drawLine(p1.x,p1.y,p2.x,p2.y,ORANGE);

 	p1=world_3dCordinateSystem(x0-1,size,y0-1);
 	p2=world_3dCordinateSystem(x1-1,size,y1-1);
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
 		p1=world_3dCordinateSystem(x1,size,y1);
 		p2=world_3dCordinateSystem(xn[i+1],size,yn[i+1]);
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

 void drawInitialinsidecube(int size)
 {
 	struct worldPoints p1;

 	int i,j;
 	int colormap[size][size];

 	for(i=0;i<size;i++)
 	{
 		for(j=0;j<size;j++)
 		{
 			if(i>=5 && i<=15 && j>=10 && j<=size-10)
 				colormap[i][j]=1;
 			else if(i>=15  && i<=25 && j>=5 && j<=25)
 				colormap[i][j]=1;
 			else if(i>=25 && i<=35 && j>=10 && j<=size-10)
 				colormap[i][j]=1;
 			else if(i>=35 && i<=45 && j>=size-20 && j<=size-10)
 				colormap[i][j]=1;
 			else if(i>=45 && i<=55 && j>=10 && j<=size-10)
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
 				p1=world_3dCordinateSystem(j,i,size);
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

 void cubeWithGraphics(){
	    pCam.xe=110;
	  	pCam.ye=110;
	  	pCam.ze=110;

	  	int cube_size=80;

 		drawRect(0,0,240,320,YELLOW);
		p0=world_3dCordinateSystem(0,0,0);
		draw_3dCordinateSystem();

		drawOutlineOfCube(cube_size);

		fill3DCube(cube_size);

		squareSaver_insidecube(cube_size);

	  	treeSaver_insidecube(35,3,35,22,cube_size);
	  	treeSaver_insidecube(55,3,55,22,cube_size);
	  	treeSaver_insidecube(15,3,15,22,cube_size);

	  	drawInitialinsidecube(cube_size);
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
  	  // for lab1
  	  /*randomsquare();
  	  randomtriangle();*/

  	  // for lab2
	  forest();
	  printf("forest is drawn");
	  lcddelay(10);
  	  while(1){
  		  char ch =getchar();
  		  printf("Character entered to start displaying the 3D cube:%c",ch);
  		  if(ch!='\0'){
  		  	  fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);
  		  	  cubeWithGraphics();
  		  }
  	  }
      lcddelay(1000);
  	  //calling function to draw and display triangle on LCD
      //fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, WHITE);
  return 0;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
