#include "kbr.h"
typedef void * kbr_glyph_t;
#define KBR_FONT_DATA_STRUCT_SIZE 21

void kbr_font_GetStrSize(const void *font, const char *s, kbr_int_t *width, kbr_int_t *height);
void kbr_font_GetStrSizeP(const void *font, const char *s, kbr_int_t *width, kbr_int_t *height);

void kbr_font_AdjustXYToDraw(const void *font, const char *s, kbr_int_t *x, kbr_int_t *y);
void kbr_font_AdjustXYToDrawP(const void *font, const char *s, kbr_int_t *x, kbr_int_t *y);

void kbr_font_GetStrMinBox(kbr_t *kbr, const void *font, const char *s, kbr_int_t *x, kbr_int_t *y, kbr_int_t *width, kbr_int_t *height);

static uint8_t kbr_font_get_byte(const kbr_fntpgm_uint8_t *font, uint8_t offset)
{
  font += offset;
  return kbr_pgm_read( (kbr_pgm_uint8_t *)font );  
}

static uint16_t kbr_font_get_word(const kbr_fntpgm_uint8_t *font, uint8_t offset) KBR_NOINLINE; 
static uint16_t kbr_font_get_word(const kbr_fntpgm_uint8_t *font, uint8_t offset)
{
    uint16_t pos;
    font += offset;
    pos = kbr_pgm_read( (kbr_pgm_uint8_t *)font );
    font++;
    pos <<= 8;
    pos += kbr_pgm_read( (kbr_pgm_uint8_t *)font);
    return pos;
}

void kbr_read_font_info(kbr_font_info_t *font_info, const kbr_fntpgm_uint8_t *font)
{
  /* offset 0 */
  font_info->glyph_cnt = kbr_font_get_byte(font, 0);
  font_info->bbx_mode = kbr_font_get_byte(font, 1);
  font_info->bits_per_0 = kbr_font_get_byte(font, 2);
  font_info->bits_per_1 = kbr_font_get_byte(font, 3);
  
  /* offset 4 */
  font_info->bits_per_char_width = kbr_font_get_byte(font, 4);
  font_info->bits_per_char_height = kbr_font_get_byte(font, 5);
  font_info->bits_per_char_x = kbr_font_get_byte(font, 6);
  font_info->bits_per_char_y = kbr_font_get_byte(font, 7);
  font_info->bits_per_delta_x = kbr_font_get_byte(font, 8);
  
  /* offset 9 */
  font_info->max_char_width = kbr_font_get_byte(font, 9);
  font_info->max_char_height = kbr_font_get_byte(font, 10);
  font_info->x_offset = kbr_font_get_byte(font, 11);
  font_info->y_offset = kbr_font_get_byte(font, 12);
  
  /* offset 13 */
  font_info->ascent_A = kbr_font_get_byte(font, 13);
  font_info->descent_g = kbr_font_get_byte(font, 14);
  font_info->ascent_para = kbr_font_get_byte(font, 15);
  font_info->descent_para = kbr_font_get_byte(font, 16);
  
  /* offset 17 */
  font_info->start_pos_upper_A = kbr_font_get_word(font, 17);
  font_info->start_pos_lower_a = kbr_font_get_word(font, 19);  
}

uint8_t kbr_font_GetCapitalAHeight(const void *font)
{
  return kbr_font_get_byte(font, 13);
}

uint8_t kbr_font_GetFontStartEncoding(const void *font_arg)
{
  const uint8_t *font = font_arg;
  font += KBR_FONT_DATA_STRUCT_SIZE;
  return kbr_pgm_read( ((kbr_pgm_uint8_t *)font) );
}

uint8_t kbr_font_GetFontEndEncoding(const void *font_arg)
{
  uint8_t encoding = 0;
  const uint8_t *font = font_arg;
  font += KBR_FONT_DATA_STRUCT_SIZE;
  
  for(;;)
  {
    if ( kbr_pgm_read( ((kbr_pgm_uint8_t *)font) + 1 ) == 0 )
      break;
    encoding = kbr_pgm_read( ((kbr_pgm_uint8_t *)font)  );
    font += kbr_pgm_read( ((kbr_pgm_uint8_t *)font) + 1 );
  }
  
  return encoding;  
}

