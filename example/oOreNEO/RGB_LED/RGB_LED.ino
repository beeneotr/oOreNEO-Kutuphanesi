/**
Yuklemeniz Gereken Kutuphane
*/

#include <FastLED.h>

#define LED_PIN     27
#define LED_SAYISI  8

CRGB ledler[LED_SAYISI];

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(ledler, LED_SAYISI);
  FastLED.clear();

  // Farklı renklere ayarla
  ledler[0] = CRGB::Red;
  ledler[1] = CRGB::Green;
  ledler[2] = CRGB::Blue;
  ledler[3] = CRGB::Yellow;
  ledler[4] = CRGB::Cyan;
  ledler[5] = CRGB::Magenta;
  ledler[6] = CRGB::Orange;
  ledler[7] = CRGB::White;

  FastLED.show();
}

void loop() {
  // Bir şey yapılmıyor, LED'ler sabit renk gösteriyor
}
