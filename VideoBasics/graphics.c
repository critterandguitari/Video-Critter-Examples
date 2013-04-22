/******************************************************************************/
/*  graphics.c , version 1.0                                                       */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  Provides higher level graphics, drawing fonts */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



#include "graphics.h"
#include "smallfont.h"
#include "arial32.h"
#include "arial16.h"
#include "arial24.h"

extern unsigned int imageArray[16][128];

void put_pixel(unsigned int on, unsigned int x, unsigned int y){    unsigned int temp;
    
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

unsigned int get_pixel(unsigned int x, unsigned int y){/* doesn't do anything yet*/}


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
          put_pixel(color, (y + k),  (x + (i * 8) + j));
        else
          put_pixel(0, (y + k),  (x + (i * 8) + j));
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
          put_pixel(color, (y + k), (x + (i * 8) + j));
        else
          put_pixel(0, (y + k), (x + (i * 8) + j));
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
          put_pixel(color, (y + k), (x + (i * 8) + j));
        else
          put_pixel(0, (y + k), (x + (i * 8) + j));
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
			    put_pixel(color, y + i, x + j);
			else
                put_pixel(0, y + i, x + j);
		}
	}
        return 5;
}



