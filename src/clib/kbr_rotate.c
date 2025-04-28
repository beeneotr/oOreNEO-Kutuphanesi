#include "kbr.h"
#include <assert.h>

/* Side-Effects: Update dimension and reset clip range to max */
void kbr_UndoRotate(kbr_t *kbr)
{
  if ( kbr->rotate_chain_device_cb != NULL )
  {
    kbr->device_cb = kbr->rotate_chain_device_cb;
    kbr->rotate_chain_device_cb = NULL;
  }
  kbr_GetDimension(kbr);
  kbr_SetMaxClipRange(kbr);
}

/*================================================*/
/* 90 degree */

static void kbr_rotate_90_xy(kbr_xy_t *xy, kbr_int_t display_width)
{   
    kbr_int_t x, y;
    y = xy->x;
    x = display_width;
    x -= xy->y;
    x--;
    xy->x = x;
    xy->y = y;  
  
}

kbr_int_t kbr_dev_rotate90(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_GET_DIMENSION:
      kbr->rotate_chain_device_cb(kbr, msg, &(kbr->rotate_dimension)); 
      ((kbr_wh_t *)data)->h = kbr->rotate_dimension.w;
      ((kbr_wh_t *)data)->w = kbr->rotate_dimension.h;
    
      return 1;
      
    case KBR_MSG_SET_CLIP_BOX:
      /* to rotate the box, the lower left corner will become the new xy value pair */
      /* so the unrotated lower left is put into "ul" */
      ((kbr_box_t * )data)->ul.y += ((kbr_box_t * )data)->size.h-1;
      /* then apply rotation */
      kbr_rotate_90_xy(&(((kbr_box_t * )data)->ul), kbr->rotate_dimension.w); 
      /* finally, swap dimensions */
      {
        kbr_int_t tmp;
        tmp = ((kbr_box_t *)data)->size.w;
        ((kbr_box_t * )data)->size.w = ((kbr_box_t *)data)->size.h;
        ((kbr_box_t * )data)->size.h = tmp;
      }
      break;
    case KBR_MSG_DRAW_PIXEL:
    case KBR_MSG_DRAW_L90FX:
#ifdef KBR_MSG_DRAW_L90TC
    case KBR_MSG_DRAW_L90TC:
#endif /* KBR_MSG_DRAW_L90TC */
#ifdef KBR_MSG_DRAW_L90BF
    case KBR_MSG_DRAW_L90BF:
#endif /* KBR_MSG_DRAW_L90BF */
    case KBR_MSG_DRAW_L90SE:
      kbr->arg.dir+=1;
      kbr->arg.dir&=3;
      kbr_rotate_90_xy(&(kbr->arg.pixel.pos), kbr->rotate_dimension.w); 
      break;
  }
  return kbr->rotate_chain_device_cb(kbr, msg, data);  
}

/* Side-Effects: Update dimension and reset clip range to max */
void kbr_SetRotate90(kbr_t *kbr)
{
  kbr_UndoRotate(kbr);
  kbr->rotate_chain_device_cb = kbr->device_cb;
  kbr->device_cb = kbr_dev_rotate90;
  kbr_GetDimension(kbr);
  kbr_SetMaxClipRange(kbr);
}

/*================================================*/
/* 180 degree */

static void kbr_rotate_180_xy(kbr_t *kbr, kbr_xy_t *xy)
{
    kbr_int_t x, y;
    y = kbr->rotate_dimension.h;
    y -= xy->y;
    y--;
    xy->y = y;
  
    x = kbr->rotate_dimension.w;
    x -= xy->x;
    x--;
    xy->x = x;
  
}

