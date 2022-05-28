#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WIFI //
//const char* ssid = "ST-MKT-1";
//const char* pass = "12345678";
//const char* ssid = "Blake_ST";
//const char* pass = "per1vat!UGK8dzk3qcy";
const char* ssid = "JHOSEP";
const char* pass = "Hurtad0C4321";

byte cont = 0;
byte max_intentos = 50;

// Server
//const char* host = "http://10.10.10.6:3000/api/v1/ambiente";
const char* host = "http://137.184.34.228:4050/api/v1/ambiente";

// DHT Sensor //
DHT dht(D1, DHT11);
float temp, hume;

// VARIABLES CAUDALIMETRO
long currentMillis = 0;         // ok
long previousMillis = 0;        // ok
int interval = 1000;            // ok
float calibrationFactor = 4.5;  // ok
volatile byte pulseCount;       // ok
byte pulse1Sec = 0;             // ok
float flowRate;                 // ok
unsigned long flowMilliLitres;  // ok
unsigned int totalMilliLitres;  // ok
float flowLitres;               // ok
float totalLitres;              // ok

// Caudalimetro parte
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

long senso = 7200000;

void setup() {
  Serial.begin(9600);
  Serial.println("\n");

  // Inicia WiFi
  pinMode(D8, OUTPUT);
  digitalWrite(D8, LOW);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED and cont < max_intentos) {
    cont++;
    delay(500);
    Serial.print(".");
  }
  
  
  Serial.println("");

  // Parametros de red recibidos
  if (cont < max_intentos) {
    Serial.println("WiFi conectado");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("MacAddress: ");
    Serial.println(WiFi.macAddress());
    digitalWrite(D8, HIGH);    

    // Inicia DHT //
    dht.begin();

    // Inicia Relay
    pinMode(D2, OUTPUT);
    digitalWrite(D2, LOW);

    // Inicia sensor suelo
    pinMode(A0, INPUT);

    // CAUDALIMETRO SETUP
    pinMode(D6, INPUT_PULLUP);
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;
    attachInterrupt(digitalPinToInterrupt(D6), pulseCounter, FALLING);
    
  }
  else {
    Serial.println("Error de Conexion");
    digitalWrite(D8, LOW);
  }

}

void loop() {

  // Comprobar estado WiFi
  if (WiFi.status() == WL_CONNECTED) {
    // Status WiFi
    digitalWrite(D8, HIGH);

    // DATOS DHT
    hume = dht.readHumidity();
    temp = dht.readTemperature();
    Serial.println("Temperatura: " + String(temp) + " C " + " Humedad: " + String(hume) + " %");
  
    //DATOS HUMEDAS SUELO
    int suelo = analogRead(A0);
    Serial.print("La lectura es: ");
    Serial.println(suelo);
    //   lecturas entre 1000 - 1023
    if (suelo >= 1000){
      Serial.println(">> EL SENSOR ESTA DECONECTADO O FUERA DEL SUELO <<");
    }
    else if (suelo <1000 && suelo >= 600){
      Serial.println(">> EL SUELO ESTA SECO <<" + suelo);
    }
    else if (suelo < 600 && suelo >= 370){
      Serial.println(">> EL SUELO ESTA HUMEDO <<" + suelo);
    }
    else if (suelo < 370){
      Serial.println(">> EL SENSOR ESTA PRACTICAMENTE EN AGUA <<" + suelo);
    }
    delay(1000);
    
    //Convirtiendo a Porcentaje
    int sueloPorcentaje = map(suelo, 1023, 500, 0, 100);
    Serial.print("La Humedad es del: ");
    Serial.print(sueloPorcentaje);
    Serial.println("%");

    // ACTIVAR RELAY
    if (sueloPorcentaje >= 40) {
      digitalWrite(D2, LOW);
    }else if (sueloPorcentaje < 45) {
      digitalWrite(D2, HIGH);
      
      // TIEMPO DE ESPERA PARA QUE EL AGUA FLUYA
      delay(1000);
      
      // APAGAR RELAY despues de regar x segundos
      digitalWrite(D2, LOW);

      delay(1000);
      
      // CAUDALIMETRO PROGRAMA PRINCIPAL
      currentMillis = millis();
      if (currentMillis - previousMillis > interval) 
      {
    
        pulse1Sec = pulseCount;
        pulseCount = 0;

        flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = millis();

        flowMilliLitres = (flowRate / 60) * 1000;
        flowLitres = (flowRate / 60);

        totalMilliLitres += flowMilliLitres;
        totalLitres += flowLitres;

        Serial.print("Flow rate: ");
        Serial.print(float(flowRate));  // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t"); 

        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);
        Serial.print("mL / ");
        Serial.print(totalLitres);
        Serial.println("L");
       }
    }
    
    // JSON DOC
    String temperatura = String(temp);
    String humedad = String(hume);
    String agua = String(totalLitres);

    DynamicJsonDocument doc(2048);
    doc["Temperatura"] = temperatura;
    doc["Humedad"] = humedad;
    doc["Hume_suelo"] = sueloPorcentaje;
    doc["Agua"] = agua;

    // JSON serializacion
    String json;
    serializeJson(doc, json);

    // Client POST Json
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, host);
    http.addHeader("Content-Type", "application/json");
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

    int codigo_respuesta = http.POST(json);

    // Respuestas de POST
    if(codigo_respuesta > 0) {
      Serial.println("Codigo HTTP: " + String(codigo_respuesta));
      if(codigo_respuesta == 200) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    http.end();
    
  } else {
    digitalWrite(D8, LOW);
  }
  delay(senso);

}