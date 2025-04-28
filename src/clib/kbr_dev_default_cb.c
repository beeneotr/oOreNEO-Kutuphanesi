#include "kbr.h"

/* 
  default device callback
  this should be (finally) called by any other device callback to handle
  messages, which are not yet handled.
*/

kbr_int_t kbr_dev_default_cb(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_DRAW_L90SE:
      return kbr->ext_cb(kbr, msg, data);
    case KBR_MSG_SET_CLIP_BOX:
      kbr->clip_box = *(kbr_box_t *)data;
      break;
  }
  return 1;	/* all ok */
}

/*
  will be used as default cb if no extentions callback is provided
*/
kbr_int_t kbr_ext_none(kbr_t *kbr, kbr_int_t msg, void *data)
{
  return 1;	/* all ok */  
}



/*
  handle KBR_MSG_DRAW_L90FX message and make calls to "dev_cb" with KBR_MSG_DRAW_PIXEL
  return 1 if something has been drawn
*/
kbr_int_t kbr_handle_l90fx(kbr_t *kbr, kbr_dev_fnptr dev_cb)
{
  if ( kbr_clip_l90fx(kbr) != 0 )
  {
    kbr_int_t dx, dy;
    kbr_int_t i;
    switch(kbr->arg.dir)
    {
      case 0: dx = 1; dy = 0; break;
      case 1: dx = 0; dy = 1; break;
      case 2: dx = -1; dy = 0; break;
      case 3: dx = 0; dy = -1; break;
      default: dx = 1; dy = 0; break;	/* avoid compiler warning */
    }
    for( i = 0; i < kbr->arg.len; i++ )
    {
      dev_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
      kbr->arg.pixel.pos.x+=dx;
      kbr->arg.pixel.pos.y+=dy;
    }
    return 1;
  }
  return 0;
}


/*
  handle KBR_MSG_DRAW_L90TC message and make calls to "dev_cb" with KBR_MSG_DRAW_PIXEL
  return 1 if something has been drawn
*/
kbr_int_t kbr_handle_l90tc(kbr_t *kbr, kbr_dev_fnptr dev_cb)
{
  if ( kbr_clip_l90tc(kbr) != 0 )
  {
    kbr_int_t dx, dy;
    kbr_int_t i;
    unsigned char pixmap;
    uint8_t bitcnt;
    switch(kbr->arg.dir)
    {
      case 0: dx = 1; dy = 0; break;
      case 1: dx = 0; dy = 1; break;
      case 2: dx = -1; dy = 0; break;
      case 3: dx = 0; dy = -1; break;
      default: dx = 1; dy = 0; break;	/* avoid compiler warning */
    }
    pixmap = kbr_pgm_read(kbr->arg.bitmap);
    bitcnt = kbr->arg.pixel_skip;
    pixmap <<= bitcnt;
    for( i = 0; i < kbr->arg.len; i++ )
    {
      if ( (pixmap & 128) != 0 )
      {
	dev_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
      }
      pixmap<<=1;
      kbr->arg.pixel.pos.x+=dx;
      kbr->arg.pixel.pos.y+=dy;
      bitcnt++;
      if ( bitcnt >= 8 )
      {
	kbr->arg.bitmap++;
	pixmap = kbr_pgm_read(kbr->arg.bitmap);
	bitcnt = 0;
      }
    }
    return 1;
  }
  return 0;
}


/*
  handle KBR_MSG_DRAW_L90FB message and make calls to "dev_cb" with KBR_MSG_DRAW_PIXEL
  return 1 if something has been drawn
*/
kbr_int_t kbr_handle_l90bf(kbr_t *kbr, kbr_dev_fnptr dev_cb)
{
  if ( kbr_clip_l90tc(kbr) != 0 )
  {
    kbr_int_t dx, dy;
    kbr_int_t i, y;
    unsigned char pixmap;
    uint8_t bitcnt;
    switch(kbr->arg.dir)
    {
      case 0: dx = 1; dy = 0; break;
      case 1: dx = 0; dy = 1; break;
      case 2: dx = -1; dy = 0; break;
      case 3: dx = 0; dy = -1; break;
      default: dx = 1; dy = 0; break;	/* avoid compiler warning */
    }
    pixmap = kbr_pgm_read(kbr->arg.bitmap);
    bitcnt = kbr->arg.pixel_skip;
    pixmap <<= bitcnt;
    for( i = 0; i < kbr->arg.len; i++ )
    {
      for( y = 0; y < kbr->arg.scale; y++ )
      {
	if ( (pixmap & 128) == 0 )
	{
	  kbr->arg.pixel.rgb = kbr->arg.rgb[1];
	  dev_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
	}
	else
	{
	  kbr->arg.pixel.rgb = kbr->arg.rgb[0];
	  dev_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
	}
	kbr->arg.pixel.pos.x+=dx;
	kbr->arg.pixel.pos.y+=dy;
      }
      pixmap<<=1;
      bitcnt++;
      if ( bitcnt >= 8 )
      {
	kbr->arg.bitmap++;
	pixmap = kbr_pgm_read(kbr->arg.bitmap);
	bitcnt = 0;
      }
    }
    return 1;
  }
  return 0;
}

