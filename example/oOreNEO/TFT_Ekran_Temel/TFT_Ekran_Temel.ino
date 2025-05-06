#include <Ucglib.h>

// ST7735 ekran bağlantıları: CD, CS, RESET
Ucglib_ST7735_18x128x160_HWSPI ekran(2, 15, 4);

void setup() {
                        
  ekran.begin(UCG_FONT_MODE_SOLID);          // Yazı modu
  ekran.setFont(ucg_font_timB14_tr);        // Font seçimi
  ekran.setColor(255, 255, 255);             // Beyaz yazı
  ekran.clearScreen();                       // Ekranı temizle

  ekran.setRotate90();  // Ekranı döndürmek istiyorsaniz
  ekran.setPrintPos(10, 60);                 // Yazının pozisyonu (x, y)
  ekran.print("Merhaba Dunya!");                // Yazıyı yazdır
}

void loop() {
  // Bu örnekte sürekli tekrar gerek yok
}
