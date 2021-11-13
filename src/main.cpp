/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <Arduino.h>        // Arduino std lib
#include <Si4703.h>         // library to control Silicon Labs' Si4703 FM Radio Receiver.
#include <Wire.h>           // Used for I2C interface.
#include <config.h>         // Configuration file
#include <eeprom_rw.h>      // To save configuration parameters such as channel and volume.
#include <terminal.h>       // Serial terminal I/O to display status and control radio
#include <rotary.h>         // To drive the rotary encoder used for frquency tuning
#include <SoftwareSerial.h> // To connect Nextion Dispaly 
#include "nextion.h"        // To drive Nextion Display
//-------------------------------------------------------------------------------------------------------------
// Initiate classes
//-------------------------------------------------------------------------------------------------------------

Si4703          radio;                  // FM Radio Class
SoftwareSerial  nextion(11, 10);        // Software Serial Class (TX to pin 11 and RX to pin 10)
Nextion         nexDisp(nextion, 9600); // Nextion Display Class

//-------------------------------------------------------------------------------------------------------------
// Arduino initial Setup
//-------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);       // start serial
  nexDisp.init();             // start Nextion Display

  pinMode(LED1, OUTPUT);      // LED1 pin is output
  digitalWrite(LED1, LOW);    // turn LED1 OFF
  radio.start();              // start radio device
  read_EEPROM(radio);         // load saved settings
  enableRotary();             // Enable rotary encoder

  // Show ready status
  digitalWrite(LED1, HIGH);           // turn LED1 ON
  radio.writeGPIO(GPIO1, GPIO_High);  // turn LED2 ON

  // Display info
  printWelcome(radio);
  printHelp();
  printCurrentSettings(radio);

  // Test Nextion Display
  nexDisp.setComponentText("rssi", "90");
  delay(50);
  nexDisp.setComponentText("freq", "104.90");
  delay(50);
  
}
//-------------------------------------------------------------------------------------------------------------
// Arduino main loop
//-------------------------------------------------------------------------------------------------------------
void loop()
{

  if (rotaryUpdated)      updateChannel(radio);  // Interrupt tells us to update the station when updateStation=True
  if (Serial.available()) processCommand(radio); // Radio control from serial interface
}