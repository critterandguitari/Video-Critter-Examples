/******************************************************************************/
/*  graphics.c , version 1.1                                                       */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  Provides higher level graphics, drawing fonts, circle, and line */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



#include "graphics.h"
#include "smallfont.h"
#include "arial32.h"
#include "arial16.h"
#include "arial24.h"


#define ABS(a)	   (((a) < 0) ? -(a) : (a))

extern unsigned int imageArray[16][128];  // image data from video.c

void put_pixel(unsigned int x, unsigned int y, unsigned int on ){    unsigned int temp;
    
    x &= 0x7f;
    y &= 0x7f;
    
    
    if (on){
        temp = imageArray[x / 8][y];
        temp &= ~(0xf << ((x & 0x7) * 4) );
        imageArray[x / 8][y] = temp | (((on % 13) + 3) << ((x & 0x7) * 4));
    }
    else{
       temp = imageArray[x / 8][y];
        temp &= ~(0xf << ((x & 0x7) * 4) );
        imageArray[x / 8][y] = temp | (1 << ((x & 0x7) * 4));
   }
}

unsigned int get_pixel(unsigned int x, unsigned int y){/* fill this in */}

void clear_screen(void){
    int i, j;
    for (i = 0; i < 16; i++){
        for (j = 0; j < 128; j++){
            imageArray[i][j] = 0x11111111;  

        }
    }

}


unsigned int put_char_arial32(unsigned char character, unsigned int y, unsigned int x, unsigned int color){
  int i;
  int j;
  int k;
  int charWidth;
  int charOffset;
  
  if (character == 32)
    return 8;
  
  character -= 33;

  charWidth = arial32Width[character + 1];
  charOffset = arial32Offset[character] * 4;

  for (i = 0; i < 4; i++){
    for (j = 0; j < 8; j++){
      for (k = 0; k < charWidth; k++){
        if ((arial32[charOffset + k + (i * charWidth)] >> j) & 0x01)
          put_pixel((y + k),  (x + (i * 8) + j), color);
        else
          put_pixel((y + k),  (x + (i * 8) + j), 0);
      }
    }
  }
  return charWidth;
}

unsigned int put_char_arial24(unsigned char character, unsigned int y, unsigned int x, unsigned int color){
  int i;
  int j;
  int k;
  int charWidth;
  int charOffset;

  if (character == 32)
    return 6;

  character -= 33;

  charWidth = arial24Width[character + 1];
  charOffset = arial24Offset[character] * 3;


  
  for (i = 0; i < 3; i++){
    for (j = 0; j < 8; j++){
      for (k = 0; k < charWidth; k++){
        if ((arial24[charOffset + k + (i * charWidth)] >> j) & 0x01)
          put_pixel((y + k), (x + (i * 8) + j), color);
        else
          put_pixel((y + k), (x + (i * 8) + j), 0);
      }
    }
  }
  return charWidth;
}

unsigned int put_char_arial16(unsigned char character, unsigned int y, unsigned int x, unsigned int color){
  int i;
  int j;
  int k;
  int charWidth;
  int charOffset;

  if (character == 32)
    return 4;

  character -= 33;

  charWidth = arial16Width[character + 1];
  charOffset = arial16Offset[character] * 2;


  
  for (i = 0; i < 2; i++){
    for (j = 0; j < 8; j++){
      for (k = 0; k < charWidth; k++){
        if ((arial16[charOffset + k + (i * charWidth)] >> j) & 0x01)
          put_pixel((y + k), (x + (i * 8) + j), color);
        else
          put_pixel((y + k), (x + (i * 8) + j), 0);
      }
    }
  }
  return charWidth;
}


unsigned int put_char_small(unsigned char c, unsigned int y, unsigned int x, unsigned int color){
	int i, j;
	c -= 32;
	for (i = 0; i < 5; i++){
		for (j = 0; j < 8; j++){
		    if ((characters[(c * 5) + i] >> j) & 0x01)
			    put_pixel(y + i, x + j, color);
			else
                put_pixel(y + i, x + j, 0);
		}
	}
        return 5;
}

// bresenham line algorithm from http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html
// modified for video critter

void line(int x0, int y0, int x1, int y1, unsigned int color) {
        int dy = y1 - y0;
        int dx = x1 - x0;
        int stepx, stepy;
        int fraction;

        if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
        if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
        dy <<= 1;                                                  // dy is now 2*dy
        dx <<= 1;                                                  // dx is now 2*dx

        put_pixel( x0, y0, color);
        if (dx > dy) {
            fraction = dy - (dx >> 1);                         // same as 2*dy - dx
            while (x0 != x1) {
                if (fraction >= 0) {
                    y0 += stepy;
                    fraction -= dx;                                // same as fraction -= 2*dx
                }
                x0 += stepx;
                fraction += dy;                                    // same as fraction -= 2*dy
                put_pixel(x0, y0, color);
            }
        } else {
            int fraction = dx - (dy >> 1);
            while (y0 != y1) {
                if (fraction >= 0) {
                    x0 += stepx;
                    fraction -= dy;
                }
                y0 += stepy;
                fraction += dx;
                put_pixel(x0, y0, color);
            }
        }
}

    
// bresenham circle algorithm
void circle(unsigned int h, unsigned int k, unsigned int r, unsigned int color){

       int x=0;
       int y=r;
       int p=(3-(2*r));

       do
	  {
	     put_pixel((h+x),(k+y),color);
	     put_pixel((h+y),(k+x),color);
	     put_pixel((h+y),(k-x),color);
	     put_pixel((h+x),(k-y),color);
	     put_pixel((h-x),(k-y),color);
	     put_pixel((h-y),(k-x),color);
	     put_pixel((h-y),(k+x),color);
	     put_pixel((h-x),(k+y),color);

	     x++;

	     if(p<0)
		p+=((4*x)+6);

	     else
		{
		   y--;
		   p+=((4*(x-y))+10);
		}
	  }
       while(x<=y);
}


