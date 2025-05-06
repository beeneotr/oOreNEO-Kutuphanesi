// Buton pin tanımlamaları
#define BUTON1_PIN 26   // Buton 1, GPIO26 pinine bağlı
#define BUTON2_PIN 17   // Buton 2, GPIO17 pinine bağlı

void setup() {
  Serial.begin(115200);           // Seri haberleşmeyi başlat
  pinMode(BUTON1_PIN, INPUT);     // Buton 1 pini giriş olarak ayarlanıyor
  pinMode(BUTON2_PIN, INPUT);     // Buton 2 pini giriş olarak ayarlanıyor
}

void loop() {
  // Buton pinlerinden gelen dijital verileri oku
  int buton1Durum = digitalRead(BUTON1_PIN);
  int buton2Durum = digitalRead(BUTON2_PIN);

  // Buton 1 için durum kontrolü
  if (buton1Durum == HIGH) {
    Serial.print("Buton 1: BASILDI");
  } else {
    Serial.print("Buton 1: BASILMADI");
  }

  Serial.print(" | ");

  // Buton 2 için durum kontrolü
  if (buton2Durum == HIGH) {
    Serial.println("Buton 2: BASILDI");
  } else {
    Serial.println("Buton 2: BASILMADI");
  }

  delay(200); // Okumalar arasında kısa bir bekleme (0.2 saniye)
}
