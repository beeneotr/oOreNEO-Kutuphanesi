#ifndef OORENE_H
#define OORENE_H

#ifdef ARDUINO
#if (ARDUINO >= 100)
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif

#ifdef USE_TINYUSB // For Serial when selecting TinyUSB
#include <Adafruit_TinyUSB.h>
#endif

#ifdef TARGET_LPC1768
#include <Arduino.h>
#endif

#if defined(ARDUINO_ARCH_RP2040)
#include <stdlib.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "rp2040_pio.h"
#endif

#include <Arduino.h>
#include <Print.h>
#include <SPI.h>
#include <WiFi.h>

#ifndef USE_PIN_LIST
#define USE_PIN_LIST
#endif

#include "clib/kbr.h"

#define ENKplus 36
#define ENKneg 39
#define ENKbut 26
#define POT 34
#define JYSx 35
#define JYSy 32
#define JYSbut 17
#define VIBmotor 33
#define buton1 26
#define buton2 17
#define buzzer 0
#define Led 27
#define Led_no 8

#define NEO_RGB ((0 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B
#define NEO_RBG ((0 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G
#define NEO_GRB ((1 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B
#define NEO_GBR ((2 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R
#define NEO_BRG ((1 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G
#define NEO_BGR ((2 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R

#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3)) ///< Transmit as W,R,G,B
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2)) ///< Transmit as W,R,B,G
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3)) ///< Transmit as W,G,R,B
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2)) ///< Transmit as W,G,B,R
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1)) ///< Transmit as W,B,R,G
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1)) ///< Transmit as W,B,G,R

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3)) ///< Transmit as R,W,G,B
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2)) ///< Transmit as R,W,B,G
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3)) ///< Transmit as R,G,W,B
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B,W
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1)) ///< Transmit as R,B,W,G
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G,W

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3)) ///< Transmit as G,W,R,B
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2)) ///< Transmit as G,W,B,R
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3)) ///< Transmit as G,R,W,B
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B,W
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,W,R
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R,W

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0)) ///< Transmit as B,W,R,G
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0)) ///< Transmit as B,W,G,R
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0)) ///< Transmit as B,R,W,G
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G,W
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,W,R
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R,W

#define NEO_KHZ800 0x0000 ///< 800 KHz data transmission
#ifndef __AVR_ATtiny85__
#define NEO_KHZ400 0x0100 ///< 400 KHz data transmission
#endif

#ifdef NEO_KHZ400
typedef uint16_t neoPixelType; ///< 3rd arg to kbr_NeoPixel constructor
#else
typedef uint8_t neoPixelType; ///< 3rd arg to kbr_NeoPixel constructor
#endif

static const uint8_t PROGMEM _NeoPixelSineTable[256] = {
    128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
    173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
    213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
    241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
    254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
    250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
    230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
    196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
    152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
    106, 103, 100, 97,  93,  90,  88,  85,  82,  79,  76,  73,  70,  67,  65,
    62,  59,  57,  54,  52,  49,  47,  44,  42,  40,  37,  35,  33,  31,  29,
    27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,  10,  9,   7,   6,
    5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,   0,
    0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,   10,  11,
    12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,  37,
    40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
    79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112, 115, 118, 121,
    124};

static const uint8_t PROGMEM _NeoPixelGammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

class kbr_NeoPixel {

public:
  // Constructor: number of LEDs, pin number, LED type
  kbr_NeoPixel(uint16_t n, int16_t pin = 6,
                    neoPixelType type = NEO_GRB + NEO_KHZ800);
  kbr_NeoPixel(void);
  ~kbr_NeoPixel();

