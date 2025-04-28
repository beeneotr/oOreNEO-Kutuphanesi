#include <SPI.h>

#include "ooreneo.h"

/*=========================================================================*/
/* 8 Bit SW SPI */

#if  defined(__SAM3X8E__)
//#elif defined(__SAM3X8E__)

//#define setbit(pio, mask) PIO_Set( (pio), (mask) )
//#define clrbit(pio, mask) PIO_Clear( (pio), (mask) )

#define setbit(pio, mask) ((pio)->PIO_SODR = (mask))
#define clrbit(pio, mask) ((pio)->PIO_CODR = (mask))

static void kbr_nano_delay(void)
{
  volatile uint32_t i;
  for( i = 0; i < 1; i++ )
  {
    __NOP;
  }
  //delayMicroseconds(1);
}

static void kbr_com_arduino_send_generic_SW_SPI(kbr_t *kbr, uint8_t data)
{
  uint32_t sda_pin = kbr->pin_list[KBR_PIN_SDA];
  uint32_t scl_pin = kbr->pin_list[KBR_PIN_SCL];
  Pio *sda_port = g_APinDescription[sda_pin].pPort;
  Pio *scl_port = g_APinDescription[scl_pin].pPort;
  uint8_t i = 8;
  sda_pin = g_APinDescription[sda_pin].ulPin;
  scl_pin = g_APinDescription[scl_pin].ulPin;

  do
  {
    if ( data & 128 )
    {
      setbit( sda_port, sda_pin) ;
    }
    else
    {
      clrbit( sda_port, sda_pin) ;
    }
    //delayMicroseconds(1);
    kbr_nano_delay();
    setbit( scl_port, scl_pin);
    //delayMicroseconds(1);
    kbr_nano_delay();
    i--;
    clrbit( scl_port, scl_pin) ;
    data <<= 1;
  } while( i > 0 );
  
}

#elif defined(__AVR__)

uint8_t u8g_bitData, u8g_bitNotData;
uint8_t u8g_bitClock, u8g_bitNotClock;
volatile uint8_t *u8g_outData;
volatile uint8_t *u8g_outClock;

static void kbr_com_arduino_init_shift_out(uint8_t dataPin, uint8_t clockPin)
{
  u8g_outData = portOutputRegister(digitalPinToPort(dataPin));
  u8g_outClock = portOutputRegister(digitalPinToPort(clockPin));
  u8g_bitData = digitalPinToBitMask(dataPin);
  u8g_bitClock = digitalPinToBitMask(clockPin);

  u8g_bitNotClock = u8g_bitClock;
  u8g_bitNotClock ^= 0x0ff;

  u8g_bitNotData = u8g_bitData;
  u8g_bitNotData ^= 0x0ff;
}


static void kbr_com_arduino_send_generic_SW_SPI(kbr_t *kbr, uint8_t val) KBR_NOINLINE;
static void kbr_com_arduino_send_generic_SW_SPI(kbr_t *kbr, uint8_t val)
{
  uint8_t cnt = 8;
  uint8_t bitData = u8g_bitData;
  uint8_t bitNotData = u8g_bitNotData;
  uint8_t bitClock = u8g_bitClock;
  uint8_t bitNotClock = u8g_bitNotClock;
  volatile uint8_t *outData = u8g_outData;
  volatile uint8_t *outClock = u8g_outClock;
  
  KBR_ATOMIC_START();
  do
  {
    if ( val & 128 )
      *outData |= bitData;
    else
      *outData &= bitNotData;
   
    *outClock |= bitClock;
    val <<= 1;
    cnt--;
    *outClock &= bitNotClock;
  } while( cnt != 0 );
  KBR_ATOMIC_END();
  
}

#else

static void kbr_com_arduino_send_generic_SW_SPI(kbr_t *kbr, uint8_t data)
{
  uint8_t i = 8;
  
  do
  {
    if ( data & 128 )
    {
      digitalWrite(kbr->pin_list[KBR_PIN_SDA], 1 );
    }
    else
    {
      digitalWrite(kbr->pin_list[KBR_PIN_SDA], 0 );
    }
    // no delay required, also Arduino Due is slow enough
    // delay required for ESP32
    delayMicroseconds(1);
    digitalWrite(kbr->pin_list[KBR_PIN_SCL], 1 );
    delayMicroseconds(1);
    i--;
    digitalWrite(kbr->pin_list[KBR_PIN_SCL], 0 );
    delayMicroseconds(1);
    data <<= 1;
  } while( i > 0 );
  
}

