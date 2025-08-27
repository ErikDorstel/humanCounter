WiFiClient tcp;
WiFiServer telnetServer(23);
WiFiClient telnetClient;
bool telnetDebug=false;
String dataStore[255];
uint8_t currentTags=0;
bool tcpReady=false;
int tcpRec=0;

void tcpConnect() {
  if (debug) { Serial.println("TCP connect."); }
  tcpReady=false; tcpRec=0;
  tcp.stop();
  tcp.connect(tcpServer,4242); }

void sendTCPall() {
  tcp.print("OK\n"); tcpRec++;
  tcp.print(tcpSecret); tcp.print("\n"); tcpRec++;
  for (uint8_t n=0;n<currentTags;n++) { tcp.print(dataStore[n] + String(";")); if (debug) { Serial.print("Send: "); Serial.println(dataStore[n]); } }
  tcpReady=true; tcp.print("\n"); tcpRec++; if (debug) { Serial.println("TCP ready."); } }

void tcpWorker() {
  static uint64_t tcpTimer=8000,recTimer=8000,keepAlive=20000;
  static String buffer="";
  if (millis()>=tcpTimer) { tcpTimer=millis()+20000;
    if (!tcp.connected() && WiFi.status()==WL_CONNECTED) { tcpConnect(); } }
  if (tcp.connected() && (!tcpReady)) { sendTCPall(); }
  if (tcp.connected() && millis()>=keepAlive) { keepAlive=millis()+20000; tcp.print("OK\n"); tcpRec++; }
  if (millis()>=recTimer && tcpRec) { tcpRec=0; connectWLAN(); }
  if (!tcpRec) { recTimer=millis()+10000; }
  while (tcp.available()) { buffer+=(char)tcp.read(); if (buffer.indexOf("OK")>=0) { buffer=""; tcpRec--; if (debug) { Serial.print("Reception: "); Serial.println(tcpRec); } } } }
