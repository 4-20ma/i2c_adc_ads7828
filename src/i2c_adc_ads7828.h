/// \file
/// Arduino library for TI ADS7828 I2C A/D converter.
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
  Copyright © 2009-2012 Doc Walker <4-20ma at wvfans dot net>
  
*/


/// \mainpage Arduino library for TI ADS7828 I2C A/D converter.
/// 
/// The ADS7828 is a single-supply, low-power, 12-bit data acquisition 
/// device that features a serial I2C interface and an 8-channel 
/// multiplexer. The Analog-to-Digital (A/D) converter features a 
/// sample-and-hold amplifier and internal, asynchronous clock. The 
/// combination of an I2C serial, 2-wire interface and micropower 
/// consumption makes the ADS7828 ideal for applications requiring the A/D 
/// converter to be close to the input source in remote locations and for 
/// applications requiring isolation. The ADS7828 is available in a TSSOP-16 
/// package. 
/// \author Doc Walker
/// \version \verbinclude VERSION
/// \date 29 Dec 2012
/// \copyright GNU General Public License v3
/// \par Source Code Repository:
///   https://github.com/4-20ma/i2c_adc_ads7828
/// \par Programming Style Guidelines:
///   http://geosoft.no/development/cppstyle.html
/// \par Schematic:
///   \verbinclude SCHEMATIC


#ifndef i2c_adc_ads7828_h
#define i2c_adc_ads7828_h

// _________________________________________________________ STANDARD INCLUDES
// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// __________________________________________________________ PROJECT INCLUDES
// include twi/i2c library
#include <Wire.h>


// ____________________________________________________________ UTILITY MACROS


// _________________________________________________________________ CONSTANTS
/// Configure channels to use differential inputs (Command byte SD=0).
/// Use either \ref DIFFERENTIAL or \ref SINGLE_ENDED in ADS7828
///   constructor; default is \ref DIFFERENTIAL.
/// \par Usage:
/// \code
/// ...
/// // address 0, differential inputs, ref/ADC OFF between conversions
/// ADS7828 adc0(0, DIFFERENTIAL | REFERENCE_OFF | ADC_OFF);
/// ...
/// \endcode
/// \relates ADS7828
static const uint8_t DIFFERENTIAL        = 0 << 7; // SD == 0


/// Configure channels to use single-ended inputs (Command byte SD=1).
/// Use either \ref DIFFERENTIAL or \ref SINGLE_ENDED in ADS7828
///   constructor; default is \ref DIFFERENTIAL.
/// \par Usage:
/// \code
/// ...
/// // address 1, single-ended inputs, ref/ADC OFF between conversions
/// ADS7828 adc1(1, SINGLE_ENDED | REFERENCE_OFF | ADC_OFF);
/// ...
/// \endcode
/// \relates ADS7828
static const uint8_t SINGLE_ENDED         = 1 << 7; // SD == 1


/// Configure channels to turn internal reference OFF between conversions (Command byte PD1=0).
/// Use either \ref REFERENCE_OFF or \ref REFERENCE_ON in ADS7828
///   constructor; default is \ref REFERENCE_OFF.
/// \par Usage:
/// \code
/// ...
/// // address 0, differential inputs, ref/ADC OFF between conversions
/// ADS7828 adc0(0, DIFFERENTIAL | REFERENCE_OFF | ADC_OFF);
/// ...
/// \endcode
/// \relates ADS7828
static const uint8_t REFERENCE_OFF        = 0 << 3; // PD1 == 0


/// Configure channels to turn internal reference ON between conversions (Command byte PD1=1).
/// Use either \ref REFERENCE_OFF or \ref REFERENCE_ON in ADS7828
///   constructor; default is \ref REFERENCE_OFF.
/// \par Usage:
/// \code
/// ...
/// // address 2, differential inputs, ref ON/ADC OFF between conversions
/// ADS7828 adc2(2, DIFFERENTIAL | REFERENCE_ON | ADC_OFF);
/// ...
/// \endcode
/// \relates ADS7828
static const uint8_t REFERENCE_ON         = 1 << 3; // PD1 == 1


/// Configure channels to turn A/D converter OFF between conversions (Command byte PD0=0).
/// Use either \ref ADC_OFF or \ref ADC_ON in ADS7828
///   constructor; default is \ref ADC_OFF.
/// \par Usage:
/// \code
/// ...
/// // address 0, differential inputs, ref/ADC OFF between conversions
/// ADS7828 adc0(0, DIFFERENTIAL | REFERENCE_OFF | ADC_OFF);
/// ...
/// \endcode
/// \relates ADS7828
static const uint8_t ADC_OFF              = 0 << 2; // PD0 == 0


/// Configure channels to turn A/D converter ON between conversions (Command byte PD0=1).
/// Use either \ref ADC_OFF or \ref ADC_ON in ADS7828
///   constructor; default is \ref ADC_OFF.
/// \par Usage:
/// \code
/// ...
/// // address 3 , differential inputs, ref OFF/ADC ON between conversions
/// ADS7828 adc3(3, DIFFERENTIAL | REFERENCE_OFF | ADC_ON);
/// ...
/// \endcode
/// \relates ADS7828
static const uint8_t ADC_ON               = 1 << 2; // PD0 == 1