#endif

static int16_t kbr_com_arduino_generic_SW_SPI(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data)
{

  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      /* "data" is a pointer to kbr_com_info_t structure with the following information: */
      /*	((kbr_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((kbr_com_info_t *)data)->parallel_clk_speed value in nanoseconds */

#ifdef __AVR__
      kbr_com_arduino_init_shift_out(kbr->pin_list[KBR_PIN_SDA], kbr->pin_list[KBR_PIN_SCL]);
#endif
    
      /* setup pins */
      pinMode(kbr->pin_list[KBR_PIN_CD], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_SDA], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_SCL], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_CS], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_RST], OUTPUT);

      digitalWrite(kbr->pin_list[KBR_PIN_CD], 1);
      digitalWrite(kbr->pin_list[KBR_PIN_SDA], 1);
      digitalWrite(kbr->pin_list[KBR_PIN_SCL], 0);
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], 1);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], 1);

      break;
    case KBR_COM_MSG_POWER_DOWN:
      break;
    case KBR_COM_MSG_DELAY:
      delayMicroseconds(arg);
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], arg);
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
#ifdef __AVR__
      kbr_com_arduino_init_shift_out(kbr->pin_list[KBR_PIN_SDA], kbr->pin_list[KBR_PIN_SCL]);
#endif    
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], arg);      
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      digitalWrite(kbr->pin_list[KBR_PIN_CD], arg);
      break;
    case KBR_COM_MSG_SEND_BYTE:
      kbr_com_arduino_send_generic_SW_SPI(kbr, arg);
      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	kbr_com_arduino_send_generic_SW_SPI(kbr, data[0]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_generic_SW_SPI(kbr, data[0]);
	kbr_com_arduino_send_generic_SW_SPI(kbr, data[1]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_generic_SW_SPI(kbr, data[0]);
	kbr_com_arduino_send_generic_SW_SPI(kbr, data[1]);
	kbr_com_arduino_send_generic_SW_SPI(kbr, data[2]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_STR:
      while( arg > 0 ) {
	kbr_com_arduino_send_generic_SW_SPI(kbr, *data++);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      while(arg > 0)
      {
	if ( *data != 0 )
	{
	  if ( *data == 1 )
	  {
	    digitalWrite(kbr->pin_list[KBR_PIN_CD], 0);
	  }
	  else
	  {
	    digitalWrite(kbr->pin_list[KBR_PIN_CD], 1);
	  }
	}
	data++;
	kbr_com_arduino_send_generic_SW_SPI(kbr, *data);
	data++;
	arg--;
      }
      break;
  }
  return 1;
}

void kbrlib4WireSWSPI::begin(uint8_t is_transparent)
{ 
  kbr_Init(&kbr, dev_cb, ext_cb, kbr_com_arduino_generic_SW_SPI); 
  kbr_SetFontMode(&kbr, is_transparent);
}

/*=========================================================================*/
/* 9 Bit SW SPI */

static void kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr_t *kbr, uint8_t first_bit, uint8_t data)
{
  uint8_t i;

  if ( first_bit != 0 )
  {
    digitalWrite(kbr->pin_list[KBR_PIN_SDA], 1 );
  }
  else
  {
    digitalWrite(kbr->pin_list[KBR_PIN_SDA], 0 );
  }
  // no delay required, also Arduino Due is slow enough
  //delayMicroseconds(1);
  digitalWrite(kbr->pin_list[KBR_PIN_SCL], 1 );
  //delayMicroseconds(1);
  digitalWrite(kbr->pin_list[KBR_PIN_SCL], 0 );
  //delayMicroseconds(1);

  i = 8;
  do
  {
    if ( data & 128 )
    {
      digitalWrite(kbr->pin_list[KBR_PIN_SDA], 1 );
    }
    else
    {
      digitalWrite(kbr->pin_list[KBR_PIN_SDA], 0 );
    }
    // no delay required, also Arduino Due is slow enough
    //delayMicroseconds(1);
    digitalWrite(kbr->pin_list[KBR_PIN_SCL], 1 );
    //delayMicroseconds(1);
    i--;
    digitalWrite(kbr->pin_list[KBR_PIN_SCL], 0 );
    //delayMicroseconds(1);
    data <<= 1;
  } while( i > 0 );
  
}

static int16_t kbr_com_arduino_3wire_9bit_SW_SPI(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data)
{

  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      /* "data" is a pointer to kbr_com_info_t structure with the following information: */
      /*	((kbr_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((kbr_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
      
      /* setup pins */
      pinMode(kbr->pin_list[KBR_PIN_SDA], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_SCL], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_CS], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_RST], OUTPUT);

      digitalWrite(kbr->pin_list[KBR_PIN_SDA], 1);
      digitalWrite(kbr->pin_list[KBR_PIN_SCL], 0);
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], 1);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], 1);

      break;
    case KBR_COM_MSG_POWER_DOWN:
      break;
    case KBR_COM_MSG_DELAY:
      delayMicroseconds(arg);
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], arg);
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], arg);
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      /* ignored, there is not CD line */
      break;
    case KBR_COM_MSG_SEND_BYTE:
      kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, arg);
      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[0]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[0]);
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[1]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[0]);
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[1]);
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[2]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_STR:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, *data++);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      {
	uint8_t last_cd = kbr->com_status &KBR_COM_STATUS_MASK_CD;
	while(arg > 0)
	{
	  if ( *data != 0 )
	  {
	    if ( *data == 1 )
	    {
	      last_cd = 0;
	    }
	    else
	    {
	      last_cd = 1;
	    }
	  }
	  data++;
	  kbr_com_arduino_send_3wire_9bit_SW_SPI(kbr, last_cd, *data); 
	  data++;
	  arg--;
	}
      }
      break;
  }
  return 1;
}

