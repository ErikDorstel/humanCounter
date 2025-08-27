WiFiServer server(4242);
WiFiClient client;

void initServer() { server.begin(); }

void serverWorker() {
  static uint64_t serverTimer;
  static bool sent=false;
  if (server.hasClient() && (!client.connected())) { client=server.available(); sent=false; }
  if (client.connected()) {
    if (sent==true && millis()>=serverTimer) { client.stop(); }
    if (sent==false) {
      client.print(currentTags); client.print("\r\n");
      client.print("OK\r\n");
      client.print(String(dataStore[0] & 0xffffffff,HEX) + String(".") + String((int)(dataStore[0] >> 32)) + String(";"));
      if (debug) { Serial.print("Send: "); Serial.println(String(dataStore[0] & 0xffffffff,HEX) + "." + String((int)(dataStore[0] >> 32))); }
      for (uint16_t n=1;n<currentTags;n++) { client.print(String(dataStore[n] & 0xffffffffffffff,HEX) + String(".") + String((int)(dataStore[n] >> 56)) + String(";"));
        if (debug) { Serial.print("Send: "); Serial.println(String(dataStore[n] & 0xffffffffffffff,HEX) + "." + String((int)(dataStore[n] >> 56))); } }
      client.print("\r\n"); client.print("OK\r\n");
      serverTimer=millis()+5000; sent=true; }
    if (client.available()) { client.read(); } } }
