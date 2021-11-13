#ifndef _nextion_h_
#define _nextion_h_

#include <Arduino.h>

#define USE_SOFTWARE_SERIAL //Comment this line for use  HardwareSerial

#if defined(USE_SOFTWARE_SERIAL)
#include <SoftwareSerial.h>
#endif

class Nextion{
  private:
    void flushSerial();
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

    void          buttonToggle(boolean &buttonState, String objName, uint8_t picDefualtId, uint8_t picPressedId);
    uint8_t       buttonOnOff(String find_component, String unknown_component, uint8_t pin, int btn_prev_state);
    boolean       ack(void);
    boolean       setComponentValue(String component, int value);
    unsigned int  getComponentValue(String component);
    boolean       setComponentText(String component, String txt);
    String        getComponentText(String component, uint32_t timeout = 100);
    boolean       updateProgressBar(int x, int y, int maxWidth, int maxHeight, int value, int emptyPictureID, int fullPictureID, int orientation=0);
    String        listen(unsigned long timeout=100);
    void          sendCommand(const char* cmd);
    uint8_t       pageId(void);
    boolean       init(const char* pageId = "0");

};
#endif






