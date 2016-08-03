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
  als.setGain(VEML7700::ALS_GAIN_d8);


  als.setPower(0);
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_25ms);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_50ms);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_100ms);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_200ms);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_400ms);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.setPower(0);
  als.setIntegrationTime(VEML7700::ALS_INTEGRATION_800ms);
  als.setPower(1);
  als.sampleDelay();
  als.getALSLux(lux);
  Serial.print(lux);
  Serial.print(" ");

  als.getAutoALSLux(lux);
  Serial.println(lux);

  delay(200);
}
