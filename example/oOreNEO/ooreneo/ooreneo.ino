#include "ooreneo.h" // include the header file for the class



void setup()
{
Serial.begin(115200);

}

void loop()
{
  ooreneo.begin();
int a = ooreneo.analogRead(0);
  Serial.print(a);
  Serial.println(" Analog Read \n");
  delay(500);


}
