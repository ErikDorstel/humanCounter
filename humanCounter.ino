bool debug=true;

const char* version="1.03";
const char* ssidClient="SSID";
const char* passClient="Secret";
const char* ssidAP="humanCounter";
const char* passAP="Secret";
const char* tcpServer="XX.XX.XX.XX";
const String tcpSecret="XXXXXXXX";
const String telnetSecret="XXXXXXXX";

#include "WLAN.h"
#include "TCPclient.h"
#include "TCPserver.h"
#include "OTA.h"
#include "LED.h"
#include "DATA.h"
#include "BARRIER.h"
#include "TELNET.h"
#include "RC522A.h"
#include "RC522B.h"

void setup() {
  Serial.begin(115200);
  initWLAN();
  initServer();
  initOTA();
  initLed();
  initData();
  initBarrier();
  initTelnet();
  initRC522A();
  initRC522B(); }

void loop() { ledWorker(); wlanWorker(); tcpWorker(); serverWorker(); telnetWorker(); barrierWorker(); rc522WorkerA(); rc522WorkerB(); otaWorker(); }
