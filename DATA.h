#include <Preferences.h>
Preferences data;

void readData() {
  if (debug) { Serial.println("Read Data Store."); }
  data.begin("dataStore",true); currentTags=data.getInt("current",0);
  if (debug) { Serial.print ("Data free: "); Serial.println(data.freeEntries()); }
  if (debug) { Serial.print ("Tags: "); Serial.println(currentTags); }
  for (uint8_t n=0;n<currentTags;n++) { dataStore[n]=data.getString(String(n,HEX).c_str(),""); if (debug) { Serial.print("Read: "); Serial.println(dataStore[n]); } }
  data.end(); }

void formatData(bool test=false) {
  if (debug) { Serial.println("Format Data Store."); }
  data.begin("dataStore",false); data.clear(); data.putChar("exist",1);
  dataStore[0]="00000000.0"; data.putString(String(0,HEX).c_str(),dataStore[0]); data.putInt("current",1); currentTags=1;
  if (test) { for (int n=1;n<150;n++) { dataStore[n]="ffffffff.0"; data.putString(String(n,HEX).c_str(),dataStore[n]); } data.putInt("current",150); currentTags=150; }
  if (debug) { Serial.print ("Data free: "); Serial.println(data.freeEntries()); }
  if (debug) { Serial.print ("Tags: "); Serial.println(currentTags); }
  data.end(); }

int tagExist(String UID) {
  for (uint8_t n=0;n<currentTags;n++) { if (dataStore[n].startsWith(UID + String("."))) { return n; } } return -1; }

bool checkAdmin(String UID) {
  if (UID=="83f00f22") { return true; }
  else if (UID=="432b1421") { return true; }
  else { return false; } }

void newTag(String UID) {
  if (tagExist(UID)==-1) {
    data.begin("dataStore",false);
    dataStore[currentTags]=UID + String(".0");
    data.putString(String(currentTags,HEX).c_str(),dataStore[currentTags]);
    if (tcp.connected() && tcpReady) { tcp.print(dataStore[currentTags] + String("\n")); tcpRec++; }
    if (telnetClient.connected() && telnetDebug) { telnetClient.print(dataStore[currentTags] + String("\r\n")); }
    if (debug) { Serial.print ("New: "); Serial.println(dataStore[currentTags]); }
    currentTags++;
    data.putInt("current",currentTags);
    data.end(); } }

int getData(String UID) {
  for (uint8_t n=0;n<currentTags;n++) { if (dataStore[n].startsWith(UID + String("."))) { return dataStore[n].substring(dataStore[n].indexOf(".")+1).toInt(); } } return 0; }

void setData(int value,String UID) {
  int n=tagExist(UID); if (n>=0) {
    data.begin("dataStore",false);
    dataStore[n]=UID + String(".") + String(value);
    data.putString(String(n,HEX).c_str(),dataStore[n]);
    if (tcp.connected() && tcpReady) { tcp.print(dataStore[n] + String("\n")); tcpRec++; }
    if (telnetClient.connected() && telnetDebug) { telnetClient.print(dataStore[n] + String("\r\n")); }
    if (debug) { Serial.print ("Set: "); Serial.println(dataStore[n]); }
    data.end(); } }

void initData() {
  data.begin("dataStore",true);
  bool exist=data.getChar("exist",0);
  data.end();
  if (exist) { readData(); } else { formatData(); } }
