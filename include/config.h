/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
#ifndef config_h
#define config_h

// Used Pins
// Status LED
#define LED1        5       // LED1 pin

// Rotary Encoder
#define rotaryPinA  2       // Rotary encoder A pin
#define rotaryPinB  3       // Rotary encoder B pin 
                            // Rotary encoder C must be connected to GND

// Nextion Display
#define nexTX     11        // Serial TX Pin
#define nexRX     10        // Serial RX Pin
#define nexBaud   9600      // Serial Baud Rate

#define serBaud   115200    // Serial Port Baud Rate
#endif