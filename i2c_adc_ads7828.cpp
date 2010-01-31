/*

  i2c_adc_ads7828.cpp - Arduino library for TI i2c_adc_ads7828 I2C A/D converter
  
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


/* _____STANDARD INCLUDES____________________________________________________ */
#include "Wire.h"


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "i2c_adc_ads7828.h"


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
// constructor
i2c_adc_ads7828::i2c_adc_ads7828()
{
  uint8_t i, j, k;
  
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 8; j++)
    {
      index[i][j] = total[i][j] = scaleMin[i][j] = 0;
      scaleMax[i][j] = 4095;
      for (k = 0; k < (1 << i2c_adc_ads7828_MOV_AVG_BITS); k++)
      {
        sample[i][j][k] = 0;
      }
    }
  }
//  Wire.begin(); // do not call this from constructor function; screws up i2c bus communication byte order when using multiple i2c libraries
}


// read analog value from A/D converter
uint16_t i2c_adc_ads7828::analogRead(uint8_t device)
{
  uint8_t i, j;
  uint16_t r;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  if (!BOUND(index[i][j], 0, (1 << i2c_adc_ads7828_MOV_AVG_BITS) - 1))
  {
    index[i][j] = 0;
  }
  total[i][j] -= sample[i][j][index[i][j]];

  Wire.beginTransmission(i2c_adc_ads7828_ADR | i);
  Wire.send(device & 0b11111100);
  
  if (!Wire.endTransmission())
  {
    Wire.requestFrom(i2c_adc_ads7828_ADR | i, 2);
    sample[i][j][index[i][j]] = (Wire.receive() << 8) | Wire.receive();
  }
  else
  {
    sample[i][j][index[i][j]] = 0;
  }

  total[i][j] += sample[i][j][index[i][j]];
  index[i][j]++;
  r = (total[i][j] >> i2c_adc_ads7828_MOV_AVG_BITS);
  
  return map(r, 0, 4095, scaleMin[i][j], scaleMax[i][j]);
}


// get average value for device
uint16_t i2c_adc_ads7828::getAverage(uint8_t device)
{
  uint8_t i, j;
  uint16_t r;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  r = (total[i][j] >> i2c_adc_ads7828_MOV_AVG_BITS);
  
  return map(r, 0, 4095, scaleMin[i][j], scaleMax[i][j]);
}


// get channel number of device (0..7)
uint8_t i2c_adc_ads7828::getChannel(uint8_t device)
{
  return (device & 0b01110000) >> 4;
}


// get ID number of device (0..3)
uint8_t i2c_adc_ads7828::getId(uint8_t device)
{
  return device & 0b11;
}


// get current index number for device
uint8_t i2c_adc_ads7828::getIndex(uint8_t device)
{
  uint8_t i, j;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  return (index[i][j] - 1);
}


// get most-recent analog sample from device array
uint16_t i2c_adc_ads7828::getSample(uint8_t device)
{
  uint8_t i, j;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  return (sample[i][j][index[i][j] - 1]);
}


// get minimum scale for device
uint16_t i2c_adc_ads7828::getScaleMin(uint8_t device)
{
  uint8_t i, j;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  return scaleMin[i][j];
}


// get maximum scale for device
uint16_t i2c_adc_ads7828::getScaleMax(uint8_t device)
{
  uint8_t i, j;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  return scaleMax[i][j];
}


// get current running total for device
uint16_t i2c_adc_ads7828::getTotal(uint8_t device)
{
  uint8_t i, j;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  return total[i][j];
}


// set min/max scale values for device
void i2c_adc_ads7828::setScale(uint8_t device, uint16_t newMin, uint16_t newMax)
{
  uint8_t i, j;
  
  i = device & 0b11;
  j = (device & 0b01110000) >> 4;
  
  scaleMin[i][j] = newMin;
  scaleMax[i][j] = newMax;
}


// instantiate object
i2c_adc_ads7828 ai = i2c_adc_ads7828();
