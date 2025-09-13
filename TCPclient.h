#include <Preferences.h>
Preferences data;
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
  static uint64_t UID=0; static int value=0;
  static String buffer="";
  static bool ok=false;
  if (millis()>=tcpTimer) { tcpTimer=millis()+20000;
    if (!tcp.connected() && WiFi.status()==WL_CONNECTED) { tcpConnect(); } }
  if (tcp.connected() && (!tcpReady)) { sendTCPall(); }
  if (tcp.connected() && millis()>=keepAlive) { keepAlive=millis()+20000; tcp.print("OK\n"); tcpRec++; }
  if (millis()>=recTimer && tcpRec) { tcpConnect(); }
  if (!tcpRec) { recTimer=millis()+20000; }
  while (tcp.available()) { char letter=(char)tcp.read();
    if (letter=='O') { ok=true; }
    else if (letter=='K' && ok) { ok=false; tcpRec--; if (debug) { Serial.print("Reception: "); Serial.println(tcpRec); } }
    else if ((letter>='0' && letter<='9') || (letter=='-')) { buffer+=letter; }
    else if ((letter>='a' && letter<='f') || (letter>='A' && letter<='F')) { buffer+=letter; }
    else if (letter=='.') { UID=strtoull(buffer.c_str(),NULL,16); buffer=""; }
    else if (letter==';') { value=strtol(buffer.c_str(),NULL,10); buffer="";
      int n=-1;
      if ((dataStore[0] & 0xffffffff)==UID) { n=0; }
      else { for (uint16_t x=1;x<currentTags;x++) { if ((dataStore[x] & 0xffffffffffffff)==UID) { n=x; } } }
      if (n==-1) {
        if (currentTags>=340) { if (debug) { Serial.println("Too mmany tags."); } return; }
        data.begin("dataStore",false);
        dataStore[currentTags]=UID | ((uint64_t)value << 56);
        data.putULong64(String(currentTags,HEX).c_str(),dataStore[currentTags]);
        String text=String(dataStore[currentTags] & 0xffffffffffffff,HEX) + String(".") + String(value);
        if (telnetClient.connected() && telnetDebug) { telnetClient.print(text + String("\r\n")); }
        if (debug) { Serial.print ("New Remote: "); Serial.println(text); }
        currentTags++;
        data.putInt("current",currentTags);
        data.end(); }
      else {
        data.begin("dataStore",false);
        if (n==0) { dataStore[n]=UID | ((uint64_t)value << 32); }
        else { dataStore[n]=UID | ((uint64_t)value << 56); }
        data.putULong64(String(n,HEX).c_str(),dataStore[n]);
        String text="";
        if (n==0) { text=String(dataStore[n] & 0xffffffff,HEX) + String(".") + String(value); }
        else { text=String(dataStore[n] & 0xffffffffffffff,HEX) + String(".") + String(value); }
        if (telnetClient.connected() && telnetDebug) { telnetClient.print(text + String("\r\n")); }
        if (debug) { Serial.print ("Set Remote: "); Serial.println(text); }
        data.end(); } } } }
