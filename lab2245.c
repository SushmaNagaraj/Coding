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

char transmitData[22] = {'S','J','S','U','C','M','P','E','2','4','5','s','u','s','h','m','a','9','8','7','1','\0'};

char transmission[1024];

char n[1024];

int matchcount;

int k;

int decimalVal1 = 7;

int syncbitVal;

int bitMatchPosition;

char syncstore;

int pointerVal;

char ackdata[20]={0};

int y;

int decimalVal2 = 7;

int decimalVal3;
int j;

char z[168];

char result1[21];

int pointerVal4 =0;

char s[152];

char x[152];

char scramblingorder[2];

int m=0;

int decimalVal5=7;

int decimalVal6=7;

void timer_configure(void)
{            /*

              * for 1 sec delay MR0= 250000 and P

              *

              * */

              LPC_SC->PCONP |= (1 << 1); // power up timer0

              LPC_SC->PCLKSEL0 &= ~(3 << 2); // clock for timer0 = CCLK/4 = 25MHz

              LPC_TIM0->TCR = 2; // reset & hold timer

              LPC_TIM0->PR = 99;

              LPC_TIM0->MR0 =250; // timer0 irq @ 1kHz//for PR=99 value for 1 milisec =250

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
char * scrambling(char * transmitData,int number )
{
    char w[168];
    int k=0;
    int lenghtoftransmitData=strlen(transmitData);
    for(int i =0;i<lenghtoftransmitData;i++){
        for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
        {
            w[k]= (transmitData[i]>>j)&0x01;
            k++;
        }
    }
    for(int j = 0; j < ((number/2)+1); j++) // This will happen for the 8 bytes in 1 element
    {
        z[j]=w[j];

    }
    int b=0;
    int g = ((number/2)+1);
    for(int j = g; j < (g+(number/2)); j++) // This will happen for the 8 bytes in 1 element
    {
        z[j]=w[j]^z[b];
        ++b;
    }
    int p = 0;
    int l = number/2;
    for(int j = (g+(number/2));j<=168;j++)
    {
        z[j] = w[j]^(z[l]^z[p]);
        ++l;
        ++p;
    }
    return  z;
}
char * descrambling(char * descramleData,int number1 )
{
    /*for(int i=0;i<152;i++)
    {
    printf("%d",descramleData[i]);
    }*/
    for(int j = 0; j < ((number1/2)+1); j++) // This will happen for the 8 bytes in 1 element
    {
        s[j]=descramleData[j];

    }
    printf("\n");
    int b=0;
    int g = ((number1/2)+1);
    for(int j = g; j < (g+(number1/2)); j++) // This will happen for the 8 bytes in 1 element
    {
        s[j]=descramleData[j]^descramleData[b];
        ++b;
    }
    int p = 0;
    int l = number1/2;
    for(int j = (g+(number1/2));j<=152;j++)
    {
        s[j] = descramleData[j]^(descramleData[l]^descramleData[p]);
        ++l;
        ++p;
    }
    return  s;
}
void transmissionOfData(void)
{
        printf("\nPayload being transmitted %s\n",transmitData);
        int  number;
        printf("Type in a number for scrambling order\n");
        scanf("%d", &number);
        strcpy(transmission,syncField);
        char  * scrambled_transmitData;
        scrambled_transmitData=scrambling(transmitData,number);
        int pointerVal1 =0;
        int decimalVal4 = 7;
        for(int o = 0; o < 21; o++)
            {
                for(int h = pointerVal1; h <= pointerVal1+8; h++)
                {

                    result1[o] |= scrambled_transmitData[h] << decimalVal4;
                    decimalVal4--;

                }
                decimalVal4 = 7;
                pointerVal1 += 8;
            }
        char num[3];
        sprintf(num,"%d",number); // put the int into a string
        strcat(transmission, num); // order sending to receiver for descrambling
        strcat(transmission,result1);
        /*for(int i =0;i<53;i++){
                for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
                {
                    int bit1= (transmission[i]>>j)&0x01;
                    printf("%d",bit1);
                }
        }*/
        printf("START\n");
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
}
void receiveAckinResponse(void){
        printf("receiving started\n");
          LPC_GPIO0->FIOCLR = (1 << 3);
        for(int i = 0; i < 1024; i++)
        {
            n[i] = ((LPC_GPIO0->FIOPIN&(1<<2))>> 2) ;
            delay();
        }
        // just for printing
        /*for(int c = 0; c < 1024; c++)
        {
            printf("%d", n[c]);
        }
*/        do
        {
            syncbitVal = (syncField[j]>>decimalVal1)&01;
            if(syncbitVal == n[k]){
                bitMatchPosition = k;
                matchcount++;
                decimalVal1--;
            }else
            {
                bitMatchPosition = 0;
                syncstore =0;
                matchcount = 0;
            }
            if(decimalVal1 <= 0)
            {
                decimalVal1 = 7;
                j++;
            }
            k++;
        }while((matchcount<8)&&(k<1024));
        if(matchcount >= 8)
        {
                printf("8 bits matched\n");

        }
        else if((k >= 1024)&&(matchcount==0))
        {
                printf("No match found \t k > 1024\n");
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
        //printf("pointerVal : %d\n",pointerVal);
        for(int i=pointerVal+16;i<pointerVal+168;i++)
        //for(int i=pointerVal+16;i<pointerVal+168;i++)
        {
            x[m]=n[i];
            ++m;
        }
        printf("scrambled data :\n");
        /*for(int i=0;i<152;i++)
        {
        printf("%d",x[i]);
        }*/
        for(int h =pointerVal+8; h <pointerVal+16; h++)
        {

            scramblingorder[0] |= n[h] << decimalVal5;
            decimalVal5--;

        }
        printf("scrambled order :");
        /*for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
        {
            int bit1= (scramblingorder[0]>>j)&0x01;
            printf("%d",bit1);
        }*/
        printf("\n");
        int number1 = scramblingorder[0] - '0';
        printf("%d",number1);
        printf("\n");
        char  * descrambled_transmitData;
        descrambled_transmitData=descrambling(x,number1);
        printf("\n");
        /*for(int i=0;i<152;i++){
            printf("%d",descrambled_transmitData[i]);
        }*/

        for(int z = 0; z < 20; z++)
        {
            for(int h = pointerVal4; h < pointerVal4+8; h++)
            {

                ackdata[z] |= descrambled_transmitData[h] << decimalVal6;
                decimalVal6--;

            }
            decimalVal6 = 7;
            pointerVal4 =pointerVal4+8;
        }
       /* printf("ackdata in bits:\n");
        for(int i =0;i<20;i++){
			for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
			{
				int bit1= (ackdata[i]>>j)&0x01;
				printf("%d",bit1);
			}
         }
*/        for(int u = 0; u < 19; u++)
        {
            printf("%c", ackdata[u]);

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

 // transmissionOfData();
  LPC_GPIO0->FIOCLR = (1 << 3);
  receiveAckinResponse();
  for(int i=0;i<2000;i++){
      delay();
  }
  transmissionOfData();

}
