#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 
#include <Servo.h>
/////////////////////////////

Servo XServo;
Servo YServo;

////BASIC WEB SETUP///////////
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
///////////HTTP REQUEST FUNCTIONS//////////
void handleRoot();
void handleNotFound();
void handleServo();
///////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println('\n');
  XServo.attach(2);
  XServo.write(0);
  wifiMulti.addAP("Qdeli","lr134679");

  Serial.println("Conecting");
  int i = 0;
  while(wifiMulti.run() != WL_CONNECTED){
    delay(200);
    Serial.println('.');
    }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")){              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  }
  else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/",HTTP_GET,handleRoot);  // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/SERVO",HTTP_POST,handleServo);
  server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/")

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
}
void handleServo(){
  XServo.write(180);
  delay(500);
  XServo.write(0);
  server.sendHeader("Location","/"); // Add a header to respond with a new location for the browser to go to the home
  server.send(303); // Send it back to the browser with an HTTP status 303
}
void handleRoot() {
  //Do the Send Server Stuff
  server.send(200, "text/html", "<form action=\"/SERVO\" method=\"POST\"><input type=\"submit\" value=\"Toggle SERVO\"></form>");
}

void handleNotFound(){
  server.send(404,"text/plain","404: Not found");
}