  void begin(void);
  void show(void);
  void setPin(int16_t p);
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  void setPixelColor(uint16_t n, uint32_t c);
  void fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0);
  void setBrightness(uint8_t);
  void clear(void);
  void updateLength(uint16_t n);
  void updateType(neoPixelType t);
  /*!
    @brief   Check whether a call to show() will start sending data
             immediately or will 'block' for a required interval. NeoPixels
             require a short quiet time (about 300 microseconds) after the
             last bit is received before the data 'latches' and new data can
             start being received. Usually one's sketch is implicitly using
             this time to generate a new frame of animation...but if it
             finishes very quickly, this function could be used to see if
             there's some idle time available for some low-priority
             concurrent task.
    @return  1 or true if show() will start sending immediately, 0 or false
             if show() would block (meaning some idle time is available).
  */
  bool canShow(void) {
    // It's normal and possible for endTime to exceed micros() if the
    // 32-bit clock counter has rolled over (about every 70 minutes).
    // Since both are uint32_t, a negative delta correctly maps back to
    // positive space, and it would seem like the subtraction below would
    // suffice. But a problem arises if code invokes show() very
    // infrequently...the micros() counter may roll over MULTIPLE times in
    // that interval, the delta calculation is no longer correct and the
    // next update may stall for a very long time. The check below resets
    // the latch counter if a rollover has occurred. This can cause an
    // extra delay of up to 300 microseconds in the rare case where a
    // show() call happens precisely around the rollover, but that's
    // neither likely nor especially harmful, vs. other code that might
    // stall for 30+ minutes, or having to document and frequently remind
    // and/or provide tech support explaining an unintuitive need for
    // show() calls at least once an hour.
    uint32_t now = micros();
    if (endTime > now) {
      endTime = now;
    }
    return (now - endTime) >= 300L;
  }
  /*!
    @brief   Get a pointer directly to the NeoPixel data buffer in RAM.
             Pixel data is stored in a device-native format (a la the NEO_*
             constants) and is not translated here. Applications that access
             this buffer will need to be aware of the specific data format
             and handle colors appropriately.
    @return  Pointer to NeoPixel buffer (uint8_t* array).
    @note    This is for high-performance applications where calling
             setPixelColor() on every single pixel would be too slow (e.g.
             POV or light-painting projects). There is no bounds checking
             on the array, creating tremendous potential for mayhem if one
             writes past the ends of the buffer. Great power, great
             responsibility and all that.
  */
  uint8_t *getPixels(void) const { return pixels; };
  uint8_t getBrightness(void) const;
  /*!
    @brief   Retrieve the pin number used for NeoPixel data output.
    @return  Arduino pin number (-1 if not set).
  */
  int16_t getPin(void) const { return pin; };
  /*!
    @brief   Return the number of pixels in an kbr_NeoPixel strip object.
    @return  Pixel count (0 if not set).
  */
  uint16_t numPixels(void) const { return numLEDs; }
  uint32_t getPixelColor(uint16_t n) const;
  /*!
    @brief   An 8-bit integer sine wave function, not directly compatible
             with standard trigonometric units like radians or degrees.
    @param   x  Input angle, 0-255; 256 would loop back to zero, completing
                the circle (equivalent to 360 degrees or 2 pi radians).
                One can therefore use an unsigned 8-bit variable and simply
                add or subtract, allowing it to overflow/underflow and it
                still does the expected contiguous thing.
    @return  Sine result, 0 to 255, or -128 to +127 if type-converted to
             a signed int8_t, but you'll most likely want unsigned as this
             output is often used for pixel brightness in animation effects.
  */
  static uint8_t sine8(uint8_t x) {
    return pgm_read_byte(&_NeoPixelSineTable[x]); // 0-255 in, 0-255 out
  }
  /*!
    @brief   An 8-bit gamma-correction function for basic pixel brightness
             adjustment. Makes color transitions appear more perceptially
             correct.
    @param   x  Input brightness, 0 (minimum or off/black) to 255 (maximum).
    @return  Gamma-adjusted brightness, can then be passed to one of the
             setPixelColor() functions. This uses a fixed gamma correction
             exponent of 2.6, which seems reasonably okay for average
             NeoPixels in average tasks. If you need finer control you'll
             need to provide your own gamma-correction function instead.
  */
  static uint8_t gamma8(uint8_t x) {
    return pgm_read_byte(&_NeoPixelGammaTable[x]); // 0-255 in, 0-255 out
  }
  /*!
    @brief   Convert separate red, green and blue values into a single
             "packed" 32-bit RGB color.
    @param   r  Red brightness, 0 to 255.
    @param   g  Green brightness, 0 to 255.
    @param   b  Blue brightness, 0 to 255.
    @return  32-bit packed RGB value, which can then be assigned to a
             variable for later use or passed to the setPixelColor()
             function. Packed RGB format is predictable, regardless of
             LED strand color order.
  */
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }
  /*!
    @brief   Convert separate red, green, blue and white values into a
             single "packed" 32-bit WRGB color.
    @param   r  Red brightness, 0 to 255.
    @param   g  Green brightness, 0 to 255.
    @param   b  Blue brightness, 0 to 255.
    @param   w  White brightness, 0 to 255.
    @return  32-bit packed WRGB value, which can then be assigned to a
             variable for later use or passed to the setPixelColor()
             function. Packed WRGB format is predictable, regardless of
             LED strand color order.
  */
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }
  static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255);
  /*!
    @brief   A gamma-correction function for 32-bit packed RGB or WRGB
             colors. Makes color transitions appear more perceptially
             correct.
    @param   x  32-bit packed RGB or WRGB color.
    @return  Gamma-adjusted packed color, can then be passed in one of the
             setPixelColor() functions. Like gamma8(), this uses a fixed
             gamma correction exponent of 2.6, which seems reasonably okay
             for average NeoPixels in average tasks. If you need finer
             control you'll need to provide your own gamma-correction
             function instead.
  */
  static uint32_t gamma32(uint32_t x);

  void rainbow(uint16_t first_hue = 0, int8_t reps = 1,
               uint8_t saturation = 255, uint8_t brightness = 255,
               bool gammify = true);

