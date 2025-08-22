#include "intervalTimer.h"
intervalTimer timerBeep;
intervalTimer timerGreen;
intervalTimer timerYellow;
intervalTimer timerRed;
intervalTimer timerAlarm;

#define beepOut 27
#define greenOut 32
#define yellowOut 25
#define redOut 26

void beepOff() { digitalWrite(beepOut,LOW); }
void beepOn(int time=100) { digitalWrite(beepOut,HIGH); timerBeep.set(0,true,time,1,beepOff); }
void greenOff() { digitalWrite(greenOut,LOW); }
void greenOn(int time=2000) { digitalWrite(greenOut,HIGH); digitalWrite(beepOut,HIGH); timerGreen.set(0,true,time,1,greenOff); timerBeep.set(0,true,100,1,beepOff); }
void yellowOff() { digitalWrite(yellowOut,LOW); }
void yellowOn(int time=2000) { digitalWrite(yellowOut,HIGH); timerYellow.set(0,true,time,1,yellowOff); }
void redOff() { digitalWrite(redOut,LOW); }
void redOn(int time=2000) { digitalWrite(redOut,HIGH); timerRed.set(0,true,time,1,redOff); }
void alarmDo() { digitalWrite(redOut,HIGH); digitalWrite(beepOut,HIGH); timerRed.set(0,true,500,1,redOff); timerBeep.set(0,true,100,1,beepOff); } 
void alarmOn(int repeat=10) { timerAlarm.set(1000,true,0,repeat,alarmDo); }

void initLed() {
  pinMode(beepOut,OUTPUT);
  pinMode(greenOut,OUTPUT); greenOn(1000);
  pinMode(yellowOut,OUTPUT); yellowOn(2000);
  pinMode(redOut,OUTPUT); redOn(3000); }

void ledWorker() {
  timerBeep.check();
  timerGreen.check();
  timerYellow.check();
  timerRed.check();
  timerAlarm.check(); }
