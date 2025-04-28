#ifndef _KBR_H
#define _KBR_H

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif

#if defined(ARDUINO)
#ifndef USE_PIN_LIST
#define USE_PIN_LIST
#endif
#endif

#ifdef __GNUC__
#  define KBR_NOINLINE __attribute__((noinline))
#  define KBR_SECTION(name) __attribute__ ((section (name)))
#  if defined(__MSPGCC__)
/* mspgcc does not have .progmem sections. Use -fdata-sections. */
#    define KBR_FONT_SECTION(name)
#  elif defined(__AVR__)
#    define KBR_FONT_SECTION(name) KBR_SECTION(".progmem." name)
#  else
#    define KBR_FONT_SECTION(name)
#  endif
#else
#  define KBR_NOINLINE
#  define KBR_SECTION(name)
#  define KBR_FONT_SECTION(name)
#endif

#if defined(__AVR__)
#include <avr/pgmspace.h>
/* KBR_PROGMEM is used by the XBM example */
#define KBR_PROGMEM KBR_SECTION(".progmem.data")
typedef uint8_t PROGMEM kbr_pgm_uint8_t;
typedef uint8_t kbr_fntpgm_uint8_t;
#define kbr_pgm_read(adr) pgm_read_byte_near(adr)
#define KBR_PSTR(s) ((kbr_pgm_uint8_t *)PSTR(s))
#else
#define KBR_PROGMEM
#define PROGMEM
typedef uint8_t kbr_pgm_uint8_t;
typedef uint8_t kbr_fntpgm_uint8_t;
#define kbr_pgm_read(adr) (*(const kbr_pgm_uint8_t *)(adr)) 
#define KBR_PSTR(s) ((kbr_pgm_uint8_t *)(s))
#endif

/*================================================*/
/* several type and forward definitions */

typedef int16_t kbr_int_t;
typedef struct _kbr_t kbr_t;
typedef struct _kbr_xy_t kbr_xy_t;
typedef struct _kbr_wh_t kbr_wh_t;
typedef struct _kbr_box_t kbr_box_t;
typedef struct _kbr_color_t kbr_color_t;
typedef struct _kbr_ccs_t kbr_ccs_t;
typedef struct _kbr_pixel_t kbr_pixel_t;
typedef struct _kbr_arg_t kbr_arg_t;
typedef struct _kbr_com_info_t kbr_com_info_t;

typedef kbr_int_t (*kbr_dev_fnptr)(kbr_t *kbr, kbr_int_t msg, void *data); 
typedef int16_t (*kbr_com_fnptr)(kbr_t *kbr, int16_t msg, uint16_t arg, uint8_t *data); 
typedef kbr_int_t (*kbr_font_calc_vref_fnptr)(kbr_t *kbr);

/*================================================*/
/* list of supported display modules */

kbr_int_t kbr_dev_st7735_18x128x160(kbr_t *kbr, kbr_int_t msg, void *data);

/*================================================*/
/* 
  list of extensions for the controllers 
  
  each module can have the "none" extension (kbr_ext_none) or the specific
  extensions, that matches the controller name and color depth.
  
  example: for the module
  valid extensions are:
    kbr_ext_none
    kbr_ext_ssd1351_18
*/

kbr_int_t kbr_ext_none(kbr_t *kbr, kbr_int_t msg, void *data);

kbr_int_t kbr_ext_st7735_18(kbr_t *kbr, kbr_int_t msg, void *data);

/*================================================*/
/* list of supported display controllers */

kbr_int_t kbr_dev_ic_st7735_18(kbr_t *kbr, kbr_int_t msg, void *data);

/*================================================*/
/* struct declarations */

struct _kbr_xy_t
{
  kbr_int_t x;
  kbr_int_t y;
};

struct _kbr_wh_t
{
  kbr_int_t w;  
  kbr_int_t h;
};

struct _kbr_box_t
{
  kbr_xy_t ul;
  kbr_wh_t size;
};

struct _kbr_color_t
{
  uint8_t color[3];		/* 0: Red, 1: Green, 2: Blue */
};

struct _kbr_ccs_t
{
  uint8_t current;	/* contains the current color component */
  uint8_t start;
  kbr_int_t dir;		/* 1 if start < end or -1 if start > end */
  kbr_int_t num;
  kbr_int_t quot;
  
