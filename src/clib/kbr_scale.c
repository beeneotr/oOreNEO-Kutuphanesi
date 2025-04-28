#include "kbr.h"

void kbr_UndoScale(kbr_t *kbr)
{
  if ( kbr->scale_chain_device_cb != NULL )
  {
    kbr->device_cb = kbr->scale_chain_device_cb;
    kbr->scale_chain_device_cb = NULL;
  }
  kbr_GetDimension(kbr);
  kbr_SetMaxClipRange(kbr);
}

const kbr_fntpgm_uint8_t kbr_scale_2x2[16] KBR_FONT_SECTION("kbr_scale_2x2") =  
{ 0x00, 0x03, 0x0c, 0x0f, 0x30, 0x33, 0x3c, 0x3f, 0xc0, 0xc3, 0xcc, 0xcf, 0xf0, 0xf3, 0xfc, 0xff };

 static void kbr_scale_2x2_send_next_half_byte(kbr_t *kbr, kbr_xy_t *xy, kbr_int_t msg, kbr_int_t len, kbr_int_t dir, uint8_t b)
{
  b &= 15;
  len *=2;


  
  kbr->arg.pixel.pos = *xy;
  switch(dir)
  {
    case 0: break;
    case 1: break;
    case 2: kbr->arg.pixel.pos.x++; break;
    default: case 3: kbr->arg.pixel.pos.y++; break;
  }
  
  kbr->arg.bitmap = kbr_scale_2x2+b;
  kbr->arg.len = len;
  kbr->arg.dir = dir;
  kbr->scale_chain_device_cb(kbr, msg, &(kbr->arg));  

  kbr->arg.pixel.pos = *xy;
  switch(dir)
  {
    case 0: kbr->arg.pixel.pos.y++; break;
    case 1: kbr->arg.pixel.pos.x++; break;
    case 2: kbr->arg.pixel.pos.y++; kbr->arg.pixel.pos.x++; break;
    default: case 3: kbr->arg.pixel.pos.x++; kbr->arg.pixel.pos.y++;  break;
  }
  kbr->arg.bitmap = kbr_scale_2x2+b;
  kbr->arg.len = len;
  kbr->arg.dir = dir;
  kbr->scale_chain_device_cb(kbr, msg, &(kbr->arg));  
  
  switch(dir)
  {
    case 0: xy->x+=len; break;
    case 1: xy->y+=len; break;
    case 2: xy->x-=len; break;
    default: case 3: xy->y-=len; break;
  }
  
}

