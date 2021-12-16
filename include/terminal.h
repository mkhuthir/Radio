/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */

#ifndef _terminal_h_
#define _terminal_h_

//-------------------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------------------

// Favourate Channels fav_0..fav_9 (kHz)
int       fav_0 = 8760;   
int       fav_1 = 8820;   
int       fav_2 = 9140;  
int       fav_3 = 9220;   
int       fav_4 = 9390;  
int       fav_5 = 9440;   
int       fav_6 = 9500;  
int       fav_7 = 9760;   
int       fav_8 = 10480;  
int       fav_9 = 10740; 

//-------------------------------------------------------------------------------------------------------------
// Display Welcome Message.
//-------------------------------------------------------------------------------------------------------------
void printWelcome(Si4703& radio)
{
  Serial.println("\nWelcome...");
  
  Serial.print("\nPart Number\t:0x");
  Serial.println(radio.getPN(), HEX);

  Serial.print("Manufacturer ID\t:0x");
  Serial.println(radio.getMFGID(),HEX);

  Serial.print("Device\t\t:0x");
  Serial.println(radio.getDEV(),HEX);

  Serial.print("Chip Version\t:0x");
  Serial.println(radio.getREV(),HEX);

  Serial.print("Firmware Version:0x");
  Serial.println(radio.getFIRMWARE(),HEX);

  Serial.print("\nBand Start\t:");
  Serial.print(float(radio.getBandStart())/100,2);
  Serial.println(" MHz");

  Serial.print("Band End\t:");
  Serial.print(float(radio.getBandEnd())/100,2);
  Serial.println(" MHz");

  Serial.print("Band Space\t:");
  Serial.print(float(radio.getBandSpace())/100,2);
  Serial.println(" MHz");
}
//-------------------------------------------------------------------------------------------------------------
// Display current settings such as channel and volume.
//-------------------------------------------------------------------------------------------------------------
void printCurrentSettings(Si4703& radio)
{
   Serial.print("| Ch:");
   Serial.print(float(radio.getChannel())/100,2);
   Serial.print(" MHz | RSSI:");
   Serial.print(radio.getRSSI());
   Serial.print(" | VOL:");
   Serial.print(radio.getVolume());
   Serial.print(" | VolExt:");
   Serial.print(radio.getVolExt());
   Serial.print(" | ST:");
   Serial.print(radio.getST());
   Serial.print(" | DMute:");
   Serial.print(radio.getMute());
   Serial.print(" | DMono:");
   Serial.print(radio.getMono());
   Serial.println(" |");
}

//-------------------------------------------------------------------------------------------------------------
// Prints Favourite Stations List
//-------------------------------------------------------------------------------------------------------------
void printFavouriteList()
{
  Serial.println("List of Favourite Stations");
  
  Serial.print("0 - ");
  Serial.print(float(fav_0)/100,2);
  Serial.println(" MHz");

  Serial.print("1 - ");
  Serial.print(float(fav_1)/100,2);
  Serial.println(" MHz");

  Serial.print("2 - ");
  Serial.print(float(fav_2)/100,2);
  Serial.println(" MHz");

  Serial.print("3 - ");
  Serial.print(float(fav_3)/100,2);
  Serial.println(" MHz");

  Serial.print("4 - ");
  Serial.print(float(fav_4)/100,2);
  Serial.println(" MHz");

  Serial.print("5 - ");
  Serial.print(float(fav_5)/100,2);
  Serial.println(" MHz");

  Serial.print("6 - ");
  Serial.print(float(fav_6)/100,2);
  Serial.println(" MHz");

  Serial.print("7 - ");
  Serial.print(float(fav_7)/100,2);
  Serial.println(" MHz");

  Serial.print("8 - ");
  Serial.print(float(fav_8)/100,2);
  Serial.println(" MHz");

  Serial.print("9 - ");
  Serial.print(float(fav_9)/100,2);
  Serial.println(" MHz");

}

