#include<stdio.h>

#include<stdlib.h>

#include<string.h>

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include "GHmatrix.h"

#include <cr_section_macros.h>

#define SBIT_CNTEN   0

#define LED1 0

/*-------All global declaration start---------*/

char syncField[33] = {0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,

  0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf};

char transmitData[22] = {'S','J','S','U','C','M','P','E','2','4','5','s','u','s','h','m','a','9','8','7','1','\0'};

char transmission[1024];

//char n[1024];
char n[2000];

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

int decimalVal3 =7;
int j;

char z[168];

char result1[32] ={0};

int pointerVal4 =0;

char s[152];

char x[236];

char scramblingorder[2];

int m=0;

int decimalVal5=7;

int decimalVal6=7;

int pointerVal22=0;

char ck[1][12];

char sk[1][4];

char decode[152];

int pointval11=0;

int errorFlag =0;

/*-------All global declaration end---------*/

/*-----Timer configuration start---------*/
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
/*-----Timer configuration end---------*/

/*-----Scrambling the data start---------*/
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
/*-----Scrambling the data end---------*/

/*-----Descrambling the data start---------*/
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

/*-----Descrambling the data end---------*/

/*-----Decoding the data start---------*/
char * decoding(char * decodingData)
{
	int index=0;
	for(int i=0;i<8;i++){
		decode[index]=decodingData[i];
		index++;
	}
	//printf("%d\n",index);
	for(int k=1;k<19;k++){
		for(int i = pointval11+12;i<=pointval11+12+7;i++){
			decode[index]=decodingData[i];
			index++;
		}
	pointval11=pointval11+12;
	}
	/*printf("decoded data\n");
	for(int j=0;j<152;j++){
		printf("%d",decode[j]);
	}
	printf("\n");*/
	return decode;
}
/*-----Decoding the data end---------*/

/*-----error detection in the data start---------*/
int errorDetection(char * decodingData)
{
	/*printf("data inside for error detection\n");
    for(int i=0;i<228;i++)
    {
		printf("%d",decodingData[i]);
    }*/
    int index =0;
    int pointerval9=0;
    int count =1;
    int i=0;
    int j=0;
    int errorbitcount=0;
    char errordecodingarray[228] ={0};//12*19
    while(count>0){
    	for(int i=0; i<1; ++i)
        for(int j=0; j<4; ++j)
        {
            sk[i][j] = 0;
        }
        for(j=0; j<4;j++){
            for(int k=0; k<12;k++)
            {
                sk[i][j]^=decodingData[k]*transposeH_Matrix[k][j];
            }
            errordecodingarray[index]= sk[i][j];
            if(errordecodingarray[index]==1){
               	errorbitcount++;
            }
            ++index;
        }
        count--;
    }
    /*printf("error decoding array and count of bits of error\n");
    printf("%d\n",errorbitcount);
    for(int i=0;i<4;i++)
    printf("%d",errordecodingarray[i]);
    printf("\n");*/
    return errorbitcount;
}
/*-----error detection in the data end---------*/

