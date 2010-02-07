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


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "i2c_adc_ads7828.h"


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
/**
Constructor.

Creates class object, sets up the A/D storage array.
*/
i2c_adc_ads7828::i2c_adc_ads7828()
{
  uint8_t i, j, k;
  
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 8; j++)
    {
      _u8Index[i][j] = _u16Total[i][j] = _u16ScaleMin[i][j] = 0;
      _u16ScaleMax[i][j] = 4095;
      
      for (k = 0; k < (1 << _ku8MovingAverageBits); k++)
      {
        _u16Sample[i][j][k] = 0;
      }
    }
  }
}


/**
Initialize class object.

Sets up the TWI/I2C interface.
Call once class has been instantiated, typically within setup().
*/
void i2c_adc_ads7828::begin()
{
  Wire.begin();
}


/**
Perform A/D conversion and return mean value.

@param u8Device ID/channel of A/D converter
@return moving average of sample (0x0000..0xFFFF)
*/
uint16_t i2c_adc_ads7828::AnalogRead(uint8_t u8Device)
{
  uint8_t i, j;
  uint16_t r;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  if (!BOUND(_u8Index[i][j], 0, (1 << _ku8MovingAverageBits) - 1))
  {
    _u8Index[i][j] = 0;
  }
  _u16Total[i][j] -= _u16Sample[i][j][_u8Index[i][j]];

  Wire.beginTransmission(_ku8BaseAddress | i);
  Wire.send(u8Device & 0b11111100);
  
  if (!Wire.endTransmission())
  {
    Wire.requestFrom(_ku8BaseAddress | i, 2);
    _u16Sample[i][j][_u8Index[i][j]] = (Wire.receive() << 8) | Wire.receive();
  }
  else
  {
    _u16Sample[i][j][_u8Index[i][j]] = 0;
  }

  _u16Total[i][j] += _u16Sample[i][j][_u8Index[i][j]];
  _u8Index[i][j]++;
  r = (_u16Total[i][j] >> _ku8MovingAverageBits);
  
  return map(r, 0, 4095, _u16ScaleMin[i][j], _u16ScaleMax[i][j]);
}


/**
Retrieve mean value of A/D conversion; no A/D conversion is performed.

@param u8Device ID/channel of A/D converter
@return moving average of sample (0x0000..0xFFFF)
*/
uint16_t i2c_adc_ads7828::GetAverage(uint8_t u8Device)
{
  uint8_t i, j;
  uint16_t r;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  r = (_u16Total[i][j] >> _ku8MovingAverageBits);
  
  return map(r, 0, 4095, _u16ScaleMin[i][j], _u16ScaleMax[i][j]);
}


/**
Retrieve channel number of A/D converter.

@param u8Device ID/channel of A/D converter
@return channel number of device ID/channel (0..7)
*/
uint8_t i2c_adc_ads7828::GetChannel(uint8_t u8Device)
{
  return (u8Device & 0b01110000) >> 4;
}


/**
Retrieve ID number of A/D converter.

@param u8Device ID/channel of A/D converter
@return ID number of device ID/channel (0..3)
*/
uint8_t i2c_adc_ads7828::GetId(uint8_t u8Device)
{
  return u8Device & 0b11;
}


/**
Retrieve mean index number of A/D converter.

@param u8Device ID/channel of A/D converter
@return index number of device ID/channel (0..2^_ku8MovingAverageBits)
*/
uint8_t i2c_adc_ads7828::GetIndex(uint8_t u8Device)
{
  uint8_t i, j;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  return (_u8Index[i][j] - 1);
}


/**
Retrieve most-recent sample from A/D converter.

@param u8Device ID/channel of A/D converter
@return most-recent analog sample without averaging (0x0000..0xFFFF)
*/
uint16_t i2c_adc_ads7828::GetSample(uint8_t u8Device)
{
  uint8_t i, j;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  return (_u16Sample[i][j][_u8Index[i][j] - 1]);
}


/**
Retrieve minimum scale of device ID/channel.

@param u8Device ID/channel of A/D converter
@return minimum scale of device ID/channel (0x0000..0xFFFF)
*/
uint16_t i2c_adc_ads7828::GetScaleMin(uint8_t u8Device)
{
  uint8_t i, j;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  return _u16ScaleMin[i][j];
}


/**
Retrieve maximum scale of device ID/channel.

@param u8Device ID/channel of A/D converter
@return maximum scale of device ID/channel (0x0000..0xFFFF)
*/
uint16_t i2c_adc_ads7828::GetScaleMax(uint8_t u8Device)
{
  uint8_t i, j;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  return _u16ScaleMax[i][j];
}


/**
Retrieve current running total of device ID/channel.

@param u8Device ID/channel of A/D converter
@return running total of device ID/channel (0x0000..0xFFFF)
*/
uint16_t i2c_adc_ads7828::GetTotal(uint8_t u8Device)
{
  uint8_t i, j;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  return _u16Total[i][j];
}


/**
Set minimum/maximum scale values for device ID/channel.

@param u8Device ID/channel of A/D converter
@param u16Min minimum scale (0x0000..0xFFFF)
@param u16Max maximum scale (0x0000..0xFFFF)
*/
void i2c_adc_ads7828::SetScale(uint8_t u8Device, uint16_t u16Min, uint16_t u16Max)
{
  uint8_t i, j;
  
  i = u8Device & 0b11;
  j = (u8Device & 0b01110000) >> 4;
  
  _u16ScaleMin[i][j] = u16Min;
  _u16ScaleMax[i][j] = u16Max;
}