kbr_int_t kbr_dev_scale2x2(kbr_t *kbr, kbr_int_t msg, void *data)
{
  kbr_xy_t xy;
  kbr_int_t len;
  kbr_int_t dir;
  switch(msg)
  {
    case KBR_MSG_GET_DIMENSION:
      kbr->scale_chain_device_cb(kbr, msg, data); 
      ((kbr_wh_t *)data)->h /= 2;
      ((kbr_wh_t *)data)->w /= 2;
      return 1;
      
    case KBR_MSG_SET_CLIP_BOX:
      ((kbr_box_t * )data)->ul.y *= 2; 
      ((kbr_box_t * )data)->ul.x *= 2; 
      ((kbr_box_t * )data)->size.h *= 2;
      ((kbr_box_t * )data)->size.w *= 2;
      
      break;
    case KBR_MSG_DRAW_PIXEL:
      xy = kbr->arg.pixel.pos;
      kbr->arg.pixel.pos.x *= 2;
      kbr->arg.pixel.pos.y *= 2;
      kbr->scale_chain_device_cb(kbr, msg, data); 
      kbr->arg.pixel.pos.x++;
      kbr->scale_chain_device_cb(kbr, msg, data); 
      kbr->arg.pixel.pos.y++;
      kbr->scale_chain_device_cb(kbr, msg, data); 
      kbr->arg.pixel.pos.x--;
      kbr->scale_chain_device_cb(kbr, msg, data); 
      kbr->arg.pixel.pos = xy;
      return 1;
    case KBR_MSG_DRAW_L90SE:
    case KBR_MSG_DRAW_L90FX:
      xy = kbr->arg.pixel.pos;
      len = kbr->arg.len;
      dir = kbr->arg.dir;
    
    
      kbr->arg.pixel.pos.x *= 2;
      kbr->arg.pixel.pos.y *= 2;

      switch(dir)
      {
	case 0: break;
	case 1: break;
	case 2: kbr->arg.pixel.pos.x++; break;
	default: case 3: kbr->arg.pixel.pos.y++; break;
      }
    
      kbr->arg.len *= 2;
      kbr->scale_chain_device_cb(kbr, msg, data);  
    
      kbr->arg.pixel.pos = xy;
      kbr->arg.pixel.pos.x *= 2;
      kbr->arg.pixel.pos.y *= 2;
      kbr->arg.len = len*2;
      kbr->arg.dir = dir;
      switch(dir)
      {
	case 0: kbr->arg.pixel.pos.y++; break;
	case 1: kbr->arg.pixel.pos.x++; break;
	case 2: kbr->arg.pixel.pos.y++; kbr->arg.pixel.pos.x++; break;
	default: case 3: kbr->arg.pixel.pos.x++; kbr->arg.pixel.pos.y++;  break;
      }
      kbr->scale_chain_device_cb(kbr, msg, data);
      
      kbr->arg.pixel.pos = xy;
      kbr->arg.len = len;
      kbr->arg.dir = dir;
      return 1;
#ifdef KBR_MSG_DRAW_L90TC
    case KBR_MSG_DRAW_L90TC:
#endif /* KBR_MSG_DRAW_L90TC */
#ifdef KBR_MSG_DRAW_L90BF
    case KBR_MSG_DRAW_L90BF:
#endif /* KBR_MSG_DRAW_L90BF */

#if defined(KBR_MSG_DRAW_L90TC) || defined(KBR_MSG_DRAW_L90BF)
      xy = kbr->arg.pixel.pos;
      len = kbr->arg.len;
      dir = kbr->arg.dir;

      kbr->arg.pixel.pos.x *= 2;
      kbr->arg.pixel.pos.y *= 2;
    
      {
	const unsigned char *b = kbr->arg.bitmap;
	kbr_xy_t my_xy = kbr->arg.pixel.pos;
	kbr_int_t i;
	for( i = 8; i < len; i+=8 )
	{
	  kbr_scale_2x2_send_next_half_byte(kbr, &my_xy, msg, 4, dir, kbr_pgm_read(b)>>4);
	  kbr_scale_2x2_send_next_half_byte(kbr, &my_xy, msg, 4, dir, kbr_pgm_read(b));
	  b+=1;
	}
	i = len+8-i;
	if ( i > 4 )
	{
	  kbr_scale_2x2_send_next_half_byte(kbr, &my_xy, msg, 4, dir, kbr_pgm_read(b)>>4);
	  kbr_scale_2x2_send_next_half_byte(kbr, &my_xy, msg, i-4, dir, kbr_pgm_read(b));
	}
	else
	{
	  kbr_scale_2x2_send_next_half_byte(kbr, &my_xy, msg, i, dir, kbr_pgm_read(b)>>4);
	}
      }
      kbr->arg.pixel.pos = xy;
      kbr->arg.len = len;
      kbr->arg.dir = dir;
      return 1;
#endif 
  }
  return kbr->scale_chain_device_cb(kbr, msg, data);  
}

/* Side-Effects: Update dimension and reset clip range to max */
void kbr_SetScale2x2(kbr_t *kbr)
{
  kbr_UndoScale(kbr);
  kbr->scale_chain_device_cb = kbr->device_cb;
  kbr->device_cb = kbr_dev_scale2x2;
  kbr_GetDimension(kbr);
  kbr_SetMaxClipRange(kbr);
}
