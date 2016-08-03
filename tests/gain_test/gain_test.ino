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
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_100ms);

  als.setPower(0);
  als.setGain(VEML7700::ALS_GAIN_d8);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setGain(VEML7700::ALS_GAIN_d4);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setGain(VEML7700::ALS_GAIN_x1);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setGain(VEML7700::ALS_GAIN_x2);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.getAutoALSLux(lux);
  Serial.println(lux);

  delay(200);
}
