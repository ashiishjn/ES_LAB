#include <lpc17xx.h>
#include "doppler_lib.h"
#include <stdio.h>
 
void initTimer0(void) //PCLK must be = 25Mhz!
{
    LPC_TIM0->CTCR = 0x0; // To enable timer mode
    LPC_TIM0->PR = PRESCALE; //Increment TC at every 24999+1 clock cycles
    LPC_TIM0->TCR = 0x02; //Reset Timer
}
 
void startTimer0(void)
{
    LPC_TIM0->TCR = 0x02; //Reset Timer
    LPC_TIM0->TCR = 0x01; //Enable timer
}
 
unsigned int stopTimer0(void)
{
    LPC_TIM0->TCR = 0x00; //Disable timer
    return LPC_TIM0->TC;
}
 
void delayUS(unsigned int microseconds) //Using Timer0
{
    LPC_TIM0->TCR = 0x02; //Reset Timer
    LPC_TIM0->TCR = 0x01; //Enable timer
    while(LPC_TIM0->TC < microseconds); //wait until timer counter reaches the desired delay
    LPC_TIM0->TCR = 0x00; //Disable timer
}
 
void delayMS(unsigned int milliseconds) //Using Timer0
{
    delayUS(milliseconds * 1000);
}
