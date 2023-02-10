#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial softwareSerial(10, 11);
DFRobotDFPlayerMini DFPlayer;

void nextSong(){
  DFPlayer.next();
}

void setup() 
{
  attachInterrupt(digitalPinToInterrupt(2),nextSong,RISING);
  softwareSerial.begin(9600);
  DFPlayer.begin(softwareSerial);
  DFPlayer.volume(30); 
}


void loop() { }
