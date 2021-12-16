/*  
 *   Nextion Display Example to set text component
 *   Muthanna Alwahash 2021
 *   
 */

#ifndef _nextion_h_
#define _nextion_h_

//-------------------------------------------------------------------------------------------------------------
// Required Libraries
//-------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include <Si4703.h>

//-------------------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------------------
#define USE_SOFTWARE_SERIAL //Comment this line for use  HardwareSerial

#if defined(USE_SOFTWARE_SERIAL)
#include <SoftwareSerial.h>
#endif

//-------------------------------------------------------------------------------------------------------------
// Main Class
//-------------------------------------------------------------------------------------------------------------
class Nextion{
  private:
    #if defined(USE_SOFTWARE_SERIAL)
      SoftwareSerial *nextion;
    #else
      HardwareSerial *nextion;
    #endif

  public:
    Nextion(){};

    #if defined(USE_SOFTWARE_SERIAL)
      Nextion(SoftwareSerial &next, uint32_t baud);
    #else
      Nextion(HardwareSerial &next, uint32_t baud);
    #endif
    
    boolean       ackCommand(void);
    void          sendCommand(const char* cmd);
    boolean       init(const char* pageId = "0");
    boolean       setComponentText(String component, String txt);
    boolean       setComponentValue(String component, int value);
    void          updateDisplay(Si4703& radio);

};
#endif






