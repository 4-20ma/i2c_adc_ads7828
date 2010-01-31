/*

  ADS7828_example.pde - example using i2c_adc_ads7828 library
  
  This file is part of i2c_adc_ads7828.
  
  i2c_adc_ads7828 is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  i2c_adc_ads7828 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with i2c_adc_ads7828.  If not, see <http://www.gnu.org/licenses/>.
  
  Written by Doc Walker (Rx)
  Copyright © 2009, 2010 Doc Walker <dfwmountaineers at gmail dot com>
  $Id$
  
*/


#include <Wire.h>
#include <i2c_adc_ads7828.h>


// instantiate i2c_adc_ads7828 object
i2c_adc_ads7828 ai;


// examples of constants representing ID/channel combinations
// this list is not all-inclusive; ID range 0..3, channel range 0..7
// ID 0
#define SENSOR00     (i2c_adc_ads7828_ID0 + i2c_adc_ads7828_CH0)
#define SENSOR01     (i2c_adc_ads7828_ID0 + i2c_adc_ads7828_CH1)
#define SENSOR02     (i2c_adc_ads7828_ID0 + i2c_adc_ads7828_CH2)
#define SENSOR07     (i2c_adc_ads7828_ID0 + i2c_adc_ads7828_CH7)

// ID 1
#define SENSOR10     (i2c_adc_ads7828_ID1 + i2c_adc_ads7828_CH0)
#define SENSOR11     (i2c_adc_ads7828_ID1 + i2c_adc_ads7828_CH1)
#define SENSOR12     (i2c_adc_ads7828_ID1 + i2c_adc_ads7828_CH2)
#define SENSOR17     (i2c_adc_ads7828_ID1 + i2c_adc_ads7828_CH7)

// ID 2
#define SENSOR20     (i2c_adc_ads7828_ID2 + i2c_adc_ads7828_CH0)

// ID 3
#define SENSOR30     (i2c_adc_ads7828_ID3 + i2c_adc_ads7828_CH0)


void setup()
{
  // initialize serial interface
  Serial.begin(19200);
  
  // initialize i2c interface
  ai.begin();
  
  // set scale of each channel; defaults to 0..4095 if not set explicitly
  // set scale of sensor id0, channel 2 to 0..20000
  ai.setScale(SENSOR02, 0, 4095);
  
  // set scale of sensor id0, channel 7 to 0..2500
  ai.setScale(SENSOR07, 0, 2500);
  
  // set scale of sensor id1, channel 0 to 0..100
  ai.setScale(SENSOR10, 0, 100);
}


void loop()
{
  serialPrint(SENSOR02, ai.analogRead(SENSOR02));
  serialPrint(SENSOR07, ai.analogRead(SENSOR07));
  serialPrint(SENSOR10, ai.analogRead(SENSOR10));
  serialPrint(SENSOR20, ai.analogRead(SENSOR20));
  serialPrint(SENSOR30, ai.analogRead(SENSOR30));
  Serial.print("\n- - - - - - - - - - - - - - - - - - - - ");
  delay(1000);
}


void serialPrint(uint8_t u8Device, uint16_t u16Value)
{
  Serial.print("\nID");
  Serial.print(ai.getId(u8Device), HEX);
  Serial.print(", CH");
  Serial.print(ai.getChannel(u8Device), HEX);
  Serial.print(" - I: ");
  Serial.print(ai.getIndex(u8Device), DEC);
  Serial.print(", S: ");
  Serial.print(ai.getSample(u8Device), DEC);
  Serial.print(", T: ");
  Serial.print(ai.getTotal(u8Device), DEC);
  Serial.print(", A: ");
  Serial.print(u16Value, DEC);
}

