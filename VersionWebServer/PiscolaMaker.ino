#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>

int pisco_pump = 0;
int bebida_pump = 0;

int push_button = 0;
int pin_wheel = 0;

const char* ssid = "Piscoleitor";
const char* pass = "";

String pushed = "";
String Header = "";

const char index_html[] PROGMEM  = R"rawliteral(
    <!DOCTYPE HTML><html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
        }
        h2 { font-size: 3.0rem; }
        p { font-size: 3.0rem; }
        .dht-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 15px;
        }
    </style>
    </head>
    <body>
        <h2>ESP8266 >Server</h2>
        <p>
            <span class="dht-labels">pushed</span> 
            <span id="PUSHED">%PUSHED%</span>
        </p>
    </body>
    <script>
    setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        document.getElementById("pushed").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/pushed", true);
    xhttp.send();
    }, 10000 ) ;
    </script>
    </html>)rawliteral";




String processor(const String& var){
  //Serial.println(var);
    if(var == "pushed"){
        return String(pushed);
    return String();
  }

ESP8266WebServer server(80);

void setup(){

    Serial.begin(115200);

    //--------------Pinout--------------//
    pinMode(pisco_pump, OUTPUT);
    pinMode(bebida_pump, OUTPUT);
    pinMode(push_button, INPUT);
    pinMode(pin_wheel, INPUT);

    //-----------Wifi SoftAP config---------//

    WiFi.softAP(ssid, pass);

    Serial.print("Soft AP starting with name " + ssid);

    IPAddress IP = Wifi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

      server.on("/", HTTP_GET, [](ESP8266WebServerRes *request){
    request->send_P(200, "text/html", index_html, processor);
    });
    
    server.on("/temperature", HTTP_GET, [](WebServerRequest *request){
        request->send_P(200, "text/plain    ", String(t).c_str());

    server.begin();
    Serial.print("");

}   

void loop(){

}

void handleClient(){

}

void servir(int leftqt, int rightqt){

    qtPump_1 = DoTheMaths(leftqt, rightqt);
    qtPump_2 = DoTheMaths(rightqt, leftqt);

    for (int i = 0, i<qtPump_1, i++){
        analogWrite(pisco_pump, 2550);
        delay(100);
        analogWrite(pisco_pump, 2550);
    };

    delay(1000);

    analogWrite(bebida_pump, 2500);

    delay(qtPump_2);

    analogWrite(bebida_pump, 0);
}