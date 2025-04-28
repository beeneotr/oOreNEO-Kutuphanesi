#include "kbr.h"


const kbr_pgm_uint8_t kbr_st7735_set_pos_seq[] = 
{
  KBR_CS(0),					/* enable chip */
  KBR_C11( 0x036, 0x000),
  KBR_C10(0x02a),	KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0), KBR_A2(0x000, 0x07f),					/* set x position */
  KBR_C10(0x02b),	KBR_VARY(0,0x00, 0), KBR_VARY(0,0x0ff, 0), KBR_A2(0x000, 0x09f),		/* set y position */
  KBR_C10(0x02c),							/* write to RAM */
  KBR_DATA(),								/* change to data mode */
  KBR_END()
};


const kbr_pgm_uint8_t kbr_st7735_set_pos_dir0_seq[] = 
{
  KBR_CS(0),					/* enable chip */
  
  /* 0x000 horizontal increment (dir = 0) */
  /* 0x000 vertical increment (dir = 1) */
  /* 0x040 horizontal deccrement (dir = 2) */
  /* 0x080 vertical deccrement (dir = 3) */
  KBR_C11( 0x036, 0x000),
  KBR_C10(0x02a),	KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0), KBR_A2(0x000, 0x07f),					/* set x position */
  KBR_C10(0x02b),	KBR_VARY(0,0x00, 0), KBR_VARY(0,0x0ff, 0), KBR_A2(0x000, 0x09f),		/* set y position */

  KBR_C10(0x02c),							/* write to RAM */
  KBR_DATA(),								/* change to data mode */
  KBR_END()
};

const kbr_pgm_uint8_t kbr_st7735_set_pos_dir1_seq[] = 
{
  KBR_CS(0),					/* enable chip */
  /* 0x000 horizontal increment (dir = 0) */
  /* 0x000 vertical increment (dir = 1) */
  /* 0x040 horizontal deccrement (dir = 2) */
  /* 0x080 vertical deccrement (dir = 3) */
  KBR_C11( 0x036, 0x000),
  KBR_C10(0x02a),	KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0), KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0),					/* set x position */
  KBR_C10(0x02b),	KBR_VARY(0,0x00, 0), KBR_VARY(0,0x0ff, 0), KBR_A2(0x000, 0x09f),		/* set y position */

  KBR_C10(0x02c),							/* write to RAM */
  KBR_DATA(),								/* change to data mode */
  KBR_END()
};

const kbr_pgm_uint8_t kbr_st7735_set_pos_dir2_seq[] = 
{
  KBR_CS(0),					/* enable chip */
  
  /* 0x000 horizontal increment (dir = 0) */
  /* 0x000 vertical increment (dir = 1) */
  /* 0x040 horizontal deccrement (dir = 2) */
  /* 0x080 vertical deccrement (dir = 3) */
  
  KBR_C11( 0x036, 0x040),
  KBR_C11( 0x036, 0x040),			/* it seems that this command needs to be sent twice */
  KBR_C10(0x02a),	KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0), KBR_A2(0x000, 0x07f),					/* set x position */
  KBR_C10(0x02b),	KBR_VARY(8,0x01, 0), KBR_VARY(0,0x0ff, 0), KBR_A2(0x000, 0x09f),		/* set y position */

  KBR_C10(0x02c),							/* write to RAM */
  KBR_DATA(),								/* change to data mode */
  KBR_END()
};

const kbr_pgm_uint8_t kbr_st7735_set_pos_dir3_seq[] = 
{
  KBR_CS(0),					/* enable chip */
  
  /* 0x000 horizontal increment (dir = 0) */
  /* 0x000 vertical increment (dir = 1) */
  /* 0x0c0 horizontal deccrement (dir = 2) */
  /* 0x0c0 vertical deccrement (dir = 3) */
  KBR_C11( 0x036, 0x080),
  KBR_C11( 0x036, 0x080),		/* it seems that this command needs to be sent twice */
  KBR_C10(0x02a),	KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0), KBR_VARX(0,0x00, 0), KBR_VARX(0,0x0ff, 0),					/* set x position */
  KBR_C10(0x02b),	KBR_VARY(0,0x00, 0), KBR_VARY(0,0x0ff, 0), KBR_A2(0x000, 0x09f),		/* set y position */

  KBR_C10(0x02c),							/* write to RAM */
  KBR_DATA(),								/* change to data mode */
  KBR_END()
};

