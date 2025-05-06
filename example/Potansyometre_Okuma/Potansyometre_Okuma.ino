#define POT_PIN 34  // Dahili potansiyometre pini (ESP32)

void setup() {
  Serial.begin(115200);
  pinMode(POT_PIN, INPUT);
}

void loop() {
  int potDegeri = analogRead(POT_PIN);  // 0–4095 arası
  Serial.print("Potansiyometre Degeri: ");
  Serial.println(potDegeri);
  delay(200);  // Okumalar arasında kısa bekleme
}
