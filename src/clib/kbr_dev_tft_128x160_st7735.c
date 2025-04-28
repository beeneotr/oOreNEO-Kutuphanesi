#include "kbr.h"

//static const uint8_t kbr_dev_ssd1351_128x128_init_seq[] PROGMEM = {
static const kbr_pgm_uint8_t kbr_tft_128x160_st7735_init_seq[] = {
  KBR_CFG_CD(0,1),				/* DC=0 for command mode, DC=1 for data and args */
  KBR_RST(1),					
  KBR_CS(1),					/* disable chip */
  KBR_DLY_MS(5),
  KBR_RST(0),					
  KBR_DLY_MS(5),
  KBR_RST(1),
  KBR_DLY_MS(50),
  KBR_CS(0),					/* enable chip */
  
  KBR_C10(0x011),				/* sleep out */
  KBR_DLY_MS(10),
  KBR_C10(0x013),				/* normal display on */

  KBR_C10(0x20), 				/* not inverted */

  KBR_C11(0x03a, 0x006), 		/* set pixel format to 18 bit */
  KBR_C10(0x029), 				/* display on */

  KBR_C11( 0x036, 0x000),		/* memory control */
  
  KBR_C14(  0x02a, 0x000, 0x000, 0x000, 0x07f),              /* Horizontal GRAM Address Set */
  KBR_C14(  0x02b, 0x000, 0x000, 0x000, 0x09f),              /* Vertical GRAM Address Set */
  KBR_C10(  0x02c),               /* Write Data to GRAM */

  
  KBR_CS(1),					/* disable chip */
  KBR_END(),					/* end of sequence */
};

kbr_int_t kbr_dev_st7735_18x128x160(kbr_t *kbr, kbr_int_t msg, void *data)
{
  switch(msg)
  {
    case KBR_MSG_DEV_POWER_UP:
      /* 1. Call to the controller procedures to setup the com interface */
      if ( kbr_dev_ic_st7735_18(kbr, msg, data) == 0 )
	return 0;

      /* 2. Send specific init sequence for this display module */
      kbr_com_SendCmdSeq(kbr, kbr_tft_128x160_st7735_init_seq);
      
      return 1;
      
    case KBR_MSG_DEV_POWER_DOWN:
      /* let do power down by the conroller procedures */
      return kbr_dev_ic_st7735_18(kbr, msg, data);  
    
    case KBR_MSG_GET_DIMENSION:
      ((kbr_wh_t *)data)->w = 128;
      ((kbr_wh_t *)data)->h = 160;
      return 1;
  }
  
  /* all other messages are handled by the controller procedures */
  return kbr_dev_ic_st7735_18(kbr, msg, data);  
}