kbr_int_t kbr_handle_st7735_l90fx(kbr_t *kbr)
{
  uint8_t c[3];
  kbr_int_t tmp;
  if ( kbr_clip_l90fx(kbr) != 0 )
  {
    switch(kbr->arg.dir)
    {
      case 0: 
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir0_seq);	
	break;
      case 1: 
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir1_seq);	
	break;
      case 2: 
	tmp = kbr->arg.pixel.pos.x;
	kbr->arg.pixel.pos.x = 127-tmp;
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir2_seq);	
	kbr->arg.pixel.pos.x = tmp;
	break;
      case 3: 
      default: 
	tmp = kbr->arg.pixel.pos.y;
	kbr->arg.pixel.pos.y = 159-tmp;
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir3_seq);	
	kbr->arg.pixel.pos.y = tmp;
	break;
    }
    c[0] = kbr->arg.pixel.rgb.color[0];
    c[1] = kbr->arg.pixel.rgb.color[1];
    c[2] = kbr->arg.pixel.rgb.color[2];
    kbr_com_SendRepeat3Bytes(kbr, kbr->arg.len, c);
    kbr_com_SetCSLineStatus(kbr, 1);		/* disable chip */
    return 1;
  }
  return 0;
}

/*
  L2TC (Glyph Output)
  
*/

/* with CmdDataSequence */ 
kbr_int_t kbr_handle_st7735_l90tc(kbr_t *kbr)
{
  if ( kbr_clip_l90tc(kbr) != 0 )
  {
    uint8_t buf[16];
    kbr_int_t dx, dy;
    kbr_int_t i;
    unsigned char pixmap;
    uint8_t bitcnt;
    kbr_com_SetCSLineStatus(kbr, 0);		/* enable chip */
    kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_seq);	

    buf[0] = 0x001;	// change to 0 (cmd mode)
    buf[1] = 0x02a;	// set x
    buf[2] = 0x002;	// change to 1 (arg mode)
    buf[3] = 0x000;	// upper part x
    buf[4] = 0x000;	// no change
    buf[5] = 0x000;	// will be overwritten by x value
    buf[6] = 0x001;	// change to 0 (cmd mode)
    buf[7] = 0x02c;	// write data
    buf[8] = 0x002;	// change to 1 (data mode)
    buf[9] = 0x000;	// red value
    buf[10] = 0x000;	// no change
    buf[11] = 0x000;	// green value
    buf[12] = 0x000;	// no change
    buf[13] = 0x000;	// blue value      
    
    switch(kbr->arg.dir)
    {
      case 0: 
	dx = 1; dy = 0; 
	buf[1] = 0x02a;	// set x
	break;
      case 1: 	
	dx = 0; dy = 1; 
        buf[1] = 0x02b;	// set y
	break;
      case 2: 
	dx = -1; dy = 0; 
        buf[1] = 0x02a;	// set x
	break;
      case 3: 
      default:
	dx = 0; dy = -1; 
        buf[1] = 0x02b;	// set y
	break;
    }
    pixmap = kbr_pgm_read(kbr->arg.bitmap);
    bitcnt = kbr->arg.pixel_skip;
    pixmap <<= bitcnt;
    buf[9] = kbr->arg.pixel.rgb.color[0];
    buf[11] = kbr->arg.pixel.rgb.color[1];
    buf[13] = kbr->arg.pixel.rgb.color[2];
    
    for( i = 0; i < kbr->arg.len; i++ )
    {
      if ( (pixmap & 128) != 0 )
      {
	if ( (kbr->arg.dir&1) == 0 )
	{
	  buf[5] = kbr->arg.pixel.pos.x;
	}
	else
	{
	  buf[3] = kbr->arg.pixel.pos.y>>8;
	  buf[5] = kbr->arg.pixel.pos.y&255;
	}
	kbr_com_SendCmdDataSequence(kbr, 7, buf, 0);
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
    kbr_com_SetCSLineStatus(kbr, 1);		/* disable chip */
    return 1;
  }
  return 0;
}


kbr_int_t kbr_handle_st7735_l90se(kbr_t *kbr)
{
  uint8_t i;
  uint8_t c[3];
  kbr_int_t tmp;
  
  /* Setup ccs for l90se. This will be updated by kbr_clip_l90se if required */
  
  for ( i = 0; i < 3; i++ )
  {
    kbr_ccs_init(kbr->arg.ccs_line+i, kbr->arg.rgb[0].color[i], kbr->arg.rgb[1].color[i], kbr->arg.len);
  }
  
  /* check if the line is visible */
  
  if ( kbr_clip_l90se(kbr) != 0 )
  {
    kbr_int_t i;
    switch(kbr->arg.dir)
    {
      case 0: 
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir0_seq);	
	break;
      case 1: 
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir1_seq);	
	break;
      case 2: 
	tmp = kbr->arg.pixel.pos.x;
	kbr->arg.pixel.pos.x = 127-tmp;
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir2_seq);	
	kbr->arg.pixel.pos.x = tmp;
	break;
      case 3: 
      default: 
	tmp = kbr->arg.pixel.pos.y;
	kbr->arg.pixel.pos.y = 159-tmp;
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_dir3_seq);	
	kbr->arg.pixel.pos.y = tmp;
	break;
    }
    
    for( i = 0; i < kbr->arg.len; i++ )
    {
      c[0] = kbr->arg.ccs_line[0].current;
      c[1] = kbr->arg.ccs_line[1].current; 
      c[2] = kbr->arg.ccs_line[2].current;
      kbr_com_SendRepeat3Bytes(kbr, 1, c);
      kbr_ccs_step(kbr->arg.ccs_line+0);
      kbr_ccs_step(kbr->arg.ccs_line+1);
      kbr_ccs_step(kbr->arg.ccs_line+2);
    }
    kbr_com_SetCSLineStatus(kbr, 1);		/* disable chip */
    return 1;
  }
  return 0;
}

