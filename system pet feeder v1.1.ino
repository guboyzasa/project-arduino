#include <SPI.h>
#include <Keypad.h> //Keypad
#include <LiquidCrystal_I2C.h> //LCD I2C
#include <ESP32_Servo.h> //Servo ESP32
#include "RTClib.h" //RTC
#include "DHT.h"

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {19, 18, 5, 17};
byte colPins[COLS] = {16, 4, 2, 15};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

RTC_DS3231 rtc;       //RTC

Servo myservo;          //servo
int servoPin = 12;

#define DHTPIN 23                   //Sensor DHT22 at 23
#define DHTTYPE DHT22              //Type DHT22    
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 20, 4); //LCD I2C 20x4

int t1, t2, t3, t4, t5, t6;
boolean feed = true;
char key;
int r[6];

void setup() {
  rtc.begin();
  dht.begin();
  lcd.init();
  lcd.backlight();
  myservo.attach(servoPin);
  myservo.write (0);
  Serial.begin(9600);
  rtc.adjust(DateTime(__DATE__, __TIME__)); //set date,time
}

void loop() {
  float humi = dht.readHumidity();          //Read DHT Humi
  float temp = dht.readTemperature();       //Read DHT Temp
  DateTime now = rtc.now();              //set Time
  lcd.setCursor(5, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(3, 1);
  lcd.print("Time: ");
  char buf1[] = "hh:mm:ss";             // set time
  lcd.print(now.toString(buf1));        // Print Time HH:MM:SS
  lcd.setCursor(9, 3);
  lcd.print("T:");
  lcd.print(temp);                         //show temp
  lcd.setCursor(15, 3);
  lcd.print("H:");
  lcd.print(humi);                      //show humi
  delay(1000);

  key = kpd.getKey();
  if (key != NO_KEY) {
    Serial.print(key);
  }
  if ('A' == key)
    autoSet();                //void autoSet
  if ('B' == key)
    setFeedingTime();         //void setFeedingTime
  if ('C' == key)
    clear();                  //void clear
  if ('D' == key)
    setManual();              //void setManual

  String t = "";
  t = now.toString(buf1);     //set time at t
  t1 = t.charAt(0) - 48;
  t2 = t.charAt(1) - 48;
  t3 = t.charAt(3) - 48;
  t4 = t.charAt(4) - 48;
  t5 = t.charAt(6) - 48;
  t6 = t.charAt(7) - 48;

  if (t1 == r[0] && t2 == r[1] && t3 == r[2] && t4 == r[3] && t5 < 1 && t6 < 3 && feed == true) {
    myservo.write(100);
    delay(200);
    myservo.write (0);
    delay(200);
    myservo.write(100);
    delay(200);
    myservo.write (0);
    delay(200);
    myservo.write(100);
    delay(200);
    myservo.write (0);
    lcd.clear();
    feed = false;
  }
}
void autoSet() {
  lcd.setCursor(6, 2);
  lcd.print("Auto Mode");
  delay(1000);
  myservo.write(100);
  delay(200);
  myservo.write (0);
  delay(200);
  myservo.write(100);
  delay(200);
  myservo.write (0);
  delay(200);
  myservo.write(100);
  delay(200);
  myservo.write (0);
  lcd.clear();
  feed = false;
}
void setFeedingTime() {
  feed = true;
  int i = 0;

  lcd.setCursor(2, 2);
  lcd.print("SetTime to HH:MM");

  while (1) {
    key = kpd.getKey();
    char j;
    if (key != NO_KEY) {
      lcd.setCursor(j, 3);
      lcd.print(key);
      r[i] = key - 48;
      i++;
      j++;
      if (j == 2) {
        lcd.print(":"); j++;
      }
      delay(500);
    }
    if (key == '#') {
      key = 0;
      break;
    }
    if (key == 'C') {
      break;;
    }
  }
}
void clear() {
  lcd.clear();
  feed = false;
}
void setManual() {
  feed = true;
  int x = 0;
  lcd.setCursor(2, 2);
  lcd.print("SetManual : ");
  while (1) {
    key = kpd.getKey();
    if (key != NO_KEY) {
      lcd.setCursor(14, 2);
      Serial.print(key);
      lcd.print(key);
      x = key - 49;
      if (x > 0) {
        for (int i = 0; x >= i ; i++) {
          myservo.write(100);
          digitalWrite(led, HIGH);
          Serial.print("on");
          delay(200);
          myservo.write (0);
          digitalWrite(led, LOW);
          Serial.print("off");
          delay(200);
        }
        clear();
        break;
      }
      if (key == 'C') {
        break;
      }
    }
  }
}
