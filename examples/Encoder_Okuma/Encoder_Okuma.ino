#define ENK_A 36   // Encoder A sinyali (ENKplus)
#define ENK_B 39   // Encoder B sinyali (ENKneg)

int konum = 0;               // Mevcut pozisyon
int oncekiDurum = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENK_A, INPUT);
  pinMode(ENK_B, INPUT);
  oncekiDurum = digitalRead(ENK_A);
}

void loop() {
  int yeniDurum = digitalRead(ENK_A);

  if (yeniDurum != oncekiDurum) {
    if (digitalRead(ENK_B) != yeniDurum) {
      konum++;  // Saat yönü
    } else {
      konum--;  // Saat yönünün tersi
    }

    Serial.print("Encoder Pozisyonu: ");
    Serial.println(konum);
  }

  oncekiDurum = yeniDurum;
}
