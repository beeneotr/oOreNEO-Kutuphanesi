#include "kbr.h"

#ifdef KBR_MSG_DRAW_L90TC
void kbr_DrawTransparentBitmapLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t dir, kbr_int_t len, const unsigned char *bitmap)
{
  kbr->arg.pixel.rgb.color[0] = kbr->arg.rgb[0].color[0];
  kbr->arg.pixel.rgb.color[1] = kbr->arg.rgb[0].color[1];
  kbr->arg.pixel.rgb.color[2] = kbr->arg.rgb[0].color[2];
  kbr->arg.pixel.pos.x = x;
  kbr->arg.pixel.pos.y = y;
  kbr->arg.dir = dir;
  kbr->arg.len = len;
  kbr->arg.bitmap = bitmap;
  kbr->arg.pixel_skip = 0;
  kbr_DrawL90TCWithArg(kbr);
}
#endif /* KBR_MSG_DRAW_L90TC */

#ifdef KBR_MSG_DRAW_L90BF
void kbr_DrawBitmapLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t dir, kbr_int_t len, const unsigned char *bitmap)
{
  kbr->arg.pixel.pos.x = x;
  kbr->arg.pixel.pos.y = y;
  kbr->arg.dir = dir;
  kbr->arg.len = len;
  kbr->arg.bitmap = bitmap;
  kbr->arg.pixel_skip = 0;
  kbr_DrawL90BFWithArg(kbr);
}
#endif /* KBR_MSG_DRAW_L90BF */

#ifdef ON_HOLD
void kbr_DrawRLBitmap(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t dir, const unsigned char *rl_bitmap)
{
  kbr->arg.pixel.rgb.color[0] = kbr->arg.rgb[0].color[0];
  kbr->arg.pixel.rgb.color[1] = kbr->arg.rgb[0].color[1];
  kbr->arg.pixel.rgb.color[2] = kbr->arg.rgb[0].color[2];
  kbr->arg.pixel.pos.x = x;
  kbr->arg.pixel.pos.y = y;
  kbr->arg.dir = dir;
  kbr->arg.len = 0;
  kbr->arg.bitmap = rl_bitmap;
  kbr_DrawL90RLWithArg(kbr);
}
#endif