  kbr_int_t den;
  kbr_int_t rem;  
  kbr_int_t frac;
};

struct _kbr_pixel_t
{
  kbr_xy_t pos;
  kbr_color_t rgb;  
};

struct _kbr_arg_t
{
  kbr_pixel_t pixel;
  kbr_int_t len;
  kbr_int_t dir;
  kbr_int_t offset;			/* calculated offset from the inital point to the start of the clip window (kbr_clip_l90fx) */
  kbr_int_t scale;			/* upscale factor, used by KBR_MSG_DRAW_L90BF */
  const unsigned char *bitmap;
  kbr_int_t pixel_skip;		/* within the "bitmap" skip the specified number of pixel with the bit. pixel_skip is always <= 7 */
  kbr_color_t rgb[4];			/* start and end color for L90SE , two more colors for the gradient box */
  kbr_ccs_t ccs_line[3];		/* color component sliders used by L90SE */
};

#define KBR_FONT_HEIGHT_MODE_TEXT 0
#define KBR_FONT_HEIGHT_MODE_XTEXT 1
#define KBR_FONT_HEIGHT_MODE_ALL 2

struct _kbr_com_info_t
{
  uint16_t serial_clk_speed;	/* nano seconds cycle time */
  uint16_t parallel_clk_speed;	/* nano seconds cycle time */
};


struct _kbr_font_info_t
{
  /* offset 0 */
  uint8_t glyph_cnt;
  uint8_t bbx_mode;
  uint8_t bits_per_0;
  uint8_t bits_per_1;
  
  /* offset 4 */
  uint8_t bits_per_char_width;
  uint8_t bits_per_char_height;		
  uint8_t bits_per_char_x;
  uint8_t bits_per_char_y;
  uint8_t bits_per_delta_x;
  
  /* offset 9 */
  int8_t max_char_width;
  int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
  int8_t x_offset;
  int8_t y_offset;
  
  /* offset 13 */
  int8_t  ascent_A;
  int8_t  descent_g;
  int8_t  ascent_para;
  int8_t  descent_para;
  
  /* offset 17 */
  uint16_t start_pos_upper_A;
  uint16_t start_pos_lower_a;  
};
typedef struct _kbr_font_info_t kbr_font_info_t;

struct _kbr_font_decode_t
{
  const uint8_t *decode_ptr;			/* pointer to the compressed data */
  
  kbr_int_t target_x;
  kbr_int_t target_y;
  
  int8_t x;						/* local coordinates, (0,0) is upper left */
  int8_t y;
  int8_t glyph_width;	
  int8_t glyph_height;

  uint8_t decode_bit_pos;			/* bitpos inside a byte of the compressed data */
  uint8_t is_transparent;
  uint8_t dir;				/* direction */
};
typedef struct _kbr_font_decode_t kbr_font_decode_t;



#ifdef USE_PIN_LIST
#define KBR_PIN_RST 0
#define KBR_PIN_CD 1
#define KBR_PIN_CS 2
#define KBR_PIN_SCL 3
#define KBR_PIN_WR 3
#define KBR_PIN_SDA 4

#define KBR_PIN_D0 5
#define KBR_PIN_D1 6
#define KBR_PIN_D2 7
#define KBR_PIN_D3 8
#define KBR_PIN_D4 9
#define KBR_PIN_D5 10
#define KBR_PIN_D6 11
#define KBR_PIN_D7 12

#define KBR_PIN_COUNT 13

#define KBR_PIN_VAL_NONE 255
#endif

struct _kbr_t
{
  unsigned is_power_up:1;
  /* the dimension of the display */
  kbr_wh_t dimension;
  /* display callback procedure to handle display specific code */
  /* controller and device specific code, high level procedure will call this */
  kbr_dev_fnptr device_cb;
  /* name of the extension cb. will be called by device_cb if required */
  kbr_dev_fnptr ext_cb;
  /* if rotation is applied, than this cb is called after rotation */
  kbr_dev_fnptr rotate_chain_device_cb;
  kbr_wh_t rotate_dimension;

