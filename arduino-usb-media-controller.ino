// Anor Batista Esteves Neto 
// August 2015
// A simples usb media controller using a pro micro , buttons and a rottary encoder
// Uses the rotary encoder library(https://github.com/mathertel/RotaryEncoder)  
// and the Arduino HID project bootloader ( https://github.com/NicoHood/HID )
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

#define GREEN_LED 8
#define RED_LED   9

void setup()
{
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(SWITCH_PIN, INPUT); // set the switch button port as an input
  pinMode(PLAY_PIN, INPUT); // set the play/pause button port as an input
  pinMode(NEXT_PIN, INPUT); // set the next button port as an input
  pinMode(PREVIOUS_PIN, INPUT); // set the next button port as an input
  digitalWrite(SWITCH_PIN, HIGH); // activate internal pull-up on switch_pin
  digitalWrite(PLAY_PIN, HIGH); // activate internal pull-up on play_pin
  digitalWrite(NEXT_PIN, HIGH); // activate internal pull-up on next_pin
  digitalWrite(PREVIOUS_PIN, HIGH); // activate internal pull-up on next_pin

  digitalWrite(GREEN_LED,HIGH);
  
  Consumer.begin();
}

void loop()
{
  //Let's verify if the button was pressed, it is pressed when it's HIGH
  static boolean muted_state_controll = 0 ;
  if (!digitalRead(SWITCH_PIN))
  {
    Consumer.write(MEDIA_VOLUME_MUTE);
    
    digitalWrite(RED_LED, !muted_state_controll);
    digitalWrite(GREEN_LED, muted_state_controll); 
    muted_state_controll = !muted_state_controll ;
       
    while (digitalRead(SWITCH_PIN) == LOW)
      delay(10);
  }
  else if (!digitalRead(PLAY_PIN))
  {
    Consumer.write(MEDIA_PLAY_PAUSE);   
    while (digitalRead(PLAY_PIN) == LOW)
      delay(10);
  }
  else if (!digitalRead(NEXT_PIN))
  {
    Consumer.write(MEDIA_NEXT);   
    while (digitalRead(NEXT_PIN) == LOW)
      delay(10);
  }
  else if (!digitalRead(PREVIOUS_PIN))
  {
    Consumer.write(MEDIA_PREVIOUS);   
    while (digitalRead(PREVIOUS_PIN) == LOW)
      delay(10);
  }

  //Now let's read the encoder and check if it was incremeted
  // or decremented
  static int pos,new_pos = 0;
  
  encoder.tick();
  
  new_pos = encoder.getPosition();
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
