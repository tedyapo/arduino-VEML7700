#include <Wire.h>
#include <VEML7700.h>

VEML7700 als;

int16_t counter;

void setup()
{
  Serial.begin(9600);
  als.begin();
  als.setGain(VEML7700::ALS_GAIN_d8);
}



void loop()
{
  uint8_t status;
  float lux;
  status = als.getALSLux(lux);
  Serial.print(status, HEX);
  Serial.print(" ");
  Serial.println(lux);
  delay(200);
}
