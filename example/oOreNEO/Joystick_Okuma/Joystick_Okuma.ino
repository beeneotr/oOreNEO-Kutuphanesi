// Joystick X ve Y eksenleri için pin tanımlamaları
#define JOYSTICK_X_PIN 35  // Joystick'in X ekseni GPIO35'e bağlı
#define JOYSTICK_Y_PIN 32  // Joystick'in Y ekseni GPIO32'ye bağlı

void setup() {
  Serial.begin(115200); // Seri iletişimi başlat
  pinMode(JOYSTICK_X_PIN, INPUT); // X ekseni girişi
  pinMode(JOYSTICK_Y_PIN, INPUT); // Y ekseni girişi
}

void loop() {
  // Joystick'ten analog değerleri oku (0 - 4095 arası)
  int xDegeri = analogRead(JOYSTICK_X_PIN);
  int yDegeri = analogRead(JOYSTICK_Y_PIN);

  // Değerleri seri ekrana yazdır
  Serial.print("X: ");
  Serial.print(xDegeri);
  Serial.print(" | Y: ");
  Serial.println(yDegeri);

  delay(200); // Okumalar arasında 200 ms bekle
}