void kbrlib3Wire9bitSWSPI::begin(uint8_t is_transparent)
{ 
  kbr_Init(&kbr, dev_cb, ext_cb, kbr_com_arduino_3wire_9bit_SW_SPI); 
  kbr_SetFontMode(&kbr, is_transparent);
}

/*=========================================================================*/
/* 9 Bit HW SPI */

#define KBR_COM_ARDUINO_3WIRE_8BIT_BUF_LEN 9
static uint8_t kbr_com_3wire_9bit_buffer[KBR_COM_ARDUINO_3WIRE_8BIT_BUF_LEN];
static uint8_t kbr_com_3wire_9bit_buf_bytepos;
static uint8_t kbr_com_3wire_9bit_buf_bitpos;
static uint8_t kbr_com_3wire_9bit_cd_mask;

static void kbr_com_arduino_init_3wire_9bit_HW_SPI(kbr_t *kbr) KBR_NOINLINE;
static void kbr_com_arduino_init_3wire_9bit_HW_SPI(kbr_t *kbr)
{
  uint8_t i;
  kbr_com_3wire_9bit_buf_bytepos = 0;
  kbr_com_3wire_9bit_buf_bitpos = 7;
  kbr_com_3wire_9bit_cd_mask = 128;
  for( i = 0; i < KBR_COM_ARDUINO_3WIRE_8BIT_BUF_LEN; i++ )
    kbr_com_3wire_9bit_buffer[i] = 0; /* this is also the NOP command for the PCF8833 */
}

static void kbr_com_arduino_flush_3wire_9bit_HW_SPI(kbr_t *kbr) KBR_NOINLINE;
static void kbr_com_arduino_flush_3wire_9bit_HW_SPI(kbr_t *kbr)
{
  uint8_t i;
  if ( kbr_com_3wire_9bit_buf_bytepos == 0 && kbr_com_3wire_9bit_buf_bitpos == 7 )
    return;
  
  for( i = 0; i < KBR_COM_ARDUINO_3WIRE_8BIT_BUF_LEN; i++ )
    SPI.transfer(kbr_com_3wire_9bit_buffer[i] );
  
  kbr_com_arduino_init_3wire_9bit_HW_SPI(kbr);
}

