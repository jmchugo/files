
#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__

  #include <avr/power.h>
#endif

#define PIXEL_PIN 14
#define PIXEL_COUNT 74
#define PIXEL_TYPE NEO_GRB + NEO_KHZ800

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

#include "AdafruitIO_WiFi.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Portal", "Password");

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
