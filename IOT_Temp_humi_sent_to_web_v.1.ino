#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>

//const char* ssid = "Office";
//const char* pass = "11111111";

#define DHTTYPE DHT22
uint8_t DHTPIN = 5;

int ledred = 0;
int ledgreen = 4;

DHT dht(DHTPIN, DHTTYPE);

#define Temperature
#define Humidity
#define ESP_AP_NAME "IOT By BOy Barbiq"
void setup () {
  //  WiFi.begin(ssid, pass);
  pinMode(DHTPIN, INPUT);
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);

  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  wifiManager.autoConnect(ESP_AP_NAME);
  digitalWrite(ledred, HIGH);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledred, HIGH);
    Serial.print(".");
    delay(500);
  }
  digitalWrite(ledred, LOW);
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void loop () {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if ((WiFi.status() == WL_CONNECTED)) {
    digitalWrite(ledgreen, HIGH);

    HTTPClient http;
    String url = "http://35.198.244.110/smb/add.php?temp=" + String(t) + "&humi=" + String(h);
    Serial.println(url);
    http.begin(url); //HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  delay(5000);
}
