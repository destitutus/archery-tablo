#include <timer-api.h>
#include <timer_setup.h>

#include <Adafruit_NeoPixel.h>

#define INFO_PIN 7
#define BUZZER_PIN 11
#define NUMBERS_PIN 6

#define START_PIN 14
#define STOP_PIN 15

Adafruit_NeoPixel numbersStrip = Adafruit_NeoPixel(120, NUMBERS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel infoStrip = Adafruit_NeoPixel(240, INFO_PIN, NEO_GRB + NEO_KHZ800);

uint32_t RED_COLOR = infoStrip.Color(255, 0, 0);
uint32_t GREEN_COLOR = infoStrip.Color(0, 255, 0);
uint32_t YELOW_COLOR = infoStrip.Color(255, 255, 0);

uint32_t infoColor = infoStrip.Color(255, 0, 0);
uint32_t numberColor = numbersStrip.Color(255, 0, 0);

int writeAArray [] = { 14, 15, 16, 17, 18, 19, 21, 24, 59, 62, 63, 66, 98, 99, 100, 101, 102, 103 };
int writeBArray [] = { 105, 106, 107, 108, 109, 110, 111, 140, 143,  146, 147, 150, 153, 182, 185, 188, 190, 191, 193, 194};
int writeCArray [] = { 7, 8, 9, 10, 11, 29, 35, 48, 54, 71, 77, 91, 95 };
int writeDArray [] = { 113, 114, 115, 116, 117, 118, 119, 132, 138, 155, 161, 174, 180, 198, 199, 200, 201, 202 };

bool wasChanged = true;
bool wasStarted = false;
bool isPrepare = true;

unsigned long toneActiveTill = 0;

int countDown = 10;

void turnAllInfoOff() {
  for (int i=0; i < infoStrip.numPixels(); i++) {
    infoStrip.setPixelColor(i, 0);
  }
}

void turnAllNumOff() {
  for (int i=0; i < numbersStrip.numPixels(); i++) {
    numbersStrip.setPixelColor(i, 0);
  }
}

void writeA() {
  for (int i = 0; i < 18; i++) {
    infoStrip.setPixelColor(writeAArray[i], infoColor);
  }
}

void writeB() {
  for (int i = 0; i < 20; i++) {
    infoStrip.setPixelColor(writeBArray[i], infoColor);
  }
}

void writeC() {
  for (int i = 0; i < 13; i++) {
    infoStrip.setPixelColor(writeCArray[i], infoColor);
  }
}

void writeD() {  
  for (int i = 0; i < 18; i++) {
    infoStrip.setPixelColor(writeDArray[i], infoColor);
  }
}

void writeAB() {
  writeA();
  writeB();
}

void writeCD() {
  writeC();
  writeD();
}

//int writeDArray [] = { 113, 114, 115, 116, 117, 118, 119, 132, 138, 155, 161, 174, 180, 198, 199, 200, 201, 202 };

void writeI1() {
  int writeI1Array [] = {  };
  for (int i = 0; i < 11; i++) {
    //infoStrip.setPixelColor(writeI1Array[i], infoColor);
  }
}

void write0(int offset) {  
  for (int i = 0; i < 40; i++) {
    if (i < 18 || i > 21) {
      numbersStrip.setPixelColor(i + offset, numberColor);
    }
  }
}

void write1(int offset) {  
  for (int i = 11; i < 18; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 33; i < 40; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write2(int offset) {  
  for (int i = 6; i < 34; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write3(int offset) {  
  for (int i = 6; i < 40; i++) {
    if (i < 22 || i > 27) {
      numbersStrip.setPixelColor(i + offset, numberColor);
    }
  }
}

void write4(int offset) {  
  for (int i = 0; i < 7; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 11; i < 22; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 33; i < 40; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write5(int offset) {  
  for (int i = 0; i < 12; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 18; i < 22; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 28; i < 40; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write6(int offset) {  
  for (int i = 0; i < 12; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 18; i < 40; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write7(int offset) {  
  for (int i = 7; i < 18; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
  for (int i = 33; i < 40; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write8(int offset) {  
  for (int i = 0; i < 40; i++) {
    numbersStrip.setPixelColor(i + offset, numberColor);
  }
}

void write9(int offset) {  
  for (int i = 0; i < 40; i++) {
    if (i < 22 || i > 27) {
      numbersStrip.setPixelColor(i + offset, numberColor);
    }
  }
}

void writeNum(int num, int offset) {
  switch (num) {
    case 0:
      write0(offset);
      break;
    case 1:
      write1(offset);
      break;
    case 2:
      write2(offset);
      break;
    case 3:
      write3(offset);
      break;
    case 4:
      write4(offset);
      break;
    case 5:
      write5(offset);
      break;
    case 6:
      write6(offset);
      break;
    case 7:
      write7(offset);
      break;
    case 8:
      write8(offset);
      break;
    case 9:
      write9(offset);
      break;
     
  }
}

int getDigit(int chislo, size_t offset)
{
    char str_chislo[20];
    sprintf(str_chislo, "%d", chislo);
    if (offset > strlen(str_chislo)) {
        return -1;
    }
    for (size_t i = 1; i < offset; i++) {
        chislo /= 10;
    }
    return chislo % 10;
}

void writeNum(int num) {
  turnAllNumOff();
  int n3 = getDigit(num, 3);
  if (n3 > 0) {
    writeNum(n3, 0);
  }
  int n2 = getDigit(num, 2);
  if (n2 > 0 || n3 != 0) {
    writeNum(n2, 40);
  }
  int n1 = getDigit(num, 1);
  writeNum(n1, 80);
}

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  infoStrip.begin();
  turnAllInfoOff();
  infoStrip.setBrightness(128);
  infoStrip.show();

  numbersStrip.begin();
  //numbersStrip.setBrightness(128);
  numbersStrip.show();

  pinMode(START_PIN, INPUT);
  pinMode(STOP_PIN, INPUT);

  wasChanged = true;
  countDown = 0;
  draw();
}

void beep() {
  toneActiveTill = millis() + 500;
  tone(BUZZER_PIN, 262, 600);  
  delay(600);
}

void checkBeep() {
  if (toneActiveTill != 0 && toneActiveTill < millis()) {
    toneActiveTill = 0;
  }
}

void timer_handle_interrupts(int timer) {
  wasChanged = true;
  countDown--;
  if (countDown == 30) {
    numberColor = YELOW_COLOR;
  }
}

void stop() {  
  countDown = 0;    
  numberColor = RED_COLOR;
  wasChanged = true;  
  wasStarted = false;
  isPrepare = true;
  timer_stop_ISR(TIMER_DEFAULT);
  draw();  
  beep();
}

void start() {
  countDown = 10;
  wasStarted = true;
  wasChanged = true;  
  numberColor = RED_COLOR;
  timer_init_ISR_1Hz(TIMER_DEFAULT);  
  draw();
  beep();
  Serial.println("Start signal");
}

void draw() {
    //writeA();
    //writeB();

    writeCD();
  
    infoStrip.show();
 
    writeNum(countDown);
    numbersStrip.show();
    wasChanged = false;
}

void loop() {
  int startSignal = digitalRead(START_PIN);
  if (startSignal == HIGH && !wasStarted) {
    start();
  }

  int stopSignal = digitalRead(STOP_PIN);
  if (stopSignal == HIGH && wasStarted) {
    stop();
  }

  if (wasStarted) {
    if (wasChanged) {
      draw();
    }
    if (countDown == 120) {
      beep();
    }
    if (countDown == 0) {
      if (isPrepare) {
        isPrepare = false;
        countDown = 120;
        numberColor = GREEN_COLOR;          
      } else {
        stop();
      }
      
    }
  }

  checkBeep();
}