private:
#if defined(ARDUINO_ARCH_RP2040)
  void  rp2040Init(uint8_t pin, bool is800KHz);
  void  rp2040Show(uint8_t pin, uint8_t *pixels, uint32_t numBytes, bool is800KHz);
#endif

protected:
#ifdef NEO_KHZ400 // If 400 KHz NeoPixel support enabled...
  bool is800KHz; ///< true if 800 KHz pixels
#endif
  bool begun;         ///< true if begin() previously called
  uint16_t numLEDs;   ///< Number of RGB LEDs in strip
  uint16_t numBytes;  ///< Size of 'pixels' buffer below
  int16_t pin;        ///< Output pin number (-1 if not yet set)
  uint8_t brightness; ///< Strip brightness 0-255 (stored as +1)
  uint8_t *pixels;    ///< Holds LED color values (3 or 4 bytes each)
  uint8_t rOffset;    ///< Red index within each 3- or 4-byte pixel
  uint8_t gOffset;    ///< Index of green byte
  uint8_t bOffset;    ///< Index of blue byte
  uint8_t wOffset;    ///< Index of white (==rOffset if no white)
  uint32_t endTime;   ///< Latch timing reference
#ifdef __AVR__
  volatile uint8_t *port; ///< Output PORT register
  uint8_t pinMask;        ///< Output PORT bitmask
#endif
#if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_ARDUINO_CORE_STM32)
  GPIO_TypeDef *gpioPort; ///< Output GPIO PORT
  uint32_t gpioPin;       ///< Output GPIO PIN
#endif
#if defined(ARDUINO_ARCH_RP2040)
  PIO pio = pio0;
  int sm = 0;
  bool init = true;
#endif
};

// Do not use kbrlib class directly, use kbrlib8Bit or kbrlib4WireSPI instead
class kbrlib : public Print
{
  protected:
    kbr_t kbr;
    kbr_dev_fnptr dev_cb;
    kbr_dev_fnptr ext_cb;  
  private:
    kbr_int_t tx, ty;          // current position for the Print base class procedures
    uint8_t tdir;
  protected:
    uint8_t& get_tdir(void) { return tdir; };
    kbr_int_t& get_tx(void) { return tx; };
    kbr_int_t& get_ty(void) { return ty; };
    kbr_t *get_kbr(void) { return &kbr; };
    void init(void);
  public:
    kbrlib(void) { init(); }
    kbrlib(kbr_dev_fnptr dev, kbr_dev_fnptr ext = kbr_ext_none) { init(); dev_cb = dev; ext_cb = ext; }

    void setPrintPos(kbr_int_t x, kbr_int_t y) { tx = x; ty = y; }
    void setPrintDir(uint8_t dir) { tdir = dir; }
    size_t write(uint8_t c);
    kbr_t *getkbr(void) { return &kbr; }
    
