#include <WiFi.h>

void initWLAN() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname("humanCounter");
  WiFi.softAP(ssidAP,passAP); }

void connectWLAN() {
  if (debug) { Serial.println("WLAN connect."); }
  WiFi.disconnect();
  WiFi.begin(ssidClient,passClient); }

void wlanWorker() {
  static uint64_t wlanTimer=4000;
  if (millis()>=wlanTimer) { wlanTimer=millis()+20000;
    if (WiFi.status()!=WL_CONNECTED) { connectWLAN(); } else { if (debug) { Serial.print("Local IP: "); Serial.println(WiFi.localIP()); } } } }
