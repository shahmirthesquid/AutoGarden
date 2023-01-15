#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  //#include <SPIFFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <FS.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
//#include <LittleFS.h>


void notFound(AsyncWebServerRequest *request);
String readFile(fs::FS &fs, const char * path);

void writeFile(fs::FS &fs, const char * path, const char * message);
String outputState(int relay);
String processor(const String& var);
void serverSetup();
void readSetRelays();
void toggleRelay(char* string);
void writeRelays();
void updateRelay(char* state,char* relayString);
void updateTimer(char* state,char* timerString);
