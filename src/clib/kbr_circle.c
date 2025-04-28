#include "kbr.h"

static void kbr_draw_circle_section(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t x0, kbr_int_t y0, uint8_t option) KBR_NOINLINE;

static void kbr_draw_circle_section(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t x0, kbr_int_t y0, uint8_t option)
{
    /* upper right */
    if ( option & KBR_DRAW_UPPER_RIGHT )
    {
      kbr_DrawPixel(kbr, x0 + x, y0 - y);
      kbr_DrawPixel(kbr, x0 + y, y0 - x);
    }
    
    /* upper left */
    if ( option & KBR_DRAW_UPPER_LEFT )
    {
      kbr_DrawPixel(kbr, x0 - x, y0 - y);
      kbr_DrawPixel(kbr, x0 - y, y0 - x);
    }
    
    /* lower right */
    if ( option & KBR_DRAW_LOWER_RIGHT )
    {
      kbr_DrawPixel(kbr, x0 + x, y0 + y);
      kbr_DrawPixel(kbr, x0 + y, y0 + x);
    }
    
    /* lower left */
    if ( option & KBR_DRAW_LOWER_LEFT )
    {
      kbr_DrawPixel(kbr, x0 - x, y0 + y);
      kbr_DrawPixel(kbr, x0 - y, y0 + x);
    }
}

void kbr_draw_circle(kbr_t *kbr, kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option)
{
    kbr_int_t f;
    kbr_int_t ddF_x;
    kbr_int_t ddF_y;
    kbr_int_t x;
    kbr_int_t y;

    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;

    kbr_draw_circle_section(kbr, x, y, x0, y0, option);
    
    while ( x < y )
    {
      if (f >= 0) 
      {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      kbr_draw_circle_section(kbr, x, y, x0, y0, option);    
    }
}

void kbr_DrawCircle(kbr_t *kbr, kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option)
{
  /* draw circle */
  kbr_draw_circle(kbr, x0, y0, rad, option);
}

static void kbr_draw_disc_section(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t x0, kbr_int_t y0, uint8_t option) KBR_NOINLINE;

static void kbr_draw_disc_section(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t x0, kbr_int_t y0, uint8_t option)
{
    /* upper right */
    if ( option & KBR_DRAW_UPPER_RIGHT )
    {
      kbr_DrawVLine(kbr, x0+x, y0-y, y+1);
      kbr_DrawVLine(kbr, x0+y, y0-x, x+1);
    }
    
    /* upper left */
    if ( option & KBR_DRAW_UPPER_LEFT )
    {
      kbr_DrawVLine(kbr, x0-x, y0-y, y+1);
      kbr_DrawVLine(kbr, x0-y, y0-x, x+1);
    }
    
    /* lower right */
    if ( option & KBR_DRAW_LOWER_RIGHT )
    {
      kbr_DrawVLine(kbr, x0+x, y0, y+1);
      kbr_DrawVLine(kbr, x0+y, y0, x+1);
    }
    
    /* lower left */
    if ( option & KBR_DRAW_LOWER_LEFT )
    {
      kbr_DrawVLine(kbr, x0-x, y0, y+1);
      kbr_DrawVLine(kbr, x0-y, y0, x+1);
    }
}

void kbr_draw_disc(kbr_t *kbr, kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option)
{
  kbr_int_t f;
  kbr_int_t ddF_x;
  kbr_int_t ddF_y;
  kbr_int_t x;
  kbr_int_t y;

  f = 1;
  f -= rad;
  ddF_x = 1;
  ddF_y = 0;
  ddF_y -= rad;
  ddF_y *= 2;
  x = 0;
  y = rad;

  kbr_draw_disc_section(kbr, x, y, x0, y0, option);
  
  while ( x < y )
  {
    if (f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    kbr_draw_disc_section(kbr, x, y, x0, y0, option);    
  }
}

void kbr_DrawDisc(kbr_t *kbr, kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option)
{
  /* draw disc */
  kbr_draw_disc(kbr, x0, y0, rad, option);
}

