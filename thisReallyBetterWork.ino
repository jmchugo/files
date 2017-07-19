/*NOTES TO SELF:
  - Try to create brightness feed and see if it'll work
  - OTHER SENSORS:
    - (DONE) Soil
    - Infrared
    - Sound
    - Light
    

*/


#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <Adafruit_NeoPixel.h>
#include <WiFiClient.h>
#ifdef __AVR__

  #include <avr/power.h>
#endif

#define PIXEL_PIN 14
#define PIXEL_COUNT 74
#define PIXEL_TYPE NEO_GRB + NEO_KHZ800

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//https://learn.adafruit.com/adafruit-io-basics-esp8266-arduino/
//Install Adafruit's MQTT library
//Install the Adafruit IO library
#include "AdafruitIO_WiFi.h"

/************************ Adafruit IO Config *******************************/

/******************************* WIFI Config **************************************/


/******************************* SETUP **************************************/

IPAddress localhost = IPAddress(192, 168, 1, 42);
WiFiServer server(localhost, 80);
int pixelColor = 16777215;
int valueRainbow = 3;
const char* hexInput = "ffffff";





void setup() {

//Allow the NodeMCU to talk to the computer
  Serial.begin(9600);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("Portal", "Password");

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");


  //Wait for a connection

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("strip begun");

  Serial.print("Use this URL to connect: ");
  Serial.print(localhost);
  
  
}


void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

  delay(500); // wait for server to respond

  // read response
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    // Serial.print(line);
    // we’ll parse the HTML body here
    if (section=="header") { // headers..
      Serial.print(".");
      if (line=="\n") { // skips the empty space at the beginning 
        section="json";
      }
    }

 
  // Match the request

  if (request.indexOf("/off.html") != -1) {
    colorWipe(0, 35);
    Serial.println("All off");
  }
  if (request.indexOf("/rainbow.html") != -1)  {
    rainbowCycle(35);
    Serial.println("Rainbow on");
  }

  if (request.indexOf("/day.html") != -1)  {
    colorWipe(16777215, 35); 
    Serial.println("Day on");
  }

  if (request.indexOf("/night.html") != -1)  {
    colorWipe(10111235, 35);
    Serial.println("Night on");
  }

  if (request.indexOf("/purple.html") != -1)  {
    colorWipe(8323200, 35); // Purple
    Serial.println("Purple on");
  }
  if (request.indexOf("/hex.html") !=-1) {
    Serial.println("I really need to figure this out");
  }

  Serial.println(request);
  Serial.println("request printed");
  client.flush();
  Serial.println("client flushed");
  delay(10);
  
}


  
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycle of all color
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}



/*void Hex(*hexInput) {
  
  // print RGB values and hex value
  Serial.print("Received HEX: ");
  Serial.println(hexInput);
  

  long color = hexInput->toNeoPixel();

  for(int i=0; i<PIXEL_COUNT; ++i) {
    pixels.setPixelColor(i, color);
    pixelColor = (i, color);
  }
  colorWipe(pixelColor, 35);
  Serial.print("Pixel Color: ");
  Serial.println(pixelColor);
}*/
