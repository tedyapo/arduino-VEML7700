// VEML7700 arduino driver
// Copyright 2016 Theodore C. Yapo

VEML7700::
VEML7700()
{
}

void 
VEML7700::
begin()
{
  setPower(1);
  delay(3);
  // write initial state to VEML7700
  register_cache[0] = 0x0000;
  register_cache[1] = 0x0000;
  register_cache[2] = 0x0000;
  register_cache[3] = 0x0000;
  register_cache[4] = 0x0000;
  register_cache[5] = 0x0000;
  for (uint8_t i=0; i<6; i++){
    sendData(i, regsiter_cache[i]);
  }
}

uint8_t
VEML7700::
sendData(uint8_t command, uint16_t data);
{
  Wire.beginTransmission(I2C_ADDRESS);
  if (Wire.write(command) != 1){
    return STATUS_ERROR;
  }
  if (Wire.write(uint8_t(data & 0xff)) != 1){
    return STATUS_ERROR;
  }
  if (Wire.write(uint8_t(data >> 8)) != 1){
    return STATUS_ERROR;
  }
  if (Wire.endTransmission()){
    return STATUS_ERROR;
  }
  return STATUS_OK;
}

uint8_t
VEML7700::
receiveData(uint8_t command, uint16_t& data)
{
  Wire.beginTransmission(I2C_ADDRESS);
  if (Wire.write(command) != 1){
    return STATUS_ERROR;
  }
  if (Wire.endTransmission()){
    return STATUS_ERROR;
  }
  if (Wire.requestFrom(I2C_ADDRESS, uint8_t(2)) != 2){
    return STATUS_ERROR;
  }
  data = Wire.read();
  data |= uint16_t(Wire.read()) << 8;
  return STATUS_OK;
}

uint8_t
VEML7700::
setGain(als_sens_t gain)
{
  uint16_t reg = ( (register_cache[COMMAND_ALS_SM] & ~ALS_SM_MASK) | 
                   ((uint16_t(sens) << ALS_SM_SHIFT) & ALS_SM_MASK) );
  register_cache[COMMAND_ALS_SM] = reg;
  return sendData(COMMAND_ALS_SM, reg);
}

uint8_t
VEML7700::
getGain(als_sens_t& gain)
{
  gain = (register_cache[COMMAND_ALS_SM] & ~ALS_SM_MASK) >> ALS_SM_SHIFT;
  return STATUS_OK;
}

uint8_t
VEML7700::
setIntegrationTime(als_itime_t itime)
{
  uint16_t reg = ( (register_cache[COMMAND_ALS_IT] & ~ALS_IT_MASK) | 
                   ((uint16_t(itime) << ALS_IT_SHIFT) & ALS_IT_MASK) );
  register_cache[COMMAND_ALS_IT] = reg;
  return sendData(COMMAND_ALS_IT, reg);
}

uint8_t
VEML7700::
getIntegrationTime(als_itime_t& itime)
{
  itime = (register_cache[COMMAND_ALS_IT] & ~ALS_IT_MASK) >> ALS_IT_SHIFT;
  return STATUS_OK;
}

uint8_t
VEML7700::
setPersistence(als_persist_t persist)
{
  uint16_t reg = ( (register_cache[COMMAND_ALS_PERS] & ~ALS_PERS_MASK) | 
                   ((uint16_t(persist) << ALS_PERS_SHIFT) & ALS_PERS_MASK) );
  register_cache[COMMAND_ALS_PERS] = reg;
  return sendData(COMMAND_ALS_PERS, reg);
}

uint8_t
VEML7700::
setPowerSavingMode(als_powmode_t powmode)
{
  uint16_t reg = ( (register_cache[COMMAND_PSM] & ~PSM_MASK) | 
                   ((uint16_t(powmode) << PSM_SHIFT) & PSM_MASK) );
  register_cache[COMMAND_PSM] = reg;
  return sendData(COMMAND_PSM, reg);
}

uint8_t
VEML7700::
setPowerSaving(uint8_t enabled)
{
  uint16_t reg = ( (register_cache[COMMAND_PSM_EN] & ~PSM_EN_MASK) | 
                   ((uint16_t(enabled) << PSM_EN_SHIFT) & PSM_EN_MASK) );
  register_cache[COMMAND_PSM_EN] = reg;
  return sendData(COMMAND_PSM_EN, reg);
}

uint8_t
VEML7700::
setInterrupts(uint8_t enabled)
{
  uint16_t reg = ( (register_cache[COMMAND_INT_EN] & ~PSM_INT_MASK) | 
                   ((uint16_t(enabled) << PSM_INT_SHIFT) & PSM_INT_MASK) );
  register_cache[COMMAND_INT_EN] = reg;
  return sendData(COMMAND_INT_EN, reg);
}

uint8_t
VEML7700::
setPower(uint8_t on)
{
  uint16_t reg = ( (register_cache[COMMAND_ALS_SD] & ~ALS_SD_MASK) | 
                   ((uint16_t(on) << ALS_SD_SHIFT) & ALS_SD_MASK) );
  register_cache[COMMAND_ALS_SD] = reg;
  return sendData(COMMAND_ALS_SD, reg);
}

uint8_t
VEML7700::
setALSHighThreshold(uint16_t thresh)
{
  return sendData(COMMAND_ALS_WH, thresh);
}