int8_t kbr_font_GetLowerGDescent(const void *font)
{
  return kbr_font_get_byte(font, 14);
}

size_t kbr_font_GetSize(const void *font_arg)
{
  const uint8_t *font = font_arg;
  font += KBR_FONT_DATA_STRUCT_SIZE;
  
  for(;;)
  {
    if ( kbr_pgm_read( ((kbr_pgm_uint8_t *)font) + 1 ) == 0 )
      break;
    font += kbr_pgm_read( ((kbr_pgm_uint8_t *)font) + 1 );
  }
  
  return (font - (const uint8_t *)font_arg) + 2;
  
}

uint8_t kbr_GetFontBBXWidth(kbr_t *kbr)
{
  return kbr->font_info.max_char_width;		/* new font info structure */
}

uint8_t kbr_GetFontBBXHeight(kbr_t *kbr)
{
  return kbr->font_info.max_char_height;		/* new font info structure */
}

int8_t u8g_GetFontBBXOffX(kbr_t *kbr) KBR_NOINLINE;
int8_t u8g_GetFontBBXOffX(kbr_t *kbr)
{
  return kbr->font_info.x_offset;		/* new font info structure */
}

int8_t kbr_GetFontBBXOffY(kbr_t *kbr) KBR_NOINLINE;
int8_t kbr_GetFontBBXOffY(kbr_t *kbr)
{
  return kbr->font_info.y_offset;		/* new font info structure */
}

uint8_t kbr_GetFontCapitalAHeight(kbr_t *kbr) KBR_NOINLINE; 
uint8_t kbr_GetFontCapitalAHeight(kbr_t *kbr)
{

  return kbr->font_info.ascent_A;		/* new font info structure */
}

uint8_t kbr_font_decode_get_unsigned_bits(kbr_font_decode_t *f, uint8_t cnt) 
{
  uint8_t val;
  uint8_t bit_pos = f->decode_bit_pos;
  uint8_t bit_pos_plus_cnt;
  val = kbr_pgm_read( (kbr_pgm_uint8_t *)(f->decode_ptr) );  
  
  val >>= bit_pos;
  bit_pos_plus_cnt = bit_pos;
  bit_pos_plus_cnt += cnt;
  if ( bit_pos_plus_cnt >= 8 )
  {
    uint8_t s = 8;
    s -= bit_pos;
    f->decode_ptr++;
    val |= kbr_pgm_read( (kbr_pgm_uint8_t *)(f->decode_ptr) ) << (s);
    bit_pos_plus_cnt -= 8;
  }
  val &= (1U<<cnt)-1;
  
  f->decode_bit_pos = bit_pos_plus_cnt;
  return val;
}

int8_t kbr_font_decode_get_signed_bits(kbr_font_decode_t *f, uint8_t cnt)
{
  int8_t v, d;
  v = (int8_t)kbr_font_decode_get_unsigned_bits(f, cnt);
  d = 1;
  cnt--;
  d <<= cnt;
  v -= d;
  return v;
}


static kbr_int_t kbr_add_vector_y(kbr_int_t dy, int8_t x, int8_t y, uint8_t dir) KBR_NOINLINE;
static kbr_int_t kbr_add_vector_y(kbr_int_t dy, int8_t x, int8_t y, uint8_t dir)
{
  switch(dir)
  {
    case 0:
      dy += y;
      break;
    case 1:
      dy += x;
      break;
    case 2:
      dy -= y;
      break;
    default:
      dy -= x;
      break;      
  }
  return dy;
}

static kbr_int_t kbr_add_vector_x(kbr_int_t dx, int8_t x, int8_t y, uint8_t dir) KBR_NOINLINE;
static kbr_int_t kbr_add_vector_x(kbr_int_t dx, int8_t x, int8_t y, uint8_t dir)
{
  switch(dir)
  {
    case 0:
      dx += x;
      break;
    case 1:
      dx -= y;
      break;
    case 2:
      dx -= x;
      break;
    default:
      dx += y;
      break;      
  }
  return dx;
}


