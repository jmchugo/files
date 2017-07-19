#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <Adafruit_NeoPixel.h>

 
int ledPin = LED_BUILTIN; // GPIO13
WiFiServer server(IPAddress(192,168,1,42), 80);
 
void setup() {
  Serial.begin(9600);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("Portal", "Password");

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client;
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = HIGH;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.println("<head>");
  client.println("<title>jscolor Example</title>");
  client.println("</head>");

  client.println("<body style='text-align:center;'>");
  client.println("<script src='jscolor.js'></script>");
  client.println("<button class='jscolor {valueElement:'chosen-value', onFineChange:'setTextColor(this)'}'>");
  client.println("Pick light color");
  client.println("</button>");
  client.println("</body>");
  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
