#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();  // I2C başlat (GPIO21 = SDA, GPIO22 = SCL)

  Serial.println("MPU6050 başlatılıyor...");
  if (!mpu.begin()) {
    Serial.println("MPU6050 bulunamadı!");
    while (1);  // Devam etme
  }

  mpu.calcGyroOffsets(true);  // İsteğe bağlı kalibrasyon
  Serial.println("MPU6050 hazır.");
}

void loop() {
  mpu.update();  // Yeni verileri oku

  Serial.print("Acc X: "); Serial.print(mpu.getAccX());
  Serial.print(" | Y: "); Serial.print(mpu.getAccY());
  Serial.print(" | Z: "); Serial.print(mpu.getAccZ());

  Serial.print(" || Gyro X: "); Serial.print(mpu.getGyroX());
  Serial.print(" | Y: "); Serial.print(mpu.getGyroY());
  Serial.print(" | Z: "); Serial.println(mpu.getGyroZ());

  delay(200);
}
