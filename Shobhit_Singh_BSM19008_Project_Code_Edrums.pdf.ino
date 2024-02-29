#include <MIDI.h> 
MIDI_CREATE_DEFAULT_INSTANCE();

//Ports on which the piezo sensors are attached 
const int Pad1 = A0;
const int Pad2 = A1;
const int Pad3 = A2;
const int Pad4 = A3;

//threshold 
const int volumeThreshold = 450;

//volume for each piezo(drum pad)
int Drum1_val = 0;
int Drum2_val = 0;
int Drum3_val = 0;
int Drum4_val = 0;

//variables for detecting a true hit. 
int signalTest2 = 0;
int signalTest1 = 0;
int signalTest3 = 0;
int signalTest4 = 0;

//midi mapping - addictive drums mapping (You can change the mapping but this is default) 
byte snareDrum = 38;
byte kick = 36; 
byte hihatClosed = 53;
byte crash = 77;
byte hihatOpened = 56;

void setup()
{
  MIDI.begin(4);
  //starts serial communication between arduino and serial USB port
  Serial.begin(115200); 
}

void loop()
{ 
  /*1st pad (hi-hat)*/
  //checks if magnitude of analog input is higher than volumeThreshold
  if(analogRead(Pad1) >= volumeThreshold) 
  {
  signalTest1 = 1;
  }
  //if magnitude of analoginput was higher than volumeThreshold program checks again if the signal is decreasing. If that happens, it means that drum pad was hit
  if(signalTest1 == 1 && analogRead(Pad1) <= (volumeThreshold-95))
  {
    //sets the velocity according to how hard the hit was
    Drum1_val= analogRead(Pad1);
    int velocity = Drum1_val/3 + 78;
    if(velocity >= 120)
      velocity = 125;
    else if(velocity < 0)
      velocity = 0;

    //sends midi signal over USB serial port
    MIDI.sendNoteOn(hihatOpened, velocity, 1);
    MIDI.sendNoteOff(hihatOpened, 0, 1);
    //sets signalTest again to 0
    signalTest1 = 0;
  }

   /*Note : This is a much more sophisticated way to sending MIDI signals. A much easier way would be simply checking if magnitude is greater than threshold and sending  
   MIDI.sendNoteOn(hihatOpened, 100, 1) followed by MIDI.sendNoteOff(hihatOpened, 0, 1)
   But the sophisticated implementation is much more precise and accurately judging a true hit and increasing the volume if the pad is hit hard. 
   Before this I was getting a lot of extra detections from the sensors and the volume stayed same no matter how hard it was hit. */

  /*2nd pad (snare)*/
  if(analogRead(Pad2) >= volumeThreshold) 
  {
  signalTest2 = 1;
  } 
  Drum2_val = analogRead(Pad2);
  if(signalTest2 == 1 && analogRead(Pad2) <= (volumeThreshold-95))
  {
   Drum2_val= analogRead(Pad2);
   int velocity = Drum2_val/3 + 35;
   
   if(velocity >= 120)
      velocity = 125;
    else if(velocity < 0)
      velocity = 0;  
      
    MIDI.sendNoteOn(snareDrum, velocity, 1);
    MIDI.sendNoteOff(snareDrum, 0, 1);
    signalTest2 = 0;
  }

  /*3rd pad (kick)*/
  if(analogRead(Pad3) >= volumeThreshold) 
  {
  signalTest3 = 1; 
  }
  Drum3_val = analogRead(Pad3);
  if(signalTest3 == 1 && analogRead(Pad3) <= (volumeThreshold-95))
  {
   Drum3_val= analogRead(Pad3);
   int velocity = Drum3_val/2 + 77;
   
   if(velocity >= 120)
      velocity = 125;
    else if(velocity < 0)
      velocity = 0;
      
    MIDI.sendNoteOn(kick, velocity, 1);
    MIDI.sendNoteOff(kick, 0, 1);
    signalTest3 = 0;
  }

  /*4th pad (crash/ride)*/
  if(analogRead(Pad4) >= volumeThreshold) 
  {
  signalTest4 = 1; 
  }
  if(signalTest4 == 1 && analogRead(Pad4) <= (volumeThreshold-95))
  {
   Drum4_val= analogRead(Pad4);
   int velocity = Drum4_val/3 + 75;
   
   if(velocity > 120)
      velocity = 125;
    else if(velocity < 0)
      velocity = 0;  
      
    MIDI.sendNoteOn(crash, velocity, 1);
    MIDI.sendNoteOff(crash, 0, 1);
    signalTest4 = 0;
  }
}