static void kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr_t *kbr, uint8_t first_bit, uint8_t data)
{
  
  if ( first_bit != 0 )
    kbr_com_3wire_9bit_buffer[kbr_com_3wire_9bit_buf_bytepos] |= kbr_com_3wire_9bit_cd_mask;
  
  if ( kbr_com_3wire_9bit_buf_bitpos > 0 )
  {
    kbr_com_3wire_9bit_buf_bitpos--;
    kbr_com_3wire_9bit_cd_mask >>= 1;
  }
  else
  {
    kbr_com_3wire_9bit_buf_bitpos = 7;
    kbr_com_3wire_9bit_buf_bytepos++;
    kbr_com_3wire_9bit_cd_mask = 128;
  }
  
  kbr_com_3wire_9bit_buffer[kbr_com_3wire_9bit_buf_bytepos] |=  data >> (7-kbr_com_3wire_9bit_buf_bitpos);

  if ( kbr_com_3wire_9bit_buf_bitpos == 7 )
  {
    kbr_com_3wire_9bit_buf_bytepos++;
    if ( kbr_com_3wire_9bit_buf_bytepos >= KBR_COM_ARDUINO_3WIRE_8BIT_BUF_LEN )
      kbr_com_arduino_flush_3wire_9bit_HW_SPI(kbr);      
  }
  else
  {
    kbr_com_3wire_9bit_buf_bytepos++;
    kbr_com_3wire_9bit_buffer[kbr_com_3wire_9bit_buf_bytepos] |=  data << (kbr_com_3wire_9bit_buf_bitpos+1);
  }
}

static int16_t kbr_com_arduino_3wire_9bit_HW_SPI(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data)
{

  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      /* "data" is a pointer to kbr_com_info_t structure with the following information: */
      /*	((kbr_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((kbr_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
      
      kbr_com_arduino_init_3wire_9bit_HW_SPI(kbr);
    
      /* setup pins */
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_CS], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_RST], OUTPUT);

      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], 1);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], 1);

      /* setup Arduino SPI */
#if ARDUINO >= 10600
      SPI.begin();
      SPI.beginTransaction(SPISettings(1000000000UL/((kbr_com_info_t *)data)->serial_clk_speed, MSBFIRST, SPI_MODE0));
#else
      SPI.begin();
      
      if ( ((kbr_com_info_t *)data)->serial_clk_speed/2 < 70 )
	SPI.setClockDivider( SPI_CLOCK_DIV2 );
      else if ( ((kbr_com_info_t *)data)->serial_clk_speed/2 < 140 )
	SPI.setClockDivider( SPI_CLOCK_DIV4 );
      else
	SPI.setClockDivider( SPI_CLOCK_DIV8 );
      SPI.setDataMode(SPI_MODE0);
      SPI.setBitOrder(MSBFIRST);
#endif
      break;
    case KBR_COM_MSG_POWER_DOWN:
#if ARDUINO >= 10600
      SPI.endTransaction();
      SPI.end();
#else
      SPI.end();
#endif
      break;
    case KBR_COM_MSG_DELAY:
      /* flush pending data first, then do the delay */
      kbr_com_arduino_flush_3wire_9bit_HW_SPI(kbr);      
      delayMicroseconds(arg);
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], arg);
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
      if ( arg != 0 )
	kbr_com_arduino_flush_3wire_9bit_HW_SPI(kbr);      
      
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], arg);
      
      if ( arg == 0 )
	kbr_com_arduino_init_3wire_9bit_HW_SPI(kbr);
      
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      /* not used */
      break;
    case KBR_COM_MSG_SEND_BYTE:
      kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, arg);
      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[0]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[0]);
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[1]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[0]);
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[1]);
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, data[2]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_STR:
      while( arg > 0 ) {
	kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, kbr->com_status &KBR_COM_STATUS_MASK_CD, *data++);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      {
	uint8_t last_cd = kbr->com_status &KBR_COM_STATUS_MASK_CD;
	while(arg > 0)
	{
	  if ( *data != 0 )
	  {
	    if ( *data == 1 )
	    {
	      last_cd = 0;
	    }
	    else
	    {
	      last_cd = 1;
	    }
	  }
	  data++;
	  kbr_com_arduino_send_3wire_9bit_HW_SPI(kbr, last_cd, *data); 
	  data++;
	  arg--;
	}
      }
      break;
  }
  return 1;
}

void kbrlib3Wire9bitHWSPI::begin(uint8_t is_transparent)
{ 
  kbr_Init(&kbr, dev_cb, ext_cb, kbr_com_arduino_3wire_9bit_HW_SPI); 
  kbr_SetFontMode(&kbr, is_transparent);
}


/*=========================================================================*/
/* 8 Bit Parallel */