  /* if rotation is applied, than this cb is called by the scale device */
  kbr_dev_fnptr scale_chain_device_cb;
  
  /* communication interface */
  kbr_com_fnptr com_cb;
  
  /* offset, that is additionally added to KBR_VARX/KBR_VARY */
  /* seems to be required for the Nokia display */
  // kbr_xy_t display_offset;
  
  /* data which is passed to the cb procedures */
  /* can be modified by the cb procedures (rotation, clipping, etc) */
  kbr_arg_t arg;
  /* current window to which all drawing is clipped */
  /* should be modified via KBR_MSG_SET_CLIP_BOX by a device callback. */
  /* by default this is done by kbr_dev_default_cb */
  kbr_box_t clip_box;
  

  /* information about the current font */
  const unsigned char *font;             /* current font for all text procedures */
  kbr_font_calc_vref_fnptr font_calc_vref;

  kbr_font_decode_t font_decode;		/* new font decode structure */
  kbr_font_info_t font_info;			/* new font info structure */

  int8_t glyph_dx;			/* OBSOLETE */
  int8_t glyph_x;			/* OBSOLETE */
  int8_t glyph_y;			/* OBSOLETE */
  uint8_t glyph_width;		/* OBSOLETE */
  uint8_t glyph_height;		/* OBSOLETE */
  
  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;

  /* only for Arduino/C++ Interface */
#ifdef USE_PIN_LIST
  uint8_t pin_list[KBR_PIN_COUNT];

#ifdef __AVR__
  volatile uint8_t *data_port[KBR_PIN_COUNT];
  uint8_t data_mask[KBR_PIN_COUNT];
#endif

#endif

  /* 
    Small amount of RAM for the com interface (com_cb).
    Might be unused on unix systems, where the com sub system is 
    not required, but should be usefull for all uC projects.
  */
  uint8_t com_initial_change_sent;	/* Bit 0: CD/A0 Line Status, Bit 1: CS Line Status, Bit 2: Reset Line Status */
  uint8_t com_status;		/* Bit 0: CD/A0 Line Status, Bit 1: CS Line Status, Bit 2: Reset Line Status,  Bit 3: 1 for power up */
  uint8_t com_cfg_cd;		/* Bit 0: Argument Level, Bit 1: Command Level */
  
  
};

#define kbr_GetWidth(kbr) ((kbr)->dimension.w)
#define kbr_GetHeight(kbr) ((kbr)->dimension.h)

#define KBR_MSG_DEV_POWER_UP	10
#define KBR_MSG_DEV_POWER_DOWN 11
#define KBR_MSG_SET_CLIP_BOX 12
#define KBR_MSG_GET_DIMENSION 15

/* draw pixel with color from idx 0 */
#define KBR_MSG_DRAW_PIXEL 20
#define KBR_MSG_DRAW_L90FX 21
/* draw  bit pattern, transparent and draw color (idx 0) color */
//#define KBR_MSG_DRAW_L90TC 22		/* can be commented, used by kbr_DrawTransparentBitmapLine */
#define KBR_MSG_DRAW_L90SE 23		/* this part of the extension */
//#define KBR_MSG_DRAW_L90RL 24	/* not yet implemented */
/* draw  bit pattern with foreground (idx 1) and background (idx 0) color */
//#define KBR_MSG_DRAW_L90BF 25	 /* can be commented, used by kbr_DrawBitmapLine */


#define KBR_COM_STATUS_MASK_POWER 8
#define KBR_COM_STATUS_MASK_RESET 4
#define KBR_COM_STATUS_MASK_CS 2
#define KBR_COM_STATUS_MASK_CD 1

/*
  arg:	0
  data:	kbr_com_info_t *
  return:	0 for error
  note: 
    - power up is the first command, which is sent
*/
#define KBR_COM_MSG_POWER_UP 10

/*
  note: power down my be followed only by power up command
*/
#define KBR_COM_MSG_POWER_DOWN 11

/*
  arg:	delay in microseconds  (0..4095) 
*/
#define KBR_COM_MSG_DELAY 12

/*
  kbr->com_status	contains previous status of reset line
  arg:			new logic level for reset line
*/
#define KBR_COM_MSG_CHANGE_RESET_LINE 13
/*
  kbr->com_status	contains previous status of cs line
  arg:	new logic level for cs line
*/
#define KBR_COM_MSG_CHANGE_CS_LINE 14

