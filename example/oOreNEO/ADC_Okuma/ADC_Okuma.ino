#include "ooreneo.h"

void setup() {
 Serial.begin(115200);
}

void loop()
{
  int a = ooreneo.analogRead(0);
  Serial.print(a);
  Serial.println(" Analog Read \n");
  delay(500);

}
