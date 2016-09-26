## Overview
This is an Arduino library for the Texas Instruments ADS7828 12-bit, 8-channel I2C A/D converter. **Updated to support Arduino 1.0.**

## Features
The following features have been implemented:

  * Up to (4) A/D converters can be used on the same I2C bus (hardware-addressable via pins A0, A1 and software-addressable via ID 0..3)
  * A/D conversions may be initiated on a bus-, device-, or channel-specific level
  * Retrieve values as 16-period moving average or last sample
  * Built-in scaling function to return values in user-defined engineering units

## Hardware
This library has been tested with an Arduino [Duemilanove](http://www.arduino.cc/en/Main/ArduinoBoardDuemilanove) and a Texas Instruments [ADS7828](http://focus.ti.com/docs/prod/folders/print/ads7828.html) A/D converter.

## Installation
* Arduino 17 (or later):

    Determine the location of your sketchbook by selecting _File > Preferences_ from within the Arduino IDE. If you don't already have a _libraries_ folder within your sketchbook, create one and unzip the archive there. See [this](http://arduino.cc/blog/?p=313) for more information.

* Arduino 16 (or earlier):

    Download the zip file, extract and copy the _i2c_adc_ads7828_ folder to _ARDUINO\_HOME/hardware/libraries_. If you are upgrading from a previous version, be sure to delete i2c_adc_ads7828.o.

## Caveats
Conforms to Arduino IDE 1.5 Library Specification v2.1 which requires Arduino IDE >= 1.5.

## Support
Full [source code documentation](http://4-20ma.github.com/i2c_adc_ads7828/) is available. Please report any bugs on the [Issue Tracker](/4-20ma/i2c_adc_ads7828/issues).

## Questions/Feedback
I can be contacted at 4-20ma at wvfans dot net.

## Schematic
Below is a simplified schematic diagram. Refer to the datasheet for specific requirements.

```
                               Arduino
                              .--------------------.
                              |Duemilanove         |
                              |                    |
   TI ADS7828        .-------o|5V                  |
  .--------------.   |        |                    |
-o|1 CH0   VDD 16|o--'   .---o|GND                 |
  |              |       |    |                    |
-o|2 CH1   SDA 15|o------)---o|A4 SDA              |
  |              |       |    |                    |
-o|3 CH2   SCL 14|o------)---o|A5 SCL              |
  |              |       |    '--------------------'
-o|4 CH3    A1 13|o------o
  |              |       |
-o|5 CH4    A0 12|o------o
  |              |       |
-o|6 CH5   COM 11|o-     |
  |              |       |
-o|7 CH6   REF 10|o-     |
  |              |       |
-o|8 CH7   GND  9|o------o
  '--------------'       |
                         |
                        ===
                        GND
```

## Example
The library contains sketches that demonstrates use of the i2c\_adc\_ads7828 library. You can find these in the [examples](/4-20ma/i2c_adc_ads7828/tree/master/examples/) folder.

```c++
#include <i2c_adc_ads7828.h>
#include <Wire.h>


// device 0
// Address: A1=0, A0=0
// Command: SD=1, PD1=1, PD0=1
ADS7828 device(0, SINGLE_ENDED | REFERENCE_ON | ADC_ON, 0x0F);
ADS7828* adc = &device;
ADS7828Channel* ambientTemp = adc->channel(0);
ADS7828Channel* waterTemp = adc->channel(1);
ADS7828Channel* filterPressure = adc->channel(2);
ADS7828Channel* waterLevel = adc->channel(3);


void setup()
{
  // enable serial monitor
  Serial.begin(19200);
  
  // enable I2C communication
  ADS7828::begin();
  
  // adjust scaling on an individual channel basis
  ambientTemp->minScale = 0;
  ambientTemp->maxScale = 150;
  
  waterTemp->minScale = 0;
  waterTemp->maxScale = 100;
  
  filterPressure->minScale = 0;
  filterPressure->maxScale = 30;
  
  waterLevel->minScale = 0;
  waterLevel->maxScale = 100;
}


void loop()
{
  // update all registered ADS7828 devices/unmasked channels
  ADS7828::updateAll();
  
  // output moving average values to console
  Serial.print("\n Ambient: ");
  Serial.print(ambientTemp->value(), DEC);
  Serial.print("\n Water temp: ");
  Serial.print(waterTemp->value(), DEC);
  Serial.print("\n Filter pressure: ");
  Serial.print(filterPressure->value(), DEC);
  Serial.print("\n Water level: ");
  Serial.print(waterLevel->value(), DEC);
  Serial.print("\n- - - - - - - - - - - - - - - - - - - - \n");
  
  // delay
  delay(1000);
}
```
