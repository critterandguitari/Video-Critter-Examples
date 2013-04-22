/******************************************************************************/
/*  graphics.h , version 1.1                                                       */
/*  (c)2006 GPL, Owen Osborn, Critter and Guitari                             */
/******************************************************************************/
/*                                                                            */
/*  :  Provides higher level graphics, drawing fonts, line, and cricle */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



void put_pixel(unsigned int x, unsigned int y, unsigned int color);

unsigned int get_pixel(unsigned int x, unsigned int y);

void clear_screen(void);

unsigned int put_char_arial32(unsigned char character, unsigned int y, unsigned int x, unsigned int color);

unsigned int put_char_arial24(unsigned char character, unsigned int y, unsigned int x, unsigned int color);

unsigned int put_char_arial16(unsigned char character, unsigned int y, unsigned int x, unsigned int color);

unsigned int put_char_small(unsigned char c, unsigned int y, unsigned int x, unsigned int color);

// bresenham line algorithm
void line(int x0, int y0, int x1, int y1, unsigned int color);
    
// bresenham circle algorithm
void circle(unsigned int h, unsigned int k, unsigned int r, unsigned int color);



