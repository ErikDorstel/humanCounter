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
      for (uint8_t n=0;n<currentTags;n++) { client.print(dataStore[n] + String(";")); if (debug) { Serial.print("Send: "); Serial.println(dataStore[n]); } }
      client.print("\r\n"); client.print("OK\r\n");
      serverTimer=millis()+5000; sent=true; }
    if (client.available()) { client.read(); } } }
