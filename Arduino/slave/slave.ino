#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <FastLED.h>

SoftwareSerial softwareSerial(10, 11);
DFRobotDFPlayerMini DFPlayer;

#define ARDUINO_PIN 2 // Interrupt
#define MAIN_LEDS 4
#define AUX_LEDS 3

CRGB mainLeds[6]; // 6 leds
CRGB auxLeds[2]; // 2 leds

uint32_t colors[] = {CRGB::White, CRGB::Orange, CRGB::Red, CRGB::Yellow, CRGB::Green};
int colorCounter = 0;

void nextSong(){
  DFPlayer.next();
  colorCounter++;
  if(colorCounter>4) colorCounter=1;
  fullLEDS(colors[colorCounter]);
}

void setup() {
  FastLED.addLeds<NEOPIXEL, MAIN_LEDS > (mainLeds, 6);
  FastLED.addLeds<NEOPIXEL, AUX_LEDS > (auxLeds, 2);
  
  attachInterrupt(digitalPinToInterrupt(ARDUINO_PIN),nextSong,RISING);
  
  softwareSerial.begin(9600);
  DFPlayer.begin(softwareSerial);
  DFPlayer.volume(30);
  DFPlayer.play(1);
  
  fullLEDS(colors[0]);
}


void loop() {
  
}

void fullLEDS(uint32_t color){
  for(int i=0; i<6; i++){
    mainLeds[i] = color;
    FastLED.show();   
  }
  auxLeds[0] = color;
  FastLED.show();
  auxLeds[1] = color;
  FastLED.show();
}