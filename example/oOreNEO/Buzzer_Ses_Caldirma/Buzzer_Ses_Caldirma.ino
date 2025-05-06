#define BUZZER_PIN 0  // Buzzer'ın bağlı olduğu pin (GPIO0)

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);  // Buzzer pini çıkış olarak ayarlanıyor
}

void loop() {
  // İlk "bip"
  digitalWrite(BUZZER_PIN, HIGH);  // Buzzer'ı çalıştır
  delay(100);                      // 100 ms süreyle ses ver
  digitalWrite(BUZZER_PIN, LOW);   // Buzzer'ı kapat
  delay(200);                      // Kısa bir ara

  // İkinci "bip"
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);

  delay(1000);  // 1 saniye bekle, sonra döngü tekrar başlasın
}
