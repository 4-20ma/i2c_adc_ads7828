/**
@file
Arduino library for TI ADS7828 I2C A/D converter.
*/
/*

  i2c_adc_ads7828.h - Arduino library for TI ADS7828 I2C A/D converter
  
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


#ifndef i2c_adc_ads7828_h
#define i2c_adc_ads7828_h


/* _____ STANDARD INCLUDES ________________________________________________ */
// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


/* _____ PROJECT INCLUDES _________________________________________________ */
// include twi/i2c library
#include <Wire.h>


/* _____ UTILITY MACROS ___________________________________________________ */


/* _____ CONSTANTS ________________________________________________________ */
static const uint8_t kDifferential        = 0 << 7; // SD == 0
static const uint8_t kSingleEnded         = 1 << 7; // SD == 1
static const uint8_t kReferenceOn         = 1 << 3; // PD1 == 1
static const uint8_t kADCOn               = 1 << 2; // PD0 == 1
static const uint8_t kDefaultChannelMask  = 0xFF;
static const uint16_t kDefaultMinScale    = 0;
static const uint16_t kDefaultMaxScale    = 0xFFF;


/* _____ CLASS DEFINITIONS ________________________________________________ */
// SD is channel-specific
// PD1, PD0 are device-specific
class ADS7828;
class ADS7828Channel
{
  public:
    // class functions
    // (none)
    
    // constructors
    ADS7828Channel() {};
    ADS7828Channel(uint8_t, uint8_t, uint16_t, uint16_t);
    
    // instance functions
    uint8_t commandByte();        // returns channel command byte (0x0000..0xFFF0)
    ADS7828* device();            // getter method for parent device pointer
    uint8_t id();                 // returns channel ID number (0..7)
    uint8_t index();              // getter method for array index
    void newSample(uint16_t);     // setter method updates index, totalizer
    void reset();                 // resets moving average index, totalizer
    uint16_t sample();            // getter method for sample
    uint8_t start();              // getter method calls parent device start() method
    uint16_t total();             // getter method for totalizer
    // uint8_t update();           // getter method TODO
    uint16_t value();             // getter method for moving average value
    
    // instance variables
    ADS7828* _device;             // pointer to parent device
    uint16_t maxScale;            // maximum scale (0x0000..0xFFFF, defaults to 0x0FFF)
    uint16_t minScale;            // minimum scale (0x0000..0xFFFF, defaults to 0x0000)
  
  private:
    // instance functions
    // (none)
    
    // instance variables
    uint8_t _commandByte;         // command byte for channel (SD C2 C1 C0 bits only)
    uint8_t _index;               // index of sample array (0..
    uint16_t _samples[1 << 4];    // array of sampled values
    uint16_t _total;              // running total of sample array
    static const uint8_t _kMovingAverageBits = 4;
};


class ADS7828
{
  public:
    // class functions
    static void begin();
    static uint16_t read(uint8_t);
    static uint8_t start(uint8_t, uint8_t);
    static uint8_t update(); // all devices, all unmasked channels
    static uint8_t update(ADS7828*); // single device, all unmasked channels
    static uint8_t update(ADS7828*, uint8_t); // single device, single channel
    
    // constructors
    ADS7828(uint8_t);
    ADS7828(uint8_t, uint8_t);
    ADS7828(uint8_t, uint8_t, uint8_t);
    ADS7828(uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
    
    // instance functions
    uint8_t address();
    ADS7828Channel* channel(uint8_t);       // channel getter method
    uint16_t read();
    uint8_t start();
    uint8_t start(uint8_t);
    uint8_t update(uint8_t); // single device, single channel
    
    // instance variables
    uint8_t channelMask;                    // mask of active channels
  
  private:
    // instance functions
    void init(uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
    
    // instance variables
    uint8_t _address;                       // address of device A1 A0
    ADS7828Channel _channels[8];            // array of channel objects
    uint8_t _channelIndex;                  // index of current channel
    uint8_t _commandByte;                   // PD1 PD0 bits only
    static ADS7828* _devices[4];
    static const uint8_t _kDeviceBaseAddress = 0x48;
};
#endif