    kbr_int_t getWidth(void) { return kbr_GetWidth(&kbr); }
    kbr_int_t getHeight(void) { return kbr_GetHeight(&kbr); }
    
    
    void setFontRefHeightText(void) 	{ kbr_SetFontRefHeightText(&kbr); }
    void setFontRefHeightExtendedText(void) { kbr_SetFontRefHeightExtendedText(&kbr); }
    void setFontRefHeightAll(void) 	{ kbr_SetFontRefHeightAll(&kbr); }

    void setFontPosBaseline(void) 	{ kbr_SetFontPosBaseline(&kbr); }
    void setFontPosBottom(void) 	{ kbr_SetFontPosBottom(&kbr); }
    void setFontPosTop(void) 		{ kbr_SetFontPosTop(&kbr); }
    void setFontPosCenter(void) 	{ kbr_SetFontPosCenter(&kbr); }
    
    void setFont(const kbr_fntpgm_uint8_t  *font)
      { kbr_SetFont(&kbr, font); }
    void setFontMode(uint8_t is_transparent) 
      { kbr_SetFontMode(&kbr, is_transparent); }
    kbr_int_t getFontAscent(void)
      { return kbr_GetFontAscent(&kbr); }
    kbr_int_t getFontDescent(void)
      { return kbr_GetFontDescent(&kbr); }
    kbr_int_t getStrWidth(const char *s)
      { return kbr_GetStrWidth(&kbr, s); }
    
    void setColor(uint8_t idx, uint8_t r, uint8_t g, uint8_t b)
      { kbr_SetColor(&kbr, idx, r, g, b); }
    void setColor(uint8_t r, uint8_t g, uint8_t b)
      { kbr_SetColor(&kbr, 0, r, g, b); }

      
    void undoRotate(void) { kbr_UndoRotate(&kbr); }
    void setRotate90(void) { kbr_SetRotate90(&kbr); }
    void setRotate180(void) { kbr_SetRotate180(&kbr); }
    void setRotate270(void) { kbr_SetRotate270(&kbr); }

    void undoScale(void) { kbr_UndoScale(&kbr); }
    void setScale2x2(void) { kbr_SetScale2x2(&kbr); }
    
    void powerDown(void) { kbr_PowerDown(&kbr); }
    void powerUp(void) { kbr_PowerUp(&kbr); }
    
    
    
    // Procedures, which are always available as part of the BASIC drawing procedure set
      
    void setClipRange(kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h) { kbr_SetClipRange(&kbr, x,y,w,h); }
    void setMaxClipRange(void) { kbr_SetMaxClipRange(&kbr); }
    void undoClipRange(void) { kbr_SetMaxClipRange(&kbr); }

    void drawPixel(kbr_int_t x, kbr_int_t y) { kbr_DrawPixel(&kbr, x, y); }    
    void drawHLine(kbr_int_t x, kbr_int_t y, kbr_int_t len) { kbr_DrawHLine(&kbr, x, y, len); }
    void drawVLine(kbr_int_t x, kbr_int_t y, kbr_int_t len) { kbr_DrawVLine(&kbr, x, y, len); }
    void drawLine(kbr_int_t x1, kbr_int_t y1, kbr_int_t x2, kbr_int_t y2) { kbr_DrawLine(&kbr, x1, y1, x2, y2); }

    kbr_int_t drawGlyph(kbr_int_t x, kbr_int_t y, uint8_t dir, uint8_t encoding) { return kbr_DrawGlyph(&kbr, x, y, dir, encoding); }
    kbr_int_t drawString(kbr_int_t x, kbr_int_t y, uint8_t dir, const char *str) { return kbr_DrawString(&kbr, x, y, dir, str); }
    
    void drawBox(kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h) { kbr_DrawBox(&kbr, x, y, w, h); }
    void clearScreen(void) { kbr_ClearScreen(&kbr); }
    void drawRBox(kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h, kbr_int_t r) { kbr_DrawRBox(&kbr, x, y, w, h, r); }
    
    void drawFrame(kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h) { kbr_DrawFrame(&kbr, x, y, w, h); }
    void drawRFrame(kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h, kbr_int_t r) { kbr_DrawRFrame(&kbr, x, y, w, h, r); }
 
