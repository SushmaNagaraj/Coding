#include<stdio.h>

#include<stdlib.h>

#include<string.h>

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

#define SBIT_CNTEN   0

#define LED1 0

char syncField[33] = {0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,

  0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf};

char transmitData[11] = {'s','u','s','h','m','a','9','8','7','1','\0'};

char transmission[1024];

char n[1024];

int matchcount;

int k;

int decimalVal1 = 7;

int syncbitVal;

int bitMatchPosition;

char syncstore;

int pointerVal;

char result[15];

int y;

int decimalVal2 = 7;

int decimalVal3;

void timer_configure(void)

{            /*

              * for 1 sec delay MR0= 250000 and P

              *

              * */

              LPC_SC->PCONP |= (1 << 1); // power up timer0

              LPC_SC->PCLKSEL0 &= ~(3 << 2); // clock for timer0 = CCLK/4 = 25MHz

              LPC_TIM0->TCR = 2; // reset & hold timer

              LPC_TIM0->PR = 99;

              LPC_TIM0->MR0 =2000; // timer0 irq @ 1kHz//for PR=99 value for 1 milisec =250

              LPC_TIM0->MCR |= 1 << 0; // interrupt on Match0 compare

              LPC_TIM0->MCR |= 1 << 1; // reset timer on Match 0

}

void delay()

{

              LPC_TIM0->TCR = 1;

              while((LPC_TIM0->IR & 1) == 0);

              LPC_TIM0->IR |= 1;

              LPC_TIM0->TCR = 2;

}
void transmissionOfData(void)
{
		printf("Payload being transmitted %s\n",transmitData);
		strcpy(transmission,syncField);
		strcat(transmission,transmitData);
		for(int i = 0; i < 1024; i++) // This will happen for the 64 elements in sync field
		{
			for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
			{
				if((transmission[i]>>j)&0x01)
				{ // For 7 the MSB i.e. the leftmost bit will come to right and will be & with 1. If the bit is 1 value is 1 otherwise if
						//the bir is 0 value is 0. so set the gpio pin accordingly.
					LPC_GPIO0->FIOSET = (1 << 3);
				}
				else
				{
					LPC_GPIO0->FIOCLR = (1 << 3);
				}
					delay();

			}
		}
		LPC_GPIO0->FIOCLR = (1 << 3);

}
void receiveAckinResponse(void){
		printf("receiving started\n");
		for(int i = 0; i < 1024; i++)
		{
			n[i] = ((LPC_GPIO0->FIOPIN&(1<<2))>> 2) ;
			delay();
		}

		for(int j=0;j<32;j++){
			while((matchcount<8)&&(k<1024))
			{
				syncbitVal = (syncField[j]>>decimalVal1)&01;
				if(syncbitVal == n[k]){
					bitMatchPosition = k;
					matchcount++;
					decimalVal1--;
					k++;
				}
				else
				{
					decimalVal1=7;
					bitMatchPosition = 0;
					matchcount=0;
				}
			}
		}
		if(matchcount >= 8)
		{
				printf("8 bits matched\n");

		}
		else if(k >= 1024)
		{
				printf("No match found \t k > 1024\n");
		}
		else if(matchcount == 0 ||(matchcount > 0 && matchcount <8) )
		{
				printf("No match found \t count = 0\n");

		}

		for(int z = bitMatchPosition-7; z < bitMatchPosition; z++)
		{
			syncstore |= n[z] << decimalVal2;
			decimalVal2--;
		}
		for(y = 0; y < 32; y++)
		{
			if(syncstore == syncField[y])
			{
				break;

			}
		}
		pointerVal = k + ((30-y)*8);

		for(int z = 0; z < 12; z++)
		{
			for(int h = pointerVal; h <= pointerVal+8; h++)
			{

				result[z] |= n[h] << decimalVal3;
				decimalVal3--;

			}
			decimalVal3 = 7;
			pointerVal += 8;
		}
		for(int u = 1; u < 11; u++)
		{
			printf("%c", result[u]);

		}
}
int main(void)

{

  timer_configure();

  // Pin Sel first

  LPC_PINCON->PINSEL4 &= ~(3 << 12);

  LPC_PINCON->PINSEL4 &= ~(3 << 14);

  // Selecting pull down resistor

  LPC_PINCON->PINMODE4 |= 3 << 12;

  LPC_PINCON->PINMODE4 |= 3 << 14;

  // Setting the direction of 0.22 as transmitter (output) and 0.27 as input

  LPC_GPIO0->FIODIR0 |= 1 << 3;

  LPC_GPIO0->FIODIR0 &= ~(1 << 2);

  transmissionOfData();
  receiveAckinResponse();

}
