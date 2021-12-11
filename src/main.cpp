/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <Arduino.h>        // Arduino std lib
#include <config.h>         // Configuration file
#include <Wire.h>           // Used for Si4703 I2C interface.
#include <Si4703.h>         // library to control Silicon Labs' Si4703 FM Radio Receiver.
#include <eeprom_rw.h>      // To save configuration parameters such as channel and volume.
#include <terminal.h>       // Serial terminal I/O to display status and control radio
#include <rotary.h>         // To drive the rotary encoder used for frquency tuning
#include <SoftwareSerial.h> // To connect Nextion Dispaly 
#include "nextion.h"        // To drive Nextion Display
//-------------------------------------------------------------------------------------------------------------
// Initiate classes
//-------------------------------------------------------------------------------------------------------------
Si4703          radio;                      // FM Radio Class
SoftwareSerial  nextion(nexTX, nexRX);      // Software Serial Class
Nextion         nexDisp(nextion, nexBaud);  // Nextion Display Class
//-------------------------------------------------------------------------------------------------------------
// Arduino initial Setup
//-------------------------------------------------------------------------------------------------------------


long previousMillis = 0;            // will store last time
long interval       = 1000;         // interval (milliseconds)
long currentMillis;                 // will store current time


void setup()
{


  pinMode(LED1, OUTPUT);              // LED1 pin is output
  digitalWrite(LED1, LOW);            // turn LED1 OFF
  radio.writeGPIO(GPIO1, GPIO_Low);   // turn LED2 OFF

  Serial.begin(serBaud);              // start serial
  nexDisp.init();                     // start Nextion Display

  radio.start();                      // start radio device
  read_EEPROM(radio);                 // load saved settings
  enableRotary();                     // Enable rotary encoder

  // Display info
  printWelcome(radio);
  printHelp();
  printCurrentSettings(radio);
  nexDisp.updateDisplay(radio);

  // Show ready status
  digitalWrite(LED1, HIGH);           // turn LED1 ON
  radio.writeGPIO(GPIO1, GPIO_High);  // turn LED2 ON
}
//-------------------------------------------------------------------------------------------------------------
// Arduino main loop
//-------------------------------------------------------------------------------------------------------------
void loop()
{
  // Updates from Rotary Encoder
  if (rotaryUpdated){
    updateChannel(radio);               // Interrupt tells us to update the station when updateStation=True
    printCurrentSettings(radio);
    nexDisp.updateDisplay(radio);
  }
  
  // Updates from Serial Terminal
  if (Serial.available()){
    processCommand(radio);              // Radio control from serial interface
    printCurrentSettings(radio);
    nexDisp.updateDisplay(radio);
  }
    
  // Refersh Nextion Display
  currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    nexDisp.updateDisplay(radio);
  }
  
  previousMillis = currentMillis;       // Remember the time
}