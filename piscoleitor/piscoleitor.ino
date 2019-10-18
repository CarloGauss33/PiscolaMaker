#include <ESP8266WiFi.h>
/////////////////HELP MY codigo vale callampa

static const int Button1 =4;                    ///BUTTON 1 Stands for 20/80 normalfag
static const int motor_bebida =3;                 ///BUTTON 2 Stand for 50/50 Chad and stacy style
static const int motor_pisco =6;                  ///BUTTON 3 Stand for 80/20 WTF dude, we also sell firearms

int Button2;
int Button3;

const char* ssid = "Qdeli";
const char* pass = "lr134679";

WiFiServer server(80);

String header;

String valueStrinng= String(5);
int pos1=0;
int pos2=0;

void make_basic() {
      analogWrite(motor_bebida,255);
      Serial.println("Bomba bebida funcionando");
      delay(8000);
      Serial.println("Fin Bomba Bebida");
      analogWrite(motor_bebida,0);
      analogWrite(motor_pisco,210);
      Serial.println("Bomba Pisco funcionando");
      delay(2000);
      analogWrite(motor_pisco,0);
      Serial.println("Fin Bomba Pisco");
}
void setup() {
  Serial.begin(115200);

  //pinMode(Button1,INPUT);
  //pinMode(Button2,INPUT);
  //pinMode(Button3,INPUT);
 // pinMode(motor_bebida,OUTPUT);
 // pinMode(motor_pisco,OUTPUT);
 // analogWrite(motor_bebida,0);
 // analogWrite(motor_pisco,0);

  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Inicializado");
}

String state = "APAGADO";
int buttonstate = 0;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long TimeoutTime = 2000;
void loop(){

  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;    
    Serial.println("NEW CLIENT");
    while(client.connected()&& currentTime-previousTime<=TimeoutTime){
        currentTime = millis();
        
        if (client.available()){
           char c = client.read();
           Serial.write(c);
           header += c;
      
           if (c == '\n') {
              if (currentLine.length()==0){

                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-type:text/html");
                  client.println("Connection: close");
                  client.println();

                  if (header.indexOf("Get /5/on")>=0){
                     Serial.println("Pump ON");
                     make_basic();
                     state = "ENCENDIDO";
                  }
 /////////////////////////////////////////////////////////////////////////////////////////////////////
                  client.println("<!DOCTYPE html>");
                  client.println("<head>");
                  client.println("<meta charset='utf-8'>");
                  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
                  client.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>");
                  client.println("<style>");
                  client.println("body {");
                  client.println("margin: 0;");
                  client.println("font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol';");
                  client.println("font-size: 1rem;");
                  client.println("font-weight: 400;");
                  client.println("line-height: 1.5;");
                  client.println("color: #212529;");
                  client.println("background-color: #FFF;");
                  client.println("}");
                  client.println(".my_jumbotron {     ");
                  client.println("text-align=center;");
                  client.println("padding: 2rem 1rem;");
                  client.println("margin-bottom: 2rem;");
                  client.println("margin: 1rem;");
                  client.println("background-color: yellow;");
                  client.println("border-radius: 0.3rem;");
                  client.println("        }");
                  client.println(".container {");
                  client.println("width: 100%;");
                  client.println("padding-right: 15px;");
                  client.println("padding-left: 15px;");
                  client.println("margin-right: auto;");
                  client.println("margin-left: auto;");
                  client.println("        }");
                  client.println(".container_bottom {");
                  client.println("width: 40%;");
                  client.println("padding-right: 10%;");
                  client.println("padding-left: 10%;");
                  client.println("padding-bottom: 30%;");
                  client.println("margin-right: auto;");
                  client.println("background-color: orange;");
                  client.println("margin-left: auto;");
                  client.println("border-radius: 7%;");
                  client.println("        }");
                  client.println("</style>");
                  client.println("</head>");
                  client.println("<body>");
                  client.println("<div class='my_jumbotron'>");
                  client.println("<div class='container'>");
                  client.println("<h1 style= 'text-align: center;'>SIDING Sistema de Informacion Y Gestion        </h1>");
                  client.println("</div>");
                  client.println("</div>");
                  client.println("<p style='text-align: center; color=white;'>Ouch! te hechaste la carrera :( </p>");
                  client.println("<div class='container_bottom'>");
                  client.println("<h2 style='text-align: center;'>Mejor Tomemosno una Piscola!</h2>");
                  client.println("<p>STADO= " + state + "</p>");
                  client.println("<p style='text-align:center'><a href=\"/5/on\"><button class=\"button\">UWU</button></a> <p>");
                  client.println("</div>");
                  client.println("</body>");
                  client.println("</html>");
                  client.println();

                  Serial.println(header);
                  Serial.println(currentLine);
                  break;}
           
          else {
                  currentLine="";}
        }
 //////////////////////////////////////////////////////////////////////////////////////////////////////////
         else if (c != '\r') {  
          currentLine += c;  
        }
                 
      }

                 
    }
    // Clear the header variabl
            header = ""; 
         client.stop();
         Serial.println("CLIENT DISCONECTED");

              }

     
       
       
      

    




  /*
  else if(digitalRead(Button2)==LOW){
      Serial.println("boton2 activo");
      analogWrite(motor_bebida,255);
      Serial.println("Bomba bebida funcionando");
      delay(5000);
      Serial.println("Fin Bomba Bebida");
      analogWrite(motor_bebida,0);
      analogWrite(motor_pisco,210);
      Serial.println("Bomba Pisco funcionando");
      delay(5000);
      analogWrite(motor_pisco,0);
      Serial.println("Fin Bomba Pisco");
  }
  else if(digitalRead(Button3)==LOW){
      Serial.println("boton3 activo");
      analogWrite(motor_bebida,255);
      Serial.println("Bomba bebida funcionando");
      delay(2000);
      Serial.println("Fin Bomba Bebida");
      analogWrite(motor_bebida,0);
      analogWrite(motor_pisco,210);
      Serial.println("Bomba Pisco funcionando");
      delay(8000);
      analogWrite(motor_pisco,0);
      Serial.println("Fin Bomba Pisco");
      */
    

};
