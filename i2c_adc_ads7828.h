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
  Copyright © 2009-2011 Doc Walker <dfwmountaineers at gmail dot com>
  
*/


#ifndef i2c_adc_ads7828_h
#define i2c_adc_ads7828_h


/* _____STANDARD INCLUDES____________________________________________________ */
// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// include twi/i2c library
#include <Wire.h>


/* _____UTILITY MACROS_______________________________________________________ */
/**
@def BOUND(Value,Min,Max).
Macro to check if a value is within bounds (Min <= Value <= Max ?)
*/
#define BOUND(Value,Min,Max)    (((Value)>=(Min))&&((Value)<=(Max)))


/* _____CLASS DEFINITIONS____________________________________________________ */
/**
Arduino class library for TI ADS7828 I2C A/D converter.

The ADS7828 is a single-supply, low-power, 12-bit data acquisition 
device that features a serial I2C interface and an 8-channel 
multiplexer. The Analog-to-Digital (A/D) converter features a 
sample-and-hold amplifier and internal, asynchronous clock. The 
combination of an I2C serial, 2-wire interface and micropower 
consumption makes the ADS7828 ideal for applications requiring the A/D 
converter to be close to the input source in remote locations and for 
applications requiring isolation. The ADS7828 is available in a TSSOP-16 
package. 
@author Doc Walker
@date 5 Feb 2010
@todo implement PD0/PD1 bits (_kbReferenceOn, _kbADCOn)
*/
class i2c_adc_ads7828
{
  public:
    i2c_adc_ads7828();
    
    void     begin();
    uint16_t analogRead(uint8_t);
    uint16_t getAverage(uint8_t);
    uint8_t  getChannel(uint8_t);
    uint8_t  getId(uint8_t);
    uint8_t  getIndex(uint8_t);
    uint16_t getSample(uint8_t);
    uint16_t getScaleMin(uint8_t);
    uint16_t getScaleMax(uint8_t);
    uint16_t getTotal(uint8_t);
    void     setScale(uint8_t, uint16_t, uint16_t);
    
    /**
    I2C address offset for ADS7828 module (A1=0, A0=0).
    */
    static const uint8_t ku8DeviceID0                    = 0b00;
    
    /**
    I2C address offset for ADS7828 module (A1=0, A0=1).
    */
    static const uint8_t ku8DeviceID1                    = 0b01;
    
    /**
    I2C address offset for ADS7828 module (A1=1, A0=0).
    */
    static const uint8_t ku8DeviceID2                    = 0b10;
    
    /**
    I2C address offset for ADS7828 module (A1=1, A0=1).
    */
    static const uint8_t ku8DeviceID3                    = 0b11;
    
    /**
    Channel selection for differential inputs (CH0/1).
    */
    static const uint8_t ku8DeviceCH0_1                  = (0b0000 << 4);
    
    /**
    Channel selection for differential inputs (CH1/0).
    */
    static const uint8_t ku8DeviceCH1_0                  = (0b0100 << 4);
    
    /**
    Channel selection for differential inputs (CH2/3).
    */
    static const uint8_t ku8DeviceCH2_3                  = (0b0001 << 4);
    
    /**
    Channel selection for differential inputs (CH3/2).
    */
    static const uint8_t ku8DeviceCH3_2                  = (0b0101 << 4);
    
    /**
    Channel selection for differential inputs (CH4/5).
    */
    static const uint8_t ku8DeviceCH4_5                  = (0b0010 << 4);
    
    /**
    Channel selection for differential inputs (CH5/4).
    */
    static const uint8_t ku8DeviceCH5_4                  = (0b0110 << 4);
    
    /**
    Channel selection for differential inputs (CH6/7).
    */
    static const uint8_t ku8DeviceCH6_7                  = (0b0011 << 4);
    
    /**
    Channel selection for differential inputs (CH7/6).
    */
    static const uint8_t ku8DeviceCH7_6                  = (0b0111 << 4);
    
    /**
    Channel selection for single-ended inputs (CH0).
    */
    static const uint8_t ku8DeviceCH0                    = (0b1000 << 4);
    
    /**
    Channel selection for single-ended inputs (CH1).
    */
    static const uint8_t ku8DeviceCH1                    = (0b1100 << 4);
    
    /**
    Channel selection for single-ended inputs (CH2).
    */
    static const uint8_t ku8DeviceCH2                    = (0b1001 << 4);
    
    /**
    Channel selection for single-ended inputs (CH3).
    */
    static const uint8_t ku8DeviceCH3                    = (0b1101 << 4);
    
    /**
    Channel selection for single-ended inputs (CH4).
    */
    static const uint8_t ku8DeviceCH4                    = (0b1010 << 4);
    
    /**
    Channel selection for single-ended inputs (CH5).
    */
    static const uint8_t ku8DeviceCH5                    = (0b1110 << 4);
    
    /**
    Channel selection for single-ended inputs (CH6).
    */
    static const uint8_t ku8DeviceCH6                    = (0b1011 << 4);
    
    /**
    Channel selection for single-ended inputs (CH7).
    */
    static const uint8_t ku8DeviceCH7                    = (0b1111 << 4);

  private:
    /**
    I2C base address for the ADS7828 module.
    */
    static const uint8_t _ku8BaseAddress                 = 0x48;
    
    /**
    Number of samples to be averaged = 2^_ku8MovingAverageBits.
    */
    static const uint8_t _ku8MovingAverageBits           = 4;
    
    /**
    Internal reference on or off between conversions (Command byte PD1 bit).
    @note Not currently in use.
    */
    static const bool _kbReferenceOn                     = bit(3);
    
    /**
    A/D converter on or off between conversions (Command byte PD0 bit).
    @note Not currently in use.
    */
    static const bool _kbADCOn                           = bit(2);
    
    /**
    Analog array indices for each analog device.
    */
    uint8_t _u8Index[4][8];
    
    /**
    Analog array totals for each analog device.
    */
    uint16_t _u16Total[4][8];
    
    /**
    Analog array samples for each analog device.
    */
    uint16_t _u16Sample[4][8][1 << _ku8MovingAverageBits];
    
    /**
    Analog array minimum scales for each analog device.
    */
    uint16_t _u16ScaleMin[4][8];
    
    /**
    Analog array maximum scales for each analog device.
    */
    uint16_t _u16ScaleMax[4][8];
};
#endif

/**
@example examples/ADS7828_example/ADS7828_example.pde
*/
