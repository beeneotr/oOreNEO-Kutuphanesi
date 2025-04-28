#include "kbr.h"

int16_t kbr_com_template_cb(kbr_t *kbr, int16_t msg, uint32_t arg, uint8_t *data)
{
  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      break;
    case KBR_COM_MSG_POWER_DOWN:
      break;
    case KBR_COM_MSG_DELAY:
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      break;
    case KBR_COM_MSG_SEND_BYTE:
      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      break;
    case KBR_COM_MSG_SEND_STR:
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      break;
  }
  return 1;
}


void kbr_com_PowerDown(kbr_t *kbr)
{
  if ( (kbr->com_status & KBR_COM_STATUS_MASK_POWER) != 0 )
    kbr->com_cb(kbr, KBR_COM_MSG_POWER_DOWN, 0, NULL);
  kbr->com_status &= ~KBR_COM_STATUS_MASK_POWER;
}

/*
  clk_speed in nano-seconds, range: 0..4095
*/
int16_t kbr_com_PowerUp(kbr_t *kbr, uint16_t serial_clk_speed, uint16_t parallel_clk_speed)
{
  int16_t r;
  kbr_com_info_t com_info;
  com_info.serial_clk_speed = serial_clk_speed;
  com_info.parallel_clk_speed = parallel_clk_speed;
  
  kbr_com_PowerDown(kbr);  
  kbr->com_initial_change_sent = 0;
  r = kbr->com_cb(kbr, KBR_COM_MSG_POWER_UP, 0UL, (uint8_t *)&com_info);
  if ( r != 0 )
  {
    kbr->com_status |= KBR_COM_STATUS_MASK_POWER;
  }
  return r;
}

void kbr_com_SetLineStatus(kbr_t *kbr, uint8_t level, uint8_t mask, uint8_t msg)
{
  if ( level == 0 )
  {
    if ( (kbr->com_initial_change_sent & mask) == 0 || (kbr->com_status & mask) == mask )
    {
      kbr->com_cb(kbr, msg, level, NULL);
      kbr->com_status &= ~mask;
      kbr->com_initial_change_sent |= mask;
    }
  }
  else
  {
    if ( (kbr->com_initial_change_sent & mask) == 0 || (kbr->com_status & mask) == 0 )
    {
      kbr->com_cb(kbr, msg, level, NULL);
      kbr->com_status |= mask;
      kbr->com_initial_change_sent |= mask;
    }
  }
}

void kbr_com_SetResetLineStatus(kbr_t *kbr, uint8_t level)
{
  kbr_com_SetLineStatus(kbr, level, KBR_COM_STATUS_MASK_RESET, KBR_COM_MSG_CHANGE_RESET_LINE);
}

void kbr_com_SetCSLineStatus(kbr_t *kbr, uint8_t level)
{
  kbr_com_SetLineStatus(kbr, level, KBR_COM_STATUS_MASK_CS, KBR_COM_MSG_CHANGE_CS_LINE);
}

void kbr_com_SetCDLineStatus(kbr_t *kbr, uint8_t level)
{
  kbr_com_SetLineStatus(kbr, level, KBR_COM_STATUS_MASK_CD, KBR_COM_MSG_CHANGE_CD_LINE);
}

/* delay in microseconds */
void kbr_com_DelayMicroseconds(kbr_t *kbr, uint16_t delay)
{
  kbr->com_cb(kbr, KBR_COM_MSG_DELAY, delay, NULL);
}

/* delay in milliseconds */
void kbr_com_DelayMilliseconds(kbr_t *kbr, uint16_t delay)
{
  while( delay > 0 )
  {
    kbr_com_DelayMicroseconds(kbr, 1000);
    delay--;
  }
}


#ifndef kbr_com_SendByte
void kbr_com_SendByte(kbr_t *kbr, uint8_t byte)
{
  kbr->com_cb(kbr, KBR_COM_MSG_SEND_BYTE, byte, NULL);
}
#endif

void kbr_com_SendRepeatByte(kbr_t *kbr, uint16_t cnt, uint8_t byte)
{
  kbr->com_cb(kbr, KBR_COM_MSG_REPEAT_1_BYTE, cnt, &byte);
}

void kbr_com_SendRepeat2Bytes(kbr_t *kbr, uint16_t cnt, uint8_t *byte_ptr)
{
  kbr->com_cb(kbr, KBR_COM_MSG_REPEAT_2_BYTES, cnt, byte_ptr);
}


#ifndef kbr_com_SendRepeat3Bytes
void kbr_com_SendRepeat3Bytes(kbr_t *kbr, uint16_t cnt, uint8_t *byte_ptr)
{
  kbr->com_cb(kbr, KBR_COM_MSG_REPEAT_3_BYTES, cnt, byte_ptr);
}
#endif

