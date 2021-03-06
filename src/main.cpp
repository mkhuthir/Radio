/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2021
 *   
 */

//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <Arduino.h>        // Arduino std lib
#include <SoftwareSerial.h> // To connect Nextion Dispaly 
#include <Wire.h>           // Used for Si4703 I2C interface.

#include <Si4703.h>         // library to control Silicon Labs' Si4703 FM Radio Receiver.
#include <nextion.h>        // library to drive Nextion Display

#include <radio_config.h>   // Configuration file
#include <radio_eeprom.h>   // To save configuration parameters such as channel and volume.
#include <radio_terminal.h> // Serial terminal I/O to display status and control radio
#include <radio_rotary.h>   // To drive the rotary encoder used for frquency tuning
#include <radio_display.h>  // to control radio display  

//-------------------------------------------------------------------------------------------------------------
// Initiate classes
//-------------------------------------------------------------------------------------------------------------
Si4703          radio;                      // FM Radio Class
SoftwareSerial  nextion(nexTX, nexRX);      // Software Serial Class
Nextion         nexDisp(nextion, nexBaud);  // Nextion Display Class

//-------------------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------------------
long previousMillis   = 0;            // will store last refresh time

//-------------------------------------------------------------------------------------------------------------
// Arduino initial Setup
//-------------------------------------------------------------------------------------------------------------
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
  updateDisplay(radio, nexDisp);

  // Capture Current Time
  previousMillis = millis();          // Remember the time

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
    updateDisplay(radio, nexDisp);      // Refresh display every time rotary is changed
  }
  
  // Updates from Serial Terminal
  if (Serial.available()){
    processCommand(radio);              // Radio control from serial interface
    printCurrentSettings(radio);
    updateDisplay(radio, nexDisp);      // Refresh display every time terminal command is received
  }
    
  // Periodical Refersh of Nextion Display to show RSSI and other changing paramters.
  if(millis() - previousMillis > nexRefresh) {
    previousMillis = millis();          // Remember the time
    updateDisplay(radio, nexDisp);      // Time to update Display
  }
  
  
}