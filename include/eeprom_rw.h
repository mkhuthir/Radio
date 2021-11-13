/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
#ifndef eeprom_rw_h
#define eeprom_rw_h

//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <EEPROM.h> // To save configuration parameters such as channel and volume.

//-------------------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------------------
// EEPROM Usage Map
#define eeprom_chn_msb  1
#define eeprom_chn_lsb  2
#define eeprom_vol      3

//-------------------------------------------------------------------------------------------------------------
// Read settings from EEPROM
//-------------------------------------------------------------------------------------------------------------
void read_EEPROM(Si4703& radio)
{
  // Read channel value
  int MSB = EEPROM.read(eeprom_chn_msb);  // load the msb into one 8-bit register
  int LSB = EEPROM.read(eeprom_chn_lsb);  // load the lsb into one 8-bit register
  radio.setChannel((MSB << 8)|LSB);       // concatenate the lsb and msb
  
  // Read Volume
  radio.setVolume(EEPROM.read(eeprom_vol));
}

//-------------------------------------------------------------------------------------------------------------
// Write current settings to EEPROM
//-------------------------------------------------------------------------------------------------------------
void write_EEPROM(Si4703& radio)
{
  // Save current channel value
  int chan = radio.getChannel();      // get the current channel
  int msb = chan >> 8;                // move channel over 8 spots to grab MSB
  int lsb = chan & 0x00FF;            // clear the MSB, leaving only the LSB
  EEPROM.write(eeprom_chn_msb, msb);  // write each byte to a single 8-bit position
  EEPROM.write(eeprom_chn_lsb, lsb);

  // Save volume
  EEPROM.write(eeprom_vol, radio.getVolume());
  
}

#endif