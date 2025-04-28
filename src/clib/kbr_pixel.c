#include "kbr.h"

void kbr_SetColor(kbr_t *kbr, uint8_t idx, uint8_t r, uint8_t g, uint8_t b)
{
  kbr->arg.rgb[idx].color[0] = r;
  kbr->arg.rgb[idx].color[1] = g;
  kbr->arg.rgb[idx].color[2] = b;
}


void kbr_DrawPixel(kbr_t *kbr, kbr_int_t x, kbr_int_t y)
{
  kbr->arg.pixel.rgb.color[0] = kbr->arg.rgb[0].color[0];
  kbr->arg.pixel.rgb.color[1] = kbr->arg.rgb[0].color[1];
  kbr->arg.pixel.rgb.color[2] = kbr->arg.rgb[0].color[2];
  
  kbr->arg.pixel.pos.x = x;
  kbr->arg.pixel.pos.y = y;
  kbr_DrawPixelWithArg(kbr);  
}

