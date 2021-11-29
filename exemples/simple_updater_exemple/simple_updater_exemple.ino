#include "EspSimpleWifiHandler.h"
#include "EspSimpleRemoteUpdate.h"

// Automatic WiFI management with EspSimpleWifiHandler.
// You can use whatever you want for the WiFi management. Here I choose EspSimpleWifiHandler for simplicity.
EspSimpleWifiHandler wifiHandler("WIFI_SSID", "WIFI_PASSWORD", "HOSTNAME");

// The updater.
EspSimpleRemoteUpdate updater;

void setup() {
  Serial.begin(115200);
  wifiHandler.enableDebuggingMessages();
  updater.enableDebuggingMessages();

  updater.enableHTTPWebUpdater("USERNAME", "PASSWORD");
  updater.enableOTA("PASSWORD");
}

void loop() {
  updater.handle();
}
