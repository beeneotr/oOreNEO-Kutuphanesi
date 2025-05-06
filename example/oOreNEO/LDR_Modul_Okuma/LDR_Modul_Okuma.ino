#include <ooreneo.h>  // Kibris kütüphanesini dahil et

// 📌 LÜTFEN DİKKAT:
// LDR modülünü IO/03 portuna (Analog Port) takın.
// Bu port yalnızca analog girişler içindir ve MCP3208 üzerinden okunur.

#define LDR_KANALI 0  // LDR sensörü A0 pinine bağlı (IO/03 üzerinden)

void setup() {
  Serial.begin(115200);  // Seri iletişimi başlat
}

void loop() {
  // LDR'den analog değeri oku
  int ldrDegeri = ooreneo.analogRead(LDR_KANALI);

  // Seri monitöre yazdır
  Serial.print("LDR Degeri: ");
  Serial.println(ldrDegeri);

  delay(200);  // Kısa gecikme
}
