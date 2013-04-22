/******************************************************************************/
/*  main.c                                                        */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  demonstrates some simple graphics and sound on the TV */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

#include "LPC21xx.h"
#include "system.h"
#include "video.h"
#include "audio.h"

// some timing functions
void start_ms_timer(void);

unsigned int ms_time(void);

int main (void) {

    int i = 0;
    int j = 0;
    int k = 1;
    
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    
    int x_loc = 0;
    
    unsigned int delayTime = 0; 

    int color = 0;

    Initialize();               // Initialize the MCU

    led_board(0);
    delay_ms(100);              // flash LEDs
    led_board(1);
    delay_ms(100);
    led_board(0);
    delay_ms(100);
    led_board(1);  
    
    clear_screen(0);             // fill screen with black,  do this before calling video_init()
    
    video_init();               // initialize synthesis
    audio_init();               // init audio

    
    delay_ms(200);                 // wait a little
    
    
    
    put_pixel(10, 10, 1); // put pixel takes 3 arguments: x and y position, and color (0 -13)
    put_pixel(12, 12, 2);
    put_pixel(14, 14, 3);
    put_pixel(16, 16, 4); // put pixel takes 3 arguments: x and y position, and color (0 -13)
    put_pixel(18, 18, 5);
    put_pixel(20, 20, 6);
    
    delay_ms(4000);         
    
    // clear screen (fill with black)
    clear_screen();
    
    // circle:  x, y location, radius, color
    circle(64, 60, 10, 5);
    circle(64, 60, 30, 7);
    
    // draw line: x0, y0, x1, y1, color
    line(0, 0, 127, 119, 6);
    line(0, 119, 127, 0, 10);

    
    delay_ms(2000);
    
    clear_screen();
    
    // print some characters.
    // there are 4 put_char functions in the graphics.c each
    // for a different size font.
    // Each function takes 4 arguments:  character to print, x location, y location, and color.
    // The three larger fonts are variable width, and return the charcters width. 
    //  It is convinient to have variable x_loc to keep track of position.
    x_loc = 2;                  // set initial position
    x_loc += put_char_arial32('C', x_loc ,4, 3);x_loc += 2;  // add 2 more for space between characters
    x_loc += put_char_arial32('i', x_loc ,4, 4);x_loc += 2; 
    x_loc += put_char_arial32('r', x_loc ,4, 5);x_loc += 2;
    x_loc += put_char_arial32('c', x_loc ,4, 6);x_loc += 2;
    x_loc += put_char_arial32('l', x_loc ,4, 7);x_loc += 2;
    x_loc += put_char_arial32('e', x_loc ,4, 11);x_loc += 2;
    x_loc = 16;
    x_loc += put_char_arial32('F', x_loc ,40, 9);x_loc += 2;
    x_loc += put_char_arial32('u', x_loc ,40, 8);x_loc += 2;
    x_loc += put_char_arial32('n', x_loc ,40, 10);x_loc += 2;
    x_loc += put_char_arial32('.', x_loc ,40, 10);x_loc += 2;
    x_loc += put_char_arial32('.', x_loc ,40, 10);x_loc += 2;
    x_loc += put_char_arial32('.', x_loc ,40, 10);x_loc += 2;
    
    delay_ms(2000);
    
    clear_screen();
    
    delayTime = 255;
    start_ms_timer();               // start ms timer before entering the main loop

    for(;;){
    
        audio_fill_buf();           // this is the fucntion that computes the audio
                                    // it fills a 512 sample buffer that gets output to the DAC
                                    // It needs to be called requarly, about every 30 ms, 
                                    //  or else the buffer will overrun
                                    
        // here is how we do a delay in such a way that we can continue to call audio_fill_buf()
        if (ms_time() > delayTime){   // check to see if event occurs
            start_ms_timer();         // reset timer
      
            // below is where the real application happens.  
            // it draws 4 circles to the screen of different sizes and plays different frequencies   
            //    it speeds up each time thru, untill the end of the sequence and resets...
            
            delayTime--;                // speed up each time thru
            delayTime &= 0xff;          // delay time between 0 - 255
            
            if (delayTime == 0)
                clear_screen();        // clear screen each roll-over
                                         
            i++;                        
            i %= 14;
            
            j ++;
            j &= 0x1f;
            if (!j)
                k++;
            
            a++;
            b++;                    
            c++;
            d++;
            
            a &= k;
            b &= k + 1;                 // do a bunch of arbitray adding, anding to come up with a sequence
            c &= k + 2;
            d &= k + 3;
            
            i++;
            i %= 14;
            
            j ++;
            j &= 0x1f;
            if (!j)
                k++;
            
            circle(32, 30, a * 2, i);
            circle(96, 30, b * 2, i);
            circle(32, 90, c * 2, i);
            circle(96, 90, d * 2, i);
            
            // this function controls the audio synth: voice number (0 - 3), frequency, and amplitude (0 - 255)
            synth_voice(0, a * 440, 255);
            synth_voice(1, b * 220, 255);
            synth_voice(2, c * 110, 255);
            synth_voice(3, d * 55, 255);
        }
    }	
} // main()

// resets the ms clock
void start_ms_timer(void){
    T1TCR = 0x2;                        // stop and reset timer
    T1PR = 58982;                       // timer prescale for ms (58982400 clks/sec)/1000 = 58982.4,
                                        // we round down to 58982 wich is a little off. (about 24ms short in 1 hour)
    T1TCR = 0x1;                        // start timer                                      
}

// this function returns the current ms time since start_ms_timer() was called 
unsigned int ms_time(void){
    volatile unsigned int current_time = 0;
    current_time = T1TC;         
    return current_time;                                
}
