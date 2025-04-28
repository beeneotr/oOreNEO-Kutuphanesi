#include "kbr.h"

void kbr_Draw90Line(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len, kbr_int_t dir, kbr_int_t col_idx)
{
  kbr->arg.pixel.rgb.color[0] = kbr->arg.rgb[col_idx].color[0];
  kbr->arg.pixel.rgb.color[1] = kbr->arg.rgb[col_idx].color[1];
  kbr->arg.pixel.rgb.color[2] = kbr->arg.rgb[col_idx].color[2];
  kbr->arg.pixel.pos.x = x;
  kbr->arg.pixel.pos.y = y;
  kbr->arg.len = len;
  kbr->arg.dir = dir;
  kbr_DrawL90FXWithArg(kbr);
}

void kbr_DrawHLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len)
{
  kbr_Draw90Line(kbr, x, y, len, 0, 0);
}

void kbr_DrawVLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len)
{
  kbr_Draw90Line(kbr, x, y, len, 1, 0);
}

void kbr_DrawHRLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len)
{
  kbr_Draw90Line(kbr, x, y, len, 2, 0);
}

void kbr_DrawGradientLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len, kbr_int_t dir)
{
  kbr->arg.pixel.pos.x = x;
  kbr->arg.pixel.pos.y = y;
  kbr->arg.len = len;
  kbr->arg.dir = dir;
  kbr_DrawL90SEWithArg(kbr);
}

void kbr_DrawLine(kbr_t *kbr, kbr_int_t x1, kbr_int_t y1, kbr_int_t x2, kbr_int_t y2)
{
  kbr_int_t tmp;
  kbr_int_t x,y;
  kbr_int_t dx, dy;
  kbr_int_t err;
  kbr_int_t ystep;

  uint8_t swapxy = 0;
  
  /* no BBX intersection check at the moment... */

  kbr->arg.pixel.rgb.color[0] = kbr->arg.rgb[0].color[0];
  kbr->arg.pixel.rgb.color[1] = kbr->arg.rgb[0].color[1];
  kbr->arg.pixel.rgb.color[2] = kbr->arg.rgb[0].color[2];
    
  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;

  if ( dy > dx ) 
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 ) 
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
  err = dx >> 1;
  if ( y2 > y1 ) ystep = 1; else ystep = -1;
  y = y1;
  for( x = x1; x <= x2; x++ )
  {
    if ( swapxy == 0 ) 
    {
      kbr->arg.pixel.pos.x = x;
      kbr->arg.pixel.pos.y = y;
    }
    else 
    {
      kbr->arg.pixel.pos.x = y;
      kbr->arg.pixel.pos.y = x;
    }
    kbr_DrawPixelWithArg(kbr);  
    err -= (uint8_t)dy;
    if ( err < 0 ) 
    {
      y += ystep;
      err += dx;
    }
  }

}