/*
  kbr->com_status	contains previous status of cd line
  arg:	new logic level for cd line
*/
#define KBR_COM_MSG_CHANGE_CD_LINE 15

/*
  kbr->com_status	current status of Reset, CS and CD line (kbr->com_status)
  arg:			byte for display
*/
#define KBR_COM_MSG_SEND_BYTE 16

/*
  kbr->com_status	current status of Reset, CS and CD line (kbr->com_status)
  arg:			how often to repeat the 2/3 byte sequence 	
  data:			pointer to two or three bytes
*/
#define KBR_COM_MSG_REPEAT_1_BYTE 17
#define KBR_COM_MSG_REPEAT_2_BYTES 18
#define KBR_COM_MSG_REPEAT_3_BYTES 19

/*
  kbr->com_status	current status of Reset, CS and CD line (kbr->com_status)
  arg:			length of string 	
  data:			string
*/
#define KBR_COM_MSG_SEND_STR 20

/*
  kbr->com_status	current status of Reset, CS and CD line (kbr->com_status)
  arg:			number of cd_info and data pairs (half value of total byte cnt) 	
  data:			uint8_t with CD and data information
	cd_info data cd_info data cd_info data cd_info data ... cd_info data cd_info data
	cd_info is the level, which is directly applied to the CD line. This means,
	information applied to KBR_CFG_CD is not relevant.
*/
#define KBR_COM_MSG_SEND_CD_DATA_SEQUENCE 21



/*================================================*/
/* interrupt safe code */
#define KBR_INTERRUPT_SAFE
#if defined(KBR_INTERRUPT_SAFE)
#  if defined(__AVR__)
extern uint8_t global_SREG_backup;	
#    define KBR_ATOMIC_START()		do { global_SREG_backup = SREG; cli(); } while(0)
#    define KBR_ATOMIC_END()			SREG = global_SREG_backup
#    define KBR_ATOMIC_OR(ptr, val) 	do { uint8_t tmpSREG = SREG; cli(); (*(ptr) |= (val)); SREG = tmpSREG; } while(0)
#    define KBR_ATOMIC_AND(ptr, val) 	do { uint8_t tmpSREG = SREG; cli(); (*(ptr) &= (val)); SREG = tmpSREG; } while(0)
#  else
#    define KBR_ATOMIC_OR(ptr, val) (*(ptr) |= (val))
#    define KBR_ATOMIC_AND(ptr, val) (*(ptr) &= (val))
#    define KBR_ATOMIC_START()
#    define KBR_ATOMIC_END()
#  endif /* __AVR__ */
#else
#  define KBR_ATOMIC_OR(ptr, val) (*(ptr) |= (val))
#  define KBR_ATOMIC_AND(ptr, val) (*(ptr) &= (val))
#  define KBR_ATOMIC_START()
#  define KBR_ATOMIC_END()
#endif /* KBR_INTERRUPT_SAFE */

/*================================================*/
/* kbr_dev_msg_api.c */
void kbr_PowerDown(kbr_t *kbr);
kbr_int_t kbr_PowerUp(kbr_t *kbr);
void kbr_SetClipBox(kbr_t *kbr, kbr_box_t *clip_box);
void kbr_SetClipRange(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h);
void kbr_SetMaxClipRange(kbr_t *kbr);
void kbr_GetDimension(kbr_t *kbr);
void kbr_DrawPixelWithArg(kbr_t *kbr);
void kbr_DrawL90FXWithArg(kbr_t *kbr);
void kbr_DrawL90TCWithArg(kbr_t *kbr);
void kbr_DrawL90BFWithArg(kbr_t *kbr);
void kbr_DrawL90SEWithArg(kbr_t *kbr);

/*================================================*/
/* kbr_init.c */
kbr_int_t kbr_Init(kbr_t *kbr, kbr_dev_fnptr device_cb, kbr_dev_fnptr ext_cb, kbr_com_fnptr com_cb);


/*================================================*/
/* kbr_dev_sdl.c */
kbr_int_t kbr_sdl_dev_cb(kbr_t *kbr, kbr_int_t msg, void *data);

