/*  
 *   Terminal Controlled Radio using Si7403 Lib
 *   Muthanna Alwahash 2020
 *   
 */
#ifndef rotary_h
#define rotary_h

// Direction
#define UP    true
#define DOWN  false

//-------------------------------------------------------------------------------------------------------------
// Volatile variables for use in Rotary Encoder Interrupt Routine
//-------------------------------------------------------------------------------------------------------------
volatile int      rotaryLast      = 0b00;
volatile boolean  rotaryDirection = UP;
volatile boolean  rotaryUpdated   = false;

//-------------------------------------------------------------------------------------------------------------
// Interrupt handler that reads the encoder. It set the updateStation flag when a new indent is found 
//-------------------------------------------------------------------------------------------------------------
void updateRotary()
{
  int pinA = digitalRead(rotaryPinA); // read current value of Pin A
  int pinB = digitalRead(rotaryPinB); // read current value of Pin B

  int rotaryCurrent = (pinA << 1) | pinB;            // converting the 2 pins values to single number
  int pattern = (rotaryLast << 2) | rotaryCurrent;   // adding it to the previous encoded value

  if(pattern == 0b1101 || pattern == 0b0100 || pattern == 0b0010 || pattern == 0b1011)
    {
      rotaryDirection = DOWN;
      rotaryUpdated   = true;
    }
  
  if(pattern == 0b1110 || pattern == 0b0111 || pattern == 0b0001 || pattern == 0b1000)
    {
      rotaryDirection = UP;
      rotaryUpdated   = true;
    }
  
  rotaryLast = rotaryCurrent; //store current rotary AB values for next time

}

//-------------------------------------------------------------------------------------------------------------
// Initialize Rotary interface
//-------------------------------------------------------------------------------------------------------------
void enableRotary()
{
  pinMode(rotaryPinA, INPUT_PULLUP);         // pin is input and pulled high
  pinMode(rotaryPinB, INPUT_PULLUP);         // pin is input and pulled high
  attachInterrupt(0, updateRotary, CHANGE);  // call updateEncoder() when any high/low changed seen on interrupt 0 (pin 2)
  attachInterrupt(1, updateRotary, CHANGE);  // call updateEncoder() when any high/low changed seen on interrupt 1 (pin 3)
}

//-------------------------------------------------------------------------------------------------------------
// Update Channel Freq
//-------------------------------------------------------------------------------------------------------------
void updateChannel(Si4703& radio)
{
  digitalWrite(LED1, LOW);           // turn LED1 OFF
  radio.writeGPIO(GPIO1, GPIO_Low);  // turn LED2 OFF

  if(rotaryDirection == UP)
    {
      radio.incChannel();
    }
  else if(rotaryDirection == DOWN)
    {
      radio.decChannel();
    }
 
  rotaryUpdated = false;              // Clear flag
  write_EEPROM(radio);                // Save channel to EEPROM
  digitalWrite(LED1, HIGH);           // When done turn LED1 On
  radio.writeGPIO(GPIO1, GPIO_High);  // turn LED2 ON
}

#endif