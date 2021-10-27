/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */

#ifndef sevenseg_h
#define sevenseg_h

#include "LedControl.h"



void sSegStart(const LedControl& lc){
  
  lc.shutdown(0,false); // The MAX72XX is in power-saving mode on startup,we have to do a wakeup call
  lc.setIntensity(0,8); // Set the brightness to a medium values
  lc.clearDisplay(0);   // and clear the display

}

void sSegDemo(const LedControl& lc){
  
  lc.setChar(0,0,'a',false);
  
}

#endif