/*================================================*/
/* kbr_pixel.c */
void kbr_SetColor(kbr_t *kbr, uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void kbr_DrawPixel(kbr_t *kbr, kbr_int_t x, kbr_int_t y);

/*================================================*/
/* kbr_line.c */
void kbr_Draw90Line(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len, kbr_int_t dir, kbr_int_t col_idx);
void kbr_DrawHLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len);
void kbr_DrawVLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len);
void kbr_DrawHRLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len);
void kbr_DrawLine(kbr_t *kbr, kbr_int_t x1, kbr_int_t y1, kbr_int_t x2, kbr_int_t y2);
/* the following procedure is only available with the extended callback */
void kbr_DrawGradientLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t len, kbr_int_t dir);


/*================================================*/
/* kbr_box.c */
void kbr_DrawBox(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h);
void kbr_ClearScreen(kbr_t *kbr);
void kbr_DrawRBox(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h, kbr_int_t r);
void kbr_DrawGradientBox(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h);
void kbr_DrawFrame(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h);
void kbr_DrawRFrame(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h, kbr_int_t r);


/*================================================*/
/* kbr_circle.c */
#define KBR_DRAW_UPPER_RIGHT 0x01
#define KBR_DRAW_UPPER_LEFT  0x02
#define KBR_DRAW_LOWER_LEFT 0x04
#define KBR_DRAW_LOWER_RIGHT  0x08
#define KBR_DRAW_ALL (KBR_DRAW_UPPER_RIGHT|KBR_DRAW_UPPER_LEFT|KBR_DRAW_LOWER_RIGHT|KBR_DRAW_LOWER_LEFT)
void kbr_DrawDisc(kbr_t *kbr, kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option);
void kbr_DrawCircle(kbr_t *kbr, kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option);

/*================================================*/
/* kbr_bitmap.c */
void kbr_DrawTransparentBitmapLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t dir, kbr_int_t len, const unsigned char *bitmap);
void kbr_DrawBitmapLine(kbr_t *kbr, kbr_int_t x, kbr_int_t y, kbr_int_t dir, kbr_int_t len, const unsigned char *bitmap);

/*================================================*/
/* kbr_rotate.c */
void kbr_UndoRotate(kbr_t *kbr);
void kbr_SetRotate90(kbr_t *kbr);
void kbr_SetRotate180(kbr_t *kbr);
void kbr_SetRotate270(kbr_t *kbr);

/*================================================*/
/* kbr_scale.c */
void kbr_UndoScale(kbr_t *kbr);
void kbr_SetScale2x2(kbr_t *kbr);


/*================================================*/
/* kbr_polygon.c */

typedef int16_t pg_word_t;

#define PG_NOINLINE KBR_NOINLINE

struct pg_point_struct
{
  pg_word_t x;
  pg_word_t y;
};

typedef struct _pg_struct pg_struct;	/* forward declaration */

struct pg_edge_struct
{
  pg_word_t x_direction;	/* 1, if x2 is greater than x1, -1 otherwise */
  pg_word_t height;
  pg_word_t current_x_offset;
  pg_word_t error_offset;
  
  /* --- line loop --- */
  pg_word_t current_y;
  pg_word_t max_y;
  pg_word_t current_x;
  pg_word_t error;

  /* --- outer loop --- */
  uint8_t (*next_idx_fn)(pg_struct *pg, uint8_t i);
  uint8_t curr_idx;
};

/* maximum number of points in the polygon */
/* can be redefined, but highest possible value is 254 */
#define PG_MAX_POINTS 4

/* index numbers for the pge structures below */
#define PG_LEFT 0
#define PG_RIGHT 1


struct _pg_struct
{
  struct pg_point_struct list[PG_MAX_POINTS];
  uint8_t cnt;
  uint8_t is_min_y_not_flat;
  pg_word_t total_scan_line_cnt;
  struct pg_edge_struct pge[2];	/* left and right line draw structures */
};

