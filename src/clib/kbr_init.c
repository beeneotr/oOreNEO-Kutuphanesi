#include "kbr.h"
#include <string.h> 	/* memset */

#ifdef __AVR__
uint8_t global_SREG_backup;		// used by the atomic macros
#endif


void kbr_init_struct(kbr_t *kbr)
{
  kbr->is_power_up = 0;
  kbr->rotate_chain_device_cb = 0;
  kbr->arg.scale = 1;
  kbr->font = 0;
  kbr->font_decode.is_transparent = 1;  // new font procedures
  
  kbr->com_initial_change_sent = 0;
  kbr->com_status = 0;
  kbr->com_cfg_cd = 0;
}


kbr_int_t kbr_Init(kbr_t *kbr, kbr_dev_fnptr device_cb, kbr_dev_fnptr ext_cb, kbr_com_fnptr com_cb)
{
  kbr_int_t r;
  kbr_init_struct(kbr);
  if ( ext_cb == (kbr_dev_fnptr)0 )
    kbr->ext_cb = kbr_ext_none;
 else 
    kbr->ext_cb = ext_cb;
  kbr->device_cb = device_cb;
  kbr->com_cb = com_cb;
  kbr_SetFontPosBaseline(kbr);
  r = kbr_PowerUp(kbr);
  kbr_GetDimension(kbr);
  return r;
}

