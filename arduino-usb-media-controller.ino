// Anor Batista Esteves Neto 
// August 2015
// A simples usb media controller using a pro micro , buttons , a rottary encoder and a 5110 lcd to displat information
// Uses the rotary encoder library(https://github.com/mathertel/RotaryEncoder)  
// the Arduino HID project bootloader ( https://github.com/NicoHood/HID )
// and the sparkfun nokia 5110 example (https://github.com/sparkfun/GraphicLCD_Nokia_5110)
//if the pro micro ever stop being recognized by windows just press upload and quickly connect it to the coomputer  to unbrick it
#include <RotaryEncoder.h>
//Rottary encoder connections
// DT - > A2
// CLK -> A3
//Let's create the rotary encoder object
RotaryEncoder encoder(A2, A3);
// Rottary encoder SWT(SWITCH) -> D7 
#define SWITCH_PIN    7
#define PLAY_PIN     14 // button connected to ground and to digital pin 14
#define NEXT_PIN     15 // button connected to ground and to digital pin 15
#define PREVIOUS_PIN 16 // button connected to ground and to digital pin 16

// mute/unmute status leds
#define GREEN_LED 8 // on when unmute
#define RED_LED   9 // on when mute
//define the nokia lcd pins
#define LCD_RST   2
#define LCD_CE    3
#define LCD_DC    4
#define LCD_DIN   5
#define LCD_CLK   6


void setup()
{
  pinMode(GREEN_LED,OUTPUT);// set the green led pin as an output
  pinMode(RED_LED,OUTPUT); // set the red led pin as an output
  pinMode(SWITCH_PIN, INPUT); // set the switch button port as an input
  pinMode(PLAY_PIN, INPUT); // set the play/pause button port as an input
  pinMode(NEXT_PIN, INPUT); // set the next button port as an input
  pinMode(PREVIOUS_PIN, INPUT); // set the next button port as an input

  LcdInit(); //initializes the lcd
  LcdClear(); // clears the display, just in case
  
  digitalWrite(SWITCH_PIN, HIGH); // activate internal pull-up on switch_pin
  digitalWrite(PLAY_PIN, HIGH); // activate internal pull-up on play_pin
  digitalWrite(NEXT_PIN, HIGH); // activate internal pull-up on next_pin
  digitalWrite(PREVIOUS_PIN, HIGH); // activate internal pull-up on next_pin

  digitalWrite(GREEN_LED,HIGH); // turn unmute led on

  Consumer.begin(); // starts the usb hid control

  LcdString("Hello World!");
}



void loop()
{
  //Let's verify if the button was pressed, it is pressed when it's HIGH
  static boolean muted_state_controll = 0 ;
  if (!digitalRead(SWITCH_PIN))
  {
    Consumer.write(MEDIA_VOLUME_MUTE);
    
    digitalWrite(RED_LED, !muted_state_controll); // when the button is pressed it sends a mute
    digitalWrite(GREEN_LED, muted_state_controll); // command to the pc .turns off the green led
    muted_state_controll = !muted_state_controll ; // and on the red led
       
    while (digitalRead(SWITCH_PIN) == LOW) // SUPER simple debounce method
      delay(10);
  }
  else if (!digitalRead(PLAY_PIN)) // if the play/pause button is pressed send the respective
  {                                 // command to the pc
    Consumer.write(MEDIA_PLAY_PAUSE);   
    while (digitalRead(PLAY_PIN) == LOW)
      delay(10);
  }
  else if (!digitalRead(NEXT_PIN)) // if the next button is pressed send the respective
  {                                // command to the pc
    Consumer.write(MEDIA_NEXT);   
    while (digitalRead(NEXT_PIN) == LOW)
      delay(10);
  }
  else if (!digitalRead(PREVIOUS_PIN)) // if the pricous button is pressed send the respective
  {                                    // command to the pc
    Consumer.write(MEDIA_PREVIOUS);   
    while (digitalRead(PREVIOUS_PIN) == LOW)
      delay(10);
  }

  //Now let's read the encoder and check if it was incremeted
  // or decremented
  static int pos,new_pos = 0;
  
  encoder.tick(); // checks the encoder position ( part of the encoder library)
  
  new_pos = encoder.getPosition(); // get the new encoder position
  if (new_pos > pos) // if it was increment lets turn up the volume!
  {
    Consumer.write(MEDIA_VOLUME_UP);
    pos = new_pos;
  }
  else if (new_pos < pos) // if it was decremented lets turn down the volume!
  {
    Consumer.write(MEDIA_VOLUME_DOWN);
    pos = new_pos;
  }

}
