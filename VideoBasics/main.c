/******************************************************************************/
/*  main.c                                                        */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  demonstrates some simple graphics on the TV */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

#include "LPC21xx.h"
#include "system.h"
#include "video.h"


int main (void) {

    int i, j, k;
    
    unsigned int x_loc = 0;
    unsigned int y_loc = 0;

    int color = 0;


    Initialize();               // Initialize the MCU

    led_board(0);
    delay_ms(100);              // flash LEDs
    led_board(1);
    delay_ms(100);
    led_board(0);
    delay_ms(100);
    led_board(1);  
    
    video_init();               // initialize synthesis

    delay_ms(10);               // delay some 
      
    
                               // fill screen with constant color
    for (i = 0; i < 100; i++){
        for (j = 0; j < 128; j++){
            put_pixel(0, j, i);     // fill with black. put pixel takes 3 arguments: color (0 -13), x and y position 

        }
    }
    
    put_pixel(5, 64, 50);          // put a dot in center of screen, 5 is a greenish color
    
    delay_ms(2000);               // wait 2 seconds
    
    put_pixel(0, 64, 50);          // replace with black

    // print some characters.
    // there are 4 put_char functions in the graphics.c each
    // for a different size font.
    // Each function takes 4 arguments:  character to print, x location, y location, color.
    // The three larger fonts are variable width, and return the charcters width. 
    //  It is convinient to have variable x_loc to keep track of position.
    x_loc = 0;                  // set initial position


    x_loc = 17;
    x_loc += put_char_arial16('v', x_loc ,4, 3);x_loc += 2;
    x_loc += put_char_arial16('i', x_loc ,4, 4);x_loc += 2;
    x_loc += put_char_arial16('d', x_loc ,4, 5);x_loc += 2;
    x_loc += put_char_arial16('e', x_loc ,4, 6);x_loc += 2;
    x_loc += put_char_arial16('o', x_loc ,4, 7);x_loc += 8;
    x_loc += put_char_arial16('c', x_loc ,4, 11);x_loc += 2;
    x_loc += put_char_arial16('r', x_loc ,4, 9);x_loc += 2;
    x_loc += put_char_arial16('i', x_loc ,4, 8);x_loc += 2;
    x_loc += put_char_arial16('t', x_loc ,4, 10);x_loc += 2;
    x_loc += put_char_arial16('t', x_loc ,4, 12);x_loc += 2;
    x_loc += put_char_arial16('e', x_loc ,4, 13);x_loc += 2;
    x_loc += put_char_arial16('r', x_loc ,4, 2);x_loc += 2;
    
    x_loc = 30;
    x_loc += put_char_arial16('1', x_loc ,28, 12);x_loc += 1;
    x_loc += put_char_arial16('2', x_loc ,28, 12);x_loc += 5;
    x_loc += put_char_arial16('C', x_loc ,28, 12);x_loc += 1;
    x_loc += put_char_arial16('o', x_loc ,28, 12);x_loc += 1;
    x_loc += put_char_arial16('l', x_loc ,28, 12);x_loc += 1;
    x_loc += put_char_arial16('o', x_loc ,28, 12);x_loc += 1;
    x_loc += put_char_arial16('r', x_loc ,28, 12);x_loc += 1;
    x_loc += put_char_arial16('s', x_loc ,28, 12);x_loc += 1;
    
    x_loc = 12;
    x_loc += put_char_arial16('1', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('2', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('8', x_loc ,48, 7);x_loc += 4;
    x_loc += put_char_arial16('x', x_loc ,48, 7);x_loc += 4;
    x_loc += put_char_arial16('1', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('2', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('0', x_loc ,48, 7);x_loc += 4;
    x_loc += put_char_arial16('p', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('i', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('x', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('e', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('l', x_loc ,48, 7);x_loc += 1;
    x_loc += put_char_arial16('s', x_loc ,48, 7);x_loc += 1;
    
    // these functions draw big squares at the bottom of the screen
    for (i = 68; i < 76; i++){
        for (j = 0; j < 16; j++){
            put_pixel(0, (j * 8), i);
            put_pixel(0, (j * 8) + 1, i);
            for (k = 2; k < 8; k++)
                put_pixel(j, (j * 8) + k , i);
        }
    }
    
    for (i = 78; i < 86; i++){
        for (j = 0; j < 16; j++){
            put_pixel(0, (j * 8), i);
            put_pixel(0, (j * 8) + 1, i);
            for (k = 2; k < 8; k++)
                put_pixel(j + 4, (j * 8) + k , i);
        }
    }

    for (i = 88; i < 96; i++){
        for (j = 0; j < 16; j++){
            put_pixel(0, (j * 8), i);
            put_pixel(0, (j * 8) + 1, i);
            for (k = 2; k < 8; k++)
                put_pixel(j + 8, (j * 8) + k , i);
        }
    }
    
    delay_ms(2000);                 // wait 2 seconds
    

    for(;;){
        
    }	
    
    
} // main()


