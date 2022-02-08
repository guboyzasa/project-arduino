// Edittor By BOY'IT

#include <TM1637Display.h> // Download at -> https://github.com/avishorp/TM1637

#define CLK 2           // TM1637 CLK ขา 2
#define DIO 3           // TM1637 DIO ขา 3
#define IR_DETECTOR 4   // IR Sensor 1 ขา 4
#define IR_DETECTOR_2 5  // IR Sensor 2 ขา 5

bool RUN = false;
int SEC = 0; 
int MILLISEC = 0;
int HALFSEC = 0;

unsigned long LAST_COUNT_TIME, GET_COUNT_TIME;

TM1637Display display = TM1637Display(CLK, DIO);

void setup() {

  pinMode(IR_DETECTOR, INPUT);
  pinMode(IR_DETECTOR_2, INPUT);

  display.setBrightness(5);
  display.clear();

}

void loop() {
  if (digitalRead(IR_DETECTOR) == LOW) { // IR1 ถ้ามีวัตถุเข้าใกล้
    RUN = !RUN;                     // กลับสถานะ RUN = false
    LAST_COUNT_TIME = millis();     // เริ่มจับเวลา
  }
  
  if (RUN == true) {        // สถานะ RUN = true
    MAIN_COUNT();           // ทำงานใน Loop : MAIN_COUNT();
    
    if (digitalRead(IR_DETECTOR_2) == LOW) { // IR2 ถ้ามีวัตถุเข้าใกล้
      RUN = !RUN;           // กลับสถานะ RUN = false หยุดเวลา
    }
  }
}

// Loop : MAIN_COUNT();
void MAIN_COUNT() { 

  GET_COUNT_TIME = millis();
  if (GET_COUNT_TIME - LAST_COUNT_TIME >= 0) {
    HALFSEC++;

    if (HALFSEC % 2 == 0) {
      MILLISEC++;
      if (MILLISEC > 59) {
        MILLISEC = MILLISEC - 60;
        SEC++;
      }
      display.showNumberDec(SEC, true, 2, 0);
      display.showNumberDec(MILLISEC, true, 2, 2);
    }

    if (HALFSEC % 2 == 1) {
      display.showNumberDecEx(SEC, 0b01000000, true, 2, 0);
      display.showNumberDecEx(MILLISEC, 0b01000000, true, 2, 2);
    }

    LAST_COUNT_TIME = millis();
  }
}
