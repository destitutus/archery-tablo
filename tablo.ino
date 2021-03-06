#include <timer-api.h>
#include <timer_setup.h>

#include <Adafruit_NeoPixel.h>

#define INFO_PIN 7
#define BUZZER_PIN 11
#define NUMBERS_PIN 6

#define START_PIN 14
#define STOP_PIN 15
#define NEXT_PIN 16
#define MODE_PIN 17

Adafruit_NeoPixel numbersStrip = Adafruit_NeoPixel(120, NUMBERS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel infoStrip = Adafruit_NeoPixel(231, INFO_PIN, NEO_GRB + NEO_KHZ800);

uint32_t RED_COLOR = infoStrip.Color(255, 0, 0);
uint32_t GREEN_COLOR = infoStrip.Color(0, 255, 0);
uint32_t YELOW_COLOR = infoStrip.Color(255, 255, 0);

uint32_t infoColor = infoStrip.Color(255, 0, 0);
uint32_t numberColor = numbersStrip.Color(255, 0, 0);

int writeAArray [] = { 14, 15, 16, 17, 18, 19, 21, 24, 59, 62, 63, 66, 98, 99, 100, 101, 102, 103 };
int writeBArray [] = { 105, 106, 107, 108, 109, 110, 111, 140, 143,  146, 147, 150, 153, 182, 185, 188, 190, 191, 193, 194};
int writeCArray [] = { 7, 8, 9, 10, 11, 29, 35, 48, 54, 71, 77, 91, 95 };
int writeDArray [] = { 113, 114, 115, 116, 117, 118, 119, 132, 138, 155, 161, 174, 180, 198, 199, 200, 201, 202 };

int writeI1Array [] = { 129, 163, 164, 165, 166, 167, 212, 228 };
int writeI2Array [] = { 122, 130, 163, 167, 169, 172, 206, 207, 213, 226, 227, 228, 229, 230 };
int writeI3Array [] = { 122, 130, 163, 169, 172, 206, 207, 209, 210, 214, 227, 228, 229 };
int writeI4Array [] = { 121, 122, 123, 124,  127, 166, 169, 205, 206, 207, 208, 209, 210, 230 };
int writeI5Array [] = { 121, 122, 123, 127, 130, 163, 166, 169, 172, 205, 209, 210, 214, 227, 228, 229 };
int writeI6Array [] = { 122, 123, 124, 125, 127, 130, 163, 166, 169, 172, 206, 209, 210, 214, 227, 228, 229 };
int writeI7Array [] = { 121, 130, 163, 172, 205, 206, 207, 169, 167, 213, 226 };
int writeI8Array [] = { 122, 123, 125, 127, 130, 163, 166, 169, 172, 206, 207, 209, 210, 214, 228, 227, 229 };
int writeI9Array [] = { 122, 123, 127, 130, 163, 166, 169, 172, 206, 207, 209, 210, 226, 228, 227, 229 };
int writeI10Array [] = { 122, 123, 124, 125, 130, 163,  172, 206, 207, 208, 209, 210, 214, 228, 227, 229, 38, 42, 43, 44, 45, 46, 218, 223 };

/**
 * AB / CD - 120
 * AB -- 120
 * AB / CD - 240
 * AB - 240
 */

int MODES_COUNT = 4;
int currMode = 0;
int currRound = 1;
int currStep = 0;

int sDuration [] = { 120, 120, 240, 240};
bool useCD [] = { true, false, true, false};
int sRounds [] = { 10, 10, 6, 6};
bool isAB = true;

bool wasChanged = true;
bool wasStarted = false;
bool isPrepare = true;

unsigned long nextButtonClick = 0;

unsigned long toneActiveTill = 0;

int countDown = 10;

void nextRound() {
  if (useCD[currMode] && currRound % 2 == 1) {
    isAB = false;
  } if (useCD[currMode] && currRound % 2 == 0) {
    isAB = true;
  }

  currStep++;

  bool needIncRound = !useCD[currMode] || (currStep % 2 == 0 && currStep != 0);
  
  if (needIncRound) {
    currRound++;
    if (currRound > sRounds[currMode]) {
      currRound = 1;
      currStep = 0;
    }
  }
}

void drawInfo() {
  turnAllInfoOff();
  writeRound();
  if (isAB) {
    writeAB();
  } else {
    writeCD();
  }
  wasChanged = true;
  draw();
}

void writeRound() {
  switch (currRound) {
    case 10:
      writeI10();
      break;
    case 1:
      writeI1();
      break;
    case 2:
      writeI2();
      break;
    case 3:
      writeI3();
      break;
    case 4:
      writeI4();
      break;
    case 5:
      writeI5();
      break;
    case 6:
      writeI6();
      break;
    case 7:
      writeI7();
      break;
    case 8:
      writeI8();
      break;
    case 9:
      writeI9();
      break;     
  }
}

void selectMode() {
  currMode++;
  if (currMode == MODES_COUNT) {
    currMode = 0;
  }
  currRound = 1;
  currStep = 0;
  wasChanged = true;
  countDown = sDuration[currMode];

  turnAllInfoOff();
  writeI1();
  writeAB();
  if (useCD[currMode]) {
    writeCD();
  }
  draw();
  beep(1);
  delay(2000);
  turnAllInfoOff();
  countDown = 0;  
  writeI1();
  writeAB();
  wasChanged = true;
  draw();
}

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

void writeFromArray(int sArray [], int sSize) {
  for (int i = 0; i < sSize; i++) {
    infoStrip.setPixelColor(sArray[i], infoColor);
  }
}

void writeA() {
  writeFromArray(writeAArray, sizeof(writeAArray) / sizeof(int));
}

void writeB() {
  writeFromArray(writeBArray, sizeof(writeBArray) / sizeof(int));
}

void writeC() {
  writeFromArray(writeCArray, sizeof(writeCArray) / sizeof(int));
}

void writeD() {  
  writeFromArray(writeDArray, sizeof(writeDArray) / sizeof(int));
}

void writeAB() {
  writeA();
  writeB();
}

void writeCD() {
  writeC();
  writeD();
}

void writeI1() {
  writeFromArray(writeI1Array, sizeof(writeI1Array) / sizeof(int));
}

void writeI2() {
  writeFromArray(writeI2Array, sizeof(writeI2Array) / sizeof(int));
}

void writeI3() {
  writeFromArray(writeI3Array, sizeof(writeI3Array) / sizeof(int));
}

void writeI4() {
  writeFromArray(writeI4Array, sizeof(writeI4Array) / sizeof(int));
}

void writeI5() {
  writeFromArray(writeI5Array, sizeof(writeI5Array) / sizeof(int));
}

void writeI6() {
  writeFromArray(writeI6Array, sizeof(writeI6Array) / sizeof(int));
}

void writeI7() {
  writeFromArray(writeI7Array, sizeof(writeI7Array) / sizeof(int));
}

void writeI8() {
  writeFromArray(writeI8Array, sizeof(writeI8Array) / sizeof(int));
}

void writeI9() {
  writeFromArray(writeI9Array, sizeof(writeI9Array) / sizeof(int));
}

void writeI10() {
  writeFromArray(writeI10Array, sizeof(writeI10Array) / sizeof(int));
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
  //infoStrip.setBrightness(128);
  infoStrip.show();

  numbersStrip.begin();
  //numbersStrip.setBrightness(128);
  numbersStrip.show();

  pinMode(START_PIN, INPUT);
  pinMode(STOP_PIN, INPUT);
  pinMode(NEXT_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);

  wasChanged = true;
  countDown = 0;
  writeAB();
  writeI1();
  draw();
}

void beep(int count) {
  Serial.print("count");
  Serial.println(count);

  if (count == 1) {
    tone(BUZZER_PIN, 262, 500);
    delay(700);
  }
  if (count == 2) {
    tone(BUZZER_PIN, 262, 350);
    delay(600);
    tone(BUZZER_PIN, 262, 350);
    delay(250);
  }
  if (count == 3) {
    tone(BUZZER_PIN, 262, 350);
    delay(600);
    tone(BUZZER_PIN, 262, 350);
    delay(600);
    tone(BUZZER_PIN, 262, 350);
    delay(600);
  }
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

  bool needNextRound = useCD[currMode] && currStep % 2 == 0 ;

  if (needNextRound) {
    countDown = 10;
    draw();
  } else {
    draw();
    beep(3);
  }

  nextRound();
  drawInfo();

  Serial.print("currStep");
  Serial.println(currStep);

  if (needNextRound) {
    start();
  }
}

void start() {
  countDown = 10;
  wasStarted = true;
  wasChanged = true;  
  numberColor = RED_COLOR;
  timer_init_ISR_1Hz(TIMER_DEFAULT);  
  draw();
  beep(2);
  Serial.println("Start signal");
}

void draw() {
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

  int modeSignal = digitalRead(MODE_PIN);
  if (modeSignal == HIGH && !wasStarted && nextButtonClick < millis()) {
    nextButtonClick  = millis() + 350;
    Serial.println("Select mode");
    selectMode();
  }

  int nextSignal = digitalRead(NEXT_PIN);
  if (nextSignal == HIGH && !wasStarted && nextButtonClick < millis()) {
    nextButtonClick  = millis() + 350;
    nextRound();
    drawInfo();
    Serial.println("Select next");
  }

  if (wasStarted) {
    if (wasChanged) {
      draw();
    }    
    if (countDown == 0) {
      if (isPrepare) {
        isPrepare = false;
        countDown = sDuration[currMode];
        numberColor = GREEN_COLOR;          
        draw();
        Serial.println("Beep here");
        beep(1);
      } else {
        stop();
      }
    }
  }
  checkBeep();
}
