#ifndef CAPTIVE_PORTAL_H
#define CAPTIVE_PORTAL_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

extern WebServer server;
extern DNSServer dnsServer;

extern const char *apSSID;
extern const char *apPassword;
extern bool shouldStartPortal;

void checkCredentials();
void startConfigPortal();
void initializeNormalMode();
bool isPortalActive();
void handlePortal();
void handleRoot();
void handleSave();
void handleScan();
String scanNetworks();

#endif // CAPTIVE_PORTAL_H