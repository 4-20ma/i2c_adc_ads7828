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
  Copyright © 2009-2012 Doc Walker <dfwmountaineers at gmail dot com>
  
*/


/* _____ PROJECT INCLUDES _________________________________________________ */
#include "i2c_adc_ads7828.h"


/* _____ ADS7828Channel CONSTRUCTORS ______________________________________ */
ADS7828Channel::ADS7828Channel(uint8_t id, uint8_t options, uint16_t min,
  uint16_t max)
{
  this->_commandByte = (bitRead(options, 7) << 7) | (bitRead(id, 0) << 6) |
    (bitRead(id, 2) << 5) | (bitRead(id, 1) << 4);
  this->minScale = min;
  this->maxScale = max;
  reset();
}


/* _____ ADS7828Channel PUBLIC INSTANCE FUNCTIONS _________________________ */
uint8_t ADS7828Channel::commandByte()
{
  return _commandByte;
}


ADS7828* ADS7828Channel::device()
{
  return _device;
}


uint8_t ADS7828Channel::id()
{
  return ((bitRead(_commandByte, 5) << 2) | (bitRead(_commandByte, 4) << 1) |
    (bitRead(_commandByte, 6)));
}


// TODO: test this function
uint8_t ADS7828Channel::index()
{
  return _index;
}


// TODO: test this function
void ADS7828Channel::newSample(uint16_t sample)
{
  _index++;
  if (_index >= (1 << _kMovingAverageBits)) _index = 0;
  // {
  //   _index = 0;
  // }
  _total -= _samples[_index];
  _samples[_index] = sample;
  _total += _samples[_index];
}


// TODO: test this function
void ADS7828Channel::reset()
{
  _index = _total = 0;
  for (uint8_t k = 0; k < (1 << _kMovingAverageBits); k++)
  {
    _samples[k] = 0;
  }
}


// TODO: test this function
uint16_t ADS7828Channel::sample()
{
  return map(_samples[_index], kDefaultMinScale, kDefaultMaxScale, minScale,
    maxScale);
}


// TODO: is this needed?
uint8_t ADS7828Channel::start()
{
  return _device->start(id());
}


// TODO: test this function
uint16_t ADS7828Channel::total()
{
  return _total;
}


// // TOOD: implement this function
// // TODO: implement sample index inc, total -=, += as methods
// uint8_t ADS7828Channel::update()
// {
//   uint8_t ret = start();
//   if (0 == ret)
//   {
//     if (BOUND(_index, 0, (1 << _kMovingAverageBits) - 2))
//     {
//       _index++;
//     }
//     else
//     {
//       _index = 0;
//     }
//     _total -= _samples[_index];
//     _samples[_index] = _device->read();
//     _total += _samples[_index];
//   }
//   return ret;
// }


// TODO: test this function
uint16_t ADS7828Channel::value()
{
  uint16_t r = (_total >> _kMovingAverageBits);
  return map(r, kDefaultMinScale, kDefaultMaxScale, minScale, maxScale);
}


/* _____ ADS7828 PUBLIC CLASS FUNCTIONS ___________________________________ */
void ADS7828::begin()
{
  Wire.begin();
}


uint16_t ADS7828::read(uint8_t address)
{
  Wire.requestFrom(_kDeviceBaseAddress | (address & 0b11), 2);
#if defined(ARDUINO) && ARDUINO >= 100
  return word(Wire.read(), Wire.read());
#else
  return word(Wire.receive(), Wire.receive());
#endif
}


uint8_t ADS7828::start(uint8_t address, uint8_t command)
{
  Wire.beginTransmission(_kDeviceBaseAddress | (address & 0b11));
#if defined(ARDUINO) && ARDUINO >= 100
  Wire.write((uint8_t) command);
#else
  Wire.send((uint8_t) command);
#endif
  return Wire.endTransmission();
}


// TODO: test this function
uint8_t ADS7828::update()
{
  uint8_t d, ch, count = 0;
  for (d = 0; d < 4; d++)
  {
    if (_devices[d]) count += update(_devices[d]);
    // for (ch = 0; ch < 8; ch++)
    // {
    //   if (_devices[d] && bitRead(_devices[d]->channelMask, ch))
    //   {
    //     if (0 == update(_devices[d], ch)) count++;
    //   }
    // }
  }
  return count;
}


// TODO: test this function
uint8_t ADS7828::update(ADS7828* device)
{
  uint8_t ch, count = 0;
  for (ch = 0; ch < 8; ch++)
  {
    if (bitRead(device->channelMask, ch))
    {
      if (0 == update(device, ch)) count++;
    }
  }
  return count;
}


// TODO: test this function
uint8_t ADS7828::update(ADS7828* device, uint8_t ch)
{
  uint8_t status = device->start(ch);
  if (0 == status) device->channel(ch)->newSample(device->read());
  return status;
  // for (uint8_t ch = 0; ch < 8; ch++)
  // {
  //   if (bitRead(channelMask, ch))
  //   {
  //     update(ch);
  //   }
  // }
}


/* _____ ADS7828 CONSTRUCTORS _____________________________________________ */
ADS7828::ADS7828(uint8_t address)
{
  init(address, (kSingleEnded | kADCOn | kReferenceOn), kDefaultChannelMask,
    kDefaultMinScale, kDefaultMaxScale);
}


ADS7828::ADS7828(uint8_t address, uint8_t options)
{
  init(address, options, kDefaultChannelMask, kDefaultMinScale,
    kDefaultMaxScale);
}


ADS7828::ADS7828(uint8_t address, uint8_t options, uint8_t channelMask)
{
  init(address, options, channelMask, kDefaultMinScale, kDefaultMaxScale);
}


ADS7828::ADS7828(uint8_t address, uint8_t options, uint8_t channelMask,
  uint16_t min, uint16_t max)
{
  init(address, options, channelMask, min, max);
}


/* _____ ADS7828 PUBLIC INSTANCE FUNCTIONS ________________________________ */
uint8_t ADS7828::address()
{
  return _address;
}


ADS7828Channel* ADS7828::channel(uint8_t ch)
{
  return &_channels[ch & 0b111];
}


uint16_t ADS7828::read()
{
  return read(_address);
}


uint8_t ADS7828::start()
{
  return start(0);
}


uint8_t ADS7828::start(uint8_t ch)
{
  return start(_address, _commandByte | channel(ch)->commandByte());
}


uint8_t ADS7828::update(uint8_t ch)
{
  return update(this, ch);
  // uint8_t status = start(ch);
  // if (0 == status) channel(ch)->newSample(read());
  // return status;
}


/* _____ ADS7828 PRIVATE INSTANCE FUNCTIONS _______________________________ */
void ADS7828::init(uint8_t address, uint8_t options,
  uint8_t channelMask, uint16_t min, uint16_t max)
{
  this->_address = address & 0b11;        // A1 A0 bits
  this->_commandByte = options & 0b1100;  // PD1 PD0 bits
  this->channelMask = channelMask;
  for (uint8_t ch = 0; ch < 8; ch++)
  {
    // if (bitRead(channelMask, i)) {}
    _channels[ch] = ADS7828Channel(ch, options, min, max);
    _channels[ch]._device = this;
  }
  this->_devices[_address] = this;
}


/* _____ ADS7828 PRIVATE CLASS VARIABLES __________________________________ */
ADS7828* ADS7828::_devices[] = {};

