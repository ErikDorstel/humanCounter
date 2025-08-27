#include <Preferences.h>
Preferences data;

void readData() {
  if (debug) { Serial.println("Read Data Store."); }
  data.begin("dataStore",true); currentTags=data.getInt("current",0);
  if (debug) { Serial.print ("Data free: "); Serial.println(data.freeEntries()); }
  if (debug) { Serial.print ("Tags: "); Serial.println(currentTags); }
  for (uint16_t n=0;n<currentTags;n++) { dataStore[n]=data.getULong64(String(n,HEX).c_str(),0);
    if (debug) { Serial.print("Read: ");
      if (n==0) { Serial.println(String(dataStore[n] & 0xffffffff,HEX) + "." + String((int)(dataStore[n] >> 32))); }
      else { Serial.println(String(dataStore[n] & 0xffffffffffffff,HEX) + "." + String((int)(dataStore[n] >> 56))); } } }
  data.end(); }

void formatData(bool test=false) {
  if (debug) { Serial.println("Format Data Store."); }
  data.begin("dataStore",false); data.clear(); data.putChar("exist",1);
  dataStore[0]=0; data.putULong64(String(0,HEX).c_str(),dataStore[0]); data.putInt("current",1); currentTags=1;
  if (test) { for (uint16_t n=1;n<300;n++) { dataStore[n]=0xaaaaaaaa; data.putULong64(String(n,HEX).c_str(),dataStore[n]); } data.putInt("current",300); currentTags=300; }
  if (debug) { Serial.print ("Data free: "); Serial.println(data.freeEntries()); }
  if (debug) { Serial.print ("Tags: "); Serial.println(currentTags); }
  data.end(); }

int tagExist(uint64_t UID) {
  if ((dataStore[0] & 0xffffffff)==UID) { return 0; }
  for (uint16_t n=1;n<currentTags;n++) { if ((dataStore[n] & 0xffffffffffffff)==UID) { return n; } } return -1; }

bool checkAdmin(uint64_t UID) {
  if (UID==0x220ff083) { return true; }
  else if (UID==0x21142b43) { return true; }
  else { return false; } }

void newTag(uint64_t UID) {
  if (tagExist(UID)==-1) {
    data.begin("dataStore",false);
    dataStore[currentTags]=UID;
    data.putULong64(String(currentTags,HEX).c_str(),dataStore[currentTags]);
    String text=String(dataStore[currentTags],HEX) + String(".0");
    if (tcp.connected() && tcpReady) { tcp.print(text + String("\n")); tcpRec++; }
    if (telnetClient.connected() && telnetDebug) { telnetClient.print(text + String("\r\n")); }
    if (debug) { Serial.print ("New: "); Serial.println(text); }
    currentTags++;
    data.putInt("current",currentTags);
    data.end(); } }

int getData(uint64_t UID) {
  if ((dataStore[0] & 0xffffffff)==UID) { return (int)(dataStore[0] >> 32); }
  for (uint16_t n=1;n<currentTags;n++) { if ((dataStore[n] & 0xffffffffffffff)==UID) { return (int)(dataStore[n] >> 56); } } return 0; }

void setData(int value,uint64_t UID) {
  int n=tagExist(UID); if (n>=0) {
    data.begin("dataStore",false);
    if (n==0) { dataStore[n]=UID | ((uint64_t)value << 32); }
    else { dataStore[n]=UID | ((uint64_t)value << 56); }
    data.putULong64(String(n,HEX).c_str(),dataStore[n]);
    String text="";
    if (n==0) { text=String(dataStore[n] & 0xffffffff,HEX) + String(".") + String(value); }
    else { text=String(dataStore[n] & 0xffffffffffffff,HEX) + String(".") + String(value); }
    if (tcp.connected() && tcpReady) { tcp.print(text + String("\n")); tcpRec++; }
    if (telnetClient.connected() && telnetDebug) { telnetClient.print(text + String("\r\n")); }
    if (debug) { Serial.print ("Set: "); Serial.println(text); }
    data.end(); } }

void initData() {
  data.begin("dataStore",true);
  bool exist=data.getChar("exist",0);
  data.end();
  if (exist) { readData(); } else { formatData(); } }
