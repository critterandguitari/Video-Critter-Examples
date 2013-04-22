/******************************************************************************/
/*  video.c                                                        */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  simple color raster graphics using LPC2138, HC04 delays, and 4051 MUXs */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

#include "video.h"            
#include "LPC21xx.h"                // register definitions for processor
#include "system.h"                 // system level stuff, interrupt stuff


#define SYNC_RATE 3631    

#define ZERO            0x150;
#define BLACK           0x160;
#define COLORBURST      0x190;
        
#define DELAY1	{asm volatile("nop");}
#define DELAY2 	{asm volatile("nop");asm volatile("nop");}
#define DELAY4	{asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");}
#define DELAY8	{asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");}
#define DELAY16 {asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");}
#define DELAY32 {asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");asm volatile("nop");}
        
#define DELAY256 {DELAY32;DELAY32;DELAY32;DELAY32;DELAY32;DELAY32;DELAY32;DELAY32;}     
        
        
        

int lineCount = 0;                      // keeps track of the line number
unsigned int imageVindex = 0;           // counts vertical lines in graphic array
unsigned int imageArray[16][128];       // array to store graphics
static int pixelCount = 0;              // keeps track of the pixel count on each line
int syncOn = ZERO;                      // stores sync states
int syncOff = BLACK;       


static void timer0ISR(void) __attribute__ ((interrupt ("IRQ")));  // interrupts, we use a IRQ for main sync, then 
void fiq_handler(void) __attribute__((interrupt("FIQ")));          // FIQ for raster 


void video_init(void){
    IODIR1 |= (1 << 26);
    PINSEL0 |= (1 << 15);                            // PWM2
    PINSEL1 |= ((1 << 12) | (1 << 13));              // p0.22 is output match 0 timer 0
    PINSEL1 |= (1 << 10);                            // pwm 5
    PINSEL1 |= ((1 << 22) | (1 << 23) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27));    // match timer0. 1, 2,3
    PWMPR = 0x00000000;                             // Set Pre Scale to 0
    PWMPCR = (1 << 10) | (1 << 13);                 //0x1000; // Enable PWM4 Output (bit12)
    PWMMR0 = 16;                                    // Max Value of Pulse Width
    PWMMR2 = 8;                                     // PWM4 Match Value
    PWMMR5 = 8;                                     // PWM5 Match Value
    PWMMCR = 0x00000002;                            // Reset PWMMTC when Matched with PWMMR0
    PWMLER = 0x7F;                                  // Latch in all PWMMMR's
    PWMTCR = 0x02;                                  // Reset PWMMTC, (bit1)
    PWMTCR = 0x09;                                  // Counter Enable, PWM Mode Enabled(bit0, bit3) */
    
    IODIR0 |= ((1 << 4) | (1 << 5) | (1 << 6) );    // outs for multiplexer
    
    IODIR0 |= (1 << 23);
    PINSEL1 |= 0x00080000;                          // enable dac
    VICIntSelect &= ~0x10;                          // timer 0 is IRQ
    VICIntEnable = 0x10;                            // enable timer 0 interrupt
    VICVectCntl0 = 0x24;                            //  use slot 0 for timer 0 interrupt 
    VICVectAddr0 = (unsigned int)timer0ISR;         // address of ISR for slot 0
    T0TCR = 0;                                      // reset timer
    T0PR = 0;                                       // timer prescale
    T0MR0 = SYNC_RATE;                              // match at sampling rate
    T0MR1 = SYNC_RATE;
    T0MR2 = SYNC_RATE;
    T0MR3 = SYNC_RATE;
    
    T0EMR &= ~0xff0;
    T0EMR |= ZERO;                                  // produce 0 volts at first match
    
    T0MCR = 3;                                      // interrupt and reset on match
    
    T0TCR = 1;                                      // start timer
    enableIRQ();                                    // enable interrupt
}

void video_stop(void){
    T0TCR = 0;                                      // stop timer
    disableIRQ();                                   // interrupts off
}


static void timer0ISR(void)
{ 


    T0MCR = 0;                          // don't interrupt or reset on match
    
    T0MR0 = 256;                        // match at next event
    T0MR1 = 256;
    T0MR2 = 256;
    T0MR3 = 256;   
    T0EMR &= ~0xff0;
    T0EMR |= syncOff;
    while (T0TC < 257);

    // if its a viewable line, put the processor to sleep, and set the FIQ to wake it up
    // this eliminates horizontal jitter.
    if ((lineCount > 25) && (lineCount < 225)){
        T0EMR = 2;                  // force color burst
        T0MR0 = 550;                // back to black at next event
        T0MR1 = 550;
         T0MR2 = 550;
        T0MR3 = 550;   
        T0EMR &= ~0xff0;
        T0EMR |= BLACK;
        while (T0TC < 551);

        // setup FIQ interrupt
        T0MR0 = 700;
        T0IR = 0xFF;                // clear interrupts
        VICIntSelect = 0x10;        // timer 0 is FIQ
        T0MCR = 1;                  // interrupt but don't reset on match
        PCON = 1;                   // stop cpu              
    }
    
    else if (!((lineCount >= 246) && (lineCount <= 253))){
        T0EMR = 2;                  //force color burst
        T0MR0 = 550;                // black at next event
        T0MR1 = 550;
        T0MR2 = 550;
        T0MR3 = 550;   
        T0EMR &= ~0xff0;
        T0EMR |= BLACK;
        while (T0TC < 551);
    }
    
    T0MR0 = SYNC_RATE;                // match at sampling rate
    T0MR1 = SYNC_RATE;
    T0MR2 = SYNC_RATE;
    T0MR3 = SYNC_RATE;

    lineCount++;
    imageVindex = lineCount - 25;     // imageVindex is our line in the graphics ram
    imageVindex >>= 1;                // it is 2 lines on the TV
    
    if ((lineCount >= 248) && (lineCount < 251)){  // invert sync for vertical
        
        syncOn = BLACK;
        syncOff = ZERO;
    }
    else {
        syncOn = ZERO;
        syncOff = BLACK;
    }
    
    T0EMR &= ~0xff0;
    T0EMR |= syncOn;                    // next sync

    if (lineCount == (264)){
        lineCount = 1;
    }

    T0MCR = 3;                          // interrupt and reset on match
    
    T0IR = 0xFF;                        // clear interrupts
    VICVectAddr = 0;                    // update vic priorities
}


void fiq_handler(void){

    T0MCR = 0;                          // don't interrupt or reset on match

    // put picture on screen
    for (pixelCount = 0; pixelCount < 16; pixelCount++){
            T0EMR = (imageArray[pixelCount][imageVindex] >> 0);DELAY4; DELAY4;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 4);DELAY4; DELAY2;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 8);DELAY4; DELAY4;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 12);DELAY4;DELAY2;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 16);DELAY4;DELAY4;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 20);DELAY4;DELAY2;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 24);DELAY4;DELAY4;
            T0EMR = (imageArray[pixelCount][imageVindex] >> 28);DELAY4;
    }
    
    T0EMR = 1;                          // black
    VICIntSelect &= ~0x10;              // timer 0 is IRQ
    T0IR = 0xFF;                        // clear interrupts
}
