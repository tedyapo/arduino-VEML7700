# arduino-VEML7700
Arduino driver for VEML7700 ambient light sensor
# Connections
The VEML7700 uses I2C communication.  Connect it to the arduino SCL and SDA pins.

**Caution:** the VEML7700 runs on 3.6V maximum, and does not have 5V-tolerant I/O pins.  If you have a 3.3V arduino-compatible board, you can connect the SCL/SDA lines directly to the VEML7700.  Otherwise you will need a 5V-to-3.3V level converter.
# Usage
see examples/read_VEML7700 for a simple example
# API
The code exposes a low-level set of API functions corresponding to the command set described in the datasheet and a higher-level API for ease of use
## High-level API

    uint8_t VEML7700::getALSLux(float& lux);
samples the current ambient light value (photopic curve) using the current gain and integration time settings. Returns 0 on success, non-zero on failure.

    uint8_t VEML7700::getWhiteLux(float& lux);
samples the current ambient light value (white-like curve) using the current gain and integration time settings. Returns 0 on success, non-zero on failure.

    uint8_t VEML7700::getAutoALSLux(float& lux);
samples the current ambient light value (photopic curve) using auto-ranging algorithm described in application note. Returns 0 on success, non-zero on failure.

    uint8_t VEML7700::getAutoWhiteLux(float& lux);
samples the current ambient light value (white-like curve) using auto-ranging algorithm described in application note. Returns 0 on success, non-zero on failure.

    void VEML7700::sampleDelay();
delays (busy-wait) long enough to ensure a new sample is generated after the call is executed, based on current setting of integration time.
