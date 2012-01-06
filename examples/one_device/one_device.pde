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
  ambientTemp->minScale = -40;
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
  Serial.print("\n Ambient: ");
  Serial.print(waterTemp->value(), DEC);
  Serial.print("\n Ambient: ");
  Serial.print(filterPressure->value(), DEC);
  Serial.print("\n Ambient: ");
  Serial.print(waterLevel->value(), DEC);
  Serial.print("\n- - - - - - - - - - - - - - - - - - - - \n");
  
  // delay
  delay(1000);
}

