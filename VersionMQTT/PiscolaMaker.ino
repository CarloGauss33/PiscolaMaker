#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;

const char *ssid = "Qdeli";
const char *password = "lr134679";
char   SERVER[50]   = "3.83.223.148";
int    SERVERPORT   = 13083;
String USERNAME = "piscolamaker_1";   
char   PASSWORD[50] = "123456789";     

unsigned long previousMillis = 0;

char charPulsador [15];
String strPulsador;
String strPulsadorUltimo;

char PLACA[50];

char valueStr[15];
String strStatus = "";
char PISCO[50];
char STATUS[50];
char PULSADOR[50];
char TIEMPO[50];

int pisco_ammount = 20;

int flujo_bomba = 10; // ml/s

int bomba_pisco = 12;
int bomba_bebida = 11;

int led_R = 6;
int led_G = 4;
int Led_B = 5;

int pulsador = 3;

//-------------------------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);

//------------------------CALLBACK-----------------------------
void callback(char* topic, byte* payload, unsigned int length) {

  char PAYLOAD[5] = "    ";
  
  Serial.print("Mensaje Recibido: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    PAYLOAD[i] = (char)payload[i];
  }
  Serial.println(PAYLOAD);

  if (String(topic) ==  String(STATUS)) {
    if (payload[1] == 'N'){
        tiempo_restante();
        servir();
    }
  }

  if (String(topic) ==  String(PISCO)) {
    pisco_ammount = String(PAYLOAD).toInt();
  }
}

//------------------------RECONNECT-----------------------------
void reconnect() {
  uint8_t retries = 3;
  // Loop hasta que estamos conectados
  while (!client.connected()) {
    Serial.print("Intentando conexion MQTT...");
    // Crea un ID de cliente al azar
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    USERNAME.toCharArray(PLACA, 50);
    if (client.connect("", PLACA, PASSWORD)) {
      Serial.println("conectado");
      client.subscribe(STATUS);
      client.subscribe(PISCO);
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" intenta nuevamente en 5 segundos");
      // espera 5 segundos antes de reintentar
      delay(5000);
    }
    retries--;
    if (retries == 0) {
      // esperar a que el WDT lo reinicie
      while (1);
    }
  }
}

//------------------------SETUP-----------------------------
void setup() {

  //OUTPUTS
  pinMode(Led_R, OUTPUT);
  pinMode(Led_G, OUTPUT);
  pinMode(Led_B, OUTPUT);

  pinMode(bomba_bebida, OUTPUT);
  pinMode(bomba_pisco, OUTPUT);

  //Inputs

  pinMode(pulsador, INPUT);


  // Inicia Serial
  Serial.begin(115200);
  Serial.println("");

  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      IPAddress ip(192,168,1,156); 
      IPAddress gateway(192,168,1,1); 
      IPAddress subnet(255,255,255,0); 
      WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
  
  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);

  String pisco_precentaje = "/" + USERNAME + "/" + "Pisco"; 
  pisco_precentaje.toCharArray(PISCO, 50);

  String button_on_push = "/" + USERNAME + "/" + "Pulsador"; 
  button_on_push.toCharArray(PULSADOR, 50);

  String tiempo_restante = "/" + USERNAME + "/" + "Tiempo"; 
  tiempo_restante.toCharArray(TIEMPO, 50);

  String status_sirviendo = "/" + USERNAME + "/" + "Status"; 
  status_sirviendo.toCharArray(STATUS, 50);
  
}

//--------------------------LOOP--------------------------------
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
    
  if (digitalRead(14) == 0) {
    strPulsador = "presionado";
  } else {
    strPulsador = "NO presionado";
  }

  if (strPulsador != strPulsadorUltimo) { //envia el estado del pulsador solamente cuando cambia.
    strPulsadorUltimo = strPulsador;
    strPulsador.toCharArray(valueStr, 15);
    Serial.println("Enviando: [" +  String(PULSADOR) + "] " + strPulsador);
    client.publish(PULSADOR, valueStr);
  }
}

//--------------------------------------------------------------------

void set_rgb(int red, int green, int blue){
    analogWrite(Led_R, red);
    analogWrite(Led_G, green);
    analogWrite(Led_B, blue);
}


void servir() {

    float calc_tiempo_pisco = (300 / flujo_bomba) * (pisco_ammount / 100) * 1000;

    int mitad_tiempo = calc_tiempo_pisco / 2;
    

    set_status(true);
    delay(500);
    set_rgb(255,0,0);

    delay(1000);
    analogWrite(bomba_pisco, 1022);
    set_rgb(255,40,0);
    delay(mitad_tiempo);

    set_rgb(255,150,0);
    delay(mitad_tiempo);
    analogWrite(bomba_pisco,0);
    set_rgb(255,255,0);
    delay(2000);

    float calc_tiempo_bebida = (300/flujo_bomba) * ((100 - pisco_ammount)/100) * 1000;

    int mitad_tiempo_bebida = calc_tiempo_bebida/2;

    analogWrite(bomba_bebida, 1022);
    set_rgb(150,255,0);
    delay(mitad_tiempo_bebida);

    set_rgb(40,150,0);
    delay(mitad_tiempo_bebida);
    analogWrite(bomba_bebida,0);
    set_rgb(255,0,0);

    make_success();

    set_status(false);
    delay(3000);
}


void make_success(){

}


void set_status(bool is){
    if (is == true){
        strStatus = "EN PROCESO";
        strStatus.toCharArray(strStatus, 15);
        client.publish(STATUS, strStatus);
    }

    else if (is == false){
        strStatus = "EN PAUSA";
        strStatus.toCharArray(strStatus, 15);
        client.publish(STATUS, strStatus);
    }
}
