#include <Arduino.h>
//#include "ArduinoBlueIrisALPHAESP8266.h"
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  //#include <SPIFFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  //#include <FS.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <LittleFS.h>


//#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h> //esp8266
//#include <ArduinoJson.h>
#include "MD5.h"

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


const char* login( const char* hostIP, const char* user, const char* pass);