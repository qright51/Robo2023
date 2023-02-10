#include <Servo.h>

#define pinMotorL 4
#define pinMotorR 3
#define touchSensor 2
#define musicPing 5

Servo motorL, motorR;

volatile bool isRunning = false;

int trackerPin[5] = {8,9,10,11,12};
int trackerValue[5] = {0,0,0,0,0};
int first = -1;
int last = -1;
float median = 3.0;

volatile unsigned long int minTime = 40000;

void beginTracking(){
  for(int i=0; i<90; i++){
    motor(i,i);
    delay(10);
  }
  isRunning=true;
  minTime=40000+millis();
  requestNewSong();
}

void requestNewSong(){
  delay(50);
  digitalWrite(musicPing, HIGH);
  delay(100);
  digitalWrite(musicPing, LOW);
  delay(50);
}


void motor(float a, float b){
  if(a>100) a=100;
  else if(a<-100) a=-100;
  if(b>100) b=100;
  else if(b<-100) b=-100;
  
  motorL.writeMicroseconds(1500+(2*a));
  motorR.writeMicroseconds(1500-(2*b));
}

void lineTracker(){
  for(int i=0; i<5; i++){
    trackerValue[i] = digitalRead(trackerPin[i]);
  }
  for(int i=0; i<5; i++){
    if(trackerValue[i]==1 && first==-1) first=i+1;
  }
  for(int i=4; i>=0; i--){
    if(trackerValue[i]==1 && last==-1) last=i+1;
  }

  if(millis()>=minTime && first==1 && last==5){
    isRunning = false;
    motor(90,90);
    for(int i=90; i>=0; i--){
      motor(i,i);
      delay(10);
    }
    motor(0,0);
  }

  if(isRunning==true){
    if(first==-1 || last==-1){
      if(median<3){ 
        if(median==0) median=3;
        if(median==1) motor(-20,90);
        if(median==1.5) motor(-10,80);
        if(median==2) motor(0,70);
        if(median==2.5) motor(20,50);
      }
      else if(median>3){
        if(median==5) motor(90,-20);
        if(median==4.5) motor(80,-10);
        if(median==4) motor(70,0);
        if(median==3.5) motor(50,20);
      }
      else{
        motor(90,90);
      }
    }
    else median=(float)(first+last)/2.0;

    first = -1;
    last = -1;

    if(median<3){ 
      if(median==0) median=3;
      if(median==1) motor(20,100);
      if(median==1.5) motor(35,100);
      if(median==2) motor(55,100);
      if(median==2.5) motor(70,80);
    }
    else if(median>3){
      if(median==5) motor(100,20);
      if(median==4.5) motor(100,35);
      if(median==4) motor(100,55);
      if(median==3.5) motor(80,70);
    }
    else{
      motor(90,90);
    }
  } 
}



void setup() {  
  attachInterrupt(digitalPinToInterrupt(touchSensor),beginTracking,RISING);
  pinMode(musicPing, OUTPUT);
  digitalWrite(musicPing, LOW);
  
  for(int i=0; i<5; i++){
    pinMode(trackerPin[i],INPUT);
  }
  motorL.attach(pinMotorL);
  motorR.attach(pinMotorR);
  motor(0,0);

  minTime += millis();
  isRunning = false;
}

void loop() {
  if(isRunning){
    lineTracker();
  }
}
