
#include "nextion.h"

//-------------------------------------------------------------------------------------------------------------
// Check if Hardware or Software Serial to be used.
//-------------------------------------------------------------------------------------------------------------
#if defined(USE_SOFTWARE_SERIAL)
    Nextion::Nextion(SoftwareSerial &next, uint32_t baud): nextion(&next){
        nextion->begin(baud);
        flushSerial();
    }
#else
    Nextion::Nextion(HardwareSerial &next, uint32_t baud): nextion(&next){
        nextion->begin(baud);
        flushSerial();
    }
#endif

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void Nextion::buttonToggle(boolean &buttonState, String objName, uint8_t picDefualtId, uint8_t picSelected){
    String tempStr = "";
    if (buttonState) {
      tempStr = objName + ".picc="+String(picDefualtId);  // Select this picture
      sendCommand(tempStr.c_str());
      tempStr = "ref "+objName;     // Refresh component
      sendCommand(tempStr.c_str());
      buttonState = false;
    } else {
      tempStr = objName + ".picc="+String(picSelected);   // Select this picture
      sendCommand(tempStr.c_str());
      tempStr = "ref "+objName;   // Refresh this component
      sendCommand(tempStr.c_str());
      buttonState = true;
    }
} 

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
uint8_t Nextion::buttonOnOff(String find_component, String unknown_component, uint8_t pin, int btn_prev_state){  
    uint8_t btn_state = btn_prev_state;
    if((unknown_component == find_component) && (!btn_state)){
      btn_state = 1;  //Led is ON
      digitalWrite(pin, HIGH);
    }else if((unknown_component == find_component) && (btn_state)){
      btn_state = 0;
      digitalWrite(pin, LOW);
    }
    return btn_state;
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
boolean Nextion::setComponentValue(String component, int value){
    String compValue = component +".val=" + value;  //Set component value
    sendCommand(compValue.c_str());
    boolean acki = ack();
    return acki;
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
boolean Nextion::ack(void){
  uint8_t bytes[4] = {0};
  nextion->setTimeout(20);
  if (sizeof(bytes) != nextion->readBytes((char *)bytes, sizeof(bytes))){
    return 0;
  }
  if((bytes[1]==0xFF)&&(bytes[2]==0xFF)&&(bytes[3]==0xFF)){
      switch (bytes[0]) {
          case 0x00:
            return false; 
            break;
          case 0x01:
            return true; 
            break;
          default: 
            return false;
      }
  }
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
unsigned int Nextion::getComponentValue(String component){
  String getValue = "get "+ component +".val";//Get componetn value
    unsigned int value = 0;
  sendCommand(getValue.c_str());
  uint8_t temp[8] = {0};
  nextion->setTimeout(20);
  if (sizeof(temp) != nextion->readBytes((char *)temp, sizeof(temp))){
    return -1;
  }//end if
  if((temp[0]==(0x71))&&(temp[5]==0xFF)&&(temp[6]==0xFF)&&(temp[7]==0xFF)){
    value = (temp[4] << 24) | (temp[3] << 16) | (temp[2] << 8) | (temp[1]);//Little-endian convertion
  }//end if
  return value;
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
boolean Nextion::setComponentText(String component, String txt){
  String componentText = component + ".txt=\"" + txt + "\"";//Set Component text
  sendCommand(componentText.c_str());
  return ack();
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
boolean Nextion::updateProgressBar(int x, int y, int maxWidth, int maxHeight, int value, int emptyPictureID, int fullPictureID, int orientation){
	int w1 = 0;
	int h1 = 0;
	int w2 = 0;
	int h2 = 0;
	int offset1 = 0;
	int offset2 = 0;

	if(orientation == 0){ // horizontal
	value = map(value, 0, 100, 0, maxWidth);
	w1 = value;
	h1 = maxHeight;
	w2 = maxWidth - value;
	h2 = maxHeight;
	offset1 = x + value;
	offset2 = y;
	
	}else{
    value = map(value, 0, 100, 0, maxHeight);
    offset2 = y;	
    y = y + maxHeight - value;
    w1 = maxWidth;
    h1 = value;
    w2 = maxWidth;
    h2 = maxHeight - value;
    offset1 = x;
	}
	
	String wipe = "picq " + String(x) + "," + String(y) + "," + String(w1) + "," + String(h1) + "," + String(fullPictureID);
	sendCommand(wipe.c_str());
	wipe = "picq " + String(offset1) + "," + String(offset2) + "," + String(w2) + "," + String(h2) + "," + String(emptyPictureID);
	sendCommand(wipe.c_str());

	return ack();

}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
String Nextion::getComponentText(String component, uint32_t timeout){
  String tempStr = "get " + component + ".txt";
  sendCommand(tempStr.c_str());
  tempStr = "";
  tempStr = listen(timeout);
  return tempStr;
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
String Nextion::listen(unsigned long timeout){//returns generic

  char _bite;
  char _end = 0xff;//end of file x3
  String cmd;
  int countEnd = 0;

  while(nextion->available()>0){
    delay(10);
    if(nextion->available()>0){
      
      _bite = nextion->read();
      cmd += _bite;
      
      if(_bite == _end){
        countEnd++;
      }
    
      if(countEnd == 3){
        break;
      }
	  }

  }

  String temp = "";
  switch (cmd[0]) {
      case 'e':
          countEnd = 0;//Revision for not include last space " "
          for(uint8_t i = 0; i<cmd.length(); i++){
            if(cmd[i] == _end){
                countEnd++;
            }

            temp += String(cmd[i], HEX);
            
            if(countEnd == 3){
                return temp;
            }
            
            temp += " ";
          }
          break;
      case 'f':
          return String(cmd[1], DEC);
          break;
      case 'g':
          cmd = String(cmd[2], DEC) + "," + String(cmd[4], DEC) +","+ String(cmd[5], DEC);
          return cmd;
          break;
      case 'h':
          cmd = String(cmd[2], DEC) + "," + String(cmd[4], DEC) +","+ String(cmd[5], DEC);
          cmd = "68 " + cmd;	
          return cmd;
          break;
      case 'p':
          cmd = cmd.substring(1, cmd.length()-3);
          cmd = "70 " + cmd;
          return cmd;
          break;
      default: 
          return cmd;
          break;
  }
      return "";
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
uint8_t Nextion::pageId(void){
  
  sendCommand("sendme");
      
  String pagId = listen();
  
  if(pagId != ""){
	  return pagId.toInt();
  }
  
  return -1;
  
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void Nextion::sendCommand(const char* cmd){
  while (nextion->available()){
	    nextion->read();
  }
  nextion->print(cmd);
  nextion->write(0xFF);
  nextion->write(0xFF);
  nextion->write(0xFF);
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
boolean Nextion::init(const char* pageId){
  String page = "page " + String(pageId);
  sendCommand("");
  ack();
  sendCommand(page.c_str());
  delay(100);
  return ack();
}

//-------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------
void Nextion::flushSerial(){
  Serial.flush();
  nextion->flush();
}