#if defined(__PIC32MX) || defined(__arm__) || defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266) || defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32)
/* CHIPKIT PIC32 */
static volatile uint32_t *u8g_data_port[9];
static uint32_t u8g_data_mask[9];
#else
static volatile uint8_t *u8g_data_port[9];
static uint8_t u8g_data_mask[9];
#endif

static void kbr_com_arduino_init_8bit(kbr_t *kbr)
{

  u8g_data_port[0] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D0]));
  u8g_data_mask[0] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D0]);  
  
  u8g_data_port[1] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D1]));
  u8g_data_mask[1] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D1]);  
  
  u8g_data_port[2] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D2]));
  u8g_data_mask[2] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D2]);  
  
  u8g_data_port[3] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D3]));
  u8g_data_mask[3] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D3]);  
  
  u8g_data_port[4] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D4]));
  u8g_data_mask[4] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D4]);  
  
  u8g_data_port[5] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D5]));
  u8g_data_mask[5] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D5]);  
  
  if ( kbr->pin_list[KBR_PIN_D6] != KBR_PIN_VAL_NONE )
  {
    u8g_data_port[6] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D6]));
    u8g_data_mask[6] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D6]);  
  }
  
  if ( kbr->pin_list[KBR_PIN_D7] != KBR_PIN_VAL_NONE )
  {
    u8g_data_port[7] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_D7]));
    u8g_data_mask[7] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_D7]);  
  }  

  u8g_data_port[8] =  portOutputRegister(digitalPinToPort(kbr->pin_list[KBR_PIN_WR]));
  u8g_data_mask[8] =  digitalPinToBitMask(kbr->pin_list[KBR_PIN_WR]);  
  
}

static void kbr_com_arduino_send_8bit(kbr_t *kbr, uint8_t data)
{
  int i;
  #if defined(__arm__)
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
#elif defined(__AVR__)
#else
  delayMicroseconds(1);
#endif
  for( i = 0; i < 8; i++ )
  {
    if ( data & 1 )
      *u8g_data_port[i] |= u8g_data_mask[i]; 
    else
      *u8g_data_port[i] &= ~u8g_data_mask[i]; 
    data >>= 1;
  }

  #if defined(__arm__)
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  delayMicroseconds(1);
#elif defined(__AVR__)
#else
  delayMicroseconds(1);
#endif
  
  *u8g_data_port[8] &= ~u8g_data_mask[8]; 
  
#if defined(__arm__)
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  __NOP;
  delayMicroseconds(1);
#elif defined(__AVR__)
#else
  delayMicroseconds(1);
#endif
  
  *u8g_data_port[8] |= u8g_data_mask[8]; 
}

