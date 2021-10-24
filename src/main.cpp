/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <Arduino.h>    // Arduino std lib
#include <Si4703.h>     // library to control Silicon Labs' Si4703 FM Radio Receiver.
#include <Wire.h>       // Used for I2C interface.

Si4703 radio;

#include <config.h>     // Configuration file
#include <eeprom_rw.h>  // To save configuration parameters such as channel and volume.
#include <terminal.h>   // Serial terminal I/O to display status and control radio
#include <rotary.h>     // To drive the rotary encoder used for frquency tuning


//-------------------------------------------------------------------------------------------------------------
// Arduino initial Setup
//-------------------------------------------------------------------------------------------------------------
void setup()
{
 
  Serial.begin(115200);       // start serial

  pinMode(LED1, OUTPUT);      // LED1 pin is output
  digitalWrite(LED1, LOW);    // turn LED1 OFF
  radio.start();              // start radio device
  read_EEPROM();              // load saved settings
  enableRotary();             // Enable rotary encoder

  // Show ready status
  digitalWrite(LED1, HIGH);           // turn LED1 ON
  radio.writeGPIO(GPIO1, GPIO_High);  // turn LED2 ON

  // Display info
  printWelcome();
  printHelp();
  printCurrentSettings();
  
}
//-------------------------------------------------------------------------------------------------------------
// Arduino main loop
//-------------------------------------------------------------------------------------------------------------
void loop()
{

  if (rotaryUpdated)      updateChannel();  // Interrupt tells us to update the station when updateStation=True
  if (Serial.available()) processCommand(); // Radio control from serial interface

}