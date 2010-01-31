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
  Copyright © 2009, 2010 Doc Walker <dfwmountaineers at gmail dot com>
  $Id$
  
*/


#ifndef i2c_adc_ads7828_h
#define i2c_adc_ads7828_h


/* _____STANDARD INCLUDES____________________________________________________ */
// include types & constants of Wiring core API
#include "WProgram.h"

// include twi/i2c library
#include <Wire.h>


/* _____UTILITY MACROS_______________________________________________________ */
// Macro to check if a value is within bounds (Min <= Value <= Max ?)
#define BOUND(Value,Min,Max)    (((Value)>=(Min))&&((Value)<=(Max)))


/* _____LOCAL DEFINITIONS____________________________________________________ */
// i2c address (main, each module)
#define i2c_adc_ads7828_ADR          0x48
#define i2c_adc_ads7828_ID0          0x0
#define i2c_adc_ads7828_ID1          0x1
#define i2c_adc_ads7828_ID2          0x2
#define i2c_adc_ads7828_ID3          0x3

// number of bits to define moving average (4 == 2^4 samples)
#define i2c_adc_ads7828_MOV_AVG_BITS 4

// Command byte PD1 bit (internal reference on or off between conversions)
#define i2c_adc_ads7828_REF_ON       bit(3)

// Command byte PD0 bit (A/D converter on or off between conversions)
#define i2c_adc_ads7828_ADC_ON       bit(2)

// channel selection for differential inputs
#define i2c_adc_ads7828_CH0_1        (0b0000 << 4)
#define i2c_adc_ads7828_CH2_3        (0b0001 << 4)
#define i2c_adc_ads7828_CH4_5        (0b0010 << 4)
#define i2c_adc_ads7828_CH6_7        (0b0011 << 4)
#define i2c_adc_ads7828_CH1_0        (0b0100 << 4)
#define i2c_adc_ads7828_CH3_2        (0b0101 << 4)
#define i2c_adc_ads7828_CH5_4        (0b0110 << 4)
#define i2c_adc_ads7828_CH7_6        (0b0111 << 4)

// channel selection for single-ended inputs
#define i2c_adc_ads7828_CH0          (0b1000 << 4)
#define i2c_adc_ads7828_CH2          (0b1001 << 4)
#define i2c_adc_ads7828_CH4          (0b1010 << 4)
#define i2c_adc_ads7828_CH6          (0b1011 << 4)
#define i2c_adc_ads7828_CH1          (0b1100 << 4)
#define i2c_adc_ads7828_CH3          (0b1101 << 4)
#define i2c_adc_ads7828_CH5          (0b1110 << 4)
#define i2c_adc_ads7828_CH7          (0b1111 << 4)


/* _____CLASS DEFINITIONS____________________________________________________ */
class i2c_adc_ads7828
{
  public:
    // constructor
    i2c_adc_ads7828();
    
    // initialize twi/i2c interface
    void begin();
    
    // read analog value from A/D converter
    uint16_t analogRead(uint8_t);
    
    // get mean value for device
    uint16_t getAverage(uint8_t);
    
    // get channel number of device (0..7)
    uint8_t getChannel(uint8_t);
    
    // get ID number of device (0..3)
    uint8_t getId(uint8_t);
    
    // get current index number for device
    uint8_t getIndex(uint8_t);
    
    // get most-recent analog sample from device array
    uint16_t getSample(uint8_t);
    
    // get minimum scale for device
    uint16_t getScaleMin(uint8_t);
    
    // get maximum scale for device
    uint16_t getScaleMax(uint8_t);
    
    // get current running total for device
    uint16_t getTotal(uint8_t);
    
    // set min/max scale values for device
    void setScale(uint8_t, uint16_t, uint16_t);
    
    
  private:
    // analog array index
    uint8_t _u8Index[4][8];
    
    // analog array totals
    uint16_t _u16Total[4][8];
    
    // analog array samples
    uint16_t _u16Sample[4][8][1 << i2c_adc_ads7828_MOV_AVG_BITS];
    
    // analog array minimum scales
    uint16_t _u16ScaleMin[4][8];
    
    // analog array maximum scales
    uint16_t _u16ScaleMax[4][8];
};
#endif
