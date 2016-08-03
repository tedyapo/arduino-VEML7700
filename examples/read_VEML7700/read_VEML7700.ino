#include <Wire.h>
#include <VEML7700.h>

VEML7700 als;

void setup()
{
  Serial.begin(9600);
  als.begin();
}

void loop()
{
  float lux;
  als.getALSLux(lux);
  Serial.println(lux);
  delay(200);
}