/*
  handle KBR_MSG_DRAW_L90SE message and make calls to "dev_cb" with KBR_MSG_DRAW_PIXEL
  return 1 if something has been drawn
*/

kbr_int_t kbr_handle_l90se(kbr_t *kbr, kbr_dev_fnptr dev_cb)
{
  uint8_t i;
  
  /* Setup ccs for l90se. This will be updated by kbr_clip_l90se if required */
  
  for ( i = 0; i < 3; i++ )
  {
    kbr_ccs_init(kbr->arg.ccs_line+i, kbr->arg.rgb[0].color[i], kbr->arg.rgb[1].color[i], kbr->arg.len);
  }
  
  /* check if the line is visible */
  
  if ( kbr_clip_l90se(kbr) != 0 )
  {
    kbr_int_t dx, dy;
    kbr_int_t i, j;
    switch(kbr->arg.dir)
    {
      case 0: dx = 1; dy = 0; break;
      case 1: dx = 0; dy = 1; break;
      case 2: dx = -1; dy = 0; break;
      case 3: dx = 0; dy = -1; break;
      default: dx = 1; dy = 0; break;	/* avoid compiler warning */
    }
    for( i = 0; i < kbr->arg.len; i++ )
    {
      kbr->arg.pixel.rgb.color[0] = kbr->arg.ccs_line[0].current;
      kbr->arg.pixel.rgb.color[1] = kbr->arg.ccs_line[1].current; 
      kbr->arg.pixel.rgb.color[2] = kbr->arg.ccs_line[2].current;
      dev_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
      kbr->arg.pixel.pos.x+=dx;
      kbr->arg.pixel.pos.y+=dy;
      for ( j = 0; j < 3; j++ )
      {
	kbr_ccs_step(kbr->arg.ccs_line+j);
      }
    }
    return 1;
  }
  return 0;
}

/*
  l90rl run lenth encoded constant color pattern

  yyllllll wwwwbbbb wwwwbbbb wwwwbbbb wwwwbbbb ...
  sequence terminates if wwwwbbbb = 0
  wwww: number of pixel to skip
  bbbb: number of pixel to draw with color
  llllll: number of bytes which follow, can be 0
*/

#ifdef ON_HOLD
void kbr_handle_l90rl(kbr_t *kbr, kbr_dev_fnptr dev_cb)
{
  kbr_int_t dx, dy;
  uint8_t i, cnt;
  uint8_t rl_code;
  kbr_int_t skip;
  
  switch(kbr->arg.dir)
  {
    case 0: dx = 1; dy = 0; break;
    case 1: dx = 0; dy = 1; break;
    case 2: dx = -1; dy = 0; break;
    case 3: dx = 0; dy = -1; break;
    default: dx = 1; dy = 0; break;	/* avoid compiler warning */
  }
    
  cnt = kbr_pgm_read(kbr->arg.bitmap);
  cnt &= 63;
  kbr->arg.bitmap++;
  for( i = 0; i < cnt; i++ )
  {
    rl_code = kbr_pgm_read(kbr->arg.bitmap);
    if ( rl_code == 0 )
      break;
    
    skip = (kbr_int_t)(rl_code >> 4);
    switch(kbr->arg.dir)
    {
      case 0: kbr->arg.pixel.pos.x+=skip; break;
      case 1: kbr->arg.pixel.pos.y+=skip; break;
      case 2: kbr->arg.pixel.pos.x-=skip; break;
      default:
      case 3: kbr->arg.pixel.pos.y-=skip; break;
    }

    rl_code &= 15;
    while( rl_code )
    {
      dev_cb(kbr, KBR_MSG_DRAW_PIXEL, NULL);
      kbr->arg.pixel.pos.x+=dx;
      kbr->arg.pixel.pos.y+=dy;
      rl_code--;
    }
    kbr->arg.bitmap++;
  }
}
#endif