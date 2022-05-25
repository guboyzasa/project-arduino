//iclude
#include <WiFi.h>               //wifi
#include <WiFiClient.h>         //wifi
#include <BlynkSimpleEsp32.h>   //blynk ESP32
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  //LCD I2C
#include <ESP32_Servo.h>        //Servo ESP32
#include "DHT.h"            //DHT

//myservo
Servo myservo;
int ser = 15;     //Servo at 15 || D15

//blynk
#define BLYNK_PRINT Serial
char auth[] = "XtkHWcvZc68ms9SPVn3DdEXZSRAyQId_";  //Token Project App Blynk
char ssid[] = "Office_2021_WiFi";                   //Name WiFi
char pass[] = "11110000b";                          //Password WiFi
char server[] = "blynk.honey.co.th";                //Server App Blynk
int port = 8080;                                    //Port Http
WidgetLCD lcd1(V5);            //LCD App Blynk

//dht
#define DHTPIN 5                   //Sensor DHT22 at 5 || D5
#define DHTTYPE DHT22                     //Type DHT22
DHT dht(DHTPIN, DHTTYPE);

//LCD I2C
LiquidCrystal_I2C lcd(0x3F, 16, 2);        //LCD I2C (code LCD I2C,column,row)

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);        //Serial Port 115200
  
  myservo.attach(ser);         //Servo at 15 || D15 at int ser = 15;

  dht.begin();

  Blynk.begin(auth, ssid, pass, server, port);  //Connect WiFi App Blynk
  lcd.setCursor(0, 0);                          //LCD I2C 0x0
  lcd.print("Blynk Wait ..");                   //LCD I2C show "Blynk Wait .."
  lcd1.clear();                                 //LCD App Blynk Clear
  lcd1.print(3, 0, "Blynk Wait ..");            //LCD App Blynk 6x0  show "Blynk Wait .."
  delay(5000);

  if (Blynk.connected() == true) {       //if connect true will working
    lcd1.print(3, 1, "Blynk Connect");   //LCD App Blynk 6x0  show "Blynk Connect"
    lcd.setCursor(0, 1);                 //LCD I2C 0x1
    lcd.print("Blynk Connect");          //LCD I2C show "Blynk Connect"
    delay(5000);
    lcd1.clear();                        //LCD App Blynk Clear
  }
}

void loop()
{
  lcd1.print(6, 0, "WELCOME");           //LCD App Blynk 6x0  show "Welcome"
  lcd1.print(4, 1, "please press");      //LCD App Blynk 4x1  show "please press"
  
  float h = dht.readHumidity();          //Read DHT Humi
  float t = dht.readTemperature();       //Read DHT Temp
  float f = dht.readTemperature(true);   //Read DHT Fahrenheit

  lcd.setCursor(0, 0);
  lcd.print("Humi : ");
  lcd.print(h);                          //show DHT Humi on LCD I2C
  lcd.print(" %    ");
  Serial.println(h);

  lcd.setCursor(0, 1);
  lcd.print("Temp : ");
  lcd.print(t);                          //show DHT Temp on LCD I2C
  lcd.print(" C    ");
  Serial.println( t);
  Blynk.virtualWrite(V0, h);             //show DHT Humi on App Blynk at V0
  Blynk.virtualWrite(V1, t);             //show DHT Temp on App Blynk at V1
  delay(1000);

  Blynk.run();                           //App Blynk Run Function
}
BLYNK_WRITE(V6)                          //App Blynk Write at V6
{
  myservo.write(180);                    //Servo on 180 
  delay(1000);
  myservo.write(param.asInt());          //Servo set 0
}