    void drawDisc(kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option) { kbr_DrawDisc(&kbr, x0, y0, rad, option); }
    void drawCircle(kbr_int_t x0, kbr_int_t y0, kbr_int_t rad, uint8_t option) { kbr_DrawCircle(&kbr, x0, y0, rad, option); }

    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) { kbr_DrawTriangle(&kbr, x0, y0, x1, y1, x2, y2); }
    /* the polygon procedure only works for convex tetragons (http://en.wikipedia.org/wiki/Convex_polygon) */
    void drawTetragon(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) { kbr_DrawTetragon(&kbr, x0, y0, x1, y1, x2, y2, x3, y3); }
    
    // Procedures, which are only available with the EXTENDED option
    
    void drawGradientLine(kbr_int_t x, kbr_int_t y, kbr_int_t len, kbr_int_t dir) {kbr_DrawGradientLine(&kbr, x, y, len, dir); }
    void drawGradientBox(kbr_int_t x, kbr_int_t y, kbr_int_t w, kbr_int_t h) { kbr_DrawGradientBox(&kbr, x, y, w, h); }
};

class kbrlib4WireSWSPI : public kbrlib
{
  public:
    kbrlib4WireSWSPI(kbr_dev_fnptr dev, kbr_dev_fnptr ext, uint8_t scl, uint8_t sda, uint8_t cd, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE)
      { init(); dev_cb = dev; ext_cb = ext; 

	  kbr.pin_list[KBR_PIN_SCL] = scl; 	
	  kbr.pin_list[KBR_PIN_SDA] = sda; 
	  kbr.pin_list[KBR_PIN_RST] = reset; 
	  kbr.pin_list[KBR_PIN_CD] = cd;
	  kbr.pin_list[KBR_PIN_CS] = cs; }
    void begin(uint8_t is_transparent);
};

class kbrlib3Wire9bitSWSPI : public kbrlib
{
  public:
    kbrlib3Wire9bitSWSPI(kbr_dev_fnptr dev, kbr_dev_fnptr ext, uint8_t scl, uint8_t sda, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE)
      { init(); dev_cb = dev; ext_cb = ext; 

	  kbr.pin_list[KBR_PIN_SCL] = scl; 	
	  kbr.pin_list[KBR_PIN_SDA] = sda; 
	  kbr.pin_list[KBR_PIN_RST] = reset; 
	  kbr.pin_list[KBR_PIN_CS] = cs; }
    void begin(uint8_t is_transparent);
};

class kbrlib4WireHWSPI : public kbrlib
{
  public:
    kbrlib4WireHWSPI(kbr_dev_fnptr dev, kbr_dev_fnptr ext, uint8_t cd, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE)
      { init(); dev_cb = dev; ext_cb = ext; 

	kbr.pin_list[KBR_PIN_RST] = reset; 
	kbr.pin_list[KBR_PIN_CD] = cd;
	kbr.pin_list[KBR_PIN_CS] = cs; 

#ifdef __AVR__	
	  kbr.data_port[KBR_PIN_RST] =  portOutputRegister(digitalPinToPort(reset));
	  kbr.data_mask[KBR_PIN_RST] =  digitalPinToBitMask(reset);
	  kbr.data_port[KBR_PIN_CD] =  portOutputRegister(digitalPinToPort(cd));
	  kbr.data_mask[KBR_PIN_CD] =  digitalPinToBitMask(cd);
	  kbr.data_port[KBR_PIN_CS] =  portOutputRegister(digitalPinToPort(cs));
	  kbr.data_mask[KBR_PIN_CS] =  digitalPinToBitMask(cs);
#endif
    }
    void begin(uint8_t is_transparent);
};

class kbrlib3Wire9bitHWSPI : public kbrlib
{
  public:
    kbrlib3Wire9bitHWSPI(kbr_dev_fnptr dev, kbr_dev_fnptr ext, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE)
      { init(); dev_cb = dev; ext_cb = ext; 

	  kbr.pin_list[KBR_PIN_RST] = reset; 
	  kbr.pin_list[KBR_PIN_CS] = cs; }
    void begin(uint8_t is_transparent);
};


