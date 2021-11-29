# Simple remote program update handling for ESP8266 and ESP32

Simple remote update management for ESP8266 and ESP32. 
Allow to easily set a light Web updater and/or OTA update. Intended to be used with an ESP8266 or ESP32.

What this library can do:
- Can host and manage a minimal web GUI to upload a new program (web updater)
- Can enable and manage Arduino OTA
- Automatically restart these services when a WiFI connection is lost.

I took some code from palpointe6/EspMQTTClient. So, thanks to the contributors that worked on this part from EspMQTTClient: @EdJoPaTo, @hallard.


# Get started

```c++
#include "EspSimpleRemoteUpdate.h"

EspSimpleRemoteUpdate updater;

void setup() {
  updater.enableHTTPWebUpdater("USERNAME", "PASSWORD");
  updater.enableOTA("PASSWORD");

  // Obiviously, you need to do some WiFi connection stuff.
  // EspSimpleRemoteUpdate will automatically detect a new WiFI connection and will start the enabled features automatically.
}

void loop() {
  updater.handle(); // Calling this at every loop() is mandatory.
}
```

# Some documentation

```c++
// Enable debugging messages (sent to Serial)
enableDebuggingMessages();

// Set the hostname
setHostname(const char* hostname);

// Enable the web updater
enableHTTPWebUpdater(const char* username = "", const char* password = "", const char* baseAddr = "/");

// Enable Arduino OTA
enableOTA(const char* password, const uint16_t port = 0);

// Calling this at every loop() is mandatory.
handle();
```