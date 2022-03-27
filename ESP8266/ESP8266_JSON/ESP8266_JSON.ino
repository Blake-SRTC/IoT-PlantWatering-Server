#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define DHTPIN 1     // what digital pin the DHT11 is conected to
#define DHTTYPE DHT11   // setting the type as DHT11
DHT dht(DHTPIN, DHTTYPE);
const char *ssid = "ST-MKT-1"; //Enter your WIFI ssid
const char *password = "12345678"; //Enter your WIFI password
const char *server_url = "https://10.10.10.6:3000/api/v1/ambiente";// Nodejs application endpoint
StaticJsonBuffer<200> jsonBuffer;
// Set up the client objet
WiFiClient client;
HTTPClient http;
void setup() {
   delay(3000);
   Serial.begin(9600);
   dht.begin();
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
   Serial.println("WiFi connected");
   delay(1000);
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();         
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("Temperature = ");
  Serial.print(t); 
  Serial.println("C  ");
JsonObject& values = jsonBuffer.createObject();
  values["humidity"] = h;
  values["temperature"] = t;
  
   http.begin(client, server_url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(values);
    if(httpCode > 0){
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.print("Response: ");Serial.println(payload);
        }
    }else{
         Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }
    http.end();
}
delay(5000);
}
