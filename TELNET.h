String buffer="";

void initTelnet() { telnetServer.begin(); }

void doCommand(String command) {
  if (buffer.indexOf("exit")>=0) { telnetClient.stop(); } else
  if (buffer.indexOf("help")>=0) { telnetClient.print("help\r\nexit\r\nfree\r\nformat\r\nformat test\r\nreboot\r\nruntime\r\nclear\r\ngreenOn\r\nalarmOn\r\ndebug [on/off]\r\n"); } else
  if (buffer.indexOf("free")>=0) { data.begin("dataStore",false);
    telnetClient.print("Data free: "); telnetClient.print(data.freeEntries()); telnetClient.print("\r\n");
    telnetClient.print("Tags: "); telnetClient.print(currentTags); telnetClient.print("\r\n"); data.end(); } else
  if (buffer.indexOf("reboot")>=0) { telnetClient.stop(); ESP.restart(); } else
  if (buffer.indexOf("runtime")>=0) { telnetClient.print((float)millis()/1000.0/3600.0/24.0,2); telnetClient.print(" days\r\n"); } else
  if (buffer.indexOf("clear")>=0) { setData(0,"00000000"); telnetClient.print("done\r\n"); } else
  if (buffer.indexOf("greenOn")>=0) { greenOn(); telnetClient.print("done\r\n"); } else
  if (buffer.indexOf("alarmOn")>=0) { alarmOn(); telnetClient.print("done\r\n"); } else
  if (buffer.indexOf("debug on")>=0) { telnetDebug=true; telnetClient.print("done\r\n"); } else
  if (buffer.indexOf("debug off")>=0) { telnetDebug=false; telnetClient.print("done\r\n"); } else
  if (buffer.indexOf("format test")>=0) { formatData(true); telnetClient.print("done\r\n"); } else
  if (buffer.indexOf("format")>=0) { formatData(false); telnetClient.print("done\r\n"); } }

void telnetWorker() {
  static uint8_t isAuth=0;
  if (telnetServer.hasClient() && (!telnetClient.connected())) { telnetClient=telnetServer.available(); isAuth=0; buffer=""; }
  if (!telnetClient.connected()) { telnetClient.stop(); }
  if (telnetClient.connected()) {
    if (isAuth==0) { telnetClient.print("Password: "); isAuth=1; telnetClient.flush(); }
    if (telnetClient.available()) { char c=telnetClient.read();
      if (c==10) {
        if (isAuth==1) { if (buffer.indexOf(telnetSecret)>=0) { isAuth=2; buffer=""; } else { telnetClient.stop(); buffer=""; } }
        if (isAuth==2) { doCommand(buffer); buffer=""; telnetClient.print("> "); } }
      else { buffer+=c; } } } }