#ifdef __AVR__	
class kbrlib8BitPortD : public kbrlib
{
  public:
    kbrlib8BitPortD(kbr_dev_fnptr dev, kbr_dev_fnptr ext, uint8_t wr, uint8_t cd, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE)
      { init(); dev_cb = dev; ext_cb = ext; 
	  kbr.pin_list[KBR_PIN_RST] = reset;
	  kbr.pin_list[KBR_PIN_CD] = cd;
	  kbr.pin_list[KBR_PIN_CS] = cs;
	  kbr.pin_list[KBR_PIN_WR] = wr; 
	  kbr.data_port[KBR_PIN_RST] =  portOutputRegister(digitalPinToPort(reset));
	  kbr.data_mask[KBR_PIN_RST] =  digitalPinToBitMask(reset);
	  kbr.data_port[KBR_PIN_CD] =  portOutputRegister(digitalPinToPort(cd));
	  kbr.data_mask[KBR_PIN_CD] =  digitalPinToBitMask(cd);
	  kbr.data_port[KBR_PIN_CS] =  portOutputRegister(digitalPinToPort(cs));
	  kbr.data_mask[KBR_PIN_CS] =  digitalPinToBitMask(cs);
	  kbr.data_port[KBR_PIN_WR] =  portOutputRegister(digitalPinToPort(wr));
	  kbr.data_mask[KBR_PIN_WR] =  digitalPinToBitMask(wr);
    }
    void begin(uint8_t is_transparent);
};
#endif


class kbrlib8Bit : public kbrlib
{
  public:
    kbrlib8Bit(kbr_dev_fnptr dev, kbr_dev_fnptr ext, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cd, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE)
      { init(); dev_cb = dev; ext_cb = ext; 
	  kbr.pin_list[KBR_PIN_RST] = reset;
	  kbr.pin_list[KBR_PIN_CD] = cd;
	  kbr.pin_list[KBR_PIN_CS] = cs;
	  kbr.pin_list[KBR_PIN_WR] = wr; 
	  kbr.pin_list[KBR_PIN_D0] = d0;
	  kbr.pin_list[KBR_PIN_D1] = d1;
	  kbr.pin_list[KBR_PIN_D2] = d2;
	  kbr.pin_list[KBR_PIN_D3] = d3;
	  kbr.pin_list[KBR_PIN_D4] = d4;
	  kbr.pin_list[KBR_PIN_D5] = d5;
	  kbr.pin_list[KBR_PIN_D6] = d6;
	  kbr.pin_list[KBR_PIN_D7] = d7; }
    void begin(uint8_t is_transparent);
};

class kbrlib_ST7735_18x128x160_HWSPI : public kbrlib4WireHWSPI
{
  public:
  kbrlib_ST7735_18x128x160_HWSPI( uint8_t cd, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE) : 
    kbrlib4WireHWSPI(kbr_dev_st7735_18x128x160, kbr_ext_st7735_18, /*cd=*/ cd , /*cs=*/ cs, /*reset=*/ reset)
    { }
};

class kbrlib_ST7735_18x128x160_SWSPI : public kbrlib4WireSWSPI
{
  public:
  kbrlib_ST7735_18x128x160_SWSPI( uint8_t scl, uint8_t sda, uint8_t cd, uint8_t cs = KBR_PIN_VAL_NONE, uint8_t reset = KBR_PIN_VAL_NONE) : 
    kbrlib4WireSWSPI(kbr_dev_st7735_18x128x160, kbr_ext_st7735_18, /*scl=*/ scl, /*sda=*/ sda, /*cd=*/ cd , /*cs=*/ cs, /*reset=*/ reset)
    { }
};

class OORENEO
{
private:
  
  const int csPin = 5; // Chip select pin for MCP3208
  const int spiClock = 1000000; // 1 MHz clock speed
  int Variable1;
  int Module_analog_value[5];
  int Module_indice[5]={0,0,0,0,0};
  int current_detected_module[5]={0,0,0,0,0};
  int analogRead1(int sayi);
  
public:
  static OORENEO& getInstance();
  int analogRead(int channel);
  void begin();

};

extern OORENEO ooreneo;
#endif // OORENEO_H