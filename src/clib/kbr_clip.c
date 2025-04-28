#include "kbr.h"

static kbr_int_t kbr_clip_is_x_visible(kbr_t *kbr) KBR_NOINLINE;
static kbr_int_t kbr_clip_is_y_visible(kbr_t *kbr) KBR_NOINLINE;

static kbr_int_t kbr_clip_is_x_visible(kbr_t *kbr)
{
  kbr_int_t t;
  t = kbr->arg.pixel.pos.x;
  t -= kbr->clip_box.ul.x;
  if ( t < 0 )
    return 0;
  if ( t >= kbr->clip_box.size.w )
    return 0;
  
  return 1;
}

static kbr_int_t kbr_clip_is_y_visible(kbr_t *kbr)
{
  kbr_int_t t;
  t = kbr->arg.pixel.pos.y;
  t -= kbr->clip_box.ul.y;
  if ( t < 0 )
    return 0;
  if ( t >= kbr->clip_box.size.h )
    return 0;
  
  return 1;
}

/*
  Description:
    clip range from a (included) to b (excluded) agains c (included) to d (excluded)
  Assumptions:
    a <= b
    c <= d
*/
static kbr_int_t kbr_clip_intersection(kbr_int_t *ap, kbr_int_t *bp, kbr_int_t c, kbr_int_t d)
{
  kbr_int_t a = *ap;
  kbr_int_t b = *bp;
  
  if ( a >= d )
    return 0;
  if ( b <= c )
    return 0;
  if ( a < c )
    *ap = c;
  if ( b > d )
    *bp = d;
  return 1;
}

kbr_int_t kbr_clip_is_pixel_visible(kbr_t *kbr)
{
  if ( kbr_clip_is_x_visible(kbr) == 0 )
    return 0;
  if ( kbr_clip_is_y_visible(kbr) == 0 )
    return 0;
  return 1;
}



/*
  assumes, that kbr->arg contains data for l90fx and does clipping 
  against kbr->clip_box
*/
kbr_int_t kbr_clip_l90fx(kbr_t *kbr)
{
  kbr_int_t a;
  kbr_int_t b;
  kbr->arg.offset = 0;
  switch(kbr->arg.dir)
  {
    case 0:
      if ( kbr_clip_is_y_visible(kbr) == 0 )
	return 0; 
      a = kbr->arg.pixel.pos.x;
      b = a;
      b += kbr->arg.len;
      
      if ( kbr_clip_intersection(&a, &b, kbr->clip_box.ul.x, kbr->clip_box.ul.x+kbr->clip_box.size.w) == 0 )
	return 0;
      
      kbr->arg.offset = a - kbr->arg.pixel.pos.x;
      kbr->arg.pixel.pos.x = a;
      b -= a;
      kbr->arg.len = b;
      
      break;
    case 1:
      if ( kbr_clip_is_x_visible(kbr) == 0 )
	return 0;
      
      a = kbr->arg.pixel.pos.y;
      b = a;
      b += kbr->arg.len;
      
      if ( kbr_clip_intersection(&a, &b, kbr->clip_box.ul.y, kbr->clip_box.ul.y+kbr->clip_box.size.h) == 0 )
	return 0;

      kbr->arg.offset = a - kbr->arg.pixel.pos.y;
      kbr->arg.pixel.pos.y = a;
      b -= a;
      kbr->arg.len = b;
      
      break;
    case 2:
      if ( kbr_clip_is_y_visible(kbr) == 0 )
	return 0;
      
      b = kbr->arg.pixel.pos.x;
      b++;
      
      a = b;
      a -= kbr->arg.len;
      
      
      if ( kbr_clip_intersection(&a, &b, kbr->clip_box.ul.x, kbr->clip_box.ul.x+kbr->clip_box.size.w) == 0 )
	return 0;
      
      kbr->arg.len = b-a;
      
      b--;
      kbr->arg.offset = kbr->arg.pixel.pos.x-b;
      kbr->arg.pixel.pos.x = b;
      
      break;
    case 3:
      if ( kbr_clip_is_x_visible(kbr) == 0 )
	return 0;

      b = kbr->arg.pixel.pos.y;
      b++;
      
      a = b;
      a -= kbr->arg.len;
      
      
      if ( kbr_clip_intersection(&a, &b, kbr->clip_box.ul.y, kbr->clip_box.ul.y+kbr->clip_box.size.h) == 0 )
	return 0;
      
      kbr->arg.len = b-a;
      
      b--;
      kbr->arg.offset = kbr->arg.pixel.pos.y-b;
      kbr->arg.pixel.pos.y = b;
      
      
      break;
  }

  return 1;
}

kbr_int_t kbr_clip_l90tc(kbr_t *kbr)
{
  if ( kbr_clip_l90fx(kbr) == 0 )
      return 0;
  kbr->arg.pixel_skip = kbr->arg.offset & 0x07;
  kbr->arg.bitmap += (kbr->arg.offset >>3);
  return 1;
}


kbr_int_t kbr_clip_l90se(kbr_t *kbr)
{
  uint8_t i;
  if ( kbr_clip_l90fx(kbr) == 0 )
      return 0;
  for ( i = 0; i < 3; i++ )
  {
    kbr_ccs_seek(kbr->arg.ccs_line+i, kbr->arg.offset);
  }  
  return 1;
}