void pg_ClearPolygonXY(pg_struct *pg);
void pg_AddPolygonXY(pg_struct *pg, kbr_t *kbr, int16_t x, int16_t y);
void pg_DrawPolygon(pg_struct *pg, kbr_t *kbr);
void kbr_ClearPolygonXY(void);
void kbr_AddPolygonXY(kbr_t *kbr, int16_t x, int16_t y);
void kbr_DrawPolygon(kbr_t *kbr);
void kbr_DrawTriangle(kbr_t *kbr, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
/* the polygon procedure only works for convex tetragons (http://en.wikipedia.org/wiki/Convex_polygon) */
void kbr_DrawTetragon(kbr_t *kbr, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);


/*================================================*/
// new font procedures
#define KBR_FONT_MODE_TRANSPARENT 1
#define KBR_FONT_MODE_SOLID 0
#define KBR_FONT_MODE_NONE 1


/* Information on a specific given font */
uint8_t kbr_font_GetFontStartEncoding(const void *font);
uint8_t kbr_font_GetFontEndEncoding(const void *font);

uint8_t kbr_font_GetCapitalAHeight(const void *font);

int8_t kbr_font_GetFontAscent(const void *font);
int8_t kbr_font_GetFontDescent(const void *font);

int8_t kbr_font_GetFontXAscent(const void *font);
int8_t kbr_font_GetFontXDescent(const void *font);

size_t kbr_font_GetSize(const void *font);

/* Information on the current font */

uint8_t kbr_GetFontBBXWidth(kbr_t *kbr);
uint8_t kbr_GetFontBBXHeight(kbr_t *kbr);
uint8_t kbr_GetFontCapitalAHeight(kbr_t *kbr) KBR_NOINLINE; 
uint8_t kbr_IsGlyph(kbr_t *kbr, uint8_t requested_encoding);
int8_t kbr_GetGlyphWidth(kbr_t *kbr, uint8_t requested_encoding);

#define kbr_GetFontAscent(kbr)	((kbr)->font_ref_ascent)
#define kbr_GetFontDescent(kbr)	((kbr)->font_ref_descent)

/* Drawing procedures */

kbr_int_t kbr_DrawGlyph(kbr_t *kbr, kbr_int_t x, kbr_int_t y, uint8_t dir, uint8_t encoding);
kbr_int_t kbr_DrawString(kbr_t *kbr, kbr_int_t x, kbr_int_t y, uint8_t dir, const char *str);

/* Mode selection/Font assignment */

void kbr_SetFontRefHeightText(kbr_t *kbr);
void kbr_SetFontRefHeightExtendedText(kbr_t *kbr);
void kbr_SetFontRefHeightAll(kbr_t *kbr);

void kbr_SetFontPosBaseline(kbr_t *kbr) KBR_NOINLINE;
void kbr_SetFontPosBottom(kbr_t *kbr);
void kbr_SetFontPosTop(kbr_t *kbr);
void kbr_SetFontPosCenter(kbr_t *kbr);

void kbr_SetFont(kbr_t *kbr, const kbr_fntpgm_uint8_t  *font);
void kbr_SetFontMode(kbr_t *kbr, uint8_t is_transparent);

kbr_int_t kbr_GetStrWidth(kbr_t *kbr, const char *s);


/*================================================*/
/* LOW LEVEL PROCEDRUES, ONLY CALLED BY DEV CB */

/*================================================*/
/* kbr_clip.c */
kbr_int_t kbr_clip_is_pixel_visible(kbr_t *kbr);
kbr_int_t kbr_clip_l90fx(kbr_t *kbr);
kbr_int_t kbr_clip_l90tc(kbr_t *kbr);
kbr_int_t kbr_clip_l90se(kbr_t *kbr);


/*================================================*/
/* kbr_ccs.c */
void kbr_ccs_init(kbr_ccs_t *ccs, uint8_t start, uint8_t end, kbr_int_t steps);
void kbr_ccs_step(kbr_ccs_t *ccs);
void kbr_ccs_seek(kbr_ccs_t *ccs, kbr_int_t pos);

/*================================================*/
/* kbr_dev_default_cb.c */
kbr_int_t kbr_dev_default_cb(kbr_t *kbr, kbr_int_t msg, void *data);
kbr_int_t kbr_handle_l90fx(kbr_t *kbr, kbr_dev_fnptr dev_cb);
kbr_int_t kbr_handle_l90tc(kbr_t *kbr, kbr_dev_fnptr dev_cb);
kbr_int_t kbr_handle_l90se(kbr_t *kbr, kbr_dev_fnptr dev_cb);
kbr_int_t kbr_handle_l90bf(kbr_t *kbr, kbr_dev_fnptr dev_cb);
void kbr_handle_l90rl(kbr_t *kbr, kbr_dev_fnptr dev_cb);


/*================================================*/
/* kbr_com_msg_api.c */

/* send command bytes and optional arguments */
#define KBR_C10(c0)				0x010, (c0)
#define KBR_C20(c0,c1)				0x020, (c0),(c1)
#define KBR_C11(c0,a0)				0x011, (c0),(a0)
#define KBR_C21(c0,c1,a0)			0x021, (c0),(c1),(a0)
#define KBR_C12(c0,a0,a1)			0x012, (c0),(a0),(a1)
#define KBR_C22(c0,c1,a0,a1)		0x022, (c0),(c1),(a0),(a1)
#define KBR_C13(c0,a0,a1,a2)		0x013, (c0),(a0),(a1),(a2)
#define KBR_C23(c0,c1,a0,a1,a2)		0x023, (c0),(c1),(a0),(a1),(a2)
#define KBR_C14(c0,a0,a1,a2,a3)		0x014, (c0),(a0),(a1),(a2),(a3)
#define KBR_C24(c0,c1,a0,a1,a2,a3)	0x024, (c0),(c1),(a0),(a1),(a2),(a3)
#define KBR_C15(c0,a0,a1,a2,a3,a4)	0x015, (c0),(a0),(a1),(a2),(a3),(a4)


#define KBR_C25(c0,c1,a0,a1,a2,a3,a4)	0x025, (c0),(c1),(a0),(a1),(a2),(a3),(a4)
#define KBR_C16(c0,a0,a1,a2,a3,a4,a5)	0x016, (c0),(a0),(a1),(a2),(a3),(a4),(a5)
#define KBR_C26(c0,c1,a0,a1,a2,a3,a4,a5)	0x026, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5)
#define KBR_C17(c0,a0,a1,a2,a3,a4,a5,a6)	0x017, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6)
#define KBR_C27(c0,c1,a0,a1,a2,a3,a4,a5,a6)	0x027, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6)
#define KBR_C18(c0,a0,a1,a2,a3,a4,a5,a6,a7)	0x018, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7)
#define KBR_C28(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7)	0x028, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7)
#define KBR_C19(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8)	0x019, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)
#define KBR_C29(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8)	0x029, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)
#define KBR_C1A(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)
#define KBR_C2A(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)
#define KBR_C1B(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa)
#define KBR_C2B(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa)
#define KBR_C1C(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab)
#define KBR_C2C(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab)
#define KBR_C1D(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac)
#define KBR_C2D(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac)
#define KBR_C1E(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad)
#define KBR_C2E(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad)
#define KBR_C1F(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad),(ae)
#define KBR_C2F(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad),(ae)
 


/* send one or more argument bytes */
#define KBR_A1(d0)					0x061, (d0)
#define KBR_A2(d0,d1)					0x062, (d0),(d1)
#define KBR_A3(d0,d1,d2)				0x063, (d0),(d1),(d2)
#define KBR_A4(d0,d1,d2,d3)				0x064, (d0),(d1),(d2),(d3)
#define KBR_A5(d0,d1,d2,d3,d4)			0x065, (d0),(d1),(d2),(d3),(d4)
#define KBR_A6(d0,d1,d2,d3,d4,d5)		0x066, (d0),(d1),(d2),(d3),(d4),(d5)
#define KBR_A7(d0,d1,d2,d3,d4,d5,d6)		0x067, (d0),(d1),(d2),(d3),(d4),(d5),(d6)
#define KBR_A8(d0,d1,d2,d3,d4,d5,d6,d7)	0x068, (d0),(d1),(d2),(d3),(d4),(d5),(d6),(d7)

/* force data mode on CD line */
#define KBR_DATA()					0x070
/* send one or more data bytes */
#define KBR_D1(d0)				0x071, (d0)
#define KBR_D2(d0,d1)				0x072, (d0),(d1)
#define KBR_D3(d0,d1,d2)			0x073, (d0),(d1),(d2)
#define KBR_D4(d0,d1,d2,d3)			0x074, (d0),(d1),(d2),(d3)
#define KBR_D5(d0,d1,d2,d3,d4)		0x075, (d0),(d1),(d2),(d3),(d4)
#define KBR_D6(d0,d1,d2,d3,d4,d5)	0x076, (d0),(d1),(d2),(d3),(d4),(d5)

/* delay by specified value. t = [0..4095] */
#define KBR_DLY_MS(t)				0x080 | (((t)>>8)&15), (t)&255
#define KBR_DLY_US(t)				0x090 | (((t)>>8)&15), (t)&255

/* access procedures to kbr->arg.pixel.pos.x und kbr->arg.pixel.pos.y */
#define KBR_VARX(s,a,o)				0x0a0 | ((s)&15), (a), (o)
#define KBR_VARY(s,a,o)				0x0b0 | ((s)&15), (a), (o)

/* force specific level on RST und CS */
#define KBR_RST(level)				0x0f0 | ((level)&1)
#define KBR_CS(level)				0x0f4 | ((level)&1)

/* Configure CD line for command, arguments and data */
/* Configure CMD/DATA line: "c" logic level CMD, "a" logic level CMD Args */
#define KBR_CFG_CD(c,a)			0x0fc | (((c)&1)<<1) | ((a)&1)

/* Termination byte */
#define KBR_END()					0x00

#define kbr_com_SendRepeat3Bytes(kbr, cnt, byte_ptr) \
  (kbr)->com_cb((kbr), KBR_COM_MSG_REPEAT_3_BYTES, (cnt), (byte_ptr))

void kbr_com_PowerDown(kbr_t *kbr);
int16_t kbr_com_PowerUp(kbr_t *kbr, uint16_t serial_clk_speed, uint16_t parallel_clk_speed);  /* values are nano seconds */
void kbr_com_SetLineStatus(kbr_t *kbr, uint8_t level, uint8_t mask, uint8_t msg) KBR_NOINLINE;
void kbr_com_SetResetLineStatus(kbr_t *kbr, uint8_t level);
void kbr_com_SetCSLineStatus(kbr_t *kbr, uint8_t level);
void kbr_com_SetCDLineStatus(kbr_t *kbr, uint8_t level);
void kbr_com_DelayMicroseconds(kbr_t *kbr, uint16_t delay) KBR_NOINLINE;
void kbr_com_DelayMilliseconds(kbr_t *kbr, uint16_t delay) KBR_NOINLINE;
#ifndef kbr_com_SendByte
void kbr_com_SendByte(kbr_t *kbr, uint8_t byte);
#endif
void kbr_com_SendRepeatByte(kbr_t *kbr, uint16_t cnt, uint8_t byte);
void kbr_com_SendRepeat2Bytes(kbr_t *kbr, uint16_t cnt, uint8_t *byte_ptr);
#ifndef kbr_com_SendRepeat3Bytes
void kbr_com_SendRepeat3Bytes(kbr_t *kbr, uint16_t cnt, uint8_t *byte_ptr);
#endif
void kbr_com_SendString(kbr_t *kbr, uint16_t cnt, const uint8_t *byte_ptr);
void kbr_com_SendCmdDataSequence(kbr_t *kbr, uint16_t cnt, const uint8_t *byte_ptr, uint8_t cd_line_status_at_end);
void kbr_com_SendCmdSeq(kbr_t *kbr, const kbr_pgm_uint8_t *data);


/*================================================*/
/* kbr_dev_tga.c */
int tga_init(uint16_t w, uint16_t h);
void tga_save(const char *name);

kbr_int_t kbr_dev_tga(kbr_t *kbr, kbr_int_t msg, void *data);

/*================================================*/

#ifdef OLD_FONTS

extern const kbr_fntpgm_uint8_t kbr_font_04b_03b[] KBR_FONT_SECTION("kbr_font_04b_03b");

#else

extern const kbr_fntpgm_uint8_t kbr_font_inr21_mr[] KBR_FONT_SECTION("kbr_font_inr21_mr");

#endif

#ifdef __cplusplus
}
#endif


#endif /* _KBR_H */
