//C program to calculate and display the distance
#include <lpc17xx.h>
#include <stdio.h>
#include "doppler_lib.h"

#define TRIG (1<<23) //P0.23
#define ECHO (1<<24) //P0.24
 
// Stores Hex values of each digit from 0 to F
unsigned char seven_seg[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0X5E,0X79,0X71};
//To activate a particular seven segment display using decoder
unsigned int dig_sel[4] = {0<<23, 1<<23, 2<<23, 3<<23}; 
// will store the digit to be displayed
unsigned char digits[] = {0,0,0,0};
unsigned int i,x;
int echoTime = 0;
float distance = 0;
unsigned long r1;
 
void timer_init(void);
void display(void);
 
int main(void)
{
  SystemInit(); //Gets called by Startup code, sets CCLK=100Mhz, PCLK=25Mhz
  SystemCoreClockUpdate();

  initTimer0(); //Init Timer for delay functions

  LPC_GPIO0->FIODIR=0xFF<<4; // set the P0.4 - P0.11 as output
  LPC_GPIO1->FIODIR=0xF<<23; // set P1.23 - P1.26 as output

  LPC_GPIO0->FIODIR |= (TRIG);    //Set P0.23(TRIG) as output
  LPC_GPIO0->FIODIR &= ~(1<<24);  //Set P0.24(ECHO) as input (explicitly)
  LPC_GPIO0->FIOCLR |= (TRIG);   	//Set P0.23 LOW initially
	

  distance=0;
  while(1)
  {
    echoTime = 0; //Resetting echo time
    distance = 0; //Resetting total distance
    x = 100000;   //Resetting delay variable
    
    LPC_GPIO0->FIOPIN |= TRIG;
    delayUS(10); //Output 10us HIGH on TRIG pin
    LPC_GPIO0->FIOCLR |= TRIG;

    while(!(LPC_GPIO0->FIOPIN & ECHO)); //Wait for a HIGH on ECHO pin

    startTimer0(); //Start counting

    while(LPC_GPIO0->FIOPIN & ECHO);    //Wait for a LOW on ECHO pin
  
    echoTime = stopTimer0(); //Stop counting and save value(us) in echoTime
  
    distance = (0.0343 * echoTime)/2; //Find the distance
    distance *= 100;  //Converting to Centimeters
    
    for(i = 3;  ;i--)
    {
      digits[i] = (int)distance%10;
      distance/=10;
      if(i==0)
        break;
    }
    display();
    while(x > 0)
      x--;
  }
}
 
 
void display(void)
{
  int x=0, y = 3,i;
  //Display 4 segments values one by one
  for(x=3,y=0;y<=3;x--, y++)
  {
    LPC_GPIO1->FIOPIN=dig_sel[x]; //enable the decoder lines according to the x value
    r1=(seven_seg[digits[y]]); //for other segments get the 7 segment values of the digits from seven_seg[]
    LPC_GPIO0->FIOPIN=r1<<4; //Put the 7 segment value into data lines(P0.4 to P0.11)
    for(i=0;i<100000;i++); //Wait for some time (small delay)
    LPC_GPIO0->FIOPIN=00<<4; //clear the data lines
  }
}
 
void timer_init()
{
  LPC_TIM0->CTCR=0X00; //timer mode
  LPC_TIM0->TCR=0X02; //reset TC and PC
  LPC_TIM0->MCR=0X02; //reset the TC and PC on match
  LPC_TIM0->PR=0X02; //TC will increment for every 3 PCLK
  LPC_TIM0->MR0=2999999; //calculated using formula "MR=(PCLK*DELAY)/PR+1" where the delay is 3s
  LPC_TIM0->EMR=0X011; //initially EMC0 is HIGH when there is a match it is configured to become LOW
  LPC_TIM0->TCR=0X01; //start the timer
}
