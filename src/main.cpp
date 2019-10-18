#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 
/////////////////////////////

int PiscoPump = 4;
int BebidaPump = 2;
int LED_RGB = NULL;

////BASIC WEB SETUP///////////
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
///////////HTTP REQUEST FUNCTIONS//////////
void handleRoot();
void handleNotFound();
void handleServo();
//////////Liquid Serve Declaring////////

void Servir();
void LED(bool A);
//////////////////////////////////////////
void setup() {
  
  Serial.begin(115200);
  delay(100);
  Serial.println('\n');
  pinMode(PiscoPump,OUTPUT);
  pinMode(BebidaPump,OUTPUT);
  pinMode(LED_RGB,OUTPUT);
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
  server.on("/Servir",HTTP_POST,Servir);
  server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/")

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

////////////////////////////////////////////////////////////////////////////////
void loop() {
  MDNS.update();
  server.handleClient();
}
//////////////////////////////////////////////////////////////////////////////////
void handleRoot() {
  //Do the Send Server Stuff
  server.send(200, "text/html", "<form action=\"/Servir\" method=\"<POST>\"><input type=\"submit\" value=\"Servir\"></form>");
}
//////////////////////////////////////////////////////////////////////////////////////////

void handleNotFound(){
  server.send(404,"text/plain","404: Not found");
}

//////////////////////////////////////////////////////////////////////////////////
void Servir(){
  int P = 20;
  int B = 80;
  Serial.println("SIRVIENDO:");
  LED(false); 
  int Max = 2100; ///ml
  int Proportioner = P+B;
  int PiscoQ = P/Proportioner;
  int BebidaQ = B/Proportioner;
  Serial.print(PiscoQ);
  Serial.print(BebidaQ);
  digitalWrite(PiscoPump,1);
  delay(25000);
  digitalWrite(PiscoPump,0);
  delay(8000);
  digitalWrite(BebidaPump,1);
  delay(22000);
  digitalWrite(BebidaPump,0);
  LED(true);
  delay(5000);
  LED(false);
}
void LED(bool A){
   Serial.println(".");  
}