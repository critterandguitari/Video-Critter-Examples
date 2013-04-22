/******************************************************************************/
/*  graphics.h , version 1.0                                                       */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  Provides higher level graphics, drawing fonts */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



void put_pixel(unsigned int color, unsigned int x, unsigned int y);

unsigned int get_pixel(unsigned int x, unsigned int y);

unsigned int put_char_arial32(unsigned char character, unsigned int y, unsigned int x, unsigned int color);

unsigned int put_char_arial24(unsigned char character, unsigned int y, unsigned int x, unsigned int color);

unsigned int put_char_arial16(unsigned char character, unsigned int y, unsigned int x, unsigned int color);

unsigned int put_char_small(unsigned char c, unsigned int y, unsigned int x, unsigned int color);



