#ifndef TEAM2_LPC176X
#define TEAM2_LPC176X
  
#define PRESCALE (25000-1)

void initTimer0(void);
void startTimer0(void);
unsigned int stopTimer0(void);
void delayUS(unsigned int microseconds);
void delayMS(unsigned int milliseconds);
 
#endif 