kbr_int_t kbr_dev_rotate180(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_GET_DIMENSION:
      kbr->rotate_chain_device_cb(kbr, msg, &(kbr->rotate_dimension)); 
      *((kbr_wh_t *)data) = (kbr->rotate_dimension);
      return 1;
    case KBR_MSG_SET_CLIP_BOX:
      /* calculate and rotate lower right point of the clip box */
      ((kbr_box_t * )data)->ul.y += ((kbr_box_t * )data)->size.h-1;
      ((kbr_box_t * )data)->ul.x += ((kbr_box_t * )data)->size.w-1;
      kbr_rotate_180_xy(kbr, &(((kbr_box_t * )data)->ul)); 
      /* box dimensions are the same */
      break;
    case KBR_MSG_DRAW_PIXEL:
    case KBR_MSG_DRAW_L90FX:
#ifdef KBR_MSG_DRAW_L90TC
    case KBR_MSG_DRAW_L90TC:
#endif /* KBR_MSG_DRAW_L90TC */
#ifdef KBR_MSG_DRAW_L90BF
    case KBR_MSG_DRAW_L90BF:
#endif /* KBR_MSG_DRAW_L90BF */
    case KBR_MSG_DRAW_L90SE:
      kbr->arg.dir+=2;
      kbr->arg.dir&=3;
      kbr_rotate_180_xy(kbr, &(kbr->arg.pixel.pos)); 
      break;
  }
  return kbr->rotate_chain_device_cb(kbr, msg, data);  
}

/* Side-Effects: Update dimension and reset clip range to max */
void kbr_SetRotate180(kbr_t *kbr)
{
  kbr_UndoRotate(kbr);
  kbr->rotate_chain_device_cb = kbr->device_cb;
  kbr->device_cb = kbr_dev_rotate180;
  kbr_GetDimension(kbr);
  kbr_SetMaxClipRange(kbr);
}

/*================================================*/
/* 270 degree */

static void kbr_rotate_270_xy(kbr_t *kbr, kbr_xy_t *xy)
{
    kbr_int_t x, y;
    x = xy->y;
  
    y = kbr->rotate_dimension.h;
    y -= xy->x;
    y--;
  
    xy->y = y;
    xy->x = x;  
}

kbr_int_t kbr_dev_rotate270(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_GET_DIMENSION:
      kbr->rotate_chain_device_cb(kbr, msg, &(kbr->rotate_dimension)); 
      ((kbr_wh_t *)data)->h = kbr->rotate_dimension.w;
      ((kbr_wh_t *)data)->w = kbr->rotate_dimension.h;
      return 1;
    case KBR_MSG_SET_CLIP_BOX:
      /* calculate and rotate upper right point of the clip box */
      ((kbr_box_t * )data)->ul.x += ((kbr_box_t * )data)->size.w-1;
      kbr_rotate_270_xy(kbr, &(((kbr_box_t * )data)->ul)); 
      /* finally, swap dimensions */
      {
        kbr_int_t tmp;
        tmp = ((kbr_box_t *)data)->size.w;
        ((kbr_box_t * )data)->size.w = ((kbr_box_t *)data)->size.h;
        ((kbr_box_t * )data)->size.h = tmp;
      }
      break;
    case KBR_MSG_DRAW_PIXEL:
    case KBR_MSG_DRAW_L90FX:
#ifdef KBR_MSG_DRAW_L90TC
    case KBR_MSG_DRAW_L90TC:
#endif /* KBR_MSG_DRAW_L90TC */
#ifdef KBR_MSG_DRAW_L90BF
    case KBR_MSG_DRAW_L90BF:
#endif /* KBR_MSG_DRAW_L90BF */
    case KBR_MSG_DRAW_L90SE:
      kbr->arg.dir+=3;
      kbr->arg.dir&=3;
      kbr_rotate_270_xy(kbr, &(kbr->arg.pixel.pos)); 
      break;
  }
  return kbr->rotate_chain_device_cb(kbr, msg, data);  
}

/* Side-Effects: Update dimension and reset clip range to max */
void kbr_SetRotate270(kbr_t *kbr)
{
  kbr_UndoRotate(kbr);
  kbr->rotate_chain_device_cb = kbr->device_cb;
  kbr->device_cb = kbr_dev_rotate270;
  kbr_GetDimension(kbr);
  kbr_SetMaxClipRange(kbr);
}

