#include "kbr.h"

void kbr_DrawBox(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h)
{
  while( h > 0 )
  {
    kbr_DrawHLine(kbr, x, y, w);
    h--;
    y++;
  }  
}

/*
  - clear the screen with black color
  - reset clip range to max
  - set draw color to white
*/

void kbr_ClearScreen(kbr_t *kbr)
{
  kbr_SetColor(kbr, 0, 0, 0, 0);
  kbr_SetMaxClipRange(kbr);
  kbr_DrawBox(kbr, 0, 0, kbr_GetWidth(kbr), kbr_GetHeight(kbr));
  kbr_SetColor(kbr, 0, 255, 255, 255);
}



void kbr_DrawRBox(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h, kbr_int_t r)
{
  kbr_int_t xl, yu;
  kbr_int_t yl, xr;

  xl = x;
  xl += r;
  yu = y;
  yu += r;
 
  xr = x;
  xr += w;
  xr -= r;
  xr -= 1;
  
  yl = y;
  yl += h;
  yl -= r; 
  yl -= 1;

  kbr_DrawDisc(kbr, xl, yu, r, KBR_DRAW_UPPER_LEFT);
  kbr_DrawDisc(kbr, xr, yu, r, KBR_DRAW_UPPER_RIGHT);
  kbr_DrawDisc(kbr, xl, yl, r, KBR_DRAW_LOWER_LEFT);
  kbr_DrawDisc(kbr, xr, yl, r, KBR_DRAW_LOWER_RIGHT);

  {
    kbr_int_t ww, hh;

    ww = w;
    ww -= r;
    ww -= r;
    ww -= 2;
    hh = h;
    hh -= r;
    hh -= r;
    hh -= 2;
    
    xl++;
    yu++;
    h--;
    kbr_DrawBox(kbr, xl, y, ww, r+1);
    kbr_DrawBox(kbr, xl, yl, ww, r+1);
    kbr_DrawBox(kbr, x, yu, w, hh);
  }
}


kbr_ccs_t kbr_ccs_box[6];	/* color component sliders used by GradientBox */

void kbr_DrawGradientBox(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h)
{
  uint8_t i;
  
  /* Setup ccs for l90se. This will be updated by kbr_clip_l90se if required */
  /* 8. Jan 2014: correct? */

  //printf("%d %d %d\n", kbr->arg.rgb[3].color[0], kbr->arg.rgb[3].color[1], kbr->arg.rgb[3].color[2]);
  
  for ( i = 0; i < 3; i++ )
  {
    kbr_ccs_init(kbr_ccs_box+i, kbr->arg.rgb[0].color[i], kbr->arg.rgb[2].color[i], h);
    kbr_ccs_init(kbr_ccs_box+i+3, kbr->arg.rgb[1].color[i], kbr->arg.rgb[3].color[i], h);
  }
  
  
  while( h > 0 )
  {
    kbr->arg.rgb[0].color[0] = kbr_ccs_box[0].current;
    kbr->arg.rgb[0].color[1] = kbr_ccs_box[1].current;
    kbr->arg.rgb[0].color[2] = kbr_ccs_box[2].current;
    kbr->arg.rgb[1].color[0] = kbr_ccs_box[3].current;
    kbr->arg.rgb[1].color[1] = kbr_ccs_box[4].current;
    kbr->arg.rgb[1].color[2] = kbr_ccs_box[5].current;
    kbr->arg.pixel.pos.x = x;
    kbr->arg.pixel.pos.y = y;
    kbr->arg.len = w;
    kbr->arg.dir = 0;
    kbr_DrawL90SEWithArg(kbr);
    for ( i = 0; i < 6; i++ )
      kbr_ccs_step(kbr_ccs_box+i);
    h--;
    y++;
  }
}


/* restrictions: w > 0 && h > 0 */
void kbr_DrawFrame(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h)
{
  kbr_int_t xtmp = x;
  
  kbr_DrawHLine(kbr, x, y, w);
  kbr_DrawVLine(kbr, x, y, h);
  x+=w;
  x--;
  kbr_DrawVLine(kbr, x, y, h);
  y+=h;
  y--;
  kbr_DrawHLine(kbr, xtmp, y, w);
}

void kbr_DrawRFrame(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h, kbr_int_t r)
{
  kbr_int_t xl, yu;

  xl = x;
  xl += r;
  yu = y;
  yu += r;
 
  {
    kbr_int_t yl, xr;
      
    xr = x;
    xr += w;
    xr -= r;
    xr -= 1;
    
    yl = y;
    yl += h;
    yl -= r; 
    yl -= 1;

    kbr_DrawCircle(kbr, xl, yu, r, KBR_DRAW_UPPER_LEFT);
    kbr_DrawCircle(kbr, xr, yu, r, KBR_DRAW_UPPER_RIGHT);
    kbr_DrawCircle(kbr, xl, yl, r, KBR_DRAW_LOWER_LEFT);
    kbr_DrawCircle(kbr, xr, yl, r, KBR_DRAW_LOWER_RIGHT);
  }

  {
    kbr_int_t ww, hh;

    ww = w;
    ww -= r;
    ww -= r;
    ww -= 2;
    hh = h;
    hh -= r;
    hh -= r;
    hh -= 2;
    
    xl++;
    yu++;
    h--;
    w--;
    kbr_DrawHLine(kbr, xl, y, ww);
    kbr_DrawHLine(kbr, xl, y+h, ww);
    kbr_DrawVLine(kbr, x,         yu, hh);
    kbr_DrawVLine(kbr, x+w, yu, hh);
  }
}