uint8_t
VEML7700::
setALSLowThreshold(uint16_t thresh)
{
  return sendData(COMMAND_ALS_WL, thresh);
}

uint8_t
VEML7700::
getALS(uint16_t& als)
{
  return readData(COMMAND_ALS, als);
}

uint8_t
VEML7700::
getWhite(uint16_t& white)
{
  return readData(COMMAND_WHITE, white);
}

uint8_t
VEML7700::
getHighThresholdEvent(uint8_t& event)
{
  uint16_t reg;
  uint8_t status = readData(ALS_IF_H, reg);
  event = (reg & ALS_IF_H_MASK) >> ALS_IF_H_SHIFT;
  return status;
}

uint8_t
VEML7700::
getLowThresholdEvent(uint8_t& event)
{
  uint16_t reg;
  uint8_t status = readData(ALS_IF_L, reg);
  event = (reg & ALS_IF_L_MASK) >> ALS_IF_L_SHIFT;
  return status;
}

void
VEML7700::
scaleLux(uint16_t raw_counts, float& lux)
{
  uint8_t gain;
  uint8_t itime;
  getGain(gain);
  getIntegrationTime(itime);
  
  float factor1, factor2;

  switch(gain & 0x3){
  case ALS_GAIN_x1:
    factor1 = 1.f;
    break;
  case ALS_GAIN_x2:
    factor1 = 2.f;
    break;
  case ALS_GAIN_d8:
    factor1 = 0.125f;
    break;
  case ALS_GAIN_d4:
    factor1 = 0.25f;
    break;
  }

  switch(itime){
  case ALS_INTEGRATION_25ms:
    factor2 = 0.2304f;
    break;
  case ALS_INTEGRATION_50ms:
    factor2 = 0.1152f;
    break;
  case ALS_INTEGRATION_100ms:
    factor2 = 0.0576f;
    break;
  case ALS_INTEGRATION_200ms:
    factor2 = 0.0288f;
    break;
  case ALS_INTEGRATION_400ms:
    factor2 = 0.0144f;
    break;
  case ALS_INTEGRATION_800ms:
    factor2 = 0.0072f;
    break;
  default:
    factor2 = 0.2304f;
    break;
  }

  lux = raw_counts * factor1 * factor2;

  // apply correction from App. Note. for raw lux > 1000
  //   using Horner's method
  float correction_thresh = 1000.f;
  if (lux > correction_thresh){
    lux = lux * (1.0023f + lux * (8.1488e-5f + lux * (-9.3924e-9f + 
                                                      lux * 6.0135e-13f)));
  }
}

uint8_t
VEML7700::
getALSLux(float& lux)
{
  uint16_t raw_counts;
  uint8_t status = getALS(raw_counts);
  scaleLux(raw_counts, lux);
  return status;
}

uint8_t
VEML7700::
getWhiteLux(float& lux)
{
  uint16_t raw_counts;
  uint8_t status = getWhite(raw_counts);
  scaleLux(raw_counts, lux);
  return status;
}

uint8_t
VEML7700::
getAutoXLux(float& lux, VEML7700::getCountsFunction counts_func)
{
  uint16_t raw_counts;
  uint8_t gains[4] = { ALS_GAIN_d8,
                       ALS_GAIN_d4,
                       ALS_GAIN_x1,
                       ALS_GAIN_x2 };
  uint8_t itimes[6] = {ALS_INTEGRATION_25ms,
                       ALS_INTEGRATION_50ms,
                       ALS_INTEGRATION_100ms,
                       ALS_INTEGRATION_200ms,
                       ALS_INTEGRATION_400ms,
                       ALS_INTEGRATION_800ms };

  uint16_t counts_threshold = 100;

  if (setPower(0)){
    return STATUS_ERROR;
  }
  for (int8_t itime_idx = 2; itime_idx < 6; itime_idx++){
    if (setIntegrationTime(itimes[itime_idx])){
      return STATUS_ERROR;
    }
    for (uint8_t gain_idx = 0; gain_idx < 4; gain_idx++){
      if (setGain(gains[gain_idx])){
        return STATUS_ERROR;
      }
      if (setPower(1)){
        return STATUS_ERROR;
      }
      // use pointer-to-member-function here??
      if (counts_func(raw_counts)){
        return STATUS_ERROR;
      }

      if (raw_counts > counts_threshold){
        do {
          if (raw_counts < 10000){
            scaleLux(raw_counts, lux);
            return STATUS_OK;  
          }
          if(setPower(0)){
            return STATUS_ERROR;
          }
          itime_idx--;
          if (setIntegrationTime(itimes[itime_idx])){
            return STATUS_ERROR;
          }          
          if (setPower(1)){
            return STATUS_ERROR;
          }
          if (counts_func(raw_counts)){
            return STATUS_ERROR;
          }
        } while (itime_idx > 0);
        scaleLux(raw_counts, lux);
        return STATUS_OK;  
      }
      if(setPower(0)){
        return STATUS_ERROR;
      }
    }
  }
  scaleLux(raw_counts, lux);
  return STATUS_OK;
}

uint8_t
VEML7700::
getAutoALSLux(float& lux)
{
  return getAutoXLux(float& lux, &VEML7700::getALS)
}

uint8_t
VEML7700::
getAutoWhiteLux(float& lux)
{
  return getAutoXLux(float& lux, &VEML7700::getWhite)
}
