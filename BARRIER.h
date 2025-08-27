#define barrierAGPIO 36
#define barrierBGPIO 39

uint64_t barrierA1timer,barrierB1timer;
uint64_t UIDA,UIDB;
const int barrier1Delay=5000,barrier2Delay=50,barrier3Delay=10000,adminDelay=5000;

void initBarrier() {
  pinMode(barrierAGPIO,INPUT); pinMode(barrierBGPIO,INPUT); }

void setBarrier(char value,uint64_t UID) {
  if (value=='A') { barrierA1timer=millis()+barrier1Delay; UIDA=UID; greenOn(barrier1Delay); }
  if (value=='B') { barrierB1timer=millis()+barrier1Delay; UIDB=UID; greenOn(barrier1Delay); } }

void barrierWorker() {
  static uint64_t barrierA2timer,barrierA3timer;
  static bool barrierAold=false;
  bool barrierAstate=!digitalRead(barrierAGPIO);
  if (barrierAold!=barrierAstate && barrierAstate) { barrierA2timer=millis()+barrier2Delay; }
  if (barrierAold!=barrierAstate && (!barrierAstate) && millis()>=barrierA2timer) {
    if (millis()>=barrierA1timer) { alarmOn(); int x1=getData(0)+1; setData(x1,0); }
    else { barrierA1timer=millis()+barrier1Delay; greenOn(barrier1Delay); int x2=getData(UIDA)+1;
      if (x2>4) { x2=4; barrierA1timer=0; greenOff(); alarmOn(); int x3=getData(0)+1; setData(x3,0); }
      setData(x2,UIDA); } }
  if (millis()>=barrierA3timer && barrierAstate) { alarmOn(); }
  if (!barrierAstate) { barrierA3timer=millis()+barrier3Delay; }
  barrierAold=barrierAstate;

  static uint64_t barrierB2timer,barrierB3timer;
  static bool barrierBold=false,z=false;
  bool barrierBstate=!digitalRead(barrierBGPIO);
  if (barrierBold!=barrierBstate && barrierBstate) { barrierB2timer=millis()+barrier2Delay; }
  if (barrierBold!=barrierBstate && (!barrierBstate) && millis()>=barrierB2timer) {
    if (millis()>=barrierB1timer) { alarmOn(); int y1=getData(0)-1; setData(y1,0); }
    else { barrierB1timer=millis()+barrier1Delay; greenOn(barrier1Delay); int y2=getData(UIDB)-1;
      if (y2<0) { y2=0; barrierB1timer=0; greenOff(); alarmOn(); int y3=getData(0)-1; setData(y3,0); }
      setData(y2,UIDB); z=true; } }
  if (millis()>=barrierB1timer && z==true) { z=false; setData(0,UIDB); }
  if (millis()>=barrierB3timer && barrierBstate) { alarmOn(); }
  if (!barrierBstate) { barrierB3timer=millis()+barrier3Delay; }
  barrierBold=barrierBstate; }