/*-----transmission of the data start---------*/
void transmissionOfData(void)
{
        printf("\nPayload being transmitted %s\n",transmitData);
        int  number;
		char  * scrambled_transmitData;
		int numberOfErrorBits = 0;
		char errorMatrix[1][12];
		int index =0;
		int pointerval9=0;
		int count =21;
		int i=0;
		int j=0;
		int index2=0;
		int pointval12=0;
        int m=0;
		char encoding[252] ={0};//12*21
		char num[3];
		for(int i=0;i<1;i++){
			for(int j=0;j<12;j++){
			errorMatrix[i][j]=0;
			}
		}
		/*printf("error matrix before\n");
		for(int j=0;j<1;j++){
			for(int k=0;k<12;k++){
				printf("%d ",errorMatrix[j][k]);
			}
			printf("\n");
		}*/
	    printf("Enter the number of error bits to be inserted which can be either 1 or 2\n");
		scanf("%d", &numberOfErrorBits);
		for(int i=0;i<numberOfErrorBits;i++){
			int errorBitLocation = (rand()%12);
			printf("%d\n",errorBitLocation);
			errorMatrix[0][errorBitLocation]=errorMatrix[0][errorBitLocation]^0x01;
		}
		/*printf("error matrix after\n");
		for(int j=0;j<1;j++){
			for(int k=0;k<12;k++){
				printf("%d ",errorMatrix[j][k]);
		}
		printf("\n");
		}*/
	    printf("Type in a number for scrambling order\n");
        scanf("%d", &number);
		/*printf("before scrambling the data\n");
	    for(int i =0;i<21;i++){
			for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
			{
				int bit1= (transmitData[i]>>j)&0x01;
				printf("%d",bit1);
			}
	    }
		printf("\n");*/
        scrambled_transmitData=scrambling(transmitData,number);
		/*printf("after scrambling the data\n");
		for(int i=0;i<168;i++){
				printf("%d",scrambled_transmitData[i]);
		}
		printf("\n");*/
		while(count>=0){
    		for(int i=0; i<1; ++i)
				for(int j=0; j<12; ++j)
				{
					ck[i][j] = 0;
				}
			for(j=0; j<12;j++){
				for(int k=0; k<8;k++)
				{
            		if(index<12){
						ck[i][j]^=scrambled_transmitData[k]*generatorG_Matrix[k][j];
					}else{
                		if(index>=12 && index <=23)
                		pointerval9=0;
						ck[i][j]^=scrambled_transmitData[pointerval9+8+k]*generatorG_Matrix[k][j];
					}
				}
					encoding[index]= ck[i][j];
					++index;
				}
			pointerval9 = pointerval9+8;
			count--;
		}
        /*printf("before error matrix exoring with encoded data\n");
        for(int i=0;i<252;i++)
        printf("%d",encoding[i]);
        printf("\n");*/
        if(numberOfErrorBits==0){
        	for(int i=0;i<252;i++)
        	encoding[i] = encoding[i];
        }else if(numberOfErrorBits==1 || numberOfErrorBits==2){
        	for(int i=0;i<21;i++){
        		for(int j=pointval12;j<pointval12+12;j++){
        			encoding[index2]=encoding[j]^errorMatrix[0][m];
        			index2++;
        			m++;
        		}
        		m=0;
        		pointval12=pointval12+12;
        		}
			}
		/*printf("after error matrix exoring with encoded data\n");
        for(int i=0;i<252;i++)
        printf("%d",encoding[i]);
        printf("\n");
*/
		for(int o = 0; o < 32; o++)
		{
			for(int h = pointerVal22; h <= pointerVal22+8 && h<252; h++)
			{
				result1[o] |= encoding[h] << decimalVal3;
				decimalVal3--;
			}
			decimalVal3 = 7;
			pointerVal22 += 8;
		}
		/*printf("after scrambling and encoding the data\n");
		for(int i =0;i<32;i++){
			for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
			{
				int bit1= (result1[i]>>j)&0x01;
				printf("%d",bit1);
			}
		}
		printf("\n");*/
		strcpy(transmission,syncField);
		if(errorFlag==0){
			sprintf(num,"%d",number); // put the int into a string55
			strcat(transmission, num);
		}else if(errorFlag==1){
			number = 6;
			sprintf(num,"%d",number);
			strcat(transmission, num);
		}
		strcat(transmission,result1);
		/*for(int i =0;i<65;i++){//32+1+21+10 extra
			for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
			{
				int bit1= (transmission[i]>>j)&0x01;
				printf("%d",bit1);
			}
		}*/
		/*printf("data transmitting started\n");
		for(int i=0;i<500;i++){
			delay();
		}*/
        printf("START\n");
        for(int i = 0; i < 1024; i++) // This will happen for the 64 elements in sync field
        {
            for(int j = 7; j >=0 ; j--) // This will happen for the 8 bytes in 1 element
            {

                //LPC_GPIO0->FIOCLR = (1 << 3);
                if((transmission[i]>>j)&0x01)
                { // For 7 the MSB i.e. the leftmost bit will come to right and will be & with 1. If the bit is 1 value is 1 otherwise if
                        //the bir is 0 value is 0. so set the gpio pin accordingly.
                    LPC_GPIO0->FIOSET = (1 << 3);
                	//LPC_GPIO0->FIOCLR = (1 << 3);
                }
                else
                {
                    LPC_GPIO0->FIOCLR = (1 << 3);
                }
                    delay();
            }
        }
}
/*-----transmission of the data end---------*/

/*-----reception of the data start---------*/
void receiveAckinResponse(void){
        printf("receiving started\n");
        LPC_GPIO0->FIOCLR = (1 << 3);
       // for(int i = 0; i < 1024; i++)
        for(int i = 0; i < 2000; i++)
        {
            n[i] = ((LPC_GPIO0->FIOPIN&(1<<2))>> 2) ;
            delay();
        }
        // just for printing
        //for(int c = 0; c < 1024; c++)
        /*for(int c = 0; c < 2000; c++)
        {
            printf("%d", n[c]);
        }*/
      do
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
        for(int i=pointerVal+16;i<pointerVal+244;i++)
        //for(int i=pointerVal+16;i<pointerVal+168;i++)
        {
            x[m]=n[i];
            ++m;
        }
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
		/*printf("data sent for decoding\n");
		for(int i=0;i<228;i++)//12*19
		{
			printf("%d",x[i]);
		}
		printf("\n");*/
		char  * descrambled_transmitData;
		char * decoded_transmitData;
		int countflag=0;
		if(number1!=6){
			countflag=errorDetection(x);
			if(countflag==0){
				errorFlag=0;
    			//printf("no error soo decoding the data\n");
				decoded_transmitData=decoding(x);
				//printf("\n");
				descrambled_transmitData=descrambling(decoded_transmitData,number1);
				//printf("\n");
				/*printf("descrambled data:\n");
	 			for(int i=0;i<152;i++){ //19*8
					printf("%d",descrambled_transmitData[i]);
				}
*/				printf("\n");
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
*/			for(int u = 0; u < 19; u++)
			{
				printf("%c", ackdata[u]);
			}
			}else{
    	//retransmit
				errorFlag=1;
    			printf("retransmit the data");
			}
		}else if(number1==6){
			printf("start new transmission");
		}
}
/*-----reception of the data end---------*/

/*-----main start---------*/
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

  for(int i=0; i<1; ++i)
	for(int j=0; j<12; ++j)
	{
		ck[i][j] = 0;
	}
  LPC_GPIO0->FIOCLR = (1 << 3);
  //transmissionOfData();
  receiveAckinResponse();
  for(int i=0;i<2000;i++){
      delay();
  }
  transmissionOfData();
}
/*-----main end---------*/
