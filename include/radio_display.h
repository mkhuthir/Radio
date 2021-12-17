/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2021
 *   
 */

#ifndef _radio_display_h_
#define _radio_display_h_


//-------------------------------------------------------------------------------------------------------------
// Update Display
//-------------------------------------------------------------------------------------------------------------
void updateDisplay(Si4703& radio, Nextion& disp)
{
  disp.setComponentText("rssi", String(radio.getRSSI()));
  disp.setComponentText("freq", String(float(radio.getChannel())/100,2));
  // TODO: Add other parameters VOL,...
}

#endif