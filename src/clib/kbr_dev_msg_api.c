#include "kbr.h"
#include <stddef.h>

void kbr_PowerDown(kbr_t *kbr)
{
  if ( kbr->is_power_up != 0 )
  {
    kbr->device_cb(kbr, KBR_MSG_DEV_POWER_DOWN, NULL);
    kbr->is_power_up = 0;
  }
}

kbr_int_t kbr_PowerUp(kbr_t *kbr)
{
  kbr_int_t r;
  /* power down first. will do nothing if power is already down */
  kbr_PowerDown(kbr);
  /* now try to power up the display */
  r = kbr->device_cb(kbr, KBR_MSG_DEV_POWER_UP, NULL);
  if ( r != 0 )
  {
    kbr->is_power_up = 1;
  }
  return r;
}

void kbr_SetClipBox(kbr_t *kbr, kbr_box_t *clip_box)
{
  kbr->device_cb(kbr, KBR_MSG_SET_CLIP_BOX, (void *)(clip_box));
}

void kbr_SetClipRange(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h)
{
  kbr_box_t clip_box;
  clip_box.ul.x = x;
  clip_box.ul.y = y;
  clip_box.size.w = w;
  clip_box.size.h = h;
  kbr_SetClipBox(kbr, &clip_box);
}

void kbr_SetMaxClipRange(kbr_t *kbr)
{
  kbr_box_t new_clip_box;
  new_clip_box.size = kbr->dimension;
  new_clip_box.ul.x = 0;
  new_clip_box.ul.y = 0;
  kbr_SetClipBox(kbr, &new_clip_box);
}

/* 
  Query the display dimension from the driver, reset clip window to maximum 
  new dimension
*/
void kbr_GetDimension(kbr_t *kbr)
{
  kbr->device_cb(kbr, KBR_MSG_GET_DIMENSION, &(kbr->dimension));
  kbr_SetMaxClipRange(kbr);
}

void kbr_DrawPixelWithArg(kbr_t *kbr)
{
  kbr->device_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
}

void kbr_DrawL90FXWithArg(kbr_t *kbr)
{
  kbr->device_cb(kbr, KBR_MSG_DRAW_L90FX, &(kbr->arg));
}

#ifdef KBR_MSG_DRAW_L90TC
void kbr_DrawL90TCWithArg(kbr_t *kbr)
{
  kbr->device_cb(kbr, KBR_MSG_DRAW_L90TC, &(kbr->arg));
}
#endif /* KBR_MSG_DRAW_L90TC */

#ifdef KBR_MSG_DRAW_L90BF
void kbr_DrawL90BFWithArg(kbr_t *kbr)
{
  kbr->device_cb(kbr, KBR_MSG_DRAW_L90BF, &(kbr->arg));
}
#endif /* KBR_MSG_DRAW_L90BF */

void kbr_DrawL90SEWithArg(kbr_t *kbr)
{
  kbr->device_cb(kbr, KBR_MSG_DRAW_L90SE, &(kbr->arg));
}
