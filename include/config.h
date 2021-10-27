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

// 7-Segment Display
#define SSegDIN     10      // DataIN Pin
#define SSegCLK     12      // Clock Pin
#define SSegCS      11      // Chip Select Pin
#define SSegNumDev  1       // Number of MAX72xx Devices

#endif