#ifdef OBSOLETE

void kbr_font_decode_draw_pixel(kbr_t *kbr, uint8_t lx, uint8_t ly, uint8_t cnt, uint8_t is_foreground)
{
  kbr_int_t x, y;
  kbr_font_decode_t *decode = &(kbr->font_decode);
  
  x = decode->target_x;
  y = decode->target_y;

  x = kbr_add_vector_x(x, lx, ly, decode->dir);
  y = kbr_add_vector_y(y, lx, ly, decode->dir);

  if ( is_foreground )
  {
    kbr_Draw90Line(kbr, 
      x, 
      y, 
      cnt, 
      /* dir */ decode->dir, 
      /* col_idx */ 0);   
  }
  else if ( decode->is_transparent == 0 )    
  {
    kbr_Draw90Line(kbr, 
      x, 
      y, 
      cnt, 
      /* dir */ decode->dir, 
      /* col_idx */ 1);   
  }
}
#endif

void kbr_font_decode_len(kbr_t *kbr, uint8_t len, uint8_t is_foreground)
{
  uint8_t cnt;
  uint8_t rem; 
  uint8_t current;	
  uint8_t lx,ly;
  kbr_int_t x, y;
  kbr_font_decode_t *decode = &(kbr->font_decode);
  cnt = len;
  lx = decode->x;
  ly = decode->y;
  
  for(;;)
  {
    rem = decode->glyph_width;
    rem -= lx;
    current = rem;
    if ( cnt < rem )
      current = cnt;
    x = decode->target_x;
    y = decode->target_y;

    x = kbr_add_vector_x(x, lx, ly, decode->dir);
    y = kbr_add_vector_y(y, lx, ly, decode->dir);
    
    if ( is_foreground )
    {
      kbr_Draw90Line(kbr, 
	x, 
	y, 
	current, 
	/* dir */ decode->dir, 
	/* col_idx */ 0);   
    }
    else if ( decode->is_transparent == 0 )    
    {
      kbr_Draw90Line(kbr, 
	x, 
	y, 
	current, 
	/* dir */ decode->dir, 
	/* col_idx */ 1);   
    }
    if ( cnt < rem )
      break;
    cnt -= rem;
    lx = 0;
    ly++;
  }
  lx += cnt;
  
  decode->x = lx;
  decode->y = ly;
  
}

static void kbr_font_setup_decode(kbr_t *kbr, const uint8_t *glyph_data)
{
  kbr_font_decode_t *decode = &(kbr->font_decode);
  decode->decode_ptr = glyph_data;
  decode->decode_bit_pos = 0;
  
  decode->decode_ptr += 1;
  decode->decode_ptr += 1;
  
  decode->glyph_width = kbr_font_decode_get_unsigned_bits(decode, kbr->font_info.bits_per_char_width);
  decode->glyph_height = kbr_font_decode_get_unsigned_bits(decode,kbr->font_info.bits_per_char_height);
}

int8_t kbr_font_decode_glyph(kbr_t *kbr, const uint8_t *glyph_data)
{
  uint8_t a, b;
  int8_t x, y;
  int8_t d;
  int8_t h;
  kbr_font_decode_t *decode = &(kbr->font_decode);
    
  kbr_font_setup_decode(kbr, glyph_data);
  h = kbr->font_decode.glyph_height;
  
  x = kbr_font_decode_get_signed_bits(decode, kbr->font_info.bits_per_char_x);
  y = kbr_font_decode_get_signed_bits(decode, kbr->font_info.bits_per_char_y);
  d = kbr_font_decode_get_signed_bits(decode, kbr->font_info.bits_per_delta_x);
  
  if ( decode->glyph_width > 0 )
  {
    decode->target_x = kbr_add_vector_x(decode->target_x, x, -(h+y), decode->dir);
    decode->target_y = kbr_add_vector_y(decode->target_y, x, -(h+y), decode->dir);
    //kbr_add_vector(&(decode->target_x), &(decode->target_y), x, -(h+y), decode->dir);
   
    /* reset local x/y position */
    decode->x = 0;
    decode->y = 0;
    
    /* decode glyph */
    for(;;)
    {
      a = kbr_font_decode_get_unsigned_bits(decode, kbr->font_info.bits_per_0);
      b = kbr_font_decode_get_unsigned_bits(decode, kbr->font_info.bits_per_1);
      do
      {
	kbr_font_decode_len(kbr, a, 0);
	kbr_font_decode_len(kbr, b, 1);
      } while( kbr_font_decode_get_unsigned_bits(decode, 1) != 0 );

      if ( decode->y >= h )
	break;
    }
  }
  return d;
}

