#include "ooreneo.h"
#include <WiFi.h>



OORENEO& OORENEO::getInstance()
{
  static OORENEO instance;
  return instance;
}

void OORENEO::init(){
  SPI.begin(18, 19, 23, csPin); // Initialize SPI 
}

int OORENEO::analogRead(int channel) {

    OORENEO::getInstance().init();
    if (channel != 0 && channel != 1) {
        Serial.println("Please insert a pin number 0 or 1");
        return -1;
    }

    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH); // ensure it's deselected first
    delayMicroseconds(1);

    digitalWrite(csPin, LOW);
    delayMicroseconds(1);

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // Construct command: start bit (1), SGL/DIFF (1), and channel bits
    uint8_t commandByte = 0b00000110 | ((channel & 0x04) >> 2);
    uint8_t secondByte = (channel & 0x03) << 6;

    SPI.transfer(commandByte);       // Start + SGL + D2
    uint8_t highByte = SPI.transfer(secondByte); // D1 + D0 + 5 zeros
    uint8_t lowByte = SPI.transfer(0x00);        // read result

    SPI.endTransaction();

    digitalWrite(csPin, HIGH);

    int result = ((highByte & 0x0F) << 8) | lowByte;
    return result;
}


int OORENEO::analogRead1(int sayi) {

  OORENEO::getInstance().init();
  pinMode(csPin, OUTPUT);
  // Select the MCP3208 chip by setting the chip select pin low
  digitalWrite(csPin, LOW);

  // Send the start bit, single-ended conversion bit, and the sayi number (in the top 3 bits)
  int command =  0b0000011000 | sayi; // Start bit and single-ended conversion bit
  SPI.transfer(command);

  // Read the lower and upper byte of the result
  int result = SPI.transfer(0x00) << 8;
  result |= SPI.transfer(0x00);

  // Deselect the MCP3208 chip by setting the chip select pin high
  digitalWrite(csPin, HIGH);

  // Return the 12-bit result (mask out the last 4 bits)
  return result & 0x0FFF;
}


void OORENEO::begin() {
  
  for (int i = 0; i<5; i++)
  {
    Module_analog_value[i] = analogRead1(i+2);
  }

  for (int i = 0; i<5; i++)
  {

   if(Module_analog_value[i] > 100 && Module_analog_value[i] < 240)
    {
      Module_indice[i] = 1; // 01 is E-Module ID 
    }

    if(Module_analog_value[i] > 325 && Module_analog_value[i] < 480)
    {
      Module_indice[i] = 2; // 02 is E-Module ID 
    }

   if(Module_analog_value[i] > 580 && Module_analog_value[i] < 720)
    {
      Module_indice[i] = 3; // 03 is E-Module ID 
    }

    if(Module_analog_value[i] > 820 && Module_analog_value[i] < 960)
    {
      Module_indice[i] = 4; // 04 is E-Module ID 
    }

    if(Module_analog_value[i] > 1020 && Module_analog_value[i] < 1200)
    {
      Module_indice[i] = 5; // 05 is E-Module ID 
    }

    if(Module_analog_value[i] > 1250 && Module_analog_value[i] < 1440)
    {
      Module_indice[i] = 6; // 06 is E-Module ID 
    }

    if(Module_analog_value[i] > 1500 && Module_analog_value[i] < 1680)
    {
      Module_indice[i] = 7; // 07 is E-Module ID 
    }

    if(Module_analog_value[i] > 1800 && Module_analog_value[i] < 1920)
    {
      Module_indice[i] = 8; // 08 is E-Module ID  
    }

    if(Module_analog_value[i] > 2000 && Module_analog_value[i] < 2160)
    {
      Module_indice[i] = 9; // 09 is E-Module ID 
    }

    if(Module_analog_value[i] > 2280 && Module_analog_value[i] < 2400)
    {
      Module_indice[i] = 10;  // 10 is E-Module ID 
    }

    if(Module_analog_value[i] > 2480 && Module_analog_value[i] < 2640)
    {
      Module_indice[i] = 11;  // 11 is E-Module ID 
    }

    if(Module_analog_value[i] > 2770 && Module_analog_value[i] < 2880)
    {
      Module_indice[i] = 12;  // 12 is E-Module ID 
    }

    if(Module_analog_value[i] > 2925 && Module_analog_value[i] < 3120)
    {
      Module_indice[i] = 13;  // 13 is E-Module ID 
    }

    if(Module_analog_value[i] > 3200 && Module_analog_value[i] < 3360)
    {
      Module_indice[i] = 14;  // 14 is E-Module ID 
    }

    if(Module_analog_value[i] > 3480 && Module_analog_value[i] < 3600)
    {
      Module_indice[i] = 15;  // 15 is E-Module ID 
    }

    if(Module_analog_value[i] > 3680 && Module_analog_value[i] < 3840)
    {
      Module_indice[i] = 16;  // 16 is E-Module ID 
    }

    if(Module_analog_value[i] > 3950 && Module_analog_value[i] < 4095)
    {
      Module_indice[i] = 17;  // 17 is E-Module ID 
    }

  }

  for (int i = 0; i<5; i++)
  {

    if (Module_indice[i] != current_detected_module[i])
    {
      Serial.print("[:2023:41:1.1:");
      String macAddress = WiFi.macAddress();
      Serial.print("" + macAddress);
      Serial.print(":");
      Serial.print(i+1);
      Serial.print(":");
      Serial.print(Module_indice[i]);
      Serial.print(":");
      Serial.println("]");
      current_detected_module[i] = Module_indice[i];
    }  
  }

}



// Declare and initialize the global instance of the OORENEO class
OORENEO ooreneo = OORENEO::getInstance();