//-------------------------------------------------------------------------------------------------------------
// Display Help on commands.
//-------------------------------------------------------------------------------------------------------------
void printHelp()
{
  Serial.println("\n----------- Volume Control --------------");
  Serial.println("+ -     inc/Dec Volume (max 15)");
  Serial.println("e       Enable/Disable Volume 30dB Ext");
  Serial.println("m       Mute/Un-mute volume");
  Serial.println("s       Set Mono/Sterio");
  Serial.println("----------- Tuneing -----------------------");
  Serial.println("u d     Frequency up / down");
  Serial.println("n l     Channel Seek next / last");
  Serial.println("0..9    Favourite stations");
  Serial.println("f       Prints Favourite stations list");
  Serial.println("----------- RDS & Info --------------------");
  Serial.println("r       Listen for RDS Data");
  Serial.println("i       Prints current settings");
  Serial.println("h       Prints this help");
  Serial.println("-------------------------------------------");
  Serial.println("Select a command:");
}

//-------------------------------------------------------------------------------------------------------------
// Process a command from serial terminal
//-------------------------------------------------------------------------------------------------------------
void processCommand(Si4703& radio)
{
  digitalWrite(LED1, LOW);            // turn LED1 OFF
  radio.writeGPIO(GPIO1, GPIO_Low);   // turn LED2 OFF

  char ch = Serial.read();        // Read command
  Serial.print(" ");              // print single space after reading a command
  
  switch (ch)
  {
    case '+':                              // Increment Volume (max 15)
      radio.incVolume();
      break;

    case '-':                              // Decrement Volume (min 0)
      radio.decVolume();
      break;

    case 'm':                              // Mute/Un-mute volume
      radio.setMute(!radio.getMute());     // flip status
      break;

    case 'e':                              // Enable/Disable Volume 30dB Ext
      radio.setVolExt(!radio.getVolExt()); // flip status
      break;

    case 's':                              // Set Mono/Sterio"
      radio.setMono(!radio.getMono());     // flip status
      break;
    
    case 'u':                              // Tune Frequency up
      radio.incChannel();                  // increment freq
      break;

    case 'd':                              // Tune Frequency DOWN
      radio.decChannel();                  // decrement freq
      break;
   
    case 'n':                               // Channel Seek next
      if (!radio.seekUp()) Serial.println("| Error: Seek failure or band limit reached!!");
      break;

    case 'l':                               // Channel Seek last
      if (!radio.seekDown()) Serial.println("| Error: Seek failure or band limit reached!!");
      break;

    case '0':                               // Tune to favorite channel 
      radio.setChannel(fav_0);
      break;

    case '1':                               // Tune to favorite channel 
      radio.setChannel(fav_1);
      break;

    case '2':                               // Tune to favorite channel 
      radio.setChannel(fav_2);
      break;
    
    case '3':                               // Tune to favorite channel 
      radio.setChannel(fav_3);
      break;
    
    case '4':                               // Tune to favorite channel 
      radio.setChannel(fav_4);
      break;
    
    case '5':                               // Tune to favorite channel 
      radio.setChannel(fav_5);
      break;
    
    case '6':                               // Tune to favorite channel 
      radio.setChannel(fav_6);
      break;
    
    case '7':                               // Tune to favorite channel 
      radio.setChannel(fav_7);
      break;
    
    case '8':                               // Tune to favorite channel 
      radio.setChannel(fav_8);
      break;
    
    case '9':                               // Tune to favorite channel 
      radio.setChannel(fav_9);
      break;

    case 'r':           // Listen for RDS Data
      // TODO:
      break;

    case 'i':           // Print current settings
      // do nothing, setting will be printed after breaking out
      break;

    case 'f':                               // List favorite channels
      printFavouriteList();
      break;

    case 'h':                               // Print Help
      printHelp();
      break;

    default:
      Serial.print("Unknown command:'");
      Serial.print(ch);
      Serial.print("'");
      Serial.println(" send 'h' for help.");
      break;
  }
  
  write_EEPROM(radio);                // Save channel to EEPROM
  digitalWrite(LED1, HIGH);           // When done turn LED1 On
  radio.writeGPIO(GPIO1, GPIO_High);  // and turn LED2 ON
}
#endif