const uint8_t *kbr_font_get_glyph_data(kbr_t *kbr, uint8_t encoding)
{
  const uint8_t *font = kbr->font;
  font += KBR_FONT_DATA_STRUCT_SIZE;
  
  if ( encoding >= 'a' )
  {
    font += kbr->font_info.start_pos_lower_a;
  }
  else if ( encoding >= 'A' )
  {
    font += kbr->font_info.start_pos_upper_A;
  }
  
  for(;;)
  {
    if ( kbr_pgm_read( ((kbr_pgm_uint8_t *)font) + 1 ) == 0 )
      break;
    if ( kbr_pgm_read( (kbr_pgm_uint8_t *)font ) == encoding )
    {
      return font;
    }
    font += kbr_pgm_read( ((kbr_pgm_uint8_t *)font) + 1 );
  }
  return NULL;
}

kbr_int_t kbr_font_draw_glyph(kbr_t *kbr, kbr_int_t x, kbr_int_t y, uint8_t dir, uint8_t encoding)
{
  kbr_int_t dx = 0;
  kbr->font_decode.target_x = x;
  kbr->font_decode.target_y = y;
  kbr->font_decode.dir = dir;
  const uint8_t *glyph_data = kbr_font_get_glyph_data(kbr, encoding);
  if ( glyph_data != NULL )
  {
    dx = kbr_font_decode_glyph(kbr, glyph_data);
  }
  return dx;
}



uint8_t kbr_IsGlyph(kbr_t *kbr, uint8_t requested_encoding)
{
  if ( kbr_font_get_glyph_data(kbr, requested_encoding) != NULL )
    return 1;
  return 0;
}

int8_t kbr_GetGlyphWidth(kbr_t *kbr, uint8_t requested_encoding)
{
  const uint8_t *glyph_data = kbr_font_get_glyph_data(kbr, requested_encoding);
  if ( glyph_data == NULL )
    return 0; 
  
  kbr_font_setup_decode(kbr, glyph_data);
  kbr_font_decode_get_signed_bits(&(kbr->font_decode), kbr->font_info.bits_per_char_x);
  kbr_font_decode_get_signed_bits(&(kbr->font_decode), kbr->font_info.bits_per_char_y);

  return kbr_font_decode_get_signed_bits(&(kbr->font_decode), kbr->font_info.bits_per_delta_x);
}

void kbr_SetFontMode(kbr_t *kbr, uint8_t is_transparent)
{
  kbr->font_decode.is_transparent = is_transparent;		
}

kbr_int_t kbr_DrawGlyph(kbr_t *kbr, kbr_int_t x, kbr_int_t y, uint8_t dir, uint8_t encoding)
{
  switch(dir)
  {
    case 0:
      y += kbr->font_calc_vref(kbr);
      break;
    case 1:
      x -= kbr->font_calc_vref(kbr);
      break;
    case 2:
      y -= kbr->font_calc_vref(kbr);
      break;
    case 3:
      x += kbr->font_calc_vref(kbr);
      break;
  }
  return kbr_font_draw_glyph(kbr, x, y, dir, encoding);
}

kbr_int_t kbr_DrawString(kbr_t *kbr, kbr_int_t x, kbr_int_t y, uint8_t dir, const char *str)
{
  kbr_int_t delta, sum;
  sum = 0;
  while( *str != '\0' )
  {
    delta = kbr_DrawGlyph(kbr, x, y, dir, (uint8_t)*str);
    
    switch(dir)
    {
      case 0:
	x += delta;
	break;
      case 1:
	y += delta;
	break;
      case 2:
	x -= delta;
	break;
      case 3:
	y -= delta;
	break;
    }
    
    sum += delta;    
    str++;
  }
  return sum;
}

