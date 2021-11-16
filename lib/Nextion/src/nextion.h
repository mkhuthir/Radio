#ifndef _nextion_h_
#define _nextion_h_

#include <Arduino.h>
#include <Si4703.h>

#define USE_SOFTWARE_SERIAL //Comment this line for use  HardwareSerial

#if defined(USE_SOFTWARE_SERIAL)
#include <SoftwareSerial.h>
#endif

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






