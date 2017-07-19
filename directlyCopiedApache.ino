#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>




const char* host     = "192.168.1.42"; // Your domain  
String path          = "/theThing/light.json";  
const int pin        = LED_BUILTIN;

void setup() {  
  pinMode(pin, OUTPUT); 
  pinMode(pin, LOW);
  Serial.begin(9600);

  delay(10);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Portal", "Password");

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
    Serial.print(line);
    // we’ll parse the HTML body here
    if (section=="header") { // headers..
      Serial.print(".");
      if (line=="\n") { // skips the empty space at the beginning 
        section="json";
      }
    }
    else if (section=="json") {  // print the good stuff
      section="ignore";
      String result = line.substring(1);

      // Parse JSON
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success())
      {
        Serial.println("parseObject() failed");
        return;
      }

      // Make the decision to turn off or on the LED
      if (strcmp(json_parsed["light"], "on") == 0) {
        digitalWrite(pin, HIGH); 
        Serial.println("LED ON");
      }
      else {
        digitalWrite(pin, LOW);
        Serial.println("led off");
      }
    }
  }
  Serial.println("closing connection. ");
}