void kbr_UpdateRefHeight(kbr_t *kbr)
{
  if ( kbr->font == NULL )
    return;
  kbr->font_ref_ascent = kbr->font_info.ascent_A;
  kbr->font_ref_descent = kbr->font_info.descent_g;
  if ( kbr->font_height_mode == KBR_FONT_HEIGHT_MODE_TEXT )
  {
  }
  else if ( kbr->font_height_mode == KBR_FONT_HEIGHT_MODE_XTEXT )
  {
    if ( kbr->font_ref_ascent < kbr->font_info.ascent_para )
      kbr->font_ref_ascent = kbr->font_info.ascent_para;
    if ( kbr->font_ref_descent > kbr->font_info.descent_para )
      kbr->font_ref_descent = kbr->font_info.descent_para;
  }
  else
  {
    if ( kbr->font_ref_ascent < kbr->font_info.max_char_height+kbr->font_info.y_offset )
      kbr->font_ref_ascent = kbr->font_info.max_char_height+kbr->font_info.y_offset;
    if ( kbr->font_ref_descent > kbr->font_info.y_offset )
      kbr->font_ref_descent = kbr->font_info.y_offset;
  }  
}

void kbr_SetFontRefHeightText(kbr_t *kbr)
{
  kbr->font_height_mode = KBR_FONT_HEIGHT_MODE_TEXT;
  kbr_UpdateRefHeight(kbr);
}

void kbr_SetFontRefHeightExtendedText(kbr_t *kbr)
{
  kbr->font_height_mode = KBR_FONT_HEIGHT_MODE_XTEXT;
  kbr_UpdateRefHeight(kbr);
}

void kbr_SetFontRefHeightAll(kbr_t *kbr)
{
  kbr->font_height_mode = KBR_FONT_HEIGHT_MODE_ALL;
  kbr_UpdateRefHeight(kbr);
}

kbr_int_t kbr_font_calc_vref_font(kbr_t *kbr)
{
  return 0;
}

void kbr_SetFontPosBaseline(kbr_t *kbr)
{
  kbr->font_calc_vref = kbr_font_calc_vref_font;
}


kbr_int_t kbr_font_calc_vref_bottom(kbr_t *kbr)
{
  return (kbr_int_t)(kbr->font_ref_descent);
}

void kbr_SetFontPosBottom(kbr_t *kbr)
{
  kbr->font_calc_vref = kbr_font_calc_vref_bottom;
}

kbr_int_t kbr_font_calc_vref_top(kbr_t *kbr)
{
  kbr_int_t tmp;
  tmp = (kbr_int_t)(kbr->font_ref_ascent);
  tmp++;
  return tmp;
}

void kbr_SetFontPosTop(kbr_t *kbr)
{
  kbr->font_calc_vref = kbr_font_calc_vref_top;
}

kbr_int_t kbr_font_calc_vref_center(kbr_t *kbr)
{
  int8_t tmp;
  tmp = kbr->font_ref_ascent;
  tmp -= kbr->font_ref_descent;
  tmp /= 2;
  tmp += kbr->font_ref_descent;  
  return tmp;
}

void kbr_SetFontPosCenter(kbr_t *kbr)
{
  kbr->font_calc_vref = kbr_font_calc_vref_center;
}

void kbr_SetFont(kbr_t *kbr, const kbr_fntpgm_uint8_t  *font)
{
  if ( kbr->font != font )
  {
    kbr->font = font;
    kbr_read_font_info(&(kbr->font_info), font);
    kbr_UpdateRefHeight(kbr);
  }
}

kbr_int_t kbr_GetStrWidth(kbr_t *kbr, const char *s)
{
  kbr_int_t  w;
  uint8_t encoding;
  
  w = 0;
  
  for(;;)
  {
    encoding = *s;
    if ( encoding == 0 )
      break;
    w += kbr_GetGlyphWidth(kbr, encoding);
    
    s++;
  }
  return w;  
}
