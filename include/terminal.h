/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
#ifndef terminal_h
#define terminal_h

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
void printWelcome()
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
void printCurrentSettings()
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
  Serial.println("m       Mute/Unmute volume");
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
void processCommand()
{
  
  char ch = Serial.read();        // Read command
  Serial.print(" ");              // print single space after reading a command
  
  
  if (ch == '+')                  // Increment Volume (max 15)
    {
      radio.incVolume();
      printCurrentSettings();
    }
  else if (ch == '-')             // Decrement Volume (min 0)
    {
      radio.decVolume();
      printCurrentSettings();
    }
  
  else if (ch == 'e')             // Enable/Disable Volume 30dB Ext
    {
      radio.setVolExt(!radio.getVolExt()); // flip status
      printCurrentSettings();
    } 
  else if (ch == 'm')             // Mute/Unmute volume
    {
      radio.setMute(!radio.getMute()); // flip status
      printCurrentSettings();
    }
  else if (ch == 's')             // Set Mono/Sterio"
    {
      radio.setMono(!radio.getMono()); // flip status
      printCurrentSettings();
    }
  else if (ch == 'u')             // Tune Frequency up
    {
      digitalWrite(LED1, LOW);           // turn LED1 OFF
      radio.writeGPIO(GPIO1, GPIO_Low);  // turn LED2 OFF
      radio.incChannel();
      write_EEPROM();                    // Save channel to EEPROM
      printCurrentSettings();
      digitalWrite(LED1, HIGH);          // When done turn LED1 On
      radio.writeGPIO(GPIO1, GPIO_High); // turn LED2 ON
    } 
  else if (ch == 'd')             // Tune Frequency down
    {
      digitalWrite(LED1, LOW);           // turn LED1 OFF
      radio.writeGPIO(GPIO1, GPIO_Low);  // turn LED2 OFF
      radio.decChannel();
      write_EEPROM();                    // Save channel to EEPROM
      printCurrentSettings();
      digitalWrite(LED1, HIGH);          // When done turn LED1 On
      radio.writeGPIO(GPIO1, GPIO_High); // turn LED2 ON
    } 
  else if (ch == 'n')             // Channel Seek next
    {
      digitalWrite(LED1, LOW);           // turn LED1 OFF
      radio.writeGPIO(GPIO1, GPIO_Low);  // turn LED2 OFF
      if (!radio.seekUp()) 
        {
          Serial.println("| Error: Seek failure or band limit reached!!");
        }
      else
        {
          write_EEPROM();                    // Save channel to EEPROM
          printCurrentSettings();
        }
      digitalWrite(LED1, HIGH);          // When done turn LED1 On
      radio.writeGPIO(GPIO1, GPIO_High); // turn LED2 ON
    } 
  else if (ch == 'l')             // Channel Seek last
    {
      digitalWrite(LED1, LOW);           // turn LED1 OFF
      radio.writeGPIO(GPIO1, GPIO_Low);  // turn LED2 OFF
      if (!radio.seekDown())
        {
          Serial.println("| Error: Seek failure or band limit reached!!");
        }
      else
        {
          write_EEPROM();                    // Save channel to EEPROM
          printCurrentSettings();
        }
      digitalWrite(LED1, HIGH);          // When done turn LED1 On
      radio.writeGPIO(GPIO1, GPIO_High); // turn LED2 ON
    } 
  else if (ch == '0')             // Tune to favorite channel 0
    {
      radio.setChannel(fav_0);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '1')             // Tune to favorite channel 1
    {
      radio.setChannel(fav_1);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '2')             // Tune to favorite channel 2
    {
      radio.setChannel(fav_2);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '3')             // Tune to favorite channel 3
    {
      radio.setChannel(fav_3);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '4')             // Tune to favorite channel 4
    {
      radio.setChannel(fav_4);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '5')             // Tune to favorite channel 5
    {
      radio.setChannel(fav_5);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '6')             // Tune to favorite channel 6
    {
      radio.setChannel(fav_6);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '7')             // Tune to favorite channel 7
    {
      radio.setChannel(fav_7);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '8')             // Tune to favorite channel 8
    {
      radio.setChannel(fav_8);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == '9')             // Tune to favorite channel 9
    {
      radio.setChannel(fav_9);
      write_EEPROM();             // Save channel to EEPROM
      printCurrentSettings();
    }
  else if (ch == 'r')             // Listen for RDS Data
    {
      // TODO:
    }
  else if (ch == 'i')             // Print current settings
    {
      printCurrentSettings();
    }
  else if (ch == 'f')             // List favorite channels
    {
      printFavouriteList();
    }
  else if (ch == 'h')             // Print help
    {
      printHelp();
    }
  else                            // Unknown commands
    {
      Serial.print("Unknown command:'");
      Serial.print(ch);
      Serial.print("'");
      Serial.println(" send 'h' for help.");
    }
}
#endif
