WiFiClient tcp;
WiFiServer telnetServer(23);
WiFiClient telnetClient;
bool telnetDebug=false;
uint64_t dataStore[500];
uint16_t currentTags=0;
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
  tcp.print(String(dataStore[0] & 0xffffffff,HEX) + "." + String((int)(dataStore[0] >> 32)) + String(";"));
  if (debug) { Serial.print("Send: "); Serial.println(String(dataStore[0] & 0xffffffff,HEX) + "." + String((int)(dataStore[0] >> 32))); }
  for (uint16_t n=1;n<currentTags;n++) { tcp.print(String(dataStore[n] & 0xffffffffffffff,HEX) + "." + String((int)(dataStore[n] >> 56)) + String(";"));
    if (debug) { Serial.print("Send: "); Serial.println(String(dataStore[n] & 0xffffffffffffff,HEX) + "." + String((int)(dataStore[n] >> 56))); } }
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