static const kbr_pgm_uint8_t kbr_st7735_power_down_seq[] = {
	KBR_CS(0),					/* enable chip */
	KBR_C10(0x010),				/* sleep in */
	KBR_C10(0x28), 				/* display off */	
	KBR_CS(1),					/* disable chip */
	KBR_END(),					/* end of sequence */
};

kbr_int_t kbr_dev_ic_st7735_18(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_DEV_POWER_UP:
      /* setup com interface and provide information on the clock speed */
      /* of the serial and parallel interface. Values are nanoseconds. */
      return kbr_com_PowerUp(kbr, 100, 66);
    case KBR_MSG_DEV_POWER_DOWN:
      kbr_com_SendCmdSeq(kbr, kbr_st7735_power_down_seq);
      return 1;
    case KBR_MSG_GET_DIMENSION:
      ((kbr_wh_t *)data)->w = 128;
      ((kbr_wh_t *)data)->h = 160;
      return 1;
    case KBR_MSG_DRAW_PIXEL:
      if ( kbr_clip_is_pixel_visible(kbr) !=0 )
      {
	uint8_t c[3];
	kbr_com_SendCmdSeq(kbr, kbr_st7735_set_pos_seq);	
	c[0] = kbr->arg.pixel.rgb.color[0];
	c[1] = kbr->arg.pixel.rgb.color[1];
	c[2] = kbr->arg.pixel.rgb.color[2];
	kbr_com_SendRepeat3Bytes(kbr, 1, c);
	kbr_com_SetCSLineStatus(kbr, 1);		/* disable chip */
      }
      return 1;
    case KBR_MSG_DRAW_L90FX:
      kbr_handle_st7735_l90fx(kbr);
      return 1;
#ifdef KBR_MSG_DRAW_L90TC
    case KBR_MSG_DRAW_L90TC:
      kbr_handle_st7735_l90tc(kbr);
      return 1;	
#endif /* KBR_MSG_DRAW_L90TC */
#ifdef KBR_MSG_DRAW_L90BF
     case KBR_MSG_DRAW_L90BF:
      kbr_handle_l90bf(kbr, kbr_dev_ic_st7735_18);
      return 1;
#endif /* KBR_MSG_DRAW_L90BF */
   
  }
  return kbr_dev_default_cb(kbr, msg, data);  
}

kbr_int_t kbr_ext_st7735_18(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_DRAW_L90SE:
      kbr_handle_st7735_l90se(kbr);
      break;
  }
  return 1;
}