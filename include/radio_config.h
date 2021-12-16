/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2021
 *   
 */

#ifndef _radio_config_h_
#define _radio_config_h_

//-------------------------------------------------------------------------------------------------------------
// Status LED used Pins
//-------------------------------------------------------------------------------------------------------------
#define LED1        5       // Arduino LED1 pin

//-------------------------------------------------------------------------------------------------------------
// Rotary Encoder used Pins
//-------------------------------------------------------------------------------------------------------------
#define rotaryPinA  2       // Rotary encoder A pin
#define rotaryPinB  3       // Rotary encoder B pin 
                            // Rotary encoder C must be connected to GND

//-------------------------------------------------------------------------------------------------------------
// Nextion Display
//-------------------------------------------------------------------------------------------------------------
#define nexTX       11      // Serial TX Pin
#define nexRX       10      // Serial RX Pin
#define nexBaud     9600    // Serial Baud Rate
#define nexRefresh  300     // Display refresh interval (milliseconds)

//-------------------------------------------------------------------------------------------------------------
// Terminal Serial Port
//-------------------------------------------------------------------------------------------------------------
#define serBaud     115200  // Serial Port Baud Rate

#endif