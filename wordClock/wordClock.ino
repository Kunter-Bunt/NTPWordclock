#define FASTLED_ESP8266_D1_PIN_ORDER
#include "FastLED.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//WiFi Config
#define WLAN_SSID "ssid"
#define WLAN_PASS "pass"

//LED Config
#define LED_AMPS    400
#define LED_VOLTS   5
#define NUM_LEDS    150
#define DATA_PIN    D2
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB

//Time Config
#define TIMESERVER  "time.google.com"

//Other
#define SERIAL_SPEED  115200
#define INTERVAL      30000 //Milliseconds

byte hours = 0, minutes = 0;

void setup() {
  Serial.begin(SERIAL_SPEED);
  EEPROM.begin(512);
  
  initWifi(); 
  initNTP();
  Serial.println();
  
  initLEDS();
  Serial.println();
}

void loop() {
  handleRESTServer(); //Handle possible requests
  getCurrentTime(); //NTP
  showTime(hours, minutes); //Light LEDS
  delay(INTERVAL); 
}