static int16_t kbr_com_arduino_generic_8bit(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data)
{
  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      /* "data" is a pointer to kbr_com_info_t structure with the following information: */
      /*	((kbr_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((kbr_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
      
      /* setup pins */
      pinMode(kbr->pin_list[KBR_PIN_CD], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_WR], OUTPUT);
    
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_CS], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_RST], OUTPUT);

      pinMode(kbr->pin_list[KBR_PIN_D0], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_D1], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_D2], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_D3], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_D4], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_D5], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_D6] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_D6], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_D7] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_D7], OUTPUT);

      digitalWrite(kbr->pin_list[KBR_PIN_CD], 1);
      digitalWrite(kbr->pin_list[KBR_PIN_WR], 1);
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], 1);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], 1);

      kbr_com_arduino_init_8bit(kbr);
      
      break;
    case KBR_COM_MSG_POWER_DOWN:
      break;
    case KBR_COM_MSG_DELAY:
      delayMicroseconds(arg);
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], arg);
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], arg);
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      digitalWrite(kbr->pin_list[KBR_PIN_CD], arg);
      break;
    case KBR_COM_MSG_SEND_BYTE:
      kbr_com_arduino_send_8bit(kbr, arg);
      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	kbr_com_arduino_send_8bit(kbr, data[0]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_8bit(kbr, data[0]);
	kbr_com_arduino_send_8bit(kbr, data[1]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_send_8bit(kbr, data[0]);
	kbr_com_arduino_send_8bit(kbr, data[1]);
	kbr_com_arduino_send_8bit(kbr, data[2]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_STR:
      while( arg > 0 ) {
	kbr_com_arduino_send_8bit(kbr, *data++);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      while(arg > 0)
      {
	if ( *data != 0 )
	{
	  if ( *data == 1 )
	  {
	    digitalWrite(kbr->pin_list[KBR_PIN_CD], 0);
	  }
	  else
	  {
	    digitalWrite(kbr->pin_list[KBR_PIN_CD], 1);
	  }
	}
	data++;
	kbr_com_arduino_send_8bit(kbr, *data);
	data++;
	arg--;
      }
      break;
  }
  return 1;
}

void kbrlib8Bit::begin(uint8_t is_transparent)
{ 
  kbr_Init(&kbr, dev_cb, ext_cb, kbr_com_arduino_generic_8bit); 
  kbr_SetFontMode(&kbr, is_transparent);
}


/*=========================================================================*/
/* 8 Bit Parallel on Port D of AVR controller */

#ifdef __AVR__

static void kbr_com_arduino_port_d_send(uint8_t data, volatile uint8_t *port, uint8_t and_mask, uint8_t or_mask)
{
    PORTD = data;
    *port &= and_mask;
    *port |= or_mask;
}

static int16_t kbr_com_arduino_port_d(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data)
{
  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      /* "data" is a pointer to kbr_com_info_t structure with the following information: */
      /*	((kbr_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((kbr_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
      
      /* setup pins */
      pinMode(kbr->pin_list[KBR_PIN_CD], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_WR], OUTPUT);
      
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_CS], OUTPUT);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_RST], OUTPUT);

      pinMode(0, OUTPUT);
      pinMode(1, OUTPUT);
      pinMode(2, OUTPUT);
      pinMode(3, OUTPUT);
      pinMode(4, OUTPUT);
      pinMode(5, OUTPUT);
      pinMode(6, OUTPUT);
      pinMode(7, OUTPUT);

      digitalWrite(kbr->pin_list[KBR_PIN_CD], 1);
      digitalWrite(kbr->pin_list[KBR_PIN_WR], 1);
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], 1);
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], 1);

      break;
    case KBR_COM_MSG_POWER_DOWN:
      break;
    case KBR_COM_MSG_DELAY:
      delayMicroseconds(arg);
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], arg);
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], arg);
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      if ( arg == 0 )
	*kbr->data_port[KBR_PIN_CD] &= ~kbr->data_mask[KBR_PIN_CD];
      else
	*kbr->data_port[KBR_PIN_CD] |= kbr->data_mask[KBR_PIN_CD];
      break;
    case KBR_COM_MSG_SEND_BYTE:
      kbr_com_arduino_port_d_send(arg, kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);

      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	kbr_com_arduino_port_d_send(data[0], kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_port_d_send(data[0], kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	kbr_com_arduino_port_d_send(data[1], kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
	kbr_com_arduino_port_d_send(data[0], kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	kbr_com_arduino_port_d_send(data[1], kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	kbr_com_arduino_port_d_send(data[2], kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_STR:
      while( arg > 0 ) {
	kbr_com_arduino_port_d_send(*data++, kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      while(arg > 0)
      {
	if ( *data != 0 )
	{
	  if ( *data == 1 )
	  {
	    *kbr->data_port[KBR_PIN_CD] &= ~kbr->data_mask[KBR_PIN_CD];
	  }
	  else
	  {
	    *kbr->data_port[KBR_PIN_CD] |= kbr->data_mask[KBR_PIN_CD];
	  }
	}
	data++;
	kbr_com_arduino_port_d_send(*data, kbr->data_port[KBR_PIN_WR], ~kbr->data_mask[KBR_PIN_WR], kbr->data_mask[KBR_PIN_WR]);	
	data++;
	arg--;
      }
      break;
  }
  return 1;
}

void kbrlib8BitPortD::begin(uint8_t is_transparent)
{ 
  kbr_Init(&kbr, dev_cb, ext_cb, kbr_com_arduino_port_d); 
  kbr_SetFontMode(&kbr, is_transparent);
}

#endif /* __AVR__ */

/*=========================================================================*/

static int16_t kbr_com_arduino_4wire_HW_SPI(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data)
{
  switch(msg)
  {
    case KBR_COM_MSG_POWER_UP:
      /* "data" is a pointer to kbr_com_info_t structure with the following information: */
      /*	((kbr_com_info_t *)data)->serial_clk_speed value in nanoseconds */
      /*	((kbr_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
      
      /* setup pins */
    
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_RST], OUTPUT);
      pinMode(kbr->pin_list[KBR_PIN_CD], OUTPUT);
      
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	pinMode(kbr->pin_list[KBR_PIN_CS], OUTPUT);
      
      /* setup Arduino SPI */

#if ARDUINO >= 10600
      SPI.begin();
      SPI.beginTransaction(SPISettings(1000000000UL/((kbr_com_info_t *)data)->serial_clk_speed, MSBFIRST, SPI_MODE0));
#else
      SPI.begin();
      
      if ( ((kbr_com_info_t *)data)->serial_clk_speed/2 < 70 )
	SPI.setClockDivider( SPI_CLOCK_DIV2 );
      else if ( ((kbr_com_info_t *)data)->serial_clk_speed/2 < 140 )
	SPI.setClockDivider( SPI_CLOCK_DIV4 );
      else
	SPI.setClockDivider( SPI_CLOCK_DIV8 );
      SPI.setDataMode(SPI_MODE0);
      SPI.setBitOrder(MSBFIRST);
#endif
      
      
      break;
    case KBR_COM_MSG_POWER_DOWN:
#if ARDUINO >= 10600
      SPI.endTransaction();
      SPI.end();
#else
      SPI.end();
#endif
      break;
    case KBR_COM_MSG_DELAY:
      delayMicroseconds(arg);
      break;
    case KBR_COM_MSG_CHANGE_RESET_LINE:
      if ( kbr->pin_list[KBR_PIN_RST] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_RST], arg);
      break;
    case KBR_COM_MSG_CHANGE_CS_LINE:
      if ( kbr->pin_list[KBR_PIN_CS] != KBR_PIN_VAL_NONE )
	digitalWrite(kbr->pin_list[KBR_PIN_CS], arg);
      break;
    case KBR_COM_MSG_CHANGE_CD_LINE:
      digitalWrite(kbr->pin_list[KBR_PIN_CD], arg);
      break;
    case KBR_COM_MSG_SEND_BYTE:
      SPI.transfer(arg); 
      break;
    case KBR_COM_MSG_REPEAT_1_BYTE:
      while( arg > 0 ) {
	SPI.transfer(data[0]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_2_BYTES:
      while( arg > 0 ) {
	SPI.transfer(data[0]);
	SPI.transfer(data[1]);
	arg--;
      }
      break;
    case KBR_COM_MSG_REPEAT_3_BYTES:
      while( arg > 0 ) {
	SPI.transfer(data[0]);
	SPI.transfer(data[1]);
	SPI.transfer(data[2]);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_STR:
      while( arg > 0 ) {
	SPI.transfer(*data++);
	arg--;
      }
      break;
    case KBR_COM_MSG_SEND_CD_DATA_SEQUENCE:
      while(arg > 0)
      {
	if ( *data != 0 )
	{
	  /* set the data line directly, ignore the setting from KBR_CFG_CD */
	  if ( *data == 1 )
	  {
	    digitalWrite(kbr->pin_list[KBR_PIN_CD], 0);
	  }
	  else
	  {
	    digitalWrite(kbr->pin_list[KBR_PIN_CD], 1);
	  }
	}
	data++;
	SPI.transfer(*data);
	data++;
	arg--;
      }
      break;
  }
  return 1;
}

void kbrlib4WireHWSPI::begin(uint8_t is_transparent)
{ 
  kbr_Init(&kbr, dev_cb, ext_cb, kbr_com_arduino_4wire_HW_SPI); 
  kbr_SetFontMode(&kbr, is_transparent);
}


/*=========================================================================*/

void kbrlib::init(void) {
  uint8_t i;
  
  // do a dummy init so that something usefull is part of the kbr structure
  kbr_Init(&kbr, kbr_dev_default_cb, kbr_ext_none, (kbr_com_fnptr)0);

  // reset cursor position
  tx = 0;
  ty = 0;
  tdir = 0;	// default direction for Arduino print() 
  
  for( i = 0; i < KBR_PIN_COUNT; i++ )
    kbr.pin_list[i] = KBR_PIN_VAL_NONE;
  
}

size_t kbrlib::write(uint8_t c) { 
  kbr_int_t delta;
  delta = kbr_DrawGlyph(get_kbr(), get_tx(), get_ty(), get_tdir(), c); 
  switch(get_tdir()) {
    case 0: get_tx() += delta; break;
    case 1: get_ty() += delta; break;
    case 2: get_tx() -= delta; break;
    default: case 3: get_ty() -= delta; break;
  }
  return 1;
}

