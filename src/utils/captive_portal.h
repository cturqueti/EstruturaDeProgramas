#ifndef CAPTIVE_PORTAL_H
#define CAPTIVE_PORTAL_H

#include "wifi/wifi_manager.h"
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

extern WebServer server;

class CaptivePortal
{
public:
    CaptivePortal();
    ~CaptivePortal();

    bool startCaptivePortal();
    void stopCaptivePortal();

    void handleRoot();
    void handleSave();

    void handlePortal();
    void handleScan();

    String scanNetworksToJSON();
    void handleGetNetworks();

    inline bool isPortalActive() { return _shouldStartPortal && (millis() - _portalStartTime < PORTAL_TIMEOUT); };

private:
    WiFiManager wifiManager;
    DNSServer dnsServer;
    unsigned long _portalStartTime = 0;
    bool _shouldStartPortal = false;
    const unsigned long PORTAL_TIMEOUT = 5 * 60 * 1000; // 5 minutos
    String scanNetworks();

    bool startTask();
};

void connectCaptivePortalStatic(void *pvParameters);

#endif // CAPTIVE_PORTAL_H