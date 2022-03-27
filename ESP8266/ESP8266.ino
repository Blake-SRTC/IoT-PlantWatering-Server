#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WIFI //
const char* ssid = "ST-MKT-1";
const char* pass = "12345678";

// Server
const char* host = "http://10.10.10.6:3000/api/v1/ambiente";

byte cont = 0;
byte max_intentos = 50;

// DHT Sensor //
DHT dht(D1, DHT11);
float temp, hume;

void setup() {
  Serial.begin(9600);
  Serial.println("\n");

  // Inicia WiFi
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

    // Inicia DHT //
    dht.begin();

    // Inicia Relay
    pinMode(D2, OUTPUT);
  }
  else {
    Serial.println("Error de Conexion");
  }

}

void loop() {
  // Relay test
  digitalWrite(D2, LOW);
  delay(2000);
  digitalWrite(D2, HIGH);
  delay(2000);

  // Comprobar estado WiFi
  if (WiFi.status()== WL_CONNECTED) {

    // DHT
    hume = dht.readHumidity();
    temp = dht.readTemperature();
    Serial.println("Temperatura: " + String(temp) + " C " + " Humedad: " + String(hume) + " %");

    // JSON DOC
    String temperatura = String(temp);
    String humedad = String(hume);

    DynamicJsonDocument doc(2048);
    doc["Temperatura"] = temperatura;
    doc["Humedad"] = humedad;

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
    
  }
  delay(5000);

}