void kbr_com_SendString(kbr_t *kbr, uint16_t cnt, const uint8_t *byte_ptr)
{
  kbr->com_cb(kbr, KBR_COM_MSG_SEND_STR, cnt, (uint8_t *)byte_ptr);
}

void kbr_com_SendStringP(kbr_t *kbr, uint16_t cnt, const kbr_pgm_uint8_t *byte_ptr)
{
  uint8_t b;
  while( cnt > 0 )
  {
    b = kbr_pgm_read(byte_ptr);
    kbr->com_cb(kbr, KBR_COM_MSG_SEND_BYTE, b, NULL);
    byte_ptr++;
    cnt--;
  }
}


void kbr_com_SendCmdDataSequence(kbr_t *kbr, uint16_t cnt, const uint8_t *byte_ptr, uint8_t cd_line_status_at_end)
{
  kbr->com_cb(kbr, KBR_COM_MSG_SEND_CD_DATA_SEQUENCE, cnt, (uint8_t *)byte_ptr);
  kbr_com_SetCDLineStatus(kbr, cd_line_status_at_end);	// ensure that the status is set correctly for the CD line */
}

static void kbr_com_SendCmdArg(kbr_t *kbr, const kbr_pgm_uint8_t *data, uint8_t cmd_cnt, uint8_t arg_cnt)
{
  kbr_com_SetCDLineStatus(kbr, (kbr->com_cfg_cd>>1)&1 );
  kbr_com_SendStringP(kbr, cmd_cnt, data);
  if ( arg_cnt > 0 )
  {
    data += cmd_cnt;
    kbr_com_SetCDLineStatus(kbr, (kbr->com_cfg_cd)&1 );
    kbr_com_SendStringP(kbr, arg_cnt, data);
  }
}

void kbr_com_SendCmdSeq(kbr_t *kbr, const kbr_pgm_uint8_t *data)
{
  uint8_t b;
  uint8_t bb;
  uint8_t hi;
  uint8_t lo;

  for(;;)
  {
    b = kbr_pgm_read(data);
    hi = (b) >> 4;
    lo = (b) & 0x0f;
    switch( hi )
    {
      case 0:
	return;		/* end marker */
      case 1:
      case 2:
      case 3:
	kbr_com_SendCmdArg(kbr, data+1, hi, lo);
	data+=1+hi+lo;
	break;
      case 6:
	kbr_com_SetCDLineStatus(kbr, (kbr->com_cfg_cd)&1 );
	kbr_com_SendStringP(kbr, lo, data+1);
	data+=1+lo;      
	break;
      case 7:	/* note: 0x070 is used to set data line status */
	kbr_com_SetCDLineStatus(kbr, ((kbr->com_cfg_cd>>1)&1)^1 );
	if ( lo > 0 )
	  kbr_com_SendStringP(kbr, lo, data+1);
	data+=1+lo;      
	break;
      case 8:
	data++;
	b = kbr_pgm_read(data);
	kbr_com_DelayMilliseconds(kbr, (((uint16_t)lo)<<8) + b );
	data++;
	break;
      case 9:
	data++;
	b = kbr_pgm_read(data);
	kbr_com_DelayMicroseconds(kbr, (((uint16_t)lo)<<8) + b );
	data++;
	break;
      case 10:
	data++;
	b = kbr_pgm_read(data);
	data++;
	bb = kbr_pgm_read(data);
	data++;
	kbr_com_SetCDLineStatus(kbr, (kbr->com_cfg_cd)&1 );
	kbr_com_SendByte(kbr, (((uint8_t)(((kbr->arg.pixel.pos.x)>>lo)))&b)|bb );
	break;
      case 11:
	data++;
	b = kbr_pgm_read(data);
	data++;
	bb = kbr_pgm_read(data);
	data++;
	kbr_com_SetCDLineStatus(kbr, (kbr->com_cfg_cd)&1 );
	kbr_com_SendByte(kbr, (((uint8_t)(((kbr->arg.pixel.pos.y)>>lo)))&b)|bb );
	break;
      case 15:
	hi = lo >> 2;
	lo &= 3;
	switch(hi)
	{
	  case 0:
	    kbr_com_SetResetLineStatus(kbr, lo&1);
	    break;
	  case 1:
	    kbr_com_SetCSLineStatus(kbr, lo&1);
	    break;
	  case 3:
	    kbr->com_cfg_cd = lo;
	    break;
	}
	data++;
	break;
      default:
	return;
    }  
  }
}

