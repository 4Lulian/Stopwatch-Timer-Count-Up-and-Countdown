#include <TM1637Display.h>


const int BUTTON_BLUE  = 2;
const int BUTTON_GREEN = 6;
const int BUTTON_RED   = 3;


const int CLK = 9;
const int DIO = 8;


TM1637Display display(CLK, DIO);


bool timerMode = false;
bool running = false;


unsigned long lastSecond = 0;


int currentTime = 0;
int setTime = 0;


bool lastBlue = HIGH;
bool lastGreen = HIGH;
bool lastRed = HIGH;


void setup() {


  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);


  Serial.begin(9600);


  display.setBrightness(7);


  showTime(0);


  Serial.println("System Ready");
  Serial.println("Stopwatch Mode");
}


void loop() {


  bool blue = digitalRead(BUTTON_BLUE);
  bool green = digitalRead(BUTTON_GREEN);
  bool red = digitalRead(BUTTON_RED);


  if (blue == LOW && lastBlue == HIGH) {


    if (!running) {


      timerMode = !timerMode;


      currentTime = 0;
      setTime = 0;


      showTime(0);


      if (timerMode) {
        Serial.println("Timer Mode");
      }
      else {
        Serial.println("Stopwatch Mode");
      }
    }


    delay(150);
  }


  if (green == LOW && lastGreen == HIGH) {


    if (timerMode && !running) {


      setTime = setTime + 5;
      currentTime = setTime;


      showTime(currentTime);


      Serial.print("Timer: ");
      Serial.println(currentTime);
    }


    delay(150);
  }


  if (red == LOW && lastRed == HIGH) {


    if (!running && currentTime == 0) {


      running = true;
      lastSecond = millis();


      Serial.println("START");
    }


    else if (!running && timerMode && setTime > 0) {


      running = true;
      currentTime = setTime;
      lastSecond = millis();


      Serial.println("START");
    }


    else if (running) {


      running = false;


      Serial.println("STOP");
    }


    else if (!running) {


      currentTime = 0;
      setTime = 0;


      showTime(0);


      Serial.println("RESET");
    }


    delay(150);
  }


  if (running && !timerMode) {
    if (millis() - lastSecond >= 1000) {


      lastSecond = lastSecond + 1000;


      currentTime++;


      showTime(currentTime);


      Serial.print("Stopwatch: ");
      Serial.println(currentTime);
    }
  }


  if (running && timerMode) {


    if (millis() - lastSecond >= 1000) {


      lastSecond = lastSecond + 1000;


      currentTime--;


      showTime(currentTime);


      Serial.print("Timer: ");
      Serial.println(currentTime);


      if (currentTime <= 0) {


        currentTime = 0;
        running = false;


        showTime(0);


        Serial.println("TIME FINISHED");
      }
    }
  }


  lastBlue = blue;
  lastGreen = green;
  lastRed = red;
}
void showTime(int seconds) {
  int minutes = seconds / 60;
  int secs = seconds % 60;


  int number = minutes * 100 + secs;


  display.showNumberDecEx(
    number,
    0b01000000,
    true
  );
}
