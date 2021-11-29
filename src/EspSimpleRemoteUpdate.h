/*
    EspSimpleRemoteUpdate.h
    Goal: Provide utilities to update your program wirelessly. Works with both ESP32 and ESP8266
    Github: https://github.com/plapointe6/EspSimpleRemoteUpdate
*/

#ifndef ESP_SIMPLE_REMOTE_UPDATE_H
#define ESP_SIMPLE_REMOTE_UPDATE_H

#include <ArduinoOTA.h>

#ifdef ESP8266
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPUpdateServer.h>
  #include <ESP8266mDNS.h> 
  #define ESPHTTPUpdateServer ESP8266HTTPUpdateServer
  #define WebServer ESP8266WebServer
  #define ESPmDNS ESP8266mDNS
#else // for ESP32
  #include <WebServer.h>
  #include "ESP32HTTPUpdateServer.h"
  #include <ESPmDNS.h>
  #define ESPHTTPUpdateServer ESP32HTTPUpdateServer
#endif

class EspSimpleRemoteUpdate {

private:
    // Http updater related
    WebServer* _httpServer = nullptr;
    ESPHTTPUpdateServer* _httpUpdater = nullptr;
    const char* _httpUpdaterUsername;
    const char* _httpUpdaterPassword;
    const char* _httpUpdaterBaseAddr = "/";

    // OTA related
    bool _enableOTA = false;

    // WiFI connection monitoring
    bool _wifiConnected = false;

    // Other
    const char* _hostname = nullptr;
    bool _enableDebugMessages = false;

public:
    EspSimpleRemoteUpdate() {};

    ~EspSimpleRemoteUpdate() {
        if (_httpServer)
            delete _httpServer;
        if (_httpUpdater)
            delete _httpUpdater;
    };

    inline void enableDebuggingMessages(bool enabled = true) {
        _enableDebugMessages = enabled;
    }

    inline void setHostname(const char* hostname) {
        _hostname = hostname;
    }

    void enableHTTPWebUpdater(const char* username = "", const char* password = "", const char* baseAddr = "/") {
        if (!_httpServer) {
            if (_enableDebugMessages)
                Serial.printf("UPDATER: Enabling web updater ...\n");

            _httpServer = new WebServer(80);
            _httpUpdater = new ESPHTTPUpdateServer(_enableDebugMessages);
            _httpUpdaterUsername = username;
            _httpUpdaterPassword = password;
            _httpUpdaterBaseAddr = baseAddr;
        }
        else if(_enableDebugMessages) {
            Serial.printf("UPDATER: Web updater already enabled.\n");
        }
    }

    void enableOTA(const char* password, const uint16_t port = 0) {
        if(_enableDebugMessages)
            Serial.printf("UPDATER: Enabling Arduino OTA ...\n");

        _enableOTA = true;

        if (password)
            ArduinoOTA.setPassword(password);

        if (port)
            ArduinoOTA.setPort(port);
    };

    void handle() {
        const bool wifiConnected = WiFi.status() == WL_CONNECTED;

        // A WiFi connection has occured
        if (wifiConnected && !_wifiConnected) {

            // Take the WiFi hostname if none is provided
            if (!_hostname) {
                _hostname = WiFi.getHostname();
            }

            // Enable mdns
            MDNS.begin(_hostname);

            // Init the web updater
            if (_httpServer)
            {
                _httpUpdater->setup(_httpServer, _httpUpdaterBaseAddr, _httpUpdaterUsername, _httpUpdaterPassword);
                _httpServer->begin();
                MDNS.addService("http", "tcp", 80);

                if (_enableDebugMessages) {
                    Serial.printf("UPDATER: Updater ready, open http://%s.local in your browser and login with username '%s' and password '%s'.\n", _hostname, _httpUpdaterUsername, _httpUpdaterPassword);
                }
            }

            // Init arduino OTA
            if (_enableOTA) {
                ArduinoOTA.setHostname(_hostname);
                ArduinoOTA.begin();

                if (_enableDebugMessages) {
                    Serial.printf("UPDATER: Arduino OTA ready.\n");
                }
            }
        }
        // WiFi has disconnected
        else if (!wifiConnected && _wifiConnected) {
            MDNS.end();
        }
        // We are connected to WiFi
        else if(wifiConnected && _wifiConnected) {

            if(_httpServer) {
                _httpServer->handleClient();
                #ifdef ESP8266
                    MDNS.update();
                #endif
            }

            if (_enableOTA)
                ArduinoOTA.handle();
        }

        _wifiConnected = wifiConnected;
    };
};

#endif
