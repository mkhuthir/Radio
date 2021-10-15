/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include <radio.h>
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