/// Default channel mask used in ADS7828 constructor.
/// \relates ADS7828
static const uint8_t DEFAULT_CHANNEL_MASK  = 0xFF;


/// Default scaling minimum value used in ADS7828 constructor.
/// \relates ADS7828
static const uint16_t DEFAULT_MIN_SCALE    = 0;


/// Default scaling maximum value used in ADS7828 constructor.
/// \relates ADS7828
static const uint16_t DEFAULT_MAX_SCALE    = 0xFFF;


// _________________________________________________________ CLASS DEFINITIONS
class ADS7828;
class ADS7828Channel
{
  public:
    // ............................................... public member functions
    ADS7828Channel() {};
    ADS7828Channel(ADS7828* const, uint8_t, uint8_t, uint16_t, uint16_t);
    uint8_t commandByte();
    ADS7828* device();
    uint8_t id();
    uint8_t index();
    void newSample(uint16_t);
    void reset();
    uint16_t sample();
    uint8_t start();
    uint16_t total();
    uint8_t update();
    uint16_t value();
    
    // ........................................ static public member functions
    
    // ..................................................... public attributes
    /// Maximum value of moving average (defaults to 0x0FFF).
    /// \par Usage:
    /// \code
    /// ...
    /// ADS7828 device(0);
    /// ADS7828Channel* temperature = device.channel(0);
    /// uint16_t old = temperature->maxScale; // get current value and/or
    /// temperature->maxScale = 100;          // set new value
    /// ...
    /// \endcode
    uint16_t maxScale;
    
    /// Minimum value of moving average (defaults to 0x0000).
    /// \par Usage:
    /// \code
    /// ...
    /// ADS7828 device(0);
    /// ADS7828Channel* temperature = device.channel(0);
    /// uint16_t old = temperature->minScale; // get current value and/or
    /// temperature->minScale = 0;            // set new value
    /// ...
    /// \endcode
    uint16_t minScale;
    
    // .............................................. static public attributes
  
  private:
    // .............................................. private member functions
    
    // ....................................... static private member functions
    
    // .................................................... private attributes
    /// Command byte for channel object (SD C2 C1 C0 bits only).
    uint8_t commandByte_;
    
    /// Pointer to parent device object.
    ADS7828* device_;
    
    /// Index position within moving average array. 
    uint8_t index_;
    
    /// Array of (unscaled) sample values.
    /// \note Bit shift must match \ref MOVING_AVERAGE_BITS_.
    uint16_t samples_[1 << 4];
    
    /// (Unscaled) running total of moving average array elements.
    uint16_t total_;
    
    // ............................................. static private attributes
    /// Quantity of samples to be averaged =
    ///   2<sup>\ref MOVING_AVERAGE_BITS_</sup>.
    /// \note \ref MOVING_AVERAGE_BITS_ must match \ref samples_ bit shift.
    static const uint8_t MOVING_AVERAGE_BITS_ = 4;
};


class ADS7828
{
  public:
    // ............................................... public member functions
    ADS7828(uint8_t);
    ADS7828(uint8_t, uint8_t);
    ADS7828(uint8_t, uint8_t, uint8_t);
    ADS7828(uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
    uint8_t address();
    ADS7828Channel* channel(uint8_t);
    uint8_t commandByte();
    uint8_t start();
    uint8_t start(uint8_t);
    uint8_t update(); // single device, all unmasked channel
    uint8_t update(uint8_t); // single device, single channel
    
    // ........................................ static public member functions
    static void begin();
    static ADS7828* device(uint8_t);
    static uint8_t updateAll(); // all devices, all unmasked channels
    
    // ..................................................... public attributes
    /// Each bit position containing a 1 represents a channel that is to be
    /// read via update() / updateAll().
    uint8_t channelMask;                    // mask of active channels
    
    // .............................................. static public attributes
  
  private:
    // .............................................. private member functions
    void init(uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
    uint16_t read();
    
    // ....................................... static private member functions
    static uint16_t read(uint8_t);
    static uint8_t start(uint8_t, uint8_t);
    static uint8_t update(ADS7828*); // single device, all unmasked channels
    static uint8_t update(ADS7828*, uint8_t); // single device, single channel
    
    // .................................................... private attributes
    /// Device address as defined by pins A1, A0
    uint8_t address_;
    
    /// Array of channel objects.
    ADS7828Channel channels_[8];
    
    /// Command byte for device object (PD1 PD0 bits only).
    uint8_t commandByte_;
    
    // ............................................. static private attributes
    /// Array of pointers to registered device objects.
    static ADS7828* devices_[4];
    
    /// Factory pre-set slave address.
    static const uint8_t BASE_ADDRESS_ = 0x48;
};
#endif
/// \example examples/one_device/one_device.pde
/// \example examples/two_devices/two_devices.